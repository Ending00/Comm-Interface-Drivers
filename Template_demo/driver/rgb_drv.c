#include "rgb_drv.h"
// �󶨿�����ʽ����
// �޸� EVENT_FUNC_EXECUTE ���Դ��� start_index �� length ����
#define EVENT_FUNC_EXECUTE(pstRgb, Event, start_index, length)        \
    do                                          \
    {                                           \
        if (pstRgb->func[Event])                        \
        {                                           \
            pstRgb->func[Event](pstRgb, start_index, length);   \
        }                                           \
    } while(0)

/**@brief RBG�¼�����ͷָ�붨�� */    
static RGBController_t *s_pstRgbListHead = NULL;

/**@brief RGB��ʼ��
 *
 * @param[in] controller   ָ��������ṹ
 * @param[in] num_leds     ָ��RGB����
 * @param[in] RGBColor     ָ��������ɫ
 *
 */	
uint8_t rgb_init(RGBController_t *pstRgb,uint8_t num_leds,uint8_t start_index,uint8_t length, RGBColor *led_colors)
{
	RGBController_t *pstCurrRgb = s_pstRgbListHead;
	
    pstRgb->num_leds = num_leds;
    pstRgb->led_colors = led_colors;
    /* ����RGB�������ҵ�ǰ��ǰģʽ�Ƿ��ѳ�ʼ�� */
    while (pstCurrRgb) 
    {
        if (pstCurrRgb == pstRgb) 
        {
			/* RGB�ѳ�ʼ�� */
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
	
/**@brief ��RGBģʽ�¼����ܻص�
 *
 * @param[in] pstRgb    ָ��ǰRGB
 * @param[in] Event     ָ�������¼� @ref RGB_Event_t
 * @param[in] func  ָ�򰴼��¼�����ص�����
 *
 * @retval ��
 */	
void rgb_attach(RGBController_t *pstBtn, RGB_Event_t Event, RgbCallback func)
{
    pstBtn->func[Event] = func;
}	
	
/**@brief ��ȡ��ӦRGB�¼�
 *
 * @param[in] pstRgb  ָ�򰴼�ʵ��
 *
 * @retval ����RGB�¼� @ref  
 */
RGB_Event_t rgb_get_event(RGBController_t *pstRgb)
{
    return (RGB_Event_t)(pstRgb->Event);
}	
	
/**@brief ״̬������ ���ڽ�����λ������״̬
 *
 * @param[in] pstRgb  ָ��ʵ��
 *
 * @retval ��
 */
static void RGB_Handler(RGBController_t *pstRgb)	
{
	switch(pstRgb->State)
	{
		case 0x01:
			pstRgb->Event = (INT8U)RGB_EVT_MODE0;   /* ��ɫģʽ�¼�*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE0, pstRgb->start_index, pstRgb->length);
		    pstRgb->State = 0;
			break;
		case 0x02:
			pstRgb->Event = (INT8U)RGB_EVT_MODE1;   /* ��ɫģʽ�¼�*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE1, pstRgb->start_index, pstRgb->length);
			pstRgb->State = 0;
			break;
		case 0x03:
			pstRgb->Event = (INT8U)RGB_EVT_MODE2;   /* ��ɫ�����¼�*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE2, pstRgb->start_index, pstRgb->length);
			break;
		case 0x04:
			pstRgb->Event = (INT8U)RGB_EVT_MODE3;   /* ��ɫ����ģʽ�¼�*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE3, pstRgb->start_index, pstRgb->length);
		case 0x05:
			pstRgb->Event = (INT8U)RGB_EVT_MODE4;   /* ��ɫ����ͬ��ģʽ�¼�*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE4, pstRgb->start_index, pstRgb->length);		
			break;
		case 0x06:
			pstRgb->Event = (INT8U)RGB_EVT_MODE5;   /* ��ɫ��ˮ���¼�*/
			EVENT_FUNC_EXECUTE(pstRgb, RGB_EVT_MODE5, pstRgb->start_index, pstRgb->length);		
			break;
		    /*user add*/
		    
		default:
			break;
	}
	
}	
	
/**/
/**@brief RGBģʽɨ��
 *
 * @note Ӧ�������Ե��ã�����ʱ��1ms��Ϊ��ʱ��׼��������
 *
 * @param ��
 *
 * @retval ��
 */	

void rgb_scan(void)
{
	RGBController_t *pstCurrRgb = s_pstRgbListHead;

    /* ����RGBģʽ�����������δ��� */
    while (pstCurrRgb) 
    {       
        RGB_Handler(pstCurrRgb);
        pstCurrRgb = pstCurrRgb->Next;
    }
}













