#ifndef __SYSTICK_DELAY_H
#define __SYSTICK_DELAY_H 			   
#include "sys.h" 

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
#define Delay_ms(x) Delay_us(100*x)	 //��λms

void SysTick_Delay_Us( __IO uint32_t us);
void SysTick_Delay_Ms( __IO uint32_t ms);



#endif