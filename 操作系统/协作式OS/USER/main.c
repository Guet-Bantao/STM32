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
	if(t==12)//����12��(12*1000ms)��ɾ������2��3
	{
		Delete_Task(Task2);
		Delete_Task(Task3);
	}
	return 0;
}

void Scheduler_Tasks(void) ;

int main(void)
{
	my_mem_init(0);		//��ʼ���ڲ��ڴ��
	(void)CreateEmptyList();
	System_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LED_Init();
	Create_Task(Task1, 0, 120)  ; //����1��ϵͳʱ�ӿ�ʼ��0ms���У�ÿ��120ms����һ��
	Create_Task(Task2, 0, 250)  ; 	
	Create_Task(Task3, 0, 1000)  ; //����3��ϵͳʱ�ӿ�ʼ��0ms���У�ÿ��1000ms����һ��

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
			if(Rotation->Run != 0) //�����ǰ�����Ǻ���ʽ�������Ѿ��� 
			{
				if(!(Rotation->TaskHook)())  	//���и�����,�������������ϵͳ��ʱ���������
					Rotation->Run-=1; 					//�����д�����һ
				//�������д����������ȴ���һ��CPU����Ȩ
				 if(Rotation->Period_TICKS== 0)  //������Ǹ������Ρ����񣬽����Ӷ�����ɾ��
				{
					Delete_Task(Rotation->TaskHook);
				} //���������Ե����񽫼�������
			}			
			Rotation=Rotation->link; 
	}while(Rotation != NULL);
}

