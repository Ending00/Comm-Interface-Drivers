#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "MyCAN.h"

uint32_t Tx_ID = 0x555;
uint8_t Tx_Length = 4;
uint8_t Tx_Message[] = {0x11, 0x12, 0x13, 0x14};

uint32_t Rx_ID;
uint8_t Rx_Length;
uint8_t Rx_Message[8];
uint8_t KeyNum;

uint8_t Tx_Message1[] ={0x01, 0x02, 0x03};


int main(void)
{
	/***ģ���ʼ��***/
	OLED_Init();
	Key_Init();
	MyCAN_Init();
	
	/***OLED��ʾ����ʾ***/
	OLED_ShowString(1, 1, "Tx_ID:");
	OLED_ShowHexNum(1, 8,  Tx_ID, 3);
	OLED_ShowString(2, 1, "Tx_MS:");
	
	OLED_ShowString(3, 1, "Rx_ID:");
	OLED_ShowString(4, 1, "Rx_MS:");
	
	
	
	
	while (1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			Tx_Message[0]++;
			Tx_Message[1]++;
			Tx_Message[2]++;
			Tx_Message[3]++;                                  //��ʽ1�����ͱ��ġ���ʾ���ġ����䱨�ĺ����ѵ�ͨ
			OLED_ShowHexNum(2, 8,  Tx_Message[0], 2);
	        OLED_ShowHexNum(2, 10, Tx_Message[1], 2);
	        OLED_ShowHexNum(2, 12, Tx_Message[2], 2);
	        OLED_ShowHexNum(2, 14, Tx_Message[3], 2);
			MyCAN_TransmitMessage(Tx_ID, Tx_Length, Tx_Message);
			
			
//			Tx_Message1[0]++;
//			Tx_Message1[1]++;
//			Tx_Message1[2]++;
//			OLED_ShowHexNum(2, 8,  Tx_Message1[0], 2);       //��ʽ2�����ͱ��ġ���ʾ���ġ����䱨�ĺ����ѵ�ͨ      
//	        OLED_ShowHexNum(2, 11, Tx_Message1[1], 2);
//			OLED_ShowHexNum(2, 14, Tx_Message1[2], 2);
//			MyCAN_TransmitMessage(0x555, 3, Tx_Message1);
		}
		
//	/*****���ò�ѯ��־λ�ķ�ʽʵ�ֱ�׼ID����֡��ʽ�ı��ķ��ͺͽ���****/
//	if(MyCAN_ReceiveFlag() == SET)
//	{
//		MyCAN_ReceiveMessage(&Rx_ID, &Rx_Length, Rx_Message);
//		OLED_ShowHexNum(3, 8, Rx_ID, 3);
//		OLED_ShowHexNum(3, 14, Rx_Length, 1);
//		OLED_ShowHexNum(4, 8,  Rx_Message[0], 2);
//		OLED_ShowHexNum(4, 10, Rx_Message[1], 2);        //��ʽ1�����ձ��ġ���ʾ���ձ��ġ����ձ��ĺ����ѵ�ͨ
//		OLED_ShowHexNum(4, 12, Rx_Message[2], 2);
//		OLED_ShowHexNum(4, 14, Rx_Message[3], 2);
//		
////		uint32_t Rx_ID;
////		uint8_t Rx_Length;
////		uint8_t Rx_Message[8];
////		MyCAN_ReceiveMessage(&Rx_ID, &Rx_Length, Rx_Message);
////		OLED_ShowHexNum(3, 8, Rx_ID, 3);
////		OLED_ShowHexNum(3, 14, Rx_Length, 1);           //��ʽ2�����ձ��ġ�������ʾ���ġ����ձ��ĺ����ѵ�ͨ       
////		OLED_ShowHexNum(4, 8,  Rx_Message[0], 2);
////		OLED_ShowHexNum(4, 11, Rx_Message[1], 2);
////		OLED_ShowHexNum(4, 14, Rx_Message[2], 2);
////		
////		for(uint8_t i=0; i< Rx_Length; i++)
////		{
////			OLED_ShowHexNum(4, i*3 + 6, Rx_Message[i], 2);			
////		}
//	}


       /*****�����жϷ�ʽʵ�ֱ�׼ID������֡��ʽ�ı��ķ��ͺͽ���****/
		if(MyCAN_GetReceiveFlag())
		{
			MyCAN_ReceiveMessage(&Rx_ID, &Rx_Length, Rx_Message);
			OLED_ShowHexNum(3, 8, Rx_ID, 3);
			OLED_ShowHexNum(3, 14, Rx_Length, 1);
			OLED_ShowHexNum(4, 8,  Rx_Message[0], 2);
			OLED_ShowHexNum(4, 10, Rx_Message[1], 2);        //��ʽ1�����ձ��ġ���ʾ���ձ��ġ����ձ��ĺ����ѵ�ͨ
			OLED_ShowHexNum(4, 12, Rx_Message[2], 2);
			OLED_ShowHexNum(4, 14, Rx_Message[3], 2);
		}


	}
}
