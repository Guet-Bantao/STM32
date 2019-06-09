/*==========================================================
 * �������ƣ�TFT����ͼƬ��ʾ
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������MDK5
 * ����ʱ�䣺2016/10/26
 * �޸�ʱ�䣺
 * ����˵����Img2Lcd����:C�������顢ˮƽɨ�衢16λ���ɫ������ͼ��ͷ�ļ���R5G6B5
 *			lcd.c��ע�͵���	��ӡLCD ID	����
 *==========================================================*/

//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "led.h"
#include "image2lcd.h"
//#include "usart.h"

extern const u8 gImage_image1[];//ͼƬ����(������Ϣͷ),�洢��image1.c����.
 	
 int main(void)
 {
	HEADCOLOR *imginfo;//�õ��ļ���Ϣ 
	imginfo=(HEADCOLOR*)gImage_image1;	//�õ��ļ���Ϣ//ǿ��ת��Ϊ�ṹ��
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LCD_Init();
	LED_Init();
	POINT_COLOR=RED;
	LCD_Clear(WHITE);//LCD_Clear(0X0000);//����
	LCD_ShowChineseString(80,40,200,24,24,(u8*)"����ѧԺ ����");
	image_display(50,100,(u8*)gImage_image1);	//��ָ����ַ��ʾͼƬ 
  	while(1) 
	{		 	  
//		LCD_ShowString(30,40,210,24,24,"STM32F103ZET6 ^_^"); 
//		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
//		LCD_ShowChineseString(30,90,200,16,16,"���ֵ��ӿƼ���ѧ");
//		LCD_ShowChineseString(80,110,200,24,24,"����ѧԺ ����");
//		LCD_ShowString(30,150,200,12,12,"2016/8/13");	
		LED0=0;
		LED1=0;
		delay_ms(200);
		LED0=1;
		LED1=1;
		delay_ms(200);
	} 
}
