#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "can.h" 
#include "lin.h" 
#include <string.h>
/*byte0Ϊ���ʹ���*/
u8 can_cyclicbuf[8] = {0x00,0x11,0x22,0x33,0x44,0x66,0x77,0xAA};
u8 can_eventbuf[8] = {0x00,0x02,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
u8 canrbuf[8],id_cnt = 0;

void LIN_LED_Drive(LIN_MSG* pLINMsg);
LIN_MSG*  pWiperMsg;    //LIN���� 
extern    LIN_BUFFER  LIN_RxDataBuff;   //������

u8 Data[3] = {0x23,0x24,0x25};
int main(void)
{	
	delay_init();	    	//��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(115200);	   //���ڳ�ʼ��Ϊ9600
	LED_Init();		  	   //��ʼ����LED���ӵ�Ӳ���ӿ� 
//	TIM3_Int_Init(99,7199);//10Khz�ļ���Ƶ�ʣ�������100Ϊ10ms
//	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN��ʼ������ģʽ,������500Kbps
//	CAN_RXMODE_Init();     //���չ�������ʼ��
	LIN_Mode_Init(1200);
	while(1)
	{
//		app_can_cyclicmsg_test(can_cyclicbuf,8); 
//		app_can_eventmsg_test(can_eventbuf,8);
//		LIN_Send_data(id_cnt++,Data,3);
//		delay_ms(100);
//		if(id_cnt > 0x3C) id_cnt = 0;
		LIN_RingBUF_Read(&LIN_RxDataBuff,&pWiperMsg);  //����ȡ������������
		LIN_LED_Drive(pWiperMsg);                      //�����ݲ��Ͻ���
	}	 
}

//�ӻ���������LED
void LIN_LED_Drive(LIN_MSG* pLINMsg)
{	
	if (pLINMsg->FrameID == 0x31){
		switch (pLINMsg->Data[0]){		
			case 0x01:
					LED0 = 0;
					pLINMsg->Data[0] = 0;
					LIN_Send_data(0x13,Data,3);	
					
			break;
			case 0x02:	
					LED1 = 0;
					pLINMsg->Data[0] = 0;
					LIN_Send_data(0x12,Data,3);
			break;	
			default: 
				 return;				
		}			
	}
}

