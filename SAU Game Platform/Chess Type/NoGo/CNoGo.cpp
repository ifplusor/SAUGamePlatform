#include "CNoGo.h"


const int lineVector[4][2] = { 0, 1, 0, -1, -1, 0, 1, 0 };


CNoGo::CNoGo(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);
	hPen = NULL;
	hFont = NULL;

	//获取配置
	GetConfig();

	//创建兼容DC
	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hMarkDC = CreateCompatibleDC(hDC);

	InitGame();
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

/**
 * SetBoard - 设置棋盘参数
 * @rtBoard:	棋盘在窗口客户区的位置
 */
VOID CNoGo::SetBoard(RECT rtBoard)
{
	//设置棋盘参数
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left;
	d = side / 10;

	//设置刻线画笔
	if (hPen != NULL)
		DeleteObject(hPen);
	pWidth = (int)(side / 300);
	if (pWidth == 0)
		pWidth = 1;
	hPen = CreatePen(PS_SOLID, pWidth, RGB(0, 0, 0));

	//设置刻度字体
	if (hFont != NULL)
		DeleteObject(hFont);
	fWidth = (int)(d / 3);
	fHeight = (int)(d * 2 / 3);
	hFont = CreateSimpleFont(fWidth, fHeight);

	//绘制棋子元素
	DrawChess();
}

/**
 * DrawBoard - 绘制棋盘
 * @hDC:	主窗口DC
 */
VOID CNoGo::DrawBoard(HDC hDC)
{
	int i, j;
	HPEN hOldPen;
	HFONT hOldFont;

	//设置刻线画笔
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	//设置刻度字体、颜色
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2] = { 0 }, number[3] = { 0 };

	//绘制棋盘
	for (i = 1; i <= 9; i++)//9纬线
	{
		//绘制刻线
		MoveToEx(hDC, rtBoard.left + side / 10, rtBoard.top + side*i / 10, NULL);
		LineTo(hDC, rtBoard.left + side * 9 / 10, rtBoard.top + side*i / 10);
		//绘制刻度
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + side / 20 - fWidth / 2, rtBoard.top + side*i / 10 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side * 19 / 20 - fWidth / 2, rtBoard.top + side*i / 10 - fHeight / 2, number, 2);
	}
	for (i = 1; i <= 9; i++)//9经线
	{
		//绘制刻线
		MoveToEx(hDC, rtBoard.left + side*i / 10, rtBoard.top + side / 10, NULL);
		LineTo(hDC, rtBoard.left + side*i / 10, rtBoard.top + side * 9 / 10);
		//绘制刻度
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + side*i / 10 - fWidth / 2, rtBoard.top + side / 20 - fHeight / 2, letter, 1);			
		TextOut(hDC, rtBoard.left + side*i / 10 - fWidth / 2, rtBoard.top + side * 19 / 20 - fHeight / 2, number, 2);
	}

	//绘制棋子
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (board[i][j] == BLACK)
			{//rtBoard.left+d*(i+1)-d/2=rtBoard.left+(i+1)*side/10-side/20=rtBoard.left+(2*i+1)*side/20
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hBlcDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hBlcDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hWhtDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 20, rtBoard.top + side*(j * 2 + 1) / 20, d, d, hWhtDC, 0, 0, SRCPAINT);
			}
		}
	}

	//绘制提示标记
	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		int x = curStep.point.x;
		int y = curStep.point.y;
		BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 20, rtBoard.top + side*(y * 2 + 1) / 20, d, d, hMarkDC, d, 0, SRCAND);
		BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 20, rtBoard.top + side*(y * 2 + 1) / 20, d, d, hMarkDC, 0, 0, SRCPAINT);
	}

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldFont);
}

/**
 * DrawChess - 绘制棋子元素
 */
VOID CNoGo::DrawChess()
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
}

/**
 * PlaySnd - 播放音效
 * @tag:	音效标签
 */
bool CNoGo::PlaySnd(int tag)
{
	char filename[MAX_PATH] = { 0 };

	switch (tag)
	{
	case 0://落子音效
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\落子.wav");
		break;
	default:
		break;
	}

	//播放音效
	if (!PlaySound(filename, NULL, SND_NOWAIT | SND_FILENAME))
	{
		ErrorBox("PlaySound failed");
		return false;
	}

	return true;
}

/**
 * InitGame - 游戏初始化
 */
VOID CNoGo::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;
	count = 0;
	CleanStack(stepStack);//清空着法栈
	InitBoard();//初始化棋盘
}

/**
 * InitBoard - 初始化棋盘
 */
VOID CNoGo::InitBoard()
{
	int i, j;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			board[i][j] = EMPTY;
		}
	}

	//刷新棋盘
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);
}

/**
 * ProcessMove - 处理引擎消息
 * @moveCmd:	着法信息
 */
BOOL CNoGo::ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{
	Step tStep;
	char *res;
	int pos = 0;
	int len = strlen("move ");

	curCmd[0] = denCmd[0] = '\0';//默认空命令

	if ((res = strstr(moveCmd, "move")) == NULL)//寻找move关键字
	{
		return 0;//未找到“move”关键字
	}
	else
	{
		pos = (res - moveCmd);
		pos += len;

		//解析着法
		tStep.point.x = moveCmd[pos] - BX;
		tStep.point.y = moveCmd[pos + 1] - BY;
		tStep.side = player;
		stepStack.push(tStep);//完整着法压栈

		//判断是否符合规则
		if (!FitRules())
		{
			strcat(curCmd, "error\n");
			stepStack.pop();//非法着法出栈
			return -1;//行棋违规
		}

		//落子
		board[tStep.point.x][tStep.point.y] = tStep.side;

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放落子音效
		PlaySnd(0);

		//追加着法历史
		ShowStepHis(moveCmd + pos);

		//生成命令串
		sprintf(denCmd, "move %c%c\n", tStep.point.x + BX, tStep.point.y + BY);

		//累计步数
		StepNum[player]++;

		//判断胜负
		if (WinOrLose())
		{
			strcat(denCmd, "end\n");//追加终盘命令
			strcat(curCmd, "end\n");
			return 2;//分出胜负
		}

		//行棋换手
		player = NEXTPLAYER(player);

		return 1;//获取成功
	}
	return 0;
}

/**
 * OnLButtonDown - 响应鼠标点击棋盘输入着法
 * @x:	指针横坐标
 * @y:	指针纵坐标
 * return:	返回着法进行状态，-1表示输入错误，0表示输入进行中，1表示输入结束
 */
BOOL CNoGo::OnLButtonDown(int x, int y)
{
	Point point;

	if (count == -1)//处于屏蔽输入状态，count=-1时return0可能造成意外
		return 0;

	//把窗口坐标映射为棋盘坐标
	point.x = ((x - rtBoard.left) * 10 - side / 2) / side;
	point.y = ((y - rtBoard.top) * 10 - side / 2) / side;
	if (point.x < 0 || point.x >= 9 || point.y < 0 || point.y >= 9)//非有效落点
		return 2;

	return SToS(point);
}

/**
 * SToS - 手动逻辑处理
 */
BOOL CNoGo::SToS(Point point)
{
	Step tStep;

	//检查着法合法性
	if (board[point.x][point.y] != EMPTY)//非空落点
		return -1;//着法非法

	//落子
	board[point.x][point.y] = player;

	//填充临时着法
	tStep.side = player;//填充棋手
	tStep.point = point;//填充落点
	stepStack.push(tStep);//临时着法压栈

	count = -1;//输入完成

	//刷新棋盘
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);
	//播放落子音效
	PlaySnd(0);

	return 1;//着法成立
}

/**
 * OkMove - 确认着法
 */
INT CNoGo::OkMove(char *denCmd)
{
	Step tStep;

	denCmd[0] = '\0';//默认空命令

	if (count == -1)
	{
		tStep = stepStack.top();//获取完整着法

		//生成命令串
		sprintf(denCmd, "move %c%c", tStep.point.x + BX, tStep.point.y + BY);

		//追加着法历史
		ShowStepHis(denCmd + 5);

		//累计步数
		StepNum[player]++;

		//判断胜负
		if (WinOrLose())
		{
			strcat(denCmd, "\nend\n");//追加终盘命令
			return 2;//分出胜负
		}

		//补全命令串
		strcat(denCmd, "\n");

		//行棋换手
		player = NEXTPLAYER(player);

		count = 0;//重置输入

		return 1;//着法成立
	}

	return 0;
}

/**
 * CancelMove - 取消着法
 */
INT CNoGo::CancelMove()
{
	Step tStep;

	if (count == 0)//无输入，无意义
		return 0;

	tStep = stepStack.top();//获取着法
	stepStack.pop();//着法出栈

	//恢复落子
	board[tStep.point.x][tStep.point.y] = EMPTY;

	//刷新棋盘
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);

	count = 0;//重置输入

	return 1;
}

/**
 * FitRules - 规则测试
 */
bool CNoGo::FitRules()
{
	Step tStep = stepStack.top();//获取着法

	if (tStep.point.x < 0 || tStep.point.x > 8 || tStep.point.y < 0 || tStep.point.y > 8)
	{
		return false;
	}
	if (board[tStep.point.x][tStep.point.y] != EMPTY)//只要是空点就可以落子
	{
		return false;
	}
	return true;
}

/**
 * WinOrLose - 终盘测试
 */
bool CNoGo::WinOrLose()
{
	Step tStep = stepStack.top();
	BYTE side = tStep.side;
	int deplayer = 1 - side;

	//判断自杀
	if (GetQi(tStep.point.x, tStep.point.y, side) == 0)
	{
		PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)deplayer);
		return true;
	}

	int i, j, k, tx, ty;
	int emptyPN = 0, wnPN = 0;
	for (j = 0; j < 9; j++)
	{
		for (i = 0; i < 9; i++)
		{
			if (board[i][j] == deplayer)
			{
				if (GetQi(i, j, deplayer) == 0)//使对方子无气
				{
					PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)deplayer);
					return true;
				}
			}
			else if (board[i][j] == EMPTY)
			{
				emptyPN++;
				board[i][j] = deplayer;
				if (GetQi(i, j, deplayer) == 0)//对方若落子将自杀
				{
					wnPN++;
					board[i][j] = EMPTY;
					continue;
				}
				for (k = 0; k < 4; k++)
				{
					tx = i + lineVector[k][0];
					ty = j + lineVector[k][1];
					if (tx >= 0 && tx < 9 && ty >= 0 && ty < 9 && board[tx][ty] == side && GetQi(tx, ty, side) == 0)//对方若落子将使本方无气
					{
						wnPN++;
						break;
					}
				}
				board[i][j] = EMPTY;
			}
		}
	}
	if (emptyPN == wnPN)//对方没有可正常落子点
	{
		PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)side);
		return true;
	}
	return false;
}

/**
 * GetQi - 获取棋子气数
 * @x:	棋子x坐标
 * @y:	棋子y坐标
 * @side:	棋子颜色
 */
int CNoGo::GetQi(int x, int y, int side)
{
	for (int k = 0; k < 81; k++)
		Biaoji[k] = -1;
	return GetQiForPoint(x, y, side);
}

int CNoGo::GetQiForPoint(int a, int b, int side)
{
	int x, y, i, qs = 0;
	Biaoji[a * 9 + b] = MARK;
	for (i = 0; i < 4; i++)
	{
		x = a + lineVector[i][0];
		y = b + lineVector[i][1];
		if (x < 0 || y < 0 || x >= 9 || y >= 9)//超出棋盘边界
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
