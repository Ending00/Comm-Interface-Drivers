/*
 * @Author: sven
 * @Date: 2024-09-18 15:51:58
 * @Last Modified by: sven
 * @Last Modified time: Do not edit
 * @FilePath: \device_driver\serial_device\DRIVERS\uart_driver\usart_driver.h
 * @Description: This file contains the implementation of a universal driver designed for flexibility and portability across various microcontroller platforms.
 * @Version: 1.0
 */
// usart_driver.h
#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include "stdint.h"
#include "stdbool.h"
#include <stdlib.h>
#include "string.h"

#define FRAME_HEADER 0xAA55  // 帧头
#define FRAME_TAIL 0x55AA    // 帧尾

#define BUFFER_SIZE 256         // 环形缓冲区大小
#define MAX_FUNCTION_CODE 0xFF   // 最大功能数大小

//回调函数类型 用户根据需求自行修改 根据功能自动链接到对应回调
// typedef void (*FrameCallback_t)(void* data, uint16_t lenght);
typedef void (*FrameCallback_t)(uint8_t* data, uint16_t lenght);

// 帧状态
typedef enum {
	FRAME_STATE_IDLE,
    FRAME_STATE_HEADER,
    FRAME_STATE_FUNCTION,
    FRAME_STATE_LENGTH,
    FRAME_STATE_DATA,
    FRAME_STATE_CHECKSUM,
    FRAME_STATE_TAIL,
	FRAME_STATE_DONE
} FrameState;

// 帧结构
typedef struct {
    uint16_t header;       // 帧头
    uint8_t functionCode; // 功能字
    uint16_t length;       // 数据长度
    uint8_t *data;         // 数据域
    uint16_t checksum;     // 校验和
    uint16_t tail;         // 帧尾
} Frame_t;

// 帧接收状态
typedef enum {
	FRAME_IDLE = 0,
    FRAME_OK,            // 成功
    FRAME_HEADER_ERROR,      // 帧头错误
	FRAME_FUNCTION_ERROR,
    FRAME_TAIL_ERROR,        // 帧尾错误
    FRAME_CRC_ERROR,         // CRC 校验失败
    FRAME_LENGTH_ERROR,      // 数据长度错误
	FRAME_MEMORY_ERROR,
} FrameStatus_t;

// 环形缓冲区 用于接收缓冲数据
typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    uint16_t head;  // 指向写入位置
    uint16_t tail;  // 指向读取位置
    uint16_t size;  // 当前缓冲区中的数据量
} RingBuffer_t;

extern RingBuffer_t usartRxBuffer;
extern Frame_t receivedFrame;
extern FrameStatus_t status;
extern FrameState state;
void FrameParse_Test(void);
FrameStatus_t USART_ReceiveFrame(Frame_t *frame);
void USART_SendFrame(uint16_t functionCode, uint8_t *data, uint16_t dataLength);
bool RingBuffer_Read(RingBuffer_t *ringBuffer, uint8_t *data);
bool RingBuffer_Write(RingBuffer_t *ringBuffer, uint8_t data);
uint8_t RingBuffer_GetMsg(RingBuffer_t* ringBuffer);
void FrameParse_Init(void);
uint16_t CalculateCRC16(uint8_t *data, uint16_t length);
void uart_driver_test(void);
#endif // USART_DRIVER_H
