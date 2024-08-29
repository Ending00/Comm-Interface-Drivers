#include "uart.h"
#include <stdio.h>


void LIN_Init(uint16 baudrate, uint32_t mck)
{
    uint32 integerdivider = 0x00;
    uint32 fractionaldivider = 0x00;
    uint32 tmpreg = 0;
    
    CRG->CKEN_CTRL0 |= 1<<24;                       //UART0时钟使能
    IOCFG->IOCFG_CTRLA |= (Bit21_En|Bit24_En);      //PA7 -> UART0_TX  PA8 -> UART0_RX

    UART0->UART_CR |= (Bit4_En|Bit6_En);            //发送接收器使能
//    UART0->UART_MR |=  (Bit1_En|Bit3_En);           //LIN Master
    UART0->UART_MR |= (Bit0_En|Bit1_En|Bit3_En);      //LIN Slave
    
    UART0->UART_MR &= ~(Bit4_En|Bit5_En);           //MCK：系统时钟
    UART0->UART_MR |= (Bit6_En|Bit7_En);            //8Bit
    UART0->UART_MR &= Bit8_Dis;                     //异步
    UART0->UART_MR |= Bit11_En;                     //无校验
    UART0->UART_MR &= ~(Bit9_En|Bit10_En);
    UART0->UART_MR &= ~(Bit12_En|Bit13_En);         //1位停止位
    
    
    //收听节点只能收听
    UART0->UART_LINMR |= Bit0_En;                   //收听节点
//    UART0->UART_LINMR &= ~(Bit0_En|Bit1_En);          //发送节点
    
//    UART0_LINMR |= Bit4_En;                     //LIN1.0模式使能
    UART0->UART_LINMR |= (0x7<<8);               //数据长度=DLC+1
    
    
    UART0->UART_IER |= Bit0_En;                     //RXRDY中断
//    UART0->UART_IER |= Bit13_En;                    //同步间隔段接收中断
//    UART0->UART_IER |= Bit14_En;                    //标识符接收中断   
    UART0->UART_IER |= Bit26_En;                    //同步段不一致 
//    UART0->UART_IER |= Bit27_En;                    //标识符奇偶校验中断 
//    UART0->UART_IER |= Bit28_En;                    //校验和 
    UART0->UART_IER |= Bit30_En;                    //同步容差错误中断 
//    UART0->UART_IER |= Bit31_En;                    //帧头超时错误中断   

    UART0->UART_CR |= Bit8_En;                        //标志位复位
    
    integerdivider = mck/(baudrate*16);
    fractionaldivider = ((mck/(2*baudrate))-(8*integerdivider));
    tmpreg = (integerdivider|(fractionaldivider<<16));
    UART0->UART_BRGR = tmpreg;
    
}

void UART0_Init(uint32_t baudrate, uint32_t mck)
{
    uint32 integerdivider = 0x00;
    uint32 fractionaldivider = 0x00;
    uint32 tmpreg = 0;

    CRG->CKEN_CTRL0 |= 1<< 24;
    UART0->UART_CR |= (Bit4_En|Bit6_En);

    UART0->UART_MR &= ~(Bit0_En|Bit1_En|Bit2_En|Bit3_En);        //UART模式
    UART0->UART_MR &= ~(Bit4_En|Bit5_En);                        //MCK：系统时钟
    UART0->UART_MR |= (Bit6_En|Bit7_En);                         //8Bit
    UART0->UART_MR &= Bit8_Dis;                                  //异步
    UART0->UART_MR |= Bit11_En;                                  //无校验
    UART0->UART_MR &= ~(Bit9_En|Bit10_En);
    UART0->UART_MR &= ~(Bit12_En|Bit13_En);                      //1位停止位

    UART0->UART_IER = Bit0_En;                                  //RXRDY中断允许
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

void UART0_LINSendByte(uint8 data)
{
    while((UART0->UART_CSR & Bit1_En) != Bit1_En);        //等待TXRDY位置起
    UART0->UART_THR = data;
}
 
void UART0_LINSendData(uint8 *data,uint16 length)
{
    uint16 count;
    for(count = 0; count < length; count++)
    {
        UART0_LINSendByte(*data);
        data++;
    }
    while((UART0->UART_CSR & Bit15_En) != Bit15_En);      //等待LINTC位置起
    UART0->UART_CR |= Bit8_En;                            //LINTC标志位复位
}








