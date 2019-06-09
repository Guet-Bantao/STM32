#include "led.h"
#include "motor.h"
#include "time.h"
#include "pid.h"


int PWM_MAX =1100;//输出值限幅
extern	float	roll;

void motor_PWM_Forward(int val)   //
{
   	In1=1;
	In2=0;
   TIM_SetCompare2(TIM3,val);
}

void motor_PWM_Backward(int  val)
{
    In1=0;
	In2=1;
   TIM_SetCompare2(TIM3,val);
}

void MotorMove( long int pwm)
{
 	if(pwm>0)
	  {					 
		motor_PWM_Forward(pwm);
		}
	else if(pwm<0)
	  {	
		pwm=-(pwm);		
		motor_PWM_Backward(pwm);
	  }
	else
		  motor_PWM_Forward(0);
		  
}

void Run(int Angle)
{
	long int  M1_PWM;
	PID_SetPoint(Angle);  //设置转动角度
	PID_Setkp_ki_kd(26.6,34.6,14.2);//(59.6,0.2,18)
	M1_PWM=PID_PosLocCalc(roll);
	if(M1_PWM > PWM_MAX)	M1_PWM=  PWM_MAX;
	if(M1_PWM < -PWM_MAX)	M1_PWM= -PWM_MAX;

	MotorMove(M1_PWM);
}


