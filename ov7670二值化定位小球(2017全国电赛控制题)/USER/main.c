/*==========================================================
 * �������ƣ�2017����ϵͳ 
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ F1��
 * ����������MDK5
 * ����ʱ�䣺2017/08/13
 * �޸�ʱ�䣺��ɵ�� ����
 * ����˵��������ǰ����
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

//const u8*LMODE_TBL[5]={"Auto","Sunny","Cloudy","Office","Home"};							//5�ֹ���ģʽ	    
extern u8 ov_sta;	//��exit.c�� �涨��
extern u8 ov_frame;	//��timer.c���涨��	
extern u8 fps_put;	//��timer.c���涨��
extern u8 fps_flag;	//��timer.c���涨��	
//����LCD��ʾ
u16 point_x,point_y;
u16 aim_x,aim_y;//267.195  180.110// 95.200 180.110
static unsigned short ReadColor(unsigned int x,unsigned int y)
{
	unsigned short C16;//0:��  535:��
	//u8 res;

	C16 = LCD_ReadPoint(x,y);     //��ĳ����ɫ
	//res =	 (unsigned char)((C16&0xf800)>>8);
	return C16;
}

u16 Y_Start =15;//ɨ����ʼ
u16 Y_End =230 ;//ɨ����ֹ
u16 X_Start=65 ;
u16 X_End =300; 
//С��ʶ��  �ҵ�����1
int SearchCentre(void)
{
	static u8 mode=1;
	unsigned int SpaceX=6,SpaceY=6;//Ŀ����С��� �߶�
	unsigned int i,j,k,FailCount=0;
	int res;
	if(mode){	//������
		for(i=Y_Start;i<Y_End;i+=SpaceY)//i����������ʼ�㣬iС����������㣬iÿ���Լ�Ŀ����С�߶�
		{
			for(j=X_Start;j<X_End;j+=SpaceX)//j���ں�����ʼ��,jС�ں�������㣬jÿ���Լ�Ŀ����С���
			{
				FailCount=0;
				for(k=0;k<SpaceX+SpaceY;k++)//kС��Ŀ����С�����Ŀ����С�߶Ⱥ�
				{
					if(k<SpaceX)//���kС��Ŀ����С���
						res = ReadColor(j+k,i+SpaceY/2);//����j+k,i+SpaceY/2����ɫ
					else//����
						res = ReadColor(j+SpaceX/2,i+(k-SpaceX));//����j+SpaceX/2,i+(k-SpaceX)����ɫ
					if(res)//���Ǻ���
						FailCount++;	//ʧ�ܼ���
					if(FailCount>6)//�ݴ���Ϊ6/(SpaceX+SpaceY)
						break;
				}
				if(k==SpaceX+SpaceY)//���k����Ŀ����С�����Ŀ����С�߶Ⱥ�
				{
					
					point_x = j+SpaceX/2;
					point_y = i+SpaceY/2;								
					
					if((point_y>30)&&(point_x>75)){
						X_Start=point_x-10;//Ҫ��30-10>=X_Start
						Y_Start=point_y-10;//Ҫ��75-10>=Y_Start
					}
					else if((point_x>75)&&(point_y<=30)){//����
						X_Start=point_x-10;
					}
					else if((point_x<=75)&&(point_y>30)){//����
						Y_Start=point_y-10;
					}
					return 1;
				}
			}
		}
		mode=0;//����һ��ͼ��ʶ���Ż��ٶ�
	}
	else{//ȫ����
		X_Start=65;//ɨ����ʼ
		Y_Start=15;
		for(i=Y_Start;i<Y_End;i+=SpaceY)//i����������ʼ�㣬iС����������㣬iÿ���Լ�Ŀ����С�߶ȵ�1/3
		{
			for(j=X_Start;j<X_End;j+=SpaceX)//j���ں�����ʼ��,jС�ں�������㣬jÿ���Լ�Ŀ����С���1/3
			{
				FailCount=0;
				for(k=0;k<SpaceX+SpaceY;k++)//kС��Ŀ����С�����Ŀ����С�߶Ⱥ�
				{
					if(k<SpaceX)//���kС��Ŀ����С���
						res = ReadColor(j+k,i+SpaceY/2);//����j+k,i+SpaceY/2����ɫ
					else//����
						res = ReadColor(j+SpaceX/2,i+(k-SpaceX));//����j+SpaceX/2,i+(k-SpaceX)����ɫ
					if(res)//���Ǻ���
						FailCount++;	//ʧ�ܼ���
					if(FailCount>5)//�ݴ���Ϊ5/(SpaceX+SpaceY)
						break;
				}
				if(k==SpaceX+SpaceY)//���k����Ŀ����С�����Ŀ����С�߶Ⱥ�
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

void camera_refresh(void) { //��ȡ��fifo�������
    u16 x,y;
    u32 k;
	u16 color;
	//u32 data=0;	
	if(ov_sta) {//��֡�жϸ���
	LCD_Scan_Dir(U2D_R2L);		//���ϵ���,������ U2D_L2R
    LCD_SetCursor(0,0);		
    LCD_Set_Window(0,0,320,240);//����ʾ�������õ���Ļ���� 
	LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
	OV7670_RRST=0;				//��ʼ��λ��ָ�� 
	OV7670_RCK_L;
	OV7670_RCK_H;
	OV7670_RCK_L;
	OV7670_RRST=1;				//��λ��ָ����� 
	OV7670_RCK_H;
	for(y=0;y<240;y++) {
		for(x=0;x<320;x++) {
			//data++;
			OV7670_RCK_L;
			color=GPIOC->IDR&0XFF;	//������
			OV7670_RCK_H;
			// color<<=8;
			OV7670_RCK_L;
			k|=GPIOC->IDR&0XFF;	//������
			OV7670_RCK_H;		
          	 
			if(color>0x5F){
				color=0xff;//��ɫ									
			}									
			else{     //��ɫ
				color=0x00;
			}					
			LCD->LCD_RAM=GRAY_2_RGB565(color);					
		}   
	} 
	SearchCentre();//׷��С��
  	ov_sta=0;					//����֡�жϱ��
	ov_frame++; 
	LCD_Fill(point_x-5,point_y-5,point_x+5,point_y-5+1,0xf800);//����С��
	LCD_Fill(point_x-5,point_y-5,point_x-5+1,point_y+5,0xf800);
	LCD_Fill(point_x-5,point_y+5,point_x+5,point_y+5+1,0xf800);
	LCD_Fill(point_x+5,point_y-5,point_x+5+1,point_y+5,0xf800);
	//LCD_Fill(point_x-2,point_y-2,point_x+2,point_y+2,0xf800);
	LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
    } 
}
/*����LCDˢ����ʾ
 * ����0v7670 ���ϵ���  �������� ��û�лָ�Ĭ��ˢ�·�ʽ���������� ���ϵ��£���ʾ����Ч
 *
 */
#ifdef text//�����岻����
void camera_refresh_text(void) { //��ȡ��fifo�������
    u16 x,y;
	
	LCD_Scan_Dir(U2D_R2L);		//���ϵ���,������ 
    LCD_SetCursor(0,0);		
    LCD_Set_Window(0,0,320,240);//����ʾ�������õ���Ļ���� 
	LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
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
	LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽��  
}
#endif


int main(void) {
	u8 first=1;
	u8 can=1;
	u8 key_flag=0;
	delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//uart_init(115200);	 	//���ڳ�ʼ��Ϊ 115200	
	//LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	if(B5==0)//ģʽѡ��
		key_flag=1;
	LCD_Init();			   		//��ʼ��LCD  
 	POINT_COLOR=RED;			//��������Ϊ��ɫ  
	while(OV7670_Init());//��ʼ��OV7670,����20֡
	delay_ms(1500);	 	    
	TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�									  
	EXTI8_Init();						//ʹ�ܶ�ʱ������
	OV7670_Window_Set(12,176,240,320);	//���ô���	   
  	OV7670_CS=0;			
	LCD_Clear(WHITE);
	LCD_ShowString(20,340,200,24,24,(u8 *)"x:");
	LCD_ShowString(20,380,200,24,24,(u8 *)"y:");
	LCD_ShowString(20,420,200,24,24,(u8 *)"fps:");
	TIM3_PWM_Init(19999,71);//72��Ƶ��PWMƵ��=1000000/20000=50hz,20ms
	
	TIM_SetCompare2(TIM3,1600);//1000����1ms 1300 1900
	TIM_SetCompare1(TIM3,1600);//�ϵ���ƽ��
 	while(1) {	
		camera_refresh();//������ʾ
		LCD_ShowNum(60,340,point_x,3,24);
		LCD_ShowNum(60,380,point_y,3,24);			
		if(fps_flag)
		{
			fps_flag=0;//1s��
			LCD_ShowNum(60,420,fps_put,3,24);//ˢ��֡��
		}
		if(point_x>5)//��������Ŀ��ʱ������
		{
			if(first==1)//�����Լ�
			{
				if(point_y <150)
					can=0;//������pid
				first=0;
				if(key_flag==0){
				aim_x=point_x+80;//Ŀ���5�������
				aim_y=point_y-90;
				}
				else if(key_flag==1)
				{
					aim_x=point_x+90;//Ŀ���4�������
					aim_y=point_y;
				}
				LCD_ShowNum(150,420,aim_x,3,24);
				LCD_ShowNum(200,420,aim_y,3,24);
			}
			if(can==1)
				Handler();//pid����
			if((abs(point_x-aim_x)<4)&&(abs(point_y-aim_y)<4)&&(key_flag==1))
			{
				delay_ms(2000);//ͣ��2s������ûɶ��
				key_flag=0;
				aim_x=point_x;//Ŀ���5�������
				aim_y=point_y-90;
//				TIM_SetCompare2(TIM3,1600);//1000����1ms 1300 1900
//				TIM_SetCompare1(TIM3,1600);
			}
			if((abs(point_x-aim_x)<4)&&(abs(point_y-aim_y)<4)&&(key_flag==0))
			{
				TIM_SetCompare2(TIM3,1600);//1000����1ms 1300 1900
				TIM_SetCompare1(TIM3,1600);
			}
		}
	

	}	   
}




