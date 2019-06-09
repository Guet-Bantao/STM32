/*==========================================================
 * 程序名称：智能平衡杆
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * 开发环境：keil5
 * 创建时间：2016/09/05
 * 修改时间：
 * 功能说明：In1--PA4;In2--PA5;PWM--PA7
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

float pitch,roll,yaw; 		//欧拉角

int	Transform (int ret);


int main(void)
{
	u8	i;
	delay_init();
	Moter_Init();//电机驱动初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);
	I2C_Configuration();
	OLED_Init();
	MPU_Init();					//初始化MPU6050
	OLED_CLS();//清屏
	OLED_ShowStr(0,2,"MPU6050_Init:",2);
	while(mpu_dmp_init())//初始化DMP
 	{
		OLED_ShowStr(10 ,4,"Erron",2);
	} 
	OLED_CLS();//清屏
	for(i=8;i<=11;i++)
	{
		OLED_ShowCN(20+(i-8)*20,2,i);//创新学院
	}
	OLED_ShowStr(0,6,"Roll:",2);
	TIM3_PWM_Init(1199,0);//不分频。PWM频率=72000000/1200Khz
	while(1)
	{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			OLED_ShowVar(40,6,(int)roll,2);//显示变量
			if((roll<10)&&(roll>=0))
			OLED_ShowStr(48,6,"' ",2);
			if((roll<100)&&(roll>=10))
			OLED_ShowStr(56,6,"' ",2);
		}
			Run(Transform(USART_RX_BUF[0]));//转动

	}
}

int	Transform (int ret)
{
	u8 ret4,ret3;//定义位的值

    ret4=ret/0x10;                   //取出千位的值,注意0x1000是个十六进制数

//    ret=ret<<4;                       //ret变量左移四位

    ret3=ret%0x10;                  //取出百位的值

//    ret=ret<<4;

//    ret2=ret/0x1000;                 //取出十位的值

//    ret=ret<<4;

//    ret1=ret/0x1000;                 //取出个位的值

//    ret=ret1+ret2*16+ret3*16*16+ret4*16*16*16;//各位的值乘以位权,完成十六转十进制
ret=ret4*10+ret3;
    return(ret);
}
