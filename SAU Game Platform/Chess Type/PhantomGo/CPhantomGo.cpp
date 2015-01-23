#pragma comment(lib,"winmm.lib")

#include "CPhantomGo.h"
#include <stdio.h>

const int lineVector[4][2] = { 0, 1, 0, -1, -1, 0, 1, 0 };


VOID __cdecl ErrorBox(LPTSTR ErrorInfo)//错误提示框
{
	CHAR error1[50],error2[20];
	strcpy(error1,ErrorInfo);
	sprintf(error2,"\n\nerror: %d",GetLastError());	
	strcat(error1,error2);	
	MessageBox(NULL,error1,"error",MB_OK);
}

CPhantomGo::CPhantomGo(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);

	BkColor = RGB(0, 0, 0);
	BoardColor = RGB(255, 255, 0);
	hPen = NULL;
	hFont = NULL;
	InitGame();
	count = -1;

	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hMarkDC = CreateCompatibleDC(hDC);
}

CPhantomGo::~CPhantomGo()
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hMarkDC);
}


VOID CPhantomGo::SetBoard(RECT rtBoard)
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left;
	d = side / 10;
	pixel = ((double)side) / 600;
	hPen = CreatePen(PS_SOLID, (int)(2 * pixel), RGB(0, 0, 0));
	fWidth = (int)(d / 3);
	fHeight = (int)(d * 2 / 3);
	hFont = CreateSimpleFont(fWidth, fHeight);
	DrawChess();
}

VOID CPhantomGo::DrawBoard(HDC hDC)//绘制棋盘
{
	int i,j;
	HPEN hOldPen;
	HFONT hOldFont;
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	
	::SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2],number[3];
	memset(letter,0,sizeof(letter));
	memset(number,0,sizeof(number));
	for(i=1;i<=9;i++)
	{			
		letter[0]='A'+i-1;
		itoa(i,number,10);
		
		TextOut(hDC, rtBoard.left + side*i / 10 - fWidth / 2, rtBoard.top+side/20-fHeight/2, letter, 1);//绘制文字				
		TextOut(hDC, rtBoard.left + side*i / 10 - fWidth / 2, rtBoard.top + side*19 / 20 - fHeight / 2, number, 2);

		MoveToEx(hDC, rtBoard.left + side / 10, rtBoard.top + side*i / 10, NULL);//绘制线
		LineTo(hDC, rtBoard.left + side * 9 / 10, rtBoard.top + side*i / 10);
	}
	for(i=1;i<=9;i++)
	{
		letter[0]='A'+i-1;
		itoa(i,number,10);

		TextOut(hDC, rtBoard.left+side/20 - fWidth/2, rtBoard.top + side*i / 10 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side*19 / 20 - fWidth / 2, rtBoard.top + side*i / 10 - fHeight / 2, number, 2);
		
		MoveToEx(hDC, rtBoard.left + side*i / 10, rtBoard.top + side / 10, NULL);
		LineTo(hDC, rtBoard.left + side*i / 10, rtBoard.top + side*9 / 10);
	}	
	
	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			if(board[i][j]==BLACK)
			{				
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hBlcDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hBlcDC, 0, 0, SRCPAINT);
			}
			else if(board[i][j]==WHITE)
			{				
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hWhtDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hWhtDC, 0, 0, SRCPAINT);
			}
		}
	}
	
	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		int x1 = curStep.point.x; int y1 = curStep.point.y;
		BitBlt(hDC, rtBoard.left + side*(x1 * 2 + 1) / 20, rtBoard.top + side*(y1 * 2 + 1) / 20, d, d, hMarkDC, d, 0, SRCAND);
		BitBlt(hDC, rtBoard.left + side*(x1 * 2 + 1) / 20, rtBoard.top + side*(y1 * 2 + 1) / 20, d, d, hMarkDC, 0, 0, SRCPAINT);
	}
}

bool CPhantomGo::DrawChess()
{
	char filename[MAX_PATH] = { 0 };
	HBITMAP hBlcBmp, hWhtBmp, hMarkBmp;

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\black.bmp");
	hBlcBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\white.bmp");
	hWhtBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\mark.bmp");
	hMarkBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	SelectObject(hBlcDC, hBlcBmp);
	SelectObject(hWhtDC, hWhtBmp);
	SelectObject(hMarkDC, hMarkBmp);

	DeleteObject(hBlcBmp);
	DeleteObject(hWhtBmp);
	DeleteObject(hMarkBmp);

	return true;
}

BOOL CPhantomGo::ProcessMove(char *moveCmd)
{	
	Step tStep;
	char *res;
	int pos = 0, i;
	int len=strlen("move ");
	curCmd[0] = denCmd[0] = '\0';//默认空消息
	if ((res = strstr(moveCmd, "move")) == NULL)//寻找move关键字，将过滤“access”命令
	{
		return 0;
	}	
	else
	{		
		pos = (res - moveCmd);
		pos+=len;
		
		if (strncmp("pass", moveCmd + pos, 4) == 0)// "move pass\n"
		{
			ShowStepHis("pass");
			if (flagP)
			{
				StepNum[player]++;//累计步数
				WinOrLose();
				sprintf(denCmd, "move pass\nend\n");
				sprintf(curCmd, "end\n");
				return 2;
			}
			flagP = true;
			sprintf(denCmd, "move pass\n");
		}
		else if (strncmp("go", moveCmd + pos, 2) == 0)// "move go\n"
		{
			sprintf(curCmd, "move quest\n");
			return 0;
		}
		else if (strncmp("quest", moveCmd + pos, 5) == 0)// "move quest\n"
		{
			tStep = stepStack.top();
			sprintf(curCmd, "move %c%c\n", tStep.point.x + 'A', tStep.point.y + 'A');
			return 0;
		}
		else
		{
			flagP = false;

			tStep.point.x = moveCmd[pos] - 'A';
			tStep.point.y = moveCmd[pos + 1] - 'A';
			tStep.side = player;
			stepStack.push(tStep);
			if (!FitRules())//判断是否符合规则
			{
				sprintf(curCmd, "error\n");
				stepStack.pop();
				return -1;
			}

			sprintf(curCmd, "access\n");//接受着法
			board[tStep.point.x][tStep.point.y] = tStep.side;
			flagJ = false;//取消劫标记
			if (Take(tStep.point.x, tStep.point.y, tStep.side))//提子测试
			{
				sprintf(denCmd, "taked %d %s\n", takeNum, takeList);//记录提子
				sprintf(curCmd + strlen(curCmd), "take %d %s\n", takeNum, takeList);
				if (takeNum == 1)//提一子，需进一步判断劫
				{
					if (Take(takeList[0] - 'A', takeList[1] - 'A', 1 - tStep.side) == 1)//一对一的双方气尽，不用对反手提子和当前落子进行比较
					{
						flagJ = true;//置劫标记
						Jie.x = denCmd[6] - 'A';
						Jie.y = denCmd[7] - 'A';
					}
					board[denCmd[6] - 'A'][denCmd[7] - 'A'] = EMPTY;
				}
				else
				{
					for (i = 0; i < takeNum; i++)
						board[takeList[i * 2] - 'A'][takeList[1 + i * 2] - 'A'] = EMPTY;//提子
				}
			}
			strcat(denCmd, "move go");//生成写消息

			InvalidateRect(hWnd, &rtBoard, FALSE);
			SendMessage(hWnd, WM_PAINT, NULL, NULL);
			PlaySnd(0);

			moveCmd[pos + 2] = '\0';
			ShowStepHis(moveCmd + pos);
		}
	}
	StepNum[player]++;//累计步数
	player = NEXTPLAYER(player);
	return 1;
}

bool CPhantomGo::PlaySnd(int sel)
{
	char filename[MAX_PATH]={0};
	switch(sel)
	{
	case 0:
		strcpy(filename, LibPath);
		strcat(filename,"\\wav\\落子.wav");
		break;
	default:
		break;
	}	
	if(!PlaySound(filename,NULL,SND_NOWAIT |SND_FILENAME))
	{
		ErrorBox("PlaySound failed");
		return false;
	}
	return true;
}


VOID CPhantomGo::InitGame()//游戏初始化
{
	memset(StepNum,0,sizeof(StepNum));
	player=BLACK;			
	InitBoard();	//初始化棋盘
	takeNum = 0;
	flagJ = false;
	flagP = false;
	count = 0;
	CleanStack(stepStack);
	return;
}

VOID CPhantomGo::InitBoard()
{
	int i,j;
	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			board[i][j]=EMPTY;
		}
	}

	InvalidateRect(hWnd,&rtBoard,FALSE);//刷新棋盘
	return;
}

BOOL CPhantomGo::OnLButtonDown(int x, int y)
{
	Point point;
	if (!InsideRect(&rtBoard, x, y))
		return 2;
	if (count == -1)//count=-1时return0可能造成意外
		return 0;

	point.x = ((x - rtBoard.left) * 10 - side / 2) / side;//把棋盘坐标转换成数组坐标
	point.y = ((y - rtBoard.top) * 10 - side / 2) / side;
	if (point.x < 0 || point.x >= 9 || point.y < 0 || point.y >= 9)
		return 2;
	return SToS(point);
}

BOOL CPhantomGo::SToS(Point point)
{
	Step tStep;
	tStep.point = point;
	tStep.side = player;
	stepStack.push(tStep);
	if (!FitRules())
	{
		stepStack.pop();
		return -1;
	}
	board[point.x][point.y] = player;
	InvalidateRect(hWnd, &rtBoard, FALSE);
	SendMessage(hWnd,WM_PAINT,NULL,NULL);
	PlaySnd(0);	
	count = -1;
	return 1;
}

INT CPhantomGo::OkMove()
{
	Step tStep;
	char step[5];
	int i;
	denCmd[0] = '\0';
	if (count == 0)//pass
	{
		ShowStepHis("pass");
		if (flagP)//双方Pass进入终盘
		{
			StepNum[player]++;//累计步数
			WinOrLose();
			sprintf(denCmd, "move pass\nend\n");// "end\n"命令
			return 2;
		}
		flagP = true;
		sprintf(denCmd, "move pass\n");// "move pass\n"命令
	}
	else if (count == -1)
	{
		flagP = false;
		tStep = stepStack.top();
		sprintf(step, "%c%c", tStep.point.x + 'A', tStep.point.y + 'A');
		ShowStepHis(step);
		flagJ = false;//取消劫标记
		if (Take(tStep.point.x, tStep.point.y, tStep.side))//提子测试
		{
			sprintf(denCmd, "taked %d %s\n", takeNum, takeList);//记录提子
			if (takeNum == 1)//提一子，需进一步判断劫
			{
				if (Take(takeList[0] - 'A', takeList[1] - 'A', 1 - tStep.side) == 1)//一对一的双方气尽，不用对反手提子和当前落子进行比较
				{
					flagJ = true;//置劫标记
					Jie.x = denCmd[6] - 'A';
					Jie.y = denCmd[7] - 'A';
				}
				board[denCmd[6] - 'A'][denCmd[7] - 'A'] = EMPTY;
			}
			else
			{
				for (i = 0; i < takeNum; i++)
					board[takeList[i * 2] - 'A'][takeList[1 + i * 2] - 'A'] = EMPTY;//提子
			}
			InvalidateRect(hWnd, &rtBoard, FALSE);
		}
		strcat(denCmd, "move go\n");// "move go\n"命令
		count = 0;
	}
	StepNum[player]++;//累计步数
	player = NEXTPLAYER(player);
	return 0;
}

VOID CPhantomGo::CancelMove()
{
	Step tStep;
	if (count == 0)
		return;
	tStep = stepStack.top();
	stepStack.pop();
	board[tStep.point.x][tStep.point.y] = EMPTY;
	InvalidateRect(hWnd, &rtBoard, FALSE);
	SendMessage(hWnd, WM_PAINT, NULL, NULL);
	count = 0;
	return;
}

int CPhantomGo::GetQi(int x, int y, int side)
{
	for (int k = 0; k<81; k++)
		Biaoji[k] = -1;
	return GetQiForPoint(x, y, side);
}

int CPhantomGo::GetQiForPoint(int a, int b, int side)
{
	int x, y, i, qs = 0;
	Biaoji[a * 9 + b] = MARK;
	for (i = 0; i<4; i++)
	{
		x = a + lineVector[i][0];
		y = b + lineVector[i][1];
		if (x<0 || y<0 || x >= 9 || y >= 9)//超出棋盘边界
			continue;
		if (Biaoji[x * 9 + y] == MARK)
			continue;
		if (board[x][y] == EMPTY)
		{
			qs++;
			Biaoji[x * 9 + y] = MARK;
		}
		else if (board[x][y] == side)
			qs += GetQiForPoint(x, y, side);
	}
	return qs;
}

/**
 * Take - 提子测试
 * @x:	待落子点横坐标
 * @y：	待落子点纵坐标
 * @side:	待落子颜色
 */
bool CPhantomGo::Take(int x, int y, int side)
{
	bool take = false;
	int i, j, tx, ty, deplayer = 1 - side;
	for (i = 0; i < 81; i++)
		NoQi[i] = 0;
	for (i = 0; i < 4; i++)
	{
		tx = x + lineVector[i][0];
		ty = y + lineVector[i][1];
		if (tx<0 || ty<0 || tx >= 9 || ty >= 9)//超出棋盘边界
			continue;
		if (board[tx][ty] == deplayer)//已被提子
		{
			if (NoQi[tx * 9 + ty] == 1)
				continue;
			if (GetQi(tx, ty, deplayer) == 0)//可以提子
			{
				take = true;
				for (j = 0; j < 81; j++)
					if (Biaoji[j] == MARK)
						NoQi[j] = 1;
			}
		}
	}
	takeNum = 0;
	for (i = 0; i < 81; i++)
	{
		if (NoQi[i])
		{
			sprintf(takeList + takeNum * 2, "%c%c", 'A' + i / 9, 'A' + i % 9);
			takeNum++;
		}
	}
	return take;
}

bool CPhantomGo::WinOrLose()//判断胜负
{
	BYTE side = 0;
	PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)side);
	return true;	
}

/*
 * FitRules - 规则测试
 */
bool CPhantomGo::FitRules()//是否符合规则
{
	Step sp = stepStack.top();
	int deplayer = 1 - sp.side;
	if(sp.point.x<0 ||sp.point.x>8 ||sp.point.y<0 ||sp.point.y>8)
	{
		return false;
	}
	if (board[sp.point.x][sp.point.y] != EMPTY)
	{
		return false;
	}
	board[sp.point.x][sp.point.y] = sp.side;
	if (GetQi(sp.point.x, sp.point.y, sp.side) == 0)//落子无气
	{
		if (!Take(sp.point.x, sp.point.y, sp.side))//不能提吃，禁招点
		{
			board[sp.point.x][sp.point.y] = EMPTY;
			return false;
		}
		if (flagJ)//判断劫点
		{
			if ((Jie.x == sp.point.x) && (Jie.y == sp.point.y))//劫点不可落子，需要应劫
			{
				board[sp.point.x][sp.point.y] = EMPTY;
				return false;
			}
		}
	}
	board[sp.point.x][sp.point.y] = EMPTY;
	return true;
}
