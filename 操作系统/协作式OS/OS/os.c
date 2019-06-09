/*---------------------------------------
-* File:    OS.H
-* AUTHOR： kite
-* E_MAIL:  1216104245@qq.com
-* TIME：   2017/02/20
-* VERSION：V2.1
-*---------------------------------------*/
/*============V2.0修改说明=================
	*新增Delete_Task任务删除函数
	*设置了System_Ticks系统时钟间隔宏
	*修改Create_Task入口参数
==========================================*/
/*============V2.1修改说明=================
	*修改PROCESS_BEGIN()、Os_Delay(ms)宏定义
	*纠正任务运行周期及系统延时的时间
	*
==========================================*/
#include "os.h"
#include "sys.h"
#include <stdlib.h>
#include <malloc.h>
#include "led.h"
LinkStack * List=NULL;
u32 timer=0;
#define System_Ticks  100						//每1/System_Ticks 进入一次中断

/*---------------------------------------
-* File: System_init
-* Description：系统(滴答)时钟初始化
-* Arguments:  	void
-*---------------------------------------*/
void System_init(void)
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8 
	reload=SystemCoreClock/8000000;							//每秒钟的计数次数 单位为K	   
	reload*=1000000/System_Ticks;		//根据System_Ticks设定溢出时间
											//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	 
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 		//每1/System_Ticks秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    

}	

//判断链表是否为空
int IsEmptyList(struct LinkStack * list)  
{  
	return (list->top == NULL);  
}

/*---------------------------------------
-* File: CreateEmptyList
-* Description：创建一个空的任务的链表 
-* Arguments:  	void
-*---------------------------------------*/
//创建一个的链表  
struct LinkStack * CreateEmptyList(void)  
{  
	LinkStack * list=(LinkStack *)mymalloc(0,sizeof(LinkStack)); //mymalloc(sramx,2048);//申请2K字节 
	if(list == NULL)  
		
		return NULL;  
	else  
		list->top=NULL; 
	List=list;	
	return list;     
} 

/*---------------------------------------
-* File: Create_Task
-* Description：创建一个新的任务
-* Arguments:  	TaskHook		任务函数名
-* 							DELAY				第一次任务执行时间
-* 							PERIOD			每次任务间隔时间
-*---------------------------------------*/
u8 Create_Task(		int (* TaskHook)(void), 
									const u32 DELAY, 
									const u16 PERIOD	)    
{
	OsTask * p =(OsTask *)mymalloc(0,sizeof(OsTask));  
	if(p == NULL )    
	{  	
		return 0;  
	}  
	else  
	{  
		p->Delay_MS=DELAY; 
		p->Period_TICKS=PERIOD;
		p->Run=0;
		//p->Preemptive_En=PREEMPTIVE_EN;
		p->TaskHook=TaskHook;	
		p->link=List->top;     
		List->top=p;  
		return 1;  
	} 
}

/*---------------------------------------
-* File: Delete_Task
-* Description：删除一个任务
-* Arguments:  	TaskHook		任务函数名
-*---------------------------------------*/
u8 Delete_Task( int (* TaskHook)(void)	)
{
	OsTask * Del_Pre=List->top;
	OsTask * Del=List->top;
	do
	{
			if(Del->TaskHook==(* TaskHook)) 
			{
				Del_Pre->link=Del->link;
				if(Del==List->top)
				{
					List->top=Del->link;
				}
				myfree(0,Del);
				return 1;
			}	
			Del_Pre=Del;
			Del=Del->link; 
	}while(Del != NULL);
	return 0;
}

void SysTick_Handler(void)
{
	OsTask * p; 
	p=List->top;  
	timer+=(1000/System_Ticks);
	do
	{
			if(p->Delay_MS <= 0)				  
      { 
				 p->Run += 1;		 			  
				if(p->Period_TICKS)	 //Period_TICKS != 0表示该任务周期性执行，否则，只执行一次
				{
					p->Delay_MS= p->Period_TICKS;
				}
      }	
			else 
			{ 								     
         p->Delay_MS -= (1000/System_Ticks);	 //减去一个时间间隔
		  }			
			p=p->link; 
	}while(p != NULL);	
}



