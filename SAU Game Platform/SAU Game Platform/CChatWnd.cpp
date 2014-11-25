#include "CChatWnd.h"
#include "CMainWnd.h"
#include "Control.h"

extern int GameMode_2;

CChatWnd ChatWnd;

CChatWnd::CChatWnd()
{	
	ClsName="CHATWND";
	WndName="Chat";	
}

CChatWnd::~CChatWnd()
{

}

//注册窗口类
BOOL CChatWnd::RegisterWnd(HINSTANCE hInst)
{		
	WNDCLASS wc;//窗口类

	wc.hInstance=hInst;
	wc.lpszClassName=ClsName;
	wc.lpfnWndProc=(WNDPROC)ChatWndProc;
	wc.hIcon=NULL;
	wc.lpszMenuName=NULL;
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);	
	wc.style=CS_HREDRAW |CS_VREDRAW |CS_DBLCLKS;
	wc.cbWndExtra=0;
	wc.cbClsExtra=0;
	if(RegisterClass(&wc)==NULL)
	{
		ErrorBox("RegisterSetWindowClass failed");
		return FALSE;
	}
	return TRUE;
}

//创建窗口
BOOL CChatWnd::CreateWnd(HWND hParentWnd,HMENU hMenu)
{
	DWORD WndStyle;//窗口风格
	RECT rtWindow;
	GetWindowRect(MainWnd->hWnd,&rtWindow);
	rtWindow.left=rtWindow.right+1;
	rtWindow.right=rtWindow.right+(rtWindow.bottom-rtWindow.top)*0.45;

	WndStyle=WS_VISIBLE |
		WS_POPUP |
		WS_OVERLAPPED |	
		WS_CAPTION |
		WS_SIZEBOX |
		WS_SYSMENU |
		WS_MINIMIZEBOX |
		WS_CLIPCHILDREN |//Excludes the area occupied by child windows when drawing occurs within the parent window.
		WS_CLIPSIBLINGS; //Excludes the area occupied by child windows when drawing occurs within another child window.

	hWnd=CreateWindow(
		ClsName,//Class name
		WndName,//Window name		
		WndStyle,//WndStyle
		rtWindow.left,rtWindow.top,
		rtWindow.right-rtWindow.left,rtWindow.bottom-rtWindow.top,
		hParentWnd,//Have parent window		
		hMenu,//No menu
		hInst,//Application instance
		NULL//Do not create MDI client window
		);
	if(hWnd==NULL)
	{		
		ErrorBox("Create Chat window failed");		
		return FALSE;
	}
	HMENU hMenuSys=GetSystemMenu(hWnd,NULL);
	EnableMenuItem(hMenuSys,SC_CLOSE,MF_GRAYED);//使关闭按钮无效
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);
	ZeroMemory(MessageBuffer,sizeof(MessageBuffer));
	return TRUE;
}

BOOL CChatWnd::CreateCtrl(HWND hWnd)
{
	HWND hSR,hDisconnect,hBlack,hWhite,hSend,hEdit,hShow;

	if(GameMode_2==1)
	{
		hSR=CreateWindow("BUTTON","Ready",
			WS_VISIBLE |WS_CHILD,
			CW_USEDEFAULT,0,//Default position
			CW_USEDEFAULT,0,//Default size
			hWnd,
			(HMENU)IDB_CHAT_RS,
			hInst,NULL);
		if(hSR==NULL)
		{
			ErrorBox("Create 'Start/Ready' button failed");
			return false;
		}
	}
	else
	{
		hSR=CreateWindow("BUTTON","Start",
			WS_VISIBLE |WS_CHILD,
			CW_USEDEFAULT,0,//Default position
			CW_USEDEFAULT,0,//Default size
			hWnd,
			(HMENU)IDB_CHAT_RS,
			hInst,NULL);
		if(hSR==NULL)
		{
			ErrorBox("Create 'Start/Ready' button failed");
			return false;
		}
		EnableWindow(hSR,FALSE);
	}
	ShowWindow(hSR,SW_SHOWNORMAL);
	UpdateWindow(hSR);

	hDisconnect=CreateWindow("BUTTON","Disconnect",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CHAT_DISCONNECT,
		hInst,NULL);
	if(hDisconnect==NULL)
	{
		ErrorBox("Create 'Disconnect' button failed");
		return false;
	}
	ShowWindow(hDisconnect,SW_SHOWNORMAL);
	UpdateWindow(hDisconnect);

	hBlack=CreateWindow("BUTTON","Apply Black",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CHAT_APPLY_BLC,
		hInst,NULL);
	if(hBlack==NULL)
	{
		ErrorBox("Create 'Apply Black' button failed");
		return false;
	}
	if(GameMode_2==0)
		EnableWindow(hBlack,FALSE);
	ShowWindow(hBlack,SW_SHOWNORMAL);
	UpdateWindow(hBlack);

	hWhite=CreateWindow("BUTTON","Apply White",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CHAT_APPLY_WHT,
		hInst,NULL);
	if(hWhite==NULL)
	{
		ErrorBox("Create 'Apply White' button failed");
		return false;
	}
	if(GameMode_2==1)
		EnableWindow(hWhite,FALSE);
	ShowWindow(hWhite,SW_SHOWNORMAL);
	UpdateWindow(hWhite);

	hSend=CreateWindow("BUTTON","Send",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CHAT_SEND,
		hInst,NULL);
	if(hSend==NULL)
	{
		ErrorBox("Create 'Send' button failed");
		return false;
	}
	ShowWindow(hSend,SW_SHOWNORMAL);
	UpdateWindow(hSend);

	hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",
		WS_VISIBLE |WS_CHILD|ES_MULTILINE|ES_AUTOVSCROLL|ES_LEFT,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDE_CHAT_SEND,
		hInst,NULL);
	if(hEdit==NULL)
	{
		ErrorBox("Create 'Send' edit failed");
		return false;
	}
	SendDlgItemMessage(hWnd,IDE_CHAT_SEND,EM_SETLIMITTEXT,1000,0);//设置发送编辑框最长文本数量
	ShowWindow(hEdit,SW_SHOWNORMAL);
	UpdateWindow(hEdit);

	hShow=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",
		WS_VISIBLE|WS_CHILD|WS_VSCROLL
		|ES_MULTILINE|ES_AUTOVSCROLL|ES_LEFT|ES_READONLY|ES_NOHIDESEL,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDE_CHAT_SHOW,
		hInst,NULL);
	if(hShow==NULL)
	{
		ErrorBox("Create 'Show' edit failed");
		return false;
	}
	ShowWindow(hShow,SW_SHOWNORMAL);
	UpdateWindow(hShow);

	return TRUE;
}

BOOL CChatWnd::AdjustCtrlPos(HWND hWnd)
{
	RECT rt;

	GetClientRect(hWnd,&rtClient);

	rt.left=rtClient.left;
	rt.right=rtClient.right;
	rt.top=rtClient.top+(rtClient.bottom-rtClient.top)*0.4;
	rt.bottom=rtClient.bottom-40;
	MoveWindow(GetDlgItem(hWnd,IDE_CHAT_SHOW),rt.left,rt.top,rt.right-rt.left,rt.bottom-rt.top,true);
	UpdateWindow(GetDlgItem(hWnd,IDE_CHAT_SHOW));

	rt.top=rt.bottom;
	rt.bottom=rtClient.bottom;
	rt.right=rt.right-70;
	MoveWindow(GetDlgItem(hWnd,IDE_CHAT_SEND),rt.left,rt.top,rt.right-rt.left,rt.bottom-rt.top,true);
	UpdateWindow(GetDlgItem(hWnd,IDE_CHAT_SEND));

	rt.left=rt.right;
	rt.right=rtClient.right;
	MoveWindow(GetDlgItem(hWnd,IDB_CHAT_SEND),rt.left,rt.top,rt.right-rt.left,rt.bottom-rt.top,true);
	UpdateWindow(GetDlgItem(hWnd,IDB_CHAT_SEND));

	xGap=(int)(rtClient.right*0.05);
	yGap=(int)(rtClient.bottom*0.05/3);

	rtNet.left=rtClient.left+xGap;rtNet.right=rtClient.right-xGap;
	rtNet.top=rtClient.top+yGap;rtNet.bottom=rtClient.top+(rtClient.bottom-rtClient.top)*0.4-yGap;

	SetWindowPos(
		GetDlgItem(hWnd,IDB_CHAT_RS),
		HWND_TOP,
		rtNet.left+xGap,
		rtNet.bottom-2*yGap-(int)((rtNet.bottom-rtNet.top-4*yGap)/2),
		(int)((rtNet.right-rtNet.left-3*xGap)/2),
		(int)((rtNet.bottom-rtNet.top-4*yGap)/4),
		SWP_SHOWWINDOW
		);


	SetWindowPos(
		GetDlgItem(hWnd,IDB_CHAT_DISCONNECT),
		HWND_TOP,
		rtNet.right-xGap-(int)((rtNet.right-rtNet.left-3*xGap)/2),
		rtNet.bottom-2*yGap-(int)((rtNet.bottom-rtNet.top-4*yGap)/2),
		(int)((rtNet.right-rtNet.left-3*xGap)/2),
		(int)((rtNet.bottom-rtNet.top-4*yGap)/4),
		SWP_SHOWWINDOW
		);

	SetWindowPos(
		GetDlgItem(hWnd,IDB_CHAT_APPLY_BLC),
		HWND_TOP,
		rtNet.left+xGap,
		rtNet.bottom-yGap-(int)((rtNet.bottom-rtNet.top-4*yGap)/4),
		(int)((rtNet.right-rtNet.left-3*xGap)/2),
		(int)((rtNet.bottom-rtNet.top-4*yGap)/4),
		SWP_SHOWWINDOW
		);

	SetWindowPos(
		GetDlgItem(hWnd,IDB_CHAT_APPLY_WHT),
		HWND_TOP,
		rtNet.right-xGap-(int)((rtNet.right-rtNet.left-3*xGap)/2),
		rtNet.bottom-yGap-(int)((rtNet.bottom-rtNet.top-4*yGap)/4),
		(int)((rtNet.right-rtNet.left-3*xGap)/2),
		(int)((rtNet.bottom-rtNet.top-4*yGap)/4),
		SWP_SHOWWINDOW
		);

	return TRUE;
}

VOID CChatWnd::OnPaint(WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;	
	HDC hDC;//窗口DC
	HDC hMemDC;//缓存窗口DC
	HBITMAP hBitmap,hOldBitmap;
	LOGBRUSH lb;//画刷描述符
	HBRUSH hBrush,hOldBrush;//画刷句柄

	hDC=BeginPaint(hWnd,&ps);

	GetClientRect(hWnd,&rtClient);
	hMemDC=CreateCompatibleDC(hDC);
	hBitmap=CreateCompatibleBitmap(hDC,rtClient.right-rtClient.left,rtClient.bottom-rtClient.top);
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);

 	lb.lbStyle=BS_SOLID;
	lb.lbColor=RGB(230,230,230);
	lb.lbHatch=NULL;

	hBrush=CreateBrushIndirect(&lb);
	hOldBrush=(HBRUSH)SelectObject(hMemDC,hBrush);
	FillRect(hMemDC,&rtClient,hBrush);//设置背景颜色
	SetBkMode(hMemDC,TRANSPARENT);//设置背景混合模式为透明

	SIZE edge;
	edge.cx=(int)(rtClient.right*0.05);
	edge.cy=(int)(rtClient.bottom*0.05/3);
	rtNet.left=rtClient.left;rtNet.right=rtClient.right;
	rtNet.top=rtClient.top;rtNet.bottom=rtClient.top+(rtClient.bottom-rtClient.top)*0.4;

	DrawStatic(hMemDC,&rtNet,"网络对战",lb.lbColor,RGB(150,150,150),edge);//绘制黑方静态框

	BitBlt(hDC,0,0,rtClient.right-rtClient.left,rtClient.bottom-rtClient.top,hMemDC,0,0,SRCCOPY);

	DeleteObject(hBrush);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	EndPaint(hWnd,&ps);
}
VOID CChatWnd::OnDestroy(WPARAM wParam,LPARAM lParam)
{
	if(game.IsStop()==FALSE)
		game.StopGame();
	hWnd=NULL;
}

VOID CChatWnd::AppendMessage(char *Msg,int tag)
{
	int i,j;
	int M_len;
	if(tag==0)
		strcat(MessageBuffer,"host:\r\n");
	else
		strcat(MessageBuffer,"guest:\r\n");
	strcat(MessageBuffer,Msg);
	strcat(MessageBuffer,"\r\n\r\n");
	M_len=strlen(MessageBuffer);
	while(M_len>=1024*255)
	{
		for(i=0;i<M_len;i++)
			if(MessageBuffer[i]=='\r'&&MessageBuffer[i+1]=='\n')
				break;
		i=i=2;
		for(j=i;j<=M_len;j++)
			MessageBuffer[j-i]=MessageBuffer[j];
		M_len-=i;
	}
	SetText(GetDlgItem(hWnd,IDE_CHAT_SHOW),MessageBuffer);
}