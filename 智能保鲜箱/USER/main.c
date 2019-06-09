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
#define FLASH_SAVE_ADDR  0X08070000		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
short temperature;
int main(void)
 {	 
//	unsigned char i;          //��������
//	unsigned char Send_Count; //������Ҫ���͵����ݸ���	 	 
	int pwm=950; 
	u8 datatemp[4];
	//short temperature;    	   

	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ 115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	ESP_STA();
	LCD_Init();			   	//��ʼ��LCD   
 	POINT_COLOR=RED;		//��������Ϊ��ɫ 
	Remote_Init();			//������ճ�ʼ��	
	//TIM5_Int_Init(10,719);//100Khz�ļ���Ƶ�ʣ�������10Ϊ10ms
	TIM3_PWM_Init(999,35);	 //����Ƶ��PWMƵ��=72000000/7200=10Khz	
	KEY_Init();
	
 	while(DS18B20_Init())	//DS18B20��ʼ��	
	{
		LCD_ShowString(30,130,200,16,16,"DS18B20 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	//ESP_STA();	
	LCD_ShowString(30,130,200,16,16,"DS18B20 OK");
	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
 	LCD_ShowString(30,150,200,24,24,"Temp:   . C");	 
	LCD_ShowString(30,175,200,24,24,"Goal:");
	LCD_ShowString(30,250,200,24,24,"PWM:");
	LCD_ShowString(30,275,200,24,24,"Infrared:");
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,4);
	set_temp=(datatemp[0]-'0')*10+(datatemp[1]-'0')*1+(float)(datatemp[2]-'0')*0.1;
	
	while(1)
	{
		LCD_ShowNum(30+60+8,175,set_temp*10,3,24);	//��ʾ��������	
		temperature=DS18B20_Get_Temp();	
		/*if(temperature<0)
		{
			LCD_ShowChar(30+40,150,'-',16,0);			//��ʾ����
			temperature=-temperature;					//תΪ����
		}else LCD_ShowChar(30+40,150,' ',16,0);			//ȥ������*/
		LCD_ShowNum(30+60+8,150,temperature/10,2,24);	//��ʾ��������	    
		LCD_ShowNum(30+60+48,150,temperature%10,1,24);	//��ʾС������ 	
		
	 	pwm=Run(set_temp,temperature/10+(temperature%10)*0.1);//����
//		pwm=Run(set_temp,temp/10+(temp%10)*0.1);//
		TIM_SetCompare2(TIM3,pwm);
		LCD_ShowNum(30+60+8,250,pwm,3,24);	//��ʾpwm
		get_remote_key();
		
//		DataScope_Get_Channel_Data( temperature/10+(temperature%10)*0.1 , 1 );  //������ 1.0  д��ͨ�� 1
//		Send_Count = DataScope_Data_Generate(1); //����10��ͨ���� ��ʽ��֡����
//		for( i = 0 ; i < Send_Count; i++)  //ѭ������,ֱ���������   
//	    {
//	      while((USART1->SR&0X40)==0);  
//  	      USART1->DR = DataScope_OutPut_Buffer[i]; //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ      
//	     }
		set_temp= key(set_temp);
		set_temp=esp(set_temp);

	}
}

