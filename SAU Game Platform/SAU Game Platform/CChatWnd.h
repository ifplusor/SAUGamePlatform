#ifndef CHATWND_H_GAMEPLATFROM
#define CHATWND_H_GAMEPLATFROM


#include "CWnd.h"

class CChatWnd :public CWnd
{
public:	
	HWND hNet;

	int xGap,yGap;
	RECT rtNet;
	char MessageBuffer[1024*256];

	CChatWnd();
	~CChatWnd();

	BOOL RegisterWnd(HINSTANCE hInst);
	BOOL CreateWnd(HWND hParentWnd=NULL,HMENU hMenu=NULL);
	LRESULT WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){return 0;};
	VOID OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam){};
	VOID OnPaint(WPARAM wParam,LPARAM lParam);
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

	BOOL CreateCtrl(HWND hWnd);
	BOOL AdjustCtrlPos(HWND hWnd);
	VOID AppendMessage(char *Msg,int tag);
};

extern CChatWnd ChatWnd;

LRESULT CALLBACK ChatWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);


#endif