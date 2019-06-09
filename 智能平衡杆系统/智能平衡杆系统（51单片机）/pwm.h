#ifndef __pwm_H__
#define __pwm_H__
#include <STC12C5A60S2.H>
sbit in1 =P3^7;
sbit in2 =P3^6;

void init_pwm();
void motor_PWM_Forward(int val);
void motor_PWM_Backward(int  val);
void pwm(unsigned char speed);
void MotorMove( long int pwm);
void Mode_develop(int Angle);


#endif
