#ifndef __OS_H
#define __OS_H	 
#include "sys.h"

typedef  struct Task			   
{
   int (*TaskHook)(void);     // Ҫ���е�������ָ��
   u32 Delay_MS;       	          //��λ��ms
   u16 Period_TICKS;               //��λ��ʱ����(TICKS_TIME_MS)
   u8  Run;				      					//���о�����־
   //u8  Preemptive_En;              //��ռ�Ա�־
	 struct Task * link;
   
}OsTask; 
//typedef struct OsTask * sTask;         //���ָ������ 

typedef struct LinkStack           //������  
{  
	 OsTask	* top;        //����ָ��  
}LinkStack;

extern u32 timer;
extern LinkStack * List;

#define LINE_NUM_GET(line)   	line = __LINE__; case __LINE__:
#define PROCESS_BEGIN()  			static u32 now_time=0;													\
															static u8 task_LineNum=0;												\
															switch(task_LineNum) { case 0:
#define PROCESS_END()       	}	return 0;

#define Os_Delay(ms)      		now_time=timer;																	\
                              LINE_NUM_GET(task_LineNum);                 		\
															do                                              \
                              { 																							\
                                 if( timer >= (now_time+ ms) )   							\
                                 {                                           	\
																		task_LineNum=0; 													\
                                    break;                                  	\
                                 }                                           	\
                                 return 1;                                     	\
                              } while(0)
																	/*ʱ��û����������������*/


void System_init(void);																		
int IsEmptyList(struct LinkStack * list)  ;																		
struct LinkStack * CreateEmptyList(void);																		
u8 Create_Task(int (* TaskHook)(void), const u32 DELAY, const u16 PERIOD)  ; 
u8 Delete_Task( int (* TaskHook)(void)	);


#endif

