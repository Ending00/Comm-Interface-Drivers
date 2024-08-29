#include "pal_button.h"
#include "pal_rgb.h"
#include "button_drv.h"
#include "usart.h"
#include "bsp_usart2.h"
#define DEBUG_Log		printf  

/**@brief 按键定义 */
BUTTON_t g_stdKey; 
BUTTON_t g_stdKey1;

uint8_t dev_dkey_read_pin_level(void)
{
    return KEY0;
}

uint8_t dev_dkey1_read_pin_level(void)
{
    return KEY1;
}


void button_callback(void *pArg)
{
	int i = 0;
    switch(button_get_event((BUTTON_t *)pArg))
    {
        case BTN_EVT_UP:
            {

            }
            break;

        case BTN_EVT_DOWN:
            {

            }
            break;

        case BTN_EVT_LONG_HOLD:
            {

            }

            break;

        case BTN_EVT_SINGLE:
            {
                stdRgb.State = 0x01;
				globalColor.r = 0x27;
				globalColor.g = 0xD8;
				globalColor.b = 0x27;
				for (i = stdRgb.start_index; i < stdRgb.start_index + stdRgb.length && i < num_leds; i++) {
					stdRgb.led_colors[i] = globalColor;
				}
            }
            break;
			
        case BTN_EVT_DOUBLE:
            {
                stdRgb.State = 0x01;
				globalColor.r = 0xD3;
				globalColor.g = 0x1B;
				globalColor.b = 0xEA;	
				for (i = stdRgb.start_index; i < stdRgb.start_index + stdRgb.length && i < num_leds; i++) {
					stdRgb.led_colors[i] = globalColor;
				}				
            }
            break;
			
        case BTN_EVT_LONG:
            {
				stdRgb1.State = 0x02;
            }
            break;

        default:
            break;

    }	
}

void button_callback1(void *pArg)
{
//	int i = 0;
    switch(button_get_event((BUTTON_t *)pArg))
    {
        case BTN_EVT_SINGLE:
            {
				stdRgb2.State = 0x03;
				stdRgb2.red_weight = 0;
				stdRgb2.green_weight = 0.5;
				stdRgb2.blue_weight = 0.5;
            }
            break;
        case BTN_EVT_DOUBLE:
            {
                stdRgb3.State = 0x04;	
				stdRgb3.red_weight = 0.34;
				stdRgb3.green_weight = 0.33;
				stdRgb3.blue_weight = 0.33;				
            }
            break;
	   case BTN_EVT_LONG:
            {
				stdRgb4.State = 0x05;
            }
            break;
        default:
            break;

    }	
}
void dev_key_attach_init(void)
{
    /* 按键实体功能绑定 */
    button_init(&g_stdKey, dev_dkey_read_pin_level, BTN_ACTIVE_LEVEL_LOW);
//	button_attach(&g_stdKey, BTN_EVT_DOWN, button_callback);	
//	button_attach(&g_stdKey, BTN_EVT_UP, button_callback);		
	button_attach(&g_stdKey, BTN_EVT_SINGLE, button_callback);	
	button_attach(&g_stdKey, BTN_EVT_DOUBLE, button_callback);		
	button_attach(&g_stdKey, BTN_EVT_LONG, button_callback);	
//	button_attach(&g_stdKey, BTN_EVT_LONG_HOLD, button_callback);	
	button_set_time_param(&g_stdKey, 100, 600, 200);
	
    button_init(&g_stdKey1, dev_dkey1_read_pin_level, BTN_ACTIVE_LEVEL_LOW);		
	button_attach(&g_stdKey1, BTN_EVT_SINGLE, button_callback1);
	button_attach(&g_stdKey1, BTN_EVT_DOUBLE, button_callback1);	
	button_attach(&g_stdKey1, BTN_EVT_LONG, button_callback1);
	button_set_time_param(&g_stdKey1, 100, 600, 200);
}




