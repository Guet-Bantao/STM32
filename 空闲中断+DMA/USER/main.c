/*==========================================================
 * �������ƣ����ڿ����ж�+DMA��������
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������MDK5
 * ����ʱ�䣺2016/10/30
 * �޸�ʱ�䣺
 * ����˵����
 *==========================================================*/

//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "dma.h"
 
extern u8 GBK_BUF_Flag;
u8 receive_flag = 0;
u8 ReceBuff[USART_REC_LEN];	//���ջ���,���USART_REC_LEN���ֽ�.

 int main(void)
 {	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	MYDMA_Config(DMA1_Channel5,(u32)&USART1->DR,(u32)ReceBuff,USART_REC_LEN);//
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(WHITE);
	LCD_ShowString(30,40,210,24,24,"STM32F103ZET6 ^_^"); 
	LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
	LCD_ShowChineseString(30,90,200,16,16,"���ֵ��ӿƼ���ѧ");
	LCD_ShowChineseString(80,110,200,24,24,"����ѧԺ ����");
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
