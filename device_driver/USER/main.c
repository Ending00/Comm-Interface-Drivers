/*
 * @Author: sven
 * @Date: 2024-09-13 11:38:23
 * @Last Modified by: sven
 * @Last Modified time: Do not edit
 * @FilePath: \device_driver\device_driver\USER\main.c
 * @Description: This file contains the implementation of a universal driver designed for flexibility and portability across various microcontroller platforms.
 * @Version: 1.0
 */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart_driver.h"
#include "iic_driver_pal.h"
void Delay(__IO uint32_t nCount);

void Delay(__IO uint32_t nCount)
{
  while(nCount--){}
}

int main(void)
{  
    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	uart_init(115200);					//初始化串口
	
	/*************串口驱动测试**************/
//	uart_driver_test();
	iic_driver_test();
	while(1)
	{

	}
}
