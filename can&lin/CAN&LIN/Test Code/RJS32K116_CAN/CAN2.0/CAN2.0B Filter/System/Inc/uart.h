#ifndef __UART_H__
#define __UART_H__


#include "rjs32k116.h"


void UART0_Init(uint32_t baudrate, uint32_t mck);
void UART0_SendData(uint8 Data);
uint8 UART0_ReceiveData(void);




#endif
