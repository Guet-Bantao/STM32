/*==========================================================
 * 程序名称：Nokia5110功能演示程序
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * 开发环境：keil5
 * 创建时间：2016/03/10
 * 修改时间：
 * 功能说明：
 *==========================================================*/
#include "STC12C5A60S2.H"
#include  "ziku.h"

sbit    res  = P2^7;  //复位,0复位
sbit    sce  = P2^6;  //片选
sbit    dc   = P2^5;  //1写数据，0写指令
sbit    sdin = P2^4;  //数据
sbit    sclk = P2^3;  //时钟


/*--------------------------------------------
LCD_write_byte: 使用SPI接口写数据到LCD
输入参数：dt：写入的数据；
command ：写数据/命令选择；
----------------------------------------------*/
void LCD_write_byte(unsigned char dt, unsigned char command)
{
	unsigned char i; 
	sce=0;	
	dc=command;	
	for(i=0;i<8;i++)
	{ 
		if(dt&0x80)
			sdin=1;
		else
			sdin=0;
		dt=dt<<1;		
		sclk=0; 		
		sclk=1; 
	}	
	dc=1;	
	sce=1;	
	sdin=1;
}
/*---------------------------------------
LCD_init: 3310LCD初始化
-----------------------------------------  */
void LCD_init(void)
{
	  res=0;  	
  	//delay_1ms();
  	res=1;  
	LCD_write_byte(0x21,0);//初始化Lcd,功能设定使用扩充指令，水平寻址
	LCD_write_byte(0xd0,0);//设定液晶偏置电压
	LCD_write_byte(0x20,0);//使用基本指令
	LCD_write_byte(0x0C,0);//设定显示模式，正常显示
}
/*-------------------------------------------
LCD_set_XY: 设置LCD坐标函数
输入参数：X：0－83  Y：0－5
---------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x80 | X, 0);// row
	LCD_write_byte(0x40 | Y, 0);// column
} 
/*------------------------------------------
LCD_clear: LCD清屏函数
--------------------------------------------*/
void LCD_clear(void)
{
	unsigned char t;
	unsigned char k;
	LCD_set_XY(0,0); 
	for(t=0;t<6;t++)
	{ 
		for(k=0;k<84;k++)
		{ 
			LCD_write_byte(0x00,1);//0xff为全黑				
		} 
	}
}
/*---------------------------------------------
LCD_write_shu: 显示8（宽）*16（高）点阵列数字字母符号等半角类
输入参数：c：显示的字符；
-----------------------------------------------*/
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c) //row:列 page:页 dd:字符
{
	unsigned char i;  	
	
	LCD_set_XY(row*8, page);// 列，页 
	for(i=0; i<8;i++) 
	{
		LCD_write_byte(shuzi[c*32+i],1); 
	}
	
    LCD_set_XY(row*8, page+1);// 列，页 
	for(i=8; i<16;i++) 
	{
		LCD_write_byte(shuzi[c*32+i],1);
	}	 	
}
/*---------------------------------------------
LCD_write_hanzi: 显示16（宽）*16（高）点阵列汉字等半角类
输入参数：c：显示的字符；
-----------------------------------------------*/
void LCD_write_hanzi(unsigned char row, unsigned char page,unsigned char c) //row:列 page:页 dd:字符
{
	unsigned char i;  	
	
	LCD_set_XY(row*8, page);// 列，页 
	for(i=0; i<16;i++) 
	{
		LCD_write_byte(tu[c*32+i],1); 
	}
	
    LCD_set_XY(row*8, page+1);// 列，页 
	for(i=16; i<32;i++) 
	{
		LCD_write_byte(tu[c*32+i],1);
	}	
}

void delayms(unsigned int ms)
{
	unsigned char i=100,j;
	for(;ms;ms--)
	{
		while(--i)
		{
			j=10;
			while(--j);
		}
	}
}

/*********************************************************** 
函数名称：Write_FullPic
函数功能：写整屏的图片
出口参数：无 
备 注：一般要求图片大小：宽84，高48 
***********************************************************/
void Write_FullPic(unsigned char *c,unsigned char _size)
{
        unsigned int i,j;
        for(i=0;i<6;i++)
        {
                LCD_set_XY(0,i);
                for(j=0;j<_size;j++)
                { LCD_write_byte((c[i*_size+j]),1);}        
        }
}
main()
{
		res=0;
		//for(k=0;k<250;k++);
		res=1;	  	
  	LCD_init();  //初始化LCD模块 		
  	while(1)
  	{
			LCD_clear(); //清屏幕	
		  Write_FullPic(tu,64);  //图片
//    LCD_clear(); //清屏幕	
//		LCD_write_hanzi(1,0,0);  //小
//		LCD_write_hanzi(3,0,1);  //车
//		LCD_write_hanzi(5,0,2);  //本
//		LCD_write_hanzi(7,0,3);  //次
//		LCD_write_hanzi(2,2,4);  //用		
//		LCD_write_hanzi(6,2,5);  //时

  //				
	//	LCD_write_shu(1,4,29);   //t
	//	LCD_write_shu(2,4,18); 	 //i
	//	LCD_write_shu(3,4,22); 	 //m
	//  LCD_write_shu(4,4,14); 	 //e
	//	LCD_write_shu(6,4,45); 	 //.
	//	LCD_write_shu(7,4,n); 	 //n

	  delayms(80);	
  	}
}