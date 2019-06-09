#include "pid.h"
#include "led.h"


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


int  PID_PosLocCalc(float  NextPoint)//NextPoint为当前温度
{  
	int out;
	NextPoint=NextPoint*10;//温度扩大十倍
	if((NextPoint<=PID.SetPoint+1)&&(NextPoint>=PID.SetPoint-1))//PID.SetPoint为目标温度
		out=300+(PID.SetPoint*1.2-NextPoint)*4;
	else if(NextPoint < PID.SetPoint+3 &&NextPoint > PID.SetPoint+1)
		out = 200+(NextPoint-PID.SetPoint*1.1)*4;
	else if(NextPoint > PID.SetPoint-3 &&NextPoint < PID.SetPoint-1)
		out = 500+(NextPoint-PID.SetPoint*1.1)*4;
	else if(NextPoint <= PID.SetPoint+6 &&NextPoint >= PID.SetPoint+3)
		out = 200+(NextPoint-PID.SetPoint*1.1)*2;
	else if(NextPoint >= PID.SetPoint-6 &&NextPoint <= PID.SetPoint-3)
		out = 500+(NextPoint-PID.SetPoint*1.1)*2;
	else if(NextPoint > PID.SetPoint+6)//
		out = 50;//全速输出
	else if(NextPoint < PID.SetPoint-6)
		out = 950;//
	return out;//输出pwm

	
//	register float iError, iIncpid; //当前误差 位置式pid
//	int state;
//	iError = -(PID.SetPoint - NextPoint); //增量计算
//	PID.SumError+=iError;
////	if(PID.SumError>800) PID.SumError=800;
////	if(PID.SumError<-800) PID.SumError=-800;
//	if(iError<0.1&&iError>-0.1)
//	{
//		state=0;
//		LED0=0;
//	}
//	else
//	{
//		state=1;
//		LED0=1;
//	}
//	
//	iIncpid=(PID.Porportion*iError)/* 当前偏差*比例系数Kp */
//	
//	+ ( PID.Intergral * PID.SumError * state )/* 积分*积分系数Ki */

//	+ PID.Derivative*(iError-PID.LastError);/* （当前偏差-上次偏差）*微分系数 */
//	
//	PID.LastError = iError; 
//	
//	return(iIncpid);
	
	
/*	register float iError, iIncpid; //当前误差  增量式pid
  
	
	iError = (PID.SetPoint - NextPoint); //增量计算  

	iIncpid = PID.Porportion * iError //E[k]项  
	  
	- PID.Intergral * PID.LastError //E[k－1]项 - 
	  
	+ PID.Derivative * PID.PrevError; //E[k－2]项 + 
	  
	//存储误差，用于下次计算  
	  
	PID.PrevError = PID.LastError;  
	  
	PID.LastError = iError;  
	//返回增量值  
	return(iIncpid);	*/
	
}

