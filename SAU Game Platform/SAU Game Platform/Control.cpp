/**
 * file: Control.h
 * date: 2014/9/13
 * version: 1.0
 * description: 
 */


#include "Control.h"
#include "GameType.h"
#include "CMainWnd.h"

Game game;


/**
 * EngineRun - 引擎命令获取线程主函数
 * @lpParam:	指明函数承担的引擎角色，即黑方/白方
 * description：	函数是一个线程主函数，函数内建一个循环过程用来获取博弈引擎发送的行棋事件
 *					函数用来承担裁判-棋手角色模型中的对等双核心交互技术。作为核心的承担者，函数不会维护完整的对弈逻辑。
 */
DWORD WINAPI EngineRun(LPVOID lpParam)
{
	CEngine *side,*unside;
	char rMsg[BUFSIZE],wMMsg[BUFSIZE],wDMsg[BUFSIZE];
	int temp;

	if((int)lpParam==BLACK)//判断线程承担的引擎角色
	{
		side=game.GetBlackE();
		unside=game.GetWhiteE();
	}
	else
	{
		side=game.GetWhiteE();
		unside=game.GetBlackE();
	}

	side->WriteMsg("name?\n");//询问引擎名称
	side->GetCommand("name",rMsg);//获取引擎名称
	side->SetName(rMsg);
	MainWnd->SetName(rMsg,(int)lpParam);
	while(1)
	{
		side->GetCommand("move",rMsg);//获取行棋事件
		if(CT_GetCurPlayer()!=(int)lpParam)//当前行棋方与本方引擎执棋颜色不同
			continue;
		temp = game.GameMode;
		MainWnd->UpdateTime();
		if(CT_ProcessMove(rMsg,wMMsg,wDMsg)==-1)//处理行棋事件，产生相应命令
			MsgBox("Break rule!", "error", 5000);
		if(wMMsg[0]!='\0')
			side->WriteMsg(wMMsg);
		if(wDMsg[0]!='\0'&&temp == 2)//机器对弈，发给对方引擎
			unside->WriteMsg(wDMsg);
	}

	return 0;
}

Game::Game()
{
	GameMode=-1;
}

Game::~Game()
{
}

bool Game::LoadEngine(int side)
{
	if(side==BLACK)
	{
		if(!BlackE.LoadEngine())
			return false;
		if(BlackT.StartThread(EngineRun,(LPVOID)BLACK)==NULL)
			return false;
	}
	else if(side==WHITE)
	{
		if(!WhiteE.LoadEngine())
			return false;
		if(WhiteT.StartThread(EngineRun,(LPVOID)WHITE)==NULL)
			return false;
	}
	return true;
}

bool Game::UnloadEngine(int side)
{
	if(side==BLACK)
	{
		if(BlackE.GetLoaded()==false)//看是否已卸载
		{
			MsgBox("Engine has unloaded!","Msg",1500);
			return true;
		}
		if(BlackT.StopThread()==FALSE)
			return false;
		if(!BlackE.UnloadEngine())
			return false;
	}
	else if(side==WHITE)
	{
		if(WhiteE.GetLoaded()==false)//看是否已卸载
		{
			MsgBox("Engine has unloaded!","Msg",1500);
			return true;
		}
		if(WhiteT.StopThread()==FALSE)
			return false;
		if(!WhiteE.UnloadEngine())
			return false;
	}
	SendMessage(MainWnd->hWnd, GM_NAME, (WPARAM)NULL, (LPARAM)side);
	return true;
}

bool Game::CheckEngineLoad()
{
	if (BlackE.GetLoaded() || WhiteE.GetLoaded())
		return true;
	return false;
}

void Game::StartGame()
{
	if(IsStop()==FALSE)
	{
		MsgBox("对弈正在进行","error",0);
		return;
	}
	CT_OnRun();//初始化棋局
	MainWnd->GameStart();
		if(BlackE.GetLoaded() && WhiteE.GetLoaded())//黑白引擎都加在载
		{
			GameMode=2;
			BlackE.WriteMsg("new black\n");
			WhiteE.WriteMsg("new white\n");
		}
		else if(BlackE.GetLoaded() || WhiteE.GetLoaded())//只加载一个引擎
		{
			if(chessType[chesstype].type!=0)
			{
				MsgBox("该棋种不支持人机对弈！","error",0);
				return;
			}
			if(BlackE.GetLoaded())
			{
				GameMode=1;
				BlackE.WriteMsg("new black\n");
			}			
			else
			{
				GameMode=0;
				WhiteE.WriteMsg("new white\n"); 
			}								
		}
		else//人人对弈
		{
			if(chessType[chesstype].type!=0)
			{
				MsgBox("该棋种不支持人人对弈！","error",0);
				return;
			}
			GameMode=3;
		}
}

void Game::StopGame()
{
	if(IsStop()==TRUE)
	{
		MsgBox("对弈尚未开始","error",3000);
		return;
	}
	MainWnd->GameStop();
	switch(GameMode)
	{
	case 0://人执黑，机器/网络执白
	case 1://人执白，机器/网络执黑
	case 2://机器对弈
	case 3://人人对弈
		break;
	case 4://机器执黑，网络执白
	case 5://机器执白，网络执黑
		break;
	}
	GameMode=-1;
}

bool Game::MoveStep(int x,int y)
{
	int temp,player=CT_GetCurPlayer();

	if(IsStop()==TRUE)
	{
		MsgBox("The game is end or not start!","error",1500);
		return false;
	}
	if(GameMode==2)//机器对弈
	{
		MsgBox("The game's mode is'engine vs engine',stop clicking in board!","Msg",1500);
	}
	else if(GameMode==3)//人人对弈
	{
		temp=CT_OnLButtonDown(x,y);
		switch(temp)
		{
		case -1://错误着法
			MsgBox("error step!","error",1000);
			return false;
		case 1://着法成立
			EnableWindow(GetDlgItem(MainWnd->hWnd, player ? IDB_CONTROL_OK_WHT : IDB_CONTROL_OK_BLC), TRUE);
		case 0://着法进行
			EnableWindow(GetDlgItem(MainWnd->hWnd, player ? IDB_CONTROL_CANCEL_WHT : IDB_CONTROL_CANCEL_BLC), TRUE);
			break;
		case 2://对弈结束
			break;
		}
	}
	else if(GameMode==4||GameMode==5)
	{
		MsgBox("The game's mode is'engine vs net',stop clicking in board!","Msg",1500);
	}
	else if(GameMode!=-1)//人机对弈
	{
		if(GameMode==player)//当前行棋方为人
		{
			temp=CT_OnLButtonDown(x,y);
			switch(temp)
			{
			case -1://错误着法
				MsgBox("error step!","error",1000);
				return false;
			case 1://着法成立
				EnableWindow(GetDlgItem(MainWnd->hWnd, player ? IDB_CONTROL_OK_WHT : IDB_CONTROL_OK_BLC), TRUE);
			case 0://着法进行
				EnableWindow(GetDlgItem(MainWnd->hWnd, player ? IDB_CONTROL_CANCEL_WHT : IDB_CONTROL_CANCEL_BLC), TRUE);
				break;
			case 2://对弈结束
				break;
			}
//			player=CT_GetCurPlayer();
//			if(player!=GameMode)//行棋换手
//			{
//				if(NetWork==0)
//				{
//					if(player==BLACK)
//						BlackE.WriteMsg(wMsg);
//					else
//						WhiteE.WriteMsg(wMsg);
//				}
//				else
//				{
//					if(player==BLACK)
//						NetShell(NULL,ConnectMode?SERVERINFO.s:CLIENTINFO.s,wMsg,strlen(wMsg)+1,2);
//					else
//						NetShell(NULL,ConnectMode?SERVERINFO.s:CLIENTINFO.s,wMsg,strlen(wMsg)+1,2);
//				}
//			}
		}
		else
		{
//			if(NetWork==0)
//			{
//				MsgBox("The computer hasn't move,stop clicking in board!","error",1500);
//			}
//			else
//			{
				MsgBox("Opponent hasn't move,stop clicking in board!","error",1500);
//			}
		}
	}
	return true;
}

bool Game::MoveStep(char *step)
{
	char wMMsg[256],wDMsg[256];
	int temp = GameMode;
	CT_ProcessMove(step, wMMsg, wDMsg);
	if (temp == 4)
	{
		BlackE.WriteMsg(wDMsg);
	}
	else if (temp == 5)
	{
		WhiteE.WriteMsg(wDMsg);
	}
	return true;
}

void Game::OkMove()
{
	char denCmd[256];
	BYTE player = CT_GetCurPlayer();
	int temp = GameMode;
	CT_OkMove(denCmd);
	switch (temp)
	{
	case 0:
		WhiteE.WriteMsg(denCmd);
		break;
	case 1:
		BlackE.WriteMsg(denCmd);
		break;
	}
	if (player == 0)
	{
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), FALSE);
	}
}

void Game::CancelMove()
{
	BYTE player = CT_GetCurPlayer();
	CT_CancelMove();
	if (player == 0)
	{
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), FALSE);
	}
}