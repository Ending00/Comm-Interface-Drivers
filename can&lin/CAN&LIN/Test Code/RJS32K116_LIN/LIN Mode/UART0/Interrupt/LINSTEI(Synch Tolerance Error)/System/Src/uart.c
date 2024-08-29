#include "uart.h"
#include <stdio.h>


void LIN_Init(uint16 baudrate, uint32_t mck)
{
    uint32 integerdivider = 0x00;
    uint32 fractionaldivider = 0x00;
    uint32 tmpreg = 0;
    
    CRG->CKEN_CTRL0 |= 1<<24;                       //UART0ʱ��ʹ��
    IOCFG->IOCFG_CTRLA |= (Bit21_En|Bit24_En);      //PA7 -> UART0_TX  PA8 -> UART0_RX

    UART0->UART_CR |= (Bit4_En|Bit6_En);            //���ͽ�����ʹ��
//    UART0->UART_MR |=  (Bit1_En|Bit3_En);           //LIN Master
    UART0->UART_MR |= (Bit0_En|Bit1_En|Bit3_En);      //LIN Slave
    
    UART0->UART_MR &= ~(Bit4_En|Bit5_En);           //MCK��ϵͳʱ��
    UART0->UART_MR |= (Bit6_En|Bit7_En);            //8Bit
    UART0->UART_MR &= Bit8_Dis;                     //�첽
    UART0->UART_MR |= Bit11_En;                     //��У��
    UART0->UART_MR &= ~(Bit9_En|Bit10_En);
    UART0->UART_MR &= ~(Bit12_En|Bit13_En);         //1λֹͣλ
    
    
    //�����ڵ�ֻ������
    UART0->UART_LINMR |= Bit0_En;                   //�����ڵ�
//    UART0->UART_LINMR &= ~(Bit0_En|Bit1_En);          //���ͽڵ�
    
//    UART0_LINMR |= Bit4_En;                     //LIN1.0ģʽʹ��
    UART0->UART_LINMR |= (0x7<<8);               //���ݳ���=DLC+1
    
    
    UART0->UART_IER |= Bit0_En;                     //RXRDY�ж�
//    UART0->UART_IER |= Bit13_En;                    //ͬ������ν����ж�
//    UART0->UART_IER |= Bit14_En;                    //��ʶ�������ж�   
    UART0->UART_IER |= Bit26_En;                    //ͬ���β�һ�� 
//    UART0->UART_IER |= Bit27_En;                    //��ʶ����żУ���ж� 
//    UART0->UART_IER |= Bit28_En;                    //У��� 
    UART0->UART_IER |= Bit30_En;                    //ͬ���ݲ�����ж� 
//    UART0->UART_IER |= Bit31_En;                    //֡ͷ��ʱ�����ж�   

    UART0->UART_CR |= Bit8_En;                        //��־λ��λ
    
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

void UART0_LINSendByte(uint8 data)
{
    while((UART0->UART_CSR & Bit1_En) != Bit1_En);        //�ȴ�TXRDYλ����
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
    while((UART0->UART_CSR & Bit15_En) != Bit15_En);      //�ȴ�LINTCλ����
    UART0->UART_CR |= Bit8_En;                            //LINTC��־λ��λ
}








