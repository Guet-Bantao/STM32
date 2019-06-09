/*==========================================================
 * 程序名称：TFT彩屏图片显示
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * 开发环境：MDK5
 * 创建时间：2016/10/26
 * 修改时间：
 * 功能说明：Img2Lcd设置:C语言数组、水平扫描、16位真彩色、包含图像头文件、R5G6B5
 *			lcd.c里注释掉了	打印LCD ID	函数
 *==========================================================*/

//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "led.h"
#include "image2lcd.h"
//#include "usart.h"

extern const u8 gImage_image1[];//图片数据(包含信息头),存储在image1.c里面.
 	
 int main(void)
 {
	HEADCOLOR *imginfo;//得到文件信息 
	imginfo=(HEADCOLOR*)gImage_image1;	//得到文件信息//强制转换为结构体
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	//uart_init(115200);	 	//串口初始化为115200
	LCD_Init();
	LED_Init();
	POINT_COLOR=RED;
	LCD_Clear(WHITE);//LCD_Clear(0X0000);//黑屏
	LCD_ShowChineseString(80,40,200,24,24,(u8*)"创新学院 风筝");
	image_display(50,100,(u8*)gImage_image1);	//在指定地址显示图片 
  	while(1) 
	{		 	  
//		LCD_ShowString(30,40,210,24,24,"STM32F103ZET6 ^_^"); 
//		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
//		LCD_ShowChineseString(30,90,200,16,16,"桂林电子科技大学");
//		LCD_ShowChineseString(80,110,200,24,24,"创新学院 风筝");
//		LCD_ShowString(30,150,200,12,12,"2016/8/13");	
		LED0=0;
		LED1=0;
		delay_ms(200);
		LED0=1;
		LED1=1;
		delay_ms(200);
	} 
}
