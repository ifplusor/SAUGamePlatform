#include "CConSix.h"


const int lineVector[4][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 } };//方向向量


CConSix::CConSix(HINSTANCE hInst, HWND hWnd, char *LibPath)
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
}

CConSix::~CConSix()
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
VOID CConSix::SetBoard(RECT rtBoard)
{
	//设置棋盘参数
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left;
	d = side / 20;

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
VOID CConSix::DrawBoard(HDC hDC)
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
	for (i = 1; i <= 19; i++)//19纬线
	{
		//绘制刻线
		MoveToEx(hDC, rtBoard.left + side / 20, rtBoard.top + side*i / 20, NULL);
		LineTo(hDC, rtBoard.left + side * 19 / 20, rtBoard.top + side*i / 20);
		//绘制刻度
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + fWidth, rtBoard.top + side*i / 20 - fHeight / 2, letter, 1);
		if (i < 10)
			TextOut(hDC, rtBoard.left + side * 19 / 20 + fWidth * 3 / 2, rtBoard.top + side*i / 20 - fHeight / 2, number, 2);
		else
			TextOut(hDC, rtBoard.left + side * 19 / 20 + fWidth, rtBoard.top + side*i / 20 - fHeight / 2, number, 2);
	}
	for (i = 1; i <= 19; i++)//19经线
	{
		//绘制刻线
		MoveToEx(hDC, rtBoard.left + side*i / 20, rtBoard.top + side / 20, NULL);
		LineTo(hDC, rtBoard.left + side*i / 20, rtBoard.top + side * 19 / 20);
		//绘制刻度
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + side*i / 20 - fWidth / 2, rtBoard.top + side / 40 - fHeight / 2, letter, 1);
		if (i < 10)
			TextOut(hDC, rtBoard.left + side*i / 20 - fWidth / 2, rtBoard.top + side * 39 / 40 - fHeight / 2, number, 1);
		else
			TextOut(hDC, rtBoard.left + side*i / 20 - fWidth, rtBoard.top + side * 39 / 40 - fHeight / 2, number, 2);
	}

	//绘制棋子
	for (i = 0; i < 19; i++)
	{
		for (j = 0; j < 19; j++)
		{
			if (board[i][j] == BLACK)
			{//rtBoard.left+d*(i+1)-d/2=rtBoard.left+(i+1)*side/20-side/40=rtBoard.left+(2*i+1)*side/40
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 40, rtBoard.top + side*(j * 2 + 1) / 40, d, d, hBlcDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 40, rtBoard.top + side*(j * 2 + 1) / 40, d, d, hBlcDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 40, rtBoard.top + side*(j * 2 + 1) / 40, d, d, hWhtDC, d, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i * 2 + 1) / 40, rtBoard.top + side*(j * 2 + 1) / 40, d, d, hWhtDC, 0, 0, SRCPAINT);
			}
		}
	}

	//绘制提示标记
	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		int x, y;
		IsChess(curStep.first)
		{
			x = curStep.first.x;
			y = curStep.first.y;
			BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 40, rtBoard.top + side*(y * 2 + 1) / 40, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 40, rtBoard.top + side*(y * 2 + 1) / 40, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
		IsChess(curStep.second)
		{
			x = curStep.second.x;
			y = curStep.second.y;
			BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 40, rtBoard.top + side*(y * 2 + 1) / 40, d, d, hMarkDC, d, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(x * 2 + 1) / 40, rtBoard.top + side*(y * 2 + 1) / 40, d, d, hMarkDC, 0, 0, SRCPAINT);
		}
	}

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldFont);
}

/**
 * DrawChess - 绘制棋子元素
 */
VOID CConSix::DrawChess()
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
bool CConSix::PlaySnd(int tag)
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
	if (!PlaySound(filename, NULL, SND_ASYNC | SND_NOWAIT | SND_FILENAME))
	{
		ErrorBox("PlaySound failed");
		return false;
	}

	return true;
}

/**
 * InitGame - 游戏初始化
 */
VOID CConSix::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;
	first_hand = true;
	count = 0;
	CleanStack(stepStack);//清空着法栈
	InitBoard();//初始化棋盘
}

/**
 * InitBoard - 初始化棋盘
 */
VOID CConSix::InitBoard()
{
	int i, j;
	for (i = 0; i < 19; i++)
	{
		for (j = 0; j < 19; j++)
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
BOOL CConSix::ProcessMove(char *moveCmd)
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
		tStep.side = player;
		stepStack.push(tStep);//完整着法压栈

		//判断是否符合规则
		if (!FitRules())
		{
			strcat(curCmd, "error\n");
			stepStack.pop();//非法着法出栈
			return -1;//行棋违规
		}

		//落第一子
		board[tStep.first.x][tStep.first.y] = tStep.side;

		if (first_hand == true)//第一手
		{
			first_hand = false;
		}
		else
		{
			//落第二子
			if (tStep.second.x != -1 && tStep.second.y != -1)
				board[tStep.second.x][tStep.second.y] = tStep.side;
		}

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放落子音效
		PlaySnd(0);

		//追加着法历史
		ShowStepHis(moveCmd + pos);

		//生成命令串
		sprintf(denCmd, "move %c%c%c%c\n", tStep.first.x + BX, tStep.first.y + BY, tStep.second.x + BX, tStep.second.y + BY);

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
BOOL CConSix::OnLButtonDown(int x, int y)
{
	Point point;

	if (count == -1)//处于屏蔽输入状态
		return 0;

	//把窗口坐标映射为棋盘坐标
	point.x = ((x - rtBoard.left) * 20 - side / 2) / side;
	point.y = ((y - rtBoard.top) * 20 - side / 2) / side;
	if (point.x < 0 || point.x >= 19 || point.y < 0 || point.y >= 19)//非有效落点
		return 2;

	return SToS(point);
}

/**
 * SToS - 手动逻辑处理
 */
BOOL CConSix::SToS(Point point)
{
	Step tStep;

	//检查着法合法性
	if (board[point.x][point.y] != EMPTY)//非空落点
		return -1;//着法非法

	//落子
	board[point.x][point.y] = player;

	if (count == 0)//落第一子
	{
		//填充临时着法
		InitStep(tStep);//初始化着法
		tStep.side = player;//填充棋手
		tStep.first = point;//填充第一子落点
		stepStack.push(tStep);//临时着法压栈

		if (first_hand)//第一手
		{
			count = -1;//输入完成

			//刷新棋盘
			InvalidateRect(hWnd, &rtBoard, FALSE);
			UpdateWindow(hWnd);
			//播放落子音效
			PlaySnd(0);

			return 1;//着法成立
		}
		else
		{
			count = 1;

			//刷新棋盘
			InvalidateRect(hWnd, &rtBoard, FALSE);
			UpdateWindow(hWnd);
			//播放音效
			PlaySnd(0);

			//识别落一子获胜着法
			if (WinOrLose())
			{
				count = -1;//输入结束
				return 1;//着法成立
			}

			return 0;//着法进行
		}
	}
	else if (count == 1)//落第二子
	{
		//填充临时着法
		tStep = stepStack.top();//获取临时着法
		stepStack.pop();//临时着法出栈
		tStep.second = point;//填充第二子落点
		stepStack.push(tStep);//完整着法压栈

		count = -1;//输入完成

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放落子音效
		PlaySnd(0);

		return 1;//着法成立
	}

	return 0;
}

/**
 * OkMove - 确认着法
 */
INT CConSix::OkMove()
{
	Step tStep;

	denCmd[0] = '\0';//默认空命令

	if (count == -1)//输入完成
	{
		tStep = stepStack.top();//获取完整着法

		//生成命令串
		sprintf(denCmd, "move %c%c%c%c", tStep.first.x + BX, tStep.first.y + BY, tStep.second.x + BX, tStep.second.y + BY);

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

		//取消第一手标志
		first_hand = false;

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
INT CConSix::CancelMove()
{
	Step tStep;

	if (count == 0)//无输入，无意义
		return 0;

	tStep = stepStack.top();//获取着法
	stepStack.pop();//着法出栈

	//恢复第二子
	IsChess(tStep.second)
	{
		board[tStep.second.x][tStep.second.y] = EMPTY;
	}

	//恢复第一子
	IsChess(tStep.first)
	{
		board[tStep.first.x][tStep.first.y] = EMPTY;
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
bool CConSix::FitRules()
{
	Step tStep = stepStack.top();//获取着法

	if (tStep.first.x < 0 || tStep.first.x > 18 || tStep.first.y < 0 || tStep.first.y > 18)//第一子不在棋盘内，无效
	{
		return false;
	}
	if (first_hand == true)//第一手
	{
		if (board[tStep.first.x][tStep.first.y] != EMPTY)//第一子不为空，无效
		{
			return false;
		}
	}
	else
	{
		if (tStep.second.x == -1 && tStep.second.y == -1)//第二子无效
		{
			if (board[tStep.first.x][tStep.first.y] != EMPTY)//第一子落点不为空，无效
				return false;
			else
				return true;
		}
		if (tStep.second.x < 0 || tStep.second.x > 18 || tStep.second.y < 0 || tStep.second.y > 18)//第二子不在棋盘内，无效
		{
			return false;
		}
		if (board[tStep.first.x][tStep.first.y] != EMPTY || board[tStep.second.x][tStep.second.y] != EMPTY)//第二子落点不为空，无效
		{
			return false;
		}
	}
	return true;
}

/**
 * WinOrLose - 终盘测试
 */
bool CConSix::WinOrLose()
{
	Step tStep = stepStack.top();//获取着法
	BYTE side = tStep.side;
	Point point[2] = { tStep.first, tStep.second };
	int i, j, tx, ty, connect;

	for (j = 0; j < 2; j++)
	{
		if (point[j].x == -1 || point[j].y == -1)
			break;
		for (i = 0; i < 4; i++)
		{
			connect = 0;
			//按照向量进行移动
			tx = point[j].x - lineVector[i][0];
			ty = point[j].y - lineVector[i][1];
			while (tx >= 0 && tx < 19 && ty >= 0 && ty < 19 && board[tx][ty] == side)
			{
				tx -= lineVector[i][0];
				ty -= lineVector[i][1];
			}
			//向量反向
			tx += lineVector[i][0];
			ty += lineVector[i][1];
			while (tx >= 0 && tx < 19 && ty >= 0 && ty < 19 && board[tx][ty] == side)
			{
				connect++;//计数
				tx += lineVector[i][0];
				ty += lineVector[i][1];
			}
			if (connect >= 6)//连六
			{
				if (count != -1)//count为-1时表示从棋手处获取着法，count为0时表示从引擎处获取着法，count为1时表示模块检查一子获胜
				{
					PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)side);
				}
				return true;
			}
		}
	}
	return false;
}

