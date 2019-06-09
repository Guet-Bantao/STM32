#include "pid.h"
#include "pwm.h"
#include <math.h>    //Keil library  µ÷ÓÃfabsº¯Êý
#include <STC12C5A60S2.H>

typedef struct
{
	float SetPoint;
	double SumError;

	float Porportion;
	float Intergral;
	float Derivative;

	float LastError;
	float PrevError;

}PIDTypdDef;

PIDTypdDef PID;

void PID_Init(void)
{
	PID.LastError  = 0;
	PID.PrevError   = 0;
	PID.Porportion = 0;
	PID.Intergral  = 0;
	PID.Derivative = 0;
	PID.SetPoint   = 0;
	PID.SumError   = 0;	
}

void PID_SetPoint(float setpoint)
{
	PID.SetPoint = setpoint;
}

void PID_Setkd(float kd)
{
	PID.Derivative = kd;
}

void PID_Setki(float ki)
{
	PID.Intergral  = ki;
}

void PID_Setkp(float kp)
{
	PID.Porportion = kp;
}

int  PID_PosLocCalc(float  NextPoint)
{   
	register float iError,dError;
	iError = PID.SetPoint - NextPoint;

	PID.SumError+= iError;

	if(PID.SumError >6000.0)
		PID.SumError=6000.0;
	else if(PID.SumError < -6000.0)
		PID.SumError=-6000.0;
	dError =iError -PID.LastError;
	PID.LastError=iError;
	
	return   (long int)   (PID.Porportion  * iError
					      +PID.Intergral  * PID.SumError //2000 *  
					      +PID.Derivative * dError);	
	
}

