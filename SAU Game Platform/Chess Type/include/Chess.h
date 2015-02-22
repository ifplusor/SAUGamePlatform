#ifndef CHESSTYPE_CHESS
#define CHESSTYPE_CHESS

#include <Windows.h>

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#ifdef CHESSTYPE
#define DLLFUCTION extern "C" EXPORT
#else
#define DLLFUCTION extern "C" IMPORT
#endif



//模块正确性校验
DLLFUCTION VOID CheckModule(char *Info,char *ChessName,int *ChessType);
//初始化模块
DLLFUCTION VOID* InitModule(HWND hWnd,char *LibPath);


#endif
