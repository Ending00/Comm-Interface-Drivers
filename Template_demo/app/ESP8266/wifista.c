#include "common.h"
#include "stdlib.h"

//ATK-ESP8266 WIFI STA����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8 netpro=0;	//����ģʽ
u8 atk_8266_wifista_test(void)
{
	u8 ipbuf[16]; 	//IP����
//	u16 t=999;
	u8 *p;
	u16 rlen=0, i = 0;
	p=mymalloc(32);							//����32�ֽ��ڴ�
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//����WIFI STAģʽ
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP�������ر�(��APģʽ��Ч) 
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//����Ŀ��·����,���һ��IP
PRESTA:
	netpro|=atk_8266_netpro_sel((char*)ATK_ESP8266_CWMODE_TBL[0]);	//ѡ������ģʽ

	//TCP Client    ͸��ģʽ����
	printf("ATK-ESP WIFI-STA ����\r\n"); 
	printf("��������ATK-ESPģ��,���Ե�...\r\n");
	if(atk_8266_ip_set("WIFI-STA Զ��IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[1],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
	atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s","192.168.20.102","8086"); 
    delay_ms(1000);
	
	while(atk_8266_send_cmd(p,"OK",200))
	{
			printf("ATK-ESP ����TCPʧ��\r\n"); 
			goto PRESTA;
	}	
	atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��			

	printf("���ӳɹ�...................................\r\n"); 
	printf("��������ATK-ESPģ��,���Ե�...\r\n");			
	atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP

	printf("����״̬: %s\r\n",(u8*)ATK_ESP8266_WORKMODE_TBL[1]);
	USART2_RX_STA=0;
	atk_8266_send_cmd("AT+CIPSEND","OK",20);         //��ʼ͸�� 
	while(1)
	{		
		if(USART2_RX_STA&0X8000)		//���յ�һ��������
		{ 
			rlen=USART2_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
			USART2_RX_BUF[rlen]=0;		//��ӽ����� 
			
			sprintf((char*)p,"�յ�%d�ֽ�,��������\r\n%s",rlen,USART2_RX_BUF);//���յ����ֽ��� 
			u2_printf("%s",p);
			USART2_RX_STA=0;
		}  
		atk_8266_at_response(1);
//		myfree(p);		//�ͷ��ڴ�
	}
} 




























