/*
 * @Author: sven
 * @Date: 2024-09-18 15:51:58
 * @Last Modified by: sven
 * @Last Modified time: Do not edit
 * @FilePath: \device_driver\serial_device\DRIVERS\uart_driver\usart_driver.c
 * @Description: This file contains the implementation of a universal driver designed for flexibility and portability across various microcontroller platforms.
 * @Version: 1.0
 */
#include "usart_driver.h"

RingBuffer_t usartRxBuffer;  //接收缓冲队列
Frame_t receivedFrame;
FrameState state = FRAME_STATE_IDLE;
FrameStatus_t status = FRAME_IDLE;
// 回调函数数组
static FrameCallback_t callbacks[MAX_FUNCTION_CODE] = {0};

// 注册回调函数
void RegisterFrameCallback(uint8_t functionCode, FrameCallback_t callback) {
    if (functionCode < MAX_FUNCTION_CODE) {
        callbacks[functionCode] = callback;
    }  
}

// 调用回调函数
static void ExecuteCallback(uint8_t functionCode, uint8_t *data, uint16_t length) {
    if (functionCode < MAX_FUNCTION_CODE && callbacks[functionCode]) {
        callbacks[functionCode](data, length);
    }
}

// 初始化环形缓冲区
void RingBuffer_Init(RingBuffer_t *ringBuffer) {
    ringBuffer->head = 0;
    ringBuffer->tail = 0;
    ringBuffer->size = 0;
}

// 检查缓冲区是否为空
bool RingBuffer_IsEmpty(RingBuffer_t *ringBuffer) {
    return (ringBuffer->size == 0);
}

// 检查缓冲区是否已满
bool RingBuffer_IsFull(RingBuffer_t *ringBuffer) {
    return (ringBuffer->size >= BUFFER_SIZE);
}

// 清空当前缓冲区
void RingBufferClearRxMsg(RingBuffer_t *ringBuffer) {
    // 将缓冲区的head和tail指针都复位到初始状态，并将count清零
    ringBuffer->head = 0;
    ringBuffer->tail = 0;
	
	receivedFrame.checksum = 0;
	receivedFrame.functionCode = 0;
	receivedFrame.header = 0;
	receivedFrame.length = 0;
	receivedFrame.tail = 0;
	
    // 如果需要清空缓冲区中的数据，也可以将buffer中的数据置为0
    memset(ringBuffer->buffer, 0, sizeof(ringBuffer->buffer));
}

//获取缓冲区消息长度
uint16_t RingBuffer_GetLength(RingBuffer_t* ringBuffer)
{
	return ringBuffer->size <= 0 ? 0 : ringBuffer->size;
}

//获取接收帧消息消息
uint8_t RingBuffer_GetMsg(RingBuffer_t* ringBuffer)
{
	return ringBuffer->buffer[ringBuffer->tail];
}

// 向环形缓冲区中写入一个字节
bool RingBuffer_Write(RingBuffer_t *ringBuffer, uint8_t data) {
    if (RingBuffer_IsFull(ringBuffer)) {
        return false;  // 缓冲区已满，无法写入
    }

    ringBuffer->buffer[ringBuffer->tail] = data;
    ringBuffer->tail = (ringBuffer->tail + 1) % BUFFER_SIZE;
    ringBuffer->size++;
    return true;
}

// 从环形缓冲区中读取一个字节
bool RingBuffer_Read(RingBuffer_t *ringBuffer, uint8_t *data) {
	__disable_irq();
    if (RingBuffer_IsEmpty(ringBuffer)) {	
		__enable_irq();		
        return false;  // 缓冲区为空，无法读取
    }

    *data = ringBuffer->buffer[ringBuffer->head];
    ringBuffer->head = (ringBuffer->head + 1) % BUFFER_SIZE;
    ringBuffer->size--;
	__enable_irq();
    return true;
}

// 对应硬件串口中断，根据硬件自行修改
// void USART_IRQHandler(void) {
//     uint8_t receivedByte;

//     // 检查接收中断标志
//     if (USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET) {
//         receivedByte = USART_ReceiveData(USARTx) & 0xFF;  // 读取接收数据
        
//         // 将接收到的数据写入环形缓冲区
//         RingBuffer_Write(&usartRxBuffer, receivedByte);
//     }
// }
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//     if (huart->Instance == USART1) {  // 检查是否是我们使用的 UART 实例
//         // 将接收到的数据写入环形缓冲区
//         RingBuffer_Write(&uartRxRingBuffer, uartRxBuffer[0]);

//         // 重新启用接收中断
//         HAL_UART_Receive_IT(&huart1, uartRxBuffer, 1);
//     }
// }

// CRC校验
#define CRC16_POLY 0x1021  // CRC-16-CCITT多项式

uint16_t CalculateCRC16(uint8_t *data, uint16_t length) {
    uint16_t crc = 0xFFFF;  // 初始值为0xFFFF
    for (uint16_t i = 0; i < length; i++) {
        crc ^= (data[i] << 8);
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ CRC16_POLY;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

// 发送数据帧
void USART_SendFrame(uint16_t functionCode, uint8_t *data, uint16_t dataLength) {
//    Frame_t sendframe;
//    sendframe.header = FRAME_HEADER;
//    sendframe.functionCode = functionCode;
//    sendframe.length = dataLength;
//    sendframe.data = data;
//    sendframe.tail = FRAME_TAIL;

//    // 计算CRC校验
//    sendframe.checksum = CalculateCRC16(data, dataLength);

/* 根据硬件发送函数发送数据帧 */
    // // 发送帧头
    // USART_SendByte((sendframe.header >> 8) & 0xFF);
    // USART_SendByte(sendframe.header & 0xFF);

    // // 发送功能字
    // // USART_SendByte((sendframe.functionCode >> 8) & 0xFF);
    // USART_SendByte(sendframe.functionCode & 0xFF);

    // // 发送数据长度
    // USART_SendByte((sendframe.length >> 8) & 0xFF);
    // USART_SendByte(sendframe.length & 0xFF);

    // // 发送数据域
    // for (uint16_t i = 0; i < dataLength; i++) {
    //     USART_SendByte(sendframe.data[i]);
    // }

    // // 发送CRC校验
    // USART_SendByte((sendframe.checksum >> 8) & 0xFF);
    // USART_SendByte(sendframe.checksum & 0xFF);

    // // 发送帧尾
    // USART_SendByte((sendframe.tail >> 8) & 0xFF);
    // USART_SendByte(sendframe.tail & 0xFF);
}

// 接收数据帧并返回解析状态
uint8_t byte = 0,len = 0,crc_len = 0,tail_len = 0;

FrameStatus_t USART_ReceiveFrame(Frame_t *frame) {
	static uint8_t frame_len[2] , frame_crc[2] , frame_tail[2];
	switch(state)
	{
		case FRAME_STATE_DONE:
		{
			state = FRAME_STATE_HEADER;
		}
		break;
		
		case FRAME_STATE_HEADER:
			if(RingBuffer_Read(&usartRxBuffer, &byte))
			{
				if(byte == ((FRAME_HEADER >> 8) & 0xFF))
				{
					frame->header |= (byte << 8);
				}
				else if(byte == ((FRAME_HEADER) & 0xFF))
				{
					frame->header |= byte;
					state = FRAME_STATE_FUNCTION;					
				}
				else {
					RingBufferClearRxMsg(&usartRxBuffer);
					state = FRAME_STATE_IDLE;				
					return FRAME_HEADER_ERROR;
				}
			}
		break;
			
		case FRAME_STATE_FUNCTION:
			if(RingBuffer_Read(&usartRxBuffer, &byte))
			{
				frame->functionCode = byte;
				if (frame->functionCode > 0) 
				{  // 假设功能字非零有效
					state = FRAME_STATE_LENGTH;
				}
				else{
					RingBufferClearRxMsg(&usartRxBuffer);
					state = FRAME_STATE_IDLE;
					return FRAME_FUNCTION_ERROR;
				}
			}
		break;	
			
		case FRAME_STATE_LENGTH:
			if(RingBuffer_Read(&usartRxBuffer,&frame_len[len++]))
			{
				if (len == 2) {
					frame->length = (frame_len[0] << 8) | frame_len[1];
					len = 0;
					if (frame->length == 0) {
						RingBufferClearRxMsg(&usartRxBuffer);
						state = FRAME_STATE_IDLE;
						return FRAME_LENGTH_ERROR;
					}
					frame->data = (uint8_t *)malloc(frame->length);
					if (!frame->data) {
						RingBufferClearRxMsg(&usartRxBuffer);
						state = FRAME_STATE_IDLE;
						free(frame->data);
						return FRAME_MEMORY_ERROR;
					}
					state = FRAME_STATE_DATA;
				}				
			}
		break;	
			
		case FRAME_STATE_DATA:
			// 读取数据域
			for (uint16_t i = 0; i < frame->length; i++) {
				if (RingBuffer_Read(&usartRxBuffer, &frame->data[i])) {
					continue;
				} else {
					RingBufferClearRxMsg(&usartRxBuffer);
					state = FRAME_STATE_IDLE;
					free(frame->data);
				}
			}
			state = FRAME_STATE_CHECKSUM;
		break;	
			
		case FRAME_STATE_CHECKSUM:
			// 读取CRC校验
			if (RingBuffer_Read(&usartRxBuffer, &frame_crc[crc_len++])) {
				if (crc_len == 2) {
					frame->checksum = (frame_crc[0] << 8) | frame_crc[1];
					crc_len = 0;
					if (frame->checksum == CalculateCRC16(frame->data, frame->length)) {
						state = FRAME_STATE_TAIL;
					} else {
						RingBufferClearRxMsg(&usartRxBuffer);
						state = FRAME_STATE_IDLE;
						free(frame->data);
						return FRAME_CRC_ERROR;
					}
				}
			}
		break;	
		
		case FRAME_STATE_TAIL:
			// 读取帧尾
			if (RingBuffer_Read(&usartRxBuffer, &frame_tail[tail_len++])) {
				if (tail_len == 2) {
					frame->tail = (frame_tail[0] << 8) | frame_tail[1];
					tail_len = 0;
					if (frame->tail == FRAME_TAIL) {
						ExecuteCallback(frame->functionCode, frame->data, frame->length);
						free(frame->data);
						state = FRAME_STATE_HEADER;
						RingBufferClearRxMsg(&usartRxBuffer);
						state = FRAME_STATE_IDLE;						
						return FRAME_OK;

					} else {
						RingBufferClearRxMsg(&usartRxBuffer);
						state = FRAME_STATE_IDLE;
						free(frame->data);
						return FRAME_TAIL_ERROR;
					}
				}
			}
		break;	
		default:
		break;			
	}
	return FRAME_IDLE;
}


//测试回调函数Demo
void LedControlCallback(uint8_t *data, uint16_t length) {
//    if (length < 1) return;  // 检查数据长度
//    uint8_t ledState = data[0];  // 假设第一个字节是LED状态
//    if (ledState == 1) {
//        // 打开LED
//    } else {
//        // 关闭LED
//    }
}

void MotorControlCallback(uint8_t *data, uint16_t length) {
//    if (length < 2) return;  // 检查数据长度
//    uint16_t speed = (data[0] << 8) | data[1];  // 假设前两个字节表示电机速度
//    // 设置电机速度
}

void SensorDataCallback(uint8_t *data, uint16_t length) {
//    if (length < 4) return;  // 检查数据长度
//    uint32_t sensorValue = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];  // 假设前4个字节是传感器值
//    // 处理传感器数据
}

void FrameParse_Init(void)
{
    RingBuffer_Init(&usartRxBuffer);

    // 注册回调函数
    RegisterFrameCallback(0x01, LedControlCallback);
    RegisterFrameCallback(0x02, MotorControlCallback);
    RegisterFrameCallback(0x03, SensorDataCallback);
}

//测试代码如下
void FrameParse_Test(void)
{
	status = USART_ReceiveFrame(&receivedFrame);
	switch (status) {
		case FRAME_OK:
			// 解析成功，已在 USART_ReceiveFrame 中调用回调
			
			break;
		case FRAME_HEADER_ERROR:
			// 处理帧头错误
			break;
		case FRAME_TAIL_ERROR:
			// 处理帧尾错误
			break;
		case FRAME_CRC_ERROR:
			// 处理 CRC 校验错误
			break;
		case FRAME_LENGTH_ERROR:
			// 处理数据长度错误
			break;
		default:
			break;
	}
}

void uart_driver_test(void)
{
	FrameParse_Init();

	while(1)
	{
		FrameParse_Test();
	}
}

