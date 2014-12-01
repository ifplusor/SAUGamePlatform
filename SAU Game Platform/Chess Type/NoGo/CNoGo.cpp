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

CNoGo::CNoGo()
{
	BkColor = RGB(0, 0, 0);
	BoardColor = RGB(255, 255, 0);
	hPen = NULL;
	hFont = NULL;
	InitGame();
}

CNoGo::~CNoGo()
{
	if (hPen != NULL)
		DeleteObject(hPen);
	if (hFont != NULL)
		DeleteObject(hFont);
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
	for(i=0;i<9;i++)
	{			
		letter[0]='A'+i;
		itoa(i+1,number,10);
		
		TextOut(hDC, (int)(rtBoard.left + side*(i + 1) / 10 - fWidth / 2), rtBoard.top, letter, 1);//绘制文字				
		TextOut(hDC, (int)(rtBoard.left + side*(i + 1) / 10 - fWidth), rtBoard.bottom - fHeight, number, 2);

		MoveToEx(hDC, rtBoard.left + d, (int)(rtBoard.top + side*(i + 1) / 10), NULL);//绘制线
		LineTo(hDC, rtBoard.right - d, (int)(rtBoard.top + side*(i + 1) / 10));
	}
	for(i=0;i<9;i++)
	{
		letter[0]='A'+i;
		itoa(i+1,number,10);

		TextOut(hDC, rtBoard.left + fWidth, (int)(rtBoard.top + side*(i + 1) / 10 - fHeight / 2), letter, 1);
		TextOut(hDC, rtBoard.right - fWidth * 2, (int)(rtBoard.top + side*(i + 1) / 10 - fHeight / 2), number, 2);
		
		MoveToEx(hDC, rtBoard.left + side*(i + 1) / 10, rtBoard.top + d, NULL);
		LineTo(hDC, rtBoard.left + side*(i + 1) / 10, rtBoard.bottom - d);
	}
	
	HDC hAssistDC,hBlcDC,hWhtDC,hCurBlcDC,hCurWhtDC;
	hAssistDC=CreateCompatibleDC(hDC);
	hBlcDC=CreateCompatibleDC(hDC);
	hWhtDC=CreateCompatibleDC(hDC);
	hCurBlcDC=CreateCompatibleDC(hDC);
	hCurWhtDC=CreateCompatibleDC(hDC);	

	HBITMAP hAssistBmp,hBlcBmp,hWhtBmp,hCurBlcBmp,hCurWhtBmp;
	hAssistBmp=CreateCompatibleBitmap(hDC,d,d);
	hBlcBmp=CreateCompatibleBitmap(hDC,d,d);
	hWhtBmp=CreateCompatibleBitmap(hDC,d,d);
	hCurBlcBmp=CreateCompatibleBitmap(hDC,d,d);
	hCurWhtBmp=CreateCompatibleBitmap(hDC,d,d);	

	SelectObject(hAssistDC,hAssistBmp);
	SelectObject(hBlcDC,hBlcBmp);
	SelectObject(hWhtDC,hWhtBmp);
	SelectObject(hCurBlcDC,hCurBlcBmp);
	SelectObject(hCurWhtDC,hCurWhtBmp);	

	DrawAssist(hAssistDC,d/2);
	DrawChess(hBlcDC,hWhtDC,hCurBlcDC,hCurWhtDC,d);//绘制棋子
	
	
	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			if(board[i][j]==BLACK)
			{				
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 10 - d / 2, rtBoard.top + side*(j + 1) / 10 - d / 2, d, d, hAssistDC, 0, 0, SRCPAINT);
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 10 - d / 2, rtBoard.top + side*(j + 1) / 10 - d / 2, d, d, hBlcDC, 0, 0, SRCAND);
			}
			else if(board[i][j]==WHITE)
			{				
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 10 - d / 2, rtBoard.top + side*(j + 1) / 10 - d / 2, d, d, hAssistDC, 0, 0, SRCPAINT);
				BitBlt(hDC, rtBoard.left + side*(i + 1) / 10 - d / 2, rtBoard.top + side*(j + 1) / 10 - d / 2, d, d, hWhtDC, 0, 0, SRCAND);
			}
		}
	}
	
	if (!stepStack.empty())
	{
		Step curStep = stepStack.top();
		if (curStep.side == BLACK)
		{
			int x1 = curStep.point.x; int y1 = curStep.point.y;
			BitBlt(hDC, rtBoard.left + side*(x1 + 1) / 10 - d / 2, rtBoard.top + side*(y1 + 1) / 10 - d / 2, d, d, hAssistDC, 0, 0, SRCPAINT);
			BitBlt(hDC, rtBoard.left + side*(x1 + 1) / 10 - d / 2, rtBoard.top + side*(y1 + 1) / 10 - d / 2, d, d, hCurBlcDC, 0, 0, SRCAND);
		}
		else if (curStep.side == WHITE)
		{
			int x1 = curStep.point.x; int y1 = curStep.point.y;
			BitBlt(hDC, rtBoard.left + side*(x1 + 1) / 10 - d / 2, rtBoard.top + side*(y1 + 1) / 10 - d / 2, d, d, hAssistDC, 0, 0, SRCPAINT);
			BitBlt(hDC, rtBoard.left + side*(x1 + 1) / 10 - d / 2, rtBoard.top + side*(y1 + 1) / 10 - d / 2, d, d, hCurWhtDC, 0, 0, SRCAND);
		}
	}

	DeleteObject(hAssistBmp);
	DeleteObject(hBlcBmp);
	DeleteObject(hWhtBmp);
	DeleteObject(hCurBlcBmp);
	DeleteObject(hCurBlcBmp);

	DeleteDC(hAssistDC);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hCurBlcDC);
	DeleteDC(hCurWhtDC);
}

bool CNoGo::DrawChess(HDC hBlcDC,HDC hWhtDC,HDC hCurBlcDC,HDC hCurWhtDC,int d)
{
	double pixel=0.8,shadow=0.7;
	BOOL Alias=true;

	int red=255-GetRValue(BoardColor);
	int green=255-GetGValue(BoardColor);
	int blue=255-GetBValue(BoardColor);

	COLORREF pb,pw;
	double di,dj,d2=(double)d/2.0-5e-1,r=d2-2e-1,f=sqrt(3.0);
	double x,y,z,xr,xg,hh;
	int g;

	for (int i=0; i<d; i++)
	{
		for (int j=0; j<d; j++)
		{	
			di=i-d2; dj=j-d2;
			hh=r-sqrt(di*di+dj*dj);
			if (hh>=0)
			{	
				z=r*r-di*di-dj*dj;
			    if (z>0) z=sqrt(z)*f;
			    else z=0;
				x=di; y=dj;
				xr=sqrt(6*(x*x+y*y+z*z));
				xr=(2*z-x+y)/xr;
				if (xr>0.9) xg=(xr-0.9)*10;
				else xg=0;
				if (hh>pixel || !Alias)
				{
					g=(int)(10+10*xr+xg*140);
					pb=(g<<16)|(g<<8)|g;
					g=(int)(200+10*xr+xg*45);
					pw=(g<<16)|(g<<8)|g;
				}
				else
				{	hh=(pixel-hh)/pixel;
					g=(int)(10+10*xr+xg*140);
					double shade;
					if (di-dj<r/3) shade=1;
					else shade=shadow;

					pb=(//(255<<24)|
					    (((int)((1-hh)*g+hh*shade*red))<<16)
						|(((int)((1-hh)*g+hh*shade*green))<<8)
						|((int)((1-hh)*g+hh*shade*blue)));

					g=(int)(200+10*xr+xg*45);

					pw=(//(255<<24)|
					    (((int)((1-hh)*g+hh*shade*red))<<16)
						|(((int)((1-hh)*g+hh*shade*green))<<8)
						|((int)((1-hh)*g+hh*shade*blue)));
				}
				//pb=RGB(0,0,0);
				//pw=RGB(255,255,255);
			}
			else 
			{
				pb=pw=RGB(255,255,255);				
			}
					
			SetPixel(hBlcDC,j,i,pb);
			SetPixel(hWhtDC,j,i,pw);			
			
			SetPixel(hCurBlcDC,j,i,pb);
			SetPixel(hCurWhtDC,j,i,pw);			
		}
	}
	HPEN hPen=CreatePen(PS_SOLID,1,RGB(255,0,0));

	HPEN hOldPen=(HPEN)SelectObject(hCurBlcDC,hPen);
	MoveToEx(hCurBlcDC,d/2,d/4,NULL);LineTo(hCurBlcDC,d/2,d*3/4);
	MoveToEx(hCurBlcDC,d/4,d/2,NULL);LineTo(hCurBlcDC,d*3/4,d/2);	
	SelectObject(hCurBlcDC,hOldPen);	
	
	hOldPen=(HPEN)SelectObject(hCurWhtDC,hPen);
	MoveToEx(hCurWhtDC,d/2,d/4,NULL);LineTo(hCurWhtDC,d/2,d*3/4);
	MoveToEx(hCurWhtDC,d/4,d/2,NULL);LineTo(hCurWhtDC,d*3/4,d/2);	
	SelectObject(hCurWhtDC,hOldPen);	 
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
	if(WinOrLose())//判断胜负
	{
		sprintf(denCmd + strlen(denCmd), "end\n");
		sprintf(curCmd, "end\n");
		return 2;
	}	
	StepNum[player]++;//累计步数
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

VOID CNoGo::ShowStepHis(char *msg)
{
	char step[1000];	
	memset(step,0,sizeof(step));
	if(player==BLACK)
	{
		strcpy(step,"Black: ");
	}
	else if(player==WHITE)
	{
		strcpy(step,"White: ");
	}	
	strcat(step,msg);
	SendMessage(hWnd,GM_SHOWSTEP,(WPARAM)step,(LPARAM)player);
	return;
}

VOID CNoGo::InitGame()//游戏初始化
{
	memset(StepNum,0,sizeof(StepNum));
	player=BLACK;			
	InitBoard();	//初始化棋盘
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
	if (!InsideRect(&rtBoard, x, y) || count == -1)
		return 0;		

	x=(int)((x-rtBoard.left-side/20)*10/side);//把棋盘坐标转换成数组坐标
	y=(int)((y-rtBoard.top-side/20)*10/side);
	if (x >= 0 && x<9 && y >= 0 && y<9)
		return SToS(x, y);
	return 0;
}

BOOL CNoGo::SToS(int x,int y)
{
	Step tStep;
	if (board[x][y] != EMPTY)
		return -1;
	board[x][y]=player;	
	tStep.point.x = x;
	tStep.point.y = y;
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
