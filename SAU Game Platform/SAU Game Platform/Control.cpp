/**
 * file: Control.h
 * date: 2014/9/13
 * version: 1.0
 * description: 
 */


#include "Control.h"
#include "GameType.h"
#include "CMainWnd.h"
#include "Network.h"


Game game;


/**
 * EngineRun - 引擎命令获取线程主函数
 * @lpParam:	指明函数承担的引擎角色，即黑方/白方
 * description：	函数是一个线程主函数，函数内建一个循环过程用来获取博弈引擎发送的行棋事件
 *					函数用来承担裁判-棋手角色模型中的对等双核心交互技术。作为核心的承担者，函数不会维护完整的对弈逻辑。
 */
DWORD WINAPI EngineRun(LPVOID lpParam)
{
	int color;
	CEngine *side,*unside;
	char rMsg[BUFSIZE],wMMsg[BUFSIZE],wDMsg[BUFSIZE];
	int temp;//记录处理着法前的GameMode，防止因获胜置-1使着法不能发给对方引擎

	if((int)lpParam==BLACK)//判断线程承担的引擎角色
	{
		color = BLACK;
		side=game.GetBlackE();
		unside=game.GetWhiteE();
	}
	else
	{
		color = WHITE;
		side=game.GetWhiteE();
		unside=game.GetBlackE();
	}

	side->WriteMsg("name?\n");//询问引擎名称
	side->GetCommand("name",rMsg);//获取引擎名称
	side->SetName(rMsg);
	MainWnd->SetName(rMsg,(int)lpParam);
	side->SetStatus(1);//引擎就绪
	while(1)
	{
		side->GetCommand("move",rMsg);//获取行棋事件
		if(CT_GetCurPlayer()!=(int)lpParam)//当前行棋方与本方引擎执棋颜色不同
			continue;
		temp = game.GameMode;
		MainWnd->UpdateTime();
		if (CT_ProcessMove(rMsg, wMMsg, wDMsg) == -1)//处理行棋事件，产生相应命令
		{
			char errorMsg[500];
			if (!(chessType[chesstype].type & 8))
			{
				sprintf(errorMsg, "%s break rule!\nError command: %s", (color == BLACK) ? "Black" : "White", rMsg);
				MessageBox(MainWnd->hWnd, errorMsg, "error", MB_OK);
			}
		}
		if ((chessType[chesstype].type & 4) && (CT_GetCurPlayer() == game.GameMode))//允许pass，人行棋
		{
			if (game.GameMode == 0)
				game.OpenPB();
			else
				game.OpenPW();
		}
		if (wMMsg[0] != '\0')
			side->WriteMsg(wMMsg);
		if (wDMsg[0] != '\0')//机器对弈，发给对方引擎
		{
			switch (temp)
			{
			case 2:
				unside->WriteMsg(wDMsg);
				break;
			case 4:
			case 5:
				NetShell(NULL, ConnectMode ? SERVERINFO.s : CLIENTINFO.s, wDMsg, strlen(wDMsg) + 1, 2);
				break;
			}
		}
	}

	return 0;
}

Game::Game()
{
	GameMode=-1;
	passB = passW = false;
	okB = okW = false;
	cancelB = cancelW = false;
}

Game::~Game()
{
}

void Game::OpenCB()
{
	if (!cancelB)
	{
		cancelB = true;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), TRUE);
	}
	if (passB)
		ClosePB();
}

void Game::OpenCW()
{
	if (!cancelW)
	{
		cancelW = true;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), TRUE);
	}
	if (passW)
		ClosePW();
}

void Game::OpenOCB()
{
	if (!okB)
	{
		okB = true;
		if (passB)
		{
			passB = false;
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), "Ok");
		}
		else
		{
			EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), TRUE);
		}
	}
	if (!cancelB)
		OpenCB();
}

void Game::OpenOCW()
{
	if (!okW)
	{
		okW = true;
		if (passW)
		{
			passW = false;
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), "Ok");
		}
		else
		{
			EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), TRUE);
		}
	}
	if (!cancelW)
		OpenCW();
}

void Game::OpenPB()
{
	if (!passB)
	{
		passB = true;
		if (okB)
		{
			okB = false;
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), "Pass");
		}
		else
		{
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), "Pass");
			EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), TRUE);
		}
	}
}

void Game::OpenPW()
{
	if (!passW)
	{
		passW = true;
		if (okW)
		{
			okW = false;
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), "Pass");
		}
		else
		{
			SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), "Pass");
			EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), TRUE);
		}
	}
}

void Game::CloseOCB()
{
	if (okB)
	{
		okB = false;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
	}
	if (cancelB)
	{
		cancelB = false;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), FALSE);
	}
	if (passB)
	{
		passB = false;
		SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), "Ok");
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
	}
}

void Game::CloseOCW()
{
	if (okW)
	{
		okW = false;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
	}
	if (cancelW)
	{
		cancelW = false;
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), FALSE);
	}
	if (passW)
	{
		passW = false;
		SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), "Ok");
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
	}
}

void Game::ClosePB()
{
	if (passB)
	{
		passB = false;
		SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), "Ok");
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
	}
}

void Game::ClosePW()
{
	if (passW)
	{
		passW = false;
		SetText(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), "Ok");
		EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
	}
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
		if(BlackE.GetStatus()==-1)//看是否已卸载
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
		if(WhiteE.GetStatus()==-1)//看是否已卸载
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
	if (BlackE.GetStatus()!=-1 || WhiteE.GetStatus()!=-1)//有引擎处于连接状态
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
	CloseOCB();
	CloseOCW();
	MainWnd->GameStart();
	if (NetWork == 0)//本地对弈
	{
		if (BlackE.GetStatus() != -1 && WhiteE.GetStatus() != -1)//黑白引擎都加在载
		{
			if (BlackE.GetStatus() == 1 && WhiteE.GetStatus() == 1)
			{
				GameMode = 2;
				BlackE.WriteMsg("new black\n");
				WhiteE.WriteMsg("new white\n");
			}
			else
				MsgBox("引擎未就绪", "error", 3000);
		}
		else if (BlackE.GetStatus() != -1 || WhiteE.GetStatus() != -1)//只加载一个引擎
		{
			if (chessType[chesstype].type & 2)
			{
				MsgBox("该棋种不支持人机对弈！", "error", 0);
				return;
			}
			if (BlackE.GetStatus() != -1)//人执白
			{
				if (BlackE.GetStatus() == 1)
				{
					GameMode = 1;
					BlackE.WriteMsg("new black\n");
				}
				else
					MsgBox("引擎未就绪", "error", 3000);
			}
			else//人执黑
			{
				if (WhiteE.GetStatus() == 1)
				{
					GameMode = 0;
					WhiteE.WriteMsg("new white\n");
					if (chessType[chesstype].type & 4)//允许pass
					{
						OpenPB();
					}
				}
				else
					MsgBox("引擎未就绪", "error", 3000);
			}
		}
		else//人人对弈
		{
			if (chessType[chesstype].type & 1)
			{
				MsgBox("该棋种不支持人人对弈！", "error", 0);
				return;
			}
			GameMode = 3;
			if (chessType[chesstype].type & 4)//允许pass
			{
				OpenPB();
			}
		}
	}
	else//跨网络对弈
	{//跨网络对弈中，因为用户协商机制，只提供就绪状态引擎的对弈服务，即当引擎已加载但未就绪时开始的对弈按自然人对弈处理。
		if (chessType[chesstype].type & 8)
		{
			MsgBox("该棋种不支持网络对弈！", "error", 0);
		}
		if (GameMode_2 == 0)//执黑
		{
			if (BlackE.GetStatus() == 1)
			{
				GameMode = 4;
				BlackE.WriteMsg("new black\n");
			}
			else//人执黑
			{
				GameMode = 0;
				if (chessType[chesstype].type & 4)//允许pass
				{
					OpenPB();
				}
			}
		}
		else
		{
			if (WhiteE.GetStatus() == 1)
			{
				GameMode = 5;
				WhiteE.WriteMsg("new white\n");
			}
			else
				GameMode = 1;
		}
	}
}

/**
 * StopGame - 结束对弈
 * description：	该函数用于结束对弈逻辑，用户强制结束对弈和终盘时调用
 */
void Game::StopGame()
{
	if(IsStop()==TRUE)
	{
		MsgBox("对弈尚未开始","error",3000);
		return;
	}
	MainWnd->GameStop();//关闭计时器
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_BLC), FALSE);
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_BLC), FALSE);
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_OK_WHT), FALSE);
	EnableWindow(GetDlgItem(MainWnd->hWnd, IDB_CONTROL_CANCEL_WHT), FALSE);
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
			MessageBox(MainWnd->hWnd, "error step!","error", MB_OK);
			return false;
		case 1://着法成立
			if (player == BLACK)
				OpenOCB();
			else
				OpenOCW();
			break;
		case 0://着法进行
			if (player == BLACK)
				OpenCB();
			else
				OpenCW();
			break;
		case 2://不是可落位置
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
				MessageBox(MainWnd->hWnd, "error step!", "error", MB_OK);
				return false;
			case 1://着法成立
				if (player == BLACK)
					OpenOCB();
				else
					OpenOCW();
				break;
			case 0://着法进行
				if (player == BLACK)
					OpenCB();
				else
					OpenCW();
				break;
			case 2://不是可落位置
				break;
			}
		}
		else
		{
			if(NetWork==0)
			{
				MsgBox("The computer hasn't move,stop clicking in board!","error",1500);
			}
			else
			{
				MsgBox("Opponent hasn't move,stop clicking in board!","error",1500);
			}
		}
	}
	return true;
}

/**
 * MoveStep - 行棋
 * @step：	行棋命令
 * description：	该函数用于跨网络对弈，对网络另一端的行棋着法进行解析
 */
bool Game::MoveStep(char *step)
{
	char curCmd[256], denCmd[256];
	CT_ProcessMove(step, curCmd, denCmd);
	//由NetShell调用，为网络另一方平台副本的逻辑输入。
	if (curCmd[0] != '\0')//返回给网络另一端平台副本的命令。因为另一端平台副本的维护，所以不存在非法着法，故不含有“error”命令。
	{
		NetShell(NULL, ConnectMode ? SERVERINFO.s : CLIENTINFO.s, curCmd, strlen(curCmd) + 1, 2);
	}
	if (denCmd[0] != '\0')//交付给本地棋手的命令
	{
		if (GameMode == 4)//机器执黑
		{
			BlackE.WriteMsg(denCmd);
		}
		else if (GameMode == 5)//机器执白
		{
			WhiteE.WriteMsg(denCmd);
		}
		else//人行棋
		{
			if (chessType[chesstype].type & 4)
			{
				if (GameMode == BLACK)
					OpenPB();
				else
					OpenPW();
			}
	}
	}
	return true;
}

void Game::OkMove()
{
	char denCmd[256];
	BYTE player = CT_GetCurPlayer();
	int temp = GameMode;//记录处理着法前的GameMode，防止因获胜置-1使着法不能发给对方引擎
	if (CT_OkMove(denCmd) != 0)
	{
		if (denCmd[0] != '\0')//具有向另一方发送信息的意向
		{
			if (NetWork == 0)//非网络工作模式
			{
				switch (temp)
				{
				case 0://执黑，发送给白方
					WhiteE.WriteMsg(denCmd);
					break;
				case 1://执白，发送给黑方
					BlackE.WriteMsg(denCmd);
					break;
				}
			}
			else//网络工作模式
			{
				NetShell(NULL, ConnectMode ? SERVERINFO.s : CLIENTINFO.s, denCmd, strlen(denCmd) + 1, 2);
			}
		}

		if (player == 0)//禁用黑方按钮
		{
			if ((chessType[chesstype].type & 4) && GameMode == 3)
			{
				OpenPW();
			}
			CloseOCB();
		}
		else//禁用白方按钮
		{
			if ((chessType[chesstype].type & 4) && GameMode == 3)
			{
				OpenPB();
			}
			CloseOCW();
		}
	}
}

void Game::CancelMove()
{
	BYTE player = CT_GetCurPlayer();
	if (CT_CancelMove() == 1)
	{
		if (player == 0)
		{
			CloseOCB();
			if (chessType[chesstype].type & 4)
			{
				OpenPB();
			}
		}
		else
		{
			CloseOCW();
			if (chessType[chesstype].type & 4)
			{
				OpenPW();
			}
		}
	}
}