#ifndef __RGB_DRV_H
#define __RGB_DRV_H
#include "general_type.h"

#define M_PI 3.14159265358979323846
// ������ RGBController_t �ṹ��
typedef struct RGBController RGBController_t;

//extern RGBController_t *pstCurrRgb;
// ����RGB���ݽṹ
typedef struct {
    INT8U r;
    INT8U g;
    INT8U b;
} RGBColor;

/**@brief RGB�ص��������Ͷ��� */	
typedef void (*RgbCallback)(RGBController_t *controller, INT8U start_index, INT8U length);

typedef enum
{
	RGB_EVT_MODE0,   /* ��ɫģʽ�¼�*/
	RGB_EVT_MODE1,   /* ��ɫģʽ�¼�*/
	RGB_EVT_MODE2,	 /* ��ɫ����ģʽ�¼�*/
	RGB_EVT_MODE3,   /* ��ɫ����ģʽ�¼�*/
	RGB_EVT_MODE4,
	RGB_EVT_MODE5,
	RGB_EVT_SUM, 
	RGB_EVT_NONE
}RGB_Event_t;

// �������ṹ
struct RGBController{
    INT8U num_leds;                  // LED�Ƶ�����
	INT8U start_index;				 //��ʼλ��
	INT8U length;					 //��������
	INT8U Event :4;                  //rgb�¼�
	INT8U State :4;                  //rgb״̬
    RGBColor *led_colors;            // LED��ɫ����
	/*����Ȩ��*/
	float red_weight;
	float green_weight;
	float blue_weight;
//	INT8U breath_speed;
	
    RgbCallback func[RGB_EVT_SUM];   // ������ʽ����ָ��
	struct RGBController *Next;
};


// ��������
uint8_t rgb_init(RGBController_t *pstRgb,uint8_t num_leds,uint8_t start_index,uint8_t length, RGBColor *led_colors);
extern void rgb_attach(RGBController_t *pstBtn, RGB_Event_t Event, RgbCallback func);
extern RGB_Event_t rgb_get_event(RGBController_t *pstRgb);
extern void rgb_scan(void);	















#endif
