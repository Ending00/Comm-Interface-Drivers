// iic_driver.h
#ifndef iic_DRIVER_PAL_H
#define iic_DRIVER_PAL_H

#include "iic_driver.h"
#include "sys.h"
#include "delay.h"

typedef struct _GPIO_PIN
{
    GPIO_TypeDef* port;
    uint16_t pin;
}gpio_pin_t;

void PORT_SWD_PORT_CHANGE(gpio_pin_t *clk, gpio_pin_t *dio);
bool IIC_GET_SCL(void);
bool IIC_GET_SDA(void);
void IIC_SET_SDA(GPIO_PinState state);
void IIC_SET_SCL(GPIO_PinState state);
void iic_driver_test(void);
#endif // USART_DRIVER_H

