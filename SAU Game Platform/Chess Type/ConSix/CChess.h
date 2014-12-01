#ifndef CCHESS_H_GAMEPLATFORM
#define CCHESS_H_GAMEPLATFORM

#include <Windows.h>
#include <math.h>


#define ALLOW 5
#define BUFSIZE (1024*512)//管道大小512K

#define GM_SHOWSTEP		0X1012
#define GM_WINLOSE		0X1013

#define BLACK 0//黑方
#define WHITE 1//白方
#define NEXTPLAYER(S) (1-S)//轮换行棋权


typedef struct
{
	char* ChessName;//棋类
	char* BlcName;//黑方引擎名称
	char* WhtName;//白方引擎名称
	char* res;//比赛结果
	char* BlcTime;//黑方引擎用时
	char* WhtTime;//白方引擎用时
	char* BlcStep;//黑方步数
	char* WhtStep;//白方步数
	char* BlcTimePerStep;//黑方平均每步时间
	char* WhtTimePerStep;//白方平均每步时间
	char* GameDate;//比赛结束时间
}GAMEINFO;


class CChess
{
public:	
	HWND hWnd;//窗口句柄

	char LibPath[MAX_PATH];//棋种支持模块路径

	char curCmd[256];//待写入本方引擎命令
	char denCmd[256];//待写入对方引擎命令

	BYTE player;//行棋方

	CChess();
	~CChess();

	//设置棋盘大小
	virtual VOID SetBoard(RECT rtBoard) = 0;
	//绘制棋盘
	virtual VOID DrawBoard(HDC hDC) = 0;
	//初始化棋局
	virtual VOID InitGame() = 0;
	//处理引擎行棋命令  返回值：  -1：行棋违规  0：未找到关键字  1：获取成功  2：胜负手
	virtual INT ProcessMove(char *msg) = 0;
	//响应鼠标单击消息
	virtual BOOL OnLButtonDown(int x, int y)=0;
	//确认着法
	virtual INT OkMove() = 0;
	//取消着法
	virtual VOID CancelMove() = 0;

	//绘制点（圆形）
	static bool DrawPoint(HDC hDC,int r,COLORREF ptColor,COLORREF bkColor);	
	//绘制圆形辅助位图
	static bool DrawAssist(HDC hAssistDC,int r);
	//创建简单字体
	static HFONT CreateSimpleFont(int width,int height);
	//判断点是否在区域内
	static bool InsideRect(const RECT* rt,const int &x,const int &y);

protected:
	RECT rtBoard;//棋盘位置
	int side;//棋盘宽度
	int StepNum[2];//黑白双方的步数
	int count;//鼠标点击输入状态，-1为不接受鼠标点击输入
};

#endif
