/*==================Ӳ����·===============================      
 *˵��������MPU6050�ĽǶȲ�����SDA��P1^1��SCL��P1^0��
 *==========================================================*/

#include "mpu6050.h"
#include "iic.h"
#include <math.h>    //Keil library  ����atan����

unsigned int  RunningTime=0;
float xdata Gyro_x,Gyro_y,Gyro_z;//�洢����������� ��λ ��/s
float xdata Angle_x,Angle_y,Angle_z;//��λ g(9.8m/s^2)
float xdata Gyro_accX,Gyro_accY,Gyro_accY;//�洢���ֺ�ĵļ��ٶ�
float  Angle_accX,Angle_accY,Angle_accZ;//�洢���ٶȼ�����ĽǶ�
long  LastTime=0,NowTime,TimeSpan;//�����Խ��ٶȻ��ֵ�	

/*==========================================================   
 *�������ƣ�InitMPU6050()
 *��    �ܣ���ʼ��MPU6050
 *��ڲ�����
 *���ڲ�����
 *˵    ����
 *==========================================================*/
void InitMPU6050()
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//�������״̬
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}

/*==========================================================   
 *�������ƣ�GetData(unsigned char REG_Address)
 *��    �ܣ���MPU6050���ݽ��кϳ�
 *��ڲ�����REG_Address
 *���ڲ�����
 *˵    ����
 *==========================================================*/
int GetData(unsigned char REG_Address)
{
	unsigned char H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //�ϳ�����
}

/*==========================================================   
 *�������ƣ�Get_Angle()
 *��    �ܣ���ȡY��Ƕ�
 *��ڲ�����
 *���ڲ�����
 *˵    ��������һ�׻����㷨
 *==========================================================*/
float Get_Angle()
{
		Angle_y=GetData(ACCEL_YOUT_H);
		Angle_z=GetData(ACCEL_ZOUT_H);
		Gyro_y=GetData(GYRO_YOUT_H);
		Gyro_z=GetData(GYRO_ZOUT_H);
	
//�Լ��ٶȽ�����������������16384�ó���λΪg�ļ��ٶ�ֵ +-2g����
		Angle_y=(Angle_y+65)/16384.00;//ȥ�����ƫ��
		Angle_z=(Angle_z+2700)/16384.00;//ȥ�����ƫ��
//�ü��ٶȼ����������ˮƽ������ϵ֮��ļн�,�ǶȽ�Сʱ��x=sinx�õ��Ƕȣ����ȣ�, deg = rad*180/3.14
		Angle_accY= atan(Angle_y / Angle_z)*180/ pi;   //��ýǶ�ֵ������-1����
//�Խ��ٶ������� ����16.4�ó���λΪ ��/sֵ +-250�������ĸ��������� 250,500,1000,2000��Ӧ��GYRO_Sensitivity�ֱ�Ϊ131,65.5,32.8,16.4
		Gyro_y=Gyro_y/131.0;
		Gyro_z=Gyro_z/131.0; 
//===============�����Ƕ�ʱ����л��ִ���================
		NowTime=RunningTime;//��ȡ��ǰ�������еĺ�����
		RunningTime=0;
		TimeSpan=NowTime-LastTime;//����ʱ�������㲻�Ǻ��Ͻ�
		LastTime=NowTime;
//ͨ���Խ��ٶȻ���ʵ�ָ�����ĽǶȲ�����
		Gyro_accY=(Gyro_y-(Gy_offset))*TimeSpan/1000;
		Angle_accY = 0.8 * Angle_accY+ (1-0.8) * (Angle_accY + Gyro_accY);//һ�׻����˲�
		return Angle_accY;
}
