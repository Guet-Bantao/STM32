/*==========================================================
 * 程序名称：ESP8266与手机通信
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * EMAIL   ：1216104245@qq.com
 * 开发环境：keil5
 * 创建时间：2016/08/09
 * 修改时间：
 * 功能说明：通过手机控制LED灯 上位机为有人网络助手(默认server模式)
 *					注意透传只能在单连接模式下进行，所以在建立连接之前
 *					一定要用（AT+CIPMUX=0 设置单连接） 
 *					但是模块处于服务器模式下时，必须要多链接，
 *					由于冲突，所以模块开启服务器模式不能透传！
 *==========================================================*/
//#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "usart.h"	
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
	 delay_ms(20);
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

int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);
	LED_Init();LED0=0;
	Uart_Send((u8 *)"AT+CWMODE=3\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CWSAP=\"ESP8266\",\"0123456789\",11,0\r\n");//热点名称 密码
	UART_clear();
	Uart_Send((u8 *)"AT+CIPMODE=1\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CIPMUX=0\r\n");
	UART_clear();
	Uart_Send((u8 *)"AT+CIPSTART=\"TCP\",\"192.168.4.2\",1100\r\n");
	UART_clear();
	while(1)
	{
		if(USART_RX_BUF[0]=='a')//如果接受到字符a
		{
			LED1=!LED1;
			USART_RX_BUF[0]=' ';//清除字符
		}	
	}
}

