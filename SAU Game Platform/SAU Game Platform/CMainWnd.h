/**
* file: 
* date: 
* version: 
* description: 
*/

#ifndef CMAINWND_H_GAMEPLATFORM
#define CMAINWND_H_GAMEPLATFORM

#include "CWnd.h"
#include "CTime.h"


#define MAINWNDTIMER 1
#define RATIO 0.8//棋盘显示部分占主窗体比例
#define MARGIN 10
#define PI 3.14159 //边界、间隔、比例初始定义


class CMainWnd :public CWnd//主窗口类
{
public:
	HINSTANCE hRE;//当前引用的资源实例句柄
	HMENU hCurMenu;//当前菜单句柄

	//构造函数
	CMainWnd();
	//析构函数
	~CMainWnd();

	friend void PSDrawBoard(HDC hDC);
		
	VOID SetName(char* nameCmd, int player);
	VOID GameStart();
	VOID GameStop();
	VOID UpdateTime();

	BOOL RegisterWnd(HINSTANCE hInst);
	BOOL CreateWnd(HWND hParentWnd=NULL,HMENU hMenu=NULL);
	LRESULT WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	RECT GetBoardPos(){return this->rtBoard;}

private:
	CTime timer;//计时器

	RECT rtBoardPart;//棋盘显示部分
	RECT rtInfoPart;//信息板显示部分

	RECT rtBackground;//背景位置
	int BoardSide;//棋盘边长
	RECT rtBoard;//棋盘位置
	RECT rtBlcPer;//黑方人物图片位置
	RECT rtWhtPer;//白方人物图片位置

	SIZE edge;//基准点
	RECT rtSBlc;//黑方分组框
	RECT rtSWht;//白方分组框
	RECT rtBlcName;//黑方名字静态框
	RECT rtWhtName;//白方名字静态框
	RECT rtBlcTime;//黑方时间静态框
	RECT rtWhtTime;//白方时间静态框

	char strBlcName[50];//黑方引擎名
	char strWhtName[50];//白方引擎名

	char strBlcTime[20];//黑方用时
	char strWhtTime[20];//白方用时

	HWND hEditHis;//历史招法信息文本框

	char StepHis[5000];	//招法历史以双方出现胜负局面最多为100步，一步信息最多为50字节计算。

	VOID AppendStepHis(char *step);
	VOID ShowWiner(int side);

	//获取主窗口客户区RECT(不包括状态栏)
	VOID GetClientRectEx(RECT *rt);
	
	BOOL AdjustWndPos();
	//计算主体界面图像元素位置
	VOID GetBkAndBoardRT();
	//创建控件
	BOOL CreateCtrl(HWND hWnd);
	//调整控件位置
	BOOL AdjustCtrlPos();

	BOOL DrawBoard(HDC hMenDC);
	BOOL FillBkAndBoard(HDC hMemDC);
	BOOL DrawCtrlBoard(HDC hMemDC);

	VOID OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam);
	VOID OnPaint(WPARAM wParam,LPARAM lParam);
	BOOL OnEraseBkgnd(WPARAM wParam,LPARAM lParam);	
	VOID OnSize(WPARAM wParam,LPARAM lParam);
	VOID OnNotify(WPARAM wParam,LPARAM lParam);
	VOID OnCommand(WPARAM wParam,LPARAM lParam);
	VOID OnMenuSelect(WPARAM wParam,LPARAM lParam);
	VOID OnLButtonDown(WPARAM wParam,LPARAM lParam);
	VOID OnLButtonDbClick(WPARAM wParam,LPARAM lParam);
	VOID OnRButtonDown(WPARAM wParam,LPARAM lParam);
	VOID OnMouseMove(WPARAM wParam,LPARAM lParam);
	VOID OnKeyDown(WPARAM wParam,LPARAM lParam);
	VOID OnTimer(WPARAM wParam,LPARAM lParam);
	VOID OnClose(WPARAM wParam,LPARAM lParam);
	VOID OnDestroy(WPARAM wParam,LPARAM lParam);
	VOID OnName(WPARAM wParam,LPARAM lParam);
};

extern CMainWnd *MainWnd;//主窗口对象指针


#endif
