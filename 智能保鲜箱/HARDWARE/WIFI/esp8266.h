#ifndef __ESP8266_H
#define __ESP8266_H
#include "delay.h"
#include "usart.h"	



u8 hand(char *recv);
void Uart_Send(u8 * send_buf);
void UART_clear(void);
void ESP_STA(void);
void UART_clear(void);

u8 uar(char *recv);
float esp(float set_temp);








#endif

