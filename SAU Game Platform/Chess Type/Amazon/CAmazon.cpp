#pragma comment(lib,"winmm.lib")

#include "CAmazon.h"
#include <stdio.h>


VOID __cdecl ErrorBox(LPTSTR ErrorInfo)//错误提示框
{
	CHAR error1[50], error2[20];
	strcpy(error1, ErrorInfo);
	sprintf(error2, "\n\nerror: %d", GetLastError());
	strcat(error1, error2);
	MessageBox(NULL, error1, "error", MB_OK);
}

CAmazon::CAmazon(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);

	BkColor = RGB(0, 0, 0);
	BoardColor = RGB(128, 128, 128);
	hPen = NULL;
	hFont = NULL;
	InitGame();
	count = -1;

	hBrush = CreateSolidBrush(RGB(115, 74, 18));

	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hBarDC = CreateCompatibleDC(hDC);
	hMarkDC = CreateCompatibleDC(hDC);
}

CAmazon::~CAmazon()
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	DeleteObject(hBrush);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hBarDC);
	DeleteDC(hMarkDC);
}

VOID CAmazon::SetBoard(RECT rtBoard)
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left + 1;
	d = side / 12;
	pixel = side / 300;
	if (pixel == 0)
		pixel = 1;
	hPen = CreatePen(PS_SOLID, pixel, RGB(0, 0, 0));
	fWidth = d / 3;
	fHeight = d * 2 / 3;
	hFont = CreateSimpleFont(fWidth, fHeight);
	DrawChess();
}

VOID CAmazon::DrawBoard(HDC hDC)
{
	int i, j;
	RECT rect;
	HPEN hOldPen;
	HFONT hOldFont;
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldFont = (HFONT)SelectObject(hDC, hFont);

	::SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2], number[2];
	memset(letter, 0, sizeof(letter));
	memset(number, 0, sizeof(number));
	for (i = 1; i <= 10; i++)
	{
		letter[0] = 'A' + i - 1;
		itoa(i - 1, number, 10);

		TextOut(hDC, rtBoard.left + side*(i * 2 + 1) / 24 - fWidth / 2, rtBoard.top + side / 24 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side*(i * 2 + 1) / 24 - fWidth / 2, rtBoard.top + side* 23 / 24 - fHeight / 2, number, 1);
	}
	for (i = 1; i <= 10; i++)
	{
		letter[0] = 'A' + i - 1;
		itoa(i - 1, number, 10);

		TextOut(hDC, rtBoard.left + side / 24 - fWidth / 2, rtBoard.top + side*(i * 2 + 1) / 24 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side * 23 / 24 - fWidth / 2, rtBoard.top + side*(i * 2 + 1) / 24 - fHeight / 2, number, 1);
	}

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
			{
				rect.left = rtBoard.left + side*(j + 1)/12;
				rect.top = rtBoard.top + side*(i + 1)/12;
				rect.right = rtBoard.left + side*(j + 2) / 12;
				rect.bottom = rtBoard.top + side*(i + 2) / 12;
				FillRect(hDC, &rect, hBrush);
			}
		}
	}
	MoveToEx(hDC, rtBoard.left + side / 12, rtBoard.top + side / 12, NULL);
	LineTo(hDC, rtBoard.left + side * 11 / 12-1 , rtBoard.top + side / 12);
	LineTo(hDC, rtBoard.left + side * 11 / 12-1 , rtBoard.top + side * 11 / 12-1 );
	LineTo(hDC, rtBoard.left + side / 12, rtBoard.top + side * 11 / 12-1 );
	LineTo(hDC, rtBoard.left + side / 12, rtBoard.top + side / 12);

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (board[i][j] == BLACK)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hBlcDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hBlcDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hWhtDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hWhtDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == BAR)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hBarDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 12, rtBoard.top + side*(j + 1) / 12, d, d, hBarDC, 0, 0, SRCPAINT);
			}
		}
	}

	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		IsChess(curStep.second)
		{
			int x = curStep.second.x; int y = curStep.second.y;
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
		else IsChess(curStep.first)
		{
			int x = curStep.first.x; int y = curStep.first.y;
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
	}

}

bool CAmazon::DrawChess()
{
	char filename[MAX_PATH] = { 0 };
	HBITMAP hBoardBmp, hBlcBmp, hWhtBmp, hBarBmp, hMarkBmp;

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\black.bmp");
	hBlcBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\white.bmp");
	hWhtBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\bar.bmp");
	hBarBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\mark.bmp");
	hMarkBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, d * 2, d, LR_LOADFROMFILE);

	SelectObject(hBlcDC, hBlcBmp);
	SelectObject(hWhtDC, hWhtBmp);
	SelectObject(hBarDC, hBarBmp);
	SelectObject(hMarkDC, hMarkBmp);

	DeleteObject(hBlcBmp);
	DeleteObject(hWhtBmp);
	DeleteObject(hBarBmp);
	DeleteObject(hMarkBmp);
	return true;
}

BOOL CAmazon::ProcessMove(char *moveCmd)
{
	Step tStep;
	char *res;
	int pos = 0;
	int len = strlen("move ");
	curCmd[0] = denCmd[0] = '\0';//默认空消息
	if ((res = strstr(moveCmd, "move")) == NULL)//寻找move关键字
	{
		return 0;
	}
	else
	{
		pos = (res - moveCmd);
		pos += len;

		tStep.first.x = moveCmd[pos] - 'A';
		tStep.first.y = moveCmd[pos + 1] - 'A';
		tStep.second.x = moveCmd[pos + 2] - 'A';
		tStep.second.y = moveCmd[pos + 3] - 'A';
		tStep.third.x = moveCmd[pos + 4] - 'A';
		tStep.third.y = moveCmd[pos + 5] - 'A';
		tStep.side = player;
		stepStack.push(tStep);

		if (!FitRules())//判断是否符合规则
		{
			sprintf(curCmd, "error\n");
			stepStack.pop();//行棋违规，弹出栈顶着法
			return -1;
		}
		board[tStep.first.x][tStep.first.y] = EMPTY;
		board[tStep.second.x][tStep.second.y] = player;
		board[tStep.third.x][tStep.third.y] = BAR;
		InvalidateRect(hWnd, &rtBoard, FALSE);
		PlaySnd(2);
		moveCmd[pos + 6] = '\0';
		ShowStepHis(moveCmd + pos);

		sprintf(denCmd, "move %c%c%c%c%c%c\n", tStep.first.x + 'A', tStep.first.y + 'A', tStep.second.x + 'A', tStep.second.y + 'A', tStep.third.x + 'A', tStep.third.y + 'A');//生成写消息
	}
	StepNum[player]++;//步数加1
	if (WinOrLose())//判断胜负
	{
		sprintf(denCmd + strlen(denCmd), "end\n");
		sprintf(curCmd, "end\n");
		return 2;
	}
	player = NEXTPLAYER(player);
	return 1;
}

bool CAmazon::PlaySnd(int sel)//播放音效
{
	char filename[MAX_PATH] = { 0 };
	switch (sel)
	{
	case 0:
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\选子.wav");
		break;
	case 1:
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\落子.wav");
		break;
	case 2:
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\设置障碍.wav");
		break;
	default:
		break;
	}
	if (!PlaySound(filename, NULL, SND_NOWAIT | SND_FILENAME))
	{
		ErrorBox("PlaySound failed");
		return false;
	}
	return true;
}

VOID CAmazon::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));

	player = BLACK;//黑方先走	
	InitBoard();
	count = 0;

	CleanStack(stepStack);
	return;
}

VOID CAmazon::InitBoard()
{
	int i, j;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			board[i][j] = EMPTY;
		}
	}
	board[0][3] = WHITE; board[0][6] = BLACK;
	board[3][0] = WHITE; board[3][9] = BLACK;
	board[6][0] = WHITE; board[6][9] = BLACK;
	board[9][3] = WHITE; board[9][6] = BLACK;

	InvalidateRect(hWnd, &rtBoard, FALSE);
	return;
}

BOOL CAmazon::OnLButtonDown(int x, int y)
{
	Point point;
	if (!InsideRect(&rtBoard, x, y))
		return 2;
	if (count == -1)
		return 0;
	point.x = (x - rtBoard.left) * 12 / side - 1;
	point.y = (y - rtBoard.top) * 12 / side - 1;
	if (point.x < 0 || point.x >= 10 || point.y < 0 || point.y >= 10)
		return 2;
	return SToS(point);
}

BOOL CAmazon::SToS(Point point)
{
	Step tStep;

	if (count == 0)
	{
		if (board[point.x][point.y] != player)//选中本方棋子
			return -1;
		InitStep(tStep);
		tStep.first = point;
		tStep.side = player;
		stepStack.push(tStep);
		count = 1;
		::InvalidateRect(hWnd, &rtBoard, FALSE);
		PlaySnd(0);
		return 0;
	}
	else if (count == 1)
	{
		tStep = stepStack.top();
		if (board[point.x][point.y] == player)//更换选中棋子
		{
			stepStack.pop();
			tStep.first = point;
			stepStack.push(tStep);
			::InvalidateRect(hWnd, &rtBoard, FALSE);
			PlaySnd(0);
			return 0;
		}
		else if (board[point.x][point.y] == NEXTPLAYER(player) || !JudgeRule(tStep.first, point))//着法错误
		{
			return -1;
		}
		stepStack.pop();
		board[tStep.first.x][tStep.first.y] = EMPTY;
		board[point.x][point.y] = player;
		tStep.second = point;
		stepStack.push(tStep);
		count = 2;
		::InvalidateRect(hWnd, &rtBoard, FALSE);
		PlaySnd(1);
		return 0;
	}
	else if (count == 2)
	{
		tStep = stepStack.top();
		if (board[point.x][point.y] != EMPTY || !JudgeRule(tStep.second, point))
			return -1;
		stepStack.pop();
		board[point.x][point.y] = BAR;
		tStep.third = point;
		stepStack.push(tStep);
		count = -1;
		::InvalidateRect(hWnd, &rtBoard, FALSE);
		PlaySnd(2);
		return 1;
	}
	return 0;
}

INT CAmazon::OkMove()
{
	Step tStep;
	tStep = stepStack.top();
	sprintf(denCmd, "move %c%c%c%c%c%c", tStep.first.x + 'A', tStep.first.y + 'A', tStep.second.x + 'A', tStep.second.y + 'A', tStep.third.x + 'A', tStep.third.y + 'A');
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

VOID CAmazon::CancelMove()
{
	Step tStep;
	if (count == 0)
		return;
	tStep = stepStack.top();
	stepStack.pop();
	IsChess(tStep.third)
	{
		board[tStep.third.x][tStep.third.y] = EMPTY;
	}
	IsChess(tStep.second)
	{
		board[tStep.second.x][tStep.second.y] = EMPTY;
	}
	IsChess(tStep.first)
	{
		board[tStep.first.x][tStep.first.y] = tStep.side;
	}
	InvalidateRect(hWnd, &rtBoard, FALSE);
	SendMessage(hWnd, WM_PAINT, NULL, NULL);
	count = 0;
	return;
}

bool CAmazon::WinOrLose()
{
	int blc, wht;
	blc = wht = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (board[i][j] == BLACK)
			{
				if (
					((0 < i &&j > 0 && board[i - 1][j - 1] != EMPTY) || (i == 0 || j == 0))			//左上角点不为空
					&& ((0 < i &&board[i - 1][j] != EMPTY) || i == 0)								//左侧点不为空
					&& ((0 < i &&j < 9 && board[i - 1][j + 1] != EMPTY) || (i == 0 || j == 9))		//左下角点不为空
					&& ((j>0 && board[i][j - 1] != EMPTY) || j == 0)								//上方点不为空
					&& ((j < 9 && board[i][j + 1] != EMPTY) || j == 9)								//下方点不为空
					&& ((i < 9 && j>0 && board[i + 1][j - 1] != EMPTY) || (i == 9 || j == 0))		//右上角点不为空
					&& ((i < 9 && board[i + 1][j] != EMPTY) || (i == 9))							//右侧点不为空
					&& ((i < 9 && j < 9 && board[i + 1][j + 1] != EMPTY) || (i == 9 || j == 9))		//游侠角点不为空
					)
				{
					blc++;//黑方棋子满足无法走子，则计数
				}
			}
			if (board[i][j] == WHITE)
			{
				if (
					((0 < i &&j > 0 && board[i - 1][j - 1] != EMPTY) || (i == 0 || j == 0))
					&& ((0 < i &&board[i - 1][j] != EMPTY) || i == 0)
					&& ((0 < i &&j < 9 && board[i - 1][j + 1] != EMPTY) || (i == 0 || j == 9))
					&& ((j>0 && board[i][j - 1] != EMPTY) || j == 0)
					&& ((j < 9 && board[i][j + 1] != EMPTY) || j == 9)
					&& ((i < 9 && j>0 && board[i + 1][j - 1] != EMPTY) || (i == 9 || j == 0))
					&& ((i < 9 && board[i + 1][j] != EMPTY) || (i == 9))
					&& ((i < 9 && j < 9 && board[i + 1][j + 1] != EMPTY) || (i == 9 || j == 9))
					)
				{
					wht++;//白方棋子满足无法走子，则计数
				}
			}
		}
	}
	if (wht == 4 || blc == 4)//白方无路可走则黑赢,黑方无路可走则白赢
	{
		SendMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)(wht == 4 ? BLACK : WHITE));
		return true;
	}
	return false;
}

//判断跳子
bool CAmazon::JudgeRule(Point src, Point des)
{
	int tx, ty;
	if (!(src.x == des.x || src.y == des.y || abs(des.x - src.x) == abs(des.y - src.y)))//走子方向
	{
		return false;
	}
	tx = src.x;
	ty = src.y;
	if (src.x == des.x)
	{
		if (des.y > src.y)
		{
			for (ty++; ty <= des.y; ty++)
				if (board[tx][ty] != EMPTY)
					return false;
		}
		else if (des.y < src.y)
		{
			for (ty--; ty >= des.y; ty--)
				if (board[tx][ty] != EMPTY)
					return false;
		}
	}
	else if (des.x > src.x)
	{
		if (src.y == des.y)
		{
			for (tx++; tx <= des.x; tx++)
				if (board[tx][ty] != EMPTY)
					return false;
		}
		else if (des.y > src.y)
		{
			for (tx++, ty++; tx <= des.x; tx++, ty++)
			{
				if (board[tx][ty] != EMPTY)
					return false;
			}
		}
		else
		{
			for (tx++, ty--; tx <= des.x; tx++, ty--)
			{
				if (board[tx][ty] != EMPTY)
					return false;
			}
		}
	}
	else
	{
		if (des.y == src.y)
		{
			for (tx--; tx >= des.x; tx--)
				if (board[tx][ty] != EMPTY)
					return false;
		}
		else if (des.y > src.y)
		{
			for (tx--, ty++; tx >= des.x; tx--, ty++)
			{
				if (board[tx][ty] != EMPTY)
					return false;
			}
		}
		else
		{
			for (tx--, ty--; tx >= des.x; tx--, ty--)
			{
				if (board[tx][ty] != EMPTY)
					return false;
			}
		}
	}
	return true;
}

bool CAmazon::FitRules()
{
	Step tStep = stepStack.top();
	if (tStep.first.x < 0 || tStep.first.x>9 || tStep.first.y < 0 || tStep.first.y>9)
		return false;
	if (tStep.second.x < 0 || tStep.second.x>9 || tStep.second.y < 0 || tStep.second.y>9)
		return false;
	if (tStep.third.x < 0 || tStep.third.x>9 || tStep.third.y < 0 || tStep.third.y>9)
		return false;
	if (board[tStep.first.x][tStep.first.y] != tStep.side || board[tStep.second.x][tStep.second.y] != EMPTY						//起点有子，落点无子
		|| (tStep.second.x == tStep.third.x && tStep.second.y == tStep.third.y)													//落点和障碍点不为同一点
		|| ((tStep.third.x != tStep.first.x || tStep.third.y != tStep.first.y) && board[tStep.third.x][tStep.third.y] != EMPTY))	//障碍点非起点时，障碍点无子
		return false;
	if (!JudgeRule(tStep.first, tStep.second))
		return false;
	if (!JudgeRule(tStep.second, tStep.third))
		return false;
	return true;
}
