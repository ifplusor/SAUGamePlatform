#define CHESSTYPE

#include "../include/Chess.h"
#include "CSurakarta.h"


HINSTANCE hInst;
CSurakarta *Surakarta;


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
	strcpy(ChessName, "ËÕÀ­¿¨¶ûËşÆå");
	*ChessType = 0;
}

DLLFUCTION VOID InitModule(HWND hWnd, char *LibPath)
{
	Surakarta = new CSurakarta(hInst, hWnd, LibPath);
	Surakarta->InitGame();
}

DLLFUCTION VOID ExitModule()
{
	delete Surakarta;
}

DLLFUCTION VOID OnSize(RECT rtBoard)
{
	Surakarta->SetBoard(rtBoard);
}

DLLFUCTION VOID DrawBoard(HDC hDC)
{
	Surakarta->DrawBoard(hDC);
}

DLLFUCTION INT OnLButtonDown(int x, int y)
{
	return Surakarta->OnLButtonDown(x, y);
}

DLLFUCTION INT OkMove(char *denCmd)
{
	INT k;
	k = Surakarta->OkMove();
	strcpy(denCmd, Surakarta->denCmd);
	return k;
}

DLLFUCTION INT CancelMove()
{
	return Surakarta->CancelMove();
}

DLLFUCTION INT ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{
	INT k = Surakarta->ProcessMove(moveCmd);
	strcpy(curCmd, Surakarta->curCmd);
	strcpy(denCmd, Surakarta->denCmd);
	return k;
}

DLLFUCTION VOID OnRun()
{
	Surakarta->InitGame();
}

DLLFUCTION INT GetCurPlayer()
{
	return Surakarta->player;
}
