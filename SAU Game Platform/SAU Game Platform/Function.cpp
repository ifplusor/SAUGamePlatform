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
#include "Network.h"


/** 退出程序 */
int Function::Exit()
{
	return PostMessage(MainWnd->hWnd,WM_CLOSE,NULL,NULL);
}

/** 加载黑方引擎 */
int Function::LoadBlackEngine()
{
	return game.LoadEngine(BLACK);
}

/** 卸载黑方引擎 */
int Function::UnloadBlackEngine()
{
	return game.UnloadEngine(BLACK);
}

/** 加载白方引擎 */
int Function::LoadWhiteEngine()
{
	return game.LoadEngine(WHITE);
}

/** 卸载白方引擎 */
int Function::UnloadWhiteEngine()
{
	return game.UnloadEngine(WHITE);
}

/** 开始对弈 */
int Function::GameStart()
{
	game.StartGame();
	return 0;
}

/** 结束对弈 */
int Function::GameStop()
{
	game.StopGame();
	return 0;
}

/** 行棋 */
int Function::MoveStep(int x, int y)
{
	return game.MoveStep(x,y);
}

/** 确认着法 */
int Function::OkMove()
{
	game.OkMove();
	return 0;
}

/** 取消着法 */
int Function::CancelMove()
{
	game.CancelMove();
	return 0;
}

/** 检查是否有引擎加载 */
int Function::CheckEngineLoad()
{
	if (game.CheckEngineLoad())
		return 1;
	return 0;
}

/** 截取棋盘图片 */
int Function::ShotBoard()
{
	::PrintScrOnTemp();
	return 0;
}

/** 保存行棋截图 */
int Function::SaveBoardShot()
{
	::SavePrintScreen();
	return 0;
}

/** 保存棋谱 */
int Function::SaveChessManual(char *step)
{
	::SaveChessManual(step);
	return 0;
}

/** 启动网络对战 */
int Function::NetworkStart()
{
	//创建网络服务窗体，同时启动网络对战服务
	if (NetWork==0)
		NetWnd->CreateWnd();
	return 0;
}

/** 关闭网络对战 */
int Function::NetworkStop()
{
	//销毁网络对战服务窗体，同时关闭网络对战服务
	if (NetWork!=0)
		DestroyWindow(NetWnd->hWnd);
	return 0;
}