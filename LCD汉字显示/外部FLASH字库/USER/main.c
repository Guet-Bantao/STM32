/*==========================================================
 * �������ƣ����ڸ����ⲿFLASH�ֿ�
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������MDK5
 * ����ʱ�䣺2016/10/31
 * �޸�ʱ�䣺
 * ����˵����bqs����С��128000
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

//���������ֿ������Ϣ����ַ����С��
extern _font_info ftinfo;

u8 ReceBuff[USART_REC_LEN];		//���ջ���,���USART_REC_LEN���ֽ�.
u8 ReceBuff1[USART_REC_LEN];	//���ջ���,���USART_REC_LEN���ֽ�.

 int main(void)
 {	 
	LED_Init();
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(128000);	 	//���ڳ�ʼ��Ϊ115200
//	MYDMA_Config(DMA1_Channel5,(u32)&USART1->DR,(u32)ReceBuff,USART_REC_LEN);
	LCD_Init();
	W25QXX_Init();				//��ʼ��W25Q128
	 
//	while(font_init()) 			//����ֿ�
	{
		update_font(20,110,24);//�����ֿ�//������ɺ�ע�ʹ˾���������¼�������ٴ��ϵ绹������ֿ�
	}
	POINT_COLOR=RED;
	LCD_Clear(WHITE);
	LCD_ShowString(30,40,210,24,24,"STM32F103ZET6 ^_^"); 
	LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
	LCD_ShowChineseString(30,90,200,16,16,"���ֵ��ӿƼ���ѧ");//�ڲ��ֿ�
	LCD_ShowChineseString(80,110,200,24,24,"����ѧԺ ����");//�ڲ��ֿ�
	LCD_ShowString(30,150,200,12,12,"2016/10/31");	
	LCD_ShowString(30,190,200,16,16,"Please send GBK12.FON");
	ftinfo.f12addr=FONTINFOADDR;//GBK12����ַ
	ftinfo.f16addr=ftinfo.f12addr+GBK12_FONSIZE;//GBK16����ַ
	ftinfo.f24addr=ftinfo.f16addr+GBK16_FONSIZE;//GBK24����ַ
  	while(1) 
	{
		Show_Str(30,240,200,12,"���STM32F103������",12,0);//FLASH�ⲿ�ֿ�
		Show_Str(30,280,200,16,"���STM32F103������",16,0);//FLASH�ⲿ�ֿ�
		Show_Str(30,320,250,24,"���STM32F103������",24,0);//FLASH�ⲿ�ֿ�
		LED0=0;
		delay_ms(500);
		LED0=1;
		delay_ms(500);
	} 
}
