#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "can.h" 
/*byte0为发送次数*/
u8 can_cyclicbuf[8] = {0x00,0x11,0x22,0x33,0x44,0x66,0x77,0xAA};
u8 can_eventbuf[8] = {0x00,0x02,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
u8 canrbuf[8];

 int main(void)
 {	
	delay_init();	    	//延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(115200);	   //串口初始化为9600
	LED_Init();		  	   //初始化与LED连接的硬件接口 
	TIM3_Int_Init(99,7199);//10Khz的计数频率，计数到100为10ms
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps
	CAN_RXMODE_Init();     //接收过滤器初始化
	while(1)
	{
		app_can_cyclicmsg_test(can_cyclicbuf,8); 
		app_can_eventmsg_test(can_eventbuf,8);
	}	 
}


