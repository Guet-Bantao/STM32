/*---------------------------------------
-* File:    OS.H
-* AUTHOR�� kite
-* E_MAIL:  1216104245@qq.com
-* TIME��   2017/02/20
-* VERSION��V2.1
-*---------------------------------------*/
/*============V2.0�޸�˵��=================
	*����Delete_Task����ɾ������
	*������System_Ticksϵͳʱ�Ӽ����
	*�޸�Create_Task��ڲ���
==========================================*/
/*============V2.1�޸�˵��=================
	*�޸�PROCESS_BEGIN()��Os_Delay(ms)�궨��
	*���������������ڼ�ϵͳ��ʱ��ʱ��
	*
==========================================*/
#include "os.h"
#include "sys.h"
#include <stdlib.h>
#include <malloc.h>
#include "led.h"
LinkStack * List=NULL;
u32 timer=0;
#define System_Ticks  100						//ÿ1/System_Ticks ����һ���ж�

/*---------------------------------------
-* File: System_init
-* Description��ϵͳ(�δ�)ʱ�ӳ�ʼ��
-* Arguments:  	void
-*---------------------------------------*/
void System_init(void)
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8 
	reload=SystemCoreClock/8000000;							//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/System_Ticks;		//����System_Ticks�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	 
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 		//ÿ1/System_Ticks���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    

}	

//�ж������Ƿ�Ϊ��
int IsEmptyList(struct LinkStack * list)  
{  
	return (list->top == NULL);  
}

/*---------------------------------------
-* File: CreateEmptyList
-* Description������һ���յ���������� 
-* Arguments:  	void
-*---------------------------------------*/
//����һ��������  
struct LinkStack * CreateEmptyList(void)  
{  
	LinkStack * list=(LinkStack *)mymalloc(0,sizeof(LinkStack)); //mymalloc(sramx,2048);//����2K�ֽ� 
	if(list == NULL)  
		
		return NULL;  
	else  
		list->top=NULL; 
	List=list;	
	return list;     
} 

/*---------------------------------------
-* File: Create_Task
-* Description������һ���µ�����
-* Arguments:  	TaskHook		��������
-* 							DELAY				��һ������ִ��ʱ��
-* 							PERIOD			ÿ��������ʱ��
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
-* Description��ɾ��һ������
-* Arguments:  	TaskHook		��������
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
				if(p->Period_TICKS)	 //Period_TICKS != 0��ʾ������������ִ�У�����ִֻ��һ��
				{
					p->Delay_MS= p->Period_TICKS;
				}
      }	
			else 
			{ 								     
         p->Delay_MS -= (1000/System_Ticks);	 //��ȥһ��ʱ����
		  }			
			p=p->link; 
	}while(p != NULL);	
}



