#ifndef CNOGO_H_GAMEPLATFORM
#define CNOGO_H_GAMEPLATFORM

#include "..\include\CChess.h"

#define EMPTY 2
#define MARK 1


struct Step{
	Point point;
	BOOL side;
};
#define InitStep(step) { step.point.x=step.point.y=-1; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)


class CNoGo : public CChess
{
public:	

	CNoGo();
	~CNoGo();

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

	//绘制棋子
	bool DrawChess(HDC hBlcDC,HDC hCurBlcDC,HDC hWhtDC,HDC hCurWhtDC,int d);
	//初始化棋盘状态
	VOID InitBoard();
	//双方对弈
	BOOL SToS(int x,int y);
	//判断胜负
	bool WinOrLose();
	//判断棋步合法性
	bool FitRules();
	//显示招法历史
	VOID ShowStepHis(char *msg);
	//播放落子音效
	bool PlaySnd(int sel);

	int GetQiForPoint(int x,int y,int side);
	int GetQi(int x,int y,int side);

private:

	BYTE board[9][9];//棋盘
	int Biaoji[81];

	COLORREF BkColor;//背景色
	COLORREF BoardColor;//棋盘颜色
	HPEN hPen;//画笔句柄
	HFONT hFont;//字体句柄

	int d;//不围棋棋盘刻线间距
	double pixel;//不围棋棋盘刻线宽度
	int fWidth, fHeight;//字体宽、高

	stack<Step> stepStack;//着法栈
};

#endif