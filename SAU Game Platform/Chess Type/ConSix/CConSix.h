#ifndef CCONSIX_H_GAMEPLATFORM
#define CCONSIX_H_GAMEPLATFORM

#include "..\include\CChess.h"


#define EMPTY 2//空


struct Step{
	Point first, second;
	BOOL side;
};
#define InitStep(step) { step.first.x=step.first.y=step.second.x=step.second.y=-1; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)

class CConSix : public CChess
{
public:

	CConSix();
	~CConSix();

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

	BYTE board[19][19];//棋盘信息结构

	bool first_hand;//第一手标志(由于六子棋第一手落一子，以后每手落两子，故需要标记)

	COLORREF BkColor;//背景色
	COLORREF BoardColor;//棋盘颜色
	HPEN hPen;//画笔句柄
	HFONT hFont;//字体句柄

	int d;//六子棋棋盘刻线间距
	double pixel;//六子棋棋盘刻线宽度
	int fWidth, fHeight;//字体宽、高

	stack<Step> stepStack;//着法栈

	//绘制棋子
	bool DrawChess(HDC hBlcDC, HDC hCurBlcDC, HDC hWhtDC, HDC hCurWhtDC, int d);
	//初始化棋盘状态
	VOID InitBoard();
	//双方对弈
	BOOL SToS(int x, int y);
	//判断胜负
	bool WinOrLose();
	//判断棋步合法性
	bool FitRules();
	//显示招法历史
	VOID ShowStepHis(char *msg);
	//播放落子音效
	bool PlaySnd(int sel);

};

#endif