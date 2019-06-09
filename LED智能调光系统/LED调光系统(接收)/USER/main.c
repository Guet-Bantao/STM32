//#include "stm32f10x.h"
#include "led.h"
#include "time.h"
#include "os.h"
//#include "stdio.h"
#include "delay.h"	
#include "24l01.h"
#include "OLED_I2C.h"
#include "codetab.h"
#include "adc.h"
#include "lsens.h"
#include "motor.h"
#include  "esp8266.h"
#include "usart.h"	
//#include "DataScope_DP.h"
#include "stmflash.h"

#define TASK_1_STK_SIZE 256
#define TASK_2_STK_SIZE 256
#define TASK_3_STK_SIZE 256
#define TASK_4_STK_SIZE 256
unsigned int TASK_1_STK[TASK_1_STK_SIZE];
unsigned int TASK_2_STK[TASK_2_STK_SIZE];
unsigned int TASK_3_STK[TASK_3_STK_SIZE];
unsigned int TASK_4_STK[TASK_3_STK_SIZE];
//ECB * s_msg;			//信号量
//ECB * m_msg;			//互斥信号量
ECB * q_msg;			//消息队列
//void * MsgGrp[255];			//消息队列存储地址,最大支持255个消息
//int pwm=0;
int adc=0;
int set_light=85;
u16 pwmval=54;
extern u8 RevTempDate[5];

#define FLASH_SAVE_ADDR  0X08009F40		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

//pwm任务
void Task1(void)
{
	
//	unsigned char i;          //计数变量//调试
//	unsigned char Send_Count; //串口需要发送的数据个数//调试
	
	while(1)
	{	
		OLED_ShowVar(50,6,pwmval,2);//显示变量									 	
		adc=Lsens_Get_Val();
		if(RevTempDate[0]=='z')
		{
			pwmval=Run(set_light,adc,pwmval);//转动
		}
		TIM_SetCompare3(TIM3,pwmval);
		
//		if(uar(USART_RX_BUF))
//		{
//			OLED_ShowStr(75, 6,(unsigned char *)"hahahha", 2);
//		}
//		DataScope_Get_Channel_Data( adc , 1 );  //将数据 1.0  写入通道 1//调试
//		Send_Count =DataScope_Data_Generate(1);
//		for( i = 0 ; i < Send_Count; i++)  //循环发送,直到发送完毕   
//	    {
//	      while((USART1->SR&0X40)==0);  
//  	      USART1->DR = DataScope_OutPut_Buffer[i]; //从串口丢一个字节数据出去 //调试     
//	     }
		OSTimeDly(50);
	}
}
//OLED显示任务
void Task4(void)
{
	u8 i;

	OS_SchedLock();
	for(i=0;i<=5;i++)
	{		
		OLED_ShowCN(20+i*15,0,i);//	智能调光系统
	}
	for(i=0;i<=2;i++)
	{		
		OLED_ShowCN(2+i*15,2,i+6);//模式：		
	}
	OLED_ShowStr(5, 4,(unsigned char *)"adc:", 2);
	OLED_ShowStr(5, 6,(unsigned char *)"pwm:", 2);
	OS_SchedUnlock();
	while(1)
	{
		OS_SchedLock();
		//OLED_Clean(6,48,8,128,0x00);
		OLED_ShowVar(50,4,adc,2);//显示变量
		//OLED_ShowVar(50,6,pwm,2);//显示变量
		OS_SchedUnlock();
		//p=OS_QPend(q_msg,0,1);////////////////////
		OS_SchedLock();
		if(RevTempDate[0]=='z')
		{
			for(i=3;i<=4;i++)
			{		
				OLED_ShowCN(2+i*15,2,i+8);//自动	
			}
		}
		else
		{
			for(i=3;i<=4;i++)
			{		
				OLED_ShowCN(2+i*15,2,i+6);//手动		
			}
		}
		OLED_ShowStr(78, 4,(unsigned char *)RevTempDate, 2);
		
		OS_SchedUnlock();
		//LED0=!LED0;
		
		OSTimeDly(150);
		
	}
}
//wifi任务
void Task3(void)
{
	u8 i=0;
	while(1)
	{
		if(USART_RX_BUF[0]=='+'||(USART_RX_BUF[2]=='+'))//接收格式：\n+PIDxxxxx：数据
		{
			if(USART_RX_BUF[i]==':')
			{
				RevTempDate[0]=USART_RX_BUF[i+1];
				RevTempDate[1]=USART_RX_BUF[i+2];
				RevTempDate[2]=USART_RX_BUF[i+3];
				OLED_ShowStr(85, 6,(unsigned char *)"receive", 2);//显示接收到数据了
				USART_RX_BUF[0]=USART_RX_BUF[2]=' ';//清除字符
				i=0;
				if(RevTempDate[0]=='z')
					set_light=(RevTempDate[1]-'0')*10+(RevTempDate[2]-'0')*1;
				else if(RevTempDate[0]=='s')
					pwmval=(RevTempDate[1]-'0')*100+(RevTempDate[2]-'0')*10;
			}
			else
			{
				i++;
			}
		}
		OSTimeDly(100);
	}
}
//24L01任务
void Task2(void)
{
	//u8 sta;
	u8 temp[3];
	temp[0]=RevTempDate[0];
	temp[1]=RevTempDate[1];
	temp[2]=RevTempDate[2];
	while(1)
	{
		OS_SchedLock();
		GetDate();//接收数据

		if(RevTempDate[0]=='z')
		{
			set_light=(RevTempDate[1]-'0')*10+(RevTempDate[2]-'0')*1;
		}
		else if(RevTempDate[0]=='s')
			pwmval=(RevTempDate[1]-'0')*100+(RevTempDate[2]-'0')*10;
		//RevTempDate[0]='a';
		if((temp[0]==RevTempDate[0])&&(temp[1]==RevTempDate[1])&&(temp[2]==RevTempDate[2]))
		{
			//频繁写入flash可能会炸！！！
		}
		else//数据变化时
		{
			STMFLASH_Write(FLASH_SAVE_ADDR,((u16 *)RevTempDate),3);//写入flash
			//OLED_ShowStr(78, 6,(unsigned char *)RevTempDate, 2);
			temp[0]=RevTempDate[0];
			temp[1]=RevTempDate[1];
			temp[2]=RevTempDate[2];
		}
		OS_SchedUnlock();

		OSTimeDly(20);
	}
}
int main(void)
{	//AT+CIPSTART="TCP","119.23.28.44",8896
	malloc_init();//内存初始化
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	I2C_Configuration();
	OLED_Init();
	OLED_CLS();//清屏
	uart_init(115200);
	OLED_ShowStr(5, 2,(unsigned char *)"net...", 2);
	ESP_STA();//初始化wifi
	NRF24L01_GPIOInit();//初始化24L01的IO口
	NRF24L01Int();
	NRFSetRXMode(RX_ADDRESS_0);//接收,先设置成接收模式再设置为发送模式，不然累成狗都不好使
	Lsens_Init(); 			//初始化光敏传感器
	TIM3_PWM_Init(999,0);//不分频。PWM频率=72000000/900=80Khz,72m时钟，1 / 72m 就是一个时钟周期， 889个时钟周期就是 889 * 1/72m 一个周期，周期的倒数是频率
	Task_Create(Task1,&TASK_1_STK[TASK_1_STK_SIZE-1],0);
	Task_Create(Task2,&TASK_2_STK[TASK_2_STK_SIZE-1],1);
	Task_Create(Task3,&TASK_3_STK[TASK_3_STK_SIZE-1],2);
	Task_Create(Task4,&TASK_4_STK[TASK_4_STK_SIZE-1],3);
	
//	STMFLASH_Write(FLASH_SAVE_ADDR,((u16 *)"abcd"),4);//写入flash
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)RevTempDate,3);
	if(RevTempDate[0]=='z')
		set_light=(RevTempDate[1]-'0')*10+(RevTempDate[2]-'0')*1;
	else if(RevTempDate[0]=='s')
		pwmval=(RevTempDate[1]-'0')*100+(RevTempDate[2]-'0')*10;
	
	
//	s_msg=OS_SemCreate(1);		//创建信号量
//	m_msg =OS_MutexCreate();	//创建互斥信号量
//	q_msg=OS_QCreate(MsgGrp,255);//创建消息队列
	
	OS_Start(); 
	return 0;

}




