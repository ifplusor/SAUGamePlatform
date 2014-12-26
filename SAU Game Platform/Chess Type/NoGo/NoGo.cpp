#define CHESSTYPE

#include "NoGo.h"

HINSTANCE hInst;
CNoGo *NoGo;

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
			hInst=hModule;
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

DLLFUCTION VOID CheckModule(char *Info, char *ChessName, int *ChessType)
{
	strcpy(Info, "SAU Game Platform chessType");
	strcpy(ChessName, "²»Î§Æå");
	*ChessType = 0;
}

DLLFUCTION VOID InitModule(HWND hWnd, char *LibPath)
{
	NoGo = new CNoGo(hInst, hWnd, LibPath);
	NoGo->InitGame();
}

DLLFUCTION VOID ExitModule()
{
	delete NoGo;
}

DLLFUCTION VOID OnSize(RECT rtBoard)
{
	NoGo->SetBoard(rtBoard);
}

DLLFUCTION VOID DrawBoard(HDC hDC)
{
	NoGo->DrawBoard(hDC);
}

DLLFUCTION INT OnLButtonDown(int x, int y)
{
	return NoGo->OnLButtonDown(x, y);
}

DLLFUCTION INT OkMove(char *denCmd)
{
	INT k;
	k = NoGo->OkMove();
	strcpy(denCmd, NoGo->denCmd);
	return k;
}

DLLFUCTION VOID CancelMove()
{
	NoGo->CancelMove();
}

DLLFUCTION INT ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{
	INT k = NoGo->ProcessMove(moveCmd);
	strcpy(curCmd, NoGo->curCmd);
	strcpy(denCmd, NoGo->denCmd);
	return k;
}

DLLFUCTION VOID OnRun()
{
	NoGo->InitGame();
}

DLLFUCTION INT GetCurPlayer()
{
	return NoGo->player;
}
