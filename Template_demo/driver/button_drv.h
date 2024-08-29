/*
 * @Author: SVEN 2847095716@qq.com
 * @Date: 2024-04-16 17:55:44
 * @LastEditors: SVEN 2847095716@qq.com
 * @LastEditTime: 2024-04-17 10:32:22
 * @FilePath: \Template_demo\APP\app_key.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: SVEN 2847095716@qq.com
 * @Date: 2024-04-16 17:55:44
 * @LastEditors: SVEN 2847095716@qq.com
 * @LastEditTime: 2024-04-17 10:32:19
 * @FilePath: \Template_demo\APP\app_key.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __BUTTON_DRV_H
#define __BUTTON_DRV_H

#include "general_type.h"

/**@brief 按键回调函数类型定义 */	
typedef void (*BtnCallback) (void *p_arg);

typedef enum{
    BTN_ACTIVE_LEVEL_LOW,					/* 键按下时低电平有效 */
    BTN_ACTIVE_LEVEL_HIGH					/* 键按下时高电平有效 */	
}BUTTON_ActiveLevel_t;	

		
/**@brief 按键事件类型定义 */    	
typedef enum 	
{	
    BTN_EVT_DOWN,                       	/* 键按下事件 		  */
    BTN_EVT_UP,                         	/* 键弹起事件 		  */
    BTN_EVT_SINGLE,                     	/* 键单击事件 		  */
    BTN_EVT_DOUBLE,                     	/* 键双击事件 		  */
    BTN_EVT_LONG,                       	/* 键长击事件 		  */
    BTN_EVT_LONG_HOLD,                  	/* 键连击事件 		  */
    BTN_EVT_SUM,                        	/* 键事件总数 		  */
    BTN_EVT_NONE                        	/* 键没有事件 		  */
} BUTTON_Event_t;	

/**@brief 按键结构类型定义 */ 	
typedef struct Button 	
{	
    INT16U       Ticks;                 	/* 键按下嘀嗒计数器   */
	INT16U       LongTicks;             	/* 键长击嘀嗒数       */
	INT8U        LongHoldPeriod;        	/* 键连击周期         */
	INT8U        ShortTicks;            	/* 键单击嘀嗒数       */
    INT8U        Repeat     :4;        		/* 击键计数变量       */
    INT8U        Event      :4;        		/* 键事件变量         */
    INT8U        State      :3;        		/* 键状态变量         */
    INT8U        DebounceCnt:3;        		/* 键消抖计数         */
    INT8U        ActiveLevel:1;        		/* 键有效电平         */
    INT8U        LastLevel  :1;        		/* 键电平备份         */
    INT8U        (*GetPinLevel)(void);  	/* 键获取引脚电平回调 */
    BtnCallback  CallBack[BTN_EVT_SUM]; 	/* 键事件处理回调     */
    struct Button *Next;
} BUTTON_t;

/**@brief 函数声明
 * 
 * @{
 */
extern INT8U 		  button_init(BUTTON_t *pstBtn, INT8U(*GetPinLevel)(void), BUTTON_ActiveLevel_t ActiveLevel);
extern void 		  button_set_time_param(BUTTON_t *pstBtn, INT8U ShortTicks, INT16U LongTicks, INT8U LongHoldPeriod);
extern void 		  button_attach(BUTTON_t *pstBtn, BUTTON_Event_t Event, BtnCallback CallBack);
extern BUTTON_Event_t button_get_event(BUTTON_t *pstBtn);
extern void 		  button_scan(void);
extern void 		  button_reset(void);

/***@}
  */

#ifdef __cplusplus
} 
#endif
#endif
