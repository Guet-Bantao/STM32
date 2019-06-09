#include "pid.h"
#include "led.h"


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


int  PID_PosLocCalc(float  NextPoint)//NextPointΪ��ǰ�¶�
{  
	int out;
	NextPoint=NextPoint*10;//�¶�����ʮ��
	if((NextPoint<=PID.SetPoint+1)&&(NextPoint>=PID.SetPoint-1))//PID.SetPointΪĿ���¶�
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
		out = 50;//ȫ�����
	else if(NextPoint < PID.SetPoint-6)
		out = 950;//
	return out;//���pwm

	
//	register float iError, iIncpid; //��ǰ��� λ��ʽpid
//	int state;
//	iError = -(PID.SetPoint - NextPoint); //��������
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
//	iIncpid=(PID.Porportion*iError)/* ��ǰƫ��*����ϵ��Kp */
//	
//	+ ( PID.Intergral * PID.SumError * state )/* ����*����ϵ��Ki */

//	+ PID.Derivative*(iError-PID.LastError);/* ����ǰƫ��-�ϴ�ƫ�*΢��ϵ�� */
//	
//	PID.LastError = iError; 
//	
//	return(iIncpid);
	
	
/*	register float iError, iIncpid; //��ǰ���  ����ʽpid
  
	
	iError = (PID.SetPoint - NextPoint); //��������  

	iIncpid = PID.Porportion * iError //E[k]��  
	  
	- PID.Intergral * PID.LastError //E[k��1]�� - 
	  
	+ PID.Derivative * PID.PrevError; //E[k��2]�� + 
	  
	//�洢�������´μ���  
	  
	PID.PrevError = PID.LastError;  
	  
	PID.LastError = iError;  
	//��������ֵ  
	return(iIncpid);	*/
	
}

