//#include "stm32f10x.h"
#include "led.h"
#include "os.h"
#include <stdlib.h>
#include <malloc.h>


int Task1(void)
{
	PROCESS_BEGIN(); 
	
	LED0=!LED0;
	Os_Delay(100);
		
	PROCESS_END();
}
int Task2(void)
{
	PROCESS_BEGIN(); 
	
	LED1=0;
	Os_Delay(100);
	LED1=1;
	Os_Delay(150);
	
	PROCESS_END();
}
int Task3(void)
{
	//LED1=!LED1;
	static u8 t=0;
	t++;
	if(t==12)//运行12次(12*1000ms)后删除任务2、3
	{
		Delete_Task(Task2);
		Delete_Task(Task3);
	}
	return 0;
}

void Scheduler_Tasks(void) ;

int main(void)
{
	my_mem_init(0);		//初始化内部内存池
	(void)CreateEmptyList();
	System_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	LED_Init();
	Create_Task(Task1, 0, 120)  ; //任务1在系统时钟开始后0ms运行，每隔120ms运行一次
	Create_Task(Task2, 0, 250)  ; 	
	Create_Task(Task3, 0, 1000)  ; //任务3在系统时钟开始后0ms运行，每隔1000ms运行一次

	while(1)
		{		
		 Scheduler_Tasks(); 
		}
}
void Scheduler_Tasks(void) 
{
	OsTask * Rotation; 
	Rotation=List->top;  
	do
	{
			if(Rotation->Run != 0) //如果当前任务是合作式任务且已就绪 
			{
				if(!(Rotation->TaskHook)())  	//运行该任务,且如果任务中有系统延时函数已完成
					Rotation->Run-=1; 					//则运行次数减一
				//否则运行次数不减，等待下一次CPU控制权
				 if(Rotation->Period_TICKS== 0)  //如果这是个“单次”任务，将它从队列中删除
				{
					Delete_Task(Rotation->TaskHook);
				} //否则周期性的任务将继续运行
			}			
			Rotation=Rotation->link; 
	}while(Rotation != NULL);
}

