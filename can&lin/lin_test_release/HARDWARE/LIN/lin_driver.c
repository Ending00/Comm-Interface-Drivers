#include "stdio.h"
#include "usart.h"
#include "lin.h"
#include "lin_driver.h"
LIN_RxState       LIN_RxStateGet = BREAK_GET; 
LIN_ERROR_Code    ErrCode = FIFO_VOID;
u8                data[8] = {0x11,0x01,0x34,0x34,0x56,0x34,0x78,0x67};
 
//LIN接收缓冲区
LIN_BUFFER  LIN_RxDataBuff;
 
/**
  * @brief  LIN从机中断函数
  * @param  None
  * @retval None
	*/
void LIN_MASTER_IRQHandler(void)
{
	uint8_t ReceiveData = 0;
	
	//LIN断开帧中断
	if ((USART_GetITStatus(USART2,USART_IT_LBD) == SET)){		//LIN断路检测中断  1 -- 有同步间隔段	
		LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);              //清空当前缓冲区
		USART2->SR;     //读取USART1->SR会导致接收缓冲区中的数据被移除或清空
		USART2->DR;	
		LIN_RxStateGet = SYNCH_GET;
		USART_ClearITPendingBit(USART2,USART_IT_LBD);         //清除LIN断路检测中断
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		return;		
	}
	
	if (USART_GetITStatus(USART2,USART_IT_RXNE) == SET){    			  //LIN接收中断			
		ReceiveData = USART_ReceiveData(USART2);	                  	//返回USARTx外设最近接收到的数据。
		if (USART_GetFlagStatus(USART2,USART_FLAG_FE) == RESET){ 		  //帧错误标志  0 -- 没有检测到帧错误	
			if ((ReceiveData==0x55)&&(LIN_RxStateGet==BREAK_GET)){ 		  //处理无同步间隔信号的LIN数据                                    //初始化定时器值 类似喂狗
	    	LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);            	 		//清空当前缓冲区
			USART2->SR;     
			USART2->DR;
			LIN_RxStateGet = SYNCH_GET;
			return;   
			}
			LIN_MasterRxMsg(ReceiveData);   //消息处理
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	
	}
}

/**
  * @brief  LIN从机接收处理函数
	* @param  Data: 串口消息
  * @retval None
  */
void LIN_MasterRxMsg(uint8_t Data)
{
	uint8_t PID ,Checksum;
	LIN_MSG* pLINMsg = LIN_RingBUF_GetMsg(&LIN_RxDataBuff); 
	switch (LIN_RxStateGet){
		case BREAK_GET:      //同步间隔段
		{
		}
		break;
		
		case SYNCH_GET:      //同步段
		{
			if(Data != 0x55){  //判断是不是同步段
				ErrCode = SYNC_ERR;
				LIN_RxStateGet = BREAK_GET;	
			}else{
				pLINMsg->Sync = Data;
				LIN_RxStateGet = PID_GET;	
			}	
		}
		break;	
		
		case PID_GET:        //PID段
		{
			pLINMsg->FrameID = Data&0x3F;
			pLINMsg->PID = Data;
			PID = LIN_GetPID(pLINMsg->FrameID);  //根据ID获取奇偶校验位 得到校验的PID
			
			if(PID == pLINMsg->PID){                     //判断PID是否正确  后续根据LDF定
				//根据判断是执行还是反馈 改变标志位
				if(pLINMsg->FrameID == 0x31){              // 1 -- 执行  即继续接收数据
					LIN_RxStateGet = MSG_GET;	
				}else if(pLINMsg->FrameID == 0x32){        // 2 -- 反馈  即向LIN总线发送消息
					LIN_Send_data(pLINMsg->PID,data,sizeof(data)/sizeof(data[0]));   //反馈消息
					LIN_RxStateGet = BREAK_GET;
				}else{                                     // 3 -- 其他  即不执行也不反馈                       
					LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff); //清空当前缓冲区
					LIN_RxStateGet = BREAK_GET;
				}
			}else{   //PID校验不正确
				ErrCode = PID_ERR;
				LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);   //清空当前缓冲区
				LIN_RxStateGet = BREAK_GET;				
			}												
		}
		break;	
		
		case MSG_GET:       //数据段
		{
			pLINMsg->Data[pLINMsg->DataLen++] = Data;
			pLINMsg->Checksum = Data;
			LIN_RxStateGet = (pLINMsg->DataLen>=8)?CHECKSUM_GET:MSG_GET;	
		}
		break;	
		
		case CHECKSUM_GET:  //校验和段
		{
			pLINMsg->Checksum = Data;
			Checksum = LIN_GetChecksum(pLINMsg->PID,pLINMsg->Data,pLINMsg->DataLen,1);     //获取校验和段
			if((Checksum+pLINMsg->Checksum) == 0xFF){            //判断校验和是否正确
				LIN_RingBUF_Write(&LIN_RxDataBuff);
			}else{
				ErrCode = FORMAT_ERR;
				LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);           //清空当前缓冲区
			} 
			LIN_RxStateGet = BREAK_GET;		
		}
		break;
		
		default:       			//其他
			LIN_RxStateGet = BREAK_GET;	
		break;
	}
	
}
