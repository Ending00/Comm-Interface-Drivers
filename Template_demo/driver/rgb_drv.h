#ifndef __RGB_DRV_H
#define __RGB_DRV_H
#include "general_type.h"

#define M_PI 3.14159265358979323846
// 先声明 RGBController_t 结构体
typedef struct RGBController RGBController_t;

//extern RGBController_t *pstCurrRgb;
// 定义RGB数据结构
typedef struct {
    INT8U r;
    INT8U g;
    INT8U b;
} RGBColor;

/**@brief RGB回调函数类型定义 */	
typedef void (*RgbCallback)(RGBController_t *controller, INT8U start_index, INT8U length);

typedef enum
{
	RGB_EVT_MODE0,   /* 单色模式事件*/
	RGB_EVT_MODE1,   /* 七色模式事件*/
	RGB_EVT_MODE2,	 /* 单色呼吸模式事件*/
	RGB_EVT_MODE3,   /* 七色呼吸模式事件*/
	RGB_EVT_MODE4,
	RGB_EVT_MODE5,
	RGB_EVT_SUM, 
	RGB_EVT_NONE
}RGB_Event_t;

// 控制器结构
struct RGBController{
    INT8U num_leds;                  // LED灯的数量
	INT8U start_index;				 //起始位置
	INT8U length;					 //点亮长度
	INT8U Event :4;                  //rgb事件
	INT8U State :4;                  //rgb状态
    RGBColor *led_colors;            // LED颜色数组
	/*渐变权重*/
	float red_weight;
	float green_weight;
	float blue_weight;
//	INT8U breath_speed;
	
    RgbCallback func[RGB_EVT_SUM];   // 控制样式函数指针
	struct RGBController *Next;
};


// 函数声明
uint8_t rgb_init(RGBController_t *pstRgb,uint8_t num_leds,uint8_t start_index,uint8_t length, RGBColor *led_colors);
extern void rgb_attach(RGBController_t *pstBtn, RGB_Event_t Event, RgbCallback func);
extern RGB_Event_t rgb_get_event(RGBController_t *pstRgb);
extern void rgb_scan(void);	















#endif
