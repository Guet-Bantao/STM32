#include "pid.h"


typedef struct	PID
{
	float SetPoint;//�趨Ŀ�� Desired Value
	double SumError;//����ۼ�  

	float Porportion;//�������� Proportional Const 
	float Intergral;//���ֳ��� Integral Const
	float Derivative;//΢�ֳ��� Derivative Const 

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
	register float iError, iIncpid; //��ǰ���  
  
	
	iError = PID.SetPoint - NextPoint; //��������  

	iIncpid = PID.Porportion * iError //E[k]��  
	  
	+ PID.Intergral * PID.LastError //E[k��1]��  
	  
	- PID.Derivative * PID.PrevError; //E[k��2]��  
	  
	//�洢�������´μ���  
	  
	PID.PrevError = PID.LastError;  
	  
	PID.LastError = iError;  
	//��������ֵ  
	return(iIncpid);	
	
}

