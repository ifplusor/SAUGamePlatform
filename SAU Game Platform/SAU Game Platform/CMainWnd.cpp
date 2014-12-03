/**
 * file: 
 * date: 
 * version: 
 * description: 
 */

#include "CMainWnd.h"
#include "Menu.h"
#include "Function.h"
#include "GameType.h"


const char EName[]="Name:Human";
CMainWnd *MainWnd;//主窗口对象指针


LRESULT CALLBACK MainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return MainWnd->WndProc(hWnd,uMsg,wParam,lParam);
}


//构造函数
CMainWnd::CMainWnd()
{
	ClsName="GamePlatform"; //窗体类名
	WndName="Game Platform";//窗体标题
	strcpy(strBlcName, EName);//黑方引擎名
	strcpy(strWhtName, EName);//白方引擎名
	strcpy(strBlcTime, "Time:00:00");
	strcpy(strWhtTime,"Time:00:00");
	strcpy(StepHis, "Step History\r\n"
					"------------------------------------\r\n");
}

//析构函数
CMainWnd::~CMainWnd()
{
}

//注册窗口类
BOOL CMainWnd::RegisterWnd(HINSTANCE hInst)
{		
	WNDCLASS wc;//窗口类

	wc.hInstance=hInst;//所属实例
	wc.lpszClassName=ClsName;//窗口类名
	wc.lpfnWndProc=(WNDPROC)MainWndProc;//窗体回调函数的地址
	wc.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON));//图标
	wc.lpszMenuName=NULL;//菜单名
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);//指针
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//背景画刷
	wc.style=CS_HREDRAW |CS_VREDRAW |CS_DBLCLKS;//窗体支持双击事件，水平、竖直方向大小改变时重绘窗口
	wc.cbWndExtra=0;
	wc.cbClsExtra=0;
	if(RegisterClass(&wc)==NULL)
	{
		ErrorBox("RegisterMainWindowClass failed");
		return FALSE;
	}
	return TRUE;
}

//创建窗口
BOOL CMainWnd::CreateWnd(HWND hParentWnd,HMENU hMenu)
{
	DWORD WndStyle;//窗口风格

	/*窗体样式*/
	WndStyle=WS_VISIBLE |
			WS_OVERLAPPED |	
			WS_CAPTION |
			WS_SYSMENU |		
			WS_MAXIMIZEBOX |
			WS_MINIMIZEBOX |		
			WS_SIZEBOX |
			WS_CLIPCHILDREN |//Excludes the area occupied by child windows when drawing occurs within the parent window.父子窗体之间不会因为重叠而绘制混乱
			WS_CLIPSIBLINGS ;//Excludes the area occupied by child windows when drawing occurs within another child window.子窗体之间不会因为重叠而绘制混乱	

	hWnd=CreateWindow(
		ClsName,//Class name
		WndName,//Window name		
		WndStyle,//WndStyle
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		(HWND)hParentWnd,//No parent window		
		hMenu,//Main menu
		hInst,//Application instance
		NULL//Do not create MDI client window
		);	
	if(hWnd==NULL)
	{		
		ErrorBox("CreateMainWindow failed");		
		return FALSE;
	}

	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);	
	return TRUE;
}

BOOL CMainWnd::CreateCtrl(HWND hWnd)
{
	HWND hButtonBlcLoad;//黑方加载引擎按钮窗口句柄
	HWND hButtonWhtLoad;//白方加载引擎按钮窗口句柄
	HWND hButtonBlcUnload;//黑方卸载引擎按钮窗口句柄
	HWND hButtonWhtUnload;//白方卸载引擎按钮窗口句柄
	HWND hButtonBlcOk;//黑方确认招法按钮窗口句柄
	HWND hButtonWhtOk;//白方确认招法按钮窗口句柄
	HWND hButtonBlcCancel;//黑方取消招法按钮窗口句柄
	HWND hButtonWhtCancel;//白方取消招法按钮窗口句柄

	//创建 历史招法 文本框
	hEditHis=CreateWindow(
		"EDIT",
		NULL,
		WS_VISIBLE |WS_CHILD |WS_HSCROLL |WS_VSCROLL |WS_BORDER |
		// 文本框样式
		ES_LEFT |ES_MULTILINE |ES_AUTOHSCROLL |ES_AUTOVSCROLL|ES_READONLY|ES_NOHIDESEL,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDE_STEP_HIS,
		hInst,
		NULL
		);
	if(hEditHis==NULL)
	{
		ErrorBox("CreateEditWnd Failed");
		return false;
	}
	SetText(GetDlgItem(hWnd,IDE_STEP_HIS),StepHis);	
	ShowWindow(hEditHis,SW_SHOWNORMAL);
	UpdateWindow(hEditHis);

	//创建黑方 加载引擎 按钮
	hButtonBlcLoad=CreateWindow(
		"BUTTON",
		"Load",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_ENGINE_LOAD_BLC,
		hInst,
		NULL
		);
	if(hButtonBlcLoad==NULL)
	{
		ErrorBox("CreateLoadBlcWnd Failed");
		return false;
	}
	ShowWindow(hButtonBlcLoad,SW_SHOWNORMAL);
	UpdateWindow(hButtonBlcLoad);

	//创建黑方 卸载引擎 按钮
	hButtonBlcUnload=CreateWindow(
		"BUTTON",
		"Unload",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_ENGINE_UNLOAD_BLC,
		hInst,
		NULL
		);
	if(hButtonBlcUnload==NULL)
	{
		ErrorBox("CreateUnloadBlcWnd Failed");
		return false;
	}
	ShowWindow(hButtonBlcUnload,SW_SHOWNORMAL);
	UpdateWindow(hButtonBlcUnload);

	//创建黑方 确认招法 按钮
	hButtonBlcOk=CreateWindow(
		"BUTTON",
		"Ok",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CONTROL_OK_BLC,
		hInst,
		NULL
		);
	if(hButtonBlcOk==NULL)
	{
		ErrorBox("CreateOkBlcWnd Failed");
		return false;
	}
	ShowWindow(hButtonBlcOk,SW_SHOWNORMAL);
	UpdateWindow(hButtonBlcOk);
	EnableWindow(hButtonBlcOk, FALSE);

	//创建黑方 取消招法 按钮
	hButtonBlcCancel=CreateWindow(
		"BUTTON",
		"Cancel",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CONTROL_CANCEL_BLC,
		hInst,
		NULL
		);
	if(hButtonBlcCancel==NULL)
	{
		ErrorBox("CreateCancelBlcWnd Failed");
		return false;
	}
	ShowWindow(hButtonBlcCancel,SW_SHOWNORMAL);
	UpdateWindow(hButtonBlcCancel);
	EnableWindow(hButtonBlcCancel, FALSE);

	//创建白方 加载引擎 按钮
	hButtonWhtLoad=CreateWindow(
		"BUTTON",
		"Load",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_ENGINE_LOAD_WHT,
		hInst,
		NULL
		);
	if(hButtonWhtLoad==NULL)
	{
		ErrorBox("CreateLoadWhtWnd Failed");
		return false;
	}
	ShowWindow(hButtonWhtLoad,SW_SHOWNORMAL);
	UpdateWindow(hButtonWhtLoad);

	//创建白方 卸载引擎 按钮
	hButtonWhtUnload=CreateWindow(
		"BUTTON",
		"Unload",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_ENGINE_UNLOAD_WHT,
		hInst,
		NULL
		);
	if(hButtonWhtUnload==NULL)
	{
		ErrorBox("CreateUnloadWhtWnd Failed");
		return false;
	}
	ShowWindow(hButtonWhtUnload,SW_SHOWNORMAL);
	UpdateWindow(hButtonWhtUnload);

	//创建白方 确认招法 按钮
	hButtonWhtOk=CreateWindow(
		"BUTTON",
		"Ok",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CONTROL_OK_WHT,
		hInst,
		NULL
		);
	if(hButtonWhtOk==NULL)
	{
		ErrorBox("CreateOkWhtWnd Failed");
		return false;
	}
	ShowWindow(hButtonWhtOk,SW_SHOWNORMAL);
	UpdateWindow(hButtonWhtOk);
	EnableWindow(hButtonWhtOk, FALSE);

	//创建白方 取消招法 按钮
	hButtonWhtCancel=CreateWindow(
		"BUTTON",
		"Cancel",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CONTROL_CANCEL_WHT,
		hInst,
		NULL
		);
	if(hButtonWhtCancel==NULL)
	{
		ErrorBox("CreateCancelBlcWnd Failed");
		return false;
	}
	ShowWindow(hButtonWhtCancel,SW_SHOWNORMAL);
	UpdateWindow(hButtonWhtCancel);
	EnableWindow(hButtonWhtCancel, FALSE);

	return true;
}

LRESULT CMainWnd::WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	//创建窗口后先发送WM_CREATE消息创建控件，再发送WM_ERASEBKGND消息擦除背景，然后发送WM_SIZE消息调整窗口大小，发送WM_PAINT消息进行重绘
	switch(uMsg)
	{	
	case WM_CREATE:
		OnCreate(hWnd,wParam,lParam);
		break;
	case WM_PAINT:
		OnPaint(wParam,lParam);//绘制消息		
		break;
	case WM_ERASEBKGND:
		return OnEraseBkgnd(wParam,lParam);
	case WM_SIZE:
		OnSize(wParam, lParam);//大小改变消息
		break;
	case WM_MOVE:
		GetWindowRect(hWnd, &rtWindow);//获取整个主窗体的窗口矩形
		break;
	case WM_NOTIFY:
		OnNotify(wParam,lParam);//子窗体通知消息
		break;
	case WM_COMMAND:
		OnCommand(wParam,lParam);//菜单或者按钮控件消息
		break;
	case WM_MENUSELECT:	
		OnMenuSelect(wParam,lParam);	//菜单选择消息			
		break;
	case WM_LBUTTONDOWN:		
		OnLButtonDown(wParam,lParam);//左键单击消息
		break;
	case WM_LBUTTONDBLCLK:
		OnLButtonDbClick(wParam,lParam);//左键双击消息
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDown(wParam,lParam);//右键单击消息
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam,lParam);//鼠标移动消息
		break;
	case WM_KEYDOWN:		
		OnKeyDown(wParam,lParam);//键盘按键消息
		break;	
	case WM_TIMER:
		OnTimer(wParam,lParam);//定时器消息
		break;		
	case WM_CLOSE:
		OnClose(wParam,lParam);	//关闭窗体消息
		break;//用户点击关闭按钮时，不需要系统来处理
	case WM_DESTROY:		
		OnDestroy(wParam,lParam);		//破坏窗体消息
		break;

	//自定义消息
	case GM_NAME://设置引擎名
		SetName((char*)wParam, (int)lParam);
		break;
	case GM_SHOWSTEP://显示着法信息（由其中支持模块发送）
		AppendStepHis((char*)wParam);
		Function::SaveBoardShot();
		Function::SaveChessManual((char*)wParam);
		break;
	case GM_WINLOSE:
		Function::GameStop();
		ShowWiner((int)lParam);
		break;

	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);//处理不完全的消息或者其它消息统一由系统处理		
	}
	return 0;
}

//响应窗体创建
VOID CMainWnd::OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	CreateCtrl(hWnd);
	CT_InitModule(hWnd,chessType[chesstype].LibPath);
	return;
}

//响应绘图
VOID CMainWnd::OnPaint(WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;	
	HDC hDC;//窗口DC
	HDC hMemDC;//缓存窗口DC
	HBITMAP hBitmap,hOldBitmap;

	hDC=BeginPaint(hWnd,&ps);

	hMemDC=CreateCompatibleDC(hDC);//创建兼容DC
	hBitmap=CreateCompatibleBitmap(hDC,rtClient.right-rtClient.left,rtClient.bottom-rtClient.top);//创建位图
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);//将位图选入兼容DC

	FillBkAndBoard(hMemDC);//绘制主体界面背景
	DrawCtrlBoard(hMemDC);//绘制操作面板
	CT_DrawBoard(hMemDC);//绘制棋盘

	BitBlt(hDC,rtClient.left,rtClient.top,rtClient.right-rtClient.left,rtClient.bottom-rtClient.top,hMemDC,rtClient.left,rtClient.top,SRCCOPY);//用兼容DC图像更新窗口图像
	
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);

	EndPaint(hWnd,&ps);

	return;
}

//响应擦除背景
BOOL CMainWnd::OnEraseBkgnd(WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}

//响应窗口大小改变
VOID CMainWnd::OnSize(WPARAM wParam,LPARAM lParam)
{
	int cx, cy;
	cx = LOWORD(lParam);
	cy = HIWORD(lParam);
	if (cx < 650 || cy < 400)
	{
		if (cy >= 400)
			MoveWindow(hWnd, rtWindow.left, rtWindow.top, 650 + rtWindow.right - rtWindow.left - rtClient.right, rtWindow.bottom - rtWindow.top, true);
		else if (cx >= 600)
			MoveWindow(hWnd, rtWindow.left, rtWindow.top, rtWindow.right - rtWindow.left, 400 + rtWindow.bottom - rtWindow.top - rtClient.bottom, true);
		else
			MoveWindow(hWnd, rtWindow.left, rtWindow.top, 650 + rtWindow.right - rtWindow.left - rtClient.right, 400 + rtWindow.bottom - rtWindow.top - rtClient.bottom, true);
		return;
	}
	//	StatusBarOnSize();//主窗体大小改变则调整一下状态条
	if(!AdjustWndPos())
	{		
		return;
	}
	CT_OnSize(rtBoard);
	return;
}

VOID CMainWnd::OnNotify(WPARAM wParam,LPARAM lParam)
{
	return;
}

//响应命令消息
VOID CMainWnd::OnCommand(WPARAM wParam,LPARAM lParam)
{
	int ID=LOWORD(wParam);	//菜单项ID或者按钮ID
	FUNCTION function;
	map<int,FUNCTION>::iterator iter;
	switch(ID)
	{
	case IDB_CONTROL_OK_BLC:
	case IDB_CONTROL_OK_WHT:
		UpdateTime();
		Function::OkMove();
		break;
	case IDB_CONTROL_CANCEL_BLC:
	case IDB_CONTROL_CANCEL_WHT:
		Function::CancelMove();
		break;
	case IDB_ENGINE_LOAD_BLC:
		Function::LoadBlackEngine();
		break;
	case IDB_ENGINE_LOAD_WHT:
		Function::LoadWhiteEngine();
		break;
	case IDB_ENGINE_UNLOAD_BLC:
		Function::UnloadBlackEngine();
		break;
	case IDB_ENGINE_UNLOAD_WHT:
		Function::UnloadWhiteEngine();
		break;
	default:
		if(menuFunction.count(ID))
		{
			iter=menuFunction.find(ID);
			iter->second();
		}
		else if (ID >= CHESSTYPE&&ID < CHESSTYPE + chessNum)
			SetChessType(ID - CHESSTYPE, hMenu);
		break;
	}
	return;
}

//响应菜单项选择（修改状态栏显示）
VOID CMainWnd::OnMenuSelect(WPARAM wParam,LPARAM lParam)
{
	int ID=LOWORD(wParam);	//菜单项ID或者按钮ID
//	UINT wIDs[]={MH_BASE_0,MH_BASE_1,0,0};
//		MenuHelp(WM_MENUSELECT,wParam,lParam,hCurMenu,languagePack[language].languageRE,StatusBar.hWnd,wIDs);
	return;
}

VOID CMainWnd::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{	
	int x,y;
	x=LOWORD(lParam);
	y=HIWORD(lParam);//获取所在棋盘窗体客户区的坐标，根据所选棋种执行对应棋种单击事件处理函数
	if(InsideRect(&rtBoard,x,y))//棋盘内为有效输入
	{
		Function::MoveStep(x, y);
	}
	return;
}

VOID CMainWnd::OnLButtonDbClick(WPARAM wParam,LPARAM lParam)
{
	return;
}

VOID CMainWnd::OnRButtonDown(WPARAM wParam,LPARAM lParam)
{
	return;
}

VOID CMainWnd::OnMouseMove(WPARAM wParam,LPARAM lParam)
{                                               
	int x,y;
	x=LOWORD(lParam);
	y=HIWORD(lParam);
	
	return;
}

VOID CMainWnd::OnKeyDown(WPARAM wParam,LPARAM lParam)
{
	return;
}

VOID CMainWnd::OnTimer(WPARAM wParam,LPARAM lParam)
{	
	if (timer.UpdateTime(strBlcTime, strWhtTime) == 0)
		InvalidateRect(hWnd, &rtBlcTime, FALSE);
	else
		InvalidateRect(hWnd, &rtWhtTime, FALSE);
	return;
}

VOID CMainWnd::OnClose(WPARAM wParam,LPARAM lParam)
{
	if (Function::CheckEngineLoad())
	{
		MsgBox("The engine is load,unload it and then quit.", "error", 0);
		return;
	}
	RecordUserOper();
	DestroyWindow(hWnd);//销毁窗体
	return;
}

VOID CMainWnd::OnDestroy(WPARAM wParam,LPARAM lParam)
{
	DestroyMenu(hCurMenu);//销毁菜单资源
	PostQuitMessage(0);//如果破坏窗体，则退出主线程
	return;
}

//响应改名消息：载入引擎时，把引擎名称更新一下
VOID CMainWnd::OnName(WPARAM wParam,LPARAM lParam)
{
	return;
}

//获取主窗口客户区RECT(不包括状态栏)
VOID CMainWnd::GetClientRectEx(RECT* rt)
{	
	GetClientRect(hWnd,rt);	
	return; 
}

//调整两个子窗体位置
BOOL CMainWnd::AdjustWndPos()
{	
	if(hWnd==NULL)
	{
		return FALSE;
	}	
	GetWindowRect(hWnd,&rtWindow);//获取整个主窗体的窗口矩形
	GetClientRectEx(&rtClient);//获取主窗体客户区的矩形

	/*设置棋盘显示部分在主窗体客户区的位置*/
	rtBoardPart.left=rtClient.left;
	rtBoardPart.top=rtClient.top;
	rtBoardPart.right=(int)(rtClient.right*RATIO);
	rtBoardPart.bottom=rtClient.bottom;
	GetBkAndBoardRT();//计算主体界面图像位置

	/*设置信息板显示部分在主窗体客户区的位置*/
	rtInfoPart.left=rtBoardPart.right+1;
	rtInfoPart.top=rtClient.top;
	rtInfoPart.right=rtClient.right;
	rtInfoPart.bottom=rtClient.bottom;
	AdjustCtrlPos();//计算操作面板图像位置

	return TRUE;
}

/**
 * GetBkAndBoardRT - 计算主体界面图像元素位置
 */
VOID CMainWnd::GetBkAndBoardRT()
{
	//背景图片位置
	rtBackground.left=rtBoardPart.left;
	rtBackground.right=rtBoardPart.right;
	rtBackground.top=rtBoardPart.top;
	rtBackground.bottom=rtBoardPart.bottom;

	//计算棋盘边长
	BoardSide=(rtBoardPart.right>rtBoardPart.bottom? rtBoardPart.bottom:rtBoardPart.right)+1;
	BoardSide-=(MARGIN*2);

	//计算棋盘位置
	rtBoard.left=(int)((rtBoardPart.right-BoardSide+1)/2);
	rtBoard.right=rtBoard.left+BoardSide-1;		
	rtBoard.top=(int)((rtBoardPart.bottom-BoardSide+1)/2);
	rtBoard.bottom=rtBoard.top+BoardSide-1;	

	//计算黑方人物图片位置
	rtBlcPer.right=rtBoard.left-rtBoard.left/15;
	rtBlcPer.left=rtBlcPer.right-rtBoard.left*2/3;
	rtBlcPer.top=rtBoard.top+BoardSide*0.2;
	rtBlcPer.bottom=rtBlcPer.top+(rtBlcPer.right-rtBlcPer.left+1)*1.681-1;//高度根据宽度保持黄金比例
	if((rtBlcPer.bottom-rtBlcPer.top+1)>BoardSide*0.5)//高度超过最大限度
	{
		rtBlcPer.bottom=rtBlcPer.top+BoardSide*0.5-1;
		rtBlcPer.left=rtBlcPer.right-(rtBlcPer.bottom-rtBlcPer.top+1)*0.681+1;//宽度根据高度保持黄金比例
	}

	//计算白方人物图片位置（与黑方人物图片位置对称）
	rtWhtPer.left=rtBoard.right+(rtClient.right-rtBoard.right)/15;
	rtWhtPer.right=rtWhtPer.left+(rtBlcPer.right-rtBlcPer.left);
	rtWhtPer.top=rtBlcPer.top;
	rtWhtPer.bottom=rtBlcPer.bottom;
}

/**
 * AdjustCtrlPos - 计算并调整控件位置
 */
BOOL CMainWnd::AdjustCtrlPos()
{	
	//**计算图像元素位置

	//基准偏移
	edge.cx=(int)((rtInfoPart.right-rtInfoPart.left)*0.05);
	edge.cy=(int)((rtInfoPart.bottom-rtInfoPart.top)*0.05/3);
	//黑方分组框
	rtSBlc.left=rtInfoPart.left;			rtSBlc.right=rtInfoPart.right;
	rtSBlc.top=rtInfoPart.top;				rtSBlc.bottom=rtSBlc.top+(int)(rtInfoPart.bottom/3);
	//黑方引擎名静态框
	rtBlcName.left=rtSBlc.left+2*edge.cx;	rtBlcName.right=rtSBlc.right-2*edge.cx;
	rtBlcName.top=rtSBlc.top+2*edge.cy;		rtBlcName.bottom=rtBlcName.top+(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4);
	//黑方计时静态框
	rtBlcTime.left=rtSBlc.left+2*edge.cx;	rtBlcTime.right=rtSBlc.right-2*edge.cx;
	rtBlcTime.top=rtBlcName.bottom+edge.cy;	rtBlcTime.bottom=rtBlcTime.top+(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4);
	//白方分组框
	rtSWht.left=rtInfoPart.left;			rtSWht.right=rtInfoPart.right;
	rtSWht.top=rtSBlc.bottom+1;				rtSWht.bottom=rtSWht.top+(int)((rtInfoPart.bottom)/3);
	//白方引擎名静态框
	rtWhtName.left=rtSWht.left+2*edge.cx;	rtWhtName.right=rtSWht.right-2*edge.cx;
	rtWhtName.top=rtSWht.top+2*edge.cy;		rtWhtName.bottom=rtWhtName.top+(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4);
	//白方计时静态框
	rtWhtTime.left=rtSWht.left+2*edge.cx;	rtWhtTime.right=rtSWht.right-2*edge.cx;
	rtWhtTime.top=rtWhtName.bottom+edge.cy;	rtWhtTime.bottom=rtWhtTime.top+(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4);	


	//**调整空寂位置

	//设置历史招法文本框位置
	if(!SetWindowPos(
		hEditHis,
		HWND_TOP,
		rtSWht.left+edge.cx,
		rtSWht.bottom+edge.cy,
		rtSWht.right-rtSWht.left-2*edge.cx,
		rtSWht.bottom-rtSWht.top-2*edge.cy,
		SWP_SHOWWINDOW
		))
		return false;

	//设置黑方加载引擎按钮位置
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_ENGINE_LOAD_BLC),
		HWND_TOP,
		rtSBlc.left+2*edge.cx,
		rtBlcTime.bottom+edge.cy,
		(int)((rtSBlc.right-rtSBlc.left-5*edge.cx)/2),
		(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//设置黑方卸载引擎按钮位置
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_ENGINE_UNLOAD_BLC),
		HWND_TOP,
		rtSBlc.right-(int)((rtSBlc.right-rtSBlc.left-edge.cx)/2),
		rtBlcTime.bottom+edge.cy,
		(int)((rtSBlc.right-rtSBlc.left-5*edge.cx)/2),
		(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//设置黑方确认招法按钮位置
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_CONTROL_OK_BLC),
		HWND_TOP,
		rtSBlc.left+2*edge.cx,
		rtBlcTime.bottom+(int)((rtSBlc.bottom-rtBlcTime.bottom)/2),
		(int)((rtSBlc.right-rtSBlc.left-5*edge.cx)/2),
		(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//设置黑方卸载引擎按钮位置
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_CONTROL_CANCEL_BLC),
		HWND_TOP,
		rtSBlc.right-(int)((rtSBlc.right-rtSBlc.left-edge.cx)/2),
		rtBlcTime.bottom+(int)((rtSBlc.bottom-rtBlcTime.bottom)/2),
		(int)((rtSBlc.right-rtSBlc.left-5*edge.cx)/2),
		(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//设置白方加载引擎按钮位置
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_ENGINE_LOAD_WHT),
		HWND_TOP,
		rtSWht.left+2*edge.cx,
		rtWhtTime.bottom+edge.cy,
		(int)((rtSWht.right-rtSWht.left-5*edge.cx)/2),
		(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//设置白方卸载引擎按钮位置
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_ENGINE_UNLOAD_WHT),
		HWND_TOP,
		rtSWht.right-(int)((rtSWht.right-rtSWht.left-edge.cx)/2),
		rtWhtTime.bottom+edge.cy,
		(int)((rtSWht.right-rtSWht.left-5*edge.cx)/2),
		(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//设置白方确认招法按钮位置
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_CONTROL_OK_WHT),
		HWND_TOP,
		rtSWht.left+2*edge.cx,
		rtWhtTime.bottom+(int)((rtSWht.bottom-rtWhtTime.bottom)/2),
		(int)((rtSWht.right-rtSWht.left-5*edge.cx)/2),
		(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//设置白方卸载引擎按钮位置
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_CONTROL_CANCEL_WHT),
		HWND_TOP,
		rtSWht.right-(int)((rtSWht.right-rtSWht.left-edge.cx)/2),
		rtWhtTime.bottom+(int)((rtSWht.bottom-rtWhtTime.bottom)/2),
		(int)((rtSWht.right-rtSWht.left-5*edge.cx)/2),
		(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	return true;
}

//填充背景和棋盘材质
BOOL CMainWnd::FillBkAndBoard(HDC hDC)
{
	char filename[MAX_PATH]={0};	
	HFONT hFont,hOldFont;
	int x,y,len,fWidth,fHeight;
	COLORREF hOldColor;
	LOGBRUSH lb;//画刷描述符
	HBRUSH hBrush,hOldBrush;//画刷句柄

	SetBkMode(hDC,TRANSPARENT);//设置字体背景模式为透明
	
	//填充背景图片
	if(gameSet.BkPicFileName[0]!=0)
	{
		DisplayImage(hDC,gameSet.BkPicFileName,&rtBackground);
	}
	else
	{
		lb.lbStyle=BS_SOLID;
		lb.lbColor=RGB(255,255,255);
		lb.lbHatch=NULL;
		hBrush=CreateBrushIndirect(&lb);
		hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
		FillRect(hDC,&rtBackground,hBrush);
		SelectObject(hDC,hOldBrush);
		DeleteObject(hBrush);
	}

	//填充棋盘材质
	if(gameSet.BoardFileName[0]!=0)
	{
		DisplayImage(hDC,gameSet.BoardFileName,&rtBoard);
	}

	len=strlen("Black");
	fWidth=(rtBlcPer.right-rtBlcPer.left+1)/(2*len);
	fHeight=fWidth*2;
	hFont=CreateSimpleFont(fWidth,fHeight);
	hOldFont=(HFONT)SelectObject(hDC,hFont);
	hOldColor=SetTextColor(hDC,RGB(255,0,0));
	if(gameSet.BlcPerFileName[0]!=0)
	{
		DisplayImage(hDC,gameSet.BlcPerFileName,&rtBlcPer);
		x=rtBlcPer.left+(rtBlcPer.right-rtBlcPer.left-len*fWidth)/2;
		y=rtBlcPer.bottom;			
		TextOut(hDC,x,y,"Black",len);
	}
	if(gameSet.WhtPerFileName[0]!=0)
	{
		DisplayImage(hDC,gameSet.WhtPerFileName,&rtWhtPer);
		x=rtWhtPer.left+(rtWhtPer.right-rtWhtPer.left-len*fWidth)/2;
		y=rtWhtPer.bottom;			
		TextOut(hDC,x,y,"White",len);
	}

	//恢复原始样式
	SetTextColor(hDC,hOldColor);
	SelectObject(hDC,hOldFont);
	DeleteObject(hFont);
	return true;
}

BOOL CMainWnd::DrawCtrlBoard(HDC hDC)
{
	LOGBRUSH lb;//画刷描述符
	HBRUSH hBrush,hOldBrush;//画刷句柄
	HFONT hFont,hOldFont;//字体句柄
	int wFontName,hFontName;//引擎名字体宽、高
	int wFontTime,hFontTime;//时间字体宽、高
	
	SetBkMode(hDC,TRANSPARENT);//设置背景混合模式为透明

	//填充背景色为银灰色
	lb.lbStyle=BS_SOLID;
	lb.lbColor=RGB(230,230,230);
	lb.lbHatch=NULL;
	hBrush=CreateBrushIndirect(&lb);
	hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
	FillRect(hDC,&rtInfoPart,hBrush);
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);

	//**绘制黑方图像元素
	DrawStatic(hDC,&rtSBlc,"Black(先手)",RGB(230,230,230),RGB(150,150,150),edge);//绘制黑方静态框
	
	wFontName=(int)((rtBlcName.right-rtBlcName.left)/strlen(strBlcName));
	hFontName=(rtBlcName.bottom-rtBlcName.top);
	hFont=CreateSimpleFont(wFontName,hFontName);
	hOldFont=(HFONT)SelectObject(hDC,hFont);	
	DrawText(hDC,strBlcName,strlen(strBlcName),&rtBlcName,DT_CENTER);//绘制黑方引擎的名字
	DeleteObject(hFont);

	wFontTime=(int)((rtBlcTime.right-rtBlcTime.left)/strlen(strBlcTime));
	hFontTime=(rtBlcTime.bottom-rtBlcTime.top);
	hFont=CreateSimpleFont(wFontTime,hFontTime);
	hOldFont=(HFONT)SelectObject(hDC,hFont);
	DrawText(hDC,strBlcTime,strlen(strBlcTime),&rtBlcTime,DT_CENTER);//绘制黑方引擎所需时间
	DeleteObject(hFont);

	//**绘制白方图像元素
	DrawStatic(hDC,&rtSWht,"White(后手)",RGB(230,230,230),RGB(150,150,150),edge);//绘制白方静态框

	wFontName=(int)((rtWhtName.right-rtWhtName.left)/strlen(strWhtName));
	hFontName=(rtWhtName.bottom-rtWhtName.top);
	hFont=CreateSimpleFont(wFontName,hFontName);
	hOldFont=(HFONT)SelectObject(hDC,hFont);
	DrawText(hDC,strWhtName,strlen(strWhtName),&rtWhtName,DT_CENTER);//绘制白方引擎的名字
	DeleteObject(hFont);

	hFont=CreateSimpleFont(wFontTime,hFontTime);
	hOldFont=(HFONT)SelectObject(hDC,hFont);
	DrawText(hDC,strWhtTime,strlen(strWhtTime),&rtWhtTime,DT_CENTER);//绘制白方引擎所需时间
	DeleteObject(hFont);	

	return true;
}

VOID CMainWnd::AppendStepHis(char *step)
{
	strcat(StepHis, step);
	int len = strlen(StepHis);
	StepHis[len++] = 13;
	StepHis[len++] = 10;
	StepHis[len] = 0;
	SetText(hEditHis, StepHis);
}

VOID CMainWnd::SetName(char* nameCmd, int player)
{
	if (player == BLACK)
	{
		if (nameCmd == NULL)
			strcpy(strBlcName, EName);
		else
		{
			strncpy(strBlcName, nameCmd, 49);
			strBlcName[0] = 'N';
			strBlcName[4] = ':';
		}
		InvalidateRect(hWnd, &rtBlcName, FALSE);
	}
	else if (player == WHITE)
	{
		if (nameCmd == NULL)
			strcpy(strBlcName, EName);
		else
		{
			strncpy(strWhtName, nameCmd, 49);
			strWhtName[0] = 'N';
			strWhtName[4] = ':';
		}
		InvalidateRect(hWnd, &rtWhtName, FALSE);
	}
}

VOID CMainWnd::ShowWiner(int side)
{
	if (side == BLACK)
	{
		MessageBox(hWnd, "黑方胜出！", "Msg", MB_OK);
	}
	else
	{
		MessageBox(hWnd, "白方胜出！", "Msg", MB_OK);
	}
}

VOID CMainWnd::GameStart()
{
	strcpy(StepHis, "Step History\r\n"
		"------------------------------------\r\n");
	strcpy(strBlcTime, "Time: 00:00");
	strcpy(strWhtTime, "Time: 00:00");
	InvalidateRect(hWnd, &rtBlcTime, FALSE);
	InvalidateRect(hWnd, &rtWhtTime, FALSE);
	timer.StartTimer(hWnd);
}

VOID CMainWnd::GameStop()
{
	timer.StopTimer();
}

VOID CMainWnd::UpdateTime()
{
	if (timer.UpdateTime(strBlcTime, strWhtTime) == 0)
		InvalidateRect(hWnd, &rtBlcTime, FALSE);
	else
		InvalidateRect(hWnd, &rtWhtTime, FALSE);
}