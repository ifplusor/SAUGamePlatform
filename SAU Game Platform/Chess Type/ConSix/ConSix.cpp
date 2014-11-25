#define CHESSTYPE

#include "ConSix.h"

HINSTANCE hInst;
CConSix ConSix;


//ÍË³öÄ£¿é
VOID ExitModule();

BOOL APIENTRY DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			hInst=hModule;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			ExitModule();
			break;
    }
    return TRUE;
}

DLLFUCTION VOID CheckModule(char *Info,char *ChessName,int *ChessType)
{
	strcpy(Info,"SAU_GamePlatform chessType");
	strcpy(ChessName,"_Áù×ÓÆå");
	*ChessType=0;
}

DLLFUCTION VOID InitModule(HWND hWnd,GameSet *gameset)
{
	ConSix.hWnd=hWnd;
	ConSix.gameset=gameset;
	ConSix.InitGame();
}

VOID ExitModule()
{
	if(ConSix.hPen!=NULL)
		DeleteObject(ConSix.hPen);
	if(ConSix.hFont!=NULL)
		DeleteObject(ConSix.hFont);
}

DLLFUCTION VOID OnSize(RECT rt)
{
	if(ConSix.hPen!=NULL)
		DeleteObject(ConSix.hPen);
	if(ConSix.hFont!=NULL)
		DeleteObject(ConSix.hFont);
	ConSix.rtBoard=rt;
	ConSix.side=rt.right-rt.left;
	ConSix.d=ConSix.side/20;
	ConSix.pixel=((double)ConSix.side)/600;
	ConSix.hPen=CreatePen(PS_SOLID,(int)(2*ConSix.pixel),RGB(0,0,0));
	ConSix.fWidth=(int)(ConSix.d/3);
	ConSix.fHeight=(int)(ConSix.d*2/3);
	ConSix.hFont=ConSix.CreateSimpleFont(ConSix.fWidth,ConSix.fHeight);	
}

DLLFUCTION VOID DrawBoard(HDC hDC)
{
	ConSix.DrawBoard(hDC);
}

DLLFUCTION BOOL OnLButtonDown(int x,int y,char *wDMsg)
{
	BOOL k;
	k=ConSix.OnLButtonDown(x,y);
	strcpy(wDMsg,ConSix.wDMsg);
	return k;
}

DLLFUCTION BOOL ProcessMsg(char *msg,char *wMMsg,char *wDMsg)
{
	BOOL k=ConSix.ProcessMsg(msg);
	strcpy(wMMsg,ConSix.wMMsg);
	strcpy(wDMsg,ConSix.wDMsg);
	return k;
}

DLLFUCTION VOID OnRun()
{
	ConSix.InitGame();
}

DLLFUCTION int GetCurPlayer()
{
	return ConSix.player;
}
