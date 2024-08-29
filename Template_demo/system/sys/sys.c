#include "sys.h"

#ifdef GCC
// THUMBָ�֧�ֻ������
// �������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
    __asm__ volatile ("wfi");
}

// �ر������ж�
void INTX_DISABLE(void)
{		  
    __asm__ volatile ("cpsid i");
}

// ���������ж�
void INTX_ENABLE(void)
{
    __asm__ volatile ("cpsie i");
}

// ����ջ����ַ
// addr:ջ����ַ

void MSR_MSP(uint32_t addr)
{
    __asm__ volatile (
        "MSR MSP, %0\n"  // ������ջָ��
        "BX lr\n"        // ����
        :                // û�����
        : "r" (addr)     // ����Ĵ��� r0
        : "memory"       // �ڴ汻�޸�
    );
}
#endif

#ifndef GCC
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
#endif
