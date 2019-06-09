#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "sys.h"
#include "stm32f10x.h"

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78 0111 1000


//IO方向设置
 
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

////IO操作函数	 
#define SCL    PBout(15) //SCL
#define SDA    PBout(13) //SDA	 
//#define READ_SDA   PBin(7)  //输入SDA 


void I2C_Configuration(void);
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void Write_IIC_Byte(unsigned char IIC_Byte);	//IIC发送一个字节

void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);

void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_ShowVar(unsigned char x, unsigned char y, unsigned int num, unsigned char TextSize);

#endif
