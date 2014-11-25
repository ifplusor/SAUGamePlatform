#ifndef CHESSTYPE_CONSIX
#define CHESSTYPE_CONSIX

#include "CConSix.h"

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
DLLFUCTION VOID InitModule(HWND hWnd,GameSet *gameset);
//响应棋盘大小改变
DLLFUCTION VOID OnSize(RECT rt);
//绘制棋盘
DLLFUCTION VOID DrawBoard(HDC hDC);
//响应鼠标左键单击消息
DLLFUCTION BOOL OnLButtonDown(int x,int y,char *wDMsg);
//引擎信息处理
DLLFUCTION BOOL ProcessMsg(char *msg,char *wMMsg,char *wDMsg);
//响应对弈开始
DLLFUCTION VOID OnRun();
//获取当前行棋方
DLLFUCTION int GetCurPlayer();


#endif
