#ifndef __pid_H
#define __pid_H


void PID_Init(void);
void PID_SetPoint(float setpoint);
void PID_Setkp_ki_kd(float kp,float ki,float kd);
int  PID_PosLocCalc(float  NextPoint);

#endif
