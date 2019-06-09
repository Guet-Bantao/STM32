
#include <math.h>
#include "pwm.h"
#include "pid.h"
extern float  Angle_accY;
extern int Point;
int PWM_MAX =155;//输出值限幅
float	zkb=0.0;

/**********************************
*			配置PWM寄存器								*
***********************************/
void init_pwm()
{
		CCON=0x00;
		CMOD = 0x02; // 设置 PCA 定时器
		CL = 0x00;//清零
		CH = 0x00;
		CCAP0L = 0xFF; //设置低电平50%占空比 8位PWM模式最大为255，一半为128，即0x80 
		CCAP0H = 0xFF; // 当CL由ff变为00溢出时，CCAP0H装载到CCAP0L中，实现无干扰的更新PWM。
		CCAPM0 = 0x42; //0100,0010 设置使用8位PWM模式	无中断	
		//CCAPM0 =0x63;//PCA模块0工作于8位PWM模式 允许比较器功能，允许PWM输出启用CCF0中断 上升沿中断 
		CR = 1; //开启 PCA 定时器.
}
void motor_PWM_Forward(int val)   //
{
   in2=0;
   in1=1;
   pwm(val);
}

void motor_PWM_Backward(int  val)
{
   in2=1;
   in1=0;
   pwm(val);
}

void pwm(unsigned char speed)
{
		CCAP0L = ~speed;
		CCAP0H = ~speed;
		zkb=speed;
}

void MotorMove( long int pwm)
{
 	if(pwm>0)
	  {	
		pwm=(unsigned char)pwm;					 
		motor_PWM_Forward(pwm);
		}
	else if(pwm<0)
	  {	
		pwm=abs(pwm);
		pwm=(unsigned char)pwm;			
		motor_PWM_Backward(pwm);
	  }
}

void Mode_basc(int Angle)
{
	long int  M1_PWM;
	PID_SetPoint(Angle);  //设置转动角度
	PID_Setkp(1.257);//1.557 1.257
 	PID_Setki(0.066);//0.020 0.068
 	PID_Setkd(0);
	M1_PWM=PID_PosLocCalc(Angle_accY);
	if(M1_PWM > PWM_MAX)
		M1_PWM =  PWM_MAX;
	if(M1_PWM < 18&&M1_PWM>3) 
		M1_PWM=18;
	if(M1_PWM < -PWM_MAX)
		M1_PWM = -PWM_MAX; 
	 if(M1_PWM > -18&&M1_PWM<-3)
		M1_PWM=-18;
	  if(M1_PWM >= -2&&M1_PWM<=2&&Angle_accY==Angle)
		M1_PWM=0;

	MotorMove(M1_PWM);
}


