#include "pid.h"


typedef struct	PID
{
	float SetPoint;//设定目标 Desired Value
	double SumError;//误差累计  

	float Porportion;//比例常数 Proportional Const 
	float Intergral;//积分常数 Integral Const
	float Derivative;//微分常数 Derivative Const 

	float LastError;//Error[-1] 
	float PrevError;//Error[-2] 

}PIDTypdDef;

PIDTypdDef PID;

void PID_Init(void)
{
	PID.LastError  = 0;//Error[-1] 
	PID.PrevError   = 0;//Error[-2] 
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

void PID_Setkp_ki_kd(float kp,float ki,float kd)
{
	PID.Porportion = kp;
	PID.Intergral  = ki;
	PID.Derivative = kd;
}


int  PID_PosLocCalc(float  NextPoint)
{   
	register float iError, iIncpid; //当前误差  
  
	
	iError = PID.SetPoint - NextPoint; //增量计算  

	iIncpid = PID.Porportion * iError //E[k]项  
	  
	+ PID.Intergral * PID.LastError //E[k－1]项  
	  
	- PID.Derivative * PID.PrevError; //E[k－2]项  
	  
	//存储误差，用于下次计算  
	  
	PID.PrevError = PID.LastError;  
	  
	PID.LastError = iError;  
	//返回增量值  
	return(iIncpid);	
	
}

