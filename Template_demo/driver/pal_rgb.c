#include "pal_rgb.h"
#include "rgb_drv.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include <math.h>
#include <stdint.h>
#define DEBUG_Log		printf  


RGBController_t stdRgb;
RGBController_t stdRgb1;
RGBController_t stdRgb2;
RGBController_t stdRgb3;
RGBController_t stdRgb4;
// 全局变量用于存储颜色
RGBColor globalColor = {255,0,0}; //默认红色

// 定义RGB七色数组
RGBColor svencolors[] = {
    {255, 0, 0},    // 红色
    {255, 165, 0},  // 橙色
    {255, 255, 0},  // 黄色
    {0, 255, 0},    // 绿色
    {0, 0, 255},    // 蓝色
    {75, 0, 130},   // 紫色
    {128, 0, 128}   // 粉色
};

/*
	写 0码 函数
	根据RGB灯手册查得 ：RGB_H 延时 300ns，RGB_L 延时 1us
*/
void Write0(void)
{
	RGB_HL = 1;
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();
	
	RGB_HL = 0;
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
}
/*
	写 1码 函数
	根据RGB灯手册查得 ：RGB_H 延时 1us，RGB_L 延时 300ns 
*/
void Write1(void)
{
	RGB_HL = 1;
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();	
	RGB_HL = 0;
	__NOP();__NOP();                    
}

void WriteRest(void)
{	
	RGB_HL = 0;
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
}

/*
	写一个字节（8bit）数据
	备  注: RGB的亮度通过更改 0x01 处的值进行调节
*/
void RGB_WByte(uint8_t byte)
{
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
	  if((byte<<i)&0x01)
		  Write1();
	  else
		  Write0();
	}
}

// 模拟LED灯条的颜色缓冲
RGBColor leds[num_leds] = {0};

// 设置指定索引位置LED灯的颜色
void setLedColor(uint8_t index, RGBColor color) {
    if (index < num_leds) {
        leds[index] = color;
    }
}

// 更新LED灯条，发送颜色数据到LED灯
void showLeds(void) {
	int i = 0 ,j = 0;	
	for (i = 0; i < num_leds; i++) {
        // 发送24位颜色数据，按GRB的顺序发送
        for (j = 7; j >= 0; j--) {
            // 每个颜色通道的最高位先发送
            if (leds[i].g & (1 << j)) {
                Write1();
            } else {
                Write0();
            }
        }
        for (j = 7; j >= 0; j--) {
            if (leds[i].r & (1 << j)) {
                Write1();
            } else {
                Write0();
            }
        }
        for (j = 7; j >= 0; j--) {
            if (leds[i].b & (1 << j)) {
                Write1();
            } else {
                Write0();
            }
        }
    }
    // 发送复位信号，根据具体硬件实现
	WriteRest();
}

// 控制样式函数示例1：单色控制
void singleColor(RGBController_t *controller, uint8_t start_index, uint8_t length) {
    uint8_t i = 0;
	for (i = start_index; i < start_index + length && i < num_leds; i++) {
        // 设置指定位置范围内的LED灯
		controller->led_colors[i] = globalColor;
    }
    // 更新LED颜色数据到leds数组
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        setLedColor(i, controller->led_colors[i]);
    }
	// 更新LED灯条显示
	showLeds();
}

// 控制样式函数示例2：彩虹渐变
void rainbowGradient(RGBController_t *controller, uint8_t start_index, uint8_t length) {
    uint8_t i = 0;
	uint8_t r, g, b;
	// 计算每个颜色之间的渐变步长
    float step = 255.0 / (length - 1);
    // 生成七彩渐变颜色序列
    for ( i = 0; i < length; i++) {
        float h = i * step;
        float x = 1.0 - fabs((h / 60.0) - (2.0 * floor(h / 120.0)));
        if (h < 60.0) {
            r = 255;
            g = x * 255;
            b = 0;
        } else if (h < 120.0) {
            r = x * 255;
            g = 255;
            b = 0;
        } else if (h < 180.0) {
            r = 0;
            g = 255;
            b = x * 255;
        } else if (h < 240.0) {
            r = 0;
            g = x * 255;
            b = 255;
        } else if (h < 300.0) {
            r = x * 255;
            g = 0;
            b = 255;
        } else {
            r = 255;
            g = 0;
            b = x * 255;
        }
        // 设置颜色
        controller->led_colors[start_index + i].r = r;
        controller->led_colors[start_index + i].g = g;
        controller->led_colors[start_index + i].b = b;
    }
	    // 更新LED颜色数据到leds数组
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        setLedColor(i, controller->led_colors[i]);
    }
	// 更新LED灯条显示
	showLeds();
}

// 获取当前时间的函数
uint32_t getCurrentTime() {
    return current_time_ms;
}

// 控制样式函数示例3：单色呼吸渐变
void fixedBreatheGradient(RGBController_t *controller, uint8_t start_index, uint8_t length) {
    uint8_t i = 0;
    // 呼吸渐变的周期
    float period = 2000.0; // 假设为1000毫秒
    // 计算每个颜色通道的权重
    float red_weight = controller->red_weight;  // 红色通道的权重
    float green_weight = controller->green_weight; // 绿色通道的权重
    float blue_weight = controller->blue_weight;  // 蓝色通道的权重
    // 当前时间，可以是系统时间或者一个自定义的计时器
    uint32_t current_time = getCurrentTime(); // 假设有一个函数可以获取当前时间，返回值是毫秒级别的时间戳
    // 计算当前位置的亮度
    float brightness = 0.5 * (1.0 + sin(2 * M_PI * current_time / period));

    // 遍历指定范围内的LED，设置呼吸渐变的颜色
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        // 分别应用权重值到每个颜色通道
        controller->led_colors[i].r = (uint8_t)(brightness * red_weight * 255);
        controller->led_colors[i].g = (uint8_t)(brightness * green_weight * 255);
        controller->led_colors[i].b = (uint8_t)(brightness * blue_weight * 255);
    }
		    // 更新LED颜色数据到leds数组
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        setLedColor(i, controller->led_colors[i]);
    }
	// 更新LED灯条显示
	showLeds();	
}


// 控制样式函数示例4：七彩循环单色呼吸渐变
void rainbowBreath(RGBController_t *controller, uint8_t start_index, uint8_t length) {
	uint8_t i = 0;
    // 呼吸渐变的周期
    float period = 2000.0; // 假设为1000毫秒
    // 当前时间，可以是系统时间或者一个自定义的计时器
    uint32_t current_time = getCurrentTime(); // 假设有一个函数可以获取当前时间，返回值是毫秒级别的时间戳
    // 计算当前位置的亮度
    float brightness = 0.5 * (1.0 + sin(2 * M_PI * current_time / period));
    // 遍历指定范围内的LED，设置呼吸渐变的颜色
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        // 根据亮度和权重计算颜色通道的值
        controller->led_colors[i].r = (uint8_t)(brightness * controller->red_weight * svencolors[i % 7].r);
        controller->led_colors[i].g = (uint8_t)(brightness * controller->green_weight * svencolors[i % 7].g);
        controller->led_colors[i].b = (uint8_t)(brightness * controller->blue_weight * svencolors[i % 7].b);
    }
    // 更新LED颜色数据到leds数组
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        setLedColor(i, controller->led_colors[i]);
    }
    // 更新LED灯条显示
    showLeds();
}

// 控制样式函数示例5：七彩呼吸渐变同步
void rainbowBreathSync(RGBController_t *controllers, uint8_t start_index, uint8_t length) {
    uint8_t red = 0, green = 0, blue = 0;
	uint8_t i = 0 ,j = 0;
	// 定义每个阶段的持续时间（以毫秒为单位）
    const uint16_t phase_duration = 4000.0; // 每个阶段持续1秒
    static uint16_t fade_steps;// 每个颜色通道的淡入/淡出步数

    static uint8_t phase = 0; // 阶段计数器
    static uint8_t step = 0; // 步数计数器
    // 计算当前阶段的颜色
    switch (phase) {
        case 0:
            red = 255 - step;
            green = step;
            blue = 0;
            break;
        case 1:
            red = 0;
            green = 255 - step;
            blue = step;
            break;
        case 2:
            red = step;
            green = 0;
            blue = 255 - step;
            break;
    }
    // 更新RGBController_t结构体中的颜色值
    for (i = start_index; i < start_index + length; i++) {
        controllers->led_colors[i].r = red;
        controllers->led_colors[i].g = green;
        controllers->led_colors[i].b = blue;
    }	   	
    // 增加步数计数器，如果达到fade_steps则增加阶段计数器
	fade_steps++;
	if(fade_steps == (phase_duration / 256))
	{
		fade_steps = 0;
		step++;
		if(step >= 255)
		{
			step = 0;
			phase++;
			if (phase >= 3) {
				phase = 0;
			}			
		}
	}
    // 更新LED颜色数据到leds数组
    for (j = start_index; j < start_index + length && j < num_leds; j++) {
        setLedColor(j, controllers->led_colors[j]);
    }
    // 更新LED灯条显示
    showLeds();
}

// 控制样式函数示例6：七彩流水灯
// 流水灯效果函数
void rainbowEffect(RGBController_t* controllers, uint8_t start_index, uint8_t length) {
	
	static uint8_t iteration = 0, i;
	// 控制流水灯效果的速度，单位为毫秒
	uint16_t delay_ms = 500;
	static uint16_t num = 0;	
    // 控制流水灯效果的循环次数，这里假设执行 10 次
//    uint8_t num_iterations = 10;	
    // 计算颜色数组的长度
    uint8_t num_colors = sizeof(svencolors) / sizeof(svencolors[0]);	
    // 开始流水灯效果的循环
    num++;
	if(num == delay_ms)
	{
		num = 0;
		controllers->led_colors[i++] = svencolors[iteration++];
		if(iteration >= num_colors)iteration = 0;
		if(i >= length) start_index = i;
		setLedColor(i, controllers->led_colors[i]);
		 // 更新LED灯条显示
		showLeds();	
	}
}



RGBColor led_colors[num_leds];

void dev_rgb_attach_init(void)
{
	// 设置RGB控制器参数
//	rgb_init(&stdRgb, num_leds,0 ,4, led_colors);// 控制起始位置为4，控制2个LED	
//	// 绑定控制样式函数
//	rgb_attach(&stdRgb,RGB_EVT_MODE0,singleColor);  //单色模式事件
	
//	// 设置RGB控制器参数
//	rgb_init(&stdRgb1, num_leds,4 ,4, led_colors);// 控制起始位置为8，控制4个LED	
//	// 绑定控制样式函数
//	rgb_attach(&stdRgb1,RGB_EVT_MODE1,rainbowGradient); //七彩灯模式事件	

//	// 设置RGB控制器参数
//	rgb_init(&stdRgb2, num_leds,8 ,4, led_colors);// 控制起始位置为12，控制4个LED	
//	// 绑定控制样式函数
//	rgb_attach(&stdRgb2,RGB_EVT_MODE2,fixedBreatheGradient);  //单色渐变事件	
//	
//	// 设置RGB控制器参数
//	rgb_init(&stdRgb3, num_leds,12 ,4, led_colors);// 控制起始位置为12，控制4个LED	
//	// 绑定控制样式函数
//	rgb_attach(&stdRgb3,RGB_EVT_MODE3,rainbowBreath);  //七彩灯渐变事件	
	
	
	rgb_init(&stdRgb4, num_leds,0 ,16, led_colors);// 控制起始位置为12，控制4个LED	
	// 绑定控制样式函数
	rgb_attach(&stdRgb4,RGB_EVT_MODE4,rainbowBreathSync);  //七彩灯同步渐变事件件	

//	rgb_init(&stdRgb4, num_leds,0 ,16, led_colors);// 控制起始位置为12，控制4个LED	
//	// 绑定控制样式函数
//	rgb_attach(&stdRgb4,RGB_EVT_MODE5,rainbowEffect);  //七彩灯流水灯事件
}




