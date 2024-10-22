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
    uint16_t t_count;
//    HSI_Init();
    PLL_Init();     //ahb 18m  apb 18m
    Delay_Init(72);
//    for(t_count=0; t_count<70; t_count++)
//        Delay_ms(100);
    NVIC_EnableIRQ(CAN_IRQn);


//    UART0_Init(115200, 24000000);
    CAN_Config();
    CAN_TX();
    while(1)
    {


    }
}

void CAN_Config(void)
{
    CAN_FilterTypeDef CAN_FilterInit[16] =
    {
        {0, DISABLE, DISABLE, DISABLE, 0x204, 0x00000000},
//        {1, ENABLE, DISABLE, DISABLE, 0x500, 0xFF},
//        {2, ENABLE, ENABLE, ENABLE, 0x0000, 0xFFFFFFFF},
        {3, ENABLE, ENABLE, DISABLE, 0x0000, 0xFFFFFFFF},
//        {4, ENABLE, DISABLE, DISABLE, 0x200, 0x00000000},
//        {5, ENABLE, DISABLE, DISABLE, 0x300, 0x00000000},
//        {6, ENABLE, DISABLE, DISABLE, 0x460, 0x0F},
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

//    CRG->CAN_CTRL &= ~((uint32_t)3<<30);        //CAN工作时钟选择：HSI
    CRG->CAN_CTRL |= (uint32_t)3<<30;           //CAN工作时钟选择：AHB
//    CRG->CAN_CTRL |= (uint32_t)2<<30;           //CAN工作时钟选择：APB
//    CRG->CAN_CTRL |= (uint32_t)1<<30;           //CAN工作时钟选择：HSE

    CRG->CKEN_CTRL1 |= 1<<4;                    //GPIOB时钟使能
    CRG->CAN_CTRL |= 1<<0;                      //CAN时钟使能
    CRG->CAN_CTRL &= ~(1<<1);                   //CAN软复位失能

    IOCFG->IOCFG_CTRLB |= (Bit0_En|Bit3_En);    //PB0 -> CAN_TX   PB1 -> CAN_RX

    CAN_InitStructure.CAN_WorkMode = CAN_20B;
    CAN_InitStructure.SBitRateInitStructure.S_Presc = 5;
    CAN_InitStructure.SBitRateInitStructure.S_Seg1 = 7;
    CAN_InitStructure.SBitRateInitStructure.S_Seg2 = 2;
    CAN_InitStructure.SBitRateInitStructure.S_SJW = 1;

    CAN_InitStructure.FilterInitStructure = CAN_FilterInit;
    CAN_Init(&CAN_InitStructure);
}

void CAN_TX(void)
{
    //标准ID帧
    TxMsg.ID = 0x540;
    TxMsg.TTSEN = 1;
    TxMsg.DLC = 8;
    TxMsg.BRS = 0;                              //nominal /  slow bit rate for the complete frame
    TxMsg.FDF = CAN_FRAME;
    TxMsg.RTR = 0;                              //Data Frame
    TxMsg.IDE = CAN_ID_STD;
    for(count = 0; count < TxMsg.DLC; count++)
        can_data[count] = count;
    TxMsg.Data = can_data;
    CAN_Transmit(&TxMsg, CAN_20B);

//    Delay_ms(10);

    //扩展ID帧
//    TxMsg.ID = 0x5400;
//    TxMsg.TTSEN = 1;
//    TxMsg.DLC = 8;
//    TxMsg.BRS = 0;                              //nominal /  slow bit rate for the complete frame
//    TxMsg.FDF = CAN_FRAME;
//    TxMsg.RTR = 0;                              //Data Frame
//    TxMsg.IDE = CAN_ID_EXT;
//    for(count = 0; count < TxMsg.DLC; count++)
//        can_data[count] = count;
//    TxMsg.Data = can_data;
//    CAN_Transmit(&TxMsg, CAN_20B);

    //标准ID遥控帧
//    TxMsg.ID = 0x123;
//    TxMsg.TTSEN = 1;
//    TxMsg.DLC = 8;
//    TxMsg.BRS = 0;                              //nominal /  slow bit rate for the complete frame
//    TxMsg.FDF = CAN_FRAME;
//    TxMsg.RTR = CAN_RTR_REMOTE;                 //Remote Frame
//    TxMsg.IDE = CAN_ID_STD;
//    for(count = 0; count < TxMsg.DLC; count++)
//        can_data[count] = count;
//    TxMsg.Data = can_data;
//    CAN_Transmit(&TxMsg, CAN_20B);


    //扩展ID遥控帧
//    TxMsg.ID = 0x1234;
//    TxMsg.TTSEN = 1;
//    TxMsg.DLC = 8;
//    TxMsg.BRS = 0;                              //nominal /  slow bit rate for the complete frame
//    TxMsg.FDF = CAN_FRAME;
//    TxMsg.RTR = CAN_RTR_REMOTE;                 //Remote Frame
//    TxMsg.IDE = CAN_ID_EXT;
//    for(count = 0; count < TxMsg.DLC; count++)
//        can_data[count] = count;
//    TxMsg.Data = can_data;
//    CAN_Transmit(&TxMsg, CAN_20B);
}













