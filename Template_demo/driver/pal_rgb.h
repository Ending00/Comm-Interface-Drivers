#ifndef __PAL_RGB_H
#define __PAL_RGB_H

#include "rgb_drv.h"
// ����LED�Ƶ�����
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
