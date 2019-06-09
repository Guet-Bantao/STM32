/*==========================================================
 * 程序名称：OLED12864模块功能演示程序
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * 开发环境：keil5
 * 创建时间：2016/07/20
 * 修改时间：
 * 功能说明：
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
//		OLED_Fill(0xFF);//全屏点亮
//		DelayS(2);
//		OLED_Fill(0x00);//全屏灭
//		DelayS(2);
		OLED_CLS();//清屏
		for(i=0;i<=7;i++)
		{
			OLED_ShowCN(2+i*15,0,i);//桂林电子科技大学
			OLED_ShowCN(2+i*15,4,i+12);//三院15物联网工程
			OLED_ShowCN(2+i*15,6,i+20);//
		}
		for(i=8;i<=11;i++)
		{
			OLED_ShowCN(20+(i-8)*20,2,i);//创新学院
		}
		DelayS(4);
		OLED_CLS();//清屏
		OLED_ShowStr(0,1,"HelTec Automation",1);//测试6*8字符
		OLED_ShowStr(0,2,"Hello Tech",2);		//测试8*16字符
		OLED_ShowStr(0,6,"Email:1216104245@qq",1);
		OLED_ShowStr(0,7,"Number:1500360108",1);
		DelayS(3);
		OLED_CLS();//清屏
		OLED_OFF();//测试OLED休眠
		DelayS(2);
		OLED_ON();//测试OLED休眠后唤醒
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
		DelayS(2);
		OLED_CLS();//清屏
	}
}
