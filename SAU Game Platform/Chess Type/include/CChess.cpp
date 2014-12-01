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

//画特定大小的点(圆形)
bool CChess::DrawPoint(HDC hDC,int r,COLORREF ptColor,COLORREF bkColor)
{	
	for(int i=0;i<=2*r;i++)
	{
		for(int j=0;j<=2*r;j++)
		{
			if((i-r)*(i-r)+(j-r)*(j-r)<r*r)
				SetPixel(hDC,i,j,ptColor);
			else
				SetPixel(hDC,i,j,bkColor);
		}		
	}
	return true;
}

//绘制圆形辅助位图(做透明处理)
bool CChess::DrawAssist(HDC hAssistDC,int r)
{
	for(int i=0;i<r*2;i++)
	{
		for(int j=0;j<r*2;j++)
		{
			if ((i - r)*(i - r) + (j - r)*(j - r)<r*r)//圆内
				SetPixel(hAssistDC,i,j,RGB(255,255,255));//白色
			else//圆外
				SetPixel(hAssistDC,i,j,RGB(0,0,0));//黑色
		}
	}
	return true;
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

bool CChess::InsideRect(const RECT* rt,const int &x,const int &y)//判断点是否在矩形内
{
	if((rt->left+ALLOW)<x && x<(rt->right-ALLOW) && (rt->top+ALLOW)<y && y<(rt->bottom-ALLOW))
		return true;
	else
		return false;
}
