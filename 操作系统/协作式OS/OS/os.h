#ifndef __OS_H
#define __OS_H	 
#include "sys.h"

typedef  struct Task			   
{
   int (*TaskHook)(void);     // 要运行的任务函数指针
   u32 Delay_MS;       	          //单位是ms
   u16 Period_TICKS;               //单位是时标间隔(TICKS_TIME_MS)
   u8  Run;				      					//运行就绪标志
   //u8  Preemptive_En;              //抢占性标志
	 struct Task * link;
   
}OsTask; 
//typedef struct OsTask * sTask;         //结点指针类型 

typedef struct LinkStack           //链表定义  
{  
	 OsTask	* top;        //链表指针  
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
																	/*时间没到会跳出本次任务*/


void System_init(void);																		
int IsEmptyList(struct LinkStack * list)  ;																		
struct LinkStack * CreateEmptyList(void);																		
u8 Create_Task(int (* TaskHook)(void), const u32 DELAY, const u16 PERIOD)  ; 
u8 Delete_Task( int (* TaskHook)(void)	);


#endif

