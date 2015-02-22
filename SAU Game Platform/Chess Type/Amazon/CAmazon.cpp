#include "CAmazon.h"


CAmazon::CAmazon(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);
	hPen = NULL;
	hFont = NULL;

	//获取配置
	GetConfig();

	//创建暗格画刷
	hBrush = CreateSolidBrush(RGB(115, 74, 18));

	//创建兼容DC
	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hBarDC = CreateCompatibleDC(hDC);
	hMarkDC = CreateCompatibleDC(hDC);

	InitGame();
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

/**
 * SetBoard - 设置棋盘参数
 * @rtBoard:	棋盘在窗口客户区的位置
 */
VOID CAmazon::SetBoard(RECT rtBoard)
{
	//设置棋盘参数
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left + 1;
	d = side / 12;

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
VOID CAmazon::DrawBoard(HDC hDC)
{
	int i, j;
	RECT rect;
	HPEN hOldPen;
	HFONT hOldFont;

	//设置刻线画笔
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	//设置刻度字体、颜色
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2] = { 0 }, number[3] = { 0 };

	//绘制棋盘暗格
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
			{
				rect.left = rtBoard.left + side*(j + 1) / 12;
				rect.top = rtBoard.top + side*(i + 1) / 12;
				rect.right = rtBoard.left + side*(j + 2) / 12;
				rect.bottom = rtBoard.top + side*(i + 2) / 12;
				FillRect(hDC, &rect, hBrush);
			}
		}
	}

	//绘制边框
	MoveToEx(hDC, rtBoard.left + side / 12, rtBoard.top + side / 12, NULL);
	LineTo(hDC, rtBoard.left + side * 11 / 12, rtBoard.top + side / 12);
	LineTo(hDC, rtBoard.left + side * 11 / 12, rtBoard.top + side * 11 / 12);
	LineTo(hDC, rtBoard.left + side / 12, rtBoard.top + side * 11 / 12);
	LineTo(hDC, rtBoard.left + side / 12, rtBoard.top + side / 12);

	//绘制棋盘刻度
	for (i = 1; i <= 10; i++)
	{
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + side / 24 - fWidth / 2, rtBoard.top + side*(i * 2 + 1) / 24 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side*(i * 2 + 1) / 24 - fWidth / 2, rtBoard.top + side / 24 - fHeight / 2, letter, 1);
		if (i < 10)
		{
			TextOut(hDC, rtBoard.left + side * 23 / 24 - fWidth / 2, rtBoard.top + side*(i * 2 + 1) / 24 - fHeight / 2, number, 1);
			TextOut(hDC, rtBoard.left + side*(i * 2 + 1) / 24 - fWidth / 2, rtBoard.top + side * 23 / 24 - fHeight / 2, number, 1);
		}
		else
		{
			TextOut(hDC, rtBoard.left + side * 23 / 24 - fWidth, rtBoard.top + side*(i * 2 + 1) / 24 - fHeight / 2, number, 2);
			TextOut(hDC, rtBoard.left + side*(i * 2 + 1) / 24 - fWidth, rtBoard.top + side * 23 / 24 - fHeight / 2, number, 2);
		}
	}

	//绘制棋子
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

	//绘制提示标记
	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		int x, y;
		IsChess(curStep.second)
		{
			x = curStep.second.x;
			y = curStep.second.y;
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
		else IsChess(curStep.first)
		{
			x = curStep.first.x;
			y = curStep.first.y;
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x + 1) / 12, rtBoard.top + side*(y + 1) / 12, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
	}

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldFont);
}

/**
 * DrawChess - 绘制棋子元素
 */
VOID CAmazon::DrawChess()
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
}

/**
 * PlaySnd - 播放音效
 * @tag:	音效标签
 */
bool CAmazon::PlaySnd(int tag)
{
	char filename[MAX_PATH] = { 0 };

	switch (tag)
	{
	case 0://选子音效
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\选子.wav");
		break;
	case 1://落子音效
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\落子.wav");
		break;
	case 2://设置障碍音效
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\设置障碍.wav");
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
VOID CAmazon::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;//黑方先走	
	count = 0;
	CleanStack(stepStack);//清空着法栈
	InitBoard();//初始化棋盘
}

/**
 * InitBoard - 初始化棋盘
 */
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

	//刷新棋盘
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);
}

/**
 * ProcessMove - 处理引擎消息
 * @moveCmd:	着法信息
 */
BOOL CAmazon::ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
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
		tStep.first.x = moveCmd[pos] - BX;
		tStep.first.y = moveCmd[pos + 1] - BY;
		tStep.second.x = moveCmd[pos + 2] - BX;
		tStep.second.y = moveCmd[pos + 3] - BY;
		tStep.third.x = moveCmd[pos + 4] - BX;
		tStep.third.y = moveCmd[pos + 5] - BY;
		tStep.side = player;
		stepStack.push(tStep);//完整着法压栈

		//判断是否符合规则
		if (!FitRules())
		{
			strcat(curCmd, "error\n");
			stepStack.pop();//非法着法出栈
			return -1;//行棋违规
		}

		//行棋
		board[tStep.first.x][tStep.first.y] = EMPTY;//选子
		board[tStep.second.x][tStep.second.y] = player;//落子
		board[tStep.third.x][tStep.third.y] = BAR;//设置障碍

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放设置障碍音效
		PlaySnd(2);

		//追加着法历史
		ShowStepHis(moveCmd + pos);

		//生成命令串
		sprintf(denCmd, "move %c%c%c%c%c%c\n", tStep.first.x + BX, tStep.first.y + BY, tStep.second.x + BX, tStep.second.y + BY, tStep.third.x + BX, tStep.third.y + BY);

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
BOOL CAmazon::OnLButtonDown(int x, int y)
{
	Point point;

	if (count == -1)//处于屏蔽输入状态
		return 0;

	//把窗口坐标映射为棋盘坐标
	point.x = (x - rtBoard.left) * 12 / side - 1;
	point.y = (y - rtBoard.top) * 12 / side - 1;
	if (point.x < 0 || point.x >= 10 || point.y < 0 || point.y >= 10)
		return 2;

	return SToS(point);
}

/**
 * SToS - 手动逻辑处理
 */
BOOL CAmazon::SToS(Point point)
{
	Step tStep;

	if (count == 0)//选中棋子
	{
		//检查着法合法性
		if (board[point.x][point.y] != player)//未选中本方棋子
			return -1;//着法非法

		//填充临时着法
		InitStep(tStep);//初始化着法
		tStep.side = player;//填充棋手
		tStep.first = point;//填充选子
		stepStack.push(tStep);//临时着法压栈

		count = 1;

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放选子音效
		PlaySnd(0);

		return 0;//着法进行
	}
	else if (count == 1)//落子
	{
		tStep = stepStack.top();//获取临时着法

		if (board[point.x][point.y] == player)//更换选中棋子
		{
			stepStack.pop();//弹出临时着法
			tStep.first = point;//重设选中棋子
			stepStack.push(tStep);//临时着法压栈

			//刷新棋盘
			InvalidateRect(hWnd, &rtBoard, FALSE);
			UpdateWindow(hWnd);
			//播放选子音效
			PlaySnd(0);

			return 0;//着法进行
		}
		else if (board[point.x][point.y] == NEXTPLAYER(player) || !JudgeRule(tStep.first, point))//判断跳子
		{
			return -1;//着法非法
		}

		//走子
		board[tStep.first.x][tStep.first.y] = EMPTY;
		board[point.x][point.y] = player;

		//填充临时着法
		stepStack.pop();//临时着法出栈
		tStep.second = point;//填充棋子落点
		stepStack.push(tStep);//临时着法压栈

		count = 2;

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放落子音效
		PlaySnd(1);

		return 0;//着法进行
	}
	else if (count == 2)//设置障碍
	{
		tStep = stepStack.top();//获取临时着法

		if (board[point.x][point.y] != EMPTY || !JudgeRule(tStep.second, point))//判断跳子
			return -1;//着法非法

		//设置障碍
		board[point.x][point.y] = BAR;

		//填充临时着法
		stepStack.pop();//临时着法出栈
		tStep.third = point;//填充障碍坐标
		stepStack.push(tStep);//完整着法压栈

		count = -1;//输入完成

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放设置障碍音效
		PlaySnd(2);

		return 1;//着法成立
	}

	return 0;
}

/**
 * OkMove - 确认着法
 */
INT CAmazon::OkMove(char *denCmd)
{
	Step tStep;

	denCmd[0] = '\0';//默认空命令

	if (count == -1)//输入完成
	{
		tStep = stepStack.top();//获取完整着法

		//生成命令串
		sprintf(denCmd, "move %c%c%c%c%c%c", tStep.first.x + BX, tStep.first.y + BY, tStep.second.x + BX, tStep.second.y + BY, tStep.third.x + BX, tStep.third.y + BY);
		
		//追加着法历史
		ShowStepHis(denCmd + 5);

		//累计步数
		StepNum[player]++;

		//判断胜负
		if (WinOrLose())
		{
			strcat(denCmd, "\nend\n");//追加终盘命令
			return 2;
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
INT CAmazon::CancelMove()
{
	Step tStep;

	if (count == 0)//无输入，无意义
		return 0;

	tStep = stepStack.top();//获取着法
	stepStack.pop();//着法出栈

	//恢复障碍
	IsChess(tStep.third)
	{
		board[tStep.third.x][tStep.third.y] = EMPTY;
	}

	//恢复落子
	IsChess(tStep.second)
	{
		board[tStep.second.x][tStep.second.y] = EMPTY;
	}

	//恢复走子
	IsChess(tStep.first)
	{
		board[tStep.first.x][tStep.first.y] = tStep.side;
	}

	//刷新棋盘
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);

	count = 0;//重置输入

	return 1;
}

/**
 * FitRules - 规则测试
 */
bool CAmazon::FitRules()
{
	Step tStep = stepStack.top();//获取着法

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
	if (!JudgeRule(tStep.first, tStep.second))//判断跳子
		return false;
	if (!JudgeRule(tStep.second, tStep.third))//判断跳子
		return false;
	return true;
}

/**
 * WinOrLose - 终盘测试
 */
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

/**
 * JudgeRule - 判断跳子
 * @src:	棋子起点
 * @des:	棋子落点
 */
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
