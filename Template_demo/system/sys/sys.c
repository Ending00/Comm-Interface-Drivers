#include "sys.h"

#ifdef GCC
// THUMB指令不支持汇编内联
// 采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
    __asm__ volatile ("wfi");
}

// 关闭所有中断
void INTX_DISABLE(void)
{		  
    __asm__ volatile ("cpsid i");
}

// 开启所有中断
void INTX_ENABLE(void)
{
    __asm__ volatile ("cpsie i");
}

// 设置栈顶地址
// addr:栈顶地址

void MSR_MSP(uint32_t addr)
{
    __asm__ volatile (
        "MSR MSP, %0\n"  // 设置主栈指针
        "BX lr\n"        // 返回
        :                // 没有输出
        : "r" (addr)     // 输入寄存器 r0
        : "memory"       // 内存被修改
    );
}
#endif

#ifndef GCC
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
#endif
