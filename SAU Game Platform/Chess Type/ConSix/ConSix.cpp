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
	strcpy(Info,"SAU Game Platform chessType");
	strcpy(ChessName,"Áù×ÓÆå");
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
}

DLLFUCTION VOID OnSize(RECT rtBoard)
{
	ConSix.SetBoard(rtBoard);
}

DLLFUCTION VOID DrawBoard(HDC hDC)
{
	ConSix.DrawBoard(hDC);
}

DLLFUCTION INT OnLButtonDown(int x, int y)
{
	return ConSix.OnLButtonDown(x,y);
}

DLLFUCTION INT OkMove(char *denCmd)
{
	INT k;
	k=ConSix.OkMove();
	strcpy(denCmd, ConSix.denCmd);
	return k;
}

DLLFUCTION VOID CancelMove()
{
	ConSix.CancelMove();
}

DLLFUCTION INT ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{
	INT k = ConSix.ProcessMove(moveCmd);
	strcpy(curCmd, ConSix.curCmd);
	strcpy(denCmd, ConSix.denCmd);
	return k;
}

DLLFUCTION VOID OnRun()
{
	ConSix.InitGame();
}

DLLFUCTION INT GetCurPlayer()
{
	return ConSix.player;
}
