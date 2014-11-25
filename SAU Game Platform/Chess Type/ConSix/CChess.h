#ifndef CCHESS_H_GAMEPLATFORM
#define CCHESS_H_GAMEPLATFORM

#include <Windows.h>
#include <math.h>

#define BLACK 0//黑
#define WHITE 1//白
#define EMPTY 2//空
#define	BAR 3//障碍
#define UPLEFT 0//左上角
#define DOWNLEFT 1//左下角
#define CENTER 2//中心
#define ALLOW 5
#define BUFSIZE (1024*512)//管道大小512K

#define GM_SHOWSTEP		0X1012
#define GM_WINLOSE		0X1013

#define NEXT(S) (1-S)

typedef struct
{
	int x;
	int y;
	BYTE side;
}point;

typedef struct
{
	POINT first;
	POINT second;
	BYTE side;
}step;

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

//程序设置
typedef struct _GameSet{
	//路径设置
	char CurDir[MAX_PATH-100];//当前应用程序的目录
	char BoardFileName[MAX_PATH];//棋盘材质路径
	char BkPicFileName[MAX_PATH];//背景图片路径
	char BlcPerFileName[MAX_PATH];//黑方人物图片路径
	char WhtPerFileName[MAX_PATH];//白方人物图片路径
	char BkMusicFileName[MAX_PATH];//背景音乐路径
	char cmDir[MAX_PATH-100];//棋谱保存路径
	char PrintScrDir[MAX_PATH-100];//截图路径
	char cmFileName[MAX_PATH];//棋谱文件路径
	char EngineInitDir[MAX_PATH-100];//引擎初始目录

	//平台设置项标志
	bool fullScr;//全屏 标志
	bool SingleStep;//单步保存棋谱 标志
	bool AllStep;//一起保存棋谱 标志
	bool PrintScr;//截图 标志
	bool CirclePlay;//循环播放背景音乐 标志
	bool StopPlay;//停止播放背景音乐 标志
	bool swEngine;//显示引擎信息 标志
	int indexBdPic;//棋盘材质
	int indexBkPic;//背景图片
	int indexBlcPer;//人物图片
	int indexWhtPer;
	int BasePt;//棋盘的原点
}GameSet;

VOID SetText(HWND hWnd,LPCTSTR str);
VOID GetText(HWND hWnd,LPCTSTR str,int size);

class CChess
{
public:	
	HWND hWnd;//窗口句柄

	GameSet *gameset;

	COLORREF BkColor;
	COLORREF BoardColor;
	RECT rtBoard;//棋盘位置
	int side;//棋盘宽度
	int StepNum[2];//黑白双方的步数
	int BasePt;//坐标原点标志
	char wMMsg[256];//待写入本方引擎信息
	char wDMsg[256];//待写入对方引擎信息

	BYTE player;//行棋方

	CChess();
	~CChess();
	//创建简单字体
	HFONT CreateSimpleFont(int width,int height);
	//绘制点
	bool DrawPoint(HDC hDC,int r,COLORREF ptColor,COLORREF bkColor);	
	//绘制辅助位图
	bool DrawAssist(HDC hAssistDC,int d);
	//判断点是否在区域内
	bool InsideRect(const RECT* rt,const int &x,const int &y);
};

#endif
