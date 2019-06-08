#include "common.h"
#include "stdlib.h"
#ifdef USE_LCD
#include "lcd.h"
#include "image2lcd.h"
#elif USE_OLED
#include "OLED_I2C.h"
#include "codetab.h"
#endif

//ATK-ESP8266 WIFI STA����
//���ڲ���TCP/UDP����
//����ģʽ0:UDP  1:TCP Client  defalut:TCP Server
u8 p[46];
void atk_8266_wifista_test(u8 netpro)
{
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP����
	//u8 *p;
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u16 rlen=0;
	u8 constate=0;	//����״̬
	//p=malloc(48);							//����32�ֽ��ڴ�
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//����WIFI STAģʽ
	/*atk_8266_send_cmd("AT+RST","OK",20);		//DHCP�������ر�(��APģʽ��Ч) 
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);*/
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
#ifdef USE_LCD
	LCD_ShowChineseString(0,60,200,16,16,(u8*)"try link hotspot         ");//���������ȵ�
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//����Ŀ��·����,���һ��IP
	LCD_ShowChineseString(0,60,200,16,16,(u8*)"link host success    ");//�ȵ����ӳɹ�
	LCD_ShowChineseString(0,80,200,16,16,(u8*)"configure module");//��������ATK-ESPģ��,���Ե�...
#elif USE_OLED
	OLED_ShowStr(0,0,(unsigned char *)"try link hotspot",1);
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//����Ŀ��·����,���һ��IP
	OLED_ShowStr(0,0,(unsigned char *)"success link hotspot",1);
	OLED_ShowStr(0,2,(unsigned char *)"try configure module...",1);
#endif
	if(netpro==0)   //UDPģʽ
	{
		sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",wifista_ipbuf,(u8*)portnum);    //����Ŀ��UDP������
		delay_ms(200);
		atk_8266_send_cmd("AT+CIPMUX=0","OK",20);  //������ģʽ
		delay_ms(200);
		LCD_Clear(WHITE);
		while(atk_8266_send_cmd(p,"OK",500));
	}
	else     //TCPģʽ
	{
		if(netpro==1)     //TCP Client    ͸��ģʽ����
		{
			//if(atk_8266_ip_set("WIFI-STA Զ��IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",wifista_ipbuf,(u8*)portnum);    //����Ŀ��TCP������
			while(atk_8266_send_cmd(p,"OK",200))
			{
#ifdef USE_LCD
				//LCD_Clear(WHITE);
				//POINT_COLOR=RED;
				LCD_ShowChineseString(,100,200,16,16,(u8*)"fail link TCP Server");//����TCP Serverʧ��	
#elif USE_OLED
				OLED_ShowStr(0,3,(unsigned char *)"fail link TCP Server",1);
#endif
			}	
			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��		
			atk_8266_quit_trans();//�˳�͸��
			atk_8266_send_cmd("AT+CIPSEND","OK",20);         //��ʼ͸��   			
		}
		else		//TCP Server
		{
			atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
			sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //����Serverģʽ(0���رգ�1����)���˿ں�Ϊportnum
			atk_8266_send_cmd(p,"OK",50);    
		}
	}
	atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
	sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);
#ifdef USE_LCD
	LCD_Clear(WHITE);//����
	LCD_ShowChineseString(0,30,200,16,16,(u8*)"remote link success");//Զ��IP���ӳɹ�
	LCD_ShowChineseString(0,80,200,16,16,(u8*)p);	//��ʾIP��ַ�Ͷ˿�					
	LCD_ShowChineseString(0,100,200,16,16,(u8*)"state:"); //����״̬
	LCD_ShowChineseString(0,120,200,16,16,(u8*)"mode:"); 	//����״̬
	LCD_ShowChineseString(0,140,200,16,16,(u8*)"send:");//��������
	LCD_ShowChineseString(0,160,200,16,16,(u8*)"recv:");//��������
#elif USE_OLED	
	OLED_CLS();//����
	OLED_ShowStr(0,1,(unsigned char *)"Status:",1);
	OLED_ShowStr(0,0,(unsigned char *)"Success link TCP Server",1);
#endif		
	//atk_8266_wificonf_show(30,180,"������·�������߲���Ϊ:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
	POINT_COLOR=BLUE;
	USART3_RX_STA=0;
	while(1)
	{	
		if(netpro==0)   //UDP ģʽ
		{
			sprintf((char*)p,"ATK-8266,Hello%02d\r\n",t/10);//�������ݣ�����Hello����
#ifdef USE_LCD
			LCD_ShowChineseString(40+54,360,200,24,24,(u8*)p);
#endif	
			atk_8266_send_cmd("AT+CIPSEND=17","OK",200);  //����ָ�����ȵ�����
			delay_ms(200);
			atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
			timex=100;
		}
		else if(netpro==1)   //TCP Client
		{
			        
			sprintf((char*)p,"ATK-8266,Hello%02d\r\n",t/10);//��������
#ifdef USE_LCD
			LCD_ShowChineseString(40+54,140,200,16,16,(u8*)p);
#endif
			u3_printf("%s",p);//�����ݷ��ͳ�ȥ
			timex=100;
		}
		else    //TCP Server
		{
			sprintf((char*)p,"ATK-8266,Hello%02d\r\n",t/10);//�������ݣ���������ATK-8266,Hello
			LCD_ShowChineseString(0,360,200,16,16,(u8*)p);
			atk_8266_send_cmd("AT+CIPSEND=0,17","OK",200);  //����ָ�����ȵ�����
			delay_ms(200);
			atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
			timex=100;
		}
#ifdef USE_LCD		
		if(timex)timex--;
		if(timex==1)LCD_Fill(40+54,140,239,112,WHITE);
#endif
		t++;
		delay_ms(10);
		if(USART3_RX_STA&0X8000)		//���յ�һ��������
		{ 
			rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
			USART3_RX_BUF[rlen]=0;		//��ӽ����� 
			printf("%s",USART3_RX_BUF);	//���͵�����   
#ifdef USE_LCD	
			sprintf((char*)p,"recv led :%d,data is :",rlen);//���յ����ֽ��� 
			LCD_Fill(0,200,240,240,WHITE);
			POINT_COLOR=BRED;
			LCD_ShowChineseString(0,200,200,16,16,(u8*)p);//��ʾ���յ������ݳ���
			POINT_COLOR=BLUE;
			LCD_Fill(40,390,239,319,WHITE);
			LCD_ShowChineseString(84,220,200,16,16,(u8*)USART3_RX_BUF);//��ʾ���յ�������  
#elif USE_OLED	
#endif	
			USART3_RX_STA=0;
			if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
			else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
		}  
		if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
		{
/*
			constate=atk_8266_consta_check();//�õ�����״̬
#ifdef USE_LCD	
			if(constate=='+')LCD_ShowChineseString(40+40,320,200,24,24,(u8*)"link ok");//����״̬
			else LCD_ShowChineseString(40+40,320,200,24,24,(u8*)"link error");
#elif USE_OLED	
			if(constate=='+')OLED_ShowStr(0,5,"Link",2);//����״̬
			else OLED_ShowStr(0,5,"Error",2);
#endif	
	*/		t=0;
		}
		if((t%20)==0)LED0=!LED0;
		//atk_8266_at_response(1);
	}
	//free(p);		//�ͷ��ڴ� 	
} 




























