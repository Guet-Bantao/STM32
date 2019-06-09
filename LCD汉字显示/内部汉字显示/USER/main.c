/*==========================================================
 * �������ƣ�TFT����������ʾ
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������MDK5
 * ����ʱ�䣺2016/08/13
 * �޸�ʱ�䣺
 * ����˵����ȡģ��ʽ���ã�����+����ʽ+˳��+C51��ʽ
 *			lcd.c��ע�͵���	��ӡLCD ID	����
 *==========================================================*/

//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
//#include "usart.h"

 	
 int main(void)
 {	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(WHITE);	 
  	while(1) 
	{		 	  
		LCD_ShowString(30,40,210,24,24,"STM32F103ZET6 ^_^"); 
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
		LCD_ShowChineseString(30,90,200,16,16,"���ֵ��ӿƼ���ѧ");
		LCD_ShowChineseString(80,110,200,24,24,"����ѧԺ ����");
		LCD_ShowString(30,150,200,12,12,"2016/8/13");	      	
	} 
}
