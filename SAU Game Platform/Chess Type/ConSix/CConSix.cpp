#pragma comment(lib,"winmm.lib")

#include "CConSix.h"
#include <stdio.h>

VOID __cdecl ErrorBox(LPTSTR ErrorInfo)//错误提示框
{
	CHAR error1[50],error2[20];
	strcpy(error1,ErrorInfo);
	sprintf(error2,"\n\nerror: %d",GetLastError());	
	strcat(error1,error2);	
	MessageBox(NULL,error1,"error",MB_OK);
}

VOID __cdecl MsgBox(LPTSTR msg)//消息框
{
	MessageBox(NULL,msg,"OK",MB_OK);
}

CConSix::CConSix()
{
	BkColor=RGB(0,0,0);
	BoardColor=RGB(255,255,0);
	InitGame();
}

CConSix::~CConSix()
{

}

bool CConSix::DrawBoard(HDC hDC)//绘制棋盘
{
	int i,j;
	HPEN hOldPen;
	HFONT hOldFont;
	hOldPen=(HPEN)SelectObject(hDC,hPen);
	hOldFont=(HFONT)SelectObject(hDC,hFont);	

	::SetTextColor(hDC,RGB(0,0,0));

	char letter[2],number[3];
	memset(letter,0,sizeof(letter));
	memset(number,0,sizeof(number));
	//绘制棋盘
	for(i=0;i<19;i++)
	{			
		letter[0]='A'+i;
		itoa(i,number,10);
		
		TextOut(hDC,(int)(rtBoard.left+side*(i+1)/20-fWidth/2),rtBoard.top,letter,1);//绘制文字				
		TextOut(hDC,(int)(rtBoard.left+side*(i+1)/20-fWidth),rtBoard.bottom-fHeight,number,2);		

		MoveToEx(hDC,rtBoard.left+d,(int)(rtBoard.top+side*(i+1)/20),NULL);//绘制线
		LineTo(hDC,rtBoard.right-d,(int)(rtBoard.top+side*(i+1)/20));
	}
	for(i=0;i<19;i++)
	{
		letter[0]='A'+i;
		itoa(i,number,10);

		TextOut(hDC,rtBoard.left+fWidth,(int)(rtBoard.top+side*(i+1)/20-fHeight/2),letter,1);						
		TextOut(hDC,rtBoard.right-fWidth*2,(int)(rtBoard.top+side*(i+1)/20-fHeight/2),number,2);	
		
		MoveToEx(hDC,rtBoard.left+side*(i+1)/20,rtBoard.top+d,NULL);
		LineTo(hDC,rtBoard.left+side*(i+1)/20,rtBoard.bottom-d);
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

	DrawAssist(hAssistDC,d);
	DrawChess(hBlcDC,hWhtDC,hCurBlcDC,hCurWhtDC,d);//绘制棋子
	
	
	for(i=0;i<19;i++)
	{
		for(j=0;j<19;j++)
		{
			if(board[i][j]==BLACK)
			{				
				BitBlt(hDC,rtBoard.left+side*(i+1)/20-d/2,rtBoard.top+side*(j+1)/20-d/2,d,d,hAssistDC,0,0,SRCPAINT);
				BitBlt(hDC,rtBoard.left+side*(i+1)/20-d/2,rtBoard.top+side*(j+1)/20-d/2,d,d,hBlcDC,0,0,SRCAND);				
			}
			else if(board[i][j]==WHITE)
			{				
				BitBlt(hDC,rtBoard.left+side*(i+1)/20-d/2,rtBoard.top+side*(j+1)/20-d/2,d,d,hAssistDC,0,0,SRCPAINT);
				BitBlt(hDC,rtBoard.left+side*(i+1)/20-d/2,rtBoard.top+side*(j+1)/20-d/2,d,d,hWhtDC,0,0,SRCAND);
			}
		}
	}
	
	if(curStep.side==BLACK)
	{
		int x1=curStep.first.x;int y1=curStep.first.y;
		int x2=curStep.second.x;int y2=curStep.second.y;
		if(board[x1][y1]==BLACK)
		{
			BitBlt(hDC,rtBoard.left+side*(x1+1)/20-d/2,rtBoard.top+side*(y1+1)/20-d/2,d,d,hAssistDC,0,0,SRCPAINT);
			BitBlt(hDC,rtBoard.left+side*(x1+1)/20-d/2,rtBoard.top+side*(y1+1)/20-d/2,d,d,hCurBlcDC,0,0,SRCAND);
		}			
		if(x2!=-1 &&board[x2][y2]==BLACK)
		{
			BitBlt(hDC,rtBoard.left+side*(x2+1)/20-d/2,rtBoard.top+side*(y2+1)/20-d/2,d,d,hAssistDC,0,0,SRCPAINT);
			BitBlt(hDC,rtBoard.left+side*(x2+1)/20-d/2,rtBoard.top+side*(y2+1)/20-d/2,d,d,hCurBlcDC,0,0,SRCAND);
		}			
	}
	else if(curStep.side==WHITE)
	{
		int x1=curStep.first.x;int y1=curStep.first.y;
		int x2=curStep.second.x;int y2=curStep.second.y;
		if(board[x1][y1]==WHITE)
		{
			BitBlt(hDC,rtBoard.left+side*(x1+1)/20-d/2,rtBoard.top+side*(y1+1)/20-d/2,d,d,hAssistDC,0,0,SRCPAINT);
			BitBlt(hDC,rtBoard.left+side*(x1+1)/20-d/2,rtBoard.top+side*(y1+1)/20-d/2,d,d,hCurWhtDC,0,0,SRCAND);
		}			
		if(board[x2][y2]==WHITE)
		{
			BitBlt(hDC,rtBoard.left+side*(x2+1)/20-d/2,rtBoard.top+side*(y2+1)/20-d/2,d,d,hAssistDC,0,0,SRCPAINT);
			BitBlt(hDC,rtBoard.left+side*(x2+1)/20-d/2,rtBoard.top+side*(y2+1)/20-d/2,d,d,hCurWhtDC,0,0,SRCAND);
		}
	}

	DeleteObject(hAssistBmp);
	DeleteObject(hBlcBmp);
	DeleteObject(hWhtBmp);
	DeleteObject(hCurBlcBmp);
	DeleteObject(hCurWhtBmp);

	DeleteDC(hAssistDC);
	DeleteDC(hBlcDC);
	DeleteDC(hWhtDC);
	DeleteDC(hCurBlcDC);
	DeleteDC(hCurWhtDC);
	return true;
}


bool CConSix::DrawChess(HDC hBlcDC,HDC hWhtDC,HDC hCurBlcDC,HDC hCurWhtDC,int d)
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
	DeleteObject(hPen);

	return true;
}

//处理引擎消息
BOOL CConSix::ProcessMsg(char *msg)
{	
	step sp;
	char *res;
	int pos=0;
	int len=strlen("move ");
	if((res=strstr(msg,"move"))==NULL)//寻找move关键字
	{
		return 0;//未找到关键字
	}
	else
	{		
		pos=(res-msg);
		pos+=len;
		
		sp.first.x=msg[pos]-'A';
		sp.first.y=msg[pos+1]-'A';
		sp.second.x=msg[pos+2]-'A';
		sp.second.y=msg[pos+3]-'A';
		sp.side=player;			

		msg[pos+4]='\0';

		if(!FitRules(sp))//判断是否符合规则
		{
			sprintf(wMMsg,"error\n");
			sprintf(wDMsg,"\0");
			return -1;//行棋违规
		}
		board[sp.first.x][sp.first.y]=sp.side;
		if(first_hand==true)
		{
			first_hand=false;
		}
		else
		{
			if(sp.second.x!=-1&&sp.second.y!=-1)
				board[sp.second.x][sp.second.y]=sp.side;
		}
		memcpy(&curStep,&sp,sizeof(step));
		InvalidateRect(hWnd,&rtBoard,FALSE);	
		SendMessage(hWnd,WM_PAINT,NULL,NULL);
//		PlaySnd(0);
		ShowStepHis(msg+pos);			

		memset(wDMsg,0,sizeof(wDMsg));
		sprintf(wDMsg,"move %c%c%c%c\n",sp.first.x+'A',sp.first.y+'A',sp.second.x+'A',sp.second.y+'A');//生成写消息
		sprintf(wMMsg,"\0");
	}		
	if(WinOrLose(player))//判断胜负
	{
		sprintf(wDMsg+strlen(wDMsg),"end\n");
		sprintf(wMMsg,"end\n");
		return 2;//分出胜负
	}	
	StepNum[player]++;//累计步数
	player=NEXT(player);
	return 1;//获取成功
}

bool CConSix::PlaySnd(int sel)
{
	char filename[MAX_PATH]={0};
	strcpy(filename,gameset->CurDir);
	switch(sel)
	{
	case 0:
		strcat(filename,"\\wav\\ConSix\\落子.wav");
		break;
	default:
		break;
	}	
	if(!PlaySound(filename,NULL,SND_ASYNC|SND_NOWAIT |SND_FILENAME))
	{
		ErrorBox("PlaySound failed");
		return false;
	}
	return true;
}

VOID CConSix::ShowStepHis(char *msg)
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

VOID CConSix::InitGame()//游戏初始化
{
	memset(StepNum,0,sizeof(StepNum));

	player=BLACK;	
	first_hand=true;
	count=0;

	InitBoard();	//初始化棋盘
	return;
}

VOID CConSix::InitBoard()
{
	int i,j;
	for(i=0;i<19;i++)
	{
		for(j=0;j<19;j++)
		{
			board[i][j]=EMPTY;
		}
	}

	InvalidateRect(hWnd,&rtBoard,FALSE);//刷新棋盘
	return;
}

BOOL CConSix::OnLButtonDown(int x,int y)
{
	if(!InsideRect(&rtBoard,x,y))
		return 0;		

	x=(int)((x-rtBoard.left-side/40)*20/side);//把棋盘坐标转换成数组坐标
	if(gameset->BasePt==UPLEFT)
	{			
		y=(int)((y-rtBoard.top-side/40)*20/side);
	}	
	else if(gameset->BasePt==DOWNLEFT)
	{		
		y=18-(int)((y-rtBoard.top-side/40)*20/side);
	}

	return SToS(x,y);
}

BOOL CConSix::SToS(int x,int y)
{
	if(board[x][y]!=EMPTY)
		return -1;//着法非法
	board[x][y]=player;	

	count++;
	if(count==1)//点击第一次
	{			
		strcpy(wDMsg,"move ");
		wDMsg[5]='A'+x;
		wDMsg[6]='A'+y;		
		if(first_hand)//人是第一手
		{
			count=0;

			wDMsg[7]='A'+-1;
			wDMsg[8]='A'+-1;	

			wDMsg[9]='\0';
			InvalidateRect(hWnd,&rtBoard,FALSE);
			SendMessage(hWnd,WM_PAINT,NULL,NULL);
			PlaySnd(0);	
			ShowStepHis(wDMsg+5);

			wDMsg[9]='\n';
			wDMsg[10]='\0';								

			curStep.first.x=x;curStep.first.y=y;curStep.side=player;

			StepNum[player]++;//累计步数

			player=NEXT(player);
			first_hand=false;
			return 1;
		}
		else
		{
			curStep.first.x=x;curStep.first.y=y;curStep.side=EMPTY;
			InvalidateRect(hWnd,&rtBoard,FALSE);	
			SendMessage(hWnd,WM_PAINT,NULL,NULL);
			PlaySnd(0);	
		}
	}
	else//点击第二次
	{
		count=0;

		wDMsg[7]='A'+x;
		wDMsg[8]='A'+y;
		
		wDMsg[9]='\0';
		InvalidateRect(hWnd,&rtBoard,FALSE);	
		SendMessage(hWnd,WM_PAINT,NULL,NULL);
		PlaySnd(0);	
		ShowStepHis(wDMsg+5);

		wDMsg[9]='\n';
		wDMsg[10]='\0';				

		curStep.second.x=x;curStep.second.y=y;curStep.side=player;

		StepNum[player]++;//累计步数

	}

	if(WinOrLose(player))//判断胜负  因为需要填充wMsg，故后做胜负判断
	{
		if(count==1)
		{
			wDMsg[7]='A'-1;
			wDMsg[8]='A'-1;	
			wDMsg[9]='\0';
			ShowStepHis(wDMsg+5);
			wDMsg[9]='\n';
			wDMsg[10]='\0';								
			player=NEXT(player);
		}
		sprintf(wDMsg+strlen(wDMsg),"end\n");
		return 2;
	}
	if(count==0)
	{
		player=NEXT(player);
		return 1;
	}
	return 0;//返回落子步数
}

bool CConSix::WinOrLose(BYTE side)//判断胜负
{	
	bool win=false;
	int i,j;
	if(win==false)
	{
		for(i=0;i<19;i++) //扫描竖线有没有连六
		{
			for(j=0;j<14;j++)
			{
				if(board[i][j]==side &&board[i][j+1]==side &&board[i][j+2]==side &&board[i][j+3]==side &&board[i][j+4]==side &&board[i][j+5]==side )
				  win=true;		  
			}
		}	  
	}	
	if(win==false)	
	{
		for(i=0;i<19;i++) //扫描横线有没有连六
		{
			for(j=0;j<14;j++)
			{
				if(board[j][i]==side &&board[j+1][i]==side &&board[j+2][i]==side &&board[j+3][i]==side &&board[j+4][i]==side &&board[j+5][i]==side )
				  win=true;        
			}
		}	  
	}
	if(win==false)	
	{
		for(i=0;i<14;i++) //扫描45度斜线
		{
			for(j=5;j<19;j++)
			{
				if(board[i][j]==side &&board[i+1][j-1]==side &&board[i+2][j-2]==side &&board[i+3][j-3]==side &&board[i+4][j-4]==side &&board[i+5][j-5]==side )
				  win=true; 
			}
		}	   
	}	
	if(win==false)	
	{
		for(i=0;i<14;i++) //扫描135度斜线
		{
			for(j=13;j>-1;j--)
			{
				if(board[i][j]==side &&board[i+1][j+1]==side &&board[i+2][j+2]==side &&board[i+3][j+3]==side &&board[i+4][j+4]==side &&board[i+5][j+5]==side )
				  win=true;  
			}
		}
	}		   

	if(win==true)
	{
		SendMessage(hWnd,GM_WINLOSE,(WPARAM)(StepNum[BLACK]<<16)+StepNum[WHITE],(LPARAM)side);
	}	
	return win;
}

bool CConSix::FitRules(step sp)//是否符合规则
{
	if(sp.first.x<0||sp.first.x>18||sp.first.y<0||sp.first.y>18)
	{
		MsgBox("Break rules");
		return false;
	}
	if(first_hand==true)
	{
		if(board[sp.first.x][sp.first.y]!=EMPTY)
		{
			MsgBox("Break rules");
			return false;
		}	
	}
	else
	{
		if(sp.second.x==-1&&sp.second.y==-1)
		{
			if(board[sp.first.x][sp.first.y]!=EMPTY)
				return false;
			else
				return true;
		}
		if(sp.second.x<0||sp.second.x>18||sp.second.y<0||sp.second.y>18)
		{
			MsgBox("Break rules");
			return false;
		}
		if(board[sp.first.x][sp.first.y]!=EMPTY||board[sp.second.x][sp.second.y]!=EMPTY)
		{
			MsgBox("Break rules");
			return false;
		}
	}
	return true;
}

