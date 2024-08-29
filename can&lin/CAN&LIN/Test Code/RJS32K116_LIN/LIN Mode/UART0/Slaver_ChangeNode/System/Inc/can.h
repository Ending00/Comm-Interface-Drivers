#ifndef __CAN_H__
#define __CAN_H__

#include "rjs32k116.h"

#define CAN_DLC_12BYTES         0X09
#define CAN_DLC_16BYTES         0X0A
#define CAN_DLC_20BYTES         0X0B
#define CAN_DLC_24BYTES         0X0C
#define CAN_DLC_32BYTES         0X0D
#define CAN_DLC_48BYTES         0X0E
#define CAN_DLC_64BYTES         0X0F

#define CAN_20B                 0x00      
#define CAN_FD_ISO              0X01
#define CAN_FD_NONISO           0X02

#define CAN_FRAME               0
#define CANFD_FRAME             1

#define CAN_ID_STD              0
#define CAN_ID_EXT              1
#define CAN_RTR_DATA            0
#define CAN_RTR_REMOTE          1

#define PTB                     0
#define STB                     1



typedef struct
{
    uint32_t    ID;             //CAN identifier
    uint8_t     BRS;            //Bit Rate Switch
    uint8_t     FDF;            //FD Format indicator(= EDL)
    uint8_t     RTR;            //Remote transmission request frame
    uint8_t     IDE;            //Identifier Extension
    uint8_t     DLC;            //Data length code
    uint8_t     TTSEN;
    uint8_t     *Data;          //Data
}CAN_TxMsg;

typedef struct
{
    uint32_t    ID;             //CAN identifier
    uint8_t     ESI;            //Error State Indicator
    uint8_t     BRS;            //Bit Rate Switch
    uint8_t     FDF;            //FD Format indicator(= EDL)
    uint8_t     RTR;            //Remote transmission request frame
    uint8_t     IDE;            //Identifier Extension
    uint8_t     DLC;            //Data length code
    uint32_t    Data[2];        //Data
    uint32_t    RTS[2];
}CAN_RxMsg;

typedef struct
{
    uint8_t          Filter_ChannelNum;
    FunctionalState  Filter_Status;
    FunctionalState  AIDEE;             //接受掩码IDE位检查使能置，需设置SELMASK等于1
    FunctionalState  AIDE;              //接受掩码IDE位的值置，需设置SELMASK等于1
    uint32_t         Acode;
    uint32_t         Amask;
} CAN_FilterType;

typedef struct
{
    uint8_t Filter_Number;
    CAN_FilterType *CANFilterList;
    uint8_t CAN_WorkMode;
} CAN_ConfigType;


void CAN_Init(CAN_ConfigType *CANConfig);
void CAN_Transmit(CAN_TxMsg *TxMessage, uint8_t CAN_WorkMode);
void CAN_Receive(CAN_RxMsg *RxMessage, uint8_t CAN_WorkMode);

#endif

