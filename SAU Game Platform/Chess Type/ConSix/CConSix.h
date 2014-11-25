#ifndef CCONSIX_H_GAMEPLATFORM
#define CCONSIX_H_GAMEPLATFORM

#include "CChess.h"

class CConSix : public CChess
{
public:	
	HPEN hPen;//画笔句柄
	HFONT hFont;//字体句柄

	int d;//六子棋棋盘刻线间距
	double pixel;//六子棋棋盘刻线宽度
	int fWidth,fHeight;//字体宽、高

	BYTE board[19][19];//棋盘信息结构
	step curStep;//当前步

	bool first_hand;//第一手标志(由于六子棋第一手落一子，以后每手落两子，故需要标记)
	int count;

	CConSix();
	~CConSix();

	//绘制棋盘
	bool DrawBoard(HDC hDC);
	//绘制棋子
	bool DrawChess(HDC hBlcDC,HDC hCurBlcDC,HDC hWhtDC,HDC hCurWhtDC,int d);
	//处理引擎消息  返回值：  -1：行棋违规  0：未找到关键字  1：获取成功  2：胜负手
	BOOL ProcessMsg(char *msg);
	//判断胜负
	bool WinOrLose(BYTE side);
	//判断棋步合法性
	bool FitRules(step sp);
	//初始化棋局
	VOID InitGame();
	//初始化棋盘状态
	VOID InitBoard();
	//响应鼠标单击消息
	BOOL OnLButtonDown(int x,int y);	
	//显示招法历史
	VOID ShowStepHis(char *msg);
	//双方对弈
	BOOL SToS(int x,int y);
	//播放落子音效
	bool PlaySnd(int sel);
};

#endif