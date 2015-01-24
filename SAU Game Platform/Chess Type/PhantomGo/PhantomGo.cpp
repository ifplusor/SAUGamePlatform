#define CHESSTYPE

#include "..\include\Chess.h"
#include "CPhantomGo.h"


HINSTANCE hInst;
CPhantomGo *PhantomGo;


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
	strcpy(ChessName, "»ÃÓ°Î§Æå");
	*ChessType = 12;//ÔÊĞíPassÆåÖÖ
}

DLLFUCTION VOID InitModule(HWND hWnd, char *LibPath)
{
	PhantomGo = new CPhantomGo(hInst, hWnd, LibPath);
	PhantomGo->InitGame();
}

DLLFUCTION VOID ExitModule()
{
	delete PhantomGo;
}

DLLFUCTION VOID OnSize(RECT rtBoard)
{
	PhantomGo->SetBoard(rtBoard);
}

DLLFUCTION VOID DrawBoard(HDC hDC)
{
	PhantomGo->DrawBoard(hDC);
}

DLLFUCTION INT OnLButtonDown(int x, int y)
{
	return PhantomGo->OnLButtonDown(x, y);
}

DLLFUCTION INT OkMove(char *denCmd)
{
	INT k;
	k = PhantomGo->OkMove();
	strcpy(denCmd, PhantomGo->denCmd);
	return k;
}

DLLFUCTION VOID CancelMove()
{
	PhantomGo->CancelMove();
}

DLLFUCTION INT ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{
	INT k = PhantomGo->ProcessMove(moveCmd);
	strcpy(curCmd, PhantomGo->curCmd);
	strcpy(denCmd, PhantomGo->denCmd);
	return k;
}

DLLFUCTION VOID OnRun()
{
	PhantomGo->InitGame();
}

DLLFUCTION INT GetCurPlayer()
{
	return PhantomGo->player;
}
