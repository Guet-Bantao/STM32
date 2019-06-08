/*==========================================================
 * 程序名称：2017板球系统 
 * 开发人员：桂林电子科技大学 创新学院 F1队
 * 开发环境：MDK5
 * 创建时间：2017/08/13
 * 修改时间：别傻了 孩子
 * 功能说明：基础前三问
 *==========================================================*/
 
//#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7670.h"
#include "timer.h"
#include "exti.h"


#define GRAY_2_RGB565(gray)             ((u16)((((u8)(gray)>>3)<<11)|(((u8)(gray)>>2)<<5)|((u8)(gray)>>3)))

//const u8*LMODE_TBL[5]={"Auto","Sunny","Cloudy","Office","Home"};							//5种光照模式	    
extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义	
extern u8 fps_put;	//在timer.c里面定义
extern u8 fps_flag;	//在timer.c里面定义	
//更新LCD显示
u16 point_x,point_y;
u16 aim_x,aim_y;//267.195  180.110// 95.200 180.110
static unsigned short ReadColor(unsigned int x,unsigned int y)
{
	unsigned short C16;//0:黑  535:白
	//u8 res;

	C16 = LCD_ReadPoint(x,y);     //读某点颜色
	//res =	 (unsigned char)((C16&0xf800)>>8);
	return C16;
}

u16 Y_Start =15;//扫描起始
u16 Y_End =230 ;//扫描终止
u16 X_Start=65 ;
u16 X_End =300; 
//小球识别  找到返回1
int SearchCentre(void)
{
	static u8 mode=1;
	unsigned int SpaceX=6,SpaceY=6;//目标最小宽度 高度
	unsigned int i,j,k,FailCount=0;
	int res;
	if(mode){	//半搜索
		for(i=Y_Start;i<Y_End;i+=SpaceY)//i等于纵轴起始点，i小于纵轴结束点，i每次自加目标最小高度
		{
			for(j=X_Start;j<X_End;j+=SpaceX)//j等于横轴起始点,j小于横轴结束点，j每次自加目标最小宽度
			{
				FailCount=0;
				for(k=0;k<SpaceX+SpaceY;k++)//k小于目标最小宽度与目标最小高度和
				{
					if(k<SpaceX)//如果k小于目标最小宽度
						res = ReadColor(j+k,i+SpaceY/2);//读点j+k,i+SpaceY/2的颜色
					else//否则
						res = ReadColor(j+SpaceX/2,i+(k-SpaceX));//读点j+SpaceX/2,i+(k-SpaceX)的颜色
					if(res)//不是黑球
						FailCount++;	//失败计数
					if(FailCount>6)//容错率为6/(SpaceX+SpaceY)
						break;
				}
				if(k==SpaceX+SpaceY)//如果k等于目标最小宽度与目标最小高度和
				{
					
					point_x = j+SpaceX/2;
					point_y = i+SpaceY/2;								
					
					if((point_y>30)&&(point_x>75)){
						X_Start=point_x-10;//要求30-10>=X_Start
						Y_Start=point_y-10;//要求75-10>=Y_Start
					}
					else if((point_x>75)&&(point_y<=30)){//死区
						X_Start=point_x-10;
					}
					else if((point_x<=75)&&(point_y>30)){//死区
						Y_Start=point_y-10;
					}
					return 1;
				}
			}
		}
		mode=0;//抛弃一桢图像识别，优化速度
	}
	else{//全搜索
		X_Start=65;//扫描起始
		Y_Start=15;
		for(i=Y_Start;i<Y_End;i+=SpaceY)//i等于纵轴起始点，i小于纵轴结束点，i每次自加目标最小高度的1/3
		{
			for(j=X_Start;j<X_End;j+=SpaceX)//j等于横轴起始点,j小于横轴结束点，j每次自加目标最小宽度1/3
			{
				FailCount=0;
				for(k=0;k<SpaceX+SpaceY;k++)//k小于目标最小宽度与目标最小高度和
				{
					if(k<SpaceX)//如果k小于目标最小宽度
						res = ReadColor(j+k,i+SpaceY/2);//读点j+k,i+SpaceY/2的颜色
					else//否则
						res = ReadColor(j+SpaceX/2,i+(k-SpaceX));//读点j+SpaceX/2,i+(k-SpaceX)的颜色
					if(res)//不是黑球
						FailCount++;	//失败计数
					if(FailCount>5)//容错率为5/(SpaceX+SpaceY)
						break;
				}
				if(k==SpaceX+SpaceY)//如果k等于目标最小宽度与目标最小高度和
				{				
					point_x = j+SpaceX/2;
					point_y = i+SpaceY/2;
					X_Start=point_x;
					Y_Start=point_y;
					mode=1;
					return 1;
				}
			}
		}
	}
	return 0;                                              
}

void camera_refresh(void) { //读取在fifo理的数据
    u16 x,y;
    u32 k;
	u16 color;
	//u32 data=0;	
	if(ov_sta) {//有帧中断更新
	LCD_Scan_Dir(U2D_R2L);		//从上到下,从左到右 U2D_L2R
    LCD_SetCursor(0,0);		
    LCD_Set_Window(0,0,320,240);//将显示区域设置到屏幕中央 
	LCD_WriteRAM_Prepare();     //开始写入GRAM	
	OV7670_RRST=0;				//开始复位读指针 
	OV7670_RCK_L;
	OV7670_RCK_H;
	OV7670_RCK_L;
	OV7670_RRST=1;				//复位读指针结束 
	OV7670_RCK_H;
	for(y=0;y<240;y++) {
		for(x=0;x<320;x++) {
			//data++;
			OV7670_RCK_L;
			color=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H;
			// color<<=8;
			OV7670_RCK_L;
			k|=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H;		
          	 
			if(color>0x5F){
				color=0xff;//白色									
			}									
			else{     //黑色
				color=0x00;
			}					
			LCD->LCD_RAM=GRAY_2_RGB565(color);					
		}   
	} 
	SearchCentre();//追踪小球
  	ov_sta=0;					//清零帧中断标记
	ov_frame++; 
	LCD_Fill(point_x-5,point_y-5,point_x+5,point_y-5+1,0xf800);//锁定小球
	LCD_Fill(point_x-5,point_y-5,point_x-5+1,point_y+5,0xf800);
	LCD_Fill(point_x-5,point_y+5,point_x+5,point_y+5+1,0xf800);
	LCD_Fill(point_x+5,point_y-5,point_x+5+1,point_y+5,0xf800);
	//LCD_Fill(point_x-2,point_y-2,point_x+2,point_y+2,0xf800);
	LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
    } 
}
/*测试LCD刷新显示
 * 由于0v7670 由上到下  从左往右 再没有恢复默认刷新方式（从左往右 从上到下）显示都无效
 *
 */
#ifdef text//不定义不编译
void camera_refresh_text(void) { //读取在fifo理的数据
    u16 x,y;
	
	LCD_Scan_Dir(U2D_R2L);		//从上到下,从左到右 
    LCD_SetCursor(0,0);		
    LCD_Set_Window(0,0,320,240);//将显示区域设置到屏幕中央 
	LCD_WriteRAM_Prepare();     //开始写入GRAM	
	for(y=0;y<240;y++) {       
	for(x=0;x<320;x++) {	  
		LCD_DrawPoint(x,y);
		point_x = x;
		point_y = y;
		LCD_Scan_Dir(DFT_SCAN_DIR);			 
		LCD_ShowNum(60,340,point_x,3,24);
		LCD_ShowNum(60,380,point_y,3,24);
		LCD_Scan_Dir(U2D_L2R);
		delay_us(1);
		}   
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向  
}
#endif


int main(void) {
	u8 first=1;
	u8 can=1;
	u8 key_flag=0;
	delay_init();	    	 //延时函数初始化	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	//uart_init(115200);	 	//串口初始化为 115200	
	//LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	if(B5==0)//模式选择
		key_flag=1;
	LCD_Init();			   		//初始化LCD  
 	POINT_COLOR=RED;			//设置字体为红色  
	while(OV7670_Init());//初始化OV7670,满桢20帧
	delay_ms(1500);	 	    
	TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断									  
	EXTI8_Init();						//使能定时器捕获
	OV7670_Window_Set(12,176,240,320);	//设置窗口	   
  	OV7670_CS=0;			
	LCD_Clear(WHITE);
	LCD_ShowString(20,340,200,24,24,(u8 *)"x:");
	LCD_ShowString(20,380,200,24,24,(u8 *)"y:");
	LCD_ShowString(20,420,200,24,24,(u8 *)"fps:");
	TIM3_PWM_Init(19999,71);//72分频。PWM频率=1000000/20000=50hz,20ms
	
	TIM_SetCompare2(TIM3,1600);//1000就是1ms 1300 1900
	TIM_SetCompare1(TIM3,1600);//上电自平衡
 	while(1) {	
		camera_refresh();//更新显示
		LCD_ShowNum(60,340,point_x,3,24);
		LCD_ShowNum(60,380,point_y,3,24);			
		if(fps_flag)
		{
			fps_flag=0;//1s满
			LCD_ShowNum(60,420,fps_put,3,24);//刷新帧数
		}
		if(point_x>5)//板子上无目标时不启动
		{
			if(first==1)//开机自检
			{
				if(point_y <150)
					can=0;//不启用pid
				first=0;
				if(key_flag==0){
				aim_x=point_x+80;//目标坑5坐标计算
				aim_y=point_y-90;
				}
				else if(key_flag==1)
				{
					aim_x=point_x+90;//目标坑4坐标计算
					aim_y=point_y;
				}
				LCD_ShowNum(150,420,aim_x,3,24);
				LCD_ShowNum(200,420,aim_y,3,24);
			}
			if(can==1)
				Handler();//pid处理
			if((abs(point_x-aim_x)<4)&&(abs(point_y-aim_y)<4)&&(key_flag==1))
			{
				delay_ms(2000);//停留2s，好像没啥用
				key_flag=0;
				aim_x=point_x;//目标坑5坐标计算
				aim_y=point_y-90;
//				TIM_SetCompare2(TIM3,1600);//1000就是1ms 1300 1900
//				TIM_SetCompare1(TIM3,1600);
			}
			if((abs(point_x-aim_x)<4)&&(abs(point_y-aim_y)<4)&&(key_flag==0))
			{
				TIM_SetCompare2(TIM3,1600);//1000就是1ms 1300 1900
				TIM_SetCompare1(TIM3,1600);
			}
		}
	

	}	   
}




