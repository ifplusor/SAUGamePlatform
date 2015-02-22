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

	BYTE line[2][6][5];//2向6线5段  [0横向/1纵向]
	BYTE box[5][5];//6*6的点，5*5的格子
	stack<Step> stepStack;//着法栈
	char lineList[256];//提子列表
	int lineNum;//连续连线步数，用于人做参与者的模式

	HFONT hFont;//刻度字体句柄
	HDC hBlcDC, hWhtDC, hPtDC;//棋子元素DC

	int d;//刻线间距
	int pWidth;//线的宽
	int fWidth,fHeight;//字体宽、高
	int pR, pL;//点的半径，直径
	int size;//格宽

	int BE;

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
	
	//点引出线判断
	bool HaveLine(Point point);
	//捕获格子判断
	bool HaveBox(LINE line);
	//坐标映射
	bool PonitToLine(Step &step);
	bool LineToPoint(Step &step);

	VOID GetConfig();

};

#endif