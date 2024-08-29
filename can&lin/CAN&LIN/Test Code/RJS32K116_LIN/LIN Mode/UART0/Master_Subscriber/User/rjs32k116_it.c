/**
  ******************************************************************************
  * @file    RJS32K116_it.c
	* @author  MCU SYSTEM Team
	* @Version V1.0.0
  * @Date    2022-09-20
  * @brief   SystemInit function
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rjs32k116.h"
#include "rjs32k116_it.h"

/* Private variables ---------------------------------------------------------*/
uint8 in_count = 0, out_count = 0;
uint8 data_buff[0x20] = {0x00};
uint8 lin_status = 0;


/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/********************************************************************************
* Function name 				: SysTick_Handler
* Description    				: 
* Input			          	: 
* Output      		    	: 
* return   							:
********************************************************************************/
void SysTick_Handler(void)
{

}

/********************************************************************************
* Function name 				: PVD_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
/**/
void PVD_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: IWDG_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
/**/
void IWDG_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: I2C_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
/**/
void I2C0_IRQHandler(void)
{

}

void I2C1_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: SPI_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
/**/
void SPI0_IRQHandler(void)
{

}

void SPI1_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: UART_IRQHandler
* Description    				: HSE
* Input			             	:
* Output      		    	    :
********************************************************************************/
/**/
void UART0_IRQHandler(void)
{
    if((UART0->UART_CSR&Bit13_En) == Bit13_En)
    {
        lin_status = 1;
    }
    if(((UART0->UART_CSR & Bit14_En) == Bit14_En) & (lin_status == 1))
    {
        if(UART0->UART_LINIR == 0x3D)
        {
//            UART0_LINMR |= Bit0_En;                   //收听节点
            lin_status = 2;
        }  
    }
    if(((UART0->UART_CSR & Bit0_En) == Bit0_En) & (lin_status == 2))
    {
        data_buff[in_count++] = (uint8) (UART0->UART_RHR & 0xFF);
    } 
    UART0->UART_CR = Bit8_En;        //状态位复位      
}

void UART1_IRQHandler(void)
{

}

void UART2_IRQHandler(void)
{

}

void UART3_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: LPUART_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
/**/
void LPUART_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: RTC_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void RTC_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: GPIO_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void GPIOA_IRQHandler(void)
{

}

void GPIOB_IRQHandler(void)
{

}

void GPIOC_IRQHandler(void)
{

}

void GPIOH_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: TIMER_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void TIMER0_IRQHandler(void)
{

}


void TIMER1_IRQHandler(void)
{

}

void TIMER2_IRQHandler(void)
{

}

void TIMER3_IRQHandler(void)
{

}

void TIMER4_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: LPTIMER_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void LPTIMER_IRQHandler(void)
{

}


/********************************************************************************
* Function name 				: WDG_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void WDG_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: ADC_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void ADC_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: CLKCHK_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void CLKCHK_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: CLKCHK_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void COMP1COMP2_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: CAN_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void CAN_IRQHandler(void)
{

}


/********************************************************************************
* Function name 				: SRAM_ERROR_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void SRAM_ERROR_IRQHandler(void)
{

}

/********************************************************************************
* Function name 				: DMA_IRQHandler
* Description    				:
* Input			             	:
* Output      		    	    :
********************************************************************************/
void DMA_IRQHandler(void)
{

}

















