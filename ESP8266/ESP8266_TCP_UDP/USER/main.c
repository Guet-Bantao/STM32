/*==========================================================
 * �������ƣ�TFT����ͼƬ��ʾ
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������MDK5
 * ����ʱ�䣺2016/10/26
 * �޸�ʱ�䣺
 * ����˵����Img2Lcd����:C�������顢ˮƽɨ�衢16λ���ɫ������ͼ��ͷ�ļ���R5G6B5
 *			lcd.c��ע�͵���	��ӡLCD ID	����
 *==========================================================*/
/*==========================================================
 * ˵�����ȵ㡢���롢IP���˿� ��������common.c�ļ����޸�
 *		��ʹ�õ�OLED����ʾ������common.h�ļ���ע�͵���䣺#define USE_LCD 1
 *		�������䣺#define USE_OLED 1
 *		OLED�����߷�ʽ��SDA---->PB13		SCL---->PB15
 *		ESP8266���߷�ʽ��RX---->PB10		TX---->PB11
 *		ESP8266ʹ�ñȽ��鷳������ϵͳ���ʹ�ó�籦���й��磬��֤����ĵ���
 *==========================================================*/
//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart3.h"
#include "common.h" 
#ifdef USE_LCD
#include "lcd.h"
#include "image2lcd.h"
#elif USE_OLED
#include "OLED_I2C.h"
#include "codetab.h"
#endif
 	
int main(void)
{
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	usart3_init(115200);		//��ʼ������3 
#ifdef USE_LCD
//	HEADCOLOR *imginfo;//�õ��ļ���Ϣ 
//	imginfo=(HEADCOLOR*)gImage_image1;	//�õ��ļ���Ϣ//ǿ��ת��Ϊ�ṹ��
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(WHITE);//LCD_Clear(0X0000);//����
	LCD_ShowChineseString(80,40,200,24,24,(u8*)"����ѧԺ ����");
	//image_display(50,100,(u8*)gImage_image1);	//��ָ����ַ��ʾͼƬ 
#elif USE_OLED
	I2C_Configuration();
	OLED_Init();
	OLED_CLS();//����
#endif
	while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
	{
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
#ifdef USE_LCD
		LCD_ShowChineseString(0,30,200,16,16,(u8*)"cannot check module!");//δ��⵽ģ��!!!
		delay_ms(800);
		LCD_Fill(0,30,200,30+16,WHITE);
		LCD_ShowChineseString(0,30,200,16,16,(u8*)"try link module...");//��������ģ��...
#elif USE_OLED
		OLED_ShowStr(0,0,"Not find module...",1);
		delay_ms(800);
		OLED_Clean(0,0,0,128,0x00);
		OLED_ShowStr(0,0,"Try link module...",1);
#endif
	} 
	while(1) 
	{		
		atk_8266_wifista_test(1);//WIFI STA����      	
	} 
}
