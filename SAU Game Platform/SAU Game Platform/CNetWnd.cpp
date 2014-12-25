#include "CNetWnd.h"


CNetWnd *NetWnd;

CNetWnd::CNetWnd()
{	
	ClsName="NETWND";
	WndName="Net";	
}

CNetWnd::~CNetWnd()
{

}

//注册窗口类
BOOL CNetWnd::RegisterWnd(HINSTANCE hInst)
{		
	WNDCLASS wc;//窗口类

	wc.hInstance=hInst;
	wc.lpszClassName=ClsName;
	wc.lpfnWndProc=(WNDPROC)NetWndProc;
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
BOOL CNetWnd::CreateWnd(HWND hParentWnd,HMENU hMenu)
{
	DWORD WndStyle;//窗口风格

	WndStyle=WS_VISIBLE |
		WS_OVERLAPPEDWINDOW |
		WS_CLIPCHILDREN |//Excludes the area occupied by child windows when drawing occurs within the parent window.
		WS_CLIPSIBLINGS; //Excludes the area occupied by child windows when drawing occurs within another child window.

	hWnd=CreateWindow(
		ClsName,//Class name
		WndName,//Window name		
		WndStyle,//WndStyle
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
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
	ShowWindow(hWnd,SW_HIDE);
	UpdateWindow(hWnd);
	
	return TRUE;
}

VOID CNetWnd::OnDestroy(WPARAM wParam,LPARAM lParam)
{
}