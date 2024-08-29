#include "common.h"

//���Ӷ˿ں�:8086,�������޸�Ϊ�����˿�.
const u8* portnum="8086";		 

//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const u8* wifista_ssid="ALIENTEK";			//·����SSID��
const u8* wifista_encryption="wpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifista_password="15902020353"; 	//��������

//WIFI APģʽ,ģ���������߲���,�������޸�.
const u8* wifiap_ssid="ATK-ESP8266";			//����SSID��
const u8* wifiap_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifiap_password="12345678"; 		//�������� 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//4������ģʽ
const char *ATK_ESP8266_CWMODE_TBL[3]={"STAģʽ ","APģʽ ","AP&STAģʽ "};	//ATK-ESP8266,3������ģʽ,Ĭ��Ϊ·����(ROUTER)ģʽ 
//4�ֹ���ģʽ
const char *ATK_ESP8266_WORKMODE_TBL[3]={"TCP������","TCP�ͻ���"," UDPģʽ"};	//ATK-ESP8266,4�ֹ���ģʽ
//5�ּ��ܷ�ʽ
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//usmart֧�ֲ���
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART2_RX_STA;
//     1,����USART2_RX_STA;
void atk_8266_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART2_RX_BUF);	//���͵�����
		if(mode)USART2_RX_STA=0;
	} 
}
//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//��ATK-ESP8266����ָ������
//data:���͵�����(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))break;//�õ���Ч���� 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
u8 atk_8266_quit_trans(void)
{
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(500);					//�ȴ�500ms
	return atk_8266_send_cmd("AT","OK",20);//�˳�͸���ж�.
}
//��ȡATK-ESP8266ģ���AP+STA����״̬
//����ֵ:0��δ����;1,���ӳɹ�
u8 atk_8266_apsta_check(void)
{
	if(atk_8266_quit_trans())return 0;			//�˳�͸�� 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	if(atk_8266_check_cmd("+CIPSTATUS:0")&&
		 atk_8266_check_cmd("+CIPSTATUS:1")&&
		 atk_8266_check_cmd("+CIPSTATUS:2")&&
		 atk_8266_check_cmd("+CIPSTATUS:4"))
		return 0;
	else return 1;
}
//��ȡATK-ESP8266ģ�������״̬
//����ֵ:0,δ����;1,���ӳɹ�.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			//�˳�͸�� 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	p=atk_8266_check_cmd("+CIPSTATUS:"); 
	res=*p;		
	return res;
}

//��ȡClient ip��ַ
//ipbuf:ip��ַ���������
void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//��ȡWAN IP��ַʧ��
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);	
}

//��ȡAP+STA ip��ַ����ָ��λ����ʾ
//ipbuf:ip��ַ���������
void atk_8266_get_ip(void)
{
		u8 *p;
		u8 *p1;
		u8 *p2;
		u8 *ipbuf;
		u8 *buf;
		p=mymalloc(32);							//����32�ֽ��ڴ�
		p1=mymalloc(32);							//����32�ֽ��ڴ�
		p2=mymalloc(32);							//����32�ֽ��ڴ�
	    ipbuf=mymalloc(32);							//����32�ֽ��ڴ�
		buf=mymalloc(32);							//����32�ֽ��ڴ�
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//��ȡWAN IP��ַʧ��
		{ 
			*ipbuf=0;
		}
		else
		{		
			p=atk_8266_check_cmd("APIP,\"");
			p1=(u8*)strstr((const char*)(p+6),"\"");
			p2=p1;
			*p1=0;
			ipbuf=p+6;
			sprintf((char*)buf,"AP IP:%s �˿�:%s",ipbuf,(u8*)portnum);
//			Show_Str(x,y,200,12,buf,12,0);				//��ʾAPģʽ��IP��ַ�Ͷ˿�
			p=(u8*)strstr((const char*)(p2+1),"STAIP,\"");
			p1=(u8*)strstr((const char*)(p+7),"\"");
			*p1=0;
			ipbuf=p+7;
			sprintf((char*)buf,"STA IP:%s �˿�:%s",ipbuf,(u8*)portnum);
//			Show_Str(x,y+15,200,12,buf,12,0);				//��ʾSTAģʽ��IP��ַ�Ͷ˿�
			myfree(p);		//�ͷ��ڴ�
			myfree(p1);		//�ͷ��ڴ�
			myfree(p2);		//�ͷ��ڴ�
			myfree(ipbuf);		//�ͷ��ڴ�
			myfree(buf);		//�ͷ��ڴ�
		}
}

//����ģʽѡ��
//����ֵ:
//0,TCP������
//1,TCP�ͻ���
//2,UDPģʽ
u8 atk_8266_netpro_sel(const char* name) 
{
	u8 *p;
	u8 netpro=0;
	p=mymalloc(50);//����50���ֽڵ��ڴ�
	sprintf((char*)p,"%s ����ģʽѡ��",name);

//	while(1)
//	{
    // �ж�����ģʽ
    if (strcmp(name, "STAģʽ") == 0) {
        netpro = 1;
    } else if (strcmp(name, "APģʽ") == 0) {
        netpro = 2;
    } else if (strcmp(name, "AP&STAģʽ") == 0) {
        netpro = 3;
    }
			
	delay_ms(10);
	atk_8266_at_response(1);
//		if((t++)==20){t=0;LED0=!LED0;}//LED��˸
//	} 
	myfree(p);
	return netpro;//��������ģʽѡ��ֵ 
} 


//IP����
//title:ip���ñ���
//mode:����ģʽ
//port:�˿ں�
//*ip:ip������(����IP���ϲ㺯��)
//����ֵ:0,ȷ������;1,ȡ������.
u8 atk_8266_ip_set(u8* title,u8* mode,u8* port,u8* ip) 
{
	u8 res=0;
	ip[0]=0; 		
	delay_ms(10);
	atk_8266_at_response(1);//WIFIģ�鷢����������,��ʱ�ϴ�������

	return res;
}

//STAģʽ�µ�AP��TCP��UDP����ģʽ����
u8 atk_8266_mode_cofig(const char* name)
{
//	u8 netpro;
	u8 ipbuf[16]; 	//IP����
	u8 *p;
	p=mymalloc(32);//����32���ֽڵ��ڴ�
PRESTA:		
//	netpro|=(atk_8266_netpro_sel((u8*)ATK_ESP8266_CWMODE_TBL[1]))<<4;	//����ģʽѡ�� Ĭ����APģʽ
	if (strcmp(name, "UDPģʽ") == 0)
	{
		if(atk_8266_ip_set("WIFI-APԶ��UDP IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[2],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
		sprintf((char*)p,"AT+CIPSTART=1,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������,��ID�ţ�STAģʽ��Ϊ0
//		sprintf((char*)p,"AT+CIPSTART=0,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������,��ID�ţ�STAģʽ��Ϊ0
		delay_ms(200);
		atk_8266_send_cmd(p,"OK",200);
	}
	else if(strcmp(name, "TCP�ͻ���") == 0)     //AP TCP Client    ͸��ģʽ����
	{
		if(atk_8266_ip_set("WIFI-AP Զ��IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[1],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
		sprintf((char*)p,"AT+CIPSTART=1,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������,��ID�ţ�STAģʽΪclientʱ��Ϊ1
		//sprintf((char*)p,"AT+CIPSTART=0,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������,��ID�ţ�STAģʽΪserverʱ��Ϊ0
		while(atk_8266_send_cmd(p,"OK",200)){}

	}
	else;   //������ģʽ��������
	myfree(p);
	return 0;
}

//���Խ�����UI
void atk_8266_mtest_ui(void)
{ 
	u8 *p,*p1,*p2;
	p=mymalloc(32);							//����32�ֽ��ڴ�
	p1=mymalloc(32);							//����32�ֽ��ڴ�
	p2=mymalloc(32);							//����32�ֽ��ڴ�
	printf("ATK_ESP8266 WIFIģ�����\r\n"); 
	
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);
	atk_8266_send_cmd("AT+RST","OK",20);
	delay_ms(1000);//��ʱ2s�ȴ�ģ������
	delay_ms(1000);//
	delay_ms(1000);
	delay_ms(1000);
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //����ģ��APģʽ���߲���
	atk_8266_send_cmd(p,"OK",1000);	

	atk_8266_send_cmd("AT+GMR","OK",20);		//��ȡ�̼��汾��
	p=atk_8266_check_cmd("SDK version:");
	printf("�̼��汾:%s\r\n",p);
	
	atk_8266_send_cmd("AT+CWMODE?","+CWMODE:",20);	//��ȡ����ģʽ
	p=atk_8266_check_cmd(":");
	printf("����ģʽ:%s\r\n",(u8*)ATK_ESP8266_CWMODE_TBL[*(p+1)-'1']);
	
	atk_8266_send_cmd("AT+CWSAP?","+CWSAP:",20);	//��ȡwifi����
	p=atk_8266_check_cmd("\"");
	p1=(u8*)strstr((const char*)(p+1),"\"");
	p2=p1;
	*p1=0;
	printf("SSID��:%s\r\n",p+1);
	p=(u8*)strstr((const char*)(p2+1),"\"");
	p1=(u8*)strstr((const char*)(p+1),"\"");
	p2=p1;
	*p1=0;		
	printf("����:%s\r\n",p+1);
	p=(u8*)strstr((const char*)(p2+1),",");
	p1=(u8*)strstr((const char*)(p+1),",");
	*p1=0;
	printf("ͨ����:%s\r\n",p+1);
	printf("���ܷ�ʽ:%s\r\n",(u8*)ATK_ESP8266_ECN_TBL[*(p1+1)-'0']);
	myfree(p);		//�ͷ��ڴ� 
	myfree(p1);		//�ͷ��ڴ� 
	myfree(p2);		//�ͷ��ڴ� 	
}


//ATK-ESP8266ģ�����������
void atk_8266_test(const char* name)
{
	printf("ATK-ESP8266 WIFIģ�����"); 
	while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
	{
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
		printf("δ��⵽ģ��!!!");
		delay_ms(800);
		printf("��������ģ��..."); 
	} 
	while(atk_8266_send_cmd("ATE0","OK",20));//�رջ���
	atk_8266_mtest_ui();
	while(1)
	{
		delay_ms(10); 
		atk_8266_at_response(1);//���ATK-ESP8266ģ�鷢�͹���������,��ʱ�ϴ�������
		
		printf("ATK-ESP WIFI-STA ����\n");
		printf("��������ATK-ESP8266ģ�飬���Ե�...\n");
		atk_8266_wifista_test();
	} 
}
















































