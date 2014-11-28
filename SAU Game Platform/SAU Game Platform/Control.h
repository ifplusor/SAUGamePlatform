/**
 * file: Control.h
 * date: 2014/9/13
 * version: 1.0
 * description: 
 */


#ifndef CONTROL_H_GAMEPLATFORM
#define CONTROL_H_GAMEPLATFORM

#include "Game Platform.h"
#include "CEngine.h"


DWORD WINAPI EngineRun(LPVOID lpParam);//引擎运行线程函数

class Game
{
public:
	int GameMode;	//对弈模式  -1:未开始 0：人执黑，机器\网络执白 1：人执白，机器\网络执黑 2：机器对弈 3：人对弈
					//4：机器执黑，网络执白 5：机器执白，网络执黑

	Game();
	~Game();

	bool LoadEngine(int side);//加载引擎：注册一个博弈活动参与者
	bool UnloadEngine(int side);//卸载引擎
	void StartGame();//启动对弈逻辑
	void StopGame();//终止对弈逻辑
	bool MoveStep(int x,int y);//行棋（提供用户）
	bool MoveStep(char *step);//行棋（提供引擎）
	void OkMove();
	void CancelMove();
	BOOL IsStop(){if(GameMode==-1)return TRUE;else return FALSE;}
	CEngine* GetBlackE(){return &BlackE;}//获取黑方引擎对象指针
	CEngine* GetWhiteE(){return &WhiteE;}//获取白方引擎对象指针
private:
	CEngine BlackE,WhiteE;//引擎对象
	CThread BlackT,WhiteT;//线程对象
};

extern Game game;


#endif