#pragma comment(lib,"winmm.lib")

#include "CNoGo.h"
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

CNoGo::CNoGo(HINSTANCE hInst, HWND hWnd, char *LibPath)
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

CNoGo::~CNoGo()
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hMarkDC);
}


VOID CNoGo::SetBoard(RECT rtBoard)
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

VOID CNoGo::DrawBoard(HDC hDC)//绘制棋盘
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

bool CNoGo::DrawChess()
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

BOOL CNoGo::ProcessMove(char *moveCmd)
{	
	Step tStep;
	char *res;
	int pos=0;
	int len=strlen("move ");
	if ((res = strstr(moveCmd, "move")) == NULL)//寻找move关键字
	{
		return 0;
	}	
	else
	{		
		pos = (res - moveCmd);
		pos+=len;
		
		tStep.point.x = moveCmd[pos] - 'A';
		tStep.point.y = moveCmd[pos + 1] - 'A';
		tStep.side = player;
		stepStack.push(tStep);

		if(!FitRules())//判断是否符合规则
		{
			sprintf(curCmd, "error\n");
			sprintf(denCmd, "\0");
			stepStack.pop();
			return -1;
		}
		board[tStep.point.x][tStep.point.y] = tStep.side;
		InvalidateRect(hWnd,&rtBoard,FALSE);
		SendMessage(hWnd,WM_PAINT,NULL,NULL);
		PlaySnd(0);
		moveCmd[pos + 2] = '\0';
		ShowStepHis(moveCmd + pos);

		sprintf(denCmd, "move %c%c\n", tStep.point.x + 'A', tStep.point.y + 'A');//生成写消息
		sprintf(curCmd, "\0");
	}
	StepNum[player]++;//累计步数
	if(WinOrLose())//判断胜负
	{
		sprintf(denCmd + strlen(denCmd), "end\n");
		sprintf(curCmd, "end\n");
		return 2;
	}	
	player = NEXTPLAYER(player);
	return 1;
}

bool CNoGo::PlaySnd(int sel)
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


VOID CNoGo::InitGame()//游戏初始化
{
	memset(StepNum,0,sizeof(StepNum));
	player=BLACK;			
	InitBoard();	//初始化棋盘
	count = 0;
	CleanStack(stepStack);
	return;
}

VOID CNoGo::InitBoard()
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

BOOL CNoGo::OnLButtonDown(int x,int y)
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

BOOL CNoGo::SToS(Point point)
{
	Step tStep;
	if (board[point.x][point.y] != EMPTY)
		return -1;
	board[point.x][point.y] = player;
	tStep.point = point;
	tStep.side = player;
	stepStack.push(tStep);
	InvalidateRect(hWnd, &rtBoard, FALSE);
	SendMessage(hWnd,WM_PAINT,NULL,NULL);
	PlaySnd(0);	
	count = -1;
	return 1;
}

INT CNoGo::OkMove()
{
	Step tStep;
	tStep = stepStack.top();
	sprintf(denCmd, "move %c%c", tStep.point.x + 'A', tStep.point.y + 'A');
	ShowStepHis(denCmd + 5);
	StepNum[player]++;//累计步数
	player = NEXTPLAYER(player);
	if (WinOrLose())
	{
		sprintf(denCmd + strlen(denCmd), "\nend\n");
		return 2;
	}
	sprintf(denCmd + strlen(denCmd), "\n");
	count = 0;
	return 0;
}

VOID CNoGo::CancelMove()
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

int CNoGo::GetQi(int x, int y, int side)
{
	for (int k = 0; k<81; k++)
		Biaoji[k] = -1;
	return GetQiForPoint(x, y, side);
}

int CNoGo::GetQiForPoint(int a, int b, int side)
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

bool CNoGo::WinOrLose()//判断胜负
{
	Step tStep = stepStack.top();
	BYTE side = tStep.side;
	if (GetQi(tStep.point.x, tStep.point.y, side) == 0)//自杀
	{
		PostMessage(hWnd,GM_WINLOSE,(WPARAM)(StepNum[BLACK]<<16)+StepNum[WHITE],(LPARAM)1-side);
		return true;
	}
	int deplayer = 1 - side;
	int i,j,k,tx, ty;
	int emptyPN=0,wnPN=0;
	for(j=0;j<9;j++)
	{
		for(i=0;i<9;i++)
		{
			if(board[i][j]==deplayer)
			{
				if(GetQi(i,j,deplayer)==0)//使对方子无气
				{
					PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)1 - side);
					return true;
				}
			}
			else if(board[i][j]==EMPTY)
			{
				emptyPN++;
				board[i][j]=deplayer;
				if (GetQi(i, j, deplayer) == 0)//对方若落子将自杀
				{
					wnPN++;
					board[i][j] = EMPTY;
					continue;
				}
				for (k = 0; k < 4; k++)
				{
					tx = i + lineVector[0][0];
					ty = j + lineVector[0][1];
					if (tx >= 0 && tx < 9 && ty >= 0 && ty < 9 && board[tx][ty] == side&&GetQi(tx, ty, side) == 0)
					{
						wnPN++;
						break;
					}
				}
				board[i][j] = EMPTY;
			}
		}
	}
	if(emptyPN==wnPN)//对方没有可正常落子点
	{
		PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)side);
		return true;
	}
	return false;	
}

bool CNoGo::FitRules()//是否符合规则
{	
	Step sp = stepStack.top();
	if(sp.point.x<0 ||sp.point.x>8 ||sp.point.y<0 ||sp.point.y>8)
	{
		return false;
	}
	if (board[sp.point.x][sp.point.y] != EMPTY)
	{
		return false;
	}
	return true;
}
