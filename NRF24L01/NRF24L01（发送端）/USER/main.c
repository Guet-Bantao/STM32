/*==========================================================
 * 程序名称：NRF24L01通信(发送端)
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * 开发环境：keil5
 * 创建时间：2016/07/22
 * 修改时间：
 * 功能说明：
 *==========================================================*/
#include "stm32f10x.h"
#include "24l01.h"
#include "delay.h"
#include "led.h"

//在发送端，数据通道0被用来接收确认信息，
//因此发送端的数据通道0的地址必须等于发送地址，这样才能收到应答信息
u8  TxAddr_P0[5]={0x34,0x42,0x11,0x13,0x02}; //发送地址
u8  TxAddr_P1[5]={0x35,0x43,0x10,0x10,0x01}; //发送地址
u8  TxAddr_P2[5]={0x36,0x43,0x10,0x10,0x01}; //发送地址
u8  TxAddr_P3[5]={0x37,0x43,0x10,0x10,0x01}; //发送地址
u8  TxAddr_P4[5]={0x38,0x43,0x10,0x10,0x01}; //发送地址
u8  TxAddr_P5[5]={0x39,0x43,0x10,0x10,0x01}; //发送地址
  
u8 RX_ADDRESS_0[5]={0x34,0x42,0x11,0x13,0x02}; //P0
u8 RX_ADDRESS_1[5]={0x35,0x43,0x10,0x10,0x01}; //P1
u8 RX_ADDRESS_2[5]={0x36,0x43,0x10,0x10,0x01}; //P2
u8 RX_ADDRESS_3[5]={0x37,0x43,0x10,0x10,0x01}; //P3
u8 RX_ADDRESS_4[5]={0x38,0x43,0x10,0x10,0x01}; //P4
u8 RX_ADDRESS_5[5]={0x39,0x43,0x10,0x10,0x01}; //P5

u8  TxDate[5];

 int main(void)
{
	//u8 sta;
	LED_Init();//初始化
	delay_init();//初始化
	NRF24L01_GPIOInit();//初始化24L01的IO口
	NRF24L01Int();
	NRFSetRXMode(RX_ADDRESS_0);//接收,先设置成接收模式再设置为发送模式，不然累成狗都不好使
	while(1)
	{								
		TxDate[0]='a';
		NRFSetTxMode(TxDate,TxAddr_P0,RX_ADDR_P0);//NRF设置为发送模式并发送数据
		//NRFReadRxDate(STATUS,&sta,1);
		while(CheckACK());	  //检测是否发送完
		//Delay(50);
		LED0=0;
	}
}

