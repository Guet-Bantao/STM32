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
//ECB * s_msg;			//�ź���
//ECB * m_msg;			//�����ź���
ECB * q_msg;			//��Ϣ����
//void * MsgGrp[255];			//��Ϣ���д洢��ַ,���֧��255����Ϣ
//int pwm=0;
int adc=0;
int set_light=85;
u16 pwmval=54;
extern u8 RevTempDate[5];

#define FLASH_SAVE_ADDR  0X08009F40		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)

//pwm����
void Task1(void)
{
	
//	unsigned char i;          //��������//����
//	unsigned char Send_Count; //������Ҫ���͵����ݸ���//����
	
	while(1)
	{	
		OLED_ShowVar(50,6,pwmval,2);//��ʾ����									 	
		adc=Lsens_Get_Val();
		if(RevTempDate[0]=='z')
		{
			pwmval=Run(set_light,adc,pwmval);//ת��
		}
		TIM_SetCompare3(TIM3,pwmval);
		
//		if(uar(USART_RX_BUF))
//		{
//			OLED_ShowStr(75, 6,(unsigned char *)"hahahha", 2);
//		}
//		DataScope_Get_Channel_Data( adc , 1 );  //������ 1.0  д��ͨ�� 1//����
//		Send_Count =DataScope_Data_Generate(1);
//		for( i = 0 ; i < Send_Count; i++)  //ѭ������,ֱ���������   
//	    {
//	      while((USART1->SR&0X40)==0);  
//  	      USART1->DR = DataScope_OutPut_Buffer[i]; //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ //����     
//	     }
		OSTimeDly(50);
	}
}
//OLED��ʾ����
void Task4(void)
{
	u8 i;

	OS_SchedLock();
	for(i=0;i<=5;i++)
	{		
		OLED_ShowCN(20+i*15,0,i);//	���ܵ���ϵͳ
	}
	for(i=0;i<=2;i++)
	{		
		OLED_ShowCN(2+i*15,2,i+6);//ģʽ��		
	}
	OLED_ShowStr(5, 4,(unsigned char *)"adc:", 2);
	OLED_ShowStr(5, 6,(unsigned char *)"pwm:", 2);
	OS_SchedUnlock();
	while(1)
	{
		OS_SchedLock();
		//OLED_Clean(6,48,8,128,0x00);
		OLED_ShowVar(50,4,adc,2);//��ʾ����
		//OLED_ShowVar(50,6,pwm,2);//��ʾ����
		OS_SchedUnlock();
		//p=OS_QPend(q_msg,0,1);////////////////////
		OS_SchedLock();
		if(RevTempDate[0]=='z')
		{
			for(i=3;i<=4;i++)
			{		
				OLED_ShowCN(2+i*15,2,i+8);//�Զ�	
			}
		}
		else
		{
			for(i=3;i<=4;i++)
			{		
				OLED_ShowCN(2+i*15,2,i+6);//�ֶ�		
			}
		}
		OLED_ShowStr(78, 4,(unsigned char *)RevTempDate, 2);
		
		OS_SchedUnlock();
		//LED0=!LED0;
		
		OSTimeDly(150);
		
	}
}
//wifi����
void Task3(void)
{
	u8 i=0;
	while(1)
	{
		if(USART_RX_BUF[0]=='+'||(USART_RX_BUF[2]=='+'))//���ո�ʽ��\n+PIDxxxxx������
		{
			if(USART_RX_BUF[i]==':')
			{
				RevTempDate[0]=USART_RX_BUF[i+1];
				RevTempDate[1]=USART_RX_BUF[i+2];
				RevTempDate[2]=USART_RX_BUF[i+3];
				OLED_ShowStr(85, 6,(unsigned char *)"receive", 2);//��ʾ���յ�������
				USART_RX_BUF[0]=USART_RX_BUF[2]=' ';//����ַ�
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
//24L01����
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
		GetDate();//��������

		if(RevTempDate[0]=='z')
		{
			set_light=(RevTempDate[1]-'0')*10+(RevTempDate[2]-'0')*1;
		}
		else if(RevTempDate[0]=='s')
			pwmval=(RevTempDate[1]-'0')*100+(RevTempDate[2]-'0')*10;
		//RevTempDate[0]='a';
		if((temp[0]==RevTempDate[0])&&(temp[1]==RevTempDate[1])&&(temp[2]==RevTempDate[2]))
		{
			//Ƶ��д��flash���ܻ�ը������
		}
		else//���ݱ仯ʱ
		{
			STMFLASH_Write(FLASH_SAVE_ADDR,((u16 *)RevTempDate),3);//д��flash
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
	malloc_init();//�ڴ��ʼ��
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	I2C_Configuration();
	OLED_Init();
	OLED_CLS();//����
	uart_init(115200);
	OLED_ShowStr(5, 2,(unsigned char *)"net...", 2);
	ESP_STA();//��ʼ��wifi
	NRF24L01_GPIOInit();//��ʼ��24L01��IO��
	NRF24L01Int();
	NRFSetRXMode(RX_ADDRESS_0);//����,�����óɽ���ģʽ������Ϊ����ģʽ����Ȼ�۳ɹ�������ʹ
	Lsens_Init(); 			//��ʼ������������
	TIM3_PWM_Init(999,0);//����Ƶ��PWMƵ��=72000000/900=80Khz,72mʱ�ӣ�1 / 72m ����һ��ʱ�����ڣ� 889��ʱ�����ھ��� 889 * 1/72m һ�����ڣ����ڵĵ�����Ƶ��
	Task_Create(Task1,&TASK_1_STK[TASK_1_STK_SIZE-1],0);
	Task_Create(Task2,&TASK_2_STK[TASK_2_STK_SIZE-1],1);
	Task_Create(Task3,&TASK_3_STK[TASK_3_STK_SIZE-1],2);
	Task_Create(Task4,&TASK_4_STK[TASK_4_STK_SIZE-1],3);
	
//	STMFLASH_Write(FLASH_SAVE_ADDR,((u16 *)"abcd"),4);//д��flash
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)RevTempDate,3);
	if(RevTempDate[0]=='z')
		set_light=(RevTempDate[1]-'0')*10+(RevTempDate[2]-'0')*1;
	else if(RevTempDate[0]=='s')
		pwmval=(RevTempDate[1]-'0')*100+(RevTempDate[2]-'0')*10;
	
	
//	s_msg=OS_SemCreate(1);		//�����ź���
//	m_msg =OS_MutexCreate();	//���������ź���
//	q_msg=OS_QCreate(MsgGrp,255);//������Ϣ����
	
	OS_Start(); 
	return 0;

}




