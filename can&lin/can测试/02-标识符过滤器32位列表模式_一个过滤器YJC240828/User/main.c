#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "MyCAN.h"

uint8_t KeyNum;

//uint32_t TxID = 0x555;
//uint8_t TxLength = 4;
//uint8_t TxData[] = {0x11, 0x22, 0x33, 0x44};
                 /***  StdId    ExtId            IDE          RTR         DLC      Data[4] ****/
//CanTxMsg TxMessage ={0x555, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}};     //标准ID  数据帧  OK 
//CanTxMsg TxMessage ={0x666, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote,   0, {0x00, 0x00, 0x00, 0x00}};   //标准ID  遥控帧   ?
//CanTxMsg TxMessage ={0x000, 0x12345678, CAN_Id_Extended, CAN_RTR_Data,   4, {0x55, 0xAA, 0x55, 0xAA}};     //扩展ID  数据帧  OK
//CanTxMsg TxMessage ={0x000, 0x1ABBCCDD, CAN_Id_Extended, CAN_RTR_Remote, 0, {0x00, 0x00, 0x00, 0x00}};     //扩展ID  遥控帧     ?
CanTxMsg TxMessageArray[] ={
	              /** StdId     ExtId          IDE            RTR      DLC          Data[4]***/
                     {0x123, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x12, 0x12, 0x12, 0x12}},
                     {0x234, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x23, 0x23, 0x23, 0x23}},
					 {0x345, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x34, 0x34, 0x34, 0x34}},
					 {0x456, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x45, 0x45, 0x45, 0x45}},
					 {0x000, 0x12345678, CAN_Id_Extended, CAN_RTR_Data, 4, {0x55, 0x55, 0x55, 0x55}},
					 {0x000, 0x0789ABCD, CAN_Id_Extended, CAN_RTR_Data, 4, {0x66, 0x66, 0x66, 0x66}},
					        };                         //复合结构体数组

uint8_t pTxMessageArray;   //定义数组索引号
//uint32_t RxID;
//uint8_t RxLength;
//uint8_t RxData[8];
CanRxMsg RxMessage;

int main(void)
{
	OLED_Init();
	Key_Init();
	MyCAN_Init();
	
	OLED_ShowString(1, 1, "Rx:");
	OLED_ShowString(2, 1, "RxID:");
	OLED_ShowString(3, 1, "Leng:");
	OLED_ShowString(4, 1, "Data:");
	
	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			MyCAN_Transmit(&TxMessageArray[pTxMessageArray]);
			pTxMessageArray++;
			if(pTxMessageArray >= 6)
			{                                     //判断数组是否越界
				pTxMessageArray = 0;
			}
		}
		
		if (MyCAN_ReceiveFlag())
		{
			MyCAN_Receive(&RxMessage);
			if(RxMessage.IDE == CAN_Id_Standard)
			{
				OLED_ShowString(1, 6,"StdId");
				OLED_ShowHexNum(2, 6, RxMessage.StdId,   8);
			}
			else if(RxMessage.IDE == CAN_Id_Extended)
			{
				OLED_ShowString(1, 6,"ExtId");
				OLED_ShowHexNum(2, 6,  RxMessage.ExtId,   8);
			}
			
			if(RxMessage.RTR == CAN_RTR_Data)
			{
				OLED_ShowHexNum(3, 6,  RxMessage.DLC,     1);
			    OLED_ShowHexNum(4, 6,  RxMessage.Data[0], 2);
			    OLED_ShowHexNum(4, 9,  RxMessage.Data[1], 2);
			    OLED_ShowHexNum(4, 12, RxMessage.Data[2], 2);
			    OLED_ShowHexNum(4, 15, RxMessage.Data[3], 2);
			}
			else if(RxMessage.RTR == CAN_RTR_Remote)
			{
				OLED_ShowHexNum(3, 6,  RxMessage.DLC, 1);
			    OLED_ShowHexNum(4, 6,  0x00, 2);
	            OLED_ShowHexNum(4, 9,  0x00, 2);
			    OLED_ShowHexNum(4, 12, 0x00, 2);
			    OLED_ShowHexNum(4, 15, 0x00, 2);
		    }
		}
	}
}
