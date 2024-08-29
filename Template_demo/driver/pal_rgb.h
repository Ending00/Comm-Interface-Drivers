#ifndef __PAL_RGB_H
#define __PAL_RGB_H

#include "rgb_drv.h"
// 定义LED灯的数量
#define num_leds 16

void dev_rgb_attach_init(void);

extern RGBController_t stdRgb;
extern RGBController_t stdRgb1;
extern RGBController_t stdRgb2;
extern RGBController_t stdRgb3;
extern RGBController_t stdRgb4;
extern RGBColor globalColor;
#ifdef __cplusplus
}
#endif

#endif
