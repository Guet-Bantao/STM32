/*==========================================================
 * 程序名称：RC522
 * 开发人员：桂林电子科技大学 创新学院
 * 开发环境：MDK5
 * 创建时间：2016/11/12
 * 修改时间：
 * 功能说明：连线说明：
 *					1--SS  <----->PB0
 *					2--SCK <----->PB13
 *					3--MOSI<----->PB15
 *					4--MISO<----->PB14
 *					5--悬空
 *					6--GND <----->GND	
 *					7--RST <----->PB1
 *					8--VCC <----->VCC
 *M1卡分為16個扇區，每個扇區由4塊（塊0、塊1、塊2、塊3）組成
 *我們也將16個扇區的64個塊按絕對地址編號0~63
 *第0扇區的塊0（即絕對地址0塊），他用於存放廠商代碼，已經固化，不可更改
 *每個扇區的塊0、塊1、塊2為數據塊，可用於存放數據
 *每個扇區的塊3為控制塊（絕對地址塊3、7、11....），包括了密碼A，存取控制、密碼B。
 *要注意写块3数据，因为块3包含了所在扇区在密钥及访问条件，如果操作不当，将导致扇区无法正常使用!!!
 *==========================================================*/

//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "rc522.h"
#include "usart.h"

void ShowID(u16 x,u16 y, u8 *p);
void ShowName(u16 x,u16 y, u8 *p);


/*全局变量*/
u8 CT[2];//卡类型
u8 UID[4]; //卡号//--卡的第0扇区第0块;块0的前4个字节是UID（序列号），第5个字节是卡UID的校验位，剩下的是厂商数据。
u8 data[16] = {'K','I','T','E',0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x01,0xFE,0x01,0xFE};
u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8 RFID[16];			//存放RFID 
//u8 SendData[4]={0x09,0x09,0x09,0x09};
 int main(void)
 {	 
	u8 status;
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	InitRc522();				//初始化射频卡模块
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(WHITE);	 
	LCD_ShowChineseString(80,30,200,24,24,(u8*)"风筝");
	 //printf("ok");
  	while(1) 
	{
		status = PcdRequest(PICC_REQALL,CT);/*尋卡 输出为卡类型----CT卡类型*/	
		if(status==MI_OK)//尋卡成功
			{
				
				status=MI_ERR;
				status = PcdAnticoll(UID);/*防冲撞*/						 
			}
		if (status==MI_OK)//防衝撞成功
			{
				status=MI_ERR;	
				ShowID(10,200,UID); //在液晶屏上显示卡的ID号
				status =PcdSelect(UID);//选卡
			}
		if(status==MI_OK)//選卡成功
			{
				LCD_ShowString(50,60,210,24,16,(u8*)"PcdSelect_MI_OK");
				status=MI_ERR;
				status =PcdAuthState(0x60,0x09,KEY,UID);////驗證
			}
		if(status==MI_OK)//驗證成功
			 {
				LCD_ShowString(50,80,210,24,16,(u8*)"PcdAuthState_MI_OK");
				status=MI_ERR;
				status=PcdWrite(0x09,data);//写卡
				 //status=PcdValue(0xC1,0x09,SendData);
				status=PcdRead(0x09,RFID);//讀卡
			 }
		if(status==MI_OK)//讀卡成功
			 {
				 LCD_ShowString(50,100,210,24,16,(u8*)"READ_MI_OK");
				 status=MI_ERR;
				 ShowName(10,250, RFID);
				 printf("14564asdad}}asd\r\n");
//				 USART_ClearFlag(USART1,USART_FLAG_TC);                 //读取USART_SR
//				 USART_SendData(USART1,RFID[t]);                         //向串口1发送数据
//				 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET); //等待发送结束
			 }

		delay_ms(200);
	} 
}
 
/*************************************
*函数功能：显示卡的卡号，以十六进制显示
*参数：x，y 坐标
*		p 卡号的地址
*		charcolor 字符的颜色
*		bkcolor   背景的颜色
***************************************/
void ShowID(u16 x,u16 y, u8 *p)	 //显示卡的卡号，以十六进制显示
{
//	u8 num[9];
	u8 i;
//	for(i=0;i<4;i++)
//	{
//		num[i*2]=p[i]/16;
//		num[i*2]>9?(num[i*2]+='7'):(num[i*2]+='0');
//		num[i*2+1]=p[i]%16;
//		num[i*2+1]>9?(num[i*2+1]+='7'):(num[i*2+1]+='0');
//	}
//	num[8]=0;
	LCD_ShowString(x,y-20,200,16,16,(u8*)"The Card ID about 16 is:");
//	for(i=0;i<8;i++)
//	{
//		  LCD_ShowNum(x+16*i,y+16,num[i],2,16);
//		
//	}
	
	for(i=0;i<4;i++)
	{
		  LCD_ShowNum(x+32*i,y,p[i],3,16);
		
	}
	
}

/*************************************
*函数功能：显示卡的卡号，以十六进制显示
*参数：x，y 坐标
*		p 卡号的地址
***************************************/
void ShowName(u16 x,u16 y, u8 *p)	 //显示卡的名字
{
	u8 i;
	for(i=0;i<4;i++)
	{
		LCD_ShowChar(x+12*i,y,p[i],16,0);		
	}
	for(i=4;i<16;i++)
	{
		  LCD_ShowNum(x+28*i-48,y,p[i],3,16);		
	}
}


