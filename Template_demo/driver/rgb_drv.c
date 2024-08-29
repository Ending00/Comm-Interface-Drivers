#include "rgb_drv.h"
// 绑定控制样式函数
// 修改 EVENT_FUNC_EXECUTE 宏以传递 start_index 和 length 参数
#define EVENT_FUNC_EXECUTE(pstRgb, Event, start_index, length)        \
    do                                          \
    {                                           \
        if (pstRgb->func[Event])                        \
        {                                           \
            pstRgb->func[Event](pstRgb, start_index, length);   \
        }                                           \
    } while(0)

/**@brief RBG事件链表头指针定义 */    
static RGBController_t *s_pstRgbListHead = NULL;

/**@brief RGB初始化
 *
 * @param[in] controller   指向控制器结构
 * @param[in] num_leds     指向RGB个数
 * @param[in] RGBColor     指明控制颜色
 *
 */	
uint8_t rgb_init(RGBController_t *pstRgb,uint8_t num_leds,uint8_t start_index,uint8_t length, RGBColor *led_colors)
{
	RGBController_t *pstCurrRgb = s_pstRgbListHead;
	
    pstRgb->num_leds = num_leds;
    pstRgb->led_colors = led_colors;
    /* 遍历RGB链表，查找当前当前模式是否已初始化 */
    while (pstCurrRgb) 
    {
        if (pstCurrRgb == pstRgb) 
        {
			/* RGB已初始化 */
            return 0;  
        }
        pstCurrRgb = pstCurrRgb->Next;
    }	
	pstRgb->State = 0;
    pstRgb->start_index = start_index;
	pstRgb->length = length;
	pstRgb->num_leds = num_leds;
    pstRgb->led_colors = led_colors;
	pstRgb->Event  = (INT8U)RGB_EVT_NONE;
	pstRgb->Next = s_pstRgbListHead;
	s_pstRgbListHead = pstRgb;
	return 1;
}	
	
/**@brief 绑定RGB模式事件功能回调
 *
 * @param[in] pstRgb    指向当前RGB
 * @param[in] Event     指明按键事件 @ref RGB_Event_t
 * @param[in] func  指向按键事件处理回调函数
 *
 * @retval 无
 */	
void rgb_attach(RGBController_t *pstBtn, RGB_Event_t Event, RgbCallback func)
{
    pstBtn->func[Event] = func;
}	
	
/**@brief 获取对应RGB事件
 *
 * @param[in] pstRgb  指向按键实体
 *
 * @retval 返回RGB事件 @ref  
 */
RGB_Event_t rgb_get_event(RGBController_t *pstRgb)
{
    return (RGB_Event_t)(pstRgb->Event);
}	
	
/**@brief 状态机处理 用于接收上位机控制状态
 *
 * @param[in] pstRgb  指向实体
 *
 * @retval 无
 */
static void RGB_Handler(RGBController_t *pstRgb)	
{
	switch(pstRgb->State)
	{
		case 0x01:
			pstRgb->Event = (INT8U)RGB_EVT_MODE0;   /* 单色模式事件*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE0, pstRgb->start_index, pstRgb->length);
		    pstRgb->State = 0;
			break;
		case 0x02:
			pstRgb->Event = (INT8U)RGB_EVT_MODE1;   /* 七色模式事件*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE1, pstRgb->start_index, pstRgb->length);
			pstRgb->State = 0;
			break;
		case 0x03:
			pstRgb->Event = (INT8U)RGB_EVT_MODE2;   /* 单色渐变事件*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE2, pstRgb->start_index, pstRgb->length);
			break;
		case 0x04:
			pstRgb->Event = (INT8U)RGB_EVT_MODE3;   /* 七色呼吸模式事件*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE3, pstRgb->start_index, pstRgb->length);
		case 0x05:
			pstRgb->Event = (INT8U)RGB_EVT_MODE4;   /* 七色呼吸同步模式事件*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE4, pstRgb->start_index, pstRgb->length);		
			break;
		case 0x06:
			pstRgb->Event = (INT8U)RGB_EVT_MODE5;   /* 七色流水灯事件*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE5, pstRgb->start_index, pstRgb->length);		
			break;
		    /*user add*/
		    
		default:
			break;
	}
	
}	
	
/**/
/**@brief RGB模式扫描
 *
 * @note 应被周期性调用，建议时间1ms且为定时基准的整数倍
 *
 * @param 无
 *
 * @retval 无
 */	

void rgb_scan(void)
{
	RGBController_t *pstCurrRgb = s_pstRgbListHead;

    /* 遍历RGB模式控制链表并依次处理 */
    while (pstCurrRgb) 
    {       
        RGB_Handler(pstCurrRgb);
        pstCurrRgb = pstCurrRgb->Next;
    }
}













