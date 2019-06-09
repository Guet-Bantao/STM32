#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "ds18b20.h" 
#include "timer.h"
#include "motor.h"
#include "remote.h"
#include "stmflash.h"
#include  "esp8266.h"
#include "DataScope_DP.h"

float set_temp=20.1;
short temp;
#define FLASH_SAVE_ADDR  0X08070000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
short temperature;
int main(void)
 {	 
//	unsigned char i;          //计数变量
//	unsigned char Send_Count; //串口需要发送的数据个数	 	 
	int pwm=950; 
	u8 datatemp[4];
	//short temperature;    	   

	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	ESP_STA();
	LCD_Init();			   	//初始化LCD   
 	POINT_COLOR=RED;		//设置字体为红色 
	Remote_Init();			//红外接收初始化	
	//TIM5_Int_Init(10,719);//100Khz的计数频率，计数到10为10ms
	TIM3_PWM_Init(999,35);	 //不分频。PWM频率=72000000/7200=10Khz	
	KEY_Init();
	
 	while(DS18B20_Init())	//DS18B20初始化	
	{
		LCD_ShowString(30,130,200,16,16,"DS18B20 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	//ESP_STA();	
	LCD_ShowString(30,130,200,16,16,"DS18B20 OK");
	POINT_COLOR=BLUE;//设置字体为蓝色 
 	LCD_ShowString(30,150,200,24,24,"Temp:   . C");	 
	LCD_ShowString(30,175,200,24,24,"Goal:");
	LCD_ShowString(30,250,200,24,24,"PWM:");
	LCD_ShowString(30,275,200,24,24,"Infrared:");
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,4);
	set_temp=(datatemp[0]-'0')*10+(datatemp[1]-'0')*1+(float)(datatemp[2]-'0')*0.1;
	
	while(1)
	{
		LCD_ShowNum(30+60+8,175,set_temp*10,3,24);	//显示整数部分	
		temperature=DS18B20_Get_Temp();	
		/*if(temperature<0)
		{
			LCD_ShowChar(30+40,150,'-',16,0);			//显示负号
			temperature=-temperature;					//转为正数
		}else LCD_ShowChar(30+40,150,' ',16,0);			//去掉负号*/
		LCD_ShowNum(30+60+8,150,temperature/10,2,24);	//显示整数部分	    
		LCD_ShowNum(30+60+48,150,temperature%10,1,24);	//显示小数部分 	
		
	 	pwm=Run(set_temp,temperature/10+(temperature%10)*0.1);//控温
//		pwm=Run(set_temp,temp/10+(temp%10)*0.1);//
		TIM_SetCompare2(TIM3,pwm);
		LCD_ShowNum(30+60+8,250,pwm,3,24);	//显示pwm
		get_remote_key();
		
//		DataScope_Get_Channel_Data( temperature/10+(temperature%10)*0.1 , 1 );  //将数据 1.0  写入通道 1
//		Send_Count = DataScope_Data_Generate(1); //生成10个通道的 格式化帧数据
//		for( i = 0 ; i < Send_Count; i++)  //循环发送,直到发送完毕   
//	    {
//	      while((USART1->SR&0X40)==0);  
//  	      USART1->DR = DataScope_OutPut_Buffer[i]; //从串口丢一个字节数据出去      
//	     }
		set_temp= key(set_temp);
		set_temp=esp(set_temp);

	}
}

