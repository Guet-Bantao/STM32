#include  "esp8266.h"
#include "led.h"
#include  "string.h"
#include  "lcd.h"
#include "stmflash.h"
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
u8 uar(char *recv)
{
	if(strstr(recv,"PID")!=0)
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
	 delay_ms(50);
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
	delay_ms(200);//�ϵ��ӳ�
	Uart_Send((u8 *)"AT+CWMODE=3\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CWSAP=\"ESP8266_xx\",\"0123456789\",11,0\r\n");//�ȵ����� ����
	UART_clear();
	LED0=0;
//	Uart_Send((u8 *)"AT+CIPMODE=1\r\n");
//	UART_clear();
	Uart_Send((u8 *)"AT+CIPMUX=1\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CIPSERVER=1,1100\r\n");
	UART_clear();
	LED0=1;
//	Uart_Send((u8 *)"AT+CIPSEND\r\n");//���봩͸
//	UART_clear();

}
#define FLASH_SAVE_ADDR  0X08070000
float esp(float set_temp)
{
	float set_num;
	static char datatemp1[4];//����flash������
	int i=10;
//	if(USART_RX_BUF[0]=='+'||(USART_RX_BUF[2]=='+'))//���ո�ʽ��\n+PIDxxxxx������
			if(USART_RX_BUF[i]==':')
		{
				LED1=0;
				set_num=(USART_RX_BUF[i+1]-'0')*10+(USART_RX_BUF[i+2]-'0')*1+(USART_RX_BUF[i+3]-'0')*0.1;
				datatemp1[0]=USART_RX_BUF[i+1];
			datatemp1[1]=USART_RX_BUF[i+2];
			datatemp1[2]=USART_RX_BUF[i+3];
			STMFLASH_Write(FLASH_SAVE_ADDR,((u16 *)datatemp1),4);//д��flash
			USART_RX_BUF[i]='0';
			return set_num;
		}
		return set_temp;
}

