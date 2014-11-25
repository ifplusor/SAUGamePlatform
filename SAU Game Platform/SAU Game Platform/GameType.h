/**
 * file: GameType.h
 * date: 2014/9/16
 * version: 1.0
 * description: 棋种管理头文件
 */


#ifndef GAMETYPE_H_GAMEPLATFORM
#define GAMETYPE_H_GAMEPLATFORM

#include "Game Platform.h"



//棋种包标识（从50500开始向上递增）
#define CHESSTYPE 50500


//棋种支持模块描述符
struct _CHESSTYPE{
	HINSTANCE chessTP;//棋种包资源实例句柄
	char chessStr[20];//棋种名
	int type;//棋种类型  0：添子类完备信息博弈
};


//模块正确性校验
typedef VOID (*_CheckModule)(char *Info,char *ChessName,int *ChessType);
//初始化模块
typedef VOID (*_InitModule)(HWND hWnd,GameSet *gameSet);
//响应棋盘大小改变
typedef VOID (*_OnSize)(RECT rt);
//绘制棋盘
typedef VOID (*_DrawBoard)(HDC hDC);
//响应鼠标左键单击消息
typedef BOOL (*_OnLButtonDown)(int x,int y,char *wDMsg);
//引擎信息处理
typedef BOOL (*_ProcessMsg)(char *msg,char *wMMsg,char *wDMsg);
//响应对弈开始
typedef VOID (*_OnRun)();
typedef int (*_GetCurPlayer)();


extern _CheckModule CT_CheckModule;
extern _InitModule CT_InitModule;
extern _OnSize CT_OnSize;
extern _DrawBoard CT_DrawBoard;
extern _OnLButtonDown CT_OnLButtonDown;
extern _ProcessMsg CT_ProcessMsg;
extern _OnRun CT_OnRun;
extern _GetCurPlayer CT_GetCurPlayer;

extern struct _CHESSTYPE chessType[25];//棋种支持模块描述
extern int chesstype;//引用棋种支持模块索引

BOOL GetChessTypeResourse();//获取棋种支持模块资源
VOID CreateChessTypeMenu(HMENU hMenu);//创建棋种子菜单
VOID InitialChessType(int i,HMENU hMenu);//初始化棋种接口
VOID SetChessType(int i,HMENU hMenu);//设置棋种


#endif