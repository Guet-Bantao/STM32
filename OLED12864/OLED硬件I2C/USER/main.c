/*==========================================================
 * �������ƣ�OLED12864ģ�鹦����ʾ����
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������keil5
 * ����ʱ�䣺2016/07/20
 * �޸�ʱ�䣺
 * ����˵����
 *==========================================================*/

#include "OLED_I2C.h"
#include "oleddelay.h"
#include "codetab.h"
extern  unsigned char BMP1[];
extern  unsigned char F6x8[][6];
extern  unsigned char F8X16[];
extern  unsigned char F16x16[];

int main(void)
{
	unsigned char i;
	
	DelayInit();
	I2C_Configuration();
	OLED_Init();
	
	while(1)
	{
//		OLED_Fill(0xFF);//ȫ������
//		DelayS(2);
//		OLED_Fill(0x00);//ȫ����
//		DelayS(2);
		OLED_CLS();//����
		for(i=0;i<=7;i++)
		{
			OLED_ShowCN(2+i*15,0,i);//���ֵ��ӿƼ���ѧ
			OLED_ShowCN(2+i*15,4,i+12);//��Ժ15����������
			OLED_ShowCN(2+i*15,6,i+20);//
		}
		for(i=8;i<=11;i++)
		{
			OLED_ShowCN(20+(i-8)*20,2,i);//����ѧԺ
		}
		DelayS(4);
		OLED_CLS();//����
		OLED_ShowStr(0,1,"HelTec Automation",1);//����6*8�ַ�
		OLED_ShowStr(0,2,"Hello Tech",2);		//����8*16�ַ�
		OLED_ShowStr(0,6,"Email:1216104245@qq",1);
		OLED_ShowStr(0,7,"Number:1500360108",1);
		DelayS(3);
		OLED_CLS();//����
		OLED_OFF();//����OLED����
		DelayS(2);
		OLED_ON();//����OLED���ߺ���
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
		DelayS(2);
		OLED_CLS();//����
	}
}
