#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//读取按键2
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2(WK_UP) 

void KEY_Init(void);//初始化
u8 KEY_Scan(u8 mode);

		 				    
#endif

