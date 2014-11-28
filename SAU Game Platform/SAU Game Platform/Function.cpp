/**
 * file: Function.cpp
 * date: 2014/9/10
 * version: 1.0
 * description: 
 */


#include "Function.h"
#include "CMainWnd.h"
#include "Control.h"


int Exit()
{
	return PostMessage(MainWnd->hWnd,WM_CLOSE,NULL,NULL);
}

int LoadBlackEngine()
{
	return game.LoadEngine(BLACK);
}

int UnloadBlackEngine()
{
	return game.UnloadEngine(BLACK);
}

int LoadWhiteEngine()
{
	return game.LoadEngine(WHITE);
}

int UnloadWhiteEngine()
{
	return game.UnloadEngine(WHITE);
}

int GameStart()
{
	game.StartGame();
	return 0;
}

int GameStop()
{
	game.StopGame();
	return 0;
}

int MoveStep(int x,int y)
{
	return game.MoveStep(x,y);
}

int OkMove()
{
	game.OkMove();
	return 0;
}

int CancelMove()
{
	game.CancelMove();
	return 0;
}