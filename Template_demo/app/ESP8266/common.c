#include "common.h"

//连接端口号:8086,可自行修改为其他端口.
const u8* portnum="8086";		 

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const u8* wifista_ssid="ALIENTEK";			//路由器SSID号
const u8* wifista_encryption="wpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifista_password="15902020353"; 	//连接密码

//WIFI AP模式,模块对外的无线参数,可自行修改.
const u8* wifiap_ssid="ATK-ESP8266";			//对外SSID号
const u8* wifiap_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifiap_password="12345678"; 		//连接密码 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//4个网络模式
const char *ATK_ESP8266_CWMODE_TBL[3]={"STA模式 ","AP模式 ","AP&STA模式 "};	//ATK-ESP8266,3种网络模式,默认为路由器(ROUTER)模式 
//4种工作模式
const char *ATK_ESP8266_WORKMODE_TBL[3]={"TCP服务器","TCP客户端"," UDP模式"};	//ATK-ESP8266,4种工作模式
//5种加密方式
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//usmart支持部分
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART2_RX_STA;
//     1,清零USART2_RX_STA;
void atk_8266_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART2_RX_BUF);	//发送到串口
		if(mode)USART2_RX_STA=0;
	} 
}
//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//向ATK-ESP8266发送指定数据
//data:发送的数据(不需要添加回车了)
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s",data);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))break;//得到有效数据 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//       1,退出失败
u8 atk_8266_quit_trans(void)
{
	while((USART2->SR&0X40)==0);	//等待发送空
	USART2->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART2->SR&0X40)==0);	//等待发送空
	USART2->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART2->SR&0X40)==0);	//等待发送空
	USART2->DR='+';      
	delay_ms(500);					//等待500ms
	return atk_8266_send_cmd("AT","OK",20);//退出透传判断.
}
//获取ATK-ESP8266模块的AP+STA连接状态
//返回值:0，未连接;1,连接成功
u8 atk_8266_apsta_check(void)
{
	if(atk_8266_quit_trans())return 0;			//退出透传 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//发送AT+CIPSTATUS指令,查询连接状态
	if(atk_8266_check_cmd("+CIPSTATUS:0")&&
		 atk_8266_check_cmd("+CIPSTATUS:1")&&
		 atk_8266_check_cmd("+CIPSTATUS:2")&&
		 atk_8266_check_cmd("+CIPSTATUS:4"))
		return 0;
	else return 1;
}
//获取ATK-ESP8266模块的连接状态
//返回值:0,未连接;1,连接成功.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			//退出透传 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//发送AT+CIPSTATUS指令,查询连接状态
	p=atk_8266_check_cmd("+CIPSTATUS:"); 
	res=*p;		
	return res;
}

//获取Client ip地址
//ipbuf:ip地址输出缓存区
void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//获取WAN IP地址失败
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);	
}

//获取AP+STA ip地址并在指定位置显示
//ipbuf:ip地址输出缓存区
void atk_8266_get_ip(void)
{
		u8 *p;
		u8 *p1;
		u8 *p2;
		u8 *ipbuf;
		u8 *buf;
		p=mymalloc(32);							//申请32字节内存
		p1=mymalloc(32);							//申请32字节内存
		p2=mymalloc(32);							//申请32字节内存
	    ipbuf=mymalloc(32);							//申请32字节内存
		buf=mymalloc(32);							//申请32字节内存
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//获取WAN IP地址失败
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
			sprintf((char*)buf,"AP IP:%s 端口:%s",ipbuf,(u8*)portnum);
//			Show_Str(x,y,200,12,buf,12,0);				//显示AP模式的IP地址和端口
			p=(u8*)strstr((const char*)(p2+1),"STAIP,\"");
			p1=(u8*)strstr((const char*)(p+7),"\"");
			*p1=0;
			ipbuf=p+7;
			sprintf((char*)buf,"STA IP:%s 端口:%s",ipbuf,(u8*)portnum);
//			Show_Str(x,y+15,200,12,buf,12,0);				//显示STA模式的IP地址和端口
			myfree(p);		//释放内存
			myfree(p1);		//释放内存
			myfree(p2);		//释放内存
			myfree(ipbuf);		//释放内存
			myfree(buf);		//释放内存
		}
}

//工作模式选择
//返回值:
//0,TCP服务器
//1,TCP客户端
//2,UDP模式
u8 atk_8266_netpro_sel(const char* name) 
{
	u8 *p;
	u8 netpro=0;
	p=mymalloc(50);//申请50个字节的内存
	sprintf((char*)p,"%s 工作模式选择",name);

//	while(1)
//	{
    // 判断网络模式
    if (strcmp(name, "STA模式") == 0) {
        netpro = 1;
    } else if (strcmp(name, "AP模式") == 0) {
        netpro = 2;
    } else if (strcmp(name, "AP&STA模式") == 0) {
        netpro = 3;
    }
			
	delay_ms(10);
	atk_8266_at_response(1);
//		if((t++)==20){t=0;LED0=!LED0;}//LED闪烁
//	} 
	myfree(p);
	return netpro;//返回网络模式选择值 
} 


//IP设置
//title:ip设置标题
//mode:工作模式
//port:端口号
//*ip:ip缓存区(返回IP给上层函数)
//返回值:0,确认连接;1,取消连接.
u8 atk_8266_ip_set(u8* title,u8* mode,u8* port,u8* ip) 
{
	u8 res=0;
	ip[0]=0; 		
	delay_ms(10);
	atk_8266_at_response(1);//WIFI模块发过来的数据,及时上传给电脑

	return res;
}

//STA模式下的AP的TCP、UDP工作模式配置
u8 atk_8266_mode_cofig(const char* name)
{
//	u8 netpro;
	u8 ipbuf[16]; 	//IP缓存
	u8 *p;
	p=mymalloc(32);//申请32个字节的内存
PRESTA:		
//	netpro|=(atk_8266_netpro_sel((u8*)ATK_ESP8266_CWMODE_TBL[1]))<<4;	//网络模式选择 默认是AP模式
	if (strcmp(name, "UDP模式") == 0)
	{
		if(atk_8266_ip_set("WIFI-AP远端UDP IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[2],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
		sprintf((char*)p,"AT+CIPSTART=1,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标UDP服务器,及ID号，STA模式下为0
//		sprintf((char*)p,"AT+CIPSTART=0,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标UDP服务器,及ID号，STA模式下为0
		delay_ms(200);
		atk_8266_send_cmd(p,"OK",200);
	}
	else if(strcmp(name, "TCP客户端") == 0)     //AP TCP Client    透传模式测试
	{
		if(atk_8266_ip_set("WIFI-AP 远端IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[1],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
		sprintf((char*)p,"AT+CIPSTART=1,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器,及ID号，STA模式为client时，为1
		//sprintf((char*)p,"AT+CIPSTART=0,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器,及ID号，STA模式为server时，为0
		while(atk_8266_send_cmd(p,"OK",200)){}

	}
	else;   //服务器模式不用配置
	myfree(p);
	return 0;
}

//测试界面主UI
void atk_8266_mtest_ui(void)
{ 
	u8 *p,*p1,*p2;
	p=mymalloc(32);							//申请32字节内存
	p1=mymalloc(32);							//申请32字节内存
	p2=mymalloc(32);							//申请32字节内存
	printf("ATK_ESP8266 WIFI模块测试\r\n"); 
	
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);
	atk_8266_send_cmd("AT+RST","OK",20);
	delay_ms(1000);//延时2s等待模块重启
	delay_ms(1000);//
	delay_ms(1000);
	delay_ms(1000);
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //配置模块AP模式无线参数
	atk_8266_send_cmd(p,"OK",1000);	

	atk_8266_send_cmd("AT+GMR","OK",20);		//获取固件版本号
	p=atk_8266_check_cmd("SDK version:");
	printf("固件版本:%s\r\n",p);
	
	atk_8266_send_cmd("AT+CWMODE?","+CWMODE:",20);	//获取网络模式
	p=atk_8266_check_cmd(":");
	printf("网络模式:%s\r\n",(u8*)ATK_ESP8266_CWMODE_TBL[*(p+1)-'1']);
	
	atk_8266_send_cmd("AT+CWSAP?","+CWSAP:",20);	//获取wifi配置
	p=atk_8266_check_cmd("\"");
	p1=(u8*)strstr((const char*)(p+1),"\"");
	p2=p1;
	*p1=0;
	printf("SSID号:%s\r\n",p+1);
	p=(u8*)strstr((const char*)(p2+1),"\"");
	p1=(u8*)strstr((const char*)(p+1),"\"");
	p2=p1;
	*p1=0;		
	printf("密码:%s\r\n",p+1);
	p=(u8*)strstr((const char*)(p2+1),",");
	p1=(u8*)strstr((const char*)(p+1),",");
	*p1=0;
	printf("通道号:%s\r\n",p+1);
	printf("加密方式:%s\r\n",(u8*)ATK_ESP8266_ECN_TBL[*(p1+1)-'0']);
	myfree(p);		//释放内存 
	myfree(p1);		//释放内存 
	myfree(p2);		//释放内存 	
}


//ATK-ESP8266模块测试主函数
void atk_8266_test(const char* name)
{
	printf("ATK-ESP8266 WIFI模块测试"); 
	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
		printf("未检测到模块!!!");
		delay_ms(800);
		printf("尝试连接模块..."); 
	} 
	while(atk_8266_send_cmd("ATE0","OK",20));//关闭回显
	atk_8266_mtest_ui();
	while(1)
	{
		delay_ms(10); 
		atk_8266_at_response(1);//检查ATK-ESP8266模块发送过来的数据,及时上传给电脑
		
		printf("ATK-ESP WIFI-STA 测试\n");
		printf("正在配置ATK-ESP8266模块，请稍等...\n");
		atk_8266_wifista_test();
	} 
}
















































