#include "fontupd.h"
#include "w25qxx.h"   
#include "lcd.h"  
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "dma.h"

//用来保存字库基本信息，地址，大小等
_font_info ftinfo;
extern u8 ReceBuff1[USART_REC_LEN];
extern u8 ReceBuff[USART_REC_LEN];
extern u8 GBK_BUF_Flag ;
extern u8 WRITE_Flag;



//初始化字体
//返回值:0,字库完好.
//		 其他,字库丢失
u8 font_init(void)
{		
	u8 t=0;
	W25QXX_Init();  
	while(t<10)//连续读取10次,都是错误,说明确实是有问题,得更新字库了
	{
		t++;
		W25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
		if(ftinfo.fontok==0xBB)break;
		delay_ms(20);
	}
	if(ftinfo.fontok!=0xBB)return 1;
	return 0;		    
}

//显示当前字体更新进度
//x,y:坐标
//size:字体大小
//fsize:整个文件大小
//pos:当前文件指针位置
u32 fupd_prog(u16 x,u16 y,u8 size,u32 fsize,u32 pos)
{
	float prog;
	u8 t=0XFF;
	prog=(float)pos/fsize;
	prog*=100;
	if(t!=prog)
	{
		LCD_ShowString(x+3*size/2,y,240,320,size,"%");		
		t=prog;
		if(t>100)t=100;
		LCD_ShowNum(x,y,t,3,size);//显示数值
	}
	return 0;					    
} 


//更新某一个
//x,y:坐标
//size:字体大小
//fx:更新的内容 0,ungbk;1,gbk12;2,gbk16;3,gbk24;
//返回值:0,成功;其他,失败.
u8 updata_fontx(u16 x,u16 y,u8 size,u8 fx)
{
	u32 flashaddr=0;								    
 	u8 res;	
	u32 offx=0;    
	u32 fsize=0;
	u32 GBK_OVER_Flag=0;
		switch(fx)
		{
			case 0:												//更新UNIGBK.BIN
				ftinfo.ugbkaddr=FONTINFOADDR+sizeof(ftinfo);	//信息头之后，紧跟UNIGBK转换码表
				fsize=ftinfo.ugbksize=UNIGBK;					//UNIGBK大小
				flashaddr=ftinfo.ugbkaddr;
			  printf("Please send UNIGBK.bin\r\n");
				break;
			case 1:
				ftinfo.f12addr=ftinfo.ugbkaddr+ftinfo.ugbksize;	//UNIGBK之后，紧跟GBK12字库
				fsize=ftinfo.gbk12size=GBK12_FONSIZE;					//GBK12字库大小
				flashaddr=ftinfo.f12addr;						//GBK12的起始地址
			  printf("Please send GBK12.FON\r\n");
				break;
			case 2:
				ftinfo.f16addr=ftinfo.f12addr+ftinfo.gbk12size;	//GBK12之后，紧跟GBK16字库
				fsize=ftinfo.gbk16size=GBK16_FONSIZE;					//GBK16字库大小
				flashaddr=ftinfo.f16addr;						//GBK16的起始地址
			  printf("Please send GBK16.FON\r\n");
				break;
			case 3:
				ftinfo.f24addr=ftinfo.f16addr+ftinfo.gbk16size;	//GBK16之后，紧跟GBK24字库
				fsize=ftinfo.gkb24size=GBK24_FONSIZE;					//GBK24字库大小
				flashaddr=ftinfo.f24addr;						//GBK24的起始地址
			  printf("Please send GBK24.FON\r\n");
				break;
		} 
		
		fupd_prog(x,y,size,fsize,offx);	 			//进度显示
		while(1)//死循环执行
		{
	 		if(GBK_OVER_Flag)
 				GBK_OVER_Flag++;
			if(WRITE_Flag==1)
			{
				WRITE_Flag=0;
				GBK_OVER_Flag=1;
				if(GBK_BUF_Flag==0)
					W25QXX_Write(ReceBuff,offx+flashaddr,USART_REC_LEN);		//开始写入Usart6_DMA_Len个数据  
				else if(GBK_BUF_Flag==1)
					W25QXX_Write(ReceBuff1,offx+flashaddr,USART_REC_LEN);		//开始写入Usart6_DMA_Len个数据  
				offx+=USART_REC_LEN;
				fupd_prog(x,y,size,fsize,offx);	 			//进度显示
			}
			//delay_us(15);
			if(GBK_OVER_Flag>65500)   //超过正常时间 则说明此字库发送完毕
				break;
	 	} 
		if(GBK_BUF_Flag==1)
		   W25QXX_Write(ReceBuff,offx+flashaddr,USART_REC_LEN-DMA_GetCurrDataCounter(DMA1_Channel5));//将DMA最后的一帧数据写入FLASH
		else
			 W25QXX_Write(ReceBuff1,offx+flashaddr,USART_REC_LEN-DMA_GetCurrDataCounter(DMA1_Channel5));//将DMA最后的一帧数据写入FLASH
		
		printf("This Font updated successfull!\r\n");
		MYDMA_Enable(DMA1_Channel5);		//重新初始化DMA

	return res;
}

//更新字体文件,UNIGBK,GBK12,GBK16,GBK24一起更新
//x,y:提示信息的显示地址
//size:字体大小
//提示信息字体大小										  
//返回值:0,更新成功;
//		 其他,错误代码.	  
u8 update_font(u16 x,u16 y,u8 size)//20,110,24
{	
	 u16 i,j;
	
	 LCD_ShowString(x,y,240,320,size,(u8*)"Erasing sectors... ");//提示正在擦除扇区	
		for(i=0;i<FONTSECSIZE;i++)	//先擦除字库区域,提高写入速度
		{
			fupd_prog(x+20*size/2,y,size,FONTSECSIZE,i);//进度显示
			W25QXX_Read((u8*)ReceBuff1,((FONTINFOADDR/4096)+i)*4096,4096);//读出整个扇区的内容(借用一下DMA缓冲区)
			for(j=0;j<1024;j++)//校验数据
			{
				if(ReceBuff1[j]!=0XFF)break;//需要擦除  	  
			}
			if(j!=1024)W25QXX_Erase_Sector((FONTINFOADDR/4096)+i);	//需要擦除的扇区
		}
		delay_ms(100);
		
	/*	LCD_ShowString(x,y,240,320,size,(u8*)"Updating UNIGBK.BIN  ");
		updata_fontx(x+20*size/2,y,size,0);	//更新UNIGBK.BIN
		LCD_ShowString(x,y,240,320,size,(u8*)"Updating GBK12.FON  ");
		updata_fontx(x+20*size/2,y,size,1);	//更新GBK12.FON
		LCD_ShowString(x,y,240,320,size,(u8*)"Updating GBK16.FON  ");
		updata_fontx(x+20*size/2,y,size,2);	//更新GBK16.FON
		LCD_ShowString(x,y,240,320,size,(u8*)"Updating GBK24.FON  ");
		updata_fontx(x+20*size/2,y,size,3);	//更新GBK24.FON
		//全部更新好了
		ftinfo.fontok=0xBB;
		W25QXX_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//保存字库信息
		printf("All Font file updated successfull!!!\r\n");
		LCD_ShowString(x,y,240,320,size,(u8*)"All Font file updated successfull!!!");
		LCD_Clear(WHITE);*/
		
		return 0;
}




