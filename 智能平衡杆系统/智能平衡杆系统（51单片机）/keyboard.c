/*==================硬件电路===============================      
 *说明：对4*4矩阵键盘扫描，扫描方法为状态机方法+定时器中断
 *			数据口为P0，列线为P0.0-P0.3，行线为P0.4-P0.7
 *==========================================================*/

#include "keyboard.h"

/*==========================================================   
 *函数名称：Obtain_key(void)
 *功    能：对矩阵键盘进行扫描，扫描方式为状态机+定时器中断
 *入口参数：
 *出口参数：key_temp
 *说    明：获取某时刻的键值
 *==========================================================*/
static unsigned char Obtain_key(void)
{
	unsigned char key1;
	unsigned char key2;
	unsigned char key_temp;//暂时存放按键键值
	KEY=0xf0;
	key1=KEY;
 	key1=KEY&0xf0;  		//确定哪一列的按键按下
 	KEY=0x0f;
	key2=KEY;
 	key2=KEY&0x0f;  		//确定哪一行的按键按下
	key_temp=key1|key2;  	//确定按键位置
	return key_temp;
}

/*==========================================================   
 *函数名称：Scan_key(void)
 *功    能：获取真正的键值
 *入口参数：
 *出口参数：key_return
 *说    明：按键释放才能获取有效键值
 *==========================================================*/
unsigned char Scan_key(void)
{
	static unsigned char key_state =0; //按键状态
	static unsigned char key_prev  =0; //上一次按键
	unsigned char key_press =NO_KEY;	 //按键真实键值
	unsigned char key_return =NO_KEY;
	
	key_press =Obtain_key();    //获取键值
	switch(key_state)
	{
		case 0:                    //状态0 按键初始状态，此时无按键按下
			if(key_press!=NO_KEY)    //有按键按下
			{
				key_state=1;           //转换为状态1 确认按键是否真的按下
				key_prev=key_press;    //保存按键状态				
			}
			break;
		case 1:                    //按键确认状态
			if(key_press==key_prev)  //确认和上次按键相同
			{
				key_state=2;           //转换为状态2 判断按键释放
			}
			else                     //按键弹起，是抖动，不响应
			{
				key_state=0;					 //转换为状态0
			}
			break;
		case 2:                    //按键释放状态
			if(key_press==NO_KEY)   //按键释放了
			{
				key_state=0;					//转换为状态0
				key_return =key_prev;
				break;
			}
		default :
			break;
	}
	return key_return;
}

/*==========================================================   
 *函数名称：Get_key(void)
 *功    能：对真正的键值进行处理
 *入口参数：
 *出口参数：key_value
 *说    明：
 *==========================================================*/
unsigned char Get_key(void)
{
	unsigned char key_acc;
	unsigned char	key_value;
	key_acc=Scan_key();
	switch(key_acc)           //当确定按键按下后，列举所有的按键情况
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
						 case 0xFF: key_value=255;break;//测试用 实际可删除
						 default :break;
		    	}
	return key_value;//返回键值
}
