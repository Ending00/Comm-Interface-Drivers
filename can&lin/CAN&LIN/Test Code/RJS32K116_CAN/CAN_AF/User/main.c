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
    {0, ENABLE, DISABLE, DISABLE, 0x200, 0x1FFFFFFF},
//    {1, ENABLE, ENABLE, DISABLE, 0x201, 0x1FFFFC00},
//    {2, ENABLE, ENABLE, DISABLE, 0x202, 0x1FFFFC00},
//    {3, ENABLE, ENABLE, DISABLE, 0x203, 0x1FFFFC00},
//    {4, ENABLE, ENABLE, DISABLE, 0x204, 0x1FFFFC00},
//    {5, ENABLE, ENABLE, DISABLE, 0x205, 0x1FFFFC00},
//    {6, ENABLE, ENABLE, DISABLE, 0x206, 0x1FFFFC00},
//    {7, ENABLE, ENABLE, DISABLE, 0x207, 0x1FFFFC00},
//    {8, ENABLE, ENABLE, DISABLE, 0x208, 0x1FFFFC00},
//    {9, ENABLE, ENABLE, DISABLE, 0x209, 0x1FFFFC00},
//    {10, ENABLE, ENABLE, DISABLE, 0x20A, 0x1FFFFC00},
//    {11, ENABLE, ENABLE, DISABLE, 0x20B, 0x1FFFFC00},
//    {12, ENABLE, ENABLE, DISABLE, 0x20C, 0x1FFFFC00},
//    {13, ENABLE, ENABLE, DISABLE, 0x20D, 0x1FFFFC00},
//    {14, ENABLE, ENABLE, DISABLE, 0x20E, 0x1FFFFC00},
//    {15, ENABLE, ENABLE, DISABLE, 0x20F, 0x1FFFFC00},
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
//    CRG->CAN_CTRL &= ~((uint32_t)3<<30);        //CAN工作时钟选择：HSI 
    CRG->CAN_CTRL |= (uint32_t)3<<30;           //CAN工作时钟选择：AHB   
//    CRG->CAN_CTRL |= (uint32_t)2<<30;           //CAN工作时钟选择：APB   
//    CRG->CAN_CTRL |= (uint32_t)1<<30;           //CAN工作时钟选择：HSE 
    
    CRG->CKEN_CTRL1 |= 1<<4;                    //GPIOB时钟使能
    CRG->CAN_CTRL |= 1<<0;                      //CAN时钟使能
    CRG->CAN_CTRL &= ~(1<<1);                   //CAN软复位失能
   
//    IOCFG->IOCFG_CTRLB |= (Bit0_En|Bit3_En);    //PB0 -> CAN_TX   PB1 -> CAN_RX   

//    IOCFG->IOCFG_CTRLH |= 2<<0;                 //PH0->CAN_TX
//    IOCFG->IOCFG_CTRLH |= 2<<3;                 //PH1->CAN_RX
    
//    IOCFG->IOCFG_CTRLH |= 2<<12;                //PH4->CAN_TX
//    IOCFG->IOCFG_CTRLH |= 2<<15;                //PH5->CAN_RX

//    IOCFG->IOCFG_CTRLK |= 3<<6;                 //PH10->CAN_TX
//    IOCFG->IOCFG_CTRLK |= 3<<9;                 //PH11->CAN_RX 

//    IOCFG->IOCFG_CTRLJ |= 2<<0;                 //PA10->CAN_TX
//    IOCFG->IOCFG_CTRLJ |= 2<<3;                 //PA11->CAN_RX      
//    
    IOCFG->IOCFG_CTRLC |= 2<<24;                //PC8->CAN_TX
    IOCFG->IOCFG_CTRLC |= 2<<27;                //PC9->CAN_RX       
    
    //1Mb/s
    CAN_InitStructure.CAN_WorkMode = CAN_20B;
    CAN_InitStructure.SBitRateInitStructure.S_Presc = 5;
    CAN_InitStructure.SBitRateInitStructure.S_Seg1 = 7;
    CAN_InitStructure.SBitRateInitStructure.S_Seg2 = 2;
    CAN_InitStructure.SBitRateInitStructure.S_SJW = 1;
    
    CAN_InitStructure.Num_Filter = 1;
    CAN_InitStructure.FilterInitStructure = CAN_FilterInit;
    CAN_Init(&CAN_InitStructure);
}

void CAN_TX(void)
{
    //标准ID帧
    TxMsg.ID = 0x500;
    TxMsg.TTSEN = 1;
    TxMsg.DLC = 8;
    TxMsg.BRS = 0;                              //nominal /  slow bit rate for the complete frame
    TxMsg.FDF = CAN_FRAME;
    TxMsg.RTR = 0;                              //Data Frame
    TxMsg.IDE = CAN_ID_STD;
    for(count = 0; count < TxMsg.DLC; count++)
        can_data[count] = count;
    TxMsg.Data = can_data;
    CAN_Transmit(&TxMsg, CAN_20B, STB);
    
    Delay_ms(10);
    
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













