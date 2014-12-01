/**
 * file: Function.h
 * date: 2014/9/10
 * version: 1.0
 * description: 
 */


#ifndef MENU_H_GAMEPLATFORM
#define MMNU_H_GAMEPLATFORM


#include "Game Platform.h"


//平台提供功能接口
class Fuction
{
public:
	static int Exit();//退出程序
	static int LoadBlackEngine();//加载黑方引擎
	static int UnloadBlackEngine();//卸载黑方引擎
	static int LoadWhiteEngine();//加载白方引擎
	static int UnloadWhiteEngine();//卸载摆放引擎
	static int GameStart();//开始对弈
	static int GameStop();//结束对弈
	static int MoveStep(int x, int y);//用户行棋
	static int OkMove();
	static int CancelMove();
	static int CheckEngineLoad();
	static int ShotBoard();
	static int SaveChessManual(char *step);
};


#endif