#include "Device/Include/stm32f10x.h"   // Device header


void MyCAN_Init(void)
{
	/****开启GPIOA、CAN模块时钟****/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	/****GPIOA初始化****/
	GPIO_InitTypeDef GPIO_Initstructure ;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_12;                  //TXD
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
	
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_11;                  //RXD 
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
	
	/***CAN初始化****/
	CAN_InitTypeDef CAN_InitStruct;
	CAN_InitStruct.CAN_ABOM = DISABLE;
	CAN_InitStruct.CAN_AWUM = DISABLE;
	CAN_InitStruct.CAN_TTCM = DISABLE;
	CAN_InitStruct.CAN_TXFP = DISABLE;
	CAN_InitStruct.CAN_NART = DISABLE;
	CAN_InitStruct.CAN_RFLM = DISABLE;
	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStruct.CAN_BS1 = CAN_BS1_2tq; //通过公式波特率=APB1时钟频率/分频系数/一位的Tq数量，高速CAN总线波特率为125K-1Mbps
	CAN_InitStruct.CAN_BS2 = CAN_BS2_3tq; //可以计算出波特率为36M/48/(1+2+3)=125Kbps,其中BS1、BS2和Prescaler的值可以自己设置，只需计算结果在125K-1M之间即可
	CAN_InitStruct.CAN_Prescaler = 48;      
//	CAN_InitStruct.CAN_Mode = CAN_Mode_LoopBack;             //回环模式
//	CAN_InitStruct.CAN_Mode = CAN_Mode_Silent;               //静默模式
    CAN_InitStruct.CAN_Mode = CAN_Mode_Silent_LoopBack;      //环回静默模式		
	CAN_Init(CAN1, &CAN_InitStruct);
	
	/***CAN滤波初始化****/
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStruct.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterNumber = 0;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
}



void MyCAN_TransmitMessage(uint32_t ID, uint8_t Length, uint8_t* Data)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = ID;			                       // 标准标识符 
	TxMessage.ExtId = ID;			                       // 设置扩展标示符 
	TxMessage.IDE = CAN_Id_Standard; 	                   // 标准帧
	TxMessage.RTR = CAN_RTR_Data;		                   // 数据帧
	TxMessage.DLC = Length;				                   // 要发送的数据长度
	for(uint8_t i = 0; i < Length; i++)
	{
	    TxMessage.Data[i] = Data[i];			
	}
          
	uint8_t TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);   
	uint32_t time_out = 100000;
	while(CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok)   //等待TransMailbox发送完成，timeout是为了
	{                                               //防止等待超时导致程序卡死，当timeout == 0时，while()循环自动退出
		time_out--;
	    if(time_out == 0)
		{
			break;
		}
	}
                                                    
}

uint8_t MyCAN_ReceiveFlag(void)
{
	if(CAN_MessagePending(CAN1, CAN_FIFO0) > 0)
	{
		return 1;
	}
		return 0;
		
}

void MyCAN_ReceiveMessage(uint32_t* ID, uint8_t* Length, uint8_t* Data)
{
	CanRxMsg RxMessage;
//	RxMessage.Data[8] = 0;
//	RxMessage.DLC = 8;
//	RxMessage.ExtId = *ID;
//	RxMessage.StdId = *ID;
//	RxMessage.IDE = CAN_Id_Standard;
//	RxMessage.RTR = CAN_RTR_Data;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	if(RxMessage.IDE == CAN_Id_Standard)
	{
//		*ID = CAN_Id_Standard;
		*ID = RxMessage.StdId;
	}
	else
	{
//		*ID = CAN_Id_Extended;	
		*ID = RxMessage.ExtId;
	}
	if(RxMessage.RTR == CAN_RTR_Data)
	{
		*Length = RxMessage.DLC; 
//		for(uint8_t i; i < *Length; i++)    //如果i不初始化为0的话，接收报文将会失败，显示为默认0      
		for(uint8_t i=0; i< *Length; i++)
	    {
	    	Data[i] = RxMessage.Data[i];
	    }
	}
	else
	{
		//
	}
	

}
