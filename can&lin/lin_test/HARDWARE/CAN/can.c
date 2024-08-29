#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
/*
CAN 发送流程为：程序选择 1 个空置的邮箱（TME=1）→设置标识符（ID），数据长度和
发送数据→设置 CAN_TIxR 的 TXRQ 位为 1，请求发送→邮箱挂号（等待成为最高优先级）→
预定发送（等待总线空闲）→发送→邮箱空置

CAN 接收流程为：FIFO 空→收到有效报文→挂号_1（存入 FIFO 的一个邮箱，这个由硬件
控制，我们不需要理会）→收到有效报文→挂号_2→收到有效报文→挂号_3→收到有效报文→
溢出
*/
//V1.1修改说明 20150528
//修正了CAN初始化函数的相关注释，更正了波特率计算公式
////////////////////////////////////////////////////////////////////////////////// 	 
 
//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024;  tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//则波特率为:36M/((8+9+1)*4)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败; 
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{ 
	GPIO_InitTypeDef 		GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
//	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
	NVIC_InitTypeDef  		NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO

	//CAN单元设置
	CAN_InitStructure.CAN_TTCM=DISABLE;			//非时间触发通信模式  
	CAN_InitStructure.CAN_ABOM=DISABLE;			//软件自动离线管理	 
	CAN_InitStructure.CAN_AWUM=DISABLE;			//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART=ENABLE;			//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM=DISABLE;		 	//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP=DISABLE;			//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode= mode;	        //模式设置： mode:0,普通模式;1,回环模式; 
	//设置波特率
	CAN_InitStructure.CAN_SJW=tsjw;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=tbs1; 			//Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=tbs2;				//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=brp;        //分频系数(Fdiv)为brp+1	
	CAN_Init(CAN1, &CAN_InitStructure);        	//初始化CAN1 

//	// 使用滤波器0
//	CAN_FilterInitStructure.CAN_FilterNumber = 0;
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;

//	// 第一个条目：0xAE 的标准数据帧
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (0xAE << 5);
//	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;

//	// 第二个条目：0xAE 的远程帧（RTR 位设置为1）
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xAE << 5) | (1 << 4);
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;

//	// 将过滤器连接到 FIFO0 并启用
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

//	CAN_FilterInit(&CAN_FilterInitStructure);

//	// 为0xFE标识符设置另一个滤波器
//	CAN_FilterInitStructure.CAN_FilterNumber = 1; // 使用另一个滤波器

//	// 第一个条目：0xFE 的标准数据帧
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (0xFE << 5);
//	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;

//	// 第二个条目：0xFE 的远程帧（RTR 位设置为1）
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xFE << 5) | (1 << 4);
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;

//	// 将过滤器连接到 FIFO0 并启用
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

//	CAN_FilterInit(&CAN_FilterInitStructure);
	
#if CAN_RX0_INT_ENABLE 
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);				//FIFO0消息挂号中断允许.		    

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN_RX0_INT_ENABLE	//使能RX0中断
//中断服务函数		
//u8 response_data[8] = {1,2,3,4,5,6,7,8};  // 根据需要设置响应数据
CanRxMsg RxMessage;
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);  // 清除中断标志
}
#endif

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 Can_Send_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0xAE;			// 标准标识符 
	TxMessage.ExtId=0xAE;			// 设置扩展标示符 
	TxMessage.IDE=CAN_Id_Standard; 	// 标准帧 or 扩展帧
	TxMessage.RTR=CAN_RTR_Data;		// 数据帧
	TxMessage.DLC=len;				// 要发送的数据长度
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];			          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	if(i>=0XFFF)return 1;
	return 0;	 
}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}

#define TX_CAN_CYCLIC_ID 	0x123
#define TX_CAN_EVENT_ID     0x124
#define TX_CAN_CE_ID      	0x125

#define RX_CAN_ID 			0x201

/***********************************************************************
* function: 编程实现周期发送报文ID：0x123，周期T为1s
************************************************************************/
u8 app_can_cyclicmsg_test(u8* msg,u8 len)
{
	static u16 timer = 0;	
	/* 发送报文中用一个字节来作为计数器 */
    static u8 tx_counter = 0;
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	
	if(tim_10ms)
	{
		tim_10ms = false;
		if(timer++>100)
		{
			LED0 = !LED0;//提示数据接收
			timer = 0;
		}
		else{
			return 1;
		}
	}else{
		return 1;
	}	
	
	TxMessage.StdId=TX_CAN_CYCLIC_ID;			// 标准标识符 
	TxMessage.ExtId = 0x0000;					// 设置扩展标示符 
	TxMessage.IDE = CAN_Id_Standard; 			// 标准帧
	TxMessage.RTR = CAN_RTR_Data;				// 数据帧
	TxMessage.DLC = len;
	
	TxMessage.Data[0] = tx_counter++;       /* 用来识别报文发送计数器 */
    for(i=1; i<TxMessage.DLC; i++)
    {
		TxMessage.Data[i] = msg[i];            
    }
	mbox = CAN_Transmit(CAN1, &TxMessage);   
	i = 0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	return (i >= 0xFFF) ? 1 : 0;	
}

/***********************************************************************
* function: 当接收到一帧报文（ID：0x201）的信号ECU_RX_MSG1_signal1=1时，触发发送事件型报文（ID:0x124）
            让ECU_MSG2_signal2（Byte1字节）=2 且两帧报文间时间间隔为50ms
************************************************************************/
void CAN_RXMODE_Init(void)
{
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	// 使用滤波器0
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	// 第一个条目：0xAE 的标准数据帧
	CAN_FilterInitStructure.CAN_FilterIdHigh = (RX_CAN_ID << 5);
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	// 第二个条目：0xAE 的远程帧（RTR 位设置为1）
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (RX_CAN_ID << 5) | (1 << 4);
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	// 将过滤器连接到 FIFO0 并启用
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

	CAN_FilterInit(&CAN_FilterInitStructure);	
}
u8 app_can_eventmsg_test(u8* msg,u8 len)
{
	static u16 timer = 0;	
	/* 发送报文中用一个字节来作为计数器 */
    static u8 tx_counter = 0;
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=TX_CAN_EVENT_ID;			// 标准标识符 
	TxMessage.ExtId = 0x0000;					// 设置扩展标示符 
	TxMessage.IDE = CAN_Id_Standard; 			// 标准帧
	TxMessage.RTR = CAN_RTR_Data;				// 数据帧
	TxMessage.DLC = len;
	if ((RxMessage.StdId == RX_CAN_ID)&&(RxMessage.RTR == CAN_RTR_Remote)) //接收到主机远程帧
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
		
		TxMessage.Data[0] = tx_counter++;       /* 用来识别报文发送计数器 */
		for(i=1; i<TxMessage.DLC; i++)
		{
			TxMessage.Data[i] = msg[i];            
		}
		mbox = CAN_Transmit(CAN1, &TxMessage);   
		if(TxMessage.Data[0] == 0x02)
		{
			tx_counter = 0;
			memset(&TxMessage, 0, sizeof(CanTxMsg));  // 将数据清零
			memset(&RxMessage, 0, sizeof(CanRxMsg));  
		}
		i = 0; 
		while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
		return (i >= 0xFFF) ? 1 : 0;				
	}
	return 0; 
}












