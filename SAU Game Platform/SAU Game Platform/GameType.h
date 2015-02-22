/**
 * file: GameType.h
 * date: 2014/9/16
 * version: 1.0
 * description: 棋种管理头文件
 */


#ifndef GAMETYPE_H_GAMEPLATFORM
#define GAMETYPE_H_GAMEPLATFORM

#include "Game Platform.h"
#include "..\\Chess Type\\include\\CChess.h"



//棋种包标识（从50500开始向上递增）
#define CHESSTYPE 50500


//棋种支持模块描述符
struct _CHESSTYPE{
	HINSTANCE chessTP;//棋种包资源实例句柄
	int type;//棋种类型位标记  0bit：禁用人人对弈标记；1bit：禁用人机对弈；2bit：允许Pass标记；3bit：禁用网络对战；4bit：禁用error对话框
	char chessStr[20];//棋种名
	char LibPath[MAX_PATH];
};


//模块正确性校验
typedef VOID (*_CheckModule)(char *Info, char *ChessName, int *ChessType);
//初始化模块
typedef VOID* (*_InitModule)(HWND hWnd, char *LibPath);


extern _CheckModule CT_CheckModule;
extern _InitModule CT_InitModule;

extern CChess *GameType;

extern struct _CHESSTYPE chessType[25];//棋种支持模块描述
extern int chesstype;//引用棋种支持模块索引
extern int chessNum;

BOOL GetChessTypeResourse();//获取棋种支持模块资源
VOID CreateChessTypeMenu(HMENU hMenu);//创建棋种子菜单
VOID InitialChessType(HMENU hMenu);//初始化棋种接口
VOID SetChessType(int i,HMENU hMenu);//设置棋种


#endif