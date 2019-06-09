/*==========================================================
 * 程序名称：串口空闲中断+DMA接收数据
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * 开发环境：MDK5
 * 创建时间：2016/10/30
 * 修改时间：
 * 功能说明：
 *==========================================================*/

//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "dma.h"
 
extern u8 GBK_BUF_Flag;
u8 receive_flag = 0;
u8 ReceBuff[USART_REC_LEN];	//接收缓冲,最大USART_REC_LEN个字节.

 int main(void)
 {	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	MYDMA_Config(DMA1_Channel5,(u32)&USART1->DR,(u32)ReceBuff,USART_REC_LEN);//
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(WHITE);
	LCD_ShowString(30,40,210,24,24,"STM32F103ZET6 ^_^"); 
	LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
	LCD_ShowChineseString(30,90,200,16,16,"桂林电子科技大学");
	LCD_ShowChineseString(80,110,200,24,24,"创新学院 风筝");
	LCD_ShowString(30,150,200,12,12,"2016/10/30");	 
  	while(1) 
	{
		if(receive_flag==1)	
		{
			receive_flag=0;
			LCD_Fill(50,200,270,220 ,WHITE);
		}			
		LCD_ShowString(50,200,200,16,24,ReceBuff);

	} 
}
