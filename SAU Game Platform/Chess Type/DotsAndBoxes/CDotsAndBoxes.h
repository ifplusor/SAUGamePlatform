#ifndef CDOTSANDBOXES_H_GAMEPLATFORM
#define CDOTSANDBOXES_H_GAMEPLATFORM

#include "..\include\CChess.h"


#define EMPTY 2

typedef struct
{
	int k;//2向：0横向，1纵向
	int i;//6线
	int j;//5段
}LINE;

struct Step
{
	Point start, end;//连线起止位置
	LINE line;//线的坐标
	BYTE side;
};
#define InitStep(step) { step.start.x=step.start.y=step.end.x=step.end.y=step.line.k=step.line.i=step.line.j=-1; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)
#define IsLine(line) if(line.k!=-1&&line.i!=-1&&line.j!=-1)

class CDotsAndBoxes : public CChess
{
public:

	CDotsAndBoxes(HINSTANCE hInst, HWND hWnd, char *LibPath);
	~CDotsAndBoxes();

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

	BYTE line[2][6][5];//2向6线5段  [0横向/1纵向]
	BYTE box[5][5];//6*6的点，5*5的格子
	int connectS;//连续连线步数，用于人做参与者的模式

	COLORREF BkColor;//背景色
	COLORREF BoardColor;//棋盘颜色
	HPEN hPen;//画笔句柄
	HFONT hFont;//字体句柄

	HDC hBlcDC, hWhtDC, hPtDC;

	int d;//点格棋棋盘刻线间距
	double pixel;//点格棋棋盘刻线宽度
	int fWidth,fHeight;//字体宽、高
	int pR, pWidth, pHeight;

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
	bool FitRules();
	
	//播放落子音效
	bool PlaySnd(int sel);

	//点引出线判断
	bool HaveLine(Point point);
	//捕获格子判断
	bool HaveBox(LINE line);
	//坐标映射
	bool PonitToLine(Step &step);
	bool LineToPoint(Step &step);

};

#endif