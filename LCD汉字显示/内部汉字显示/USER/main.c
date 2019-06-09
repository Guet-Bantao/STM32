/*==========================================================
 * 程序名称：TFT彩屏汉字显示
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * 开发环境：MDK5
 * 创建时间：2016/08/13
 * 修改时间：
 * 功能说明：取模方式设置：阴码+逐列式+顺向+C51格式
 *			lcd.c里注释掉了	打印LCD ID	函数
 *==========================================================*/

//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
//#include "usart.h"

 	
 int main(void)
 {	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	//uart_init(115200);	 	//串口初始化为115200
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(WHITE);	 
  	while(1) 
	{		 	  
		LCD_ShowString(30,40,210,24,24,"STM32F103ZET6 ^_^"); 
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
		LCD_ShowChineseString(30,90,200,16,16,"桂林电子科技大学");
		LCD_ShowChineseString(80,110,200,24,24,"创新学院 风筝");
		LCD_ShowString(30,150,200,12,12,"2016/8/13");	      	
	} 
}
