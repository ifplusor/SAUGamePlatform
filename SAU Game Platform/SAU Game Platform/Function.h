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
class Function
{
public:
	//菜单可引用功能
	static int Exit();//退出程序
	static int LoadBlackEngine();//加载黑方引擎
	static int UnloadBlackEngine();//卸载黑方引擎
	static int LoadWhiteEngine();//加载白方引擎
	static int UnloadWhiteEngine();//卸载摆放引擎
	static int GameStart();//开始对弈
	static int GameStop();//结束对弈
	static int OkMove();//确认着法
	static int CancelMove();//取消着法
	static int CheckEngineLoad();
	static int ShotBoard();//截图
	static int NetworkStart();//启动网络对战
	static int NetworkStop();//关闭网络对战

	//菜单不可引用功能
	static int MoveStep(int x, int y);//用户行棋
	static int SaveBoardShot();//保存截图，用来实现行棋中自动截图
	static int SaveChessManual(char *step);//保存棋谱，用来实现行棋中自动保存棋谱
};


#endif