#include "common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 AP+STAģʽ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 


////ATK-ESP8266 AP+STAģʽ����
////���ڲ���TCP/UDP����
////����ֵ:0,����
////    ����,�������
//u8 atk_8266_apsta_test(void)
//{
//	u8 netpro;
//	u8 ipbuf[16]; 	//IP����
//	u8 *p;
//	u8 res=0;
//	u16 rlen=0;
//	u8 constate=0;	//����״̬
//	p=mymalloc(32);							//����32�ֽ��ڴ�
//	atk_8266_send_cmd("AT+CWMODE=3","OK",50);		//����WIFI AP+STAģʽ
////	atk_8266_send_cmd("AT+RST","OK",20);		//����ģ�� 
////	delay_ms(1000);         //��ʱ2S�ȴ������ɹ�
////	delay_ms(1000);
////	delay_ms(1000);
//	//����ģ��APģʽ��WIFI��������/���ܷ�ʽ/���룬�⼸���������Լ�ϲ������
//	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);//�������߲���:ssid,����
//	atk_8266_send_cmd(p,"OK",1000);					//����APģʽ����
//	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
//	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
//	atk_8266_send_cmd(p,"WIFI GOT IP",1000);						//����Ŀ��·�����������IP
//	while(atk_8266_send_cmd("AT+CIFSR","STAIP",20));   //����Ƿ���STA IP
//	while(atk_8266_send_cmd("AT+CIFSR","APIP",20));   //����Ƿ���AP IP
////	while((atk_8266_consta_check()-'2'));   //���IP

//	atk_8266_send_cmd("AT+CIPMUX=1","OK",50);   //0�������ӣ�1��������
//	delay_ms(500);
//	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
//	atk_8266_send_cmd(p,"OK",50);     //����Serverģʽ���˿ں�Ϊ8086
//	delay_ms(500);
//	atk_8266_send_cmd("AT+CIPSTO=1200","OK",50);     //���÷�������ʱʱ��
//	
//PRESTA:
//	netpro=atk_8266_netpro_sel((u8*)ATK_ESP8266_CWMODE_TBL[0]);	//AP+STAģʽ����ģʽѡ��
////	if(netpro&0X02)   //STA UDP
////	{
//				if(atk_8266_ip_set("WIFI-STAԶ��UDP IP����","UDPģʽ",(u8*)portnum,ipbuf))goto PRESTA;	//IP����
//				sprintf((char*)p,"AT+CIPSTART=0,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������,��ID�ţ�STAģʽ��Ϊ0
//				printf("ATK-ESP WIFI-STA����\r\n"); 
//				printf("��������ATK-ESPģ��,���Ե�...\r\n");
//				atk_8266_send_cmd(p,"OK",200);
//				netpro=atk_8266_mode_cofig(ATK_ESP8266_WORKMODE_TBL[2]);     //APģʽ����ģʽ����			
//	}
//	else     //TCP
//	{
//		if(netpro&0X01)     //STA TCP Client  
//		{
//			if(atk_8266_ip_set("WIFI-STA Զ��IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
//			sprintf((char*)p,"AT+CIPSTART=0,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������,��ID�ţ�STAģʽ��Ϊ0
//			while(atk_8266_send_cmd(p,"OK",200))
//			{

//			}	
//			netpro=atk_8266_mode_cofig(netpro);     //APģʽ����ģʽ����	
//		}
//		else netpro=atk_8266_mode_cofig(netpro);     //TCP SERVER��������
//	}

//	atk_8266_get_ip();//STA+APģʽ,��ȡ IP,����ʾ	

////	atk_8266_wificonf_show(15,195,"������·�������߲���Ϊ:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);		
//	USART2_RX_STA=0;
//	while(1)
//	{
////		key=KEY_Scan(0);
////		if(key==WKUP_PRES)			//WK_UP �˳�����		 
////		{ 
////			res=0;	
////			break;												 
////		}
////		else if(key==KEY0_PRES)	//KEY0 ��ID0�������� 
////		{
////			  sprintf((char*)p,"ATK-8266ģ��ID0������%02d\r\n",t/10);//��������
////				Show_Str(15+54,125,200,12,p,12,0);
////				atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //����ָ�����ȵ�����
////				delay_ms(200);
////				atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
////				timex=100;
////		}
////		else if(key==KEY1_PRES)	//KEY1 ��ID1�������� 
////		{
////			  sprintf((char*)p,"ATK-8266ģ��ID1������%02d\r\n",t/10);//��������
////				Show_Str(15+54,125,200,12,p,12,0);
////				atk_8266_send_cmd("AT+CIPSEND=1,25","OK",200);  //����ָ�����ȵ�����
////				delay_ms(200);
////				atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
////				timex=100;
////		}
//			
////	 if(timex)timex--;
//// 	 if(timex==1)LCD_Fill(30+54,125,239,122,WHITE);
////	 t++;
//	 delay_ms(10);
//	 if(USART2_RX_STA&0X8000)		//���յ�һ��������
//	{ 
//		rlen=USART2_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
//		USART2_RX_BUF[rlen]=0;		//��ӽ����� 
//		printf("%s",USART2_RX_BUF);	//���͵�����   
//		sprintf((char*)p,"�յ�%d�ֽ�,��������",rlen);//���յ����ֽ��� 
////		LCD_Fill(15+54,140,239,130,WHITE);
////		POINT_COLOR=BRED;
////		Show_Str(15+54,140,156,12,p,12,0); 			//��ʾ���յ������ݳ���
////		POINT_COLOR=BLUE;
////		LCD_Fill(15,155,239,319,WHITE);
////		Show_Str(15,155,180,190,USART2_RX_BUF,12,0);//��ʾ���յ�������  
//		USART2_RX_STA=0;
////		if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
////		else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
//	}  
//	atk_8266_at_response(1);
//}
//	myfree(p);		//�ͷ��ڴ� 
//	return res;		
//} 


























