/*****************************************
功能：NRF24L01射频模块头文件
*****************************************/

#ifndef _24l01_H_
#define _24l01_H_
#include "stm32f10x.h"
#include "sys.h"

extern u8 RX_ADDRESS_0[5];

#define CE 		PAout(4)//RX/TX模式选择端
#define CSN		PCout(4)//SPI片选端//就是SS
#define SCLK	PAout(5)//SPI时钟端
#define MOSI	PAout(7)//SPI主机输出从机输入端
#define MISO	PAout(6)//SPI主机输出从机输出端
#define IRQ		PCout(5)//可屏蔽中断端

#define READMISO  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)//读取

/*******************************************************/
#define TX_ADDR_WITDH 5	//发送地址宽度设置为5个字节
#define RX_ADDR_WITDH 5	//接收地址宽度设置为5个字节
#define TX_DATA_WITDH 5//发送数据宽度1个字节
#define RX_DATA_WITDH 5//接收数据宽度1个字节
/*******************命令寄存器***************************/
#define  R_REGISTER      0x00//读取配置寄存器
#define  W_REGISTER      0x20//写配置寄存器
#define  R_RX_PAYLOAD 	 0x61//读取RX有效数据
#define  W_TX_PAYLOAD	 0xa0//写TX有效数据
#define  FLUSH_TX		 0xe1//清除TXFIFO寄存器
#define  FLUSH_RX		 0xe2//清除RXFIFO寄存器
#define  REUSE_TX_PL     0xe3//重新使用上一包有效数据
#define  NOP             0xff//空操作
/******************寄存器地址****************************/
#define  CONFIG          0x00//配置寄存器
#define  EN_AA			 0x01//使能自动应答
#define  EN_RXADDR       0x02//接收通道使能0-5个通道
#define  SETUP_AW        0x03//设置数据通道地址宽度3-5
#define  SETUP_RETR      0x04//建立自动重发
#define  RF_CH           0x05//射频通道设置
#define  RF_SETUP        0x06//射频寄存器
#define  STATUS          0x07//状态寄存器
#define  OBSERVE_TX      0x08//发送检测寄存器
#define  CD              0x09//载波
#define  RX_ADDR_P0      0x0a//数据通道0接收地址
#define  RX_ADDR_P1      0x0b//数据通道1接收地址
#define  RX_ADDR_P2      0x0c//数据通道2接收地址
#define  RX_ADDR_P3      0x0d//数据通道3接收地址
#define  RX_ADDR_P4      0x0e//数据通道4接收地址
#define  RX_ADDR_P5      0x0f//数据通道5接收地址
#define  TX_ADDR         0x10//发送地址
#define  RX_PW_P0        0x11//P0通道数据宽度设置
#define  RX_PW_P1        0x12//P1通道数据宽度设置
#define  RX_PW_P2        0x13//P2通道数据宽度设置
#define  RX_PW_P3        0x14//P3通道数据宽度设置
#define  RX_PW_P4        0x15//P4通道数据宽度设置
#define  RX_PW_P5        0x16//P5通道数据宽度设置
#define  FIFO_STATUS     0x17//FIFO状态寄存器



/*******************相关函数声明**************************/
void NRF24L01_GPIOInit(void);
u8 NRFSPI(u8 date);
u8 NRFReadReg(u8 RegAddr);
u8 NRFWriteReg(u8 RegAddr,u8 date);
u8 NRFReadRxDate(u8 RegAddr,u8 *RxDate,u8 DateLen);
u8 NRFWriteTxDate(u8 RegAddr,u8 *TxDate,u8 DateLen);
void NRF24L01Int(void);
//void NRFSetRXMode();
void NRFSetRXMode(u8 *Rx_addr);
//void NRFSetTxMode(uchar *TxDate);
void NRFSetTxMode(u8 *TxDate,u8 *TxAddr,u8 PX);
u8 CheckACK(void);
void GetDate(void);

extern u8  TxAddr_P0[5]; //发送地址
/*****************状态标志*****************************************/
//sbit MAX_RT=sta^4;
/*********************************************************/
#endif

