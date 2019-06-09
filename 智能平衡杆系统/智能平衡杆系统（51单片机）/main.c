/*==========================================================
 * 程序名称：智能平衡杆系统
 * 开发人员：桂林电子科技大学	创新学院		班涛	郭绪亮
 * 开发 环境：keil5
 * 创建时间：2016/06/23
 * 修改时间：2016/06/30
 * 功能说明：按下按键13 完成基础要求一（45度）
 *					 按下按键14	完成基础要求二(90度)
 *					 按下按键11	完成基础要求三(0~180度)
 *					 参考电压：7.6V
 *==========================================================*/

//#include <stdio.h>   //Keil library	测试
#include "mpu6050.h"
#include "iic.h"
#include "pwm.h"
#include "pid.h"
#include "12864.h"
#include "keyboard.h"
#include "show.h"
//#include "uart.h"//测试

//****************************************
//定义类型及变量
//****************************************
bit  Flag_Set = 0; //12864界面切换标志位
//bit  PIDRunFlag = 0;   	//PID运行标志位，PID算法不是一直在运算。而是每隔一定时间，算一次。
unsigned char xdata T0RH = 0;  //T0重载值的高字节
unsigned char xdata T0RL = 0;  //T0重载值的低字节
extern unsigned int  RunningTime;//计算时间
extern float  Angle_accY;
extern unsigned char xdata Flag_decade_unit;//设置角度十、个位标志位
unsigned char pre_key = 255;
unsigned char key;
unsigned int Key_Counter = 0;
int Point;//目标角度

void	Deal_key();
/*==========================================================   
 *函数名称：delay1ms(unsigned int k)
 *功    能：延时
 *入口参数：k
 *出口参数：
 *说    明：ms级延时，11.0592Mhz
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
 *函数名称：ConfigTimer0(unsigned int ms)
 *功    能：配置定时器0
 *入口参数：ms
 *出口参数：
 *说    明：ms级定时器，11.0592Mhz
 *==========================================================*/
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //临时变量

    tmp = 11059200 / 12;      //定时器计数频率
    tmp = (tmp * ms) / 1000;  //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 17;           //补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp>>8);  //定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}

/*==========================================================   
 *函数名称：Tain_key()
 *功    能：获取固定按键值
 *入口参数：
 *出口参数：
 *说    明：最开始没有按键按下时，key键值为255
 *==========================================================*/
void Tain_key()
{
	if(Key_Counter>=2)//消抖
			{
				Key_Counter=0;
				key=Get_key();
				if(key>15)
					key=pre_key;
				pre_key=key;
			}
}

//*********************************************************
//主程序
//*********************************************************
void main()
{
		OLED_Init(); //OLED初始化
		delay1ms(500);		//MPU6050上电延时		
		ConfigTimer0(1);
		init_pwm();
		InitMPU6050();	//初始化MPU6050
		delay1ms(300);
		EA=1;
		PID_Init();
		Draw_function();//显示初始界面
		while(1)
		{
			Angle_accY=Get_Angle();
			Angle_accY=Angle_accY+90.0;
			Obtain_Angle_accY(Angle_accY);//12864显示角度
			Tain_key();//获取按键值
			Deal_key();//处理按键值
		}
}

/*==========================================================   
 *函数名称：Deal_key()
 *功    能：处理按键值
 *入口参数：
 *出口参数：
 *说    明：不同的键值执行相应的功能
 *==========================================================*/
void	Deal_key()
{
	if(Flag_Set == 0)//初始界面
	{
		if(key==13)//按13键完成基础部分1
		{
			Point=45;//基础要求1目标值	
			Mode_basc(Point);
		}
		else if(key==14)//按14键完成基础部分2
		{
			Point=90;//基础要求2目标值	
			Mode_basc(Point);
		}
		else if(key==11)//按11键设置角度并转到设置界面 基础部分3
		{
			OLED_CLS();//清屏
			Set_Watch();
			Flag_Set = 1;
			Flag_decade_unit=0;			
		}
		else if(key==15)//按15键启动电机
		{
			Mode_basc(Point);
		}
	}
//*********************************************************	
	if(Flag_Set == 1)//设定角度界面
	{
		Set_Angle_accY();	
		if(key==15)//按15键启动电机并回到初始界面
		{
			Flag_Set = 0;
			OLED_CLS();//清屏
			Draw_function();//显示
		}
	}
}

void InterruptTimer0() interrupt 1
{
//		static unsigned int PIDCounter = 0;  
    TH0 = T0RH;  //重新加载重载值
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
