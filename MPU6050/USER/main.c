/*==========================================================
 * �������ƣ�����mpu6050�ķɿ���̬����
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������MDK5
 * ����ʱ�䣺2016/08/21
 * �޸�ʱ�䣺
 * ����˵������λ������������V2.6�汾	500000bqs	�������˲�
 *==========================================================*/
 
//#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "mpu6050.h"
#include "usart.h"
#include "Kalman.h"
#include "ANO-Tech.h"


//�������
//float pitch,roll,yaw; 		//������pitch�������roll�������yaw
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����

extern float pitch; 	//X������б�Ƕ�pitch
extern float roll; 	//Y������б�Ƕ�roll
extern float yaw; 	//Z������б�Ƕ�yaw


int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(500000);
//	LED_Init();
	MPU6050_Init();
	while(1)
		{
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			Angle_Calcu();
			//printf("%f	%f	\r\n",roll-90,pitch);
			mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//���Զ���֡���ͼ��ٶȺ�������ԭʼ����
		usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)((roll-90)*100),(int)(pitch*100),(int)(yaw*10));
		
		}
}

