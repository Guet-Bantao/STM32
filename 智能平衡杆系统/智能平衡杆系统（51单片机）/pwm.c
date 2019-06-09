
#include <math.h>
#include "pwm.h"
#include "pid.h"
extern float  Angle_accY;
extern int Point;
int PWM_MAX =155;//���ֵ�޷�
float	zkb=0.0;

/**********************************
*			����PWM�Ĵ���								*
***********************************/
void init_pwm()
{
		CCON=0x00;
		CMOD = 0x02; // ���� PCA ��ʱ��
		CL = 0x00;//����
		CH = 0x00;
		CCAP0L = 0xFF; //���õ͵�ƽ50%ռ�ձ� 8λPWMģʽ���Ϊ255��һ��Ϊ128����0x80 
		CCAP0H = 0xFF; // ��CL��ff��Ϊ00���ʱ��CCAP0Hװ�ص�CCAP0L�У�ʵ���޸��ŵĸ���PWM��
		CCAPM0 = 0x42; //0100,0010 ����ʹ��8λPWMģʽ	���ж�	
		//CCAPM0 =0x63;//PCAģ��0������8λPWMģʽ ����Ƚ������ܣ�����PWM�������CCF0�ж� �������ж� 
		CR = 1; //���� PCA ��ʱ��.
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
	PID_SetPoint(Angle);  //����ת���Ƕ�
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


