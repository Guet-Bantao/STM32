#ifndef __FONTUPD_H__
#define __FONTUPD_H__	
#include "stm32f10x.h"

#define FONTINFOADDR 	0//1024*1024*12 		//字库存放首地址//WarShip STM32F103 V3是从12M地址以后开始存放字库
															//前面12M被fatfs占用了.
#define FONTSECSIZE	 	791//字库区域占用的总扇区数大小(3个字库+unigbk表+字库信息=3238700字节,约占791个W25QXX扇区)

//定义各个字库的大小
#define UNIGBK         171*1024        //171KB
#define GBK12_FONSIZE  562*1024        //562KB
#define GBK16_FONSIZE  749*1024			   //749KB
#define GBK24_FONSIZE  1684*1024       //1684KB




__packed typedef struct 
{
	u8 fontok;				//字库存在标志，0XAA，字库正常；其他，字库不存在
	u32 ugbkaddr; 			//unigbk的地址
	u32 ugbksize;			//unigbk的大小	 
	u32 f12addr;			//gbk12地址	
	u32 gbk12size;			//gbk12的大小	 
	u32 f16addr;			//gbk16地址
	u32 gbk16size;			//gbk16的大小		 
	u32 f24addr;			//gbk24地址
	u32 gkb24size;			//gbk24的大小 
}_font_info; 



u8 font_init(void);
u32 fupd_prog(u16 x,u16 y,u8 size,u32 fsize,u32 pos);
u8 updata_fontx(u16 x,u16 y,u8 size,u8 fx);
u8 update_font(u16 x,u16 y,u8 size);




#endif

