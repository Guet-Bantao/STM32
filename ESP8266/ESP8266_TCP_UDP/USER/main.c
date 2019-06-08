/*==========================================================
 * 程序名称：TFT彩屏图片显示
 * 开发人员：桂林电子科技大学 创新学院 风筝
 * 开发环境：MDK5
 * 创建时间：2016/10/26
 * 修改时间：
 * 功能说明：Img2Lcd设置:C语言数组、水平扫描、16位真彩色、包含图像头文件、R5G6B5
 *			lcd.c里注释掉了	打印LCD ID	函数
 *==========================================================*/
/*==========================================================
 * 说明：热点、密码、IP、端口 等数据在common.c文件里修改
 *		若使用的OLED屏显示，则在common.h文件里注释掉语句：#define USE_LCD 1
 *		并添加语句：#define USE_OLED 1
 *		OLED屏接线方式：SDA---->PB13		SCL---->PB15
 *		ESP8266接线方式：RX---->PB10		TX---->PB11
 *		ESP8266使用比较麻烦，整个系统最好使用充电宝进行供电，保证充足的电流
 *==========================================================*/
//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart3.h"
#include "common.h" 
#ifdef USE_LCD
#include "lcd.h"
#include "image2lcd.h"
#elif USE_OLED
#include "OLED_I2C.h"
#include "codetab.h"
#endif
 	
int main(void)
{
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	usart3_init(115200);		//初始化串口3 
#ifdef USE_LCD
//	HEADCOLOR *imginfo;//得到文件信息 
//	imginfo=(HEADCOLOR*)gImage_image1;	//得到文件信息//强制转换为结构体
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(WHITE);//LCD_Clear(0X0000);//黑屏
	LCD_ShowChineseString(80,40,200,24,24,(u8*)"创新学院 风筝");
	//image_display(50,100,(u8*)gImage_image1);	//在指定地址显示图片 
#elif USE_OLED
	I2C_Configuration();
	OLED_Init();
	OLED_CLS();//清屏
#endif
	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
#ifdef USE_LCD
		LCD_ShowChineseString(0,30,200,16,16,(u8*)"cannot check module!");//未检测到模块!!!
		delay_ms(800);
		LCD_Fill(0,30,200,30+16,WHITE);
		LCD_ShowChineseString(0,30,200,16,16,(u8*)"try link module...");//尝试连接模块...
#elif USE_OLED
		OLED_ShowStr(0,0,"Not find module...",1);
		delay_ms(800);
		OLED_Clean(0,0,0,128,0x00);
		OLED_ShowStr(0,0,"Try link module...",1);
#endif
	} 
	while(1) 
	{		
		atk_8266_wifista_test(1);//WIFI STA测试      	
	} 
}
