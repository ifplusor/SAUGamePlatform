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


int Function::Exit()
{
	return PostMessage(MainWnd->hWnd,WM_CLOSE,NULL,NULL);
}

int Function::LoadBlackEngine()
{
	return game.LoadEngine(BLACK);
}

int Function::UnloadBlackEngine()
{
	return game.UnloadEngine(BLACK);
}

int Function::LoadWhiteEngine()
{
	return game.LoadEngine(WHITE);
}

int Function::UnloadWhiteEngine()
{
	return game.UnloadEngine(WHITE);
}

int Function::GameStart()
{
	game.StartGame();
	return 0;
}

int Function::GameStop()
{
	game.StopGame();
	return 0;
}

int Function::MoveStep(int x, int y)
{
	return game.MoveStep(x,y);
}

int Function::OkMove()
{
	game.OkMove();
	return 0;
}

int Function::CancelMove()
{
	game.CancelMove();
	return 0;
}

int Function::CheckEngineLoad()
{
	if (game.CheckEngineLoad())
		return 1;
	return 0;
}

int Function::ShotBoard()
{
	::PrintScrOnTemp();
	return 0;
}

int Function::SaveBoardShot()
{
	::SavePrintScreen();
	return 0;
}

int Function::SaveChessManual(char *step)
{
	::SaveChessManual(step);
	return 0;
}