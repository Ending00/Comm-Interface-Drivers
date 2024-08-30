#include "lin.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include <stdlib.h>  // 需要包含 <stdlib.h> 以使用 malloc
void LIN_Mode_Init(u32 bound)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_LINCmd(USART2,ENABLE);
	USART_LINBreakDetectLengthConfig(USART2,USART_LINBreakDetectLength_10b);
	USART_Cmd(USART2, ENABLE);	
  //Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(USART2, USART_IT_LBD,ENABLE);        //打开LIN断开符检测中断
}	

/*主机帧头部分*/
void Lin_SendBreak(void)
{
    USART_SendBreak(USART2);
}

/*同步间隔段*/
void Lin_SendSyncSegment(void)
{
    USART_SendData(USART2,0x55);
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET );
}

/*发送PID*/
void Lin_SendHead(u8 id)
{
    Lin_SendBreak();
    Lin_SendSyncSegment();
    USART_SendData(USART2,LIN_GetPID(id));
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET );
}

/**
  * @brief LIN_PID校验函数		
  * @param ID(FrameID)：帧ID(0 ~ 63) 
  * P0(bit6) =   ID0 ^ ID1 ^ ID2 ^ ID4   <==>  (偶校验：操作数中1的个数为偶数，校验位为0，1的个数为奇数校验位为1)
  * P1(bit7) = ~(ID1 ^ ID3 ^ ID4 ^ ID5)  <==>  (奇校验：操作数中1的个数为奇数，校验位为0，1的个数为偶数校验位为1)
  * @retval 返回PID
  */
uint8_t LIN_GetPID(uint8_t ID)  
{
	uint8_t PID = 0,P0 = 0,P1 = 0;	
	P0 = (((ID>>0)^(ID>>1)^(ID>>2)^(ID>>4))&0x01)<<6; //偶校验位           			
	P1 = ((~((ID>>1)^(ID>>3)^(ID>>4)^(ID>>5)))&0x01)<<7; //奇校验位	
	PID = (ID|P0|P1);	
	return PID;   
}

/**
  * @brief  LIN协议规定校验和长度为1个字节，获取校验和	
	* @param  PID：校验ID，pData：数据指针，DataLen：数据长度，flag ：发送0 接收1 
  * @retval 累加校验和
  */
uint8_t LIN_GetChecksum(uint8_t PID, uint8_t* pData,uint8_t DataLen,uint8_t flag) 			 
{ 
    uint16_t CheckSum = 0;  	
    uint8_t i = 0;	

    for(i = 0; i < DataLen; i++)
    {
        CheckSum += pData[i];
        CheckSum = (CheckSum & 0xFF) + (CheckSum >> 8);  // 进位调整
    }
    if ((PID!=0x3C) && (PID!=0x7D)) 
    {
        CheckSum += LIN_GetPID(PID);
        CheckSum = (CheckSum & 0xFF) + (CheckSum >> 8);  // 进位调整
    }
    if(flag == 0)
    {
        return (~CheckSum) & 0xFF;  // 发送方需要取反
    }
    else
    {
        return CheckSum & 0xFF; 	 // 接收方不需要
    }
}


//uint8_t LIN_GetChecksum(uint8_t PID, uint8_t* pData,uint8_t DataLen,uint8_t flag) 			 
//{  
//	  uint16_t CheckSum = 0; 
//      uint8_t i = 0;	
//    //FrameID为3C 3D的PID为3C 7D
//	  if((PID!=0x3C)&&(PID!=0x7D)){    //诊断帧只能使用标准校验和，标准校验和不包含PID 只校验数据段              	  
//			CheckSum = PID;     		
//	  }
//	  for(i = 0; i < DataLen; i++){
//			CheckSum += pData[i];		  
//			if (CheckSum > 0xFF){
//				CheckSum -= 0xFF;  
//			}
//	  }
//		
//		if(flag == 0){
//			return (~CheckSum) & 0xFF;  //发送方需要取反
//		}else{
//			return CheckSum & 0xFF; 		//接收方不需要
//		}
//}


/**
  * @brief  LIN发送字节	
  * @param  USARTx：串口号，pData：数据指针，Length：数据长度
  * @retval 无
  */
void LIN_SendBytes(USART_TypeDef* USARTx,uint8_t* pData,uint8_t DataLen)
{
	uint8_t i = 0;
	for (i = 0; i < DataLen; i++){
		USART_SendData(USARTx,*pData++);
		while (USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);	//传输数据寄存器空标志
	}
	while (USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);		//传输完成标志
}

/*******************************************************
* 数据发送函数
*******************************************************/
/**
  * @brief  反馈消息给总线	
  * @param  PID：校验ID，pData：数据指针，Length：数据长度
  * @retval 无
  */
void LIN_Send_data(uint8_t PID, uint8_t* pData, uint8_t DataLen)
{
    uint8_t i = 0 , Checksum;
    uint8_t* Linbuffer = (uint8_t*)malloc(DataLen + 1);  // 动态分配数组(数据+校验和)
    if (Linbuffer == NULL) {
        // 处理内存分配失败的情况
        return;
    }
    Lin_SendHead(PID);
    Checksum = LIN_GetChecksum(PID, pData, DataLen, 0);  // 获取校验和段
    for (i = 0; i < DataLen; i++) {
        Linbuffer[i] = *(pData + i);
    }
    Linbuffer[DataLen] = Checksum;  // 校验和
    LIN_SendBytes(USART2, Linbuffer, DataLen + 1);  // 发送从机数据

    free(Linbuffer);  // 使用完后释放内存
}

/******************************主机读，从机写*****************************
*	LIN从机处理
*
*   主机发送：| Break | Sync Field | Identifier Field |
*	从机响应：|               Data Field                | Checksum Field |
**************************************************************************/
void LIN_Rx_data(uint8_t PID, uint8_t* pData,uint8_t DataLen)
{
	uint8_t i = 0 , Checksum = 0;	
    uint8_t* Linbuffer = (uint8_t*)malloc(DataLen + 1);  // 动态分配数组(数据+校验和)
    if (Linbuffer == NULL) {
        // 处理内存分配失败的情况
        return;
    }
	Checksum = LIN_GetChecksum(PID,pData,DataLen,0);     //获取校验和段
	for (i = 0; i < DataLen; i++)                        //存DataLen个字节数据段
	{     
		Linbuffer[i] = *(pData + i);		
	}
	Linbuffer[DataLen] = Checksum;                   //校验和
	LIN_SendBytes(USART2, Linbuffer ,DataLen+1);     //发送从机数据
	free(Linbuffer);  // 使用完后释放内存
}



//缓冲区初始化
void LIN_RingBUF_Init(LIN_BUFFER* pLINBuff)
{
	pLINBuff->Head = 0;
	pLINBuff->Tail = 0;
	pLINBuff->Length = 0;
	pLINBuff->BufOutcnt = 0;
}

//获取LIN消息
LIN_MSG* LIN_RingBUF_GetMsg(LIN_BUFFER* pLINBuff)
{
	return &pLINBuff->LINBuffer[pLINBuff->Tail];
}

//写缓冲区
void LIN_RingBUF_Write(LIN_BUFFER* pLINBuff)
{
	pLINBuff->Tail = (pLINBuff->Tail + 1) % LIN_RINGBUF_LEN; //防止越界非法访问
	if (pLINBuff->Length < LIN_RINGBUF_LEN) {                //判断缓冲区是否已满
		pLINBuff->Length++;
	}else{
		pLINBuff->BufOutcnt++;
		if (pLINBuff->BufOutcnt == LIN_RINGBUF_LEN) {
			pLINBuff->BufOutcnt = 0;			
		}
		pLINBuff->Head = pLINBuff->BufOutcnt;
	}
}

//清空当前缓冲区
void LIN_RingBUF_ClearRxMsg(LIN_BUFFER* pLINBuf)
{
	LIN_MSG* pMsg = LIN_RingBUF_GetMsg(pLINBuf);
	pMsg->Sync = 0;
	pMsg->PID = 0;
	pMsg->DataLen = 0;
	pMsg->Checksum = 0;
}

//获取缓冲区消息长度
uint16_t LIN_RingBUF_ReadLen(LIN_BUFFER* pLINBuff)
{
	return pLINBuff->Length <= 0 ? 0 : pLINBuff->Length;
}

//读缓冲区
int LIN_RingBUF_Read(LIN_BUFFER* pLINBuff, LIN_MSG** pLINMsg)
{
	 __set_PRIMASK(1);
	if (pLINBuff->Length <= 0) {  //判断缓冲区是否为空	
		pLINBuff->BufOutcnt = 0;
		 __set_PRIMASK(0);
		return 1;
	}else {
		*pLINMsg = &pLINBuff->LINBuffer[pLINBuff->Head];   	      //先进先出FIFO，从缓冲区头出
		pLINBuff->Head = (pLINBuff->Head + 1) % LIN_RINGBUF_LEN;  //防止越界非法访问
		pLINBuff->Length--;
		 __set_PRIMASK(0);
		return 0;
	}
}

