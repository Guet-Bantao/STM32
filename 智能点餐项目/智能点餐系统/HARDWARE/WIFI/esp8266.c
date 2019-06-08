#include  "esp8266.h"
#include "led.h"
#include  "string.h"
char	table[]="OK";

/*==========================================================   
 *�������ƣ�hand(unsigned char *recv)
 *��    �ܣ�����Ƿ���ָ���ַ���
 *��ڲ�����recv
 *���ڲ�����0��1
 *˵    ����ָ��
 *==========================================================*/
u8 hand(char *recv)
{
	if(strstr(recv,table)!=0)
		return 1;
	else
		return 0;
}

/*==========================================================   
 *�������ƣ�Uart_Send(unsigned char * send_buf)	
 *��    �ܣ��ù����ڷ����ַ���
 *��ڲ�����send_buf
 *���ڲ�����
 *˵    ��������������ʼ��ַ ָ��
 *==========================================================*/
void Uart_Send(u8 * send_buf)
{
	do
	{
   printf ("%s",send_buf);
	 delay_ms(200);
	}
	while(!hand((char *)USART_RX_BUF));
}

/*==========================================================   
 *�������ƣ�UART_clear(void)	
 *��    �ܣ����������buf
 *��ڲ�����
 *���ڲ�����
 *˵    ����
 *==========================================================*/ 
void UART_clear(void)	   
{
	u8 i;
	for(i=0;i<USART_REC_LEN;i++)
	USART_RX_BUF[i]='z';
}

/*==========================================================   
 *�������ƣ�ESP_STA(void)	
 *��    �ܣ����ӷ�����
 *��ڲ�����
 *���ڲ�����
 *˵    ����
 *==========================================================*/ 
void ESP_STA(void)	   
{
	delay_ms(500);//�ϵ��ӳ�
	Uart_Send((u8 *)"AT+CWMODE=3\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CWSAP=\"KITE_ESP\",\"18290019557\",11,2\r\n");//�ȵ����� ����
	UART_clear();
	LED0=0;
	Uart_Send((u8 *)"AT+CIPMODE=1\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CIPMUX=0\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CIPSTART=\"TCP\",\"192.168.4.2\",1100\r\n");
	UART_clear();
	LED1=0;
	Uart_Send((u8 *)"AT+CIPSEND\r\n");//���봩͸
	UART_clear();
	LED0=1;LED1=1;
}

