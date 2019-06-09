
#ifndef __KALMAN_H_
#define __KALMAN_H_

#define Pi 	3.1415926
//��������������
#define dt           0.012//Kalman�˲�������ʱ��
#define R_angle      0.5
#define Q_angle      0.001
#define Q_gyro       0.003//Խ��Խ�ͺ�


void Angle_Calcu(void);
void Kalman_Filter_X(float Accel,float Gyro);
void Kalman_Filter_Y(float Accel,float Gyro);
void Kalman_Filter_Z(float Accel, float Gyro);


#endif
