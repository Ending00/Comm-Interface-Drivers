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
// ȫ�ֱ������ڴ洢��ɫ
RGBColor globalColor = {255,0,0}; //Ĭ�Ϻ�ɫ

// ����RGB��ɫ����
RGBColor svencolors[] = {
    {255, 0, 0},    // ��ɫ
    {255, 165, 0},  // ��ɫ
    {255, 255, 0},  // ��ɫ
    {0, 255, 0},    // ��ɫ
    {0, 0, 255},    // ��ɫ
    {75, 0, 130},   // ��ɫ
    {128, 0, 128}   // ��ɫ
};

/*
	д 0�� ����
	����RGB���ֲ��� ��RGB_H ��ʱ 300ns��RGB_L ��ʱ 1us
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
	д 1�� ����
	����RGB���ֲ��� ��RGB_H ��ʱ 1us��RGB_L ��ʱ 300ns 
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
	дһ���ֽڣ�8bit������
	��  ע: RGB������ͨ������ 0x01 ����ֵ���е���
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

// ģ��LED��������ɫ����
RGBColor leds[num_leds] = {0};

// ����ָ������λ��LED�Ƶ���ɫ
void setLedColor(uint8_t index, RGBColor color) {
    if (index < num_leds) {
        leds[index] = color;
    }
}

// ����LED������������ɫ���ݵ�LED��
void showLeds(void) {
	int i = 0 ,j = 0;	
	for (i = 0; i < num_leds; i++) {
        // ����24λ��ɫ���ݣ���GRB��˳����
        for (j = 7; j >= 0; j--) {
            // ÿ����ɫͨ�������λ�ȷ���
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
    // ���͸�λ�źţ����ݾ���Ӳ��ʵ��
	WriteRest();
}

// ������ʽ����ʾ��1����ɫ����
void singleColor(RGBController_t *controller, uint8_t start_index, uint8_t length) {
    uint8_t i = 0;
	for (i = start_index; i < start_index + length && i < num_leds; i++) {
        // ����ָ��λ�÷�Χ�ڵ�LED��
		controller->led_colors[i] = globalColor;
    }
    // ����LED��ɫ���ݵ�leds����
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        setLedColor(i, controller->led_colors[i]);
    }
	// ����LED������ʾ
	showLeds();
}

// ������ʽ����ʾ��2���ʺ罥��
void rainbowGradient(RGBController_t *controller, uint8_t start_index, uint8_t length) {
    uint8_t i = 0;
	uint8_t r, g, b;
	// ����ÿ����ɫ֮��Ľ��䲽��
    float step = 255.0 / (length - 1);
    // �����߲ʽ�����ɫ����
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
        // ������ɫ
        controller->led_colors[start_index + i].r = r;
        controller->led_colors[start_index + i].g = g;
        controller->led_colors[start_index + i].b = b;
    }
	    // ����LED��ɫ���ݵ�leds����
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        setLedColor(i, controller->led_colors[i]);
    }
	// ����LED������ʾ
	showLeds();
}

// ��ȡ��ǰʱ��ĺ���
uint32_t getCurrentTime() {
    return current_time_ms;
}

// ������ʽ����ʾ��3����ɫ��������
void fixedBreatheGradient(RGBController_t *controller, uint8_t start_index, uint8_t length) {
    uint8_t i = 0;
    // �������������
    float period = 2000.0; // ����Ϊ1000����
    // ����ÿ����ɫͨ����Ȩ��
    float red_weight = controller->red_weight;  // ��ɫͨ����Ȩ��
    float green_weight = controller->green_weight; // ��ɫͨ����Ȩ��
    float blue_weight = controller->blue_weight;  // ��ɫͨ����Ȩ��
    // ��ǰʱ�䣬������ϵͳʱ�����һ���Զ���ļ�ʱ��
    uint32_t current_time = getCurrentTime(); // ������һ���������Ի�ȡ��ǰʱ�䣬����ֵ�Ǻ��뼶���ʱ���
    // ���㵱ǰλ�õ�����
    float brightness = 0.5 * (1.0 + sin(2 * M_PI * current_time / period));

    // ����ָ����Χ�ڵ�LED�����ú����������ɫ
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        // �ֱ�Ӧ��Ȩ��ֵ��ÿ����ɫͨ��
        controller->led_colors[i].r = (uint8_t)(brightness * red_weight * 255);
        controller->led_colors[i].g = (uint8_t)(brightness * green_weight * 255);
        controller->led_colors[i].b = (uint8_t)(brightness * blue_weight * 255);
    }
		    // ����LED��ɫ���ݵ�leds����
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        setLedColor(i, controller->led_colors[i]);
    }
	// ����LED������ʾ
	showLeds();	
}


// ������ʽ����ʾ��4���߲�ѭ����ɫ��������
void rainbowBreath(RGBController_t *controller, uint8_t start_index, uint8_t length) {
	uint8_t i = 0;
    // �������������
    float period = 2000.0; // ����Ϊ1000����
    // ��ǰʱ�䣬������ϵͳʱ�����һ���Զ���ļ�ʱ��
    uint32_t current_time = getCurrentTime(); // ������һ���������Ի�ȡ��ǰʱ�䣬����ֵ�Ǻ��뼶���ʱ���
    // ���㵱ǰλ�õ�����
    float brightness = 0.5 * (1.0 + sin(2 * M_PI * current_time / period));
    // ����ָ����Χ�ڵ�LED�����ú����������ɫ
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        // �������Ⱥ�Ȩ�ؼ�����ɫͨ����ֵ
        controller->led_colors[i].r = (uint8_t)(brightness * controller->red_weight * svencolors[i % 7].r);
        controller->led_colors[i].g = (uint8_t)(brightness * controller->green_weight * svencolors[i % 7].g);
        controller->led_colors[i].b = (uint8_t)(brightness * controller->blue_weight * svencolors[i % 7].b);
    }
    // ����LED��ɫ���ݵ�leds����
    for (i = start_index; i < start_index + length && i < num_leds; i++) {
        setLedColor(i, controller->led_colors[i]);
    }
    // ����LED������ʾ
    showLeds();
}

// ������ʽ����ʾ��5���߲ʺ�������ͬ��
void rainbowBreathSync(RGBController_t *controllers, uint8_t start_index, uint8_t length) {
    uint8_t red = 0, green = 0, blue = 0;
	uint8_t i = 0 ,j = 0;
	// ����ÿ���׶εĳ���ʱ�䣨�Ժ���Ϊ��λ��
    const uint16_t phase_duration = 4000.0; // ÿ���׶γ���1��
    static uint16_t fade_steps;// ÿ����ɫͨ���ĵ���/��������

    static uint8_t phase = 0; // �׶μ�����
    static uint8_t step = 0; // ����������
    // ���㵱ǰ�׶ε���ɫ
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
    // ����RGBController_t�ṹ���е���ɫֵ
    for (i = start_index; i < start_index + length; i++) {
        controllers->led_colors[i].r = red;
        controllers->led_colors[i].g = green;
        controllers->led_colors[i].b = blue;
    }	   	
    // ���Ӳ���������������ﵽfade_steps�����ӽ׶μ�����
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
    // ����LED��ɫ���ݵ�leds����
    for (j = start_index; j < start_index + length && j < num_leds; j++) {
        setLedColor(j, controllers->led_colors[j]);
    }
    // ����LED������ʾ
    showLeds();
}

// ������ʽ����ʾ��6���߲���ˮ��
// ��ˮ��Ч������
void rainbowEffect(RGBController_t* controllers, uint8_t start_index, uint8_t length) {
	
	static uint8_t iteration = 0, i;
	// ������ˮ��Ч�����ٶȣ���λΪ����
	uint16_t delay_ms = 500;
	static uint16_t num = 0;	
    // ������ˮ��Ч����ѭ���������������ִ�� 10 ��
//    uint8_t num_iterations = 10;	
    // ������ɫ����ĳ���
    uint8_t num_colors = sizeof(svencolors) / sizeof(svencolors[0]);	
    // ��ʼ��ˮ��Ч����ѭ��
    num++;
	if(num == delay_ms)
	{
		num = 0;
		controllers->led_colors[i++] = svencolors[iteration++];
		if(iteration >= num_colors)iteration = 0;
		if(i >= length) start_index = i;
		setLedColor(i, controllers->led_colors[i]);
		 // ����LED������ʾ
		showLeds();	
	}
}



RGBColor led_colors[num_leds];

void dev_rgb_attach_init(void)
{
	// ����RGB����������
//	rgb_init(&stdRgb, num_leds,0 ,4, led_colors);// ������ʼλ��Ϊ4������2��LED	
//	// �󶨿�����ʽ����
//	rgb_attach(&stdRgb,RGB_EVT_MODE0,singleColor);  //��ɫģʽ�¼�
	
//	// ����RGB����������
//	rgb_init(&stdRgb1, num_leds,4 ,4, led_colors);// ������ʼλ��Ϊ8������4��LED	
//	// �󶨿�����ʽ����
//	rgb_attach(&stdRgb1,RGB_EVT_MODE1,rainbowGradient); //�߲ʵ�ģʽ�¼�	

//	// ����RGB����������
//	rgb_init(&stdRgb2, num_leds,8 ,4, led_colors);// ������ʼλ��Ϊ12������4��LED	
//	// �󶨿�����ʽ����
//	rgb_attach(&stdRgb2,RGB_EVT_MODE2,fixedBreatheGradient);  //��ɫ�����¼�	
//	
//	// ����RGB����������
//	rgb_init(&stdRgb3, num_leds,12 ,4, led_colors);// ������ʼλ��Ϊ12������4��LED	
//	// �󶨿�����ʽ����
//	rgb_attach(&stdRgb3,RGB_EVT_MODE3,rainbowBreath);  //�߲ʵƽ����¼�	
	
	
	rgb_init(&stdRgb4, num_leds,0 ,16, led_colors);// ������ʼλ��Ϊ12������4��LED	
	// �󶨿�����ʽ����
	rgb_attach(&stdRgb4,RGB_EVT_MODE4,rainbowBreathSync);  //�߲ʵ�ͬ�������¼���	

//	rgb_init(&stdRgb4, num_leds,0 ,16, led_colors);// ������ʼλ��Ϊ12������4��LED	
//	// �󶨿�����ʽ����
//	rgb_attach(&stdRgb4,RGB_EVT_MODE5,rainbowEffect);  //�߲ʵ���ˮ���¼�
}




