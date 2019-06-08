/*==========================================================
 * �������ƣ�RC522
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ
 * ����������MDK5
 * ����ʱ�䣺2016/11/12
 * �޸�ʱ�䣺
 * ����˵��������˵����
 *					1--SS  <----->PB0
 *					2--SCK <----->PB13
 *					3--MOSI<----->PB15
 *					4--MISO<----->PB14
 *					5--����
 *					6--GND <----->GND	
 *					7--RST <----->PB1
 *					8--VCC <----->VCC
 *M1���֞�16���ȅ^��ÿ���ȅ^��4�K���K0���K1���K2���K3���M��
 *�҂�Ҳ��16���ȅ^��64���K���^����ַ��̖0~63
 *��0�ȅ^�ĉK0�����^����ַ0�K��������춴�ŏS�̴��a���ѽ��̻������ɸ���
 *ÿ���ȅ^�ĉK0���K1���K2�锵���K������춴�Ŕ���
 *ÿ���ȅ^�ĉK3����ƉK���^����ַ�K3��7��11....�����������ܴaA����ȡ���ơ��ܴaB��
 *Ҫע��д��3���ݣ���Ϊ��3������������������Կ������������������������������������޷�����ʹ��!!!
 *==========================================================*/

//#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "rc522.h"
#include "usart.h"

void ShowID(u16 x,u16 y, u8 *p);
void ShowName(u16 x,u16 y, u8 *p);


/*ȫ�ֱ���*/
u8 CT[2];//������
u8 UID[4]; //����//--���ĵ�0������0��;��0��ǰ4���ֽ���UID�����кţ�����5���ֽ��ǿ�UID��У��λ��ʣ�µ��ǳ������ݡ�
u8 data[16] = {'K','I','T','E',0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x01,0xFE,0x01,0xFE};
u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8 RFID[16];			//���RFID 
//u8 SendData[4]={0x09,0x09,0x09,0x09};
 int main(void)
 {	 
	u8 status;
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	InitRc522();				//��ʼ����Ƶ��ģ��
	LCD_Init();
	POINT_COLOR=RED;
	LCD_Clear(WHITE);	 
	LCD_ShowChineseString(80,30,200,24,24,(u8*)"����");
	 //printf("ok");
  	while(1) 
	{
		status = PcdRequest(PICC_REQALL,CT);/*���� ���Ϊ������----CT������*/	
		if(status==MI_OK)//�����ɹ�
			{
				
				status=MI_ERR;
				status = PcdAnticoll(UID);/*����ײ*/						 
			}
		if (status==MI_OK)//���nײ�ɹ�
			{
				status=MI_ERR;	
				ShowID(10,200,UID); //��Һ��������ʾ����ID��
				status =PcdSelect(UID);//ѡ��
			}
		if(status==MI_OK)//�x���ɹ�
			{
				LCD_ShowString(50,60,210,24,16,(u8*)"PcdSelect_MI_OK");
				status=MI_ERR;
				status =PcdAuthState(0x60,0x09,KEY,UID);////��C
			}
		if(status==MI_OK)//��C�ɹ�
			 {
				LCD_ShowString(50,80,210,24,16,(u8*)"PcdAuthState_MI_OK");
				status=MI_ERR;
				status=PcdWrite(0x09,data);//д��
				 //status=PcdValue(0xC1,0x09,SendData);
				status=PcdRead(0x09,RFID);//�x��
			 }
		if(status==MI_OK)//�x���ɹ�
			 {
				 LCD_ShowString(50,100,210,24,16,(u8*)"READ_MI_OK");
				 status=MI_ERR;
				 ShowName(10,250, RFID);
				 printf("14564asdad}}asd\r\n");
//				 USART_ClearFlag(USART1,USART_FLAG_TC);                 //��ȡUSART_SR
//				 USART_SendData(USART1,RFID[t]);                         //�򴮿�1��������
//				 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET); //�ȴ����ͽ���
			 }

		delay_ms(200);
	} 
}
 
/*************************************
*�������ܣ���ʾ���Ŀ��ţ���ʮ��������ʾ
*������x��y ����
*		p ���ŵĵ�ַ
*		charcolor �ַ�����ɫ
*		bkcolor   ��������ɫ
***************************************/
void ShowID(u16 x,u16 y, u8 *p)	 //��ʾ���Ŀ��ţ���ʮ��������ʾ
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
*�������ܣ���ʾ���Ŀ��ţ���ʮ��������ʾ
*������x��y ����
*		p ���ŵĵ�ַ
***************************************/
void ShowName(u16 x,u16 y, u8 *p)	 //��ʾ��������
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


