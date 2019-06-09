/*==========================================================
 * �������ƣ�����ƽ���
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������keil5
 * ����ʱ�䣺2016/09/05
 * �޸�ʱ�䣺
 * ����˵����In1--PA4;In2--PA5;PWM--PA7
 *==========================================================*/

#include "OLED_I2C.h"
#include "delay.h"
#include "codetab.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "led.h"	
#include "time.h"
#include "motor.h"
#include "usart.h"

float pitch,roll,yaw; 		//ŷ����

int	Transform (int ret);


int main(void)
{
	u8	i;
	delay_init();
	Moter_Init();//���������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);
	I2C_Configuration();
	OLED_Init();
	MPU_Init();					//��ʼ��MPU6050
	OLED_CLS();//����
	OLED_ShowStr(0,2,"MPU6050_Init:",2);
	while(mpu_dmp_init())//��ʼ��DMP
 	{
		OLED_ShowStr(10 ,4,"Erron",2);
	} 
	OLED_CLS();//����
	for(i=8;i<=11;i++)
	{
		OLED_ShowCN(20+(i-8)*20,2,i);//����ѧԺ
	}
	OLED_ShowStr(0,6,"Roll:",2);
	TIM3_PWM_Init(1199,0);//����Ƶ��PWMƵ��=72000000/1200Khz
	while(1)
	{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			OLED_ShowVar(40,6,(int)roll,2);//��ʾ����
			if((roll<10)&&(roll>=0))
			OLED_ShowStr(48,6,"' ",2);
			if((roll<100)&&(roll>=10))
			OLED_ShowStr(56,6,"' ",2);
		}
			Run(Transform(USART_RX_BUF[0]));//ת��

	}
}

int	Transform (int ret)
{
	u8 ret4,ret3;//����λ��ֵ

    ret4=ret/0x10;                   //ȡ��ǧλ��ֵ,ע��0x1000�Ǹ�ʮ��������

//    ret=ret<<4;                       //ret����������λ

    ret3=ret%0x10;                  //ȡ����λ��ֵ

//    ret=ret<<4;

//    ret2=ret/0x1000;                 //ȡ��ʮλ��ֵ

//    ret=ret<<4;

//    ret1=ret/0x1000;                 //ȡ����λ��ֵ

//    ret=ret1+ret2*16+ret3*16*16+ret4*16*16*16;//��λ��ֵ����λȨ,���ʮ��תʮ����
ret=ret4*10+ret3;
    return(ret);
}
