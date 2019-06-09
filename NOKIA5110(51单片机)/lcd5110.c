/*==========================================================
 * �������ƣ�Nokia5110������ʾ����
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������keil5
 * ����ʱ�䣺2016/03/10
 * �޸�ʱ�䣺
 * ����˵����
 *==========================================================*/
#include "STC12C5A60S2.H"
#include  "ziku.h"

sbit    res  = P2^7;  //��λ,0��λ
sbit    sce  = P2^6;  //Ƭѡ
sbit    dc   = P2^5;  //1д���ݣ�0дָ��
sbit    sdin = P2^4;  //����
sbit    sclk = P2^3;  //ʱ��


/*--------------------------------------------
LCD_write_byte: ʹ��SPI�ӿ�д���ݵ�LCD
���������dt��д������ݣ�
command ��д����/����ѡ��
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
LCD_init: 3310LCD��ʼ��
-----------------------------------------  */
void LCD_init(void)
{
	  res=0;  	
  	//delay_1ms();
  	res=1;  
	LCD_write_byte(0x21,0);//��ʼ��Lcd,�����趨ʹ������ָ�ˮƽѰַ
	LCD_write_byte(0xd0,0);//�趨Һ��ƫ�õ�ѹ
	LCD_write_byte(0x20,0);//ʹ�û���ָ��
	LCD_write_byte(0x0C,0);//�趨��ʾģʽ��������ʾ
}
/*-------------------------------------------
LCD_set_XY: ����LCD���꺯��
���������X��0��83  Y��0��5
---------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x80 | X, 0);// row
	LCD_write_byte(0x40 | Y, 0);// column
} 
/*------------------------------------------
LCD_clear: LCD��������
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
			LCD_write_byte(0x00,1);//0xffΪȫ��				
		} 
	}
}
/*---------------------------------------------
LCD_write_shu: ��ʾ8����*16���ߣ�������������ĸ���ŵȰ����
���������c����ʾ���ַ���
-----------------------------------------------*/
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c) //row:�� page:ҳ dd:�ַ�
{
	unsigned char i;  	
	
	LCD_set_XY(row*8, page);// �У�ҳ 
	for(i=0; i<8;i++) 
	{
		LCD_write_byte(shuzi[c*32+i],1); 
	}
	
    LCD_set_XY(row*8, page+1);// �У�ҳ 
	for(i=8; i<16;i++) 
	{
		LCD_write_byte(shuzi[c*32+i],1);
	}	 	
}
/*---------------------------------------------
LCD_write_hanzi: ��ʾ16����*16���ߣ������к��ֵȰ����
���������c����ʾ���ַ���
-----------------------------------------------*/
void LCD_write_hanzi(unsigned char row, unsigned char page,unsigned char c) //row:�� page:ҳ dd:�ַ�
{
	unsigned char i;  	
	
	LCD_set_XY(row*8, page);// �У�ҳ 
	for(i=0; i<16;i++) 
	{
		LCD_write_byte(tu[c*32+i],1); 
	}
	
    LCD_set_XY(row*8, page+1);// �У�ҳ 
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
�������ƣ�Write_FullPic
�������ܣ�д������ͼƬ
���ڲ������� 
�� ע��һ��Ҫ��ͼƬ��С����84����48 
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
  	LCD_init();  //��ʼ��LCDģ�� 		
  	while(1)
  	{
			LCD_clear(); //����Ļ	
		  Write_FullPic(tu,64);  //ͼƬ
//    LCD_clear(); //����Ļ	
//		LCD_write_hanzi(1,0,0);  //С
//		LCD_write_hanzi(3,0,1);  //��
//		LCD_write_hanzi(5,0,2);  //��
//		LCD_write_hanzi(7,0,3);  //��
//		LCD_write_hanzi(2,2,4);  //��		
//		LCD_write_hanzi(6,2,5);  //ʱ

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