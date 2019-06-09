/*==========================================================
 * 程序名称：基于mpu6050的飞控姿态解析
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * 开发环境：MDK5
 * 创建时间：2016/08/21
 * 修改时间：
 * 功能说明：上位机采用匿名的V2.6版本	500000bqs	卡尔曼滤波
 *==========================================================*/
 
//#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "mpu6050.h"
#include "usart.h"
#include "Kalman.h"
#include "ANO-Tech.h"


//定义变量
//float pitch,roll,yaw; 		//俯仰角pitch、横滚角roll、航向角yaw
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据

extern float pitch; 	//X最终倾斜角度pitch
extern float roll; 	//Y最终倾斜角度roll
extern float yaw; 	//Z最终倾斜角度yaw


int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(500000);
//	LED_Init();
	MPU6050_Init();
	while(1)
		{
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			Angle_Calcu();
			//printf("%f	%f	\r\n",roll-90,pitch);
			mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//用自定义帧发送加速度和陀螺仪原始数据
		usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)((roll-90)*100),(int)(pitch*100),(int)(yaw*10));
		
		}
}

