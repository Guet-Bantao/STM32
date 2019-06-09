#include "led.h"
#include "motor.h"
#include "time.h"
#include "pid.h"
#include "lcd.h"
#include "usart.h"
int PWM_MAX =950;//输出值限幅,pwm此时最低
int PWM_MIN =50;//输出值限幅，pwm此时最高



int Run(float Angle,float temp)
{
	long int  M1_PWM;
	PID_SetPoint(Angle*10);  
	//PID_Setkp_ki_kd(2039.4,0.00,0.05);//p500(15.8-16.0) p300(15.6) p800(15.7-16.5) p1000(15.8-16.6) p1200(15.8-16.8)
	//i2(15.5-18.0) i1(15.8-)
	
	
	
	//PID_Setkp_ki_kd(2550.6,700.2,400);//2000-(0.7,0.3) 2500-(0.6,0.6) 1500-(0.9,0.1) 2700-(0.6,0.4) 2550-(0.6,0.4)
	//1000-(0.8,0.2) 1500-(1,0) 1700-(1,-0.3) 1250-(0.9,0.1) 800-(0.7,0.3) 700-(0.7,0.3)
	//500-(0.6,0.3) 600-(0.6,0.4) 800(0.6,0.4不错) 400(0.6,0.3)
	M1_PWM=PID_PosLocCalc(temp);

//	if(M1_PWM<=-0.01)
//	{
//		if(M1_PWM<-PWM_MAX) M1_PWM=  -PWM_MAX+50;
//		M1_PWM=PWM_MAX+M1_PWM;
//	}
//		
//	if(M1_PWM > PWM_MAX)	M1_PWM=  PWM_MAX;
//	if(M1_PWM < PWM_MIN)	M1_PWM= PWM_MIN;


	
	return(M1_PWM);
}


