/*
 * @Author: sven
 * @Date: 2024-09-20 10:58:01
 * @Last Modified by: sven
 * @Last Modified time: Do not edit
 * @FilePath: \device_driver\device_driver\DRIVERS\iic_driver\iic_driver.c
 * @Description: This file contains the implementation of a universal driver designed for flexibility and portability across various microcontroller platforms.
 * @Version: 1.0
 */

#include "iic_driver.h"

static IIC_Config_t IIC_Config;
/**
 * @brief 根据频率计算延时
 * @retval none
 * @author sven
 * @date 2024-9-23
 */
static uint32_t calculate_delay_cycles(uint32_t frequency)
{
    return ((sys_clock/72) / (frequency));  // 每个SCL周期包括上升和下降沿
}

/**
 * @brief 模拟I2C延时
 * @retval none
 * @author sven
 * @date 2024-9-23
 */
void iic_delay(void)
{
    volatile uint32_t i = IIC_Config.delay_cycles;
    while(i--) {}  // 简单的空循环延时
}

/**
 * @brief IIC配置初始化
 * @retval none
 * @author sven
 * @date 2024-9-23
 */
void iic_init(void)
{
    IIC_Config.frequency = 400000; //400K 具体时间根据
    //IIC_Config.device_address = 0x11; //slave adr
    IIC_Config.delay_cycles = calculate_delay_cycles(IIC_Config.frequency); 
//	IIC_Config.delay_cycles = 72/400;
}

/**
 * @brief I2C 开始,SCL为高电平的时候SDA产生一个下降沿信号
 * @retval none
 * @author sven
 * @date 2024-9-23
 */
void bsp_analog_i2c_start(void)
{
    /*    _____
     *SDA      \_____________
     *    __________
     *SCL           \________
     */
    IIC_SET_SDA(GPIO_PIN_SET);
    IIC_SET_SCL(GPIO_PIN_SET);
    iic_delay();
    IIC_SET_SDA(GPIO_PIN_RESET);;
    iic_delay();
    IIC_SET_SCL(GPIO_PIN_RESET);;
    iic_delay();
}

/**
 * @brief I2C 停止,SCL为高电平的时候SDA产生一个上升沿信号
 * @retval none
 * @author sven
 * @date 2024-9-23
 */
void bsp_analog_i2c_stop(void)
{
    /*               _______
     *SDA __________/
     *          ____________
     *SCL _____/
     */
    IIC_SET_SDA(GPIO_PIN_RESET);;
    IIC_SET_SCL(GPIO_PIN_SET);
    iic_delay();
    IIC_SET_SDA(GPIO_PIN_SET);
    iic_delay();
}

/**
 * @brief I2C 等待响应
 * @retval none
 * @author sven
 * @date 2024-9-23
 */
uint8_t bsp_analog_i2c_wait_ack(void)
{
    uint32_t timeout = 0;

    IIC_SET_SDA(GPIO_PIN_SET);
    iic_delay();
    IIC_SET_SCL(GPIO_PIN_SET);
    iic_delay();
    while(IIC_GET_SDA())
    {
        timeout++;
        if(timeout > 2000)
        {
            return 0;
        }
    }
    IIC_SET_SCL(GPIO_PIN_RESET);;
    iic_delay();
    return 1;
}

/**
 * @brief I2C 响应
 * @retval none
 * @author sven
 * @date 2024-9-23
 */
void bsp_analog_i2c_ack(void)
{
    /*           ____
     *SCL ______/    \______
     *    ____         _____
     *SDA     \_______/
     */
    IIC_SET_SDA(GPIO_PIN_RESET);;
    iic_delay();
    IIC_SET_SCL(GPIO_PIN_SET);;
    iic_delay();
    IIC_SET_SCL(GPIO_PIN_RESET);;
    iic_delay();
    IIC_SET_SDA(GPIO_PIN_SET);;
}

/**
 * @brief I2C 不响应
 * @retval none
 * @author sven
 * @date 2024-9-23
 */
void bsp_analog_i2c_nack(void)
{
    /*           ____
     *SCL ______/    \______
     *    __________________
     *SDA
     */
    IIC_SET_SDA(GPIO_PIN_SET);;
    iic_delay();
    IIC_SET_SCL(GPIO_PIN_SET);;
    iic_delay();
    IIC_SET_SCL(GPIO_PIN_RESET);;
    iic_delay();
}

/**
 * @brief I2C 发送一个字节数据
 * @retval none
 * @author sven
 * @date 2024-9-23
 */
void bsp_analog_i2c_send_byte(uint8_t data)
{
    uint8_t i;

    for(i = 0; i < 8; i++)
    {
        if(data & 0x80)
        {
           IIC_SET_SDA(GPIO_PIN_SET);;
        }
        else
        {
            IIC_SET_SDA(GPIO_PIN_RESET);;
        }

        iic_delay();
        IIC_SET_SCL(GPIO_PIN_SET);;
        iic_delay();
        IIC_SET_SCL(GPIO_PIN_RESET);;
        if(i == 7)
        {
            IIC_SET_SDA(GPIO_PIN_SET);;
        }
        data <<= 1;
        iic_delay();
    }
}

/**
 * @brief I2C 读一个字节数据
 * @retval none
 * @author sven
 * @date 2024-9-23
 */
uint8_t bsp_analog_i2c_read_byte(void)
{
    uint8_t i, data = 0;

    for(i = 0; i < 8; i++ )
    {
        data <<= 1;
        IIC_SET_SCL(GPIO_PIN_SET);;
        iic_delay();
        if(IIC_GET_SDA())
        {
            data++;
        }
        IIC_SET_SCL(GPIO_PIN_RESET);;
        iic_delay();
    }

    return data;
}

/**
 * @brief 写入从机多字节数据
 * @param slave_addr 从机地址
 * @param reg_addr   寄存器起始地址
 * @param data       需要写入的数据指针
 * @param length     数据长度
 * @retval 0 失败, 1 成功
 */
uint8_t bsp_analog_i2c_write_bytes(uint8_t slave_addr, uint8_t reg_addr, uint8_t* data, uint16_t length)
{
    bsp_analog_i2c_start();  // 发送起始信号

    bsp_analog_i2c_send_byte((slave_addr << 1) | 0); // 发送从机地址 + 写命令
    if (!bsp_analog_i2c_wait_ack()) // 等待应答
    {
        bsp_analog_i2c_stop();
        return 0;
    }

    bsp_analog_i2c_send_byte(reg_addr); // 发送寄存器起始地址
    if (!bsp_analog_i2c_wait_ack()) // 等待应答
    {
        bsp_analog_i2c_stop();
        return 0;
    }

    // 依次发送每个数据字节
    for(uint16_t i = 0; i < length; i++)
    {
        bsp_analog_i2c_send_byte(data[i]);
        if (!bsp_analog_i2c_wait_ack()) // 等待应答
        {
            bsp_analog_i2c_stop();
            return 0;
        }
    }

    bsp_analog_i2c_stop();  // 发送停止信号
    return 1;
}

/**
 * @brief 从从机读取多字节数据
 * @param slave_addr 从机地址
 * @param reg_addr   寄存器起始地址
 * @param data       存放读取数据的缓冲区指针
 * @param length     读取的数据长度
 * @retval 0 失败, 1 成功
 */
uint8_t bsp_analog_i2c_read_bytes(uint8_t slave_addr, uint8_t reg_addr, uint8_t* data, uint16_t length)
{
    bsp_analog_i2c_start();  // 发送起始信号

    bsp_analog_i2c_send_byte((slave_addr << 1) | 0); // 发送从机地址 + 写命令
    if (!bsp_analog_i2c_wait_ack()) // 等待应答
    {
        bsp_analog_i2c_stop();
        return 0;
    }

    bsp_analog_i2c_send_byte(reg_addr); // 发送寄存器起始地址
    if (!bsp_analog_i2c_wait_ack()) // 等待应答
    {
        bsp_analog_i2c_stop();
        return 0;
    }

    bsp_analog_i2c_start();  // 再次发送起始信号
    bsp_analog_i2c_send_byte((slave_addr << 1) | 1); // 发送从机地址 + 读命令
    if (!bsp_analog_i2c_wait_ack()) // 等待应答
    {
        bsp_analog_i2c_stop();
        return 0;
    }

    // 依次读取每个字节数据
    for(uint16_t i = 0; i < length; i++)
    {
        data[i] = bsp_analog_i2c_read_byte();
        if(i == length - 1)
        {
            bsp_analog_i2c_nack();  // 最后一个字节后发送NACK
        }
        else
        {
            bsp_analog_i2c_ack();   // 发送ACK以继续接收
        }
    }

    bsp_analog_i2c_stop();  // 发送停止信号
    return 1;
}

/**
 * @brief 写入从机多字节数据（支持16位寄存器地址）
 * @param slave_addr 从机地址
 * @param reg_addr   16位寄存器起始地址
 * @param data       需要写入的数据指针
 * @param length     数据长度
 * @retval 0 失败, 1 成功
 */
uint8_t bsp_analog_i2c_write_bytes_16bit_reg(uint8_t slave_addr, uint16_t reg_addr, uint8_t* data, uint16_t length)
{
    bsp_analog_i2c_start();  // 发送起始信号

    bsp_analog_i2c_send_byte((slave_addr << 1) | 0); // 发送从机地址 + 写命令
    if (!bsp_analog_i2c_wait_ack()) // 等待应答
    {
        bsp_analog_i2c_stop();
        return 0;
    }

    // 发送16位寄存器地址（先高字节，再低字节）
    bsp_analog_i2c_send_byte((reg_addr >> 8) & 0xFF);  // 发送寄存器地址高字节
    if (!bsp_analog_i2c_wait_ack())
    {
        bsp_analog_i2c_stop();
        return 0;
    }
    
    bsp_analog_i2c_send_byte(reg_addr & 0xFF);  // 发送寄存器地址低字节
    if (!bsp_analog_i2c_wait_ack())
    {
        bsp_analog_i2c_stop();
        return 0;
    }

    // 依次发送每个数据字节
    for(uint16_t i = 0; i < length; i++)
    {
        bsp_analog_i2c_send_byte(data[i]);
        if (!bsp_analog_i2c_wait_ack()) // 等待应答
        {
            bsp_analog_i2c_stop();
            return 0;
        }
    }

    bsp_analog_i2c_stop();  // 发送停止信号
    return 1;
}

/**
 * @brief 从从机读取多字节数据（支持16位寄存器地址）
 * @param slave_addr 从机地址
 * @param reg_addr   16位寄存器起始地址
 * @param data       存放读取数据的缓冲区指针
 * @param length     读取的数据长度
 * @retval 0 失败, 1 成功
 */
uint8_t bsp_analog_i2c_read_bytes_16bit_reg(uint8_t slave_addr, uint16_t reg_addr, uint8_t* data, uint16_t length)
{
    bsp_analog_i2c_start();  // 发送起始信号

    bsp_analog_i2c_send_byte((slave_addr << 1) | 0); // 发送从机地址 + 写命令
    if (!bsp_analog_i2c_wait_ack()) // 等待应答
    {
        bsp_analog_i2c_stop();
        return 0;
    }

    // 发送16位寄存器地址（先高字节，再低字节）
    bsp_analog_i2c_send_byte((reg_addr >> 8) & 0xFF);  // 发送寄存器地址高字节
    if (!bsp_analog_i2c_wait_ack())
    {
        bsp_analog_i2c_stop();
        return 0;
    }
    
    bsp_analog_i2c_send_byte(reg_addr & 0xFF);  // 发送寄存器地址低字节
    if (!bsp_analog_i2c_wait_ack())
    {
        bsp_analog_i2c_stop();
        return 0;
    }

    bsp_analog_i2c_start();  // 再次发送起始信号
    bsp_analog_i2c_send_byte((slave_addr << 1) | 1); // 发送从机地址 + 读命令
    if (!bsp_analog_i2c_wait_ack()) // 等待应答
    {
        bsp_analog_i2c_stop();
        return 0;
    }

    // 依次读取每个字节数据
    for(uint16_t i = 0; i < length; i++)
    {
        data[i] = bsp_analog_i2c_read_byte();
        if(i == length - 1)
        {
            bsp_analog_i2c_nack();  // 最后一个字节后发送NACK
        }
        else
        {
            bsp_analog_i2c_ack();   // 发送ACK以继续接收
        }
    }

    bsp_analog_i2c_stop();  // 发送停止信号
    return 1;
}


