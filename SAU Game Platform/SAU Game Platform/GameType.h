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
	int type;//棋种类型位标记  0bit：禁用人人对弈标记；1bit：禁用人机对弈；2bit：允许Pass标记；3bit：禁用网络对战
	char chessStr[20];//棋种名
	char LibPath[MAX_PATH];
};


//模块正确性校验
typedef VOID (*_CheckModule)(char *Info, char *ChessName, int *ChessType);
//初始化模块
typedef VOID (*_InitModule)(HWND hWnd, char *LibPath);
typedef VOID (*_ExitModule)();
//响应棋盘大小改变
typedef VOID (*_OnSize)(RECT rtBoard);
//绘制棋盘
typedef VOID (*_DrawBoard)(HDC hDC);
//响应对弈开始
typedef VOID (*_OnRun)();
//响应鼠标左键单击消息
typedef INT (*_OnLButtonDown)(int x, int y);
//确认招法
typedef INT (*_OkMove)(char *denCmd);
//取消招法
typedef VOID (*_CancelMove)();
//引擎行棋事件
typedef INT (*_ProcessMove)(char *moveCmd, char *curCmd, char *denCmd);
//获取当前行棋方
typedef INT (*_GetCurPlayer)();



extern _CheckModule CT_CheckModule;
extern _InitModule CT_InitModule;
extern _ExitModule CT_ExitModule;
extern _OnSize CT_OnSize;
extern _DrawBoard CT_DrawBoard;
extern _OnLButtonDown CT_OnLButtonDown;
extern _OkMove CT_OkMove;
extern _CancelMove CT_CancelMove;
extern _ProcessMove CT_ProcessMove;
extern _OnRun CT_OnRun;
extern _GetCurPlayer CT_GetCurPlayer;

extern struct _CHESSTYPE chessType[25];//棋种支持模块描述
extern int chesstype;//引用棋种支持模块索引
extern int chessNum;

BOOL GetChessTypeResourse();//获取棋种支持模块资源
VOID CreateChessTypeMenu(HMENU hMenu);//创建棋种子菜单
VOID InitialChessType(HMENU hMenu);//初始化棋种接口
VOID SetChessType(int i,HMENU hMenu);//设置棋种


#endif