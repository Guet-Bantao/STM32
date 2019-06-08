#include  "esp8266.h"
#include "led.h"
#include  "string.h"
char	table[]="OK";

/*==========================================================   
 *函数名称：hand(unsigned char *recv)
 *功    能：检测是否有指定字符串
 *入口参数：recv
 *出口参数：0与1
 *说    明：指针
 *==========================================================*/
u8 hand(char *recv)
{
	if(strstr(recv,table)!=0)
		return 1;
	else
		return 0;
}

/*==========================================================   
 *函数名称：Uart_Send(unsigned char * send_buf)	
 *功    能：用过串口发送字符串
 *入口参数：send_buf
 *出口参数：
 *说    明：发送数据起始地址 指针
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
 *函数名称：UART_clear(void)	
 *功    能：清除接收区buf
 *入口参数：
 *出口参数：
 *说    明：
 *==========================================================*/ 
void UART_clear(void)	   
{
	u8 i;
	for(i=0;i<USART_REC_LEN;i++)
	USART_RX_BUF[i]='z';
}

/*==========================================================   
 *函数名称：ESP_STA(void)	
 *功    能：连接服务器
 *入口参数：
 *出口参数：
 *说    明：
 *==========================================================*/ 
void ESP_STA(void)	   
{
	delay_ms(500);//上电延迟
	Uart_Send((u8 *)"AT+CWMODE=3\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CWSAP=\"KITE_ESP\",\"18290019557\",11,2\r\n");//热点名称 密码
	UART_clear();
	LED0=0;
	Uart_Send((u8 *)"AT+CIPMODE=1\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CIPMUX=0\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CIPSTART=\"TCP\",\"192.168.4.2\",1100\r\n");
	UART_clear();
	LED1=0;
	Uart_Send((u8 *)"AT+CIPSEND\r\n");//进入穿透
	UART_clear();
	LED0=1;LED1=1;
}

