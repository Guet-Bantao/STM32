#include "led.h"
#include "motor.h"
#include "time.h"
#include "pid.h"


int PWM_MAX =500;//���ֵ�޷�

int PWM_MIN=54;//���ֵ�޷�




int Run(int Angle,int adc,int M1_PWM)
{
//	long int  M1_PWM;
	PID_SetPoint(Angle);  //����ת���Ƕ�
	PID_Setkp_ki_kd(2.0,0.75,0.75);//(2.0,0.75,0.5
	M1_PWM+=PID_PosLocCalc(adc);
	if(M1_PWM > PWM_MAX)	M1_PWM=  PWM_MAX;
	if(M1_PWM < PWM_MIN)	M1_PWM= PWM_MIN;

	return(M1_PWM);
}


