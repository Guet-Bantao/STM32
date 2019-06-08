#include "TestAPP.h"
#include "DIALOG.h"
#include "FRAMEWIN.h"
#include "includes.h"
#include "ff.h"


char menu='a';//菜单第一张为a
char check[16];
float	price=0;

extern  OS_TCB RC522TaskTCB;
extern u8 Name[4];
extern u8 Price[4];

static u8 i=0;//点菜数
static u8 Price_menu[12];//总价

WM_HWIN hItem7;
WM_HWIN hItem9;

//MenuAPP内部小控件ID
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x02)
#define ID_TEXT_0       (GUI_ID_USER + 0x03)
#define ID_BUTTON_0 	(GUI_ID_USER + 0x04)
#define ID_BUTTON_1 	(GUI_ID_USER + 0x05)
#define ID_BUTTON_2 	(GUI_ID_USER + 0x06)

//CheckAPP内部小控件ID
#define ID_FRAMEWIN_1   (GUI_ID_USER + 0x07)
#define ID_TEXT_1       (GUI_ID_USER + 0x08)
#define ID_LISTVIEW_0 	(GUI_ID_USER + 0x09)
#define ID_TEXT_10 		(GUI_ID_USER + 0x13)


//PaymentAPP内部小控件ID
#define ID_FRAMEWIN_2   (GUI_ID_USER + 0x0b)
#define ID_TEXT_2       (GUI_ID_USER + 0x0c)
#define ID_TEXT_4       (GUI_ID_USER + 0x0d)
#define ID_TEXT_5       (GUI_ID_USER + 0x0e)
#define ID_TEXT_6       (GUI_ID_USER + 0x0f)
#define ID_TEXT_7       (GUI_ID_USER + 0x10)
#define ID_TEXT_8       (GUI_ID_USER + 0x11)
#define ID_TEXT_9       (GUI_ID_USER + 0x12)

//BrowerAPP内部小控件ID
#define ID_FRAMEWIN_3   (GUI_ID_USER + 0x14)
#define ID_TEXT_3       (GUI_ID_USER + 0x15)

WM_HWIN MenuAPPWin=NULL;         //BagAPP窗口
WM_HWIN CheckAPPWin=NULL;        //BookAPP窗口
WM_HWIN PaymentAPPWin=NULL;      //BookshAPP窗口
WM_HWIN BrowerAPPWin=NULL;      //BrowerAPP窗口



//MenuAPP资源表
static const GUI_WIDGET_CREATE_INFO MenuAPPDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 480,0,0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 10, 220, 300, 20, 0, 0x64, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 25, 280, 80, 50, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 175, 280, 80, 50, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 100, 360, 80, 50, 0, 0x0, 0 },
};

//CheckAPP资源表
static const GUI_WIDGET_CREATE_INFO CheckAPPDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_1, 0, 0, 320, 480,FRAMEWIN_CF_MOVEABLE,0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 10,350, 300, 20, 0, 0x64, 0 },
	{ LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 17, 22, 257, 320, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_10, 80,380, 150, 20, 0, 0x64, 0 },
};

//PaymentAPP资源表
static const GUI_WIDGET_CREATE_INFO PaymentAPPDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_2, 0, 0, 320, 480,FRAMEWIN_CF_MOVEABLE,0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_2,0,50, 320, 20, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_4,30,150, 150, 20, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_5,30,180, 150, 20, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_6,30,210, 150, 20, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_7,150,150, 80, 20, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_8,150,180, 150, 20, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_9,150,210, 80, 20, 0, 0x64, 0 },
};

//BrowerAPP资源表
static const GUI_WIDGET_CREATE_INFO BrowerAPPDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_3, 0, 0, 320, 480,0,0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_3,50, 200, 200, 20, 0, 0x64, 0 },
};




//MenuAPP的回调函数
static void _cbMenuAPPDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
	int     NCode;
	int     Id;
    switch (pMsg->MsgId) 
    {
		case WM_PAINT:
       // GUI_Clear();
		//初始化TEXT
		hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_0);
		TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
		TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
		switch(menu)
		{
			case 'a':
				dispbmpex("0:/PICTURE/yumi.bmp",0,30,28,1,1);
				TEXT_SetText(hItem, "This is corn, sweets, 9.5 RMB");
			break;
			case 'b':
				dispbmpex("0:/PICTURE/cabbage.bmp",0,30,28,1,1);
				TEXT_SetText(hItem, "This is Cabbage, 5.5 RMB");
			break;
			case 'c':
				dispbmpex("0:/PICTURE/meat.bmp",0,30,28,1,1);
				TEXT_SetText(hItem, "This is Meat, 18.5 RMB");
			break;
			case 'd':
				dispbmpex("0:/PICTURE/chicken.bmp",0,30,28,1,1);
				TEXT_SetText(hItem, "This is Chicken, 26.5 RMB");
			break;
			case 'e':
				dispbmpex("0:/PICTURE/rice.bmp",0,30,28,1,1);
				TEXT_SetText(hItem, "This is Rice, 2 RMB");
			break;
		}			
            break;
        case WM_INIT_DIALOG://创建对话框后立即发送到对话窗口
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);// Initialization of 'Button'
			BUTTON_SetText(hItem, "Previous");
			BUTTON_SetFont(hItem, GUI_FONT_16_ASCII);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
			BUTTON_SetText(hItem, "Nxet");
			BUTTON_SetFont(hItem, GUI_FONT_16_ASCII);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
			BUTTON_SetText(hItem, "Select");
			BUTTON_SetFont(hItem, GUI_FONT_16_ASCII);
            
            //初始化FRAMWIN
            hItem=pMsg->hWin;
            FRAMEWIN_SetText(hItem, "MENU APP");
            FRAMEWIN_SetFont(hItem,GUI_FONT_20_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER);
            FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //添加关闭按钮
            FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
            FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮
        
		
            break;
        case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_BUTTON_0: //BUTTON_0的通知代码
					switch(NCode) 
					{
//						case WM_NOTIFICATION_CLICKED:
//							break;
						case WM_NOTIFICATION_RELEASED: //按钮被按下并释放
							menu--;
							WM_InvalidateWindow(MenuAPPWin);//窗口无效化
							break;	
					}
					break;
				case ID_BUTTON_1: //BUTTON_1的通知代码
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							menu++;
							WM_InvalidateWindow(MenuAPPWin);//窗口无效化
							break;
					}
					break;
				case ID_BUTTON_2: //BUTTON_2的通知代码
				switch(NCode) 
				{
					case WM_NOTIFICATION_RELEASED:
						check[i]=menu;
						i++;
						//WM_InvalidateWindow(MenuAPPWin);//窗口无效化
						break;
				}
				break;
			}
			//NCode=pMsg->Data.v;
            if(NCode==WM_NOTIFICATION_CHILD_DELETED) 
			{
				menu='a';
				MenuAPPWin=0;//子窗口删除了
			}
			break;
        default:
            WM_DefaultProc(pMsg);
  }
}

//CheckAPP的回调函数
static void _cbCheckAPPDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
	int     NCode;
	u8		j;
	//float	price_temp=0;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_RED);
            GUI_Clear();
            break;
        case WM_INIT_DIALOG:
            //初始化FRAMWIN
            hItem=pMsg->hWin;
            FRAMEWIN_SetText(hItem, "Check APP");
            FRAMEWIN_SetFont(hItem,GUI_FONT_20_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER);
            FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //添加关闭按钮
            FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
            FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮
		
			// Initialization of 'Listview'
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
			LISTVIEW_AddColumn(hItem, 75, "Name", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(hItem, 75, "Price", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(hItem, 75, "Numbe", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_SetGridVis(hItem, 1);
			LISTVIEW_SetRowHeight(hItem, 20);
			LISTVIEW_SetFont(hItem, GUI_FONT_20_ASCII);
			price=0.0;
			for(j=0;j<sizeof(check);j++)
			{
				switch(check[j])
				{
					case 'a':
						LISTVIEW_AddRow(hItem, NULL);
						LISTVIEW_SetItemText(hItem, 0, j, "Maize");
						LISTVIEW_SetItemText(hItem, 2, j, "1");
						LISTVIEW_SetItemText(hItem, 1, j, "9.8");
						price+=9.8;break;
					case 'b':
						LISTVIEW_AddRow(hItem, NULL);
						LISTVIEW_SetItemText(hItem, 0, j, "Cabbage");
						LISTVIEW_SetItemText(hItem, 2, j, "1");
						LISTVIEW_SetItemText(hItem, 1, j, "5.5");
						price+=5.5;break;
					case 'c':
						LISTVIEW_AddRow(hItem, NULL);
						LISTVIEW_SetItemText(hItem, 0, j, "Meat");
						LISTVIEW_SetItemText(hItem, 2, j, "1");
						LISTVIEW_SetItemText(hItem, 1, j, "18.5");
						price+=18.5;break;
					case 'd':
						LISTVIEW_AddRow(hItem, NULL);
						LISTVIEW_SetItemText(hItem, 0, j, "Chicken");
						LISTVIEW_SetItemText(hItem, 2, j, "1");
						LISTVIEW_SetItemText(hItem, 1, j, "26.5");
						price+=26.5;break;
					case 'e':
						LISTVIEW_AddRow(hItem, NULL);
						LISTVIEW_SetItemText(hItem, 0, j, "Rice");
						LISTVIEW_SetItemText(hItem, 2, j, "1");
						LISTVIEW_SetItemText(hItem, 1, j, "2.0");
						price+=2;break;
						
				}
			}
			sprintf((char *)Price_menu,"%.2f",price);
            //初始化TEXT1
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_1);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem, "These dishes in total cost:");
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
			//初始化TEXT10
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_10);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem, (char *)Price_menu);
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
		
            break;
        case WM_NOTIFY_PARENT:
			NCode=pMsg->Data.v;
            if(NCode==WM_NOTIFICATION_CHILD_DELETED) CheckAPPWin=0;//子窗口删除了
			break;
        default:
            WM_DefaultProc(pMsg);
  }
}

//PaymentAPP的回调函数
static void _cbPaymentAPPDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
	int     NCode;
	OS_ERR err;
	u8 i;

    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_BLUE);
            GUI_Clear();
			//初始化TEXT7
            hItem7=WM_GetDialogItem(pMsg->hWin,ID_TEXT_7);
            TEXT_SetFont(hItem7, GUI_FONT_24_ASCII);
			TEXT_SetText(hItem7, (char *)Name);
            TEXT_SetTextAlign(hItem7,GUI_TA_HCENTER|GUI_TA_VCENTER);
			//WM_SetCallback
			//初始化TEXT9
            hItem9=WM_GetDialogItem(pMsg->hWin,ID_TEXT_9);
            TEXT_SetFont(hItem9, GUI_FONT_24_ASCII);
			TEXT_SetText(hItem9, (char *)Price);
            TEXT_SetTextAlign(hItem9,GUI_TA_HCENTER|GUI_TA_VCENTER);
		
			OSTaskResume((OS_TCB*)&RC522TaskTCB,&err);	//恢复任务
			OSTaskResume((OS_TCB*)&RC522TaskTCB,&err);	//恢复任务
			OSTaskResume((OS_TCB*)&RC522TaskTCB,&err);	//恢复任务
			OSTaskResume((OS_TCB*)&RC522TaskTCB,&err);	//恢复任务
			OSTaskResume((OS_TCB*)&RC522TaskTCB,&err);	//恢复任务
			OSTaskResume((OS_TCB*)&RC522TaskTCB,&err);	//恢复任务
			OSTaskResume((OS_TCB*)&RC522TaskTCB,&err);	//恢复任务
			OSTaskResume((OS_TCB*)&RC522TaskTCB,&err);	//恢复任务
            break;   
        case WM_INIT_DIALOG:
            //初始化FRAMWIN
            hItem=pMsg->hWin;
            FRAMEWIN_SetText(hItem, "Payment APP");
            FRAMEWIN_SetFont(hItem,GUI_FONT_20_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER);
            FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //添加关闭按钮
            FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
            FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮
		
        
            //初始化TEXT
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_2);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem, "Please put the card on the RC522");
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
			//初始化TEXT4
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_4);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			TEXT_SetText(hItem, "Name :");
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
			//初始化TEXT5
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_5);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			TEXT_SetText(hItem, "price :");
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
			//初始化TEXT6
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_6);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			TEXT_SetText(hItem, "Your RMB :");
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
			//初始化TEXT8
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_8);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			TEXT_SetText(hItem, (char *)Price_menu);
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
			
            break;
        case WM_NOTIFY_PARENT:
			NCode=pMsg->Data.v;
            if(NCode==WM_NOTIFICATION_CHILD_DELETED)
			{
				OS_TaskSuspend((OS_TCB*)&RC522TaskTCB,&err);	//挂起任务//这条指令会被执行多次
				for(i=0;i<4;i++)
					{
						Price[i]=' ';
						Name[i]=' ';//memset(RC522_Rbuff,0,16);   //清空数组
					}
				PaymentAPPWin=0;//子窗口删除了
			}
			break;
        default:
            WM_DefaultProc(pMsg);
  }
}

//CleaAPP的回调函数
static void _cbBrowerAPPDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
	int     NCode;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_YELLOW);
            GUI_Clear();
            break;  
        case WM_INIT_DIALOG:
            //初始化FRAMWIN
            hItem=pMsg->hWin;
            FRAMEWIN_SetText(hItem, "Clear APP");
            FRAMEWIN_SetFont(hItem,GUI_FONT_20_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER);
            FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,2); //添加关闭按钮
            FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
            FRAMEWIN_AddMinButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最小化按钮
        
            //初始化TEXT
            hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_3);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem, "Data clear success");
            TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
		
			for(i=0;i<16;i++)
			{
				check[i]=' ';
			}
			i=0;
            break;
        case WM_NOTIFY_PARENT:
			NCode=pMsg->Data.v;
            if(NCode==WM_NOTIFICATION_CHILD_DELETED) BrowerAPPWin=0;//子窗口删除了
			break;
        default:
            WM_DefaultProc(pMsg);
  }
}

//创建MenuAPP
void CreateMenuAPP(void) 
{
    MenuAPPWin=GUI_CreateDialogBox(MenuAPPDialogCreate,GUI_COUNTOF(MenuAPPDialogCreate),_cbMenuAPPDialog,WM_HBKWIN,0,0);
}

//创建CheckAPP
void CreateCheckAPP(void) 
{
    CheckAPPWin=GUI_CreateDialogBox(CheckAPPDialogCreate,GUI_COUNTOF(CheckAPPDialogCreate),_cbCheckAPPDialog,WM_HBKWIN,0,0);
}

//创建BookshAPP
void CreatePaymentAPP(void) 
{
    PaymentAPPWin=GUI_CreateDialogBox(PaymentAPPDialogCreate,GUI_COUNTOF(PaymentAPPDialogCreate),_cbPaymentAPPDialog,WM_HBKWIN,0,0);
}

//创建BrowerAPP
void CreateBrowerAPP(void) 
{
    BrowerAPPWin=GUI_CreateDialogBox(BrowerAPPDialogCreate,GUI_COUNTOF(BrowerAPPDialogCreate),_cbBrowerAPPDialog,WM_HBKWIN,0,0);
}


