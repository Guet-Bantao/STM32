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
 *函数名称：Draw_function()
 *功    能：显示初始界面
 *入口参数：
 *出口参数：
 *说    明：
 *==========================================================*/
void Draw_function()
{
		unsigned char i;
		for(i=0; i<4; i++)//通过点整显示汉字 -- i表示字表数组的位置
		{
		 	OLED_P16x16Ch(20+(i*20),0,i+8);//创新学院
		}
		for(i=0; i<7; i++)//通过点整显示汉字 -- i表示字表数组的位置
		{
		 	OLED_P16x16Ch(i*16,2,i+12);//三院15级物联网
		}
}

/*==========================================================   
 *函数名称：Obtain_Angle_accY(float Angle_F_Y)
 *功    能：显示实时角度
 *入口参数：Angle_F_Y
 *出口参数：
 *说    明：
 *==========================================================*/
void Obtain_Angle_accY(float Angle_F_Y)
{
	unsigned char i;
	unsigned int Angle_I_Y;
	OLED_P16x16Ch(2,4,0); //当	//列 页 字符
	OLED_P16x16Ch(17,4,1);//前
	OLED_P16x16Ch(32,4,2);//占
	OLED_P16x16Ch(47,4,3);//空
	OLED_P16x16Ch(62,4,4);//比
	OLED_P8x16Str(107,4,"%");
	OLED_P16x16Ch(77,4,32+(((int)(zkb*100/255)))/10);//x
	OLED_P16x16Ch(92,4,32+(((int)(zkb*100/255)))%10);//x
	for(i=0; i<8; i++)//显示角度
		{
			OLED_P16x16Ch(i*16,6,i+19);//显示“角度xxx.x度”
		}
	Angle_I_Y=(int)(Angle_F_Y*10);//扩大十倍
		
	OLED_P16x16Ch(2*16,6,32+Angle_I_Y/1000);//显示百位
	if(Angle_I_Y>=1000)
		Angle_I_Y=Angle_I_Y%1000;
	OLED_P16x16Ch(3*16,6,32+Angle_I_Y/100);//显示十位
	if(Angle_I_Y>=100)
		Angle_I_Y=Angle_I_Y%100;
	OLED_P16x16Ch(4*16,6,32+Angle_I_Y/10);//显示个位
	OLED_P16x16Ch(6*16,6,32+Angle_I_Y%10);//显示小数
}

/*==========================================================   
 *函数名称：Set_Watch()
 *功    能：显示第二界面
 *入口参数：
 *出口参数：
 *说    明：显示“请设置角度”
 *==========================================================*/
void Set_Watch()
{
		OLED_P16x16Ch(2,0,27);  //请
		OLED_P16x16Ch(17,0,28);	//设
		OLED_P16x16Ch(32,0,29);	//置
		OLED_P16x16Ch(47,0,30);	//角
		OLED_P16x16Ch(62,0,31);	//度
}

/*==========================================================   
 *函数名称：Set_Angle_accY()
 *功    能：设置转到角度
 *入口参数：
 *出口参数：
 *说    明：0~9为有效按键
 *==========================================================*/
void Set_Angle_accY()
{	
		if(Flag_decade_unit==0)//百位
		{
			if((key==0)||(key==1))
			{
				OLED_P16x16Ch(2*16,2,32+key);
				Flag_decade_unit=1;
				Point_hundreds=100*key;
				pre_key=255;
			}
		}
		else if(Flag_decade_unit==1)//十位
		{
			if((key>=0)&&(key<=9))
			{
				OLED_P16x16Ch(3*16,2,32+key);
				Flag_decade_unit=2;
				Point_decade=10*key;
				pre_key=255;
			}
		}
		else if(Flag_decade_unit==2)//个位
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
