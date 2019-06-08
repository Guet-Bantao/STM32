#include "led.h"
#include "delay.h"
#include "sys.h"
#include "ILI93xx.h"
#include "usart.h"	 
#include "touch.h"
#include "sdio_sdcard.h"
#include "malloc.h"
#include "GUI.h"
#include "ff.h"
#include "exfuns.h"
#include "iconviewdemo.h"
#include "bmpdisplay.h"
#include "includes.h"
#include "DIALOG.h"
#include "rc522.h"


u8 Name[4]={' ',' ',' ',' '};//如果开辟4个字节大小，调用sprintf后 显示文本会出错
u8 Price[4];//如果开辟4个字节大小，调用sprintf后 显示文本会出错
extern WM_HWIN hItem7;
extern WM_HWIN hItem9;
extern float price;
extern char check[16];


//任务优先级
#define START_TASK_PRIO				3
//任务堆栈大小	
#define START_STK_SIZE 				128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO				5
//任务堆栈大小
#define TOUCH_STK_SIZE				128
//任务控制块
OS_TCB TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch任务
void touch_task(void *p_arg);

//RC522任务
//设置任务优先级
#define RC522_TASK_PRIO 				6
//任务堆栈大小
#define RC522_STK_SIZE				128
//任务控制块
OS_TCB RC522TaskTCB;
//任务堆栈
__align(8) CPU_STK RC522_TASK_STK[RC522_STK_SIZE];//启动代码默认使用4字节对齐
//led0任务
void RC522_task(void *p_arg);


//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK_PRIO			4//优先级最好设为最高，不然显示BMP图时会被其他任务打断
//任务堆栈大小
#define EMWINDEMO_STK_SIZE			1024
//任务控制块
OS_TCB EmwindemoTaskTCB;
//任务堆栈
__align(8) CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];//8字节对齐,不然浮点数读不出
//emwindemo_task任务
void emwindemo_task(void *p_arg);

int main(void)
{	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();	    	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			    //LED端口初始化
	TFTLCD_Init();			//LCD初始化	
	my_mem_init(SRAMIN); 	//初始化内部内存池	
	exfuns_init();			//为fatfs文件系统分配内存
	f_mount(fs[0],"0:",1);	//挂载SD卡
	f_mount(fs[1],"1:",1);	//挂载FLASH
	
	while(SD_Init())		//检测SD卡
	{
		LCD_ShowString(30,90,200,16,16,(u8*)"SD Card Failed!");
		delay_ms(200);
		LCD_Fill(30,90,200+30,70+16,WHITE);
		delay_ms(200);		    
	}
	TP_Init();				//触摸屏初始化
	InitRc522();				//初始化射频卡模块
	
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//开启CRC时钟
	GUI_Init();  			//STemWin初始化
	WM_SetCreateFlags(WM_CF_MEMDEV); //启动所有窗口的存储设备
	
	OS_CRITICAL_ENTER();	//进入临界区
	//STemWin Demo任务	
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
	//触摸屏任务
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
	//RC522任务
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
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//退出临界区
}

//EMWINDEMO任务
void emwindemo_task(void *p_arg)
{	
	iconviewdemo();
	while(1)
	{
		GUI_Delay(10);
	}
}

//TOUCH任务
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms
	}
}

//LED0任务
void led0_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}
//RC522任务
void RC522_task(void *p_arg)
{
	OS_ERR err;
	u8 status;
	u8 CT[2];//卡类型
	u8 UID[4]; //卡号
	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	u8 RFID[16];			//存放RFID
	int i;
	u16 RMB;
	
	//u8 data[16] = {'K','I','T','E',0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x01,0xFE,0x01,0xFE};//充值=====================	

	OS_TaskSuspend((OS_TCB*)&RC522TaskTCB,&err);		//挂起任务
	while(1)
	{
		status = PcdRequest(PICC_REQALL,CT);//尋卡
		if(status==MI_OK)//尋卡成功
			{
				status = PcdAnticoll(UID);/*防冲撞*/	
				if(status==MI_OK)//成功
				{
					status =PcdSelect(UID);//选卡				
					status =PcdAuthState(0x60,0x09,KEY,UID);////驗證
					status =PcdRead(0x09,RFID);//讀卡
					
					//status=PcdWrite(0x09,data);//充值=====================				
					if(status==MI_OK)//讀卡成功
					{
						for(i=0;i<4;i++)
						{
							Name[i]=RFID[i];//读取卡的名字
						}
						RMB=RFID[4]+RFID[5]+RFID[6]+RFID[7];//读取卡的金额
						sprintf((char *)Price,"%d",RMB);
						if(RMB>=(int)price)//卡的金额足够付款
						{
							RMB=(RFID[4]+RFID[5]+RFID[6]+RFID[7])-(int)price;//付款
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
							status=PcdWrite(0x09,RFID);//写卡//充值则注释此句
							if(price!=0)
								printf("%s\n",check);//发送到上位机
							price=0;
							LED1 = 0;

						}
						WM_InvalidateWindow(hItem7);//无效化
						WM_InvalidateWindow(hItem9);//无效化
					}
				}
			}
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}



