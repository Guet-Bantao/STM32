/*==========================================================
 * 程序名称：串口更新外部FLASH字库
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * 开发环境：MDK5
 * 创建时间：2016/10/31
 * 修改时间：
 * 功能说明：bqs必须小于128000
 *==========================================================*/

//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "dma.h"
#include "w25qxx.h" 
#include "fontupd.h"
#include "text.h"
#include "led.h"

//用来保存字库基本信息，地址，大小等
extern _font_info ftinfo;

u8 ReceBuff[USART_REC_LEN];		//接收缓冲,最大USART_REC_LEN个字节.
u8 ReceBuff1[USART_REC_LEN];	//接收缓冲,最大USART_REC_LEN个字节.

 int main(void)
 {	 
	LED_Init();
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(128000);	 	//串口初始化为115200
//	MYDMA_Config(DMA1_Channel5,(u32)&USART1->DR,(u32)ReceBuff,USART_REC_LEN);
	LCD_Init();
	W25QXX_Init();				//初始化W25Q128
	 
//	while(font_init()) 			//检查字库
	{
		update_font(20,110,24);//更新字库//更新完成后注释此句在重新烧录。否则再次上电还会擦除字库
	}
	POINT_COLOR=RED;
	LCD_Clear(WHITE);
	LCD_ShowString(30,40,210,24,24,"STM32F103ZET6 ^_^"); 
	LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
	LCD_ShowChineseString(30,90,200,16,16,"桂林电子科技大学");//内部字库
	LCD_ShowChineseString(80,110,200,24,24,"创新学院 风筝");//内部字库
	LCD_ShowString(30,150,200,12,12,"2016/10/31");	
	LCD_ShowString(30,190,200,16,16,"Please send GBK12.FON");
	ftinfo.f12addr=FONTINFOADDR;//GBK12基地址
	ftinfo.f16addr=ftinfo.f12addr+GBK12_FONSIZE;//GBK16基地址
	ftinfo.f24addr=ftinfo.f16addr+GBK16_FONSIZE;//GBK24基地址
  	while(1) 
	{
		Show_Str(30,240,200,12,"你好STM32F103开发板",12,0);//FLASH外部字库
		Show_Str(30,280,200,16,"你好STM32F103开发板",16,0);//FLASH外部字库
		Show_Str(30,320,250,24,"你好STM32F103开发板",24,0);//FLASH外部字库
		LED0=0;
		delay_ms(500);
		LED0=1;
		delay_ms(500);
	} 
}
