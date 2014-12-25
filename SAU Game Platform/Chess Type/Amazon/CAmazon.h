#ifndef CAMAZON_H_GAMEPLATFORM
#define CAMAZON_H_GAMEPLATFORM

#include "..\include\CChess.h"


#define EMPTY 2//空
#define	BAR 3//障碍

struct Step
{
	Point first, second, third;
	BYTE side;
};
#define InitStep(step) { step.first.x=step.first.y=step.second.x=step.second.y=step.third.x=step.third.y=-1; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)


class CAmazon : public CChess
{
public:	

	CAmazon(HINSTANCE hInst, HWND hWnd, char *LibPath);
	~CAmazon();

	//设置棋盘大小
	virtual VOID SetBoard(RECT rtBoard);
	//绘制棋盘
	virtual VOID DrawBoard(HDC hDC);
	//处理引擎行棋命令  返回值：  -1：行棋违规  0：未找到关键字  1：获取成功  2：胜负手
	virtual INT ProcessMove(char *moveCmd);
	//初始化棋局
	virtual VOID InitGame();
	//响应鼠标单击消息
	virtual BOOL OnLButtonDown(int x, int y);
	//确认着法
	virtual INT OkMove();
	//取消着法
	virtual VOID CancelMove();

private:

	BYTE board[10][10];//10*10棋盘

	COLORREF BkColor;//背景色
	COLORREF BoardColor;//棋盘颜色
	HPEN hPen;//画笔句柄
	HFONT hFont;//字体句柄
	HBRUSH hBrush;//棋盘画刷
	HDC hBlcDC, hWhtDC, hBarDC, hMarkDC;

	int d;//亚马逊棋棋盘刻线间距
	double pixel;//亚马逊棋棋盘刻线宽度
	int fWidth,fHeight;//字体宽、高

	stack<Step> stepStack;//着法栈

	//绘制棋子
	bool DrawChess();
	//初始化棋盘状态
	VOID InitBoard();
	//双方对弈
	BOOL SToS(Point point);
	//判断胜负
	bool WinOrLose();
	//判断棋步合法性
	bool JudgeRule(Point src, Point des);
	bool FitRules();
	//显示招法历史
	VOID ShowStepHis(char *msg);
	//播放落子音效
	bool PlaySnd(int sel);
};

#endif