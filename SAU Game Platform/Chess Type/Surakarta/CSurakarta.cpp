#include "CSurakarta.h"


#define PI 3.1415926


/**
 * IsCorner - 判断定点
 */
inline bool IsCorner(const int &x, const int &y)
{
	if ((x == 0 && y == 0) || (x == 0 && y == 5) || (x == 5 && y == 0) || (x == 5 && y == 5))//四个角
		return true;
	else
		return false;
}

//小弧路径
Point SmaWay[28] = { { 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 4 }, { 1, 5 }, { 0, 5 }, { 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 }, { 4, 4 }, { 5, 4 }, { 5, 5 }, { 4, 5 },
{ 4, 4 }, { 4, 3 }, { 4, 2 }, { 4, 1 }, { 4, 0 }, { 5, 0 }, { 5, 1 }, { 4, 1 }, { 3, 1 }, { 2, 1 }, { 1, 1 }, { 0, 1 }, { 0, 0 }, { 1, 0 } };
//大弧路径
Point BigWay[28] = { { 2, 2 }, { 2, 3 }, { 2, 4 }, { 2, 5 }, { 0, 5 }, { 0, 3 }, { 1, 3 }, { 2, 3 }, { 3, 3 }, { 4, 3 }, { 5, 3 }, { 5, 5 }, { 3, 5 }, { 3, 4 },
{ 3, 3 }, { 3, 2 }, { 3, 1 }, { 3, 0 }, { 5, 0 }, { 5, 2 }, { 4, 2 }, { 3, 2 }, { 2, 2 }, { 1, 2 }, { 0, 2 }, { 0, 0 }, { 2, 0 }, { 2, 1 } };

CSurakarta::CSurakarta(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);
	hRPen = NULL;
	hGPen = NULL;
	hBPen = NULL;
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

CSurakarta::~CSurakarta()
{
	if (hRPen != NULL)
		DeleteObject(hRPen);
	if (hGPen != NULL)
		DeleteObject(hGPen);
	if (hBPen != NULL)
		DeleteObject(hBPen);
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
VOID CSurakarta::SetBoard(RECT rtBoard)
{
	//设置棋盘参数
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left;
	d = side / 11;
	size = (int)(d*0.6);

	//设置刻线画笔
	if (hRPen != NULL)
		DeleteObject(hRPen);
	if (hGPen != NULL)
		DeleteObject(hGPen);
	if (hBPen != NULL)
		DeleteObject(hBPen);
	pWidth = (int)(side / 220);
	if (pWidth == 0)
		pWidth = 1;
	hRPen = CreatePen(PS_SOLID, pWidth, RGB(255, 0, 0));
	hGPen = CreatePen(PS_SOLID, pWidth, RGB(0, 255, 0));
	hBPen = CreatePen(PS_SOLID, pWidth, RGB(0, 0, 255));

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
VOID CSurakarta::DrawBoard(HDC hDC)
{
	HFONT hOldFont;
	HPEN hOldPen;

	//绘制棋盘

	int r1 = side / 11;//小弧半径
	int r2 = side * 2 / 11;//大弧半径
	int gap = side * 3 / 11;
	int left = rtBoard.left + gap;
	int top = rtBoard.top + gap;
	int right = rtBoard.right - gap;
	int bottom = rtBoard.bottom - gap;

	//绘制红色刻线
	hOldPen = (HPEN)SelectObject(hDC, hRPen);//设置红色刻线画笔
	MoveToEx(hDC, left, top, NULL);
	LineTo(hDC, right, top);
	LineTo(hDC, right, bottom);
	LineTo(hDC, left, bottom);
	LineTo(hDC, left, top);
	SelectObject(hDC, hOldPen);

	//绘制绿色刻线
	hOldPen = (HPEN)SelectObject(hDC, hGPen);//设置绿色刻线字体
	MoveToEx(hDC, left + r1, top, NULL);
	AngleArc(hDC, left, top, r1, 0, 270);
	LineTo(hDC, right, top + r1);
	AngleArc(hDC, right, top, r1, 270, 270);
	LineTo(hDC, right - r1, bottom);
	AngleArc(hDC, right, bottom, r1, 180, 270);
	LineTo(hDC, left, bottom - r1);
	AngleArc(hDC, left, bottom, r1, 90, 270);
	LineTo(hDC, left + r1, top);
	SelectObject(hDC, hOldPen);

	//绘制蓝色刻线
	hOldPen = (HPEN)SelectObject(hDC, hBPen);//设置蓝色刻线
	MoveToEx(hDC, left + r2, top, NULL);
	AngleArc(hDC, left, top, r2, 0, 270);
	LineTo(hDC, right, top + r2);
	AngleArc(hDC, right, top, r2, 270, 270);
	LineTo(hDC, right - r2, bottom);
	AngleArc(hDC, right, bottom, r2, 180, 270);
	LineTo(hDC, left, bottom - r2);
	AngleArc(hDC, left, bottom, r2, 90, 270);
	LineTo(hDC, left + r2, top);
	SelectObject(hDC, hOldPen);

	//设置刻度字体、颜色
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextColor(hDC, RGB(0, 0, 0));

	int i, j;
	char letter[2] = { 0 }, number[2] = { 0 };

	//绘制刻度
	for (i = 0; i < 6; i++)
	{
		letter[0] = 'A' + i;
		number[0] = '1' + i;
		TextOut(hDC, rtBoard.left + (d - fWidth) / 2, (int)(top + side*i / 11 - fHeight / 2), letter, 1);
		TextOut(hDC, rtBoard.right - (d + fWidth) / 2, (int)(top + side*i / 11 - fHeight / 2), number, 1);
		TextOut(hDC, (int)(left - fWidth / 2 + side*i / 11), rtBoard.top + (d - fHeight) / 2, letter, 1);
		TextOut(hDC, (int)(left - fWidth / 2 + side*i / 11), rtBoard.bottom - (d + fHeight) / 2, number, 1);
	}

	//绘制棋子
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (board[i][j] == BLACK)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 3) / 11 - size / 2, rtBoard.top + side*(j + 3) / 11 - size / 2, size, size, hBlcDC, size, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i + 3) / 11 - size / 2, rtBoard.top + side*(j + 3) / 11 - size / 2, size, size, hBlcDC, 0, 0, SRCPAINT);
			}
			else if (board[i][j] == WHITE)
			{
				BitBlt(hDC, rtBoard.left + side*(i + 3) / 11 - size / 2, rtBoard.top + side*(j + 3) / 11 - size / 2, size, size, hWhtDC, size, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(i + 3) / 11 - size / 2, rtBoard.top + side*(j + 3) / 11 - size / 2, size, size, hWhtDC, 0, 0, SRCPAINT);
			}
		}
	}

	//绘制提示标记
	if (!stepStack.empty())
	{
		Step tStep = stepStack.top();
		IsChess(tStep.end)
		{
			IsChess(ptMovie)//吃子动画棋子
			{
				if (ptMovie.side == BLACK)
				{
					BitBlt(hDC, rtBoard.left + ptMovie.x - size / 2, rtBoard.top + ptMovie.y - size / 2, size, size, hBlcDC, size, 0, SRCAND);
					BitBlt(hDC, rtBoard.left + ptMovie.x - size / 2, rtBoard.top + ptMovie.y - size / 2, size, size, hBlcDC, 0, 0, SRCPAINT);
				}
				else if (ptMovie.side == WHITE)
				{
					BitBlt(hDC, rtBoard.left + ptMovie.x - size / 2, rtBoard.top + ptMovie.y - size / 2, size, size, hWhtDC, size, 0, SRCAND);
					BitBlt(hDC, rtBoard.left + ptMovie.x - size / 2, rtBoard.top + ptMovie.y - size / 2, size, size, hWhtDC, 0, 0, SRCPAINT);
				}
			}
			else//落点标记
			{
				BitBlt(hDC, rtBoard.left + side*(tStep.end.x + 3) / 11 - size / 2, rtBoard.top + side*(tStep.end.y + 3) / 11 - size / 2, size, size, hMarkDC, size, 0, SRCAND);
				BitBlt(hDC, rtBoard.left + side*(tStep.end.x + 3) / 11 - size / 2, rtBoard.top + side*(tStep.end.y + 3) / 11 - size / 2, size, size, hMarkDC, 0, 0, SRCPAINT);
			}
		}
		else IsChess(tStep.start)//选中棋子
		{
			BitBlt(hDC, rtBoard.left + side*(tStep.start.x + 3) / 11 - size / 2, rtBoard.top + side*(tStep.start.y + 3) / 11 - size / 2, size, size, hMarkDC, size, 0, SRCAND);
			BitBlt(hDC, rtBoard.left + side*(tStep.start.x + 3) / 11 - size / 2, rtBoard.top + side*(tStep.start.y + 3) / 11 - size / 2, size, size, hMarkDC, 0, 0, SRCPAINT);
		}
	}

	SelectObject(hDC, hOldFont);
}

/**
 * DrawChess - 绘制棋子元素
 */
VOID CSurakarta::DrawChess()
{
	char filename[MAX_PATH] = { 0 };
	HBITMAP hBlcBmp, hWhtBmp, hMarkBmp;

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\black.bmp");
	hBlcBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, size * 2, size, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\white.bmp");
	hWhtBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, size * 2, size, LR_LOADFROMFILE);

	strcpy(filename, LibPath);
	strcat(filename, "\\bmp\\mark.bmp");
	hMarkBmp = (HBITMAP)LoadImage(hInst, filename, IMAGE_BITMAP, size * 2, size, LR_LOADFROMFILE);

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
bool CSurakarta::PlaySnd(int tag)
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
	case 2://吃子音效
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\吃子.wav");
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
VOID CSurakarta::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;
	ptMovie.side = EMPTY;
	ptMovie.x = ptMovie.y = -1;
	count = 0;
	CleanStack(stepStack);//清空着法栈
	InitBoard();//初始化棋盘
	return;
}

/**
 * InitBoard - 初始化棋盘
 */
VOID CSurakarta::InitBoard()
{
	int i, j;
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (j == 0 || j == 1)
			{
				board[i][j] = BLACK;
			}
			else if (j == 2 || j == 3)
			{
				board[i][j] = EMPTY;
			}
			else
			{
				board[i][j] = WHITE;
			}
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
INT CSurakarta::ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
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
		tStep.start.x = moveCmd[pos] - BX;
		tStep.start.y = moveCmd[pos + 1] - BY;
		tStep.end.x = moveCmd[pos + 2] - BX;
		tStep.end.y = moveCmd[pos + 3] - BY;
		tStep.side = player;
		stepStack.push(tStep);//临时着法压栈

		//判断是否符合规则
		if (!FitRules())
		{
			strcat(curCmd, "error\n");
			stepStack.pop();//非法着法出栈
			return -1;//行棋违规
		}

		//走子
		MoveChess();//函数内刷新棋盘，播放音效

		//追加着法历史
		ShowStepHis(moveCmd + pos);

		//生成命令串
		sprintf(denCmd, "move %c%c%c%c\n", tStep.start.x + BX, tStep.start.y + BY, tStep.end.x + BX, tStep.end.y + BY);

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
BOOL CSurakarta::OnLButtonDown(int x, int y)
{
	Point point;

	if (count == -1)//处于屏蔽输入状态
		return 0;

	//把窗口坐标映射为棋盘坐标
	point.x = (int)((x - rtBoard.left - side / 22) * 11 / side) - 2;
	point.y = (int)((y - rtBoard.top - side / 22) * 11 / side) - 2;
	if (point.x < 0 || point.x >= 6 || point.y < 0 || point.y >= 6)
		return 2;

	return SToS(point);
}

/**
 * SToS - 手动逻辑处理
 */
BOOL CSurakarta::SToS(Point point)
{
	Step tStep;

	if (count == 0)//选子
	{
		//检查着法合法性
		if (board[point.x][point.y] != player)//选中本方棋子
			return -1;//着法非法

		//填充临时着法
		InitStep(tStep);//初始化着法
		tStep.side = player;//填充棋手
		tStep.start = point;//填充选子
		stepStack.push(tStep);//临时着法压栈

		count = 1;

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放选子音效
		PlaySnd(0);

		return 0;//着法进行
	}
	else if (count == 1)//走子
	{
		//填充临时着法
		tStep = stepStack.top();//获取临时着法
		tStep.end = point;//填充落点
		stepStack.push(tStep);//临时着法压栈，为FitRules做准备

		//检查着法合法性
		if (!FitRules())
		{
			stepStack.pop();//临时着法出栈
			return -1;//着法非法
		}

		stepStack.pop();//出栈，弹出FitRules用step
		stepStack.pop();//出栈，弹出上一次临时step
		stepStack.push(tStep);//临时着法压栈

		//走子
		MoveChess();

		count = -1;//输入完成

		return 1;//着法成立
	}

	return 0;
}

/**
 * OkMove - 确认着法
 */
INT CSurakarta::OkMove(char *denCmd)
{
	Step tStep;
		
	denCmd[0] = '\0';//默认空命令

	if (count == -1)//输入完成
	{
		tStep = stepStack.top();//获取完整着法

		//生成命令串
		sprintf(denCmd, "move %c%c%c%c", tStep.start.x + BX, tStep.start.y + BY, tStep.end.x + BX, tStep.end.y + BY);

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
INT CSurakarta::CancelMove()
{
	Step tStep;

	if (count == 0)//无输入，无意义
		return 0;

	tStep = stepStack.top();//获取着法
	stepStack.pop();//着法出栈

	//恢复落点
	IsChess(tStep.end)
	{
		if (tStep.flagE)//吃子着法
			board[tStep.end.x][tStep.end.y] = NEXTPLAYER(tStep.side);
		else
			board[tStep.end.x][tStep.end.y] = EMPTY;
	}

	//恢复选子
	IsChess(tStep.start)
	{
		board[tStep.start.x][tStep.start.y] = tStep.side;
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
bool CSurakarta::FitRules()
{
	Step tStep = stepStack.top();//获取着法

	if (tStep.start.x < 0 || tStep.start.x >= 6 || tStep.start.y < 0 || tStep.start.y >= 6)
		return false;
	if (tStep.end.x < 0 || tStep.end.x >= 6 || tStep.end.y < 0 || tStep.end.y >= 6)
		return false;
	if (board[tStep.start.x][tStep.start.y] != tStep.side)
		return false;
	if (board[tStep.end.x][tStep.end.y] == tStep.side)//落点为本方子
		return false;
	else if (board[tStep.end.x][tStep.end.y] == NEXTPLAYER(tStep.side))//落点为对方子
	{
		if (SearchWay(tStep))//可吃
			return true;
		else
			return false;
	}
	else//落点为空
	{
		if (pow(tStep.end.x - tStep.start.x, 2) + pow(tStep.end.y - tStep.start.y, 2) >= 4)//dx^2+dy^2>=4，移动不是一步
			return false;
	}
	return true;
}

/**
 * WinOrLose - 终盘测试
 */
bool CSurakarta::WinOrLose()
{
	int numBlc, numWht;

	numBlc = numWht = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (board[i][j] == BLACK)
			{
				numBlc++;
			}
			else if (board[i][j] == WHITE)
			{
				numWht++;
			}
		}
	}
	if (numBlc == 0 || numWht == 0)//一方无子
	{
		PostMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)(numBlc == 0 ? WHITE : BLACK));
		return true;
	}
	return false;
}

/**
* MoveChess - 处理走子
*/
VOID CSurakarta::MoveChess()
{
	Step tStep = stepStack.top();//获取临时着法
	stepStack.pop();//临时着法出栈

	//行棋
	board[tStep.start.x][tStep.start.y] = EMPTY;//选子
	if (board[tStep.end.x][tStep.end.y] == NEXTPLAYER(tStep.side))//吃子
	{
		//进行着法合法性判断时产生吃子路径
		tStep.flagE = true;//设置着法吃子标记

		//播放吃子动画
		PlayMovie();

		board[tStep.end.x][tStep.end.y] = tStep.side;//吃子

		//播放吃子音效
		PlaySnd(2);
	}
	else//走子
	{
		board[tStep.end.x][tStep.end.y] = tStep.side;//落子

		//播放落子音效
		PlaySnd(1);
	}

	stepStack.push(tStep);//完整着法压栈

	//刷新棋盘
	InvalidateRect(hWnd, &rtBoard, FALSE);
	UpdateWindow(hWnd);
}

/**
* SearchWay - 搜索吃子路线并保存
*/
bool CSurakarta::SearchWay(Step sp)
{
	Point first, second;
	int deplayer = NEXTPLAYER(sp.side);
	int SmaID[2] = { -1, -1 };
	int BigID[2] = { -1, -1 };
	int fx, fy;
	int k, i, j;
	bool circle;
	int id;

	first = sp.start;
	second = sp.end;

	//记录棋子在轨道上位置
	for (k = 0, i = 0, j = 0; k < 28; k++)
	{
		if (SmaWay[k].x == first.x &&SmaWay[k].y == first.y)
			SmaID[i++] = k;
		if (BigWay[k].x == first.x &&BigWay[k].y == first.y)
			BigID[j++] = k;
	}

	//搜索吃子路线
	for (i = 0; i < 2; i++)
	{
		if (SmaID[i] != -1)//小弧轨道
		{
			//正向
			circle = false;
			id = (SmaID[i] + 1) % 28;//+1(mod28)
			do{
				fx = SmaWay[id].x;
				fy = SmaWay[id].y;
				if (IsCorner(fx, fy))//顶点，经过弧
					circle = true;
				else if (board[fx][fy] != EMPTY && (fx != first.x || fy != first.y))
				{
					if (board[fx][fy] == deplayer && second.x == fx && second.y == fy && circle)
					{
						SaveWay(SmaWay, SmaID[i], id, true);
						return true;
					}
					break;
				}
				id = (id + 1) % 28;//+1(mod28)
			} while (id != SmaID[i]);

			//反向
			circle = false;
			id = (SmaID[i] + 27) % 28;
			do{
				fx = SmaWay[id].x;
				fy = SmaWay[id].y;
				if (IsCorner(fx, fy))//顶点，经过弧
					circle = true;
				else if (board[fx][fy] != EMPTY && (fx != first.x || fy != first.y))
				{
					if (board[fx][fy] == deplayer && second.x == fx && second.y == fy && circle)
					{
						SaveWay(SmaWay, SmaID[i], id, false);
						return true;
					}
					break;
				}
				id = (id + 27) % 28;//-1(mod28)
			} while (id != SmaID[i]);
		}
	}
	for (i = 0; i < 2; i++)
	{
		if (BigID[i] != -1)//大弧轨道
		{
			//正向
			circle = false;
			id = (BigID[i] + 1) % 28;//+1(mod28)
			do{
				fx = BigWay[id].x;
				fy = BigWay[id].y;
				if (IsCorner(fx, fy))//顶点，经过弧
					circle = true;
				else if (board[fx][fy] != EMPTY && (fx != first.x || fy != first.y))
				{
					if (board[fx][fy] == deplayer && second.x == fx && second.y == fy && circle)
					{
						SaveWay(BigWay, BigID[i], id, true);
						return true;
					}
					break;
				}
				id = (id + 1) % 28;//+1(mod28)
			} while (id != BigID[i]);

			//反向
			circle = false;
			id = (BigID[i] + 27) % 28;//-1(mod28)
			do{
				fx = BigWay[id].x;
				fy = BigWay[id].y;
				if (IsCorner(fx, fy))//顶点，经过弧
					circle = true;
				else if (board[fx][fy] != EMPTY  && (fx != first.x || fy != first.y))
				{
					if (board[fx][fy] == deplayer && second.x == fx && second.y == fy && circle)
					{
						SaveWay(BigWay, BigID[i], id, false);
						return true;
					}
					break;
				}
				id = (id + 27) % 28;//-1(mod28)
			} while (id != BigID[i]);
		}
	}
	return false;
}

/**
 * SaveWay - 保存吃子路径
 */
VOID CSurakarta::SaveWay(Point *way, int start, int end, bool dir)
{
	MoveWay.clear();//清空容器
	for (int i = start; i != end;)
	{
		MoveWay.push_back(way[i]);
		dir ? i++ : i--;
		i = (i + 28) % 28;
	}
	MoveWay.push_back(way[end]);
}

/**
* PlayMovie - 播放吃子动画
*/
VOID CSurakarta::PlayMovie()
{
	vector<Point> ptWay;
	vector<Point>::iterator iter;

	SavePtWay(&ptWay);
	ptMovie.side = player;
	for (iter = ptWay.begin(); iter != ptWay.end(); iter++)
	{
		ptMovie.x = iter->x;
		ptMovie.y = iter->y;
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//Sleep(20);//动画循环不必sleep
	}
	ptMovie.x = ptMovie.y = -1;

	return;
}

/**
 * SavePtWay - 生成吃子动画轨迹
 */
VOID CSurakarta::SavePtWay(vector<Point> *ptWay)
{
	PointF pt;
	vector<PointF> vpt;//棋子移动路径坐标
	vector<PointF>::iterator fIter;
	vector<Point>::iterator iter;
	int r;//弧的半径
	double aleph;

	for (iter = MoveWay.begin(); iter != MoveWay.end() - 1; iter++)
	{
		if (IsCorner((iter + 1)->x, (iter + 1)->y))//角上画弧
		{
			//以起点及圆心顶弧
			if ((iter + 1)->x < iter->x)//o.x<s.x
			{
				if ((iter + 1)->y == 0)//顶边
				{
					for (int i = 1; i <= 15; i++)//左上角逆时针弧
					{
						r = iter->x - (iter + 1)->x;
						aleph = i * 0.1 * PI;// 3/2 PI 的弧分10份
						pt.x = (iter + 1)->x + r * cos(aleph);
						pt.y = (iter + 1)->y - r * sin(aleph);
						vpt.push_back(pt);
					}
				}
				else if ((iter + 1)->y == 5)//底边
				{
					for (int i = 1; i <= 15; i++)//左下角顺时针弧
					{
						r = iter->x - (iter + 1)->x;
						aleph = i * 0.1 * PI;// 3/2 PI 的弧分10份
						pt.x = (iter + 1)->x + r * cos(aleph);
						pt.y = (iter + 1)->y + r * sin(aleph);
						vpt.push_back(pt);
					}
				}
			}
			else if ((iter + 1)->x > iter->x)//o.x>s.x
			{
				if ((iter + 1)->y == 0)//顶边
				{
					for (int i = 1; i <= 15; i++)//右上角顺时针弧
					{
						r = (iter + 1)->x - iter->x;
						aleph = i * 0.1 * PI;// 3/2 PI 的弧分10份
						pt.x = (iter + 1)->x - r * cos(aleph);
						pt.y = (iter + 1)->y - r * sin(aleph);
						vpt.push_back(pt);
					}
				}
				if ((iter + 1)->y == 5)//底边
				{
					for (int i = 1; i <= 15; i++)//右下角逆时针弧
					{
						r = (iter + 1)->x - iter->x;
						aleph = i * 0.1 * PI;// 3/2 PI 的弧分10份
						pt.x = (iter + 1)->x - r * cos(aleph);
						pt.y = (iter + 1)->y + r * sin(aleph);
						vpt.push_back(pt);
					}
				}
			}
			else if ((iter + 1)->y < iter->y)//o.y<s.y
			{
				if ((iter + 1)->x == 0)//左边
				{
					for (int i = 1; i <= 15; i++)//左上角顺时针弧
					{
						r = iter->y - (iter + 1)->y;
						aleph = i * 0.1 * PI;// 3/2 PI 的弧分10份
						pt.x = (iter + 1)->x - r * sin(aleph);
						pt.y = (iter + 1)->y + r * cos(aleph);
						vpt.push_back(pt);
					}
				}
				if ((iter + 1)->x == 5)//右边
				{
					for (int i = 1; i <= 15; i++)//右上角逆时针弧
					{
						r = iter->y - (iter + 1)->y;
						aleph = i * 0.1 * PI;// 3/2 PI 的弧分10份
						pt.x = (iter + 1)->x + r * sin(aleph);
						pt.y = (iter + 1)->y + r * cos(aleph);
						vpt.push_back(pt);
					}
				}
			}
			else if ((iter + 1)->y > iter->y)//o.y>s.y
			{
				if ((iter + 1)->x == 0)//左边
				{
					for (int i = 1; i <= 15; i++)//左下角逆时针弧
					{
						r = (iter + 1)->y - iter->y;
						aleph = i * 0.1 * PI;// 3/2 PI 的弧分10份
						pt.x = (iter + 1)->x - r * sin(aleph);
						pt.y = (iter + 1)->y - r * cos(aleph);
						vpt.push_back(pt);
					}
				}
				if ((iter + 1)->x == 5)//右边
				{
					for (int i = 1; i <= 15; i++)//左下角顺时针弧
					{
						r = (iter + 1)->y - iter->y;
						aleph = i * 0.1 * PI;// 3/2 PI 的弧分10份
						pt.x = (iter + 1)->x + r * sin(aleph);
						pt.y = (iter + 1)->y - r * cos(aleph);
						vpt.push_back(pt);
					}
				}
			}
			iter++;
		}
		else if (iter->x == (iter + 1)->x &&iter->y == (iter + 1)->y - 1)//向下
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x;
				pt.y = iter->y + 0.5 * i;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x &&iter->y == (iter + 1)->y + 1)//向上
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x;
				pt.y = iter->y - 0.5 * i;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x - 1 && iter->y == (iter + 1)->y)//向右
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x + 0.5 * i;
				pt.y = iter->y;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x + 1 && iter->y == (iter + 1)->y)//向左
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x - 0.5 * i;
				pt.y = iter->y;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x - 1 && iter->y == (iter + 1)->y - 1)//右下
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x + 0.5 * i;
				pt.y = iter->y + 0.5 * i;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x + 1 && iter->y == (iter + 1)->y + 1)//左上
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x - 0.5 * i;
				pt.y = iter->y - 0.5 * i;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x - 1 && iter->y == (iter + 1)->y + 1)//右上
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x + 0.5 * i;
				pt.y = iter->y - 0.5 * i;
				vpt.push_back(pt);
			}
		}
		else if (iter->x == (iter + 1)->x + 1 && iter->y == (iter + 1)->y - 1)//左下
		{
			for (int i = 1; i <= 2; i++)
			{
				pt.x = iter->x - 0.5 * i;
				pt.y = iter->y + 0.5 * i;
				vpt.push_back(pt);
			}
		}
	}

	Point ptw;
	//浮点坐标转化为整型坐标
	for (fIter = vpt.begin(); fIter != vpt.end(); fIter++)
	{
		ptw.x = (int)((fIter->x + 3) * side / 11);
		ptw.y = (int)((fIter->y + 3) * side / 11);
		ptWay->push_back(ptw);
	}
	return;
}

