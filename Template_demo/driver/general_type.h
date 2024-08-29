/**
 **************************************************************************
 *			    Copyright(c) 2024 LW, All Rights Reserved.
 **************************************************************************
 * @fileName   general_type.h
 * @author     LW
 * @date       2024.04.18
 * @version    V1.00
 * @brief      数据类型定义
 **************************************************************************
 * @history
 * 版 本       V1.00
 * 作 者       LW
 * 日 期       2024.04.18
 * 修 改       无			 	        
 **************************************************************************
 *
 */
#ifndef __GENERAL_TYPE_H__
#define __GENERAL_TYPE_H__
#include <stdbool.h>
#include <string.h>

/**@brief 存储与数据类型定义
 *
 *@{
 */

#ifndef NULL
#define NULL			         ((void*)0)
#endif

//#ifndef TRUE
//#define TRUE			         (1 == 1)
//#endif

//#ifndef FALSE
//#define FALSE			         (0 == 1)
//#endif

#ifndef BOOL
typedef unsigned         char    BOOL;
#endif

#ifndef STRING
typedef                  char*   STRING;
#endif

#ifndef CHAR
typedef            		 char    CHAR;
#endif

#ifndef INT8S
typedef signed           char    INT8S;
#endif

#ifndef INT8U
typedef unsigned         char    INT8U;
typedef unsigned         char    uint8_t;
#endif

#ifndef INT16S
typedef signed   short   int     INT16S;
#endif

#ifndef INT16U
typedef unsigned short   int     INT16U;
#endif

#ifndef INT32S
typedef signed   long    int     INT32S;
#endif

#ifndef INT32U
typedef unsigned long    int     INT32U;
#endif

#ifndef FP32
typedef float                    FP32;
#endif

#ifndef FP64
typedef double                   FP64;
#endif

/**@}
 */

#endif


