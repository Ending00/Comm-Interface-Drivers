#include "lin.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include <stdlib.h>  // ��Ҫ���� <stdlib.h> ��ʹ�� malloc
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
  //Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART2, USART_IT_LBD,ENABLE);        //��LIN�Ͽ�������ж�
}	

/*����֡ͷ����*/
void Lin_SendBreak(void)
{
    USART_SendBreak(USART2);
}

/*ͬ�������*/
void Lin_SendSyncSegment(void)
{
    USART_SendData(USART2,0x55);
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET );
}

/*����PID*/
void Lin_SendHead(u8 id)
{
    Lin_SendBreak();
    Lin_SendSyncSegment();
    USART_SendData(USART2,LIN_GetPID(id));
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET );
}

/**
  * @brief LIN_PIDУ�麯��		
  * @param ID(FrameID)��֡ID(0 ~ 63) 
  * P0(bit6) =   ID0 ^ ID1 ^ ID2 ^ ID4   <==>  (żУ�飺��������1�ĸ���Ϊż����У��λΪ0��1�ĸ���Ϊ����У��λΪ1)
  * P1(bit7) = ~(ID1 ^ ID3 ^ ID4 ^ ID5)  <==>  (��У�飺��������1�ĸ���Ϊ������У��λΪ0��1�ĸ���Ϊż��У��λΪ1)
  * @retval ����PID
  */
uint8_t LIN_GetPID(uint8_t ID)  
{
	uint8_t PID = 0,P0 = 0,P1 = 0;	
	P0 = (((ID>>0)^(ID>>1)^(ID>>2)^(ID>>4))&0x01)<<6; //żУ��λ           			
	P1 = ((~((ID>>1)^(ID>>3)^(ID>>4)^(ID>>5)))&0x01)<<7; //��У��λ	
	PID = (ID|P0|P1);	
	return PID;   
}

/**
  * @brief  LINЭ��涨У��ͳ���Ϊ1���ֽڣ���ȡУ���	
	* @param  PID��У��ID��pData������ָ�룬DataLen�����ݳ��ȣ�flag ������0 ����1 
  * @retval �ۼ�У���
  */
uint8_t LIN_GetChecksum(uint8_t PID, uint8_t* pData,uint8_t DataLen,uint8_t flag) 			 
{ 
    uint16_t CheckSum = 0;  	
    uint8_t i = 0;	

    for(i = 0; i < DataLen; i++)
    {
        CheckSum += pData[i];
        CheckSum = (CheckSum & 0xFF) + (CheckSum >> 8);  // ��λ����
    }
    if ((PID!=0x3C) && (PID!=0x7D)) 
    {
        CheckSum += LIN_GetPID(PID);
        CheckSum = (CheckSum & 0xFF) + (CheckSum >> 8);  // ��λ����
    }
    if(flag == 0)
    {
        return (~CheckSum) & 0xFF;  // ���ͷ���Ҫȡ��
    }
    else
    {
        return CheckSum & 0xFF; 	 // ���շ�����Ҫ
    }
}


//uint8_t LIN_GetChecksum(uint8_t PID, uint8_t* pData,uint8_t DataLen,uint8_t flag) 			 
//{  
//	  uint16_t CheckSum = 0; 
//      uint8_t i = 0;	
//    //FrameIDΪ3C 3D��PIDΪ3C 7D
//	  if((PID!=0x3C)&&(PID!=0x7D)){    //���ֻ֡��ʹ�ñ�׼У��ͣ���׼У��Ͳ�����PID ֻУ�����ݶ�              	  
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
//			return (~CheckSum) & 0xFF;  //���ͷ���Ҫȡ��
//		}else{
//			return CheckSum & 0xFF; 		//���շ�����Ҫ
//		}
//}


/**
  * @brief  LIN�����ֽ�	
  * @param  USARTx�����ںţ�pData������ָ�룬Length�����ݳ���
  * @retval ��
  */
void LIN_SendBytes(USART_TypeDef* USARTx,uint8_t* pData,uint8_t DataLen)
{
	uint8_t i = 0;
	for (i = 0; i < DataLen; i++){
		USART_SendData(USARTx,*pData++);
		while (USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);	//�������ݼĴ����ձ�־
	}
	while (USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);		//������ɱ�־
}

/*******************************************************
* ���ݷ��ͺ���
*******************************************************/
/**
  * @brief  ������Ϣ������	
  * @param  PID��У��ID��pData������ָ�룬Length�����ݳ���
  * @retval ��
  */
void LIN_Send_data(uint8_t PID, uint8_t* pData, uint8_t DataLen)
{
    uint8_t i = 0 , Checksum;
    uint8_t* Linbuffer = (uint8_t*)malloc(DataLen + 1);  // ��̬��������(����+У���)
    if (Linbuffer == NULL) {
        // �����ڴ����ʧ�ܵ����
        return;
    }
    Lin_SendHead(PID);
    Checksum = LIN_GetChecksum(PID, pData, DataLen, 0);  // ��ȡУ��Ͷ�
    for (i = 0; i < DataLen; i++) {
        Linbuffer[i] = *(pData + i);
    }
    Linbuffer[DataLen] = Checksum;  // У���
    LIN_SendBytes(USART2, Linbuffer, DataLen + 1);  // ���ʹӻ�����

    free(Linbuffer);  // ʹ������ͷ��ڴ�
}

/******************************���������ӻ�д*****************************
*	LIN�ӻ�����
*
*   �������ͣ�| Break | Sync Field | Identifier Field |
*	�ӻ���Ӧ��|               Data Field                | Checksum Field |
**************************************************************************/
void LIN_Rx_data(uint8_t PID, uint8_t* pData,uint8_t DataLen)
{
	uint8_t i = 0 , Checksum = 0;	
    uint8_t* Linbuffer = (uint8_t*)malloc(DataLen + 1);  // ��̬��������(����+У���)
    if (Linbuffer == NULL) {
        // �����ڴ����ʧ�ܵ����
        return;
    }
	Checksum = LIN_GetChecksum(PID,pData,DataLen,0);     //��ȡУ��Ͷ�
	for (i = 0; i < DataLen; i++)                        //��DataLen���ֽ����ݶ�
	{     
		Linbuffer[i] = *(pData + i);		
	}
	Linbuffer[DataLen] = Checksum;                   //У���
	LIN_SendBytes(USART2, Linbuffer ,DataLen+1);     //���ʹӻ�����
	free(Linbuffer);  // ʹ������ͷ��ڴ�
}



//��������ʼ��
void LIN_RingBUF_Init(LIN_BUFFER* pLINBuff)
{
	pLINBuff->Head = 0;
	pLINBuff->Tail = 0;
	pLINBuff->Length = 0;
	pLINBuff->BufOutcnt = 0;
}

//��ȡLIN��Ϣ
LIN_MSG* LIN_RingBUF_GetMsg(LIN_BUFFER* pLINBuff)
{
	return &pLINBuff->LINBuffer[pLINBuff->Tail];
}

//д������
void LIN_RingBUF_Write(LIN_BUFFER* pLINBuff)
{
	pLINBuff->Tail = (pLINBuff->Tail + 1) % LIN_RINGBUF_LEN; //��ֹԽ��Ƿ�����
	if (pLINBuff->Length < LIN_RINGBUF_LEN) {                //�жϻ������Ƿ�����
		pLINBuff->Length++;
	}else{
		pLINBuff->BufOutcnt++;
		if (pLINBuff->BufOutcnt == LIN_RINGBUF_LEN) {
			pLINBuff->BufOutcnt = 0;			
		}
		pLINBuff->Head = pLINBuff->BufOutcnt;
	}
}

//��յ�ǰ������
void LIN_RingBUF_ClearRxMsg(LIN_BUFFER* pLINBuf)
{
	LIN_MSG* pMsg = LIN_RingBUF_GetMsg(pLINBuf);
	pMsg->Sync = 0;
	pMsg->PID = 0;
	pMsg->DataLen = 0;
	pMsg->Checksum = 0;
}

//��ȡ��������Ϣ����
uint16_t LIN_RingBUF_ReadLen(LIN_BUFFER* pLINBuff)
{
	return pLINBuff->Length <= 0 ? 0 : pLINBuff->Length;
}

//��������
int LIN_RingBUF_Read(LIN_BUFFER* pLINBuff, LIN_MSG** pLINMsg)
{
	 __set_PRIMASK(1);
	if (pLINBuff->Length <= 0) {  //�жϻ������Ƿ�Ϊ��	
		pLINBuff->BufOutcnt = 0;
		 __set_PRIMASK(0);
		return 1;
	}else {
		*pLINMsg = &pLINBuff->LINBuffer[pLINBuff->Head];   	      //�Ƚ��ȳ�FIFO���ӻ�����ͷ��
		pLINBuff->Head = (pLINBuff->Head + 1) % LIN_RINGBUF_LEN;  //��ֹԽ��Ƿ�����
		pLINBuff->Length--;
		 __set_PRIMASK(0);
		return 0;
	}
}

