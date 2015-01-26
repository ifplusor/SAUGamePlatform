#ifndef CPHANYOMGO_H_GAMEPLATFORM
#define CPHANTOMGO_H_GAMEPLATFORM

#include "..\include\CChess.h"

#define EMPTY 2
#define MARK 1


struct Step{
	Point point;
	BOOL side;
};
#define InitStep(step) { step.point.x=step.point.y=-1; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)


class CPhantomGo : public CChess
{
public:	

	CPhantomGo(HINSTANCE hInst, HWND hWnd, char *LibPath);
	~CPhantomGo();

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
	virtual INT CancelMove();

private:

	BYTE board[9][9];//棋盘
	stack<Step> stepStack;//着法栈
	int Biaoji[81],NoQi[81];
	Point Jie;//劫点
	bool flagJ;//劫标记
	bool flagP;//pass标记，用于终局确定。
	char takeList[256];//提子列表
	int takeNum;//提子数量

	HPEN hPen;//画笔句柄
	HFONT hFont;//字体句柄
	HDC hBlcDC, hWhtDC, hMarkDC;

	int d;//不围棋棋盘刻线间距
	int pWidth;//不围棋棋盘刻线宽度
	int fWidth, fHeight;//字体宽、高

	//绘制棋子
	bool DrawChess();
	//播放落子音效
	bool PlaySnd(int tag);
	//初始化棋盘状态
	VOID InitBoard();
	//双方对弈
	BOOL SToS(Point point);
	//判断胜负
	bool WinOrLose();
	//判断棋步合法性
	bool FitRules();

	//提子测试
	bool Take(int x, int y, int side);
	//获取棋子的气数
	int GetQi(int x, int y, int side);
	int GetQiForPoint(int x, int y, int side);

};

#endif