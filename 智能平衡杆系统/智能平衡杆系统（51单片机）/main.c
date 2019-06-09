/*==========================================================
 * �������ƣ�����ƽ���ϵͳ
 * ������Ա�����ֵ��ӿƼ���ѧ	����ѧԺ		����	������
 * ���� ������keil5
 * ����ʱ�䣺2016/06/23
 * �޸�ʱ�䣺2016/06/30
 * ����˵�������°���13 ��ɻ���Ҫ��һ��45�ȣ�
 *					 ���°���14	��ɻ���Ҫ���(90��)
 *					 ���°���11	��ɻ���Ҫ����(0~180��)
 *					 �ο���ѹ��7.6V
 *==========================================================*/

//#include <stdio.h>   //Keil library	����
#include "mpu6050.h"
#include "iic.h"
#include "pwm.h"
#include "pid.h"
#include "12864.h"
#include "keyboard.h"
#include "show.h"
//#include "uart.h"//����

//****************************************
//�������ͼ�����
//****************************************
bit  Flag_Set = 0; //12864�����л���־λ
//bit  PIDRunFlag = 0;   	//PID���б�־λ��PID�㷨����һֱ�����㡣����ÿ��һ��ʱ�䣬��һ�Ρ�
unsigned char xdata T0RH = 0;  //T0����ֵ�ĸ��ֽ�
unsigned char xdata T0RL = 0;  //T0����ֵ�ĵ��ֽ�
extern unsigned int  RunningTime;//����ʱ��
extern float  Angle_accY;
extern unsigned char xdata Flag_decade_unit;//���ýǶ�ʮ����λ��־λ
unsigned char pre_key = 255;
unsigned char key;
unsigned int Key_Counter = 0;
int Point;//Ŀ��Ƕ�

void	Deal_key();
/*==========================================================   
 *�������ƣ�delay1ms(unsigned int k)
 *��    �ܣ���ʱ
 *��ڲ�����k
 *���ڲ�����
 *˵    ����ms����ʱ��11.0592Mhz
 *==========================================================*/
void delay1ms(unsigned int k)	
{						
	unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
		for(j=0;j<121;j++);
	}						
}
/*==========================================================   
 *�������ƣ�ConfigTimer0(unsigned int ms)
 *��    �ܣ����ö�ʱ��0
 *��ڲ�����ms
 *���ڲ�����
 *˵    ����ms����ʱ����11.0592Mhz
 *==========================================================*/
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //��ʱ����

    tmp = 11059200 / 12;      //��ʱ������Ƶ��
    tmp = (tmp * ms) / 1000;  //��������ļ���ֵ
    tmp = 65536 - tmp;        //���㶨ʱ������ֵ
    tmp = tmp + 17;           //�����ж���Ӧ��ʱ��ɵ����
    T0RH = (unsigned char)(tmp>>8);  //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //����T0�Ŀ���λ
    TMOD |= 0x01;   //����T0Ϊģʽ1
    TH0 = T0RH;     //����T0����ֵ
    TL0 = T0RL;
    ET0 = 1;        //ʹ��T0�ж�
    TR0 = 1;        //����T0
}

/*==========================================================   
 *�������ƣ�Tain_key()
 *��    �ܣ���ȡ�̶�����ֵ
 *��ڲ�����
 *���ڲ�����
 *˵    �����ʼû�а�������ʱ��key��ֵΪ255
 *==========================================================*/
void Tain_key()
{
	if(Key_Counter>=2)//����
			{
				Key_Counter=0;
				key=Get_key();
				if(key>15)
					key=pre_key;
				pre_key=key;
			}
}

//*********************************************************
//������
//*********************************************************
void main()
{
		OLED_Init(); //OLED��ʼ��
		delay1ms(500);		//MPU6050�ϵ���ʱ		
		ConfigTimer0(1);
		init_pwm();
		InitMPU6050();	//��ʼ��MPU6050
		delay1ms(300);
		EA=1;
		PID_Init();
		Draw_function();//��ʾ��ʼ����
		while(1)
		{
			Angle_accY=Get_Angle();
			Angle_accY=Angle_accY+90.0;
			Obtain_Angle_accY(Angle_accY);//12864��ʾ�Ƕ�
			Tain_key();//��ȡ����ֵ
			Deal_key();//������ֵ
		}
}

/*==========================================================   
 *�������ƣ�Deal_key()
 *��    �ܣ�������ֵ
 *��ڲ�����
 *���ڲ�����
 *˵    ������ͬ�ļ�ִֵ����Ӧ�Ĺ���
 *==========================================================*/
void	Deal_key()
{
	if(Flag_Set == 0)//��ʼ����
	{
		if(key==13)//��13����ɻ�������1
		{
			Point=45;//����Ҫ��1Ŀ��ֵ	
			Mode_basc(Point);
		}
		else if(key==14)//��14����ɻ�������2
		{
			Point=90;//����Ҫ��2Ŀ��ֵ	
			Mode_basc(Point);
		}
		else if(key==11)//��11�����ýǶȲ�ת�����ý��� ��������3
		{
			OLED_CLS();//����
			Set_Watch();
			Flag_Set = 1;
			Flag_decade_unit=0;			
		}
		else if(key==15)//��15���������
		{
			Mode_basc(Point);
		}
	}
//*********************************************************	
	if(Flag_Set == 1)//�趨�ǶȽ���
	{
		Set_Angle_accY();	
		if(key==15)//��15������������ص���ʼ����
		{
			Flag_Set = 0;
			OLED_CLS();//����
			Draw_function();//��ʾ
		}
	}
}

void InterruptTimer0() interrupt 1
{
//		static unsigned int PIDCounter = 0;  
    TH0 = T0RH;  //���¼�������ֵ
    TL0 = T0RL;
		RunningTime++;
		Key_Counter++;
//		PIDCounter++;
//	if(PIDCounter==2)
//	{
//		PIDRunFlag = 1;
//		PIDCounter = 0;
//	}
}
