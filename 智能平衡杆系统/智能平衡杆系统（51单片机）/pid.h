#ifndef __pid_H__
#define __pid_H__
#include <STC12C5A60S2.H>


void PID_Init(void);
void PID_SetPoint(float setpoint);
void PID_Setkd(float kd);
void PID_Setki(float ki);
void PID_Setkp(float kp);
int  PID_PosLocCalc(float  NextPoint);
void Mode_basc(int Angle);

#endif
