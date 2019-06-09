#ifndef __FONTUPD_H__
#define __FONTUPD_H__	
#include "stm32f10x.h"

#define FONTINFOADDR 	0//1024*1024*12 		//�ֿ����׵�ַ//WarShip STM32F103 V3�Ǵ�12M��ַ�Ժ�ʼ����ֿ�
															//ǰ��12M��fatfsռ����.
#define FONTSECSIZE	 	791//�ֿ�����ռ�õ�����������С(3���ֿ�+unigbk��+�ֿ���Ϣ=3238700�ֽ�,Լռ791��W25QXX����)

//��������ֿ�Ĵ�С
#define UNIGBK         171*1024        //171KB
#define GBK12_FONSIZE  562*1024        //562KB
#define GBK16_FONSIZE  749*1024			   //749KB
#define GBK24_FONSIZE  1684*1024       //1684KB




__packed typedef struct 
{
	u8 fontok;				//�ֿ���ڱ�־��0XAA���ֿ��������������ֿⲻ����
	u32 ugbkaddr; 			//unigbk�ĵ�ַ
	u32 ugbksize;			//unigbk�Ĵ�С	 
	u32 f12addr;			//gbk12��ַ	
	u32 gbk12size;			//gbk12�Ĵ�С	 
	u32 f16addr;			//gbk16��ַ
	u32 gbk16size;			//gbk16�Ĵ�С		 
	u32 f24addr;			//gbk24��ַ
	u32 gkb24size;			//gbk24�Ĵ�С 
}_font_info; 



u8 font_init(void);
u32 fupd_prog(u16 x,u16 y,u8 size,u32 fsize,u32 pos);
u8 updata_fontx(u16 x,u16 y,u8 size,u8 fx);
u8 update_font(u16 x,u16 y,u8 size);




#endif

