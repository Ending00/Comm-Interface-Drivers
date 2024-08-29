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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200); 
	usart2_init(115200);
	led_init();
	key_init(); 
	tim3_int_init(9,7199);    //1ms
	dev_key_attach_init();   //��������ע��
	dev_rgb_attach_init();	 //RGB����ע��
	
//	iic_init();
//	oled_init();			//��ʼ��oled 

	atk_8266_test(ATK_ESP8266_CWMODE_TBL[0]);

    while (1) {

    }
 }

