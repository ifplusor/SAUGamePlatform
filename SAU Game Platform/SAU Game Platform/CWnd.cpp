//FileName: CWnd.cpp
//Version: 1.1
//Date: 2013/10/24
//Description: SAU_GamePalform 程序窗口基类文件，内容包括：CWnd类声明的成员函数的定义。

#include "CWnd.h"

//构造函数
CWnd::CWnd()
{
	ClsName="WINDOW";
	WndName="Window";
	hWnd=NULL;
}

//析构函数
CWnd::~CWnd()
{

}

VOID CWnd::SetWindowStyle(int nIndex,LONG dwNewLong)
{
	SetWindowLong(hWnd,nIndex,dwNewLong);
}

//判断点在矩形内
bool CWnd::InsideRect(const RECT* rt,const int &x,const int &y)
{
	if((rt->left+ALLOW)<x && x<(rt->right-ALLOW) && (rt->top+ALLOW)<y && y<(rt->bottom-ALLOW))
		return true;
	else
		return false;
}

//创建字体
HFONT CWnd::CreateSimpleFont(int width,int height)
{
	HFONT hFont;
	hFont=CreateFont(
		height,//高度
		width,//宽度
		0,
		0,
		FW_NORMAL,//字体权值（粗细）
		FALSE,//斜体
		FALSE,//下划线
		FALSE,//删除线
		DEFAULT_CHARSET,//字符集
		OUT_DEFAULT_PRECIS,//输出精度
		CLIP_DEFAULT_PRECIS,//裁剪精度
		DEFAULT_QUALITY,//输出质量
		DEFAULT_PITCH,//字体间距和字体族
		NULL//字体名
		);
	return hFont;
}

//绘制静态框
VOID CWnd::DrawStatic(HDC hDC,RECT *rt,LPCTSTR text,COLORREF BkColor,COLORREF PenColor,SIZE edge)
{
	HPEN hPen,hOldPen;
	HFONT hFont,hOldFont;
	RECT rect;
	int backgroundMode=GetBkMode(hDC);
	COLORREF textColor=GetTextColor(hDC);

	SetBkMode(hDC,OPAQUE);
	SetBkColor(hDC,BkColor);

	hPen=CreatePen(PS_SOLID,1,PenColor);
	hOldPen=(HPEN)SelectObject(hDC,hPen);	
	MoveToEx(hDC,rt->left+edge.cx,rt->top+edge.cy,NULL);
	LineTo(hDC,rt->right-edge.cx,rt->top+edge.cy);
	LineTo(hDC,rt->right-edge.cx,rt->bottom-edge.cy);
	LineTo(hDC,rt->left+edge.cx,rt->bottom-edge.cy);
	LineTo(hDC,rt->left+edge.cx,rt->top+edge.cy);

	hFont=CreateSimpleFont(edge.cx,2*edge.cy);
	hOldFont=(HFONT)SelectObject(hDC,hFont);
	rect.left=(int)(rt->left+(rt->right-rt->left-strlen(text)*edge.cx)/2);
	rect.right=rect.left+strlen(text)*edge.cx;
	rect.top=rt->top;rect.bottom=rt->top+edge.cy*2;
	DrawText(hDC,text,strlen(text),&rect,DT_CENTER);

	SetBkColor(hDC,textColor);
	SetBkMode(hDC,backgroundMode);
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC,hOldFont);
	DeleteObject(hFont);
	return;
}
