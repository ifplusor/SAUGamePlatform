/**
 * file: Function.cpp
 * date: 2014/9/10
 * version: 1.0
 * description: 
 */


#include "Function.h"
#include "CMainWnd.h"
#include "Control.h"
#include "Manual.h"


int Fuction::Exit()
{
	return PostMessage(MainWnd->hWnd,WM_CLOSE,NULL,NULL);
}

int Fuction::LoadBlackEngine()
{
	return game.LoadEngine(BLACK);
}

int Fuction::UnloadBlackEngine()
{
	return game.UnloadEngine(BLACK);
}

int Fuction::LoadWhiteEngine()
{
	return game.LoadEngine(WHITE);
}

int Fuction::UnloadWhiteEngine()
{
	return game.UnloadEngine(WHITE);
}

int Fuction::GameStart()
{
	game.StartGame();
	return 0;
}

int Fuction::GameStop()
{
	game.StopGame();
	return 0;
}

int Fuction::MoveStep(int x, int y)
{
	return game.MoveStep(x,y);
}

int Fuction::OkMove()
{
	game.OkMove();
	return 0;
}

int Fuction::CancelMove()
{
	game.CancelMove();
	return 0;
}

int Fuction::CheckEngineLoad()
{
	if (game.CheckEngineLoad())
		return 1;
	return 0;
}

int Fuction::ShotBoard()
{
	PrintScrOnTemp();
	return 0;
}

int Fuction::SaveChessManual(char *step)
{
	SaveChessManual(step);
	return 0;
}