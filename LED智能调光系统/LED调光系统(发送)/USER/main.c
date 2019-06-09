//#include "stm32f10x.h"
#include "led.h"
#include "os.h"
#include "stdio.h"
#include "lcd.h"
#include "delay.h"	
#include "key.h"
#include "24l01.h"
#include "OLED_I2C.h"
#include "codetab.h"

#define TASK_1_STK_SIZE 256
#define TASK_2_STK_SIZE 256
#define TASK_3_STK_SIZE 256
unsigned int TASK_1_STK[TASK_1_STK_SIZE];
unsigned int TASK_2_STK[TASK_2_STK_SIZE];
unsigned int TASK_3_STK[TASK_3_STK_SIZE];
//ECB * s_msg;			//信号量
//ECB * m_msg;			//互斥信号量
ECB * q_msg;			//消息队列
void * MsgGrp[255];			//消息队列存储地址,最大支持255个消息
int pwm=70;
int send=0;
u8  TxDate[5];

//按键任务
void Task1(void)
{
	unsigned char mode_flag=1;
	char *p;
	p=my_malloc(1);
	sprintf(p, "z");
	OS_QPost(q_msg,p);
	while(1)
	{
		switch(KEY_Scan(1))
		{
			case 1:
				pwm++;//OS_SemPost(s_msg);break;
				if(mode_flag ==0)
				{
					sprintf(p, "s");
					OS_QPost(q_msg,p);
				}
				else
				{
					sprintf(p, "z");
					OS_QPost(q_msg,p);
				}
				break;
			case 2:
				pwm--;//OS_SemPost(s_msg);break;
				if(mode_flag ==0)
				{
					sprintf(p, "s");
					OS_QPost(q_msg,p);
				}
				else
				{
					sprintf(p, "z");
					OS_QPost(q_msg,p);
				}
				break;
			case 3:
				if(mode_flag ==0)
				{
					mode_flag =1;
					
					sprintf(p, "z");
				}	
				else
				{
					mode_flag=0;
					
					sprintf(p, "s");
				}
				OS_QPost(q_msg,p);
				//OS_SemPost(s_msg);
				break;
			case 4://确定
				send=1;
				break;
			default:
				break;
		}
		TxDate[1]='0'+pwm/10;
		TxDate[2]='0'+pwm%10;
		if(pwm>=96) pwm=96;
		else if(pwm<=3) pwm=3;
		OSTimeDly(30);
	}
}
//OLED显示任务
void Task2(void)
{
	u8 i;
	char * p;
	OS_SchedLock();
	for(i=0;i<=5;i++)
	{		
		OLED_ShowCN(20+i*15,0,i);//	智能调光系统
	}
	for(i=0;i<=2;i++)
	{		
		OLED_ShowCN(2+i*15,2,i+6);//模式：		
	}
	OS_SchedUnlock();
	while(1)
	{
		//OS_SemPend(s_msg,0);
		OS_SchedLock();
		OLED_Clean(6,48,8,128,0x00);
		OLED_ShowStr(5, 6,(unsigned char *)"pwm:", 2);
		OLED_ShowStr(5, 4,(unsigned char *)"send:", 2);
		OLED_ShowVar(50,6,pwm,2);//显示变量
		OS_SchedUnlock();
		p=OS_QPend(q_msg,0,1);
		OS_SchedLock();
		if(*p=='s')
		{
			for(i=3;i<=4;i++)
			{		
				OLED_ShowCN(2+i*15,2,i+6);//手动
				TxDate[0]='s';
				send=1;
			}
		}
		else
		{
			for(i=3;i<=4;i++)
			{		
				OLED_ShowCN(2+i*15,2,i+8);//自动
				TxDate[0]='z';				
			}
		}
		OS_SchedUnlock();
		LED0=!LED0;
		OSTimeDly(50);
		
	}
}
//24L01任务
void Task3(void)
{
	//u8 sta;
	while(1)
	{
		
		//TxDate[0]='a';
		OS_SchedLock();
		if(send==1)
		{
			NRFSetTxMode(TxDate,TxAddr_P0,RX_ADDR_P0);//NRF设置为发送模式并发送数据
			//NRFReadRxDate(STATUS,&sta,1);
			while(CheckACK());	  //检测是否发送完
			send=0;
			OLED_ShowStr(50, 4,(unsigned char *)TxDate, 2);
		}
		OS_SchedUnlock();
		LED1=!LED1;
		OSTimeDly(100);
	}
}


int main(void)
{	u8  TxDate[5];
	malloc_init();//内存初始化
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	KEY_Init();
	I2C_Configuration();
	OLED_Init();
	OLED_CLS();//清屏

	NRF24L01_GPIOInit();//初始化24L01的IO口
	NRF24L01Int();
	NRFSetRXMode(RX_ADDRESS_0);//接收,先设置成接收模式再设置为发送模式，不然累成狗都不好使
	//NRFSetTxMode(TxDate,TxAddr_P0,RX_ADDR_P0);//NRF设置为发送模式并发送数据
	Task_Create(Task1,&TASK_1_STK[TASK_1_STK_SIZE-1],0);
	Task_Create(Task2,&TASK_2_STK[TASK_2_STK_SIZE-1],1);
	Task_Create(Task3,&TASK_3_STK[TASK_3_STK_SIZE-1],2);
	
//	s_msg=OS_SemCreate(1);		//创建信号量
//	m_msg =OS_MutexCreate();	//创建互斥信号量
	q_msg=OS_QCreate(MsgGrp,255);//创建消息队列
	
	OS_Start(); 
	return 0;

}




