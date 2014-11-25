//FileName: CWnd.h
//Version: 1.1
//Date: 2013/10/24
//Description: SAU_GamePalform 程序窗口基类头文件，内容包括：CWnd类的定义及其成员函数的声明。

#ifndef CWND_H_GAMEPLATFORM
#define CWND_H_GAMEPLATFORM


#define ALLOW 5


#include "Game Platform.h"

class CWnd//窗口类
{
public:
	HWND hWnd;//窗口句柄
	
	RECT rtWindow;//窗口RECT
	RECT rtClient;//客户区RECT

	//构造函数
	CWnd();
	//析构函数
	~CWnd();		

	//注册窗口类
	virtual BOOL RegisterWnd(HINSTANCE hInst)=0;
	//创建窗体
	virtual BOOL CreateWnd(HWND hParentWnd=NULL,HMENU hMenu=NULL)=0;
	virtual LRESULT WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)=0;
	

	//设置窗口类型
	VOID SetWindowStyle(int nIndex,LONG dwNewLong);
	//判断点在矩形内
	static bool InsideRect(const RECT* rt,const int &x,const int &y);
	//创建字体
	static HFONT CreateSimpleFont(int width,int height);
	//绘制静态框
	static VOID DrawStatic(HDC hDC,RECT *rt,LPCTSTR text,COLORREF BkColor,COLORREF PenColor,SIZE edge);

protected:
	LPTSTR WndName;//窗口标题
	LPTSTR ClsName;	//窗口类名


	//响应消息

	//响应窗体创建
	virtual VOID OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)=0;
	//响应绘图
	virtual VOID OnPaint(WPARAM wParam,LPARAM lParam)=0;
	//响应擦除背景
	virtual BOOL OnEraseBkgnd(WPARAM wParam,LPARAM lParam)=0;	
	//响应窗口大小改变
	virtual VOID OnSize(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnNotify(WPARAM wParam,LPARAM lParam)=0;
	//响应控件消息
	virtual VOID OnCommand(WPARAM wParam,LPARAM lParam)=0;
	//响应菜单项选择（修改状态栏显示）
	virtual VOID OnMenuSelect(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnLButtonDown(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnLButtonDbClick(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnRButtonDown(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnMouseMove(WPARAM wParam,LPARAM lParam)=0;
	//键盘按键消息
	virtual VOID OnKeyDown(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnTimer(WPARAM wParam,LPARAM lParam)=0;
	//响应窗口关闭消息
	virtual VOID OnClose(WPARAM wParam,LPARAM lParam)=0;
	//响应窗口销毁消息
	virtual VOID OnDestroy(WPARAM wParam,LPARAM lParam)=0;
};

#endif