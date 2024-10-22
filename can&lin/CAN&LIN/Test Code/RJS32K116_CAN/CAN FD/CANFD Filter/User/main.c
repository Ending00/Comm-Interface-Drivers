/**
  ******************************************************************************
  * @file    main.c
	* @author  MCU SYSTEM Team
	* @Version V1.0.0
  * @Date    2023-09-20
  * @brief   main function
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "RJS32K116.h"
#include <stdio.h>
#include "crg.h"
#include "delay.h"
#include "uart.h"
#include "can.h"
/* Private variables ---------------------------------------------------------*/
uint8_t count = 0, can_data[100] = {0x00};
CAN_TxMsg TxMsg;
CAN_InitTypeDef CAN_InitStructure;
CAN_FilterTypeDef CAN_FilterInit[16] =
{
        {0, DISABLE, DISABLE, DISABLE, 0x204, 0x00000000},
//        {1, ENABLE, DISABLE, DISABLE, 0x500, 0xFF},
//        {2, ENABLE, ENABLE, ENABLE, 0x0000, 0xFFFFFFFF},
//        {3, ENABLE, ENABLE, DISABLE, 0x0000, 0xFFFFFFFF},
//        {4, ENABLE, DISABLE, DISABLE, 0x200, 0x00000000},
//        {5, ENABLE, DISABLE, DISABLE, 0x300, 0x00000000},
        {6, ENABLE, DISABLE, DISABLE, 0x460, 0x0F},
//        {7, ENABLE, DISABLE, DISABLE, 0x207, 0x00000000},
//        {8, ENABLE, DISABLE, DISABLE, 0x208, 0x00000000},
//        {9, ENABLE, DISABLE, DISABLE, 0x209, 0x00000000},
//        {10, ENABLE, DISABLE, DISABLE, 0x50A, 0x00000000},
//        {11, ENABLE, DISABLE, DISABLE, 0x20B, 0x00000000},
//        {12, ENABLE, DISABLE, DISABLE, 0x20C, 0x00000000},
//        {13, ENABLE, DISABLE, DISABLE, 0x20D, 0x00000000},
//        {14, ENABLE, DISABLE, DISABLE, 0x20E, 0x00000000},
//        {15, ENABLE, DISABLE, DISABLE, 0x20F, 0x00000000},
};
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void CAN_Config(void);
void CAN_TX(void);


/********************************************************************************
* Function name 				: main
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/

int main(void)
{
    uint8_t t_count;
    PLL_Init();
    Delay_Init(72);    
//    UART0_Init(115200, 24000000);
    NVIC_EnableIRQ(CAN_IRQn);
    CAN_Config();
    CAN_TX();   
    while(1)
    {

    }
}

void CAN_Config(void)
{
//    CRG->CAN_CTRL &= ~((uint32_t)3<<30);        //CAN工作时钟选择：HSI 
    CRG->CAN_CTRL |= (uint32_t)3<<30;           //CAN工作时钟选择：AHB   
//    CRG->CAN_CTRL |= (uint32_t)2<<30;           //CAN工作时钟选择：APB   
//    CRG->CAN_CTRL |= (uint32_t)1<<30;           //CAN工作时钟选择：HSE 
    
    CRG->CKEN_CTRL1 |= 1<<4;                    //GPIOB时钟使能
    CRG->CAN_CTRL |= 1<<0;                      //CAN时钟使能
    CRG->CAN_CTRL &= ~(1<<1);                   //CAN软复位失能
   
    IOCFG->IOCFG_CTRLB |= (Bit0_En|Bit3_En);    //PB0 -> CAN_TX   PB1 -> CAN_RX        

    /*
    	tSeg1 = (S_SEG_1 + 2); tSeg2 = (S_SEG_2 + 1).
        BandRate 	= (36M / (S_PRESC + 1) / ((S_SEG_1 + 2) + (S_SEG_2 + 1)))
        SamplePoint = (tSeg1 / (tSeg1 + tSeg2)).
    
        tSeg2 >= SJW
    */
    
    //500Kb/s
//    CAN_InitStructure.CAN_WorkMode = CAN_FD_ISO;
//    CAN_InitStructure.SBitRateInitStructure.S_Presc = 5;
//    CAN_InitStructure.SBitRateInitStructure.S_Seg1 = 16;
//    CAN_InitStructure.SBitRateInitStructure.S_Seg2 = 5;
//    CAN_InitStructure.SBitRateInitStructure.S_SJW = 2;      
    
    //1Mb/s
    CAN_InitStructure.CAN_WorkMode = CAN_FD_ISO;
    CAN_InitStructure.SBitRateInitStructure.S_Presc = 5;
    CAN_InitStructure.SBitRateInitStructure.S_Seg1 = 7;
    CAN_InitStructure.SBitRateInitStructure.S_Seg2 = 2;
    CAN_InitStructure.SBitRateInitStructure.S_SJW = 1;  


    //1Mb/s
//    CAN_InitStructure.FBitRateInitStructure.F_Presc = 5;
//    CAN_InitStructure.FBitRateInitStructure.F_Seg1 = 7;
//    CAN_InitStructure.FBitRateInitStructure.F_Seg2 = 2;
//    CAN_InitStructure.FBitRateInitStructure.F_SJW = 1;  

    
    //4Mb/s
//    CAN_InitStructure.FBitRateInitStructure.F_Presc = 1;            //36M
//    CAN_InitStructure.FBitRateInitStructure.F_Seg1 = 5;
//    CAN_InitStructure.FBitRateInitStructure.F_Seg2 = 1;
//    CAN_InitStructure.FBitRateInitStructure.F_SJW = 2;  


//    //8Mb/s
    CAN_InitStructure.FBitRateInitStructure.F_Presc = 0;            //72M
    CAN_InitStructure.FBitRateInitStructure.F_Seg1 = 5;
    CAN_InitStructure.FBitRateInitStructure.F_Seg2 = 1;
    CAN_InitStructure.FBitRateInitStructure.F_SJW = 1;  

    CAN_InitStructure.Num_Filter = 2;
    CAN_InitStructure.FilterInitStructure = CAN_FilterInit;
    CAN_Init(&CAN_InitStructure);
}

void CAN_TX(void)
{
    //标准ID帧
//    TxMsg.ID = 0x666;
//    TxMsg.TTSEN = 1;
//    TxMsg.DLC = CAN_DLC_64BYTES;
//    TxMsg.BRS = 1;                              //变速
//    TxMsg.FDF = CANFD_FRAME;
//    TxMsg.IDE = CAN_ID_STD;
//    for(count = 0; count < 64; count++)
//        can_data[count] = count;
//    TxMsg.Data = can_data;
//    CAN_Transmit(&TxMsg, CAN_FD_ISO, STB);
    
    Delay_ms(10);
    
//    TxMsg.ID = 0x5000;
//    TxMsg.TTSEN = 1;
//    TxMsg.DLC = CAN_DLC_64BYTES;
//    TxMsg.BRS = 1;                              
//    TxMsg.FDF = CANFD_FRAME;
//    TxMsg.IDE = CAN_ID_EXT;
//    for(count = 0; count < 64; count++)
//        can_data[count] = count;
//    TxMsg.Data = can_data;
//    CAN_Transmit(&TxMsg, CAN_FD_ISO, STB);
    
    TxMsg.ID = 0x700;
    TxMsg.TTSEN = 1;
    TxMsg.DLC = 1;
    TxMsg.BRS = 1;                              //变速
    TxMsg.FDF = CANFD_FRAME;
    TxMsg.IDE = CAN_ID_STD;
    can_data[0] = 0xAA;
    TxMsg.Data = can_data;
    CAN_Transmit(&TxMsg, CAN_FD_ISO, STB);      
}













