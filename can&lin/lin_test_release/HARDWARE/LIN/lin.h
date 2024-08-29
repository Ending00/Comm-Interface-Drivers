#ifndef __LIN_H
#define __LIN_H
#include "sys.h"	
#include <string.h>
#include "lin_driver.h"

#define  LIN_RINGBUF_LEN  64             //缓冲区长度
 
typedef struct{
	uint16_t Head;						//队列头
	uint16_t Tail;						//队列尾
	uint16_t Length; 					//保存的数据长度
	uint16_t BufOutcnt;                	//溢出计数
	LIN_MSG  LINBuffer[LIN_RINGBUF_LEN]; 	//缓冲区	
}LIN_BUFFER;


void LIN_Mode_Init(u32 bound);

void LIN_RingBUF_Init(LIN_BUFFER* pLINBuff);
LIN_MSG* LIN_RingBUF_GetMsg(LIN_BUFFER* pLINBuff);
void LIN_RingBUF_Write(LIN_BUFFER* pLINBuff);
void LIN_RingBUF_ClearRxMsg(LIN_BUFFER* pLINBuf);
uint16_t LIN_RingBUF_ReadLen(LIN_BUFFER* pLINBuff);
int LIN_RingBUF_Read(LIN_BUFFER* pLINBuff, LIN_MSG** pLINMsg);

uint8_t LIN_GetPID(uint8_t ID); 
uint8_t LIN_GetChecksum(uint8_t PID, uint8_t* pData,uint8_t DataLen,uint8_t flag);
void LIN_SendBytes(USART_TypeDef* USARTx,uint8_t* pData,uint8_t DataLen);
void LIN_Send_data(uint8_t PID, uint8_t* pData,uint8_t DataLen);

void Lin_SendHead(u8 id);
void Lin_SendSyncSegment(void);
void Lin_SendBreak(void);

#endif





















