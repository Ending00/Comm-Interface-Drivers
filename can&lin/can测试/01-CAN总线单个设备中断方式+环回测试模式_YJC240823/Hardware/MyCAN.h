#ifndef __MyCAN_H
#define __MyCAN_H


extern uint8_t MyCAN_ReceiveFlag1;

void MyCAN_Init(void);
void MyCAN_TransmitMessage(uint32_t ID, uint8_t Length, uint8_t* Data);

uint8_t MyCAN_ReceiveFlag(void);

void MyCAN_ReceiveMessage(uint32_t* ID, uint8_t* Length, uint8_t* Data);

uint8_t MyCAN_GetReceiveFlag(void);


void CAN1_RX1_IRQnHanlder(void);


#endif
