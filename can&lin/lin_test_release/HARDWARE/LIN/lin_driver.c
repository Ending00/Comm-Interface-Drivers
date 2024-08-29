#include "stdio.h"
#include "usart.h"
#include "lin.h"
#include "lin_driver.h"
LIN_RxState       LIN_RxStateGet = BREAK_GET; 
LIN_ERROR_Code    ErrCode = FIFO_VOID;
u8                data[8] = {0x11,0x01,0x34,0x34,0x56,0x34,0x78,0x67};
 
//LIN���ջ�����
LIN_BUFFER  LIN_RxDataBuff;
 
/**
  * @brief  LIN�ӻ��жϺ���
  * @param  None
  * @retval None
	*/
void LIN_MASTER_IRQHandler(void)
{
	uint8_t ReceiveData = 0;
	
	//LIN�Ͽ�֡�ж�
	if ((USART_GetITStatus(USART2,USART_IT_LBD) == SET)){		//LIN��·����ж�  1 -- ��ͬ�������	
		LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);              //��յ�ǰ������
		USART2->SR;     //��ȡUSART1->SR�ᵼ�½��ջ������е����ݱ��Ƴ������
		USART2->DR;	
		LIN_RxStateGet = SYNCH_GET;
		USART_ClearITPendingBit(USART2,USART_IT_LBD);         //���LIN��·����ж�
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		return;		
	}
	
	if (USART_GetITStatus(USART2,USART_IT_RXNE) == SET){    			  //LIN�����ж�			
		ReceiveData = USART_ReceiveData(USART2);	                  	//����USARTx����������յ������ݡ�
		if (USART_GetFlagStatus(USART2,USART_FLAG_FE) == RESET){ 		  //֡�����־  0 -- û�м�⵽֡����	
			if ((ReceiveData==0x55)&&(LIN_RxStateGet==BREAK_GET)){ 		  //������ͬ������źŵ�LIN����                                    //��ʼ����ʱ��ֵ ����ι��
	    	LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);            	 		//��յ�ǰ������
			USART2->SR;     
			USART2->DR;
			LIN_RxStateGet = SYNCH_GET;
			return;   
			}
			LIN_MasterRxMsg(ReceiveData);   //��Ϣ����
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	
	}
}

/**
  * @brief  LIN�ӻ����մ�����
	* @param  Data: ������Ϣ
  * @retval None
  */
void LIN_MasterRxMsg(uint8_t Data)
{
	uint8_t PID ,Checksum;
	LIN_MSG* pLINMsg = LIN_RingBUF_GetMsg(&LIN_RxDataBuff); 
	switch (LIN_RxStateGet){
		case BREAK_GET:      //ͬ�������
		{
		}
		break;
		
		case SYNCH_GET:      //ͬ����
		{
			if(Data != 0x55){  //�ж��ǲ���ͬ����
				ErrCode = SYNC_ERR;
				LIN_RxStateGet = BREAK_GET;	
			}else{
				pLINMsg->Sync = Data;
				LIN_RxStateGet = PID_GET;	
			}	
		}
		break;	
		
		case PID_GET:        //PID��
		{
			pLINMsg->FrameID = Data&0x3F;
			pLINMsg->PID = Data;
			PID = LIN_GetPID(pLINMsg->FrameID);  //����ID��ȡ��żУ��λ �õ�У���PID
			
			if(PID == pLINMsg->PID){                     //�ж�PID�Ƿ���ȷ  ��������LDF��
				//�����ж���ִ�л��Ƿ��� �ı��־λ
				if(pLINMsg->FrameID == 0x31){              // 1 -- ִ��  ��������������
					LIN_RxStateGet = MSG_GET;	
				}else if(pLINMsg->FrameID == 0x32){        // 2 -- ����  ����LIN���߷�����Ϣ
					LIN_Send_data(pLINMsg->PID,data,sizeof(data)/sizeof(data[0]));   //������Ϣ
					LIN_RxStateGet = BREAK_GET;
				}else{                                     // 3 -- ����  ����ִ��Ҳ������                       
					LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff); //��յ�ǰ������
					LIN_RxStateGet = BREAK_GET;
				}
			}else{   //PIDУ�鲻��ȷ
				ErrCode = PID_ERR;
				LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);   //��յ�ǰ������
				LIN_RxStateGet = BREAK_GET;				
			}												
		}
		break;	
		
		case MSG_GET:       //���ݶ�
		{
			pLINMsg->Data[pLINMsg->DataLen++] = Data;
			pLINMsg->Checksum = Data;
			LIN_RxStateGet = (pLINMsg->DataLen>=8)?CHECKSUM_GET:MSG_GET;	
		}
		break;	
		
		case CHECKSUM_GET:  //У��Ͷ�
		{
			pLINMsg->Checksum = Data;
			Checksum = LIN_GetChecksum(pLINMsg->PID,pLINMsg->Data,pLINMsg->DataLen,1);     //��ȡУ��Ͷ�
			if((Checksum+pLINMsg->Checksum) == 0xFF){            //�ж�У����Ƿ���ȷ
				LIN_RingBUF_Write(&LIN_RxDataBuff);
			}else{
				ErrCode = FORMAT_ERR;
				LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);           //��յ�ǰ������
			} 
			LIN_RxStateGet = BREAK_GET;		
		}
		break;
		
		default:       			//����
			LIN_RxStateGet = BREAK_GET;	
		break;
	}
	
}
