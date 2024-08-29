#include "bsp_led.h"

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void led_init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LED0-->PC.4 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_ResetBits(GPIOC,GPIO_Pin_4);						 //PC.4 输出低

// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PE.5 端口配置, 推挽输出
// GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
// GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PE.5 输出高 
}
 
