#include "CDotsAndBoxes.h"


VOID CDotsAndBoxes::GetConfig()
{
	char filename[MAX_PATH];
	sprintf(filename, "%s\\Config.ini", LibPath);
	BE = GetPrivateProfileInt("COORDINATE", "E", 'A', filename);
	CChess::GetConfig();
}

CDotsAndBoxes::CDotsAndBoxes(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);
	hFont = NULL;

	//获取配置
	GetConfig();

	//创建兼容DC
	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hPtDC = CreateCompatibleDC(hDC);

	InitGame();
}

CDotsAndBoxes::~CDotsAndBoxes()
{
	if (hFont != NULL)
		DeleteObject(hFont);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hPtDC);
}

/**
 * SetBoard - 设置棋盘参数
 * @rtBoard:	棋盘在窗口客户区的位置
 */
VOID CDotsAndBoxes::SetBoard(RECT rtBoard)
{
	//设置棋盘参数
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left + 1;
	d = (int)(side / 7);
	size = (int)(d*0.8);
	pWidth = (int)(side / 100);
	pR = (int)(side / 70);
	pL = 2 * pR;
\
	//设置刻度字体
	if (hFont != NULL)
		DeleteObject(hFont);
	fWidth = d / 3;
	fHeight = d * 2 / 3;
	hFont = CreateSimpleFont(fWidth, fHeight);

	//绘制棋子元素
	DrawChess();
}

/**
 * DrawBoard - 绘制棋盘
 * @hDC:	主窗口DC
 */
VOID CDotsAndBoxes::DrawBoard(HDC hDC)
{
	int i,j,k;
	HFONT hOldFont;

	// 绘制点
	for(i=0;i<6;i++)
	{		
		for(j=0;j<6;j++)
		{
			BitBlt(hDC, rtBoard.left + (side + side*j) / 7 - pR, rtBoard.top + (side + side*i) / 7 - pR, pL, pL, hPtDC, 0, 0, SRCAND);
		}
	}

	//设置刻度字体、颜色
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2] = { 0 }, number[3] = { 0 };

	//绘制刻度
	for (i = 1; i <= 6; i++)
	{
		letter[0] = 'A' + i - 1;
		itoa(i, number, 10);
		TextOut(hDC, rtBoard.left + side*i / 7 - fWidth / 2, rtBoard.top + side / 14 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side*i / 7 - fWidth / 2, rtBoard.top + side * 13 / 14 - fHeight / 2, number, 1);
		TextOut(hDC, rtBoard.left + side / 14 - fWidth / 2, rtBoard.top + side*i / 7 - fHeight / 2, letter, 1);
		TextOut(hDC, rtBoard.left + side * 13 / 14 - fWidth / 2, rtBoard.top + side*i / 7 - fHeight / 2, number, 1);
	}
	
	// 绘制线
	HPEN hBlcPen,hWhtPen,hPen;
	hBlcPen = CreatePen(PS_SOLID, pWidth, RGB(0, 0, 0));
	hWhtPen = CreatePen(PS_SOLID, pWidth, RGB(255, 255, 255));
	hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	//横向
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (line[0][i][j] == BLACK)
			{
				SelectObject(hDC, hBlcPen);
				MoveToEx(hDC, (int)(rtBoard.left + side*(j + 1) / 7), (int)(rtBoard.top + side*(i + 1) / 7), NULL);
				LineTo(hDC, (int)(rtBoard.left + side*(j + 2) / 7), (int)(rtBoard.top + side*(i + 1) / 7));
			}
			else if (line[0][i][j] == WHITE)
			{
				SelectObject(hDC, hWhtPen);
				MoveToEx(hDC, (int)(rtBoard.left + side*(j + 1) / 7), (int)(rtBoard.top + side*(i + 1) / 7), NULL);
				LineTo(hDC, (int)(rtBoard.left + side*(j + 2) / 7), (int)(rtBoard.top + side*(i + 1) / 7));
			}
		}
	}
	//纵向
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (line[1][i][j] == BLACK)
			{
				SelectObject(hDC, hBlcPen);
				MoveToEx(hDC, (int)(rtBoard.left + side*(i + 1) / 7), (int)(rtBoard.top + side*(j + 1) / 7), NULL);
				LineTo(hDC, (int)(rtBoard.left + side*(i + 1) / 7), (int)(rtBoard.top + side*(j + 2) / 7));
			}
			else if (line[1][i][j] == WHITE)
			{
				SelectObject(hDC, hWhtPen);
				MoveToEx(hDC, (int)(rtBoard.left + side*(i + 1) / 7), (int)(rtBoard.top + side*(j + 1) / 7), NULL);
				LineTo(hDC, (int)(rtBoard.left + side*(i + 1) / 7), (int)(rtBoard.top + side*(j + 2) / 7));
			}

		}
	}

	// 绘制格
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (box[i][j] == BLACK)
			{
				BitBlt(hDC, (int)(rtBoard.left + side*(i * 2 + 3) / 14 - size / 2), (int)(rtBoard.top + side*(j * 2 + 3) / 14 - size / 2), size, size, hBlcDC, 0, 0, SRCCOPY);
			}
			else if (box[i][j] == WHITE)
			{
				BitBlt(hDC, (int)(rtBoard.left + side*(i * 2 + 3) / 14 - size / 2), (int)(rtBoard.top + side*(j * 2 + 3) / 14 - size / 2), size, size, hWhtDC, 0, 0, SRCCOPY);
			}
		}
	}

	//绘制提示标记
	if (!stepStack.empty())
	{
		Step tStep = stepStack.top();
		SelectObject(hDC, hPen);
		IsLine(tStep.line)
		{
			MoveToEx(hDC, (int)(rtBoard.left + side*(tStep.start.x + 1) / 7), (int)(rtBoard.top + side*(tStep.start.y + 1) / 7), NULL);
			LineTo(hDC, (int)(rtBoard.left + side*(tStep.end.x + 1) / 7), (int)(rtBoard.top + side*(tStep.end.y + 1) / 7));
		}
		else IsChess(tStep.start)
		{
			MoveToEx(hDC, (int)(rtBoard.left + (side + side*tStep.start.x) / 7 - pR), (int)(rtBoard.top + (side + side*tStep.start.y) / 7), NULL);
			LineTo(hDC, (int)(rtBoard.left + (side + side*tStep.start.x) / 7 + pR), (int)(rtBoard.top + (side + side*tStep.start.y) / 7));
			MoveToEx(hDC, (int)(rtBoard.left + (side + side*tStep.start.x) / 7), (int)(rtBoard.top + (side + side*tStep.start.y) / 7 - pR), NULL);
			LineTo(hDC, (int)(rtBoard.left + (side + side*tStep.start.x) / 7), (int)(rtBoard.top + (side + side*tStep.start.y) / 7 + pR));
		}
	}

	DeleteObject(hBlcPen);
	DeleteObject(hWhtPen);
	DeleteObject(hPen);
}

/**
 * DrawChess - 绘制棋子元素
 */
bool CDotsAndBoxes::DrawChess()
{
	HBITMAP hBlcBmp, hWhtBmp, hPtBmp;
	hBlcBmp = CreateCompatibleBitmap(hBlcDC, d, d);
	hWhtBmp = CreateCompatibleBitmap(hWhtDC, d, d);
	hPtBmp = CreateCompatibleBitmap(hPtDC, pL, pL);

	SelectObject(hBlcDC, hBlcBmp);
	SelectObject(hWhtDC, hWhtBmp);
	SelectObject(hPtDC, hPtBmp);

	HBRUSH hBlcBrush, hWhtBrush, hOldBrush;
	LOGBRUSH lb;

	lb.lbColor = RGB(0, 0, 0);
	lb.lbStyle = BS_SOLID;
	hBlcBrush = CreateBrushIndirect(&lb);
	hOldBrush=(HBRUSH)SelectObject(hBlcDC,hBlcBrush);
	PatBlt(hBlcDC,0,0,d,d,PATCOPY);

	lb.lbColor=RGB(255,255,255);
	hWhtBrush=CreateBrushIndirect(&lb);
	hOldBrush=(HBRUSH)SelectObject(hWhtDC,hWhtBrush);
	PatBlt(hWhtDC,0,0,d,d,PATCOPY);

	DrawPoint(hPtDC, pR, RGB(0, 0, 0),RGB(255, 255, 255));

	DeleteObject(hBlcBmp);
	DeleteObject(hWhtBmp);
	DeleteObject(hBlcBrush);
	DeleteObject(hWhtBrush);
	DeleteObject(hPtBmp);

	return true;
}

/**
 * PlaySnd - 播放音效
 * @tag:	音效标签
 */
bool CDotsAndBoxes::PlaySnd(int tag)
{
	char filename[MAX_PATH] = { 0 };

	switch (tag)
	{
	case 0://选点音效
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\选子.wav");
		break;
	case 1://连线音效
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\连线.wav");
		break;
	case 2://捕获格音效
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\捕获格.wav");
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
VOID CDotsAndBoxes::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));
	player = BLACK;
	lineNum = 0;
	count = 0;
	CleanStack(stepStack);//清空着法栈
	InitBoard();//初始化棋盘
}

/**
 * InitBoard - 初始化棋盘
 */
VOID CDotsAndBoxes::InitBoard()
{
	int i, j, k;
	for (k = 0; k<2; k++)
	{
		for (i = 0; i<6; i++)
		{
			for (j = 0; j<5; j++)
			{
				line[k][i][j] = EMPTY;
			}
		}
	}
	for (i = 0; i<5; i++)
	{
		for (j = 0; j<5; j++)
		{
			box[i][j] = EMPTY;
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
BOOL CDotsAndBoxes::ProcessMove(char *moveCmd, char *curCmd, char *denCmd)
{				
	char *res;
	int pos=0,cs;
	Step tStep;
	int len=strlen("move ");
	int lineNum;
	bool winF = false;

	curCmd[0] = denCmd[0] = '\0';//默认空消息

	if ((res = strstr(moveCmd, "move")) == NULL)//寻找move关键字
	{
		return 0;//未找到“move”关键字
	}
	else
	{		
		pos = (res - moveCmd);
		
		//解析着法
		sscanf(moveCmd + len, "%d", &lineNum);//连线数目
		if (lineNum >= 10)
			len += 3;
		else
			len += 2;
		pos += len;

		InitStep(tStep);
		tStep.side = player;
		cs = 0;
		do{
			if (cs >= lineNum)//连线过多,例外：捕获最后一个格子
			{
				if (WinOrLose())
				{
					winF = true;
					break;
				}
				strcat(curCmd, "error\n");
				while (cs--)//取消之前判别连线
				{
					tStep = stepStack.top();//获取临时着法
					line[tStep.line.k][tStep.line.i][tStep.line.j] = EMPTY;//恢复连线
					UnpackBox(tStep.line);//拆除格子
					stepStack.pop();//临时着法出栈
				}
				return -1;//着法非法
			}
			tStep.line.k = moveCmd[pos + cs * 3] - BE;
			tStep.line.i = moveCmd[pos + cs * 3 + 1] - BX;
			tStep.line.j = moveCmd[pos + cs * 3 + 2] - BY;
			LineToPoint(tStep);//线坐标映射点坐标
			stepStack.push(tStep);//临时着法压栈

			//判断是否符合规则
			if (!FitRules())
			{
				strcat(curCmd, "error\n");
				stepStack.pop();//临时着法出栈
				while (cs--)//取消之前判别连线
				{
					tStep = stepStack.top();//获取临时着法
					line[tStep.line.k][tStep.line.i][tStep.line.j] = EMPTY;//恢复连线
					UnpackBox(tStep.line);//拆除格子
					stepStack.pop();//临时着法出栈
				}
				return -1;//着法非法
			}

			//连线
			line[tStep.line.k][tStep.line.i][tStep.line.j] = player;

			cs++;
		} while (HaveBox(tStep.line));//捕获格子

		if (cs != lineNum)//连线数不足
		{
			strcat(curCmd, "error\n");
			while (cs--)//取消之前判别连线
			{
				tStep = stepStack.top();//获取临时着法
				line[tStep.line.k][tStep.line.i][tStep.line.j] = EMPTY;//恢复连线
				UnpackBox(tStep.line);//拆除格子
				stepStack.pop();//临时着法出栈
			}
			return -1;//着法非法
		}

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放连线音效
		PlaySnd(0);

		//追加着法历史
		ShowStepHis(moveCmd + pos);

		//生成命令串
		strncpy(denCmd, res, len + lineNum * 3);
		sprintf(denCmd+ len + lineNum * 3, "\n");

		//累计步数
		StepNum[player]++;

		//判断胜负
		if (winF)
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
BOOL CDotsAndBoxes::OnLButtonDown(int x,int y)
{
	Point point;

	if (count == -1)//处于屏蔽输入状态，count=-1时return0可能造成意外
		return 0;

	//把窗口坐标映射为棋盘坐标
	point.x = ((x - rtBoard.left) * 7 - side / 2) / side;
	point.y = ((y - rtBoard.top) * 7 - side / 2) / side;
	if (point.x < 0 || point.x >= 6 || point.y < 0 || point.y >= 6)
		return 2;

	return SToS(point);
}

/**
 * SToS - 手动逻辑处理
 */
BOOL CDotsAndBoxes::SToS(Point point)
{
	Step tStep;

	if (count == 0)//选点
	{
		//检查着法合法性
		if (HaveLine(point) == false)//点不能引出线，无效
			return -1;

		//填充临时着法
		InitStep(tStep);//初始化着法
		tStep.side = player;//填充棋手
		tStep.start = point;//填充选点
		stepStack.push(tStep);//临时着法压栈

		count = 1;

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放选点音效
		PlaySnd(0);

		return 0;//着法进行
	}
	else if (count == 1)//连线
	{
		//填充临时着法
		tStep = stepStack.top();//获取临时着法
		tStep.end = point;//填充连线

		//检查着法合法性
		if (PonitToLine(tStep) == false)//由两点不能映射为连线，无效
			return -1;//着法非法

		//连线
		line[tStep.line.k][tStep.line.i][tStep.line.j] = player;

		stepStack.pop();//临时着法出栈
		stepStack.push(tStep);//完整着法压栈

		count = -1;//输入完成

		//刷新棋盘
		InvalidateRect(hWnd, &rtBoard, FALSE);
		UpdateWindow(hWnd);
		//播放连线音效
		PlaySnd(1);

		return 1;//着法成立
	}

	return 0;
}

/**
 * OkMove - 确认着法
 */
INT CDotsAndBoxes::OkMove(char *denCmd)
{
	Step tStep;

	denCmd[0] = '\0';//默认空命令

	if (count == -1)
	{
		tStep = stepStack.top();//获取完整着法

		//生成命令串
		sprintf(lineList + lineNum * 3, "%c%c%c", tStep.line.k + BE, tStep.line.i + BX, tStep.line.j + BY);
		lineNum++;//连续连线数目加1

		if (HaveBox(tStep.line))//捕获格子不换手，并显示格子颜色
		{
			//刷新棋盘
			InvalidateRect(hWnd, &rtBoard, FALSE);
			UpdateWindow(hWnd);
			//播放捕获格子音效
			PlaySnd(2);

			//累计步数
			StepNum[player]++;

			//判断胜负,捕获最后一个格子
			if (WinOrLose())
			{
				//生成命令串
				sprintf(denCmd, "move %d %s\nend\n", lineNum, lineList);
				//追加着法历史
				ShowStepHis(lineList);
				return 2;//分出胜负
			}
		}
		else
		{
			//生成命令串
			sprintf(denCmd, "move %d %s\n", lineNum, lineList);

			//追加着法历史
			ShowStepHis(lineList);

			//累计步数
			StepNum[player]++;

			//行棋换手
			player = NEXTPLAYER(player);

			lineNum = 0;//换手，连续连线数归零
		}

		count = 0;//重置输入

		return 1;//着法成立
	}
	return 0;
}

/**
 * CancelMove - 取消着法
 */
INT CDotsAndBoxes::CancelMove()
{
	Step tStep;

	if (count == 0)//无输入，无意义
		return 0;

	tStep = stepStack.top();//获取着法
	stepStack.pop();//着法出栈

	//恢复连线
	IsLine(tStep.line)
	{
		line[tStep.line.k][tStep.line.i][tStep.line.j] = EMPTY;
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
bool CDotsAndBoxes::FitRules()
{
	Step tStep = stepStack.top();
	if (tStep.line.k < 0 || tStep.line.k > 1 || tStep.line.i < 0 || tStep.line.i > 5 || tStep.line.j < 0 || tStep.line.j > 4)
		return false;
	if (line[tStep.line.k][tStep.line.i][tStep.line.j] != EMPTY)
		return false;
	return true;
}

/**
 * WinOrLose - 终盘测试
 */
bool CDotsAndBoxes::WinOrLose()
{
	int numBlc, numWht, numEmt;//黑方格子，白方格子，空格子
	numBlc = numWht = numEmt = 0;
	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<5; j++)
		{
			if (box[i][j] == BLACK)
			{
				numBlc++;
			}
			else if (box[i][j] == WHITE)
			{
				numWht++;
			}
			else
			{
				numEmt++;
			}
		}
	}
	if (numEmt == 0)//所有格子被捕获，捕获格子多者获胜
	{
		SendMessage(hWnd, GM_WINLOSE, (WPARAM)(StepNum[BLACK] << 16) + StepNum[WHITE], (LPARAM)(numBlc > numWht ? BLACK : WHITE));
		return true;
	}
	return false;
}

/**
 * NoLine - 判断点是否能够引出线
 */
bool CDotsAndBoxes::HaveLine(Point point)
{
	if (point.x > 0)
	{
		if (line[0][point.y][point.x - 1] == EMPTY)
			return true;
	}
	if (point.x < 5)
	{
		if (line[0][point.y][point.x] == EMPTY)
			return true;
	}
	if (point.y > 0)
	{
		if (line[1][point.x][point.y - 1] == EMPTY)
			return true;
	}
	if (point.y < 5)
	{
		if (line[1][point.x][point.y] == EMPTY)
			return true;
	}
	return false;
}

/**
 * HaveBox - 判断连线是否能够捕获格子
 */
bool CDotsAndBoxes::HaveBox(LINE line)
{
	bool flag = false;

	// 1~5线判断左侧格子
	if (line.i>0 && this->line[line.k][line.i - 1][line.j] != EMPTY &&this->line[line.k ^ 1][line.j][line.i - 1] != EMPTY &&this->line[line.k ^ 1][line.j + 1][line.i - 1] != EMPTY)
	{
		if (line.k == 0)
		{
			box[line.j][line.i - 1] = player;
		}
		else if (line.k == 1)
		{
			box[line.i - 1][line.j] = player;
		}
		flag = true;
	}
	// 0~4线判断右侧格子
	if (line.i<5 && this->line[line.k][line.i + 1][line.j] != EMPTY &&this->line[line.k ^ 1][line.j][line.i] != EMPTY &&this->line[line.k ^ 1][line.j + 1][line.i] != EMPTY)
	{
		if (line.k == 0)
		{
			box[line.j][line.i] = player;
		}
		else if (line.k == 1)
		{
			box[line.i][line.j] = player;
		}
		flag = true;
	}
	return flag;
}

/**
 * UnpackBox - 拆除连线捕获的格子
 */
void CDotsAndBoxes::UnpackBox(LINE line)
{
	if (line.k == 0)
	{
		box[line.j][line.i - 1] = EMPTY;
		box[line.j][line.i] = EMPTY;
	}
	else if (line.k == 1)
	{
		box[line.i - 1][line.j] = EMPTY;
		box[line.i][line.j] = EMPTY;
	}
}

/**
 * PointToLine - 将线的起止点坐标转化为线的位置坐标
 */
bool CDotsAndBoxes::PonitToLine(Step &step)
{
	int dx, dy;
	dx = step.start.x - step.end.x;
	dy = step.start.y - step.end.y;
	if (dx == 0)
	{
		if (dy == 1)
			step.line.j = step.end.y;
		else if (dy == -1)
			step.line.j = step.start.y;
		else
			return false;
		step.line.k = 1;//纵向
		step.line.i = step.start.x;
	}
	else if (dx == 1)
	{
		if (dy != 0)
			return false;
		step.line.k = 0;//横向
		step.line.i = step.start.y;
		step.line.j = step.end.x;
	}
	else if (dx == -1)
	{
		if (dy != 0)
			return false;
		step.line.k = 0;//横向
		step.line.i = step.start.y;
		step.line.j = step.start.x;
	}
	else
		return false;

	return true;
}

/**
 * LineToPoint - 将线的位置坐标转化为线的起止点坐标
 */
bool CDotsAndBoxes::LineToPoint(Step &step)
{
	if (step.line.k == 0)
	{
		step.start.x = step.line.j;
		step.start.y = step.line.i;
		step.end.x = step.start.x + 1;
		step.end.y = step.start.y;
	}
	else if (step.line.k == 1)
	{
		step.start.x = step.line.i;
		step.start.y = step.line.j;
		step.end.x = step.start.x;
		step.end.y = step.start.y + 1;
	}
	else
		return false;

	return true;
}
