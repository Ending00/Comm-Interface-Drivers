#include "uart.h"
#include <stdio.h>

void UART0_Init(uint32_t baudrate, uint32_t mck)
{
    uint32 integerdivider = 0x00;
    uint32 fractionaldivider = 0x00;
    uint32 tmpreg = 0;

    CRG->CKEN_CTRL0 |= 1<< 24;
    UART0->UART_CR |= (Bit4_En|Bit6_En);

    UART0->UART_MR &= ~(Bit0_En|Bit1_En|Bit2_En|Bit3_En);        //UARTģʽ
    UART0->UART_MR &= ~(Bit4_En|Bit5_En);                        //MCK��ϵͳʱ��
    UART0->UART_MR |= (Bit6_En|Bit7_En);                         //8Bit
    UART0->UART_MR &= Bit8_Dis;                                  //�첽
    UART0->UART_MR |= Bit11_En;                                  //��У��
    UART0->UART_MR &= ~(Bit9_En|Bit10_En);
    UART0->UART_MR &= ~(Bit12_En|Bit13_En);                      //1λֹͣλ

    UART0->UART_IER = Bit0_En;                                  //RXRDY�ж�����
    integerdivider = mck/(baudrate*16);
    fractionaldivider = ((mck/(2*baudrate))-(8*integerdivider));
    tmpreg = (integerdivider|(fractionaldivider<<16));
    UART0->UART_BRGR = tmpreg;
}

void UART0_SendData(uint8 Data)
{
    UART0->UART_THR = Data;
    while((UART0->UART_CSR&Bit9_En) != Bit9_En);
}

uint8 UART0_ReceiveData(void)
{
    uint8 Data = 0;
    Data = (uint8)(UART0->UART_RHR&0xff);
    return Data;
}

int fputc(int ch, FILE *f)
{
    UART0_SendData(ch);
    return ch;
}










