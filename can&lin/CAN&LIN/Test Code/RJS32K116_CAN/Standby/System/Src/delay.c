#include "delay.h"

uint32_t  us;		//us
uint32_t  ms;		//ms

void Delay_Init(unsigned char clk)
{
    CRG->SYSTICK_SETUP |= 1<<26;            //����ϵͳ�δ�ʱ��
    SysTick->CTRL &= ~(1<<2);           //ѡ���ں˲ο�ʱ��
    us=clk;                 //us
    ms=(us*1000);           //ms = us*1000
}

/********************************************************************************
* Function name     : Delay_us
* Description        : 
* Input              : 
* Output             : 
* return          :
********************************************************************************/
void Delay_us(unsigned int nus)
{
    unsigned int temp;
    SysTick->LOAD=nus*us;         //ʱ�����
    SysTick->VAL=0x00;              //��ռ�����
    SysTick->CTRL=0x07 ;            //��ʼ����
    do
    {
        temp=SysTick->CTRL;        //��ȡ����ֵ
    }
    while((temp&0x01)&&!(temp&(1<<16))); //�ȴ�ʱ�䵽��
    SysTick->CTRL=0x00;              //�رռ�����
    SysTick->VAL =0X00;             //��ռ�����
}


/********************************************************************************
* Function name     : Delay_ms
* Description        : 
* Input              : 
* Output             : 
* return          :
* Note            :  SysTick->LOADΪ24λ�Ĵ���,nms�ķ�Χ��48M������,nms<=349
********************************************************************************/
void Delay_ms(unsigned int nms)
{
    unsigned int temp;
    SysTick->LOAD=(unsigned int)nms*ms;  //ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL =0x00;              //��ռ�����
    SysTick->CTRL=0x07 ;             //��ʼ����
    do
    {
        temp=SysTick->CTRL;     //��ȡ����ֵ
    }
    while((temp&0x01)&&!(temp&(1<<16))); //�ȴ�ʱ�䵽��
    SysTick->CTRL=0x00;           //�رռ�����
    SysTick->VAL =0X00;           //��ռ�����
}










