#include "stm32f10x.h"                  // Device header

void MyCAN_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_Prescaler = 48;
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x345<<5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x234<<5;            
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x000<<5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x567<<5;
                                                            //经过测试，0x345、0x234、0x000、0x567的ID可以在CAN_FilterIdHigh、CAN_FilterIdLow
//	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x234<<5;    //、CAN_FilterMaskIdHigh和CAN_FilterMaskIdHigh中的位置随机填写，不影响测试效果
//	CAN_FilterInitStructure.CAN_FilterIdLow = 0x345<<5;   
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x567<<5;
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x000<<5;	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
}

void MyCAN_Transmit(CanTxMsg* TxMessage)
{
//	CanTxMsg TxMessage;
//	TxMessage.StdId = ID;
//	TxMessage.ExtId = ID;
//	TxMessage.IDE = CAN_Id_Standard;          //标准ID
//	TxMessage.RTR = CAN_RTR_Data;             //数据帧
//	TxMessage.DLC = Length;                   //数据长度
//	for (uint8_t i = 0; i < Length; i ++)
//	{
//		TxMessage.Data[i] = Data[i];          //将数组Data[i]赋值给发送报文TxMessage.Data[i]
//	}
//	uint8_t TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);     //采用CAN_Transmit()函数将发送报文TxMessage赋值
//	                                                              // 给发送TransmitMailbox
	uint32_t Timeout = 0;
	
	uint8_t TransmitMailbox = CAN_Transmit(CAN1, TxMessage);     //采用CAN_Transmit()函数将发送报文TxMessage赋值	
	
	while (CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok)
	{
		Timeout ++;
		if(Timeout > 100000)
		{
			break;
		
		}
	}
}                                             //采用while()循环，等待CAN_TransmitStatus()函数传输状态完成

uint8_t MyCAN_ReceiveFlag(void)
{
	if (CAN_MessagePending(CAN1, CAN_FIFO0) > 0)
	{
		return 1;
	}
	return 0;
}

void MyCAN_Receive(CanRxMsg* RxMessage)
{
//	CanRxMsg RxMessage;                      //定义接收报文
	
	CAN_Receive(CAN1, CAN_FIFO0, RxMessage);
	
//	if (RxMessage.IDE == CAN_Id_Standard)    //标识符：标准ID
//	{
//		*ID = RxMessage.StdId;
//	}
//	else if(RxMessage.IDE == CAN_Id_Extended)
//	{
//		*ID = RxMessage.ExtId;
//	}
//	
//	if (RxMessage.RTR == CAN_RTR_Data)       //远程发送请求：数据帧
//	{
//		*Length = RxMessage.DLC;
//		for (uint8_t i = 0; i < *Length; i ++)
//		{
//			Data[i] = RxMessage.Data[i];           //将接收报文的数据RxMessage.Data[i]赋值给数组Data[i]
//		}
//	}
//	else
//	{
//		//...
//	}
}
