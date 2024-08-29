#include "crg.h"


void HSI_Init(void)
{
    //freq_Sel
    CRG->SCSYS &= ~(0x03<<6);           //RC24M
    
    //freq_Sel_ahb
    CRG->SCSYS &= ~(0x03<<8);           //1��Ƶ

//    CRG->SCSYS |= 1<<8;               //2��Ƶ
//
//    CRG->SCSYS |= 2<<8;               //4��Ƶ
//
//    CRG->SCSYS |= 3<<8;               //32��Ƶ

    //freq_sel_apb
    CRG->SCSYS &= ~(0x03<<4);           //1��Ƶ

//    CRG->SCSYS |= 1<<4;               //2��Ƶ
//
//    CRG->SCSYS |= 2<<4;               //4��Ƶ
//
//    CRG->SCSYS |= 3<<4;               //8��Ƶ

    CRG->SCSSFTRST &= ~(0x01FFFEFA);    //��λʧ��
    CRG->CKEN_CTRL1 |= Bit16_En;        //����ʱ�ӿ���

}

void PLL_Init(void)
{
//    CRG->SCSYS &= ~(1<<3);              //src_clk_sel:RC_24M
    
    //freq_Sel_ahb
    CRG->SCSYS &= ~(0x03<<8);           //1��Ƶ

//    CRG->SCSYS |= 1<<8;               //2��Ƶ

//    CRG->SCSYS |= 2<<8;               //4��Ƶ

//    CRG->SCSYS |= 3<<8;               //32��Ƶ

    //freq_sel_apb
    CRG->SCSYS &= ~(0x03<<4);           //1��Ƶ

//    CRG->SCSYS |= 1<<4;               //2��Ƶ
//
//    CRG->SCSYS |= 2<<4;               //4��Ƶ
//
//    CRG->SCSYS |= 3<<4;               //8��Ƶ
    
    CRG->PLL_CTRL1 = 0x000840C4;
    CRG->PLL_CTRL0 = 0x00000001;
    FLASH->FLASH_TIMING_0 |= 4<<0;
    CRG->SCSYS |= 1<<7;
    
    CRG->SCSSFTRST &= ~(0x01FFFEFA);    //��λʧ��
    CRG->CKEN_CTRL1 |= Bit16_En;        //����ʱ�ӿ���    
    
}









