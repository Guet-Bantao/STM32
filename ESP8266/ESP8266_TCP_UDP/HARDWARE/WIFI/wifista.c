#include "common.h"
#include "stdlib.h"
#ifdef USE_LCD
#include "lcd.h"
#include "image2lcd.h"
#elif USE_OLED
#include "OLED_I2C.h"
#include "codetab.h"
#endif

//ATK-ESP8266 WIFI STA测试
//用于测试TCP/UDP连接
//网络模式0:UDP  1:TCP Client  defalut:TCP Server
u8 p[46];
void atk_8266_wifista_test(u8 netpro)
{
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP缓存
	//u8 *p;
	u16 t=999;		//加速第一次获取链接状态
	u16 rlen=0;
	u8 constate=0;	//连接状态
	//p=malloc(48);							//申请32字节内存
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
	/*atk_8266_send_cmd("AT+RST","OK",20);		//DHCP服务器关闭(仅AP模式有效) 
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);*/
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
#ifdef USE_LCD
	LCD_ShowChineseString(0,60,200,16,16,(u8*)"try link hotspot         ");//尝试链接热点
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//连接目标路由器,并且获得IP
	LCD_ShowChineseString(0,60,200,16,16,(u8*)"link host success    ");//热点链接成功
	LCD_ShowChineseString(0,80,200,16,16,(u8*)"configure module");//正在配置ATK-ESP模块,请稍等...
#elif USE_OLED
	OLED_ShowStr(0,0,(unsigned char *)"try link hotspot",1);
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//连接目标路由器,并且获得IP
	OLED_ShowStr(0,0,(unsigned char *)"success link hotspot",1);
	OLED_ShowStr(0,2,(unsigned char *)"try configure module...",1);
#endif
	if(netpro==0)   //UDP模式
	{
		sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",wifista_ipbuf,(u8*)portnum);    //配置目标UDP服务器
		delay_ms(200);
		atk_8266_send_cmd("AT+CIPMUX=0","OK",20);  //单链接模式
		delay_ms(200);
		LCD_Clear(WHITE);
		while(atk_8266_send_cmd(p,"OK",500));
	}
	else     //TCP模式
	{
		if(netpro==1)     //TCP Client    透传模式测试
		{
			//if(atk_8266_ip_set("WIFI-STA 远端IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",wifista_ipbuf,(u8*)portnum);    //配置目标TCP服务器
			while(atk_8266_send_cmd(p,"OK",200))
			{
#ifdef USE_LCD
				//LCD_Clear(WHITE);
				//POINT_COLOR=RED;
				LCD_ShowChineseString(,100,200,16,16,(u8*)"fail link TCP Server");//连接TCP Server失败	
#elif USE_OLED
				OLED_ShowStr(0,3,(unsigned char *)"fail link TCP Server",1);
#endif
			}	
			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传		
			atk_8266_quit_trans();//退出透传
			atk_8266_send_cmd("AT+CIPSEND","OK",20);         //开始透传   			
		}
		else		//TCP Server
		{
			atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
			sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //开启Server模式(0，关闭；1，打开)，端口号为portnum
			atk_8266_send_cmd(p,"OK",50);    
		}
	}
	atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
	sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);
#ifdef USE_LCD
	LCD_Clear(WHITE);//清屏
	LCD_ShowChineseString(0,30,200,16,16,(u8*)"remote link success");//远端IP链接成功
	LCD_ShowChineseString(0,80,200,16,16,(u8*)p);	//显示IP地址和端口					
	LCD_ShowChineseString(0,100,200,16,16,(u8*)"state:"); //连接状态
	LCD_ShowChineseString(0,120,200,16,16,(u8*)"mode:"); 	//连接状态
	LCD_ShowChineseString(0,140,200,16,16,(u8*)"send:");//发送数据
	LCD_ShowChineseString(0,160,200,16,16,(u8*)"recv:");//接收数据
#elif USE_OLED	
	OLED_CLS();//清屏
	OLED_ShowStr(0,1,(unsigned char *)"Status:",1);
	OLED_ShowStr(0,0,(unsigned char *)"Success link TCP Server",1);
#endif		
	//atk_8266_wificonf_show(30,180,"请设置路由器无线参数为:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
	POINT_COLOR=BLUE;
	USART3_RX_STA=0;
	while(1)
	{	
		if(netpro==0)   //UDP 模式
		{
			sprintf((char*)p,"ATK-8266,Hello%02d\r\n",t/10);//测试数据，发送Hello数据
#ifdef USE_LCD
			LCD_ShowChineseString(40+54,360,200,24,24,(u8*)p);
#endif	
			atk_8266_send_cmd("AT+CIPSEND=17","OK",200);  //发送指定长度的数据
			delay_ms(200);
			atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
			timex=100;
		}
		else if(netpro==1)   //TCP Client
		{
			        
			sprintf((char*)p,"ATK-8266,Hello%02d\r\n",t/10);//测试数据
#ifdef USE_LCD
			LCD_ShowChineseString(40+54,140,200,16,16,(u8*)p);
#endif
			u3_printf("%s",p);//把数据发送出去
			timex=100;
		}
		else    //TCP Server
		{
			sprintf((char*)p,"ATK-8266,Hello%02d\r\n",t/10);//测试数据，发送数据ATK-8266,Hello
			LCD_ShowChineseString(0,360,200,16,16,(u8*)p);
			atk_8266_send_cmd("AT+CIPSEND=0,17","OK",200);  //发送指定长度的数据
			delay_ms(200);
			atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
			timex=100;
		}
#ifdef USE_LCD		
		if(timex)timex--;
		if(timex==1)LCD_Fill(40+54,140,239,112,WHITE);
#endif
		t++;
		delay_ms(10);
		if(USART3_RX_STA&0X8000)		//接收到一次数据了
		{ 
			rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
			USART3_RX_BUF[rlen]=0;		//添加结束符 
			printf("%s",USART3_RX_BUF);	//发送到串口   
#ifdef USE_LCD	
			sprintf((char*)p,"recv led :%d,data is :",rlen);//接收到的字节数 
			LCD_Fill(0,200,240,240,WHITE);
			POINT_COLOR=BRED;
			LCD_ShowChineseString(0,200,200,16,16,(u8*)p);//显示接收到的数据长度
			POINT_COLOR=BLUE;
			LCD_Fill(40,390,239,319,WHITE);
			LCD_ShowChineseString(84,220,200,16,16,(u8*)USART3_RX_BUF);//显示接收到的数据  
#elif USE_OLED	
#endif	
			USART3_RX_STA=0;
			if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
			else t=0;                   //状态为已经连接了,10秒后再检查
		}  
		if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
		{
/*
			constate=atk_8266_consta_check();//得到连接状态
#ifdef USE_LCD	
			if(constate=='+')LCD_ShowChineseString(40+40,320,200,24,24,(u8*)"link ok");//连接状态
			else LCD_ShowChineseString(40+40,320,200,24,24,(u8*)"link error");
#elif USE_OLED	
			if(constate=='+')OLED_ShowStr(0,5,"Link",2);//连接状态
			else OLED_ShowStr(0,5,"Error",2);
#endif	
	*/		t=0;
		}
		if((t%20)==0)LED0=!LED0;
		//atk_8266_at_response(1);
	}
	//free(p);		//释放内存 	
} 




























