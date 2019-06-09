#include "12864.h"
#include "keyboard.h"
#include "show.h"

unsigned char xdata Flag_decade_unit=0;
unsigned char xdata Point_hundreds=100;
unsigned char xdata Point_decade=10;
unsigned char xdata Point_unit=1;
extern unsigned char key;
extern unsigned char pre_key;
extern int Point;
extern float zkb;


/*==========================================================   
 *�������ƣ�Draw_function()
 *��    �ܣ���ʾ��ʼ����
 *��ڲ�����
 *���ڲ�����
 *˵    ����
 *==========================================================*/
void Draw_function()
{
		unsigned char i;
		for(i=0; i<4; i++)//ͨ��������ʾ���� -- i��ʾ�ֱ������λ��
		{
		 	OLED_P16x16Ch(20+(i*20),0,i+8);//����ѧԺ
		}
		for(i=0; i<7; i++)//ͨ��������ʾ���� -- i��ʾ�ֱ������λ��
		{
		 	OLED_P16x16Ch(i*16,2,i+12);//��Ժ15��������
		}
}

/*==========================================================   
 *�������ƣ�Obtain_Angle_accY(float Angle_F_Y)
 *��    �ܣ���ʾʵʱ�Ƕ�
 *��ڲ�����Angle_F_Y
 *���ڲ�����
 *˵    ����
 *==========================================================*/
void Obtain_Angle_accY(float Angle_F_Y)
{
	unsigned char i;
	unsigned int Angle_I_Y;
	OLED_P16x16Ch(2,4,0); //��	//�� ҳ �ַ�
	OLED_P16x16Ch(17,4,1);//ǰ
	OLED_P16x16Ch(32,4,2);//ռ
	OLED_P16x16Ch(47,4,3);//��
	OLED_P16x16Ch(62,4,4);//��
	OLED_P8x16Str(107,4,"%");
	OLED_P16x16Ch(77,4,32+(((int)(zkb*100/255)))/10);//x
	OLED_P16x16Ch(92,4,32+(((int)(zkb*100/255)))%10);//x
	for(i=0; i<8; i++)//��ʾ�Ƕ�
		{
			OLED_P16x16Ch(i*16,6,i+19);//��ʾ���Ƕ�xxx.x�ȡ�
		}
	Angle_I_Y=(int)(Angle_F_Y*10);//����ʮ��
		
	OLED_P16x16Ch(2*16,6,32+Angle_I_Y/1000);//��ʾ��λ
	if(Angle_I_Y>=1000)
		Angle_I_Y=Angle_I_Y%1000;
	OLED_P16x16Ch(3*16,6,32+Angle_I_Y/100);//��ʾʮλ
	if(Angle_I_Y>=100)
		Angle_I_Y=Angle_I_Y%100;
	OLED_P16x16Ch(4*16,6,32+Angle_I_Y/10);//��ʾ��λ
	OLED_P16x16Ch(6*16,6,32+Angle_I_Y%10);//��ʾС��
}

/*==========================================================   
 *�������ƣ�Set_Watch()
 *��    �ܣ���ʾ�ڶ�����
 *��ڲ�����
 *���ڲ�����
 *˵    ������ʾ�������ýǶȡ�
 *==========================================================*/
void Set_Watch()
{
		OLED_P16x16Ch(2,0,27);  //��
		OLED_P16x16Ch(17,0,28);	//��
		OLED_P16x16Ch(32,0,29);	//��
		OLED_P16x16Ch(47,0,30);	//��
		OLED_P16x16Ch(62,0,31);	//��
}

/*==========================================================   
 *�������ƣ�Set_Angle_accY()
 *��    �ܣ�����ת���Ƕ�
 *��ڲ�����
 *���ڲ�����
 *˵    ����0~9Ϊ��Ч����
 *==========================================================*/
void Set_Angle_accY()
{	
		if(Flag_decade_unit==0)//��λ
		{
			if((key==0)||(key==1))
			{
				OLED_P16x16Ch(2*16,2,32+key);
				Flag_decade_unit=1;
				Point_hundreds=100*key;
				pre_key=255;
			}
		}
		else if(Flag_decade_unit==1)//ʮλ
		{
			if((key>=0)&&(key<=9))
			{
				OLED_P16x16Ch(3*16,2,32+key);
				Flag_decade_unit=2;
				Point_decade=10*key;
				pre_key=255;
			}
		}
		else if(Flag_decade_unit==2)//��λ
		{
			if((key>=0)&&(key<=9))
			{
				OLED_P16x16Ch(4*16,2,32+key);
				Point_unit=key;
				pre_key=255;
			}
		}
		
		if(key==12)
		{
			Point=Point_hundreds+Point_decade+Point_unit;
			OLED_P8x16Str(5*16,2,"SET-OK");
		}
		
}
