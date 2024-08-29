#include "button_drv.h"

#define EVENT_CALLBACK_EXECUTE(pstBtn, Event)       		\
    do                                         				\
    {                                          				\
        if (pstBtn->CallBack[Event])                      	\
		{													\
            pstBtn->CallBack[Event]((BUTTON_t *)pstBtn);  	\
		}													\
    } while(0)

/**@brief 按键链表头指针定义 */    
static BUTTON_t *s_pstBtnListHead = NULL;

/**@brief 按键初始化
 *
 * @param[in] pstBtn       指向按键实体
 * @param[in] GetPinLevel  指向获取按键引脚电平回调函数
 * @param[in] ActiveLevel  指明按键触发的有效电平 @ref BUTTON_ActiveLevel_t
 *
 * @retval 0   -按键已初始化
 * @retval 1   -按键初始化成功
 */
INT8U button_init(BUTTON_t *pstBtn, INT8U(*GetPinLevel)(void), BUTTON_ActiveLevel_t ActiveLevel)
{
	BUTTON_t * pstCurrBtn = s_pstBtnListHead;
	
    /* 遍历按键链表，查找当前按键是否已初始化 */
    while (pstCurrBtn) 
    {
        if (pstCurrBtn == pstBtn) 
        {
			/* 按键已初始化 */
            return 0;  
        }
        pstCurrBtn = pstCurrBtn->Next;
    }
    
	memset(pstBtn, 0, sizeof(BUTTON_t));
    pstBtn->Event          = (INT8U)BTN_EVT_NONE;
    pstBtn->GetPinLevel    = GetPinLevel;
    pstBtn->LastLevel      = pstBtn->GetPinLevel();
    pstBtn->ActiveLevel    = (BUTTON_ActiveLevel_t)ActiveLevel;
	pstBtn->ShortTicks     = 30;
	pstBtn->LongTicks      = 50;
	pstBtn->LongHoldPeriod = 20;
	
	pstBtn->Next = s_pstBtnListHead;
	s_pstBtnListHead = pstBtn;
	
	return 1;
}

/**@brief 设置按键单击、长击、连击事件的触发时间参数
 *
 * @note 所有按键初始化时有统一默认值，除却某按键需要，
 *       可在button_init之后调用本接口进行重新设置外，
 *       无需调用
 *
 * @param[in] pstBtn          指向按键实体
 * @param[in] ShortTicks      指明单击触发（或多击间隔）时间
 * @param[in] LongTicks       指明长击触发时间
 * @param[in] LongHoldPeriod  指明连击触发周期
 *
 * @retval 无
 */
void button_set_time_param(BUTTON_t *pstBtn, INT8U ShortTicks, INT16U LongTicks, INT8U LongHoldPeriod)
{
	if (ShortTicks)
	{
		pstBtn->ShortTicks = ShortTicks;
	}
	
	if (LongTicks)
	{
		pstBtn->LongTicks = LongTicks;
	}
	
	if (LongHoldPeriod)
	{
		pstBtn->LongHoldPeriod = LongHoldPeriod;
	}
}

/**@brief 绑定按键事件功能回调
 *
 * @param[in] pstBtn    指向按键实体
 * @param[in] Event     指明按键事件 @ref BUTTON_Event_t
 * @param[in] CallBack  指向按键事件处理回调函数
 *
 * @retval 无
 */
void button_attach(BUTTON_t *pstBtn, BUTTON_Event_t Event, BtnCallback CallBack)
{
    pstBtn->CallBack[Event] = CallBack;
}

/**@brief 获取按键事件
 *
 * @param[in] pstBtn  指向按键实体
 *
 * @retval 返回按键事件 @ref BUTTON_Event_t 
 */
BUTTON_Event_t button_get_event(BUTTON_t *pstBtn)
{
    return (BUTTON_Event_t)(pstBtn->Event);
}


/**@brief 单个按键处理
 *
 * @param[in] pstBtn  指向按键实体
 *
 * @retval 无
 */
static void BUTTON_Handler(BUTTON_t *pstBtn)
{
    INT8U BtnPinLevel = pstBtn->GetPinLevel();

    /* 有键按下则嘀嗒计数器累加 */
    if ((pstBtn->State) > 0) 
    {
        pstBtn->Ticks++;
    }

	/**@note 按键消抖处理
	 *
	 * 与上次按键电平不同则重复读取DEBOUNCE_TICKS次，
	 *
	 * 状态不变则存储当前按键电平，否则清消抖计数器 
	 */
    if (BtnPinLevel != pstBtn->LastLevel)
    { 
        if (++pstBtn->DebounceCnt >= 2) 
        {
            pstBtn->LastLevel = BtnPinLevel;
            pstBtn->DebounceCnt = 0;
        }
    } 
    else 
    { 
        pstBtn->DebounceCnt = 0;
    }

    /* 按键处理状态机 */
    switch (pstBtn->State) 
    {
		case 0:
			if (pstBtn->LastLevel == pstBtn->ActiveLevel) 
			{ 
				pstBtn->Event = (INT8U)BTN_EVT_DOWN;
				EVENT_CALLBACK_EXECUTE(pstBtn, BTN_EVT_DOWN);
				pstBtn->Ticks  = 0;
				pstBtn->Repeat = 1;
				pstBtn->State  = 1; 
			} 
			else 
			{
				pstBtn->Event = (INT8U)BTN_EVT_NONE;
			}
			break;

		case 1:
			if (pstBtn->LastLevel != pstBtn->ActiveLevel)
			{ 
				pstBtn->Event = (INT8U)BTN_EVT_UP;
				EVENT_CALLBACK_EXECUTE(pstBtn, BTN_EVT_UP);
				pstBtn->Ticks = 0;
				pstBtn->State = 2;
			} 
			else if (pstBtn->LongTicks > 0)
			{
                if (pstBtn->Ticks > pstBtn->LongTicks)
                {
                    pstBtn->Event = (INT8U)BTN_EVT_LONG;
                    EVENT_CALLBACK_EXECUTE(pstBtn, BTN_EVT_LONG);
                    pstBtn->State = 4; 
                }
			}
			break;

		case 2:
			if (pstBtn->LastLevel == pstBtn->ActiveLevel) 
			{ 
				pstBtn->Event = (INT8U)BTN_EVT_DOWN;
				EVENT_CALLBACK_EXECUTE(pstBtn, BTN_EVT_DOWN);
				pstBtn->Repeat++;
				pstBtn->Ticks = 0;
				pstBtn->State = 3;
			} 
			else if (pstBtn->Ticks > pstBtn->ShortTicks) 
			{ 
				if (pstBtn->Repeat == 1) 
				{
					pstBtn->Event = (INT8U)BTN_EVT_SINGLE;
					EVENT_CALLBACK_EXECUTE(pstBtn, BTN_EVT_SINGLE);
				} 
				else if (pstBtn->Repeat == 2) 
				{
					pstBtn->Event = (INT8U)BTN_EVT_DOUBLE;
					EVENT_CALLBACK_EXECUTE(pstBtn, BTN_EVT_DOUBLE); 
				}
				pstBtn->State = 0;
			}
			break;

		case 3:
			if (pstBtn->LastLevel != pstBtn->ActiveLevel) 
			{
				pstBtn->Event = (INT8U)BTN_EVT_UP;
				EVENT_CALLBACK_EXECUTE(pstBtn, BTN_EVT_UP);
				
				if (pstBtn->Ticks < pstBtn->ShortTicks) 
				{
					pstBtn->Ticks = 0;
					pstBtn->State = 2; 
				} 
				else 
				{
					pstBtn->State = 0;
				}
			}
			break;

		case 4:
			if (pstBtn->LastLevel == pstBtn->ActiveLevel) 
			{
				if (pstBtn->LongHoldPeriod > 0)
				{
					if (pstBtn->Ticks % pstBtn->LongHoldPeriod == 0)
					{
                        pstBtn->Event = (INT8U)BTN_EVT_LONG_HOLD;
						EVENT_CALLBACK_EXECUTE(pstBtn, BTN_EVT_LONG_HOLD);
					}
				}
			} 
			else 
			{
				pstBtn->Event = (INT8U)BTN_EVT_UP;
				EVENT_CALLBACK_EXECUTE(pstBtn, BTN_EVT_UP);
				
				pstBtn->State = 0; 
			}
			break;
    }
}

/**@brief 按键扫描
 *
 * @note 应被周期性调用，建议时间5~20ms且为定时基准的整数倍
 *
 * @param 无
 *
 * @retval 无
 */
void button_scan(void)
{
	BUTTON_t *pstCurrBtn = s_pstBtnListHead;

    /* 遍历按键链表并依次处理 */
    while (pstCurrBtn) 
    {       
        BUTTON_Handler(pstCurrBtn);
        pstCurrBtn = pstCurrBtn->Next;
    }
}

/**@brief 按键复位
 *
 * @param 无
 *
 * @retval 无
 */
void button_reset(void)
{
	BUTTON_t *pstCurrBtn = s_pstBtnListHead;

    while (pstCurrBtn) 
    {
        pstCurrBtn->State = 0;
        pstCurrBtn->DebounceCnt = 0;
        pstCurrBtn->Ticks = 0;
        pstCurrBtn->Repeat = 0;
        pstCurrBtn->Event = (INT8U)BTN_EVT_NONE;
		
        pstCurrBtn = pstCurrBtn->Next;
    }
}
/*
****************** (C) Copyright 2023  ***** END OF FILE *********************
*/











