#define CHESSTYPE

#include "..\include\Chess.h"
#include "CDotsAndBoxes.h"


HINSTANCE hInst;
CDotsAndBoxes *DotsAndBoxes;


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
	strcpy(ChessName, "µã¸ñÆå");
	*ChessType = 0;
}

DLLFUCTION VOID InitModule(HWND hWnd, char *LibPath)
{
	DotsAndBoxes = new CDotsAndBoxes(hInst, hWnd, LibPath);
	DotsAndBoxes->InitGame();
}

DLLFUCTION VOID ExitModule()
{
	delete DotsAndBoxes;
}

DLLFUCTION VOID OnSize(RECT rtBoard)
{
	DotsAndBoxes->SetBoard(rtBoard);
}

DLLFUCTION VOID DrawBoard(HDC hDC)
{
	DotsAndBoxes->DrawBoard(hDC);
}

DLLFUCTION BOOL OnLButtonDown(int x,int y)
{
	return DotsAndBoxes->OnLButtonDown(x, y);
}

DLLFUCTION INT OkMove(char *denCmd)
{
	INT k;
	k = DotsAndBoxes->OkMove();
	strcpy(denCmd, DotsAndBoxes->denCmd);
	return k;
}

DLLFUCTION INT CancelMove()
{
	return DotsAndBoxes->CancelMove();
}

DLLFUCTION INT ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{
	INT k = DotsAndBoxes->ProcessMove(moveCmd);
	strcpy(curCmd, DotsAndBoxes->curCmd);
	strcpy(denCmd, DotsAndBoxes->denCmd);
	return k;
}

DLLFUCTION VOID OnRun()
{
	DotsAndBoxes->InitGame();
}

DLLFUCTION INT GetCurPlayer()
{
	return DotsAndBoxes->player;
}
