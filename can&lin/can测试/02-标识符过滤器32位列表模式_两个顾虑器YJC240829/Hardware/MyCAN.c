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
	uint32_t ID1 = 0x123 << 21;
	CAN_FilterInitStructure.CAN_FilterIdHigh = ID1>>16;          //��32λID1���Ϊ��16λID�͵�16λID����ID1����16λ��ֵ����16λID,��CAN_FilterIdHigh   
//	CAN_FilterInitStructure.CAN_FilterIdLow = ID1;               //��ID1��ֵ����16λ���Ⱥ�����ǵ�16λ���Ⱥ��ұ���32λ���Զ�������16λ��Ȼ��ֵ����16λID,��CAN_FilterIdLow
	CAN_FilterInitStructure.CAN_FilterIdLow = ID1 & (0xFFFF);    //Ҳ�ɽ�ID1&0xFFFF��ֱ�Ӹ�ֵ��CAN_FilterIdLow������һ��ȼ�

//	uint32_t ID2 = (0x12345678u <<3) | 0x4;                      //0x12345678����u��Ŀ���Ƿ�ֹ�������ݲ���ʱ���������Χ
	uint32_t ID2 = ((uint32_t)0x12345678 <<3) | 0x4;             //���߽�0x12345678ǿ��ת��Ϊuint32_t���ͼ���
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = ID2>>16;      //��32λID2���Ϊ��16λID�͵�16λID����ID2����16λ��ֵ����16λID,��CAN_FilterMaskIdHigh   
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = ID2;           //��ID2��ֵ����16λ���Ⱥ�����ǵ�16λ���Ⱥ��ұ���32λ���Զ�������16λ��Ȼ��ֵ����16λID,��CAN_FilterMaskIdLow
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = ID2 &(0xFFFF); //Ҳ�ɽ�ID2&0xFFFF��ֱ�Ӹ�ֵ��CAN_FilterMaskIdLow������һ��ȼ�
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	uint32_t ID3 = ((uint32_t)0x567) << 21;                      //����������������FIFO�Ĵ����ű��뱣��һ�£���ȻFIFO�Ĵ����Ų�һ�µĻ���MyCAN_ReceiveFlag()��MyCAN_Receive()������     
	CAN_FilterInitStructure.CAN_FilterIdHigh = ID3>>16;          //FIFO�Ĵ�����ֻ��Ϊ0��1�������Ľ��ջ�ʧ��
	CAN_FilterInitStructure.CAN_FilterIdLow = ID3;               
	uint32_t ID4 = (0xAAAAAAAAu <<3) | 0x4;                      
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = ID4>>16;      
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = ID4;           
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
}

void MyCAN_Transmit(CanTxMsg* TxMessage)
{
//	CanTxMsg TxMessage;
//	TxMessage.StdId = ID;
//	TxMessage.ExtId = ID;
//	TxMessage.IDE = CAN_Id_Standard;          //��׼ID
//	TxMessage.RTR = CAN_RTR_Data;             //����֡
//	TxMessage.DLC = Length;                   //���ݳ���
//	for (uint8_t i = 0; i < Length; i ++)
//	{
//		TxMessage.Data[i] = Data[i];          //������Data[i]��ֵ�����ͱ���TxMessage.Data[i]
//	}
//	uint8_t TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);     //����CAN_Transmit()���������ͱ���TxMessage��ֵ
//	                                                              // ������TransmitMailbox
	uint32_t Timeout = 0;
	
	uint8_t TransmitMailbox = CAN_Transmit(CAN1, TxMessage);     //����CAN_Transmit()���������ͱ���TxMessage��ֵ	
	
	while (CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok)
	{
		Timeout ++;
		if(Timeout > 100000)
		{
			break;
		
		}
	}
}                                             //����while()ѭ�����ȴ�CAN_TransmitStatus()��������״̬���

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
//	CanRxMsg RxMessage;                      //������ձ���
	
	CAN_Receive(CAN1, CAN_FIFO0, RxMessage);
	
//	if (RxMessage.IDE == CAN_Id_Standard)    //��ʶ������׼ID
//	{
//		*ID = RxMessage.StdId;
//	}
//	else if(RxMessage.IDE == CAN_Id_Extended)
//	{
//		*ID = RxMessage.ExtId;
//	}
//	
//	if (RxMessage.RTR == CAN_RTR_Data)       //Զ�̷�����������֡
//	{
//		*Length = RxMessage.DLC;
//		for (uint8_t i = 0; i < *Length; i ++)
//		{
//			Data[i] = RxMessage.Data[i];           //�����ձ��ĵ�����RxMessage.Data[i]��ֵ������Data[i]
//		}
//	}
//	else
//	{
//		//...
//	}
}
