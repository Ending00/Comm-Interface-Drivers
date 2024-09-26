/*
 * @Author: sven
 * @Date: 2024-09-23 15:32:14
 * @Last Modified by: sven
 * @Last Modified time: Do not edit
 * @FilePath: \device_driver\device_driver\TESTAPP\iic_driver_pal.c
 * @Description: This file contains the implementation of a universal driver designed for flexibility and portability across various microcontroller platforms.
 * @Version: 1.0
 */
#include "iic_driver_pal.h"

gpio_pin_t PIN_IICSCL;
gpio_pin_t PIN_IICSDA;

void PORT_SWD_PORT_CHANGE(gpio_pin_t *scl, gpio_pin_t *sda)
{
    PIN_IICSCL.port = scl->port;
    PIN_IICSCL.pin = scl->pin;
 
    PIN_IICSDA.port = sda->port;
    PIN_IICSDA.pin = sda->pin;
}

/*设置SCL引脚为高或低电平，根据硬件平台GPIO库实现*/
void IIC_SET_SCL(GPIO_PinState state)
{
    HAL_GPIO_WritePin(PIN_IICSCL.port,PIN_IICSCL.pin,state);
}

/*设置SDA引脚为高或低电平，根据硬件平台GPIO库实现*/
void IIC_SET_SDA(GPIO_PinState state)
{
    HAL_GPIO_WritePin(PIN_IICSDA.port,PIN_IICSDA.pin,state);
}

/*获取SDA线的状态*/
bool IIC_GET_SDA(void)
{
    return HAL_GPIO_ReadPin(PIN_IICSDA.port,PIN_IICSDA.pin);
}

/*获取SCL线的状态*/
bool IIC_GET_SCL(void)
{
    return HAL_GPIO_ReadPin(PIN_IICSCL.port,PIN_IICSCL.pin);
}

void app_iic_gpio_config(void)
{
    gpio_pin_t scl;
    gpio_pin_t sda;    
    
    scl.port = GPIOC;
    scl.pin = GPIO_PIN_1;
    sda.port = GPIOC;
    sda.pin = GPIO_PIN_4;
	
	PORT_SWD_PORT_CHANGE(&scl, &sda);
}

#define size 8
uint8_t test_buff[size],rx_buff[size];

void iic_driver_test(void)
{
    GPIO_InitTypeDef GPIO_Initure;   
    __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟

    app_iic_gpio_config();
    iic_init();
    //PC1,4初始化设置
    GPIO_Initure.Pin = (PIN_IICSCL.pin|PIN_IICSDA.pin);
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;//高速
    HAL_GPIO_Init(PIN_IICSCL.port,&GPIO_Initure);
//    HAL_GPIO_Init(PIN_IICSDA.port,&GPIO_Initure);

    IIC_SET_SDA(GPIO_PIN_SET);
    IIC_SET_SCL(GPIO_PIN_SET);

	for(uint8_t i=0;i<size;i++)
	{
		test_buff[i] = i;
	}
    while(1)
    {
        bsp_analog_i2c_write_bytes(0x50,0x00,test_buff,size);
		delay_ms(10);	
		bsp_analog_i2c_read_bytes(0x50,0x00,rx_buff,size);
		delay_ms(10);	
		
//		bsp_analog_i2c_write_bytes_16bit_reg(0x50,0x0000,test_buff,size);		
//		delay_ms(10);
//		bsp_analog_i2c_read_bytes_16bit_reg(0x50,0x0000,rx_buff,size);		
//		delay_ms(10);		
		
	}
}



