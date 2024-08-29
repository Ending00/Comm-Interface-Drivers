#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "pal_button.h"
//#include "oled.h"
#include "bsp_iic.h"
#include "bsp_led.h"
#include "pal_rgb.h" 
#include "bsp_usart2.h"
#include "common.h" 
#include "malloc.h" 

int main(void)
{	
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200); 
	usart2_init(115200);
	led_init();
	key_init(); 
	tim3_int_init(9,7199);    //1ms
	dev_key_attach_init();   //按键驱动注册
	dev_rgb_attach_init();	 //RGB驱动注册
	
//	iic_init();
//	oled_init();			//初始化oled 

	atk_8266_test(ATK_ESP8266_CWMODE_TBL[0]);

    while (1) {

    }
 }

