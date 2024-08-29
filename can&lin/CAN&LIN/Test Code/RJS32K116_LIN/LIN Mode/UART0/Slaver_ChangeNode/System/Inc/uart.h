#ifndef __UART_H__
#define __UART_H__


#include "rjs32k116.h"


void LIN_Init(uint16 baudrate, uint32_t mck);
void UART0_Init(uint32_t baudrate, uint32_t mck);
void UART0_SendData(uint8 Data);
uint8 UART0_ReceiveData(void);
void UART0_LINSendByte(uint8 data);
void UART0_LINSendData(uint8 *data,uint16 length);


#endif
