#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
/*
CAN ��������Ϊ������ѡ�� 1 �����õ����䣨TME=1�������ñ�ʶ����ID�������ݳ��Ⱥ�
�������ݡ����� CAN_TIxR �� TXRQ λΪ 1�������͡�����Һţ��ȴ���Ϊ������ȼ�����
Ԥ�����ͣ��ȴ����߿��У������͡��������

CAN ��������Ϊ��FIFO �ա��յ���Ч���ġ��Һ�_1������ FIFO ��һ�����䣬�����Ӳ��
���ƣ����ǲ���Ҫ��ᣩ���յ���Ч���ġ��Һ�_2���յ���Ч���ġ��Һ�_3���յ���Ч���ġ�
���
*/
//V1.1�޸�˵�� 20150528
//������CAN��ʼ�����������ע�ͣ������˲����ʼ��㹫ʽ
////////////////////////////////////////////////////////////////////////////////// 	 
 
//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024;  tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//������Ϊ:36M/((8+9+1)*4)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{ 
	GPIO_InitTypeDef 		GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
//	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
	NVIC_InitTypeDef  		NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��IO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��IO

	//CAN��Ԫ����
	CAN_InitStructure.CAN_TTCM=DISABLE;			//��ʱ�䴥��ͨ��ģʽ  
	CAN_InitStructure.CAN_ABOM=DISABLE;			//����Զ����߹���	 
	CAN_InitStructure.CAN_AWUM=DISABLE;			//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStructure.CAN_NART=ENABLE;			//��ֹ�����Զ����� 
	CAN_InitStructure.CAN_RFLM=DISABLE;		 	//���Ĳ�����,�µĸ��Ǿɵ�  
	CAN_InitStructure.CAN_TXFP=DISABLE;			//���ȼ��ɱ��ı�ʶ������ 
	CAN_InitStructure.CAN_Mode= mode;	        //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; 
	//���ò�����
	CAN_InitStructure.CAN_SJW=tsjw;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=tbs1; 			//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=tbs2;				//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=brp;        //��Ƶϵ��(Fdiv)Ϊbrp+1	
	CAN_Init(CAN1, &CAN_InitStructure);        	//��ʼ��CAN1 

//	// ʹ���˲���0
//	CAN_FilterInitStructure.CAN_FilterNumber = 0;
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;

//	// ��һ����Ŀ��0xAE �ı�׼����֡
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (0xAE << 5);
//	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;

//	// �ڶ�����Ŀ��0xAE ��Զ��֡��RTR λ����Ϊ1��
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xAE << 5) | (1 << 4);
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;

//	// �����������ӵ� FIFO0 ������
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

//	CAN_FilterInit(&CAN_FilterInitStructure);

//	// Ϊ0xFE��ʶ��������һ���˲���
//	CAN_FilterInitStructure.CAN_FilterNumber = 1; // ʹ����һ���˲���

//	// ��һ����Ŀ��0xFE �ı�׼����֡
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (0xFE << 5);
//	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;

//	// �ڶ�����Ŀ��0xFE ��Զ��֡��RTR λ����Ϊ1��
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xFE << 5) | (1 << 4);
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;

//	// �����������ӵ� FIFO0 ������
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

//	CAN_FilterInit(&CAN_FilterInitStructure);
	
#if CAN_RX0_INT_ENABLE 
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);				//FIFO0��Ϣ�Һ��ж�����.		    

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����		
//u8 response_data[8] = {1,2,3,4,5,6,7,8};  // ������Ҫ������Ӧ����
CanRxMsg RxMessage;
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);  // ����жϱ�־
}
#endif

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 Can_Send_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0xAE;			// ��׼��ʶ�� 
	TxMessage.ExtId=0xAE;			// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard; 	// ��׼֡ or ��չ֡
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
	TxMessage.DLC=len;				// Ҫ���͵����ݳ���
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];			          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;	 
}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}

#define TX_CAN_CYCLIC_ID 	0x123
#define TX_CAN_EVENT_ID     0x124
#define TX_CAN_CE_ID      	0x125

#define RX_CAN_ID 			0x201

/***********************************************************************
* function: ���ʵ�����ڷ��ͱ���ID��0x123������TΪ1s
************************************************************************/
u8 app_can_cyclicmsg_test(u8* msg,u8 len)
{
	static u16 timer = 0;	
	/* ���ͱ�������һ���ֽ�����Ϊ������ */
    static u8 tx_counter = 0;
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	
	if(tim_10ms)
	{
		tim_10ms = false;
		if(timer++>100)
		{
			LED0 = !LED0;//��ʾ���ݽ���
			timer = 0;
		}
		else{
			return 1;
		}
	}else{
		return 1;
	}	
	
	TxMessage.StdId=TX_CAN_CYCLIC_ID;			// ��׼��ʶ�� 
	TxMessage.ExtId = 0x0000;					// ������չ��ʾ�� 
	TxMessage.IDE = CAN_Id_Standard; 			// ��׼֡
	TxMessage.RTR = CAN_RTR_Data;				// ����֡
	TxMessage.DLC = len;
	
	TxMessage.Data[0] = tx_counter++;       /* ����ʶ���ķ��ͼ����� */
    for(i=1; i<TxMessage.DLC; i++)
    {
		TxMessage.Data[i] = msg[i];            
    }
	mbox = CAN_Transmit(CAN1, &TxMessage);   
	i = 0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	return (i >= 0xFFF) ? 1 : 0;	
}

/***********************************************************************
* function: �����յ�һ֡���ģ�ID��0x201�����ź�ECU_RX_MSG1_signal1=1ʱ�����������¼��ͱ��ģ�ID:0x124��
            ��ECU_MSG2_signal2��Byte1�ֽڣ�=2 ����֡���ļ�ʱ����Ϊ50ms
************************************************************************/
void CAN_RXMODE_Init(void)
{
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	// ʹ���˲���0
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	// ��һ����Ŀ��0xAE �ı�׼����֡
	CAN_FilterInitStructure.CAN_FilterIdHigh = (RX_CAN_ID << 5);
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	// �ڶ�����Ŀ��0xAE ��Զ��֡��RTR λ����Ϊ1��
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (RX_CAN_ID << 5) | (1 << 4);
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	// �����������ӵ� FIFO0 ������
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

	CAN_FilterInit(&CAN_FilterInitStructure);	
}
u8 app_can_eventmsg_test(u8* msg,u8 len)
{
	static u16 timer = 0;	
	/* ���ͱ�������һ���ֽ�����Ϊ������ */
    static u8 tx_counter = 0;
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=TX_CAN_EVENT_ID;			// ��׼��ʶ�� 
	TxMessage.ExtId = 0x0000;					// ������չ��ʾ�� 
	TxMessage.IDE = CAN_Id_Standard; 			// ��׼֡
	TxMessage.RTR = CAN_RTR_Data;				// ����֡
	TxMessage.DLC = len;
	if ((RxMessage.StdId == RX_CAN_ID)&&(RxMessage.RTR == CAN_RTR_Remote)) //���յ�����Զ��֡
	{
		if(tim_10ms)
		{
			tim_10ms = false;
			if(timer++>5)  //50ms
			{
				timer = 0;
			}
			else{
				return 1;
			}
		}else{
			return 1;
		}
		
		TxMessage.Data[0] = tx_counter++;       /* ����ʶ���ķ��ͼ����� */
		for(i=1; i<TxMessage.DLC; i++)
		{
			TxMessage.Data[i] = msg[i];            
		}
		mbox = CAN_Transmit(CAN1, &TxMessage);   
		if(TxMessage.Data[0] == 0x02)
		{
			tx_counter = 0;
			memset(&TxMessage, 0, sizeof(CanTxMsg));  // ����������
			memset(&RxMessage, 0, sizeof(CanRxMsg));  
		}
		i = 0; 
		while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
		return (i >= 0xFFF) ? 1 : 0;				
	}
	return 0; 
}












