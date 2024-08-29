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
#include "rjs32k116.h"
#include <stdio.h>
#include "crg.h"
#include "delay.h"
#include "uart.h"
#include "can.h"
/* Private variables ---------------------------------------------------------*/
uint8_t count = 0, can_data[100] = {0x00};
uint8_t test_data[0x08] = {0x00, 0xA5, 0x5A, 0x11, 0x22, 0x33, 0x44, 0x55};
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

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
     PLL_Init();     
    Delay_Init(24);
//    for(t_count=0; t_count<70; t_count++)
//        Delay_ms(100);
    NVIC_EnableIRQ(UART0_IRQn);
    

    LIN_Init(20000, 72000000);

    while(1)
    {

       
    }
}














