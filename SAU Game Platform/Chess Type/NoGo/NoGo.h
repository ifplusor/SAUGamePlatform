#ifndef CHESSTYPE_CONSIX
#define CHESSTYPE_CONSIX

#include "CNoGo.h"

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#ifdef CHESSTYPE
#define DLLFUCTION extern "C" EXPORT
#else
#define DLLFUCTION extern "C" IMPORT
#endif


//模块正确性校验
DLLFUCTION VOID CheckModule(char *Info, char *ChessName, int *ChessType);
//初始化模块
DLLFUCTION VOID InitModule(HWND hWnd, char *LibPath);
//响应棋盘大小改变
DLLFUCTION VOID OnSize(RECT rtBoard);
//绘制棋盘
DLLFUCTION VOID DrawBoard(HDC hDC);
//响应对弈开始
DLLFUCTION VOID OnRun();
//响应鼠标左键单击消息
DLLFUCTION INT OnLButtonDown(int x, int y);
//确认招法
DLLFUCTION INT OkMove(char *denCmd);
//取消招法
DLLFUCTION VOID CancelMove();
//引擎行棋事件
DLLFUCTION INT ProcessMove(char *moveCmd, char *curCmd, char *denCmd);
//获取当前行棋方
DLLFUCTION INT GetCurPlayer();


#endif
