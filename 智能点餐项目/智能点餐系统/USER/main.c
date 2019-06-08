/*==========================================================
 * �������ƣ����ܵ��ϵͳ
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������MDK5
 * ����ʱ�䣺2016/11/12
 * �޸�ʱ�䣺
 * ����˵��������˵����
 *					1--SS  <----->PB10
 *					2--SCK <----->PB13
 *					3--MOSI<----->PB15
 *					4--MISO<----->PB14
 *					5--����
 *					6--GND <----->GND	
 *					7--RST <----->PB11
 *					8--VCC <----->VCC
 *M1���֞�16���ȅ^��ÿ���ȅ^��4�K���K0���K1���K2���K3���M��
 *�҂�Ҳ��16���ȅ^��64���K���^����ַ��̖0~63
 *��0�ȅ^�ĉK0�����^����ַ0�K��������춴�ŏS�̴��a���ѽ��̻������ɸ���
 *ÿ���ȅ^�ĉK0���K1���K2�锵���K������춴�Ŕ���
 *ÿ���ȅ^�ĉK3����ƉK���^����ַ�K3��7��11....�����������ܴaA����ȡ���ơ��ܴaB��
 *Ҫע��д��3���ݣ���Ϊ��3������������������Կ������������������������������������޷�����ʹ��!!!
 *==========================================================*/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "ILI93xx.h"
#include "usart.h"
#include "touch.h"
#include "rc522.h"
#include "stdio.h"
#include "GUI.h"
//#include "w25qxx.h"
#include "includes.h"
#include  "esp8266.h"
#include "iconviewdemo.h"
#include "TestAPP.h"

/************************************************
 UCOSIII���������ȼ��û�������ʹ�ã�ALIENTEK
 ����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
 ���ȼ�0���жϷ������������� OS_IntQTask()
 ���ȼ�1��ʱ�ӽ������� OS_TickTask()
 ���ȼ�2����ʱ���� OS_TmrTask()
 ���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
 ���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
************************************************/

u8 Name[8]={' ',' ',' ',' '};//�������4���ֽڴ�С������sprintf�� ��ʾ�ı������
u8 Price[6];//�������4���ֽڴ�С������sprintf�� ��ʾ�ı������
extern WM_HWIN hItem7;
extern WM_HWIN hItem9;
extern float		price;
extern char check[24];


//�������ȼ�
#define START_TASK_PRIO				4
//�����ջ��С	
#define START_STK_SIZE 				128
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//TOUCH����
//�����������ȼ�
#define TOUCH_TASK_PRIO				5
//�����ջ��С
#define TOUCH_STK_SIZE				128
//������ƿ�
OS_TCB TouchTaskTCB;
//�����ջ
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch����
void touch_task(void *p_arg);

//RC522����
//�����������ȼ�
#define RC522_TASK_PRIO 				3
//�����ջ��С
#define RC522_STK_SIZE				128
//������ƿ�
OS_TCB RC522TaskTCB;
//�����ջ
__align(8) CPU_STK RC522_TASK_STK[RC522_STK_SIZE];//��������Ĭ��ʹ��4�ֽڶ���
//led0����
void RC522_task(void *p_arg);

//EMWINDEMO����
//�����������ȼ�
#define EMWINDEMO_TASK_PRIO			6
//�����ջ��С
#define EMWINDEMO_STK_SIZE			2048//ע��sprintf������Ҫ�ϴ��ڴ�ռ䣬�����ջҲҪ�����㹻�Ŀռ������г�
//������ƿ�
OS_TCB EmwindemoTaskTCB;
//�����ջ
__align(8) CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];//�������ݶ�������,8�ֽڶ��룬����Ը����������sprintf�����
//emwindemo_task����
void emwindemo_task(void *p_arg);


int main(void)
{	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();	    	//��ʱ������ʼ��
	LED_Init();			    //LED�˿ڳ�ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	ESP_STA();				//���ӵ��Է�����
	InitRc522();				//��ʼ����Ƶ��ģ��

	TFTLCD_Init();			//LCD��ʼ��	LCD_Display_Dir(1);		//����
	
//	KEY_Init();	 			//������ʼ��
//	FSMC_SRAM_Init();		//��ʼ��SRAM
//	my_mem_init(SRAMIN); 	//��ʼ���ڲ��ڴ��
//	my_mem_init(SRAMEX);  	//��ʼ���ⲿ�ڴ��
	
	TP_Init();				//��������ʼ��
	
	OSInit(&err);			//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
	while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//����CRCʱ��
	GUI_Init();  			//STemWin��ʼ��
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//STemWin Demo����	
	OSTaskCreate((OS_TCB*     )&EmwindemoTaskTCB,		
				 (CPU_CHAR*   )"Emwindemo task", 		
                 (OS_TASK_PTR )emwindemo_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINDEMO_TASK_PRIO,     
                 (CPU_STK*    )&EMWINDEMO_TASK_STK[0],	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//����������
	OSTaskCreate((OS_TCB*     )&TouchTaskTCB,		
				 (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);		 
	//RC522����
	OSTaskCreate((OS_TCB*     )&RC522TaskTCB,		
				 (CPU_CHAR*   )"RC522 task", 		
                 (OS_TASK_PTR )RC522_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )RC522_TASK_PRIO,     
                 (CPU_STK*    )&RC522_TASK_STK[0],	
                 (CPU_STK_SIZE)RC522_STK_SIZE/10,	
                 (CPU_STK_SIZE)RC522_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����				 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//EMWINDEMO����
void emwindemo_task(void *p_arg)
{
	//GUI_CURSOR_Show();
	iconviewdemo();
	while(1)
	{
		GUI_Delay(5);
	}
}

//TOUCH����
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//��ʱ5ms
	}
}

//RC522����
void RC522_task(void *p_arg)
{
	OS_ERR err;
	u8 status;
	u8 CT[2];//������
	u8 UID[4]; //����
	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	u8 RFID[16];			//���RFID
	int i;
	u16 RMB;
	
	//u8 data[16] = {'K','I','T','E',0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x01,0xFE,0x01,0xFE};//��ֵ=====================	

	OS_TaskSuspend((OS_TCB*)&RC522TaskTCB,&err);		//��������
	while(1)
	{
		status = PcdRequest(PICC_REQALL,CT);//����
		if(status==MI_OK)//�����ɹ�
			{
				status = PcdAnticoll(UID);/*����ײ*/	
				if(status==MI_OK)//�ɹ�
				{
					status =PcdSelect(UID);//ѡ��				
					status =PcdAuthState(0x60,0x09,KEY,UID);////��C
					status =PcdRead(0x09,RFID);//�x��
					
					//status=PcdWrite(0x09,data);//��ֵ=====================				
					if(status==MI_OK)//�x���ɹ�
					{
						for(i=0;i<4;i++)
						{
							Name[i]=RFID[i];//��ȡ��������
						}
						RMB=RFID[4]+RFID[5]+RFID[6]+RFID[7];//��ȡ���Ľ��
						sprintf((char *)Price,"%d",RMB);
						if(RMB>=(int)price)//���Ľ���㹻����
						{
							RMB=(RFID[4]+RFID[5]+RFID[6]+RFID[7])-(int)price;//����
							for(i=4;i<=7;i++)
							{
								RFID[i]=0;
							}
							i=RMB/255;
							RMB=RMB%255;
							for(i=i;i>0;i--)
							{
								RFID[7-i]=0xFF;
							}
							RFID[7]=RMB;
							status=PcdWrite(0x09,RFID);//д��//��ֵ��ע�ʹ˾�
							if(price!=0)
								printf("%s\n",check);//���͵���λ��
							price=0;
							LED1 = 0;

						}
						WM_InvalidateWindow(hItem7);//��Ч��
						WM_InvalidateWindow(hItem9);//��Ч��
					}
				}
			}
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}









