/**
 * file: Function.h
 * date: 2014/9/10
 * version: 1.0
 * description: 
 */


#ifndef MENU_H_GAMEPLATFORM
#define MMNU_H_GAMEPLATFORM


#include "Game Platform.h"


int Exit();//退出程序
int LoadBlackEngine();//加载黑方引擎
int UnloadBlackEngine();//卸载黑方引擎
int LoadWhiteEngine();//加载白方引擎
int UnloadWhiteEngine();//卸载摆放引擎
int GameStart();//开始对弈
int GameStop();//结束对弈
int MoveStep(int x,int y);//用户行棋
int OkMove();
int CancelMove();
int CheckEngineLoad();
int ShotBoard();



#endif