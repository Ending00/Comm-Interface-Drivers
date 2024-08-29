#include "button_drv.h"

#define EVENT_CALLBACK_EXECUTE(pstBtn, Event)       		\
    do                                         				\
    {                                          				\
        if (pstBtn->CallBack[Event])                      	\
		{													\
            pstBtn->CallBack[Event]((BUTTON_t *)pstBtn);  	\
		}													\
    } while(0)

/**@brief ��������ͷָ�붨�� */    
static BUTTON_t *s_pstBtnListHead = NULL;

/**@brief ������ʼ��
 *
 * @param[in] pstBtn       ָ�򰴼�ʵ��
 * @param[in] GetPinLevel  ָ���ȡ�������ŵ�ƽ�ص�����
 * @param[in] ActiveLevel  ָ��������������Ч��ƽ @ref BUTTON_ActiveLevel_t
 *
 * @retval 0   -�����ѳ�ʼ��
 * @retval 1   -������ʼ���ɹ�
 */
INT8U button_init(BUTTON_t *pstBtn, INT8U(*GetPinLevel)(void), BUTTON_ActiveLevel_t ActiveLevel)
{
	BUTTON_t * pstCurrBtn = s_pstBtnListHead;
	
    /* ���������������ҵ�ǰ�����Ƿ��ѳ�ʼ�� */
    while (pstCurrBtn) 
    {
        if (pstCurrBtn == pstBtn) 
        {
			/* �����ѳ�ʼ�� */
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

/**@brief ���ð��������������������¼��Ĵ���ʱ�����
 *
 * @note ���а�����ʼ��ʱ��ͳһĬ��ֵ����ȴĳ������Ҫ��
 *       ����button_init֮����ñ��ӿڽ������������⣬
 *       �������
 *
 * @param[in] pstBtn          ָ�򰴼�ʵ��
 * @param[in] ShortTicks      ָ���������������������ʱ��
 * @param[in] LongTicks       ָ����������ʱ��
 * @param[in] LongHoldPeriod  ָ��������������
 *
 * @retval ��
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

/**@brief �󶨰����¼����ܻص�
 *
 * @param[in] pstBtn    ָ�򰴼�ʵ��
 * @param[in] Event     ָ�������¼� @ref BUTTON_Event_t
 * @param[in] CallBack  ָ�򰴼��¼�����ص�����
 *
 * @retval ��
 */
void button_attach(BUTTON_t *pstBtn, BUTTON_Event_t Event, BtnCallback CallBack)
{
    pstBtn->CallBack[Event] = CallBack;
}

/**@brief ��ȡ�����¼�
 *
 * @param[in] pstBtn  ָ�򰴼�ʵ��
 *
 * @retval ���ذ����¼� @ref BUTTON_Event_t 
 */
BUTTON_Event_t button_get_event(BUTTON_t *pstBtn)
{
    return (BUTTON_Event_t)(pstBtn->Event);
}


/**@brief ������������
 *
 * @param[in] pstBtn  ָ�򰴼�ʵ��
 *
 * @retval ��
 */
static void BUTTON_Handler(BUTTON_t *pstBtn)
{
    INT8U BtnPinLevel = pstBtn->GetPinLevel();

    /* �м���������઼������ۼ� */
    if ((pstBtn->State) > 0) 
    {
        pstBtn->Ticks++;
    }

	/**@note ������������
	 *
	 * ���ϴΰ�����ƽ��ͬ���ظ���ȡDEBOUNCE_TICKS�Σ�
	 *
	 * ״̬������洢��ǰ������ƽ������������������ 
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

    /* ��������״̬�� */
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

/**@brief ����ɨ��
 *
 * @note Ӧ�������Ե��ã�����ʱ��5~20ms��Ϊ��ʱ��׼��������
 *
 * @param ��
 *
 * @retval ��
 */
void button_scan(void)
{
	BUTTON_t *pstCurrBtn = s_pstBtnListHead;

    /* ���������������δ��� */
    while (pstCurrBtn) 
    {       
        BUTTON_Handler(pstCurrBtn);
        pstCurrBtn = pstCurrBtn->Next;
    }
}

/**@brief ������λ
 *
 * @param ��
 *
 * @retval ��
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











