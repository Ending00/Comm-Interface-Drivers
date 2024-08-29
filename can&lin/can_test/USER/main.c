#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "can.h" 
/*byte0Ϊ���ʹ���*/
u8 can_cyclicbuf[8] = {0x00,0x11,0x22,0x33,0x44,0x66,0x77,0xAA};
u8 can_eventbuf[8] = {0x00,0x02,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
u8 canrbuf[8];

 int main(void)
 {	
	delay_init();	    	//��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(115200);	   //���ڳ�ʼ��Ϊ9600
	LED_Init();		  	   //��ʼ����LED���ӵ�Ӳ���ӿ� 
	TIM3_Int_Init(99,7199);//10Khz�ļ���Ƶ�ʣ�������100Ϊ10ms
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN��ʼ������ģʽ,������500Kbps
	CAN_RXMODE_Init();     //���չ�������ʼ��
	while(1)
	{
		app_can_cyclicmsg_test(can_cyclicbuf,8); 
		app_can_eventmsg_test(can_eventbuf,8);
	}	 
}


