#ifndef __BAGAPP_H
#define __BAGAPP_H
#include "sys.h"
#include "WM.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//STemwin 测试APP   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/3/21
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

extern WM_HWIN MenuAPPWin;           //BagAPP窗口
extern WM_HWIN CheckAPPWin;          //BookAPP窗口
extern WM_HWIN PaymentAPPWin;        //BookshAPP窗口
extern WM_HWIN BrowerAPPWin;        //BrowerAPP窗口

void CreateMenuAPP(void);            //创建BagAPP
void CreateCheckAPP(void);           //创建BookAPP
void CreatePaymentAPP(void);          //创建BookshAPP
void CreateBrowerAPP(void);         //创建BrowerAPP

int dispbmpex(u8 *BMPFileName,u8 mode,u32 x,u32 y,int member,int denom);
#endif



