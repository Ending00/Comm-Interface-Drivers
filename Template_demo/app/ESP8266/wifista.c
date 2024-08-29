#include "common.h"
#include "stdlib.h"

//ATK-ESP8266 WIFI STA测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
u8 netpro=0;	//网络模式
u8 atk_8266_wifista_test(void)
{
	u8 ipbuf[16]; 	//IP缓存
//	u16 t=999;
	u8 *p;
	u16 rlen=0, i = 0;
	p=mymalloc(32);							//申请32字节内存
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP服务器关闭(仅AP模式有效) 
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//连接目标路由器,并且获得IP
PRESTA:
	netpro|=atk_8266_netpro_sel((char*)ATK_ESP8266_CWMODE_TBL[0]);	//选择网络模式

	//TCP Client    透传模式测试
	printf("ATK-ESP WIFI-STA 测试\r\n"); 
	printf("正在配置ATK-ESP模块,请稍等...\r\n");
	if(atk_8266_ip_set("WIFI-STA 远端IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[1],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
	atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s","192.168.20.102","8086"); 
    delay_ms(1000);
	
	while(atk_8266_send_cmd(p,"OK",200))
	{
			printf("ATK-ESP 连接TCP失败\r\n"); 
			goto PRESTA;
	}	
	atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传			

	printf("连接成功...................................\r\n"); 
	printf("正在配置ATK-ESP模块,请稍等...\r\n");			
	atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP

	printf("连接状态: %s\r\n",(u8*)ATK_ESP8266_WORKMODE_TBL[1]);
	USART2_RX_STA=0;
	atk_8266_send_cmd("AT+CIPSEND","OK",20);         //开始透传 
	while(1)
	{		
		if(USART2_RX_STA&0X8000)		//接收到一次数据了
		{ 
			rlen=USART2_RX_STA&0X7FFF;	//得到本次接收到的数据长度
			USART2_RX_BUF[rlen]=0;		//添加结束符 
			
			sprintf((char*)p,"收到%d字节,内容如下\r\n%s",rlen,USART2_RX_BUF);//接收到的字节数 
			u2_printf("%s",p);
			USART2_RX_STA=0;
		}  
		atk_8266_at_response(1);
//		myfree(p);		//释放内存
	}
} 




























