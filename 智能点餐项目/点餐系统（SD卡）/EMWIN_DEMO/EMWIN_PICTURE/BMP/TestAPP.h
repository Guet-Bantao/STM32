#ifndef __BAGAPP_H
#define __BAGAPP_H
#include "sys.h"
#include "WM.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin ����APP   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/21
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

extern WM_HWIN MenuAPPWin;           //BagAPP����
extern WM_HWIN CheckAPPWin;          //BookAPP����
extern WM_HWIN PaymentAPPWin;        //BookshAPP����
extern WM_HWIN BrowerAPPWin;        //BrowerAPP����

void CreateMenuAPP(void);            //����BagAPP
void CreateCheckAPP(void);           //����BookAPP
void CreatePaymentAPP(void);          //����BookshAPP
void CreateBrowerAPP(void);         //����BrowerAPP

int dispbmpex(u8 *BMPFileName,u8 mode,u32 x,u32 y,int member,int denom);
#endif



