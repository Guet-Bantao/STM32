/*==========================================================
 * �������ƣ�ESP8266���ֻ�ͨ��
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * EMAIL   ��1216104245@qq.com
 * ����������keil5
 * ����ʱ�䣺2016/08/09
 * �޸�ʱ�䣺
 * ����˵����ͨ���ֻ�����LED�� ��λ��Ϊ������������(Ĭ��serverģʽ)
 *					ע��͸��ֻ���ڵ�����ģʽ�½��У������ڽ�������֮ǰ
 *					һ��Ҫ�ã�AT+CIPMUX=0 ���õ����ӣ� 
 *					����ģ�鴦�ڷ�����ģʽ��ʱ������Ҫ�����ӣ�
 *					���ڳ�ͻ������ģ�鿪��������ģʽ����͸����
 *==========================================================*/
//#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "usart.h"	
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
	 delay_ms(20);
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

int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);
	LED_Init();LED0=0;
	Uart_Send((u8 *)"AT+CWMODE=3\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CWSAP=\"ESP8266\",\"0123456789\",11,0\r\n");//�ȵ����� ����
	UART_clear();
	Uart_Send((u8 *)"AT+CIPMODE=1\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CIPMUX=0\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CIPSTART=\"TCP\",\"192.168.4.2\",1100\r\n");
	UART_clear();
	while(1)
	{
		if(USART_RX_BUF[0]=='a')//������ܵ��ַ�a
		{
			LED1=!LED1;
			USART_RX_BUF[0]=' ';//����ַ�
		}	
	}
}

