#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "sys.h"


#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//��ȡ����1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����2 


extern volatile uint32_t current_time_ms;
void key_init(void);//IO��ʼ��
void tim3_int_init(u16 arr,u16 psc);
#endif

