#ifndef CNETWND_H_GAMEPLATFROM
#define CNETWND_H_GAMEPLATFROM

#include "CWnd.h"


class CNetWnd :public CWnd
{
public:
	HWND hNet;

	//构造函数
	CNetWnd();
	//析构函数
	~CNetWnd();

	BOOL RegisterWnd(HINSTANCE hInst);
	BOOL CreateWnd(HWND hParentWnd=NULL,HMENU hMenu=NULL);
	LRESULT WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){return 0;};

private:

	VOID OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam){};
	VOID OnPaint(WPARAM wParam,LPARAM lParam){};
	BOOL OnEraseBkgnd(WPARAM wParam,LPARAM lParam){return FALSE;};	
	VOID OnSize(WPARAM wParam,LPARAM lParam){};
	VOID OnNotify(WPARAM wParam,LPARAM lParam){};
	VOID OnCommand(WPARAM wParam,LPARAM lParam){};
	VOID OnMenuSelect(WPARAM wParam,LPARAM lParam){};
	VOID OnLButtonDown(WPARAM wParam,LPARAM lParam){};
	VOID OnLButtonDbClick(WPARAM wParam,LPARAM lParam){};
	VOID OnRButtonDown(WPARAM wParam,LPARAM lParam){};
	VOID OnMouseMove(WPARAM wParam,LPARAM lParam){};
	VOID OnKeyDown(WPARAM wParam,LPARAM lParam){};
	VOID OnTimer(WPARAM wParam,LPARAM lParam){};
	VOID OnClose(WPARAM wParam,LPARAM lParam){};
	VOID OnDestroy(WPARAM wParam,LPARAM lParam);
};

extern CNetWnd *NetWnd;//网络服务窗口指针

LRESULT CALLBACK NetWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);


#endif