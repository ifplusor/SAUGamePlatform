#pragma comment(lib,"winmm.lib")

#include "CDotsAndBoxes.h"
#include <stdio.h>


VOID __cdecl ErrorBox(LPTSTR ErrorInfo)//错误提示框
{
	CHAR error1[50],error2[20];
	strcpy(error1,ErrorInfo);
	sprintf(error2,"\n\nerror: %d",GetLastError());	
	strcat(error1,error2);	
	MessageBox(NULL,error1,"error",MB_OK);
}

CDotsAndBoxes::CDotsAndBoxes(HINSTANCE hInst, HWND hWnd, char *LibPath)
{
	this->hInst = hInst;
	this->hWnd = hWnd;
	strncpy(this->LibPath, LibPath, MAX_PATH - 1);

	BkColor = RGB(0, 0, 0);
	BoardColor=RGB(0,255,255);
	hPen = NULL;
	hFont = NULL;
	InitGame();
	count = -1;

	HDC hDC = GetDC(hWnd);
	hBlcDC = CreateCompatibleDC(hDC);
	hWhtDC = CreateCompatibleDC(hDC);
	hPtDC = CreateCompatibleDC(hDC);
}

CDotsAndBoxes::~CDotsAndBoxes()
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hPtDC);
}

VOID CDotsAndBoxes::SetBoard(RECT rtBoard)
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
	this->rtBoard = rtBoard;
	side = rtBoard.right - rtBoard.left + 1;
	d = side / 7;
	pixel = side / 300;
	if (pixel == 0)
		pixel = 1;
	hPen = CreatePen(PS_SOLID, pixel, RGB(255, 0, 0));
	fWidth = d / 3;
	fHeight = d * 2 / 3;
	hFont = CreateSimpleFont(fWidth, fHeight);
	pR = (int)(8 * pixel);
	pWidth = pHeight = (2 * pR);
	DrawChess();
}

VOID CDotsAndBoxes::DrawBoard(HDC hDC)
{
	int i,j,k;

	// 画点
	for(i=0;i<6;i++)
	{		
		for(j=0;j<6;j++)
		{
			BitBlt(hDC, rtBoard.left + (side + side*j) / 7 - pR, rtBoard.top + (side + side*i) / 7 - pR, pWidth, pHeight, hPtDC, 0, 0, SRCAND);
		}
	}

	HFONT hOldFont;
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	::SetTextColor(hDC, RGB(0, 0, 0));

	char letter[2],number[3];
	memset(letter,0,sizeof(letter));
	memset(number,0,sizeof(number));	
	for(i=0;i<6;i++)
	{			
		letter[0]='A'+i;
		itoa(i,number,10);
		TextOut(hDC, (int)(rtBoard.left + side*(i + 1) / 7 - fWidth / 2), rtBoard.top, letter, 1);
		TextOut(hDC, (int)(rtBoard.left + side*(i + 1) / 7 - fWidth / 2), rtBoard.bottom - fHeight, number, 1);
	}
	for(i=0;i<6;i++)
	{
		letter[0]='A'+i;
		itoa(i,number,10);

		TextOut(hDC, rtBoard.left, (int)(rtBoard.top + side*(i + 1) / 7 - fHeight / 2), letter, 1);
		TextOut(hDC, rtBoard.right - fWidth, (int)(rtBoard.top + side*(i + 1) / 7 - fHeight / 2), number, 1);

	}
	
	HPEN hBlcPen,hWhtPen;
	int lWidth=pR*2/3;
	hBlcPen=CreatePen(PS_SOLID,lWidth,RGB(0,0,0));
	hWhtPen=CreatePen(PS_SOLID,lWidth,RGB(255,255,255));

	// 画线
	for(k=0;k<2;k++)
	{
		for(i=0;i<6;i++)
		{
			for(j=0;j<5;j++)
			{
				if(line[k][i][j]!=EMPTY)
				{
					SelectObject(hDC, line[k][i][j]==BLACK? hBlcPen:hWhtPen);
					if(k==0)//横向
					{
						MoveToEx(hDC, (int)(rtBoard.left + side*(j + 1) / 7), (int)(rtBoard.top + side*(i + 1) / 7), NULL);
						LineTo(hDC, (int)(rtBoard.left + side*(j + 2) / 7), (int)(rtBoard.top + side*(i + 1) / 7));
					}
					if(k==1)//纵向
					{
						MoveToEx(hDC, (int)(rtBoard.left + side*(i + 1) / 7), (int)(rtBoard.top + side*(j + 1) / 7), NULL);
						LineTo(hDC, (int)(rtBoard.left + side*(i + 1) / 7), (int)(rtBoard.top + side*(j + 2) / 7));
					}						
				}																		
			}
		}
	}	

	// 画格
	int size = (int)(d*0.8);
	for (i = 0; i<5; i++)
	{
		for(j=0;j<5;j++)
		{
			if(box[i][j]==BLACK)
			{
				BitBlt(hDC, (int)(rtBoard.left + side*(i + 1) / 7 + (d - size) / 2), (int)(rtBoard.top + side*(j + 1) / 7 + (d - size) / 2), size, size, hBlcDC, 0, 0, SRCCOPY);
			}
			if(box[i][j]==WHITE)
			{
				BitBlt(hDC, (int)(rtBoard.left + side*(i + 1) / 7 + (d - size) / 2), (int)(rtBoard.top + side*(j + 1) / 7 + (d - size) / 2), size, size, hWhtDC, 0, 0, SRCCOPY);
			}
		}
	}

	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		SelectObject(hDC, hPen);
		IsLine(curStep.line)
		{
			MoveToEx(hDC, (int)(rtBoard.left + (side + side*curStep.start.x) / 7), (int)(rtBoard.top + (side + side*curStep.start.y) / 7), NULL);
			LineTo(hDC, (int)(rtBoard.left + (side + side*curStep.end.x) / 7), (int)(rtBoard.top + (side + side*curStep.end.y) / 7));
		}
		else IsChess(curStep.start)
		{
			MoveToEx(hDC, (int)(rtBoard.left + (side + side*curStep.start.x) / 7 - pR), (int)(rtBoard.top + (side + side*curStep.start.y) / 7), NULL);
			LineTo(hDC, (int)(rtBoard.left + (side + side*curStep.start.x) / 7 + pR), (int)(rtBoard.top + (side + side*curStep.start.y) / 7));
			MoveToEx(hDC, (int)(rtBoard.left + (side + side*curStep.start.x) / 7), (int)(rtBoard.top + (side + side*curStep.start.y) / 7 - pR), NULL);
			LineTo(hDC, (int)(rtBoard.left + (side + side*curStep.start.x) / 7), (int)(rtBoard.top + (side + side*curStep.start.y) / 7 + pR));
		}
	}

	DeleteObject(hBlcPen);
	DeleteObject(hWhtPen);
}

bool CDotsAndBoxes::DrawChess()
{
	HBITMAP hBlcBmp, hWhtBmp, hPtBmp;
	hBlcBmp = CreateCompatibleBitmap(hBlcDC, d, d);
	hWhtBmp = CreateCompatibleBitmap(hWhtDC, d, d);
	hPtBmp = CreateCompatibleBitmap(hPtDC, pWidth, pHeight);

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

BOOL CDotsAndBoxes::ProcessMove(char *moveCmd)
{				
	char *res;
	int pos=0,cs;
	Step tStep;
	int len=strlen("move ");
	if ((res = strstr(moveCmd, "move")) == NULL)//寻找move关键字
	{
		return 0;
	}
	else
	{		
		pos = (res - moveCmd);
		pos += len;
		
		InitStep(tStep);
		tStep.side = player;
		cs = 0;
		do{
			tStep.line.k = moveCmd[pos + cs * 3] - 'A';
			tStep.line.i = moveCmd[pos + cs * 3 + 1] - 'A';
			tStep.line.j = moveCmd[pos + cs * 3 + 2] - 'A';
			stepStack.push(tStep);
			if (!FitRules())//判断是否符合规则
			{
				sprintf(curCmd, "error\n");
				sprintf(denCmd, "\0");
				stepStack.pop();
				while (cs)//取消之前判别连线
				{
					tStep = stepStack.top();
					line[tStep.line.k][tStep.line.i][tStep.line.j] = EMPTY;
					stepStack.pop();
				}
				return -1;
			}
			line[tStep.line.k][tStep.line.i][tStep.line.j] = player;
			cs++;
		} while (HaveBox(tStep.line));

		InvalidateRect(hWnd, &rtBoard, FALSE);
		PlaySnd(1);

		moveCmd[pos + cs * 3] = '\0';
		ShowStepHis(moveCmd + pos);
		strncpy(denCmd, res, len + cs * 3);
		sprintf(denCmd + strlen(denCmd), "\n");//生成写消息
		sprintf(curCmd, "\0");
	}
	StepNum[player]++;//步数加1
	if(WinOrLose())//判断胜负
	{				
		sprintf(denCmd + strlen(denCmd), "end\n");
		sprintf(curCmd, "end\n");
		return 2;
	}	
	player = NEXTPLAYER(player);
	return 1;
}

bool CDotsAndBoxes::PlaySnd(int sel)
{
	char filename[MAX_PATH]={0};
	switch(sel)
	{
	case 0:
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\选子.wav");
		break;
	case 1:
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\连线.wav");
		break;
	case 2:
		strcpy(filename, LibPath);
		strcat(filename, "\\wav\\捕获格.wav");
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

VOID CDotsAndBoxes::InitGame()
{
	memset(StepNum, 0, sizeof(StepNum));

	player = BLACK;
	connectS = 0;
	InitBoard();
	count = 0;

	CleanStack(stepStack);
	return;
}

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
	InvalidateRect(hWnd, &rtBoard, FALSE);
	return;
}

BOOL CDotsAndBoxes::OnLButtonDown(int x,int y)
{
	Point point;
	if(!InsideRect(&rtBoard,x,y))
		return 2;
	if (count == -1)
		return 0;
	
	point.x = ((x - rtBoard.left) * 7 - side / 2) / side;//把棋盘坐标转换成数组坐标
	point.y = ((y - rtBoard.top) * 7 - side / 2) / side;
	if (point.x < 0 || point.x >= 6 || point.y < 0 || point.y >= 6)
		return 2;

	return SToS(point);
}

BOOL CDotsAndBoxes::SToS(Point point)
{
	Step tStep;

	if (count == 0)
	{
		if (HaveLine(point) == false)//点不能引出线，无效
			return -1;
		InitStep(tStep);
		tStep.start = point;
		tStep.side = player;
		stepStack.push(tStep);
		count = 1;
		::InvalidateRect(hWnd, &rtBoard, FALSE);
		PlaySnd(0);
	}
	else if (count == 1)
	{
		tStep = stepStack.top();
		tStep.end = point;
		if (PonitToLine(tStep) == false)//由两点不能映射为连线，无效
			return -1;
		stepStack.pop();
		line[tStep.line.k][tStep.line.i][tStep.line.j] = player;
		stepStack.push(tStep);
		count = -1;
		::InvalidateRect(hWnd, &rtBoard, FALSE);
		PlaySnd(1);
		return 1;
	}
	return 0;
}

INT CDotsAndBoxes::OkMove()
{
	Step tStep;
	tStep = stepStack.top();
	denCmd[0] = '\0';
	sprintf(denCmd + (5 + connectS * 3), "%c%c%c", tStep.line.k + 'A', tStep.line.i + 'A', tStep.line.j + 'A');
	if (HaveBox(tStep.line))//捕获格子不换手，并显示格子颜色
	{
		connectS++;//连续连线数目加1
		::InvalidateRect(hWnd, &rtBoard, FALSE);
		PlaySnd(2);
	}
	else
	{
		ShowStepHis(denCmd + 5);//换手后显示着法历史，并产生裁判指示
		sprintf(denCmd, "move");
		denCmd[4] = ' ';
		sprintf(denCmd + strlen(denCmd), "\n");
		StepNum[player]++;//换手时累计步数
		player = NEXTPLAYER(player);
		connectS = 0;//换手，连续连线数归零
	}
	if (WinOrLose())
	{
		ShowStepHis(denCmd + 5);
		sprintf(denCmd, "move");
		denCmd[4] = ' ';
		sprintf(denCmd + strlen(denCmd), "\nend\n");
		return 2;
	}
	count = 0;
	return 0;
}

VOID CDotsAndBoxes::CancelMove()
{
	Step tStep;
	if (count == 0)
		return;
	tStep = stepStack.top();
	stepStack.pop();
	IsLine(tStep.line)
	{
		line[tStep.line.k][tStep.line.i][tStep.line.j] = EMPTY;
	}
	InvalidateRect(hWnd, &rtBoard, FALSE);
	SendMessage(hWnd, WM_PAINT, NULL, NULL);
	count = 0;
	return;
}

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
	if (numEmt == 0)
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
		return true;
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
		return true;
	}
	return false;
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

bool CDotsAndBoxes::FitRules()
{
	Step tStep = stepStack.top();
	if (tStep.line.k < 0 || tStep.line.i > 1 || tStep.line.i < 0 || tStep.line.i > 5 || tStep.line.j < 0 || tStep.line.j > 4)
		return false;
	if (line[tStep.line.k][tStep.line.i][tStep.line.j] != EMPTY)
		return false;
	return true;
}