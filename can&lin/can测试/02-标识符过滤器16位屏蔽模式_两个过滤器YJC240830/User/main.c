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
	                 /**StId   ExtId          IDE            RTR       DLC          Data[4]***/
                     {0x100, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x10, 0x10, 0x10, 0x10}},
                     {0x101, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x11, 0x11, 0x11, 0x11}},
					 
					 {0x200, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x20, 0x20, 0x20, 0x20}},
					 {0x201, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 4, {0x21, 0x21, 0x21, 0x21}},
					 {0x202, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x22, 0x22, 0x22, 0x22}},
					 {0x2FF, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 4, {0x22, 0x22, 0x22, 0x22}},
					 
					 {0x310, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x30, 0x30, 0x30, 0x30}},
					 {0x311, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x31, 0x31, 0x31, 0x31}},
					 {0x31F, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x11, 0x11, 0x11, 0x11}},
					 {0x331, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x22, 0x22, 0x22, 0x22}},
					 
					 {0x320, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x30, 0x30, 0x30, 0x30}},
					 {0x321, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 4, {0x31, 0x31, 0x31, 0x31}},
					 {0x322, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x32, 0x32, 0x32, 0x32}},
					 {0x32F, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 4, {0x32, 0x32, 0x32, 0x32}},
					 
					 {0x420, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x55, 0x55, 0x55, 0x55}},
					 {0x421, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 4, {0x66, 0x66, 0x66, 0x66}},
					 {0x422, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x77, 0x77, 0x77, 0x77}},
					 {0x42F, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 4, {0xEE, 0xEE, 0xEE, 0xEE}},
					 {0x431, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0xFF, 0xFF, 0xFF, 0xFF}},
					 
					 {0x520, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x88, 0x88, 0x88, 0x88}},
					 {0x521, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 4, {0x99, 0x99, 0x99, 0x99}},
					 {0x522, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0xAA, 0xAA, 0xAA, 0xAA}},
					 {0x52F, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 4, {0xBB, 0xBB, 0xBB, 0xBB}},
					 {0x530, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0xCC, 0xCC, 0xCC, 0xCC}}
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
//			if(pTxMessageArray >= 20)
			if(pTxMessageArray >= (sizeof(TxMessageArray)/sizeof(CanTxMsg))) //该种方法计算的数组元素值与上一个直接写数组元素数量是等价的
			{                                                                //判断数组是否越界
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
