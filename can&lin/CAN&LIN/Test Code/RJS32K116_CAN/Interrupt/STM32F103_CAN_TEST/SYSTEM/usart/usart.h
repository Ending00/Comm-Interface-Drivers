#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

void uart_init(u32 bound);
void USART1_SendBuff(u8 *pbuff,u32 len);


#endif


