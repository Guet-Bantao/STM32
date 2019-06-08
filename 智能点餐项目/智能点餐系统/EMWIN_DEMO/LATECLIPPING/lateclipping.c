#include "lateclipping.h"
//#include "GUI.h"
//#include "WM.h"
//#include "FRAMEWIN.h"
#include "BUTTON.h"
#include "EDIT.h"


extern GUI_BITMAP bmBMP;  //ͼ��
static WM_HWIN _hBut1;
static WM_HWIN _hBut2;
static WM_HWIN _hEdit;
static int     _PaintCount2=10;



//�������ڻص�����
static void _cbBkWin(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)//֪ͨ����
	{
		case WM_PAINT://WM_PAINT�����ػ洰���⣬����ִ������������Ҳ���ܸ��Ĵ�������
			GUI_SetTextMode(GUI_TM_TRANS);//͸���ı�
			GUI_SetFont(&GUI_Font24_ASCII);
			GUI_SetBkColor(GUI_BLACK);
			//GUI_Clear();
			GUI_SetColor(0x0060FF);
			GUI_DispStringAt("Hello World",0,0);
			GUI_DispStringAt("This is a Paint",0,14);
			
			//GUI_DispDecAt(_PaintCount2,160,14,5);//gui_dispcfol//gui_dispceol
		  
			break;
		case WM_NOTIFY_PARENT:
			if(pMsg->Data.v == WM_NOTIFICATION_RELEASED)  //��ť���ͷ�ʱ
			{
				if(pMsg->hWinSrc == _hBut1)	//button1���ͷ�
				{
					EDIT_SetText(_hEdit,"nih");	//EDIT_AddKey(_hEdit,"5");
					//_PaintCount2++;
					WM_InvalidateWindow(pMsg->hWin);//��������ʧЧ
				}
				else if(pMsg->hWinSrc == _hBut2)	//button2���ͷ�
				{
					//_PaintCount2 = 0;
					GUI_Clear();
					EDIT_SetText(_hEdit,"");
					GUI_DrawBitmap(&bmBMP,00,0);  //����ͼ��
					WM_InvalidateWindow(pMsg->hWin); //������ʧЧ
				}
			}
			break;
			default:
				WM_DefaultProc(pMsg);	
	}
}


void _ShowButtun(void)
{
	WM_SetCallback(WM_HBKWIN,_cbBkWin);	//WM_HBKWIN	���//_cbBkWin�������ڻص�����
	_hBut1 = BUTTON_CreateEx(10,70,80,50,0,WM_CF_SHOW,0,1);	//ֱ�Ӵ�����ť1
	_hBut2 = BUTTON_CreateEx(10,130,80,50,0,WM_CF_SHOW,0,2);	//������ť2
	_hEdit=EDIT_CreateEx(160,14,80,30,0,WM_CF_SHOW,5,5,10);
	EDIT_EnableBlink(_hEdit,200,1);
	BUTTON_SetFont(_hBut1,&GUI_Font16_ASCII);
	BUTTON_SetFont(_hBut2,&GUI_Font16_ASCII);
	BUTTON_SetText(_hBut1,"UP Paint");	//���ð�ť������
	BUTTON_SetText(_hBut2,"Reset Paint");
	//GUI_SetTextMode(GUI_TM_TRANS);	//͸����ʾ
	GUI_DrawBitmap(&bmBMP,00,0);  //����ͼ��	
	while(1)//��ס
	{
		GUI_Delay(50);
	}
}
