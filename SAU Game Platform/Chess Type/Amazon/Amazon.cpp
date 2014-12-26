#define CHESSTYPE

#include "Amazon.h"

HINSTANCE hInst;
CAmazon *Amazon;

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
	strcpy(ChessName, "ÑÇÂíÑ·Æå");
	*ChessType = 0;
}

DLLFUCTION VOID InitModule(HWND hWnd, char *LibPath)
{
	Amazon = new CAmazon(hInst, hWnd, LibPath);
	Amazon->InitGame();
}

DLLFUCTION VOID ExitModule()
{
	delete Amazon;
}

DLLFUCTION VOID OnSize(RECT rtBoard)
{
	Amazon->SetBoard(rtBoard);
}

DLLFUCTION VOID DrawBoard(HDC hDC)
{
	Amazon->DrawBoard(hDC);
}

DLLFUCTION INT OnLButtonDown(int x, int y)
{
	return Amazon->OnLButtonDown(x, y);
}

DLLFUCTION INT OkMove(char *denCmd)
{
	INT k;
	k = Amazon->OkMove();
	strcpy(denCmd, Amazon->denCmd);
	return k;
}

DLLFUCTION VOID CancelMove()
{
	Amazon->CancelMove();
}

DLLFUCTION INT ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{
	INT k = Amazon->ProcessMove(moveCmd);
	strcpy(curCmd, Amazon->curCmd);
	strcpy(denCmd, Amazon->denCmd);
	return k;
}

DLLFUCTION VOID OnRun()
{
	Amazon->InitGame();
}

DLLFUCTION INT GetCurPlayer()
{
	return Amazon->player;
}
