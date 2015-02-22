#ifndef CSURAKARTA_H_GAMEPLATFORM
#define CSURAKARTA_H_GAMEPLATFORM

#include <vector>
#include "../include/CChess.h"

using namespace std;


#define EMPTY 2

struct PointF
{
	double x;
	double y;
};

struct PointS
{
	int x, y;
	int side;
};

struct Step{
	Point start,end;
	bool flagE;//吃子标记
	BOOL side;
};
#define InitStep(step) { step.start.x=step.start.y=step.end.x=step.end.y=-1;step.flagE=false; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)


class CSurakarta : public CChess
{
public:	

	CSurakarta(HINSTANCE hInst, HWND hWnd, char *LibPath);
	~CSurakarta();

	//设置棋盘大小
	virtual VOID SetBoard(RECT rtBoard);
	//绘制棋盘
	virtual VOID DrawBoard(HDC hDC);
	//初始化棋局
	virtual VOID InitGame();
	//处理引擎行棋命令  返回值：  -1：行棋违规  0：未找到关键字  1：获取成功  2：胜负手
	virtual INT ProcessMove(char *moveCmd, char *curCmd, char *denCmd);
	//响应鼠标单击消息
	virtual BOOL OnLButtonDown(int x, int y);
	//确认着法
	virtual INT OkMove(char *denCmd);
	//取消着法
	virtual INT CancelMove();

private:

	BYTE board[6][6];//棋盘
	stack<Step> stepStack;//着法栈
	vector<Point> MoveWay;//吃子路径
	PointS ptMovie;//动画棋子坐标

	HPEN hRPen,hGPen,hBPen;//刻线画笔句柄
	HFONT hFont;//刻度字体句柄
	HDC hMarkDC, hBlcDC, hWhtDC;//棋子元素DC

	int d;//棋盘刻线间距
	int pWidth;//刻线宽度
	int fWidth,fHeight;//刻度字体宽、高
	int size;//棋子直径


	//绘制棋子
	VOID DrawChess();
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

	//走子
	VOID MoveChess();
	//播放吃子动画
	VOID PlayMovie();
	//搜索吃子路径
	bool SearchWay(Step sp);
	//保存吃子路径
	VOID SaveWay(Point *way, int start, int end, bool dir);
	//生成动画路径
	VOID SavePtWay(vector<Point> *ptWay);

};

#endif