/*==================Ӳ����·===============================      
 *˵������4*4�������ɨ�裬ɨ�跽��Ϊ״̬������+��ʱ���ж�
 *			���ݿ�ΪP0������ΪP0.0-P0.3������ΪP0.4-P0.7
 *==========================================================*/

#include "keyboard.h"

/*==========================================================   
 *�������ƣ�Obtain_key(void)
 *��    �ܣ��Ծ�����̽���ɨ�裬ɨ�跽ʽΪ״̬��+��ʱ���ж�
 *��ڲ�����
 *���ڲ�����key_temp
 *˵    ������ȡĳʱ�̵ļ�ֵ
 *==========================================================*/
static unsigned char Obtain_key(void)
{
	unsigned char key1;
	unsigned char key2;
	unsigned char key_temp;//��ʱ��Ű�����ֵ
	KEY=0xf0;
	key1=KEY;
 	key1=KEY&0xf0;  		//ȷ����һ�еİ�������
 	KEY=0x0f;
	key2=KEY;
 	key2=KEY&0x0f;  		//ȷ����һ�еİ�������
	key_temp=key1|key2;  	//ȷ������λ��
	return key_temp;
}

/*==========================================================   
 *�������ƣ�Scan_key(void)
 *��    �ܣ���ȡ�����ļ�ֵ
 *��ڲ�����
 *���ڲ�����key_return
 *˵    ���������ͷŲ��ܻ�ȡ��Ч��ֵ
 *==========================================================*/
unsigned char Scan_key(void)
{
	static unsigned char key_state =0; //����״̬
	static unsigned char key_prev  =0; //��һ�ΰ���
	unsigned char key_press =NO_KEY;	 //������ʵ��ֵ
	unsigned char key_return =NO_KEY;
	
	key_press =Obtain_key();    //��ȡ��ֵ
	switch(key_state)
	{
		case 0:                    //״̬0 ������ʼ״̬����ʱ�ް�������
			if(key_press!=NO_KEY)    //�а�������
			{
				key_state=1;           //ת��Ϊ״̬1 ȷ�ϰ����Ƿ���İ���
				key_prev=key_press;    //���水��״̬				
			}
			break;
		case 1:                    //����ȷ��״̬
			if(key_press==key_prev)  //ȷ�Ϻ��ϴΰ�����ͬ
			{
				key_state=2;           //ת��Ϊ״̬2 �жϰ����ͷ�
			}
			else                     //���������Ƕ���������Ӧ
			{
				key_state=0;					 //ת��Ϊ״̬0
			}
			break;
		case 2:                    //�����ͷ�״̬
			if(key_press==NO_KEY)   //�����ͷ���
			{
				key_state=0;					//ת��Ϊ״̬0
				key_return =key_prev;
				break;
			}
		default :
			break;
	}
	return key_return;
}

/*==========================================================   
 *�������ƣ�Get_key(void)
 *��    �ܣ��������ļ�ֵ���д���
 *��ڲ�����
 *���ڲ�����key_value
 *˵    ����
 *==========================================================*/
unsigned char Get_key(void)
{
	unsigned char key_acc;
	unsigned char	key_value;
	key_acc=Scan_key();
	switch(key_acc)           //��ȷ���������º��о����еİ������
	    		{
				     case 0xe7: key_value=0;break;
				     case 0xeb: key_value=1;break;
				     case 0xed: key_value=2;break;
				     case 0xee: key_value=3;break;
				     case 0xd7: key_value=4;break;
				     case 0xdb: key_value=5;break;
				     case 0xdd: key_value=6;break;
				     case 0xde: key_value=7;break;
				     case 0xb7: key_value=8;break;
				     case 0xbb: key_value=9;break;
				     case 0xbd: key_value=10;break;
				     case 0xbe: key_value=11;break;
				     case 0x77: key_value=12;break;
				     case 0x7b: key_value=13;break;
				     case 0x7d: key_value=14;break;
				     case 0x7e: key_value=15;break;
						 case 0xFF: key_value=255;break;//������ ʵ�ʿ�ɾ��
						 default :break;
		    	}
	return key_value;//���ؼ�ֵ
}
