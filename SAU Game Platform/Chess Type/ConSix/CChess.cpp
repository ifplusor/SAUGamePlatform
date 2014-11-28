#include "CChess.h"


CChess::CChess()
{	
	player=BLACK;
	memset(StepNum, 0, sizeof(StepNum));
	memset(curCmd, 0, sizeof(curCmd));//写入引擎的消息
	memset(denCmd, 0, sizeof(denCmd));
}

CChess::~CChess()
{

}

HFONT CChess::CreateSimpleFont(int width,int height)//创建一般字体
{
	HFONT hFont;
	hFont=CreateFont(
		height,
		width,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		NULL
		);
	return hFont;
}

//画特定大小的点(圆形)
bool CChess::DrawPoint(HDC hDC,int r,COLORREF ptColor,COLORREF bkColor)
{	
	for(int i=0;i<=2*r;i++)
	{
		for(int j=0;j<=2*r;j++)
		{
			if((i-r)*(i-r)+(j-r)*(j-r)<r*r)
			{
				SetPixel(hDC,i,j,ptColor);
			}
			else
			{
				SetPixel(hDC,i,j,bkColor);
			}
		}		
	}
	return true;
}

bool CChess::InsideRect(const RECT* rt,const int &x,const int &y)//判断点是否在矩形内
{
	if((rt->left+ALLOW)<x && x<(rt->right-ALLOW) && (rt->top+ALLOW)<y && y<(rt->bottom-ALLOW))
		return true;
	else
		return false;
}

//绘制辅助位图(做透明处理)
bool CChess::DrawAssist(HDC hAssistDC,int d)
{
	for(int i=0;i<d;i++)
	{
		for(int j=0;j<d;j++)
		{
			if(pow((double)(i-d/2),2)+pow((double)(j-d/2),2)<=pow((double)d/2,2))
			{
				SetPixel(hAssistDC,i,j,RGB(255,255,255));
			}
			else
			{
				SetPixel(hAssistDC,i,j,RGB(0,0,0));
			}
		}
	}
	return true;
}

//设置文本框文本内容
VOID SetText(HWND hWnd,LPCTSTR str)
{
	SendMessage(hWnd,WM_SETTEXT,(WPARAM)0,(LPARAM)str);	//添加内容
	SendMessage(hWnd,EM_SETSEL,(WPARAM)-2,(LPARAM)-1);	//设置焦点到末尾
	SendMessage(hWnd,WM_VSCROLL,SB_BOTTOM,0);
}

VOID GetText(HWND hWnd,LPCTSTR str,int size)//获取文本框文本内容
{	
	SendMessage(hWnd,WM_GETTEXT,size,(LPARAM)str);
}