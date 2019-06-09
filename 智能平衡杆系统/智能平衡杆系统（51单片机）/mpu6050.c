/*==================硬件电路===============================      
 *说明：基于MPU6050的角度测量，SDA接P1^1，SCL接P1^0；
 *==========================================================*/

#include "mpu6050.h"
#include "iic.h"
#include <math.h>    //Keil library  调用atan函数

unsigned int  RunningTime=0;
float xdata Gyro_x,Gyro_y,Gyro_z;//存储量化后的数据 单位 °/s
float xdata Angle_x,Angle_y,Angle_z;//单位 g(9.8m/s^2)
float xdata Gyro_accX,Gyro_accY,Gyro_accY;//存储积分后的的加速度
float  Angle_accX,Angle_accY,Angle_accZ;//存储加速度计算出的角度
long  LastTime=0,NowTime,TimeSpan;//用来对角速度积分的	

/*==========================================================   
 *函数名称：InitMPU6050()
 *功    能：初始化MPU6050
 *入口参数：
 *出口参数：
 *说    明：
 *==========================================================*/
void InitMPU6050()
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}

/*==========================================================   
 *函数名称：GetData(unsigned char REG_Address)
 *功    能：对MPU6050数据进行合成
 *入口参数：REG_Address
 *出口参数：
 *说    明：
 *==========================================================*/
int GetData(unsigned char REG_Address)
{
	unsigned char H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //合成数据
}

/*==========================================================   
 *函数名称：Get_Angle()
 *功    能：获取Y轴角度
 *入口参数：
 *出口参数：
 *说    明：采用一阶互补算法
 *==========================================================*/
float Get_Angle()
{
		Angle_y=GetData(ACCEL_YOUT_H);
		Angle_z=GetData(ACCEL_ZOUT_H);
		Gyro_y=GetData(GYRO_YOUT_H);
		Gyro_z=GetData(GYRO_ZOUT_H);
	
//对加速度进行量化，除灵敏度16384得出单位为g的加速度值 +-2g量程
		Angle_y=(Angle_y+65)/16384.00;//去除零点偏移
		Angle_z=(Angle_z+2700)/16384.00;//去除零点偏移
//用加速度计算三个轴和水平面坐标系之间的夹角,角度较小时，x=sinx得到角度（弧度）, deg = rad*180/3.14
		Angle_accY= atan(Angle_y / Angle_z)*180/ pi;   //获得角度值，乘以-1得正
//对角速度做量化 除以16.4得出单位为 °/s值 +-250°量程四个量程正负 250,500,1000,2000对应的GYRO_Sensitivity分别为131,65.5,32.8,16.4
		Gyro_y=Gyro_y/131.0;
		Gyro_z=Gyro_z/131.0; 
//===============以下是对时间进行积分处理================
		NowTime=RunningTime;//获取当前程序运行的毫秒数
		RunningTime=0;
		TimeSpan=NowTime-LastTime;//积分时间这样算不是很严谨
		LastTime=NowTime;
//通过对角速度积分实现各个轴的角度测量，
		Gyro_accY=(Gyro_y-(Gy_offset))*TimeSpan/1000;
		Angle_accY = 0.8 * Angle_accY+ (1-0.8) * (Angle_accY + Gyro_accY);//一阶互补滤波
		return Angle_accY;
}
