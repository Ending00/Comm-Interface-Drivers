#include "sys.h"
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
#include "usart_driver.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"					 
#endif

#if 1
#pragma import(__use_no_semihosting)             
               
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
   
void _sys_exit(int x) 
{ 
	x = x; 
} 

void _ttywrch(int ch) {
    // 空实现，什么都不做
}

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

#if EN_USART1_RX   

u8 USART_RX_BUF[USART_REC_LEN];    

u16 USART_RX_STA=0;         

u8 aRxBuffer[RXBUFFERSIZE];
UART_HandleTypeDef UART1_Handler; 
  

void uart_init(u32 bound)
{	
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    
	HAL_UART_Init(&UART1_Handler);					   
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//?ú????????????ж?????λUART_IT_RXNE??????????????????????????????????????
  
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO???????
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//????????1?????д???1 MSP?????
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//???GPIOA???
		__HAL_RCC_USART1_CLK_ENABLE();			//???USART1???
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//???????????
		GPIO_Initure.Pull=GPIO_PULLUP;			//????
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//????
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//?????PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//????????????????????	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//?????PA10
		
#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//???USART1?ж????
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//????????3?????????3
#endif	
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static uint8_t prevByte = 0x00;  // 保存前一个字节
	if(huart->Instance==USART1)
	{
        // 将接收到的数据写入环形缓冲区		
        RingBuffer_Write(&usartRxBuffer, aRxBuffer[0]);		
        if (prevByte == 0x55 && aRxBuffer[0] == 0xAA)  // 检测帧尾 0x55AA
        {
            // 收到帧尾，开始处理完整的帧
			state = FRAME_STATE_DONE;
        }
        // 更新前一个字节
        prevByte = aRxBuffer[0];
	}
}
 
//????1?ж???????
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
#if SYSTEM_SUPPORT_OS	 	//???OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//????HAL???ж???????ú???
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//???????
	{
	 timeout++;////???????
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//??δ?????????????????ж??????RxXferCount?1
	{
	 timeout++; //???????
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//???OS
	OSIntExit();  											 
#endif
} 
#endif	

/*????????????????ж???????д???ж???????????*/

//????1?ж???????
//void USART1_IRQHandler(void)                	
//{ 
//	u8 Res;
//	HAL_StatusTypeDef err;
//#if SYSTEM_SUPPORT_OS	 	//???OS
//	OSIntEnter();    
//#endif
//	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //?????ж?(????????????????0x0d 0x0a??β)
//	{
//		Res=USART1->DR; 
//		if((USART_RX_STA&0x8000)==0)//????δ???
//		{
//			if(USART_RX_STA&0x4000)//???????0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//???????,??????
//				else USART_RX_STA|=0x8000;	//????????? 
//			}
//			else //??????0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//???????????,??????????	  
//				}		 
//			}
//		}   		 
//	}
//	HAL_UART_IRQHandler(&UART1_Handler);	
//#if SYSTEM_SUPPORT_OS	 	//???OS
//	OSIntExit();  											 
//#endif
//} 
//#endif	


