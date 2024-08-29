#ifndef __LIN_H
#define __LIN_H
#include "sys.h"	
#include <string.h>

void LIN_Mode_Init(u32 bound);
void Lin_SendAnswer(u8 id ,u8 data[]);
void Lin_SentData(u8 data[]);
u8 Lin_Checksum(u8 id , u8 data[]);
void Lin_SendHead(u8 id);
u8 Lin_CheckPID(u8 id);
void Lin_SendSyncSegment(void);
void Lin_SendBreak(void);
void Lin_DataProcess(void);

#endif





















