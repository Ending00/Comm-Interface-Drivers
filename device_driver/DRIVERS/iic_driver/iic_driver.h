// iic_driver.h
#ifndef iic_DRIVER_H
#define iic_DRIVER_H

#include "stdint.h"
#include "stdbool.h"
#include <stdlib.h>
#include "iic_driver_pal.h"

#define sys_clock          72000000
// IIC 配置结构体
typedef struct 
{
    uint32_t frequency;  // IIC 频率
    uint8_t device_address;  //设备地址
    uint32_t delay_cycles;     // 根据频率计算的延时周期
}IIC_Config_t;


uint8_t bsp_analog_i2c_read_bytes(uint8_t slave_addr, uint8_t reg_addr, uint8_t* data, uint16_t length);
uint8_t bsp_analog_i2c_write_bytes(uint8_t slave_addr, uint8_t reg_addr, uint8_t* data, uint16_t length);
uint8_t bsp_analog_i2c_write_bytes_16bit_reg(uint8_t slave_addr, uint16_t reg_addr, uint8_t* data, uint16_t length);
uint8_t bsp_analog_i2c_read_bytes_16bit_reg(uint8_t slave_addr, uint16_t reg_addr, uint8_t* data, uint16_t length);
void iic_init(void);
void iic_delay(void);
#endif // USART_DRIVER_H

