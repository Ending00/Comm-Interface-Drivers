;*******************************************************************************
;* File Name          : startup_rjs32k116.s
;* Author             : Wuhan Runjet Software And Testing Department
;* Description        : RJS32K116 Ultra Low Power devices vector table for MDK-ARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the Cortex-M4 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;*******************************************************************************
;*
;* <h2><center>&copy Copyright (c) 2022 Wuhan Runjet.
;* All rights reserved.</center></h2>
;*
;* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
;* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
;* TIME. AS A RESULT, Wuhan Runjet SHALL NOT BE HELD LIABLE FOR ANY DIRECT, 
;* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM 
;* THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING 
;* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;* 
;*******************************************************************************
;
; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size		EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size      	EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     PVD_IRQHandler            ; 0:	PVD
                DCD     IWDG_IRQHandler           ; 1:  IWDG
                DCD     0            			        ; 2:  Reserved
                DCD     I2C0_IRQHandler           ; 3:  I2C0
                DCD     I2C1_IRQHandler           ; 4:  I2C1
                DCD     SPI0_IRQHandler           ; 5:  SPI0
                DCD     SPI1_IRQHandler           ; 6:  SPI1
                DCD     UART0_IRQHandler          ; 7:  UART0
                DCD     UART1_IRQHandler          ; 8:  UART1
                DCD     UART2_IRQHandler          ; 9:  UART2
                DCD     UART3_IRQHandler          ; 10: UART3
                DCD     0                         ; 11: Reserved
                DCD     0                         ; 12: Reserved
                DCD     LPUART_IRQHandler         ; 13: LPUART
                DCD     RTC_IRQHandler            ; 14: RTC
                DCD     GPIOA_IRQHandler          ; 15: GPIOA
                DCD     GPIOB_IRQHandler          ; 16: GPIOB
                DCD     GPIOC_IRQHandler          ; 17: GPIOC
                DCD     GPIOH_IRQHandler          ; 18: GPIOH
                DCD     TIMER0_IRQHandler         ; 19: TIMER0
                DCD     TIMER1_IRQHandler         ; 20: TIMER1
                DCD     TIMER2_IRQHandler         ; 21: TIMER2
                DCD     TIMER3_IRQHandler         ; 22: TIMER3
                DCD     TIMER4_IRQHandler         ; 23: TIMER4
                DCD     LPTIMER_IRQHandler        ; 24: LPTIMER
                DCD     WDG_IRQHandler            ; 25: WDG
                DCD     ADC_IRQHandler            ; 26: ADC
                DCD     CLKCHK_IRQHandler         ; 27: CLKCHK_TRIM
                DCD     COMP1COMP2_IRQHandler     ; 28: COMP1|COMP2
                DCD     CAN_IRQHandler            ; 29: CAN
                DCD     SRAM_ERROR_IRQHandler     ; 30: SRAM_ERROR
                DCD     DMA_IRQHandler     		    ; 31: DMA	

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
        
        IMPORT  __main

                LDR     R0, =__main
                BX      R0
                ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                    [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler              [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                    [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                 [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                [WEAK]
                B       .
                ENDP

Default_Handler PROC
				EXPORT     PVD_IRQHandler          [WEAK]    
				EXPORT     IWDG_IRQHandler         [WEAK]
				EXPORT     I2C0_IRQHandler         [WEAK]
				EXPORT     I2C1_IRQHandler         [WEAK]
				EXPORT     SPI0_IRQHandler         [WEAK]
				EXPORT     SPI1_IRQHandler         [WEAK]
				EXPORT	   UART0_IRQHandler        [WEAK]
				EXPORT	   UART1_IRQHandler        [WEAK]
				EXPORT	   UART2_IRQHandler        [WEAK]
				EXPORT	   UART3_IRQHandler        [WEAK]
				EXPORT	   LPUART_IRQHandler       [WEAK]
				EXPORT	   RTC_IRQHandler          [WEAK]
				EXPORT	   GPIOA_IRQHandler        [WEAK]
				EXPORT	   GPIOB_IRQHandler        [WEAK]
				EXPORT	   GPIOC_IRQHandler        [WEAK]
				EXPORT	   GPIOH_IRQHandler        [WEAK]
				EXPORT	   TIMER0_IRQHandler       [WEAK]
				EXPORT	   TIMER1_IRQHandler       [WEAK]
				EXPORT	   TIMER2_IRQHandler       [WEAK]
				EXPORT	   TIMER3_IRQHandler       [WEAK]
				EXPORT	   TIMER4_IRQHandler       [WEAK]
				EXPORT	   LPTIMER_IRQHandler      [WEAK]
				EXPORT	   WDG_IRQHandler          [WEAK]
				EXPORT	   ADC_IRQHandler          [WEAK]
				EXPORT	   CLKCHK_IRQHandler       [WEAK]
				EXPORT	   COMP1COMP2_IRQHandler   [WEAK]
				EXPORT	   CAN_IRQHandler          [WEAK]
				EXPORT	   SRAM_ERROR_IRQHandler   [WEAK]
				EXPORT	   DMA_IRQHandler    	     [WEAK]
					
PVD_IRQHandler             
IWDG_IRQHandler                        
I2C0_IRQHandler            
I2C1_IRQHandler            
SPI0_IRQHandler          
SPI1_IRQHandler         
UART0_IRQHandler        
UART1_IRQHandler         
UART2_IRQHandler          
UART3_IRQHandler        
LPUART_IRQHandler       
RTC_IRQHandler          
GPIOA_IRQHandler        
GPIOB_IRQHandler         
GPIOC_IRQHandler         
GPIOH_IRQHandler      
TIMER0_IRQHandler      
TIMER1_IRQHandler        
TIMER2_IRQHandler      
TIMER3_IRQHandler       
TIMER4_IRQHandler        
LPTIMER_IRQHandler       
WDG_IRQHandler      
ADC_IRQHandler         
CLKCHK_IRQHandler         
COMP1COMP2_IRQHandler       
CAN_IRQHandler       
SRAM_ERROR_IRQHandler   
DMA_IRQHandler 

                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB

                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit

                 ELSE

                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap

__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

;************************ (C) COPYRIGHT Wuhan Runjet *****END OF FILE*****
