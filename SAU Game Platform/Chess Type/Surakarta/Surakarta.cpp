#define CHESSTYPE

#include "../include/Chess.h"
#include "CSurakarta.h"


HINSTANCE hInst;


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
	strcpy(ChessName, "ËÕÀ­¿¨¶ûËþÆå");
	*ChessType = 0;
}

DLLFUCTION VOID* InitModule(HWND hWnd, char *LibPath)
{
	return (VOID*)new CSurakarta(hInst, hWnd, LibPath);
}
