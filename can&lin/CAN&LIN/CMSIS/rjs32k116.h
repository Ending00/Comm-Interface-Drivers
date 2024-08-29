/**
  ******************************************************************************
  * @file    rjs32k116.h
  * @author  Runjet Soft Team
  * @version V0.1.0
  * @date    2023-10-10
  * @brief   This file declares all the registers related to RJS32K116 chip.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Runjet SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM
  * THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
  * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2022 Runjet</center></h2>
  ******************************************************************************
  */

#ifndef  __RJS32K116_H
#define  __RJS32K116_H


#ifdef __cplusplus
extern "C" {
#endif


/**
*****************************************************************************
**                     THE Marco of Data Type
*****************************************************************************
*/
     
    typedef unsigned char 	BYTE;
    typedef unsigned int 	WORD;

    typedef unsigned char uint8;
    typedef unsigned int  uint16;
    typedef unsigned long uint32;
    
    typedef enum
    {
        RESET = 0,
        SET   = !RESET
    } FlagStatus, ITStatus;

    typedef enum
    {
        DISABLE = 0,
        ENABLE  = !DISABLE
    } FunctionalState, CtrlState;

#define IS_FUCTIONAL_STATE(STATE)  (((STATE) == DISABLE) || \
							        ((STATE) == ENABLE))
    typedef enum
    {
        Bit_RESET = 0,
        Bit_SET   = !Bit_RESET
    } BitAction;

    typedef enum
    {
        ERROR   = 0,
        SUCCESS = !ERROR
    } ErrorStatus;

    typedef enum
    {
        FALSE = 0,
        TRUE  = !FALSE
    } ErrorType;
    
    
    
/* Configuration of the Cortex-M0+ Processor and Core Peripherals */
#define __CM0PLUS_REV             0U 
#define __MPU_PRESENT             1U
#define __VTOR_PRESENT            1U
#define __NVIC_PRIO_BITS          2U      
#define __Vendor_SysTickConfig    0U



/************** Interupt Number **************/
    typedef enum
    {
        Reset_IRQn                  = -15,
        NonMaskableInt_IRQn         = -14,
        HardFault_IRQn              = -13,
        SVCall_IRQn                 = -5,
        PendSV_IRQn                 = -2,
        SysTick_IRQn                = -1,

        PVD_IRQn                    = 0,
        IWDG_IRQn                   = 1,
        
        I2C0_IRQn                   = 3,
        I2C1_IRQn                   = 4,
        SPI0_IRQn                   = 5,
        SPI1_IRQn                   = 6,
        UART0_IRQn                  = 7,
        UART1_IRQn                  = 8,
        UART2_IRQn                  = 9,
        UART3_IRQn                  = 10,

        LPUART_IRQn                 = 13,
        RTC_IRQn                    = 14,
        GPIOA_IRQn                  = 15,
        GPIOB_IRQn                  = 16,
        GPIOC_IRQn                  = 17,
        GPIOH_IRQn                  = 18,
        TIMER03_IRQn                 = 19,
        PWDT0_IRQn                  = 20,
        PWDT1_IRQn                   = 21,
        PWM0_IRQn                   = 22,
        PWM1_IRQn                   = 23,
        LPTIMER_IRQn                = 24,
        WDG_IRQn                    = 25,
        ADC_IRQn                    = 26,
        CLKCHK_IRQn                 = 27,
        COMP1COMP2_IRQn             = 28,
        CAN_IRQn                    = 29,
        SRAM_ERROR_IRQn             = 30,
        DMA_IRQn                    = 31 
        
    }   IRQn_Type;

#include <core_cm0plus.h>
#include <stdint.h>


/*----------- FLASH -----------*/
#define FLASH_BASE            0x40007000UL
#define FLASH                ((FLASH_TypeDef *)FLASH_BASE)

typedef struct
{
    __IO uint32_t Reserved;    
    __IO uint32_t FLASH_CON;
    __IO uint32_t CHIP_MODE;
    __IO uint32_t FLASH_STS;

    __IO uint32_t FLASH_LOCK_NVR;
    __IO uint32_t FLASH_LOCK; 
    __IO uint32_t CHIP_LOCK_EN;    
    __IO uint32_t FLASH_TIMING_0;
    
    __IO uint32_t FLASH_TIME_OUT;
    __IO uint32_t CMCC_MAINT0; 
    __IO uint32_t CMCC_MAINT1; 
    __IO uint32_t CMCC_MONITOR_CFG;
    
    __IO uint32_t CMCC_MONITOR_DATA;
    __IO uint32_t CMCC_CTRL;
}FLASH_TypeDef;


/*----------- IOCFG -----------*/
#define IOCFG_BASE            0x400B4000UL
#define IOCFG                ((IOCFG_TypeDef *)IOCFG_BASE)
typedef struct
{
    __IO uint32_t IOCFG_CTRLA;
    __IO uint32_t IOCFG_CTRLB;
    __IO uint32_t IOCFG_CTRLC;
    __IO uint32_t IOCFG_CTRLH;
   
    __IO uint32_t IOCFG_CTRLJ;
    __IO uint32_t IOCFG_CTRLK;  
    __IO uint32_t IOCFG_CTRLE;
    __IO uint32_t IOCFG_CTRLF;
	
    __IO uint32_t IOCFG_CTRLG;
    __IO uint32_t IOCFG_CTRLM;
    __IO uint32_t IOCFG_CTRLT; 
    __IO uint32_t IOCFG_CTRLR;

    __IO uint32_t IOCFG_CTRLW;
    __IO uint32_t IOCFG_CTRLN;
    __IO uint32_t IOCFG_CTRLP; 
    __IO uint32_t IOCFG_CTRLO;

    __IO uint32_t IOCFG_CTRLQ;
    __IO uint32_t IOCFG_CTRLV;
    __IO uint32_t IOCFG_CTRLD; 
    __IO uint32_t IOCFG_CTRLS;
}IOCFG_TypeDef; 

/*----------- CRG -----------*/
#define CRG_BASE            0x4008C000UL
#define CRG                ((CRG_TypeDef *)CRG_BASE)
typedef struct
{
    __IO uint32_t LP_CTRL;     
    __IO uint32_t Reserved0;
    __IO uint32_t CKEN_CTRL0;
    __IO uint32_t CKEN_CTRL1;
    
    __IO uint32_t SCSYS;        
    __IO uint32_t SCRSTEN;
    __IO uint32_t SCSSFTRST;
    __IO uint32_t Reserved1;
    
    __IO uint32_t VERSION;      
    __IO uint32_t HALT_WU_0;     
    __IO uint32_t HALT_WU_1;
    __IO uint32_t HALT_WU_2;
    
    __IO uint32_t HALT_POLAR_0; 
    __IO uint32_t HALT_POLAR_1; 
    __IO uint32_t HALT_FLAG0;   
    __IO uint32_t HALT_FALG1;
	
    __IO uint32_t HALT_FLAG2;        
    __IO uint32_t NMI_SEL;
    __IO uint32_t SYSTICK_SETUP;   
    __IO uint32_t RESET_FLAG;	
		
    __IO uint32_t REMAP_CTRL;		
    __IO uint32_t CLKCHK_CTRL;		
    __IO uint32_t CLKTRIM_CTRL;   
    __IO uint32_t CLKTRIM_VAL;
    
    __IO uint32_t CLKTRIM_CNT;		
    __IO uint32_t CLKCHK_XT32K_CFG;		
    __IO uint32_t CLKCHK_XT24M_CFG;   
    __IO uint32_t CLKCHK_PLL_CFG;    
	
    __IO uint32_t CLKCHK_STATUS; 
    __IO uint32_t CLKCHK_MASK;
    __IO uint32_t Reserved2; 
    __IO uint32_t Reserved3;	

    __IO uint32_t LPBG_CTRL;		
    __IO uint32_t HPBG_CTRL;		
    __IO uint32_t XOSC_CTRL;   
    __IO uint32_t RCOSC_CTRL; 
  	  
    __IO uint32_t PLL_CTRL0;		
    __IO uint32_t PLL_CTRL1;		
    __IO uint32_t SYSCFG_CFG0;   
    __IO uint32_t GPIO_FILTER_CTRL;  

    __IO uint32_t SRAM_FAIL_CTRL;		
    __IO uint32_t SRAM_FAIL_ADDR;		
    __IO uint32_t CTU_CFG0;   
    __IO uint32_t CAN_CTRL; 

    __IO uint32_t CAN_TIME_H;   
    __IO uint32_t CAN_TIME_L; 
}CRG_TypeDef;




/*----------- LXD -----------*/
#define LXD_BASE            0x40080000UL
#define LXD                ((LXD_TypeDef *)LXD_BASE)
typedef struct
{
    __IO uint32_t LXD_CON; 
         uint32_t RSV0[3];
	union 
	{
        __IO uint8_t DAT8_Buf[32];
		struct
		{
			__IO uint32_t LXD_DAT00_03;
			__IO uint32_t LXD_DAT04_07;
			__IO uint32_t LXD_DAT08_11;
			__IO uint32_t LXD_DAT12_15;
				
			__IO uint32_t LXD_DAT16_19;
			__IO uint32_t LXD_DAT20_23;
			__IO uint32_t LXD_DAT24_27;
			__IO uint32_t LXD_DAT28_31; 
		}DAT32;
	}LXD_DATA;
  
	uint32_t RSV1[20];

    __IO uint32_t LXD_COM;   
    __IO uint32_t LXD_SEG;
    __IO uint32_t LXD_TEST;
}LXD_TypeDef;  

/*----------- GPIO -----------*/
#define GPIOA_BASE            0x40096000UL
#define GPIOB_BASE            0x40097000UL
#define GPIOC_BASE            0x40098000UL
#define GPIOH_BASE            0x40099000UL
#define GPIOA                ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB                ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC                ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOH                ((GPIO_TypeDef *)GPIOH_BASE)

typedef struct
{
    __IO uint32_t GPIO_DATA;
    __IO uint32_t GPIO_DATA_OUT;
         uint32_t RSV0[2];
    
    __IO uint32_t GPIO_OUT_ENSET;
    __IO uint32_t RSV1[3];
    
    __IO uint32_t GPIO_INT_ENSET;
         uint32_t RSV2;
    __IO uint32_t GPIO_INT_TYPESET0;
    __IO uint32_t GPIO_INT_TYPESET1;
    
    __IO uint32_t GPIO_INT_TYPESET2;
         uint32_t RSV3;
    __IO uint32_t GPIO_INT_STATUS_CLEAR;      
}GPIO_TypeDef;  

/*----------- DMAC -----------*/
#define DMAC_BASE               ((uint32_t)0x40002000)
#define DMACCHX_BASE            ((uint32_t)(DMAC_BASE + 0x100))
#define DMACPERIPH_BASE         ((uint32_t)(DMAC_BASE + 0xFE0))
#define DMAC_TEST_BASE          ((uint32_t)(DMAC_BASE + 0x500))

#define DMAC                    ((DMAC_TypeDef *) DMAC_BASE)
#define DMACCHX                 ((DMACCHX_TypeDef *) DMACCHX_BASE)
#define DMACPERIPH              ((DMACPERIPH_TypeDef *) DMACPERIPH_BASE)
#define DMAC_TEST               ((DMAC_TEST_TypeDef *) DMAC_TEST_BASE)
typedef struct
{
    __IO uint32_t DMACIntStatus;        
    __IO uint32_t DMACIntTCStatus;      
    __IO uint32_t DMACIntTCClear;       
    __IO uint32_t DMACIntErrorStatus; 
    
    __IO uint32_t DMACIntErrClr;        
    __IO uint32_t DMACRawIntTCStatus;      
    __IO uint32_t DMACRawIntErrorStatus;       
    __IO uint32_t DMACEnbldChns; 

    __IO uint32_t DMACSoftBReq;        
    __IO uint32_t DMACSoftSReq;      
    __IO uint32_t DMACSoftLBReq;       
    __IO uint32_t DMACSoftLSReq; 

    __IO uint32_t DMACConfiguration;        
    __IO uint32_t DMACSync;
}DMAC_TypeDef;

typedef struct
{
    __IO uint32_t DMACC0SrcAddr;       
    __IO uint32_t DMACC0DestAddr; 
    __IO uint32_t DMACC0LLI;        
    __IO uint32_t DMACC0Control;

    __IO uint32_t DMACC0Configuration;
         uint32_t Reserved0[3];

    __IO uint32_t DMACC1SrcAddr;     
    __IO uint32_t DMACC1DestAddr;   
    __IO uint32_t DMACC1LLI;
    __IO uint32_t DMACC1Control; 

    __IO uint32_t DMACC1Configuration;  
         uint32_t Reserved1[3];

    __IO uint32_t DMACC2SrcAddr;            
    __IO uint32_t DMACC2DestAddr;        
    __IO uint32_t DMACC2LLI; 
    __IO uint32_t DMACC2Control; 

    __IO uint32_t DMACC2Configuration;
         uint32_t Reserved2[3]; 

    __IO uint32_t DMACC3SrcAddr;        
    __IO uint32_t DMACC3DestAddr; 
    __IO uint32_t DMACC3LLI; 
    __IO uint32_t DMACC3Control;

    __IO uint32_t DMACC3Configuration;
         uint32_t Reserved3[3]; 

    __IO uint32_t DMACC4SrcAddr; 
    __IO uint32_t DMACC4DestAddr; 
    __IO uint32_t DMACC4LLI;            
    __IO uint32_t DMACC4Control;

    __IO uint32_t DMACC4Configuration;
         uint32_t Reserved4[3]; 

    __IO uint32_t DMACC5SrcAddr;  
    __IO uint32_t DMACC5DestAddr;            
    __IO uint32_t DMACC5LLI;         
    __IO uint32_t DMACC5Control; 

    __IO uint32_t DMACC5Configuration; 
         uint32_t Reserved5[3];   

    __IO uint32_t DMACC6SrcAddr;        
    __IO uint32_t DMACC6DestAddr; 
    __IO uint32_t DMACC6LLI; 
    __IO uint32_t DMACC6Control;  

    __IO uint32_t DMACC6Configuration; 
         uint32_t Reserved6[3];

    __IO uint32_t DMACC7SrcAddr; 
    __IO uint32_t DMACC7DestAddr; 
    __IO uint32_t DMACC7LLI;            
    __IO uint32_t DMACC7Control;

    __IO uint32_t DMACC7Configuration;
}DMACCHX_TypeDef;

typedef struct
{
    __IO uint32_t DMACPeriphID0; 
    __IO uint32_t DMACPeriphID1;            
    __IO uint32_t DMACPeriphID2;        
    __IO uint32_t DMACPeriphID3; 

    __IO uint32_t DMACPCellID0; 
    __IO uint32_t DMACPCellID1;            
    __IO uint32_t DMACPCellID2;        
    __IO uint32_t DMACPCellID3; 
}DMACPERIPH_TypeDef;

typedef struct
{
    __IO uint32_t DMACITCR; 
    __IO uint32_t DMACITOP1;            
    __IO uint32_t DMACITOP2;        
    __IO uint32_t DMACITOP3;  
}DMAC_TEST_TypeDef;

/*----------- ADC -----------*/
#define ADC_BASE                ((uint32_t)0x40081000)
#define ADC                     ((ADC_TypeDef *) ADC_BASE)
typedef struct
{
    __IO uint32_t ADC_CR1; 
    __IO uint32_t ADC_CR2;            
    __IO uint32_t ADC_CR3;        
    __IO uint32_t ADC_CR4; 

    __IO uint32_t ADC_CR5; 
    __IO uint32_t ADC_STS1;            
    __IO uint32_t ADC_STS2;        
    __IO uint32_t ADC_MSK1; 

    __IO uint32_t ADC_MSK2; 
    __IO uint32_t ADC_DR;            
    __IO uint32_t ADC_WDG;        
}ADC_TypeDef;

/*----------- COMP -----------*/
#define COMP_BASE               ((uint32_t)0x4008C01C)
#define COMP                    ((COMP_TypeDef *)COMP_BASE)
typedef struct
{
    __IO uint32_t COMP_CTRL0;
    	  
}COMP_TypeDef;

/*----------- DIV/SQRT -----------*/
#define DIVSQT_BASE             ((uint32_t)0x4009B000)
#define SQRTDIV	                ((SQRTDIV_Type *) SQRTDIV_BASE )
typedef struct
{
    __IO uint32_t DIVSQRT_CTRL; 
    __IO uint32_t DIVSQRT_OP_A;            
    __IO uint32_t DIVSQRT_OP_B;        
    __IO uint32_t DIVSQRT_RESULT; 

    __IO uint32_t DIVSQRT_STS;
}DIVSQT_TypeDef;

/*----------- CRC -----------*/
//typedef struct
//{
//	volatile unsigned int  RESULT;
//	volatile unsigned int  INIT;
//	volatile unsigned int  CON;
//	volatile unsigned int  R[29];
//	volatile unsigned char DATA[128];
//} CRC_Type;

#define CRC_BASE                ((uint32_t)0x4009A000) 
#define CRC                     ((CRC_TypeDef *)CRC_BASE)
typedef struct
{
    __IO uint32_t CRC_DATA; 
    __IO uint32_t CRC_POLY;            
    __IO uint32_t CRC_SEED;        
    __IO uint32_t CRC_CTRL; 
}CRC_TypeDef;

/*----------- RNG -----------*/
#define RNG_BASE                ((uint32_t)0x4008D000)
#define RNG                     ((RNG_Type *) RNG_BASE)
typedef struct
{
    __IO uint32_t DTRNG_CON; 
    __IO uint32_t DTRNG_DAT;            
    __IO uint32_t DTRNG_STS;        
}RNG_TypeDef;

/*----------- PWM -----------*/
#define PWM0_BASE               0x400A7000
#define PWM1_BASE               0x400A8000
#define PWM0                ((PWM_TypeDef *)PWM0_BASE)
#define PWM1                ((PWM_TypeDef *)PWM1_BASE)
typedef struct
{
    __IO uint32_t PWM_INIT;
    __IO uint32_t PWM_CNT;
    __IO uint32_t PWM_MCVR;
    __IO uint32_t PWM_CNTIN;
	
    __IO uint32_t PWM_CH0SCR;		/*0x10*/
    __IO uint32_t PWM_CH1SCR;
    __IO uint32_t PWM_CH2SCR;
    __IO uint32_t PWM_CH3SCR;	
    __IO uint32_t PWM_CH4SCR;		/*0x20*/
    __IO uint32_t PWM_CH5SCR;
    __IO uint32_t PWM_CH6SCR;
    __IO uint32_t PWM_CH7SCR;
	
    __IO uint32_t PWM_CH0V;		 /*0x30*/
    __IO uint32_t PWM_CH1V;
    __IO uint32_t PWM_CH2V;
    __IO uint32_t PWM_CH3V;	
    __IO uint32_t PWM_CH4V;		 /*0x40*/
    __IO uint32_t PWM_CH5V;
    __IO uint32_t PWM_CH6V;
    __IO uint32_t PWM_CH7V;	
		
    __IO uint32_t PWM_CAPCMPSR;
    __IO uint32_t PWM_FUNCSEL;
    __IO uint32_t PWM_SYNC;
    __IO uint32_t PWM_OUTINIT;		
		
    __IO uint32_t PWM_OMCR;
    __IO uint32_t PWM_MODESEL;
    __IO uint32_t PWM_DTSET;
    __IO uint32_t PWM_EXTTRIG;	
		
    __IO uint32_t PWM_CHOPOLCR;
    __IO uint32_t PWM_FDSR;
    __IO uint32_t PWM_CAPFILTER;
    __IO uint32_t PWM_FFAFE;		
		
    __IO uint32_t PWM_QDI;
    __IO uint32_t PWM_CFG;
    __IO uint32_t PWM_FAULTPOL;
    __IO uint32_t PWM_SYNCCFG;	

    __IO uint32_t PWM_INVCR;
    __IO uint32_t PWM_CHOSWCR;		
}PWM_TypeDef;

/*----------- PWDT -----------*/
#define PWDT0_BASE               ((uint32_t)0x400A5000)
#define PWDT1_BASE               ((uint32_t)0x400A6000)
#define PWDT0                    ((PWDT_TypeDef *)PWDT0_BASE)
#define PWDT1                    ((PWDT_TypeDef *)PWDT1_BASE)
typedef struct
{
    __IO uint32_t PWDT_INIT0; 
    __IO uint32_t PWDT_NPW;            
    __IO uint32_t PWDT_INIT1; 
}PWDT_TypeDef;

/*----------- TIMER -----------*/
#define TIMER_BASE              ((uint32_t)0x400A4000) 
#define TIMER                   ((TIMER_TypeDef *)TIMER_BASE)
typedef struct
{
    __IO uint32_t TIMER_MCR; 
         uint32_t RSV[63];

    __IO uint32_t TIMER0_LDVAL;    //0x400A4100        
    __IO uint32_t TIMER0_CVAL; 
    __IO uint32_t TIMER0_INTF;            
    __IO uint32_t TIMER0_TF; 

    __IO uint32_t TIMER1_LDVAL;            
    __IO uint32_t TIMER1_CVAL; 
    __IO uint32_t TIMER1_INTF;            
    __IO uint32_t TIMER1_TF; 

    __IO uint32_t TIMER2_LDVAL;            
    __IO uint32_t TIMER2_CVAL; 
    __IO uint32_t TIMER2_INTF;            
    __IO uint32_t TIMER2_TF; 

    __IO uint32_t TIMER3_LDVAL;            
    __IO uint32_t TIMER3_CVAL; 
    __IO uint32_t TIMER3_INTF;            
    __IO uint32_t TIMER3_TF; 

}TIMER_TypeDef;

/*----------- BEEPER -----------*/
#define BEEPER_BASE              ((uint32_t)0x400A9000) 
#define BEEPER                   ((BEEPER_TypeDef *)BEEPER_BASE)
typedef struct
{
    __IO uint32_t TIMER_MCVR;    //0x400A4100        
    __IO uint32_t TIMER_CSR; 
}BEEPER_TypeDef;

/*----------- PCNT -----------*/
#define PCNT_BASE              ((uint32_t)0x400AA000) 
#define PCNT                   ((PCNT_TypeDef *)PCNT_BASE)
typedef struct
{
    __IO uint32_t PCNT_FILTER; 
    __IO uint32_t PCNT_MCVR;            
    __IO uint32_t PCNT_CSR; 
}PCNT_TypeDef;

/*----------- LPTIM -----------*/
#define LPTIM_BASE             ((uint32_t)0x400B0000)
#define LPTIM                  ((LPTIM_TypeDef *) LPTIM_BASE)
typedef struct
{
    __IO uint32_t LPTIM_EN; 
    __IO uint32_t LPTIM_TL0;   
    __IO uint32_t LPTIM_T0CON;         
    __IO uint32_t LPTIM_T0MOD; 

    __IO uint32_t LPTIM_FILTER; 
    __IO uint32_t RSV0;
    __IO uint32_t LPTIM_PCNT_TOP;   
    __IO uint32_t LPTIM_PCNT_CNT; 

    __IO uint32_t LPTIM_PCNT_BUF;
    __IO uint32_t LPTIM_PCNT_CMD; 
    __IO uint32_t LPTIM_PCNT_SR2;   
    __IO uint32_t LPTIM_PCNT_MASK;  

    __IO uint32_t LPTIM_PCNT_INT_STATUS;          
}LPTIM_TypeDef; 

/*----------- WDT -----------*/
#define WDT_BASE             ((uint32_t)0x40082000)
#define WDT                  ((WDT_TypeDef *) WDT_BASE)
typedef struct
{
    __IO uint32_t WDOGLOAD; 
    __IO uint32_t WDOGVALUE;   
    __IO uint32_t WDOGCONTROL;         
    __IO uint32_t WDOGINTCLR; 

    __IO uint32_t WDOGRIS; 
    __IO uint32_t WDOGMMIS;   
         uint32_t RSV0[14];         
    __IO uint32_t WDOGLOCK; //0x40082040

         uint32_t RSV1[8];
    __IO uint32_t WDOGITCR; //0x40082060
    __IO uint32_t WDOGCITOP;            
}WDT_TypeDef; 

/*----------- IWDT -----------*/
#define IWDG_BASE             ((uint32_t)0x40090000)
#define IWDG                  ((IWDG_TypeDef *) IWDG_BASE)
typedef struct
{
    __IO uint32_t IWDG_KR; 
    __IO uint32_t IWDG_PR;   
    __IO uint32_t IWDG_RLR;         
    __IO uint32_t IWDG_SR;         
}IWDG_TypeDef; 

/*----------- RTC -----------*/
#define RTC_BASE             ((uint32_t)0x4008B000)
#define RTC                  ((RTC_TypeDef *) RTC_BASE)
typedef struct
{
    __IO uint32_t RTC_CFG; 
    __IO uint32_t RTC_CFG_H;   
    __IO uint32_t RTC_ALARM_CFG;         
    __IO uint32_t RTC_SEC_CNT; 

    __IO uint32_t RTC_TICK_CTRL; 
    __IO uint32_t RTC_ALARM_CTRL;   
    __IO uint32_t RTC_RELOAD;         
    __IO uint32_t RTC_REFRESH;

    __IO uint32_t RTC_CLR_INT; 
    __IO uint32_t RTC_START;            
}RTC_TypeDef; 

/*----------- NVIC -----------*/
#define NVIC_ISER						(*(volatile unsigned int *)	0xE000E100)
#define NVIC_ICER						(*(volatile unsigned int *)	0xE000E180)
#define NVIC_ISPR						(*(volatile unsigned int *)	0xE000E200)
#define NVIC_ICPR						(*(volatile unsigned int *)	0xE000E280)
#define NVIC_IPR0						(*(volatile unsigned int *)	0xE000E400)
#define NVIC_IPR1						(*(volatile unsigned int *)	0xE000E404)
#define NVIC_IPR2						(*(volatile unsigned int *)	0xE000E408)
#define NVIC_IPR3						(*(volatile unsigned int *)	0xE000E40C)
#define NVIC_IPR4						(*(volatile unsigned int *)	0xE000E410)
#define NVIC_IPR5						(*(volatile unsigned int *)	0xE000E414)
#define NVIC_IPR6						(*(volatile unsigned int *)	0xE000E418)
#define NVIC_IPR7						(*(volatile unsigned int *)	0xE000E41C)

/*----------- SCB -----------*/
//#define SCB_BASE             ((uint32_t)0xE000ED00)
//#define SCB                  ((SCB_TypeDef *) SCB_BASE)
//typedef struct
//{
//    __IO uint32_t SCB_CPUID; 
//    __IO uint32_t SCB_ICSR;   
//    __IO uint32_t RSV0;         
//    __IO uint32_t SCB_AIRCR; 

//    __IO uint32_t SCB_SCR; 
//    __IO uint32_t SCB_CCR;   
//    __IO uint32_t RSV1;         
//    __IO uint32_t SCB_SHPR2;

//    __IO uint32_t SCB_SHRP3; 
//    __IO uint32_t SCB_SHCSR;            
//}SCB_TypeDef; 

/*----------- LPUART -----------*/
#define LPUART_BASE             ((uint32_t)0x4008A000)
#define LPUART                  ((LPUART_TypeDef *) LPUART_BASE)
typedef struct
{
    __IO uint32_t LPUART_CON; 
    __IO uint32_t LPUART_STS;            
    __IO uint32_t LPUART_TDAT; 
    __IO uint32_t LPUART_RDAT; 

    __IO uint32_t LPUART_DIV;            
}LPUART_TypeDef;   

/*----------- I2C -----------*/
#define I2C0_BASE               ((uint32_t)0x400B7000)
#define I2C1_BASE               ((uint32_t)0x400B7100)
#define I2C2_BASE               ((uint32_t)0x400B7200)
#define I2C0                    ((I2C_TypeDef *) I2C0_BASE)
#define I2C1                    ((I2C_TypeDef *) I2C1_BASE)
#define I2C2                    ((I2C_TypeDef *) I2C2_BASE)
typedef struct
{
    __IO uint32_t I2C_CLK_DIV; 
    __IO uint32_t I2C_CR;            
    __IO uint32_t I2C_INT_EN; 
    __IO uint32_t I2C_SR1; 

    __IO uint32_t I2C_DR; 
    __IO uint32_t I2C_DMA_CR;            
    __IO uint32_t I2C_DMA_STATUS; 
}I2C_TypeDef;

/*----------- UART -----------*/
#define UART0_BASE              ((uint32_t)0x400C0000)
#define UART1_BASE              ((uint32_t)0x400C1000) 
#define UART2_BASE              ((uint32_t)0x400C2000)
#define UART3_BASE              ((uint32_t)0x400C3000)
#define UART4_BASE              ((uint32_t)0x400C4000)
#define UART5_BASE              ((uint32_t)0x400C5000) 
#define UART0	                ((UART_TypeDef *) UART0_BASE)
#define UART1	                ((UART_TypeDef *) UART1_BASE)
#define UART2	                ((UART_TypeDef *) UART2_BASE)
#define UART3	                ((UART_TypeDef *) UART3_BASE)
#define UART4	                ((UART_TypeDef *) UART4_BASE)
#define UART5	                ((UART_TypeDef *) UART5_BASE)

#define UART0_WPMR						(*(volatile unsigned int *)	0x400C00E4)
#define UART0_WPSR						(*(volatile unsigned int *)	0x400C00E8)
	
typedef struct
{
    __IO uint32_t UART_CR; 
    __IO uint32_t UART_MR;            
    __IO uint32_t UART_IER; 
    __IO uint32_t UART_IDR; 

    __IO uint32_t UART_IMR; 
    __IO uint32_t UART_CSR;            
    __IO uint32_t UART_RHR; 
    __IO uint32_t UART_THR; 

    __IO uint32_t UART_BRGR; 
    __IO uint32_t UART_RTOR;            
    __IO uint32_t UART_TTGR; 
    __IO uint32_t Reserved0[8];

    __IO uint32_t UART_IF;

    __IO uint32_t Reserved1; 
    __IO uint32_t UART_LINMR;            
    __IO uint32_t UART_LINIR; 
    __IO uint32_t UART_LINBRR;             
}UART_TypeDef;

/*----------- CAN -----------*/
#define CAN_BASE                ((uint32_t)0x40003000)
#define CAN                     ((CAN_TypeDef *) CAN_BASE)

typedef volatile struct
{
    __IO uint32_t  ID_TTSEN;                    /*!< [0:28]: Identifier (ID), [29~31]: RSV         */
    __IO uint32_t  TXCTRL;                      /*!< [0:3]: Data length code (DLC), [4]: Bit rate switch (BRS)                 */
    /*!< [5]: FD format indicator (EDL),[6]: Remote transmission request (RTR)     */
    /*!< [7]: Identifier extension (IDE)                                           */
    __IO uint32_t  DATA[16];                    /*!< Buffer data                                                               */
} CAN_TBUF_TypeDef;

typedef volatile struct
{
    __IO uint32_t  ID_ESI;                      /*!< [0:28]: Identifier (ID), [31]: Error state indicator (ESI) */
    __IO uint32_t  RXCTRL;                      /*!< [0:3]: Data length code (DLC), [4]: Bit rate switch (BRS) */
    /*!< [5]: FD format indicator (EDL),[6]: Remote transmission request (RTR)     */
    /*!< [7]: Identifier extension (IDE),[12]: For lookback mode (Tx)              */
    __IO uint32_t  DATA[16];                    /*!< Buffer data */
    __IO uint32_t  RTS[2];
} CAN_RBUF_TypeDef;

typedef struct
{
    __IO CAN_RBUF_TypeDef  RBUF;
    __IO CAN_TBUF_TypeDef  TBUF;    
    __IO uint32_t CAN_TTS0;
         uint32_t Reserved; 

    __IO uint32_t CAN_CR0; 
    __IO uint32_t CAN_CR1; 
    __IO uint32_t CAN_SBITRATE; 
    __IO uint32_t CAN_FBITRATE;

    __IO uint32_t CAN_ERRINFO; 
    __IO uint32_t CAN_ACFCTRL; 
    __IO uint32_t CAN_ACF; 
    __IO uint32_t CAN_VERSION; 
}CAN_TypeDef;

/*----------- SPI -----------*/
#define SPI0_BASE               ((uint32_t)0x4008E000)
#define SPI1_BASE               ((uint32_t)0x4008F000)
#define SPI0                    ((SPI_TypeDef *) SPI0_BASE)
#define SPI1                    ((SPI_TypeDef *) SPI1_BASE)
typedef struct
{
    __IO uint32_t SPI_CR0; 
    __IO uint32_t SPI_CR1;            
    __IO uint32_t SPI_DR; 
    __IO uint32_t SPI_SR; 

    __IO uint32_t SPI_CPSR; 
    __IO uint32_t SPI_IMSC;            
    __IO uint32_t SPI_RIS; 
    __IO uint32_t SPI_MIS; 

    __IO uint32_t SPI_ICR; 
    __IO uint32_t SPI_DMA_CR;            
    __IO uint32_t SPI_WAIT;            
}SPI_TypeDef;

/*----------- ACMP -----------*/
#define ACMP_BASE               ((uint32_t)0x40084000)

/*----------- TOUCHKEY -----------*/
#define TOUCHKEY_BASE           ((uint32_t)0x400B3000)


/**
  * @}
  */

#define Bit0_En   (0X1<<0)
#define Bit1_En   (0X1<<1)
#define Bit2_En   (0X1<<2)
#define Bit3_En   (0X1<<3)
#define Bit4_En   (0X1<<4)
#define Bit5_En   (0X1<<5)
#define Bit6_En   (0X1<<6)
#define Bit7_En   (0X1<<7)
#define Bit8_En   (0X1<<8)
#define Bit9_En   (0X1<<9)
#define Bit10_En   (0X1<<10)
#define Bit11_En   (0X1<<11)
#define Bit12_En   (0X1<<12)
#define Bit13_En   (0X1<<13)
#define Bit14_En   (0X1<<14)
#define Bit15_En   (0X1<<15)
#define Bit16_En   (0X1<<16)
#define Bit17_En   (0X1<<17)
#define Bit18_En   (0X1<<18)
#define Bit19_En   (0X1<<19)
#define Bit20_En   (0X1<<20)
#define Bit21_En   (0X1<<21)
#define Bit22_En   (0X1<<22)
#define Bit23_En   (0X1<<23)
#define Bit24_En   (0X1<<24)
#define Bit25_En   (0X1<<25)
#define Bit26_En   (0X1<<26)
#define Bit27_En   (0X1<<27)
#define Bit28_En   (0X1<<28)
#define Bit29_En   (0X1<<29)
#define Bit30_En   (0X1<<30)
#define Bit31_En   (0X1UL<<31)

#define Bit0_Dis   (~(0X1))
#define Bit1_Dis   (~(0X1<<1))
#define Bit2_Dis   (~(0X1<<2))
#define Bit3_Dis   (~(0X1<<3))
#define Bit4_Dis   (~(0X1<<4))
#define Bit5_Dis   (~(0X1<<5))
#define Bit6_Dis   (~(0X1<<6))
#define Bit7_Dis   (~(0X1<<7))
#define Bit8_Dis   (~(0X1<<8))
#define Bit9_Dis   (~(0X1<<9))
#define Bit10_Dis   (~(0X1<<10))
#define Bit11_Dis   (~(0X1<<11))
#define Bit12_Dis   (~(0X1<<12))
#define Bit13_Dis   (~(0X1<<13))
#define Bit14_Dis   (~(0X1<<14))
#define Bit15_Dis   (~(0X1<<15))
#define Bit16_Dis   (~(0X1<<16))
#define Bit17_Dis   (~(0X1<<17))
#define Bit18_Dis   (~(0X1<<18))
#define Bit19_Dis   (~(0X1<<19))
#define Bit20_Dis   (~(0X1<<20))
#define Bit21_Dis   (~(0X1<<21))
#define Bit22_Dis   (~(0X1<<22))
#define Bit23_Dis   (~(0X1<<23))
#define Bit24_Dis   (~(0X1<<24))
#define Bit25_Dis   (~(0X1<<25))
#define Bit26_Dis   (~(0X1<<26))
#define Bit27_Dis   (~(0X1<<27))
#define Bit28_Dis   (~(0X1<<28))
#define Bit29_Dis   (~(0X1<<29))
#define Bit30_Dis   (~(0X1<<30))
#define Bit31_Dis   (~(0X1<<31))

typedef struct _16_Bits_Struct
{
    unsigned short  bit0 : 1;
    unsigned short  bit1 : 1;
    unsigned short  bit2 : 1;
    unsigned short  bit3 : 1;
    unsigned short  bit4 : 1;
    unsigned short  bit5 : 1;
    unsigned short  bit6 : 1;
    unsigned short  bit7 : 1;
    unsigned short  bit8 : 1;
    unsigned short  bit9 : 1;
    unsigned short  bit10 : 1;
    unsigned short  bit11 : 1;
    unsigned short  bit12 : 1;
    unsigned short  bit13 : 1;
    unsigned short  bit14 : 1;
    unsigned short  bit15 : 1;
} Bits_16_TypeDef;

#endif

