#include "delay.h"

uint32_t  us;		//us
uint32_t  ms;		//ms

void Delay_Init(unsigned char clk)
{
    CRG->SYSTICK_SETUP |= 1<<26;            //开启系统滴答定时器
    SysTick->CTRL &= ~(1<<2);           //选择内核参考时钟
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
    SysTick->LOAD=nus*us;         //时间加载
    SysTick->VAL=0x00;              //清空计数器
    SysTick->CTRL=0x07 ;            //开始倒数
    do
    {
        temp=SysTick->CTRL;        //获取计数值
    }
    while((temp&0x01)&&!(temp&(1<<16))); //等待时间到达
    SysTick->CTRL=0x00;              //关闭计数器
    SysTick->VAL =0X00;             //清空计数器
}


/********************************************************************************
* Function name     : Delay_ms
* Description        : 
* Input              : 
* Output             : 
* return          :
* Note            :  SysTick->LOAD为24位寄存器,nms的范围对48M条件下,nms<=349
********************************************************************************/
void Delay_ms(unsigned int nms)
{
    unsigned int temp;
    SysTick->LOAD=(unsigned int)nms*ms;  //时间加载(SysTick->LOAD为24bit)
    SysTick->VAL =0x00;              //清空计数器
    SysTick->CTRL=0x07 ;             //开始倒数
    do
    {
        temp=SysTick->CTRL;     //获取计数值
    }
    while((temp&0x01)&&!(temp&(1<<16))); //等待时间到达
    SysTick->CTRL=0x00;           //关闭计数器
    SysTick->VAL =0X00;           //清空计数器
}










