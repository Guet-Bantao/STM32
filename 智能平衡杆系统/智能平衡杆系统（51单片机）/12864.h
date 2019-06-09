#ifndef __12864_H__
#define __12864_H__
#include <STC12C5A60S2.H>

// ------------------------------------------------------------
// IO口模拟I2C通信
// SCL接P2^6
// SDA接P2^7
// ------------------------------------------------------------
sbit SCL12864=P2^0; //串行时钟
sbit SDA12864=P2^1; //串行数据

void delay(unsigned int z);
void IIC_Start();
void IIC_Stop();
void Write_IIC_Byte(unsigned char IIC_Byte);
void OLED_WrDat(unsigned char IIC_Data);
void OLED_WrCmd(unsigned char IIC_Command);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char bmp_dat);
void OLED_CLS(void);
void OLED_Init(void);
void OLED_P8x16Str(unsigned char x, y,unsigned char ch[]);
void OLED_P16x16Ch(unsigned char x, y, N);



#endif
