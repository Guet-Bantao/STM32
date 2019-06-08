#include "lateclipping.h"
//#include "GUI.h"
//#include "WM.h"
//#include "FRAMEWIN.h"
#include "BUTTON.h"
#include "EDIT.h"


extern GUI_BITMAP bmBMP;  //图标
static WM_HWIN _hBut1;
static WM_HWIN _hBut2;
static WM_HWIN _hEdit;
static int     _PaintCount2=10;



//背景窗口回调函数
static void _cbBkWin(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)//通知代码
	{
		case WM_PAINT://WM_PAINT除了重绘窗口外，不能执行其他操作，也不能更改窗口属性
			GUI_SetTextMode(GUI_TM_TRANS);//透明文本
			GUI_SetFont(&GUI_Font24_ASCII);
			GUI_SetBkColor(GUI_BLACK);
			//GUI_Clear();
			GUI_SetColor(0x0060FF);
			GUI_DispStringAt("Hello World",0,0);
			GUI_DispStringAt("This is a Paint",0,14);
			
			//GUI_DispDecAt(_PaintCount2,160,14,5);//gui_dispcfol//gui_dispceol
		  
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED)  //按钮被释放时
			{
				if(pMsg->hWinSrc == _hBut1)	//button1被释放
				{
					EDIT_SetText(_hEdit,"nih");	//EDIT_AddKey(_hEdit,"5");
					//_PaintCount2++;
					WM_InvalidateWindow(pMsg->hWin);//背景窗口失效
				}
				else if(pMsg->hWinSrc == _hBut2)	//button2被释放
				{
					//_PaintCount2 = 0;
					GUI_Clear();
					EDIT_SetText(_hEdit,"");
					GUI_DrawBitmap(&bmBMP,00,0);  //绘制图标
					WM_InvalidateWindow(pMsg->hWin); //主窗口失效
				}
			}
			break;
			default:
				WM_DefaultProc(pMsg);	
	}
}


void _ShowButtun(void)
{
	WM_SetCallback(WM_HBKWIN,_cbBkWin);	//WM_HBKWIN	句柄//_cbBkWin背景窗口回调函数
	_hBut1 = BUTTON_CreateEx(10,70,80,50,0,WM_CF_SHOW,0,1);	//直接创建按钮1
	_hBut2 = BUTTON_CreateEx(10,130,80,50,0,WM_CF_SHOW,0,2);	//创建按钮2
	_hEdit=EDIT_CreateEx(160,14,80,30,0,WM_CF_SHOW,5,5,10);
	EDIT_EnableBlink(_hEdit,200,1);
	BUTTON_SetFont(_hBut1,&GUI_Font16_ASCII);
	BUTTON_SetFont(_hBut2,&GUI_Font16_ASCII);
	BUTTON_SetText(_hBut1,"UP Paint");	//设置按钮的名字
	BUTTON_SetText(_hBut2,"Reset Paint");
	//GUI_SetTextMode(GUI_TM_TRANS);	//透明显示
	GUI_DrawBitmap(&bmBMP,00,0);  //绘制图标	
	while(1)//卡住
	{
		GUI_Delay(50);
	}
}
