/**
* file: Game Platform.cpp
* date: 2014/7/15
* version: 2.0
* description: SAU Game Platform 项目文件，定义入口函数
*/


#include "Game Platform.h"
#include "Menu.h"
#include "CMainWnd.h"
#include "GameType.h"
#include "Network.h"


HINSTANCE hInst=NULL;//程序实例句柄


MSGBOXAAPI MessageBoxTimeout=NULL;

VOID GetAndDispatchMessage();//消息循环函数
BOOL PrepareForApp(HINSTANCE hInstance);//程序准备



/**
 * WinMain - Win32应用程序的入口点
 * @hInstance:	应用程序实例句柄
 * @lpCmdLine:	命令行指针
 * @nShowCmd:	显示参数
 */
int WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nShowCmd)
{
	if(PrepareForApp(hInstance)==FALSE)
		return -1;

	//初始化棋种模块
	InitialChessType(hMenu);

	// 注册主窗口类，并创建主窗体
	MainWnd=new CMainWnd;
	if(MainWnd->RegisterWnd(hInstance)==FALSE||MainWnd->CreateWnd(NULL,hMenu)==FALSE)
	{
		ErrorBox("Create MainWnd failed!");
		return -1;
	}

	// 注册网络服务窗口类
	NetWnd = new CNetWnd;
	if(!NetWnd->RegisterWnd(hInstance))
	{
		ErrorBox("Register NetWnd failed");
		return 0;
	}

	// 注册网络协作窗口类
	ChatWnd = new CChatWnd;
	if (!ChatWnd->RegisterWnd(hInstance))
	{
		ErrorBox("Register ChatWnd failed");
		return 0;
	}

	// 程序主循环
	GetAndDispatchMessage();

	return 0;
}

/**
 * GetAndDispatchMessage - 消息循环
 */
VOID GetAndDispatchMessage()
{
	HACCEL hAccTable;//加速键表
	MSG msg;//消息

	// 载入快捷键
	hAccTable=LoadAccelerators(hInst,MAKEINTRESOURCE(IDR_ACCELERATOR));

#ifdef _DEBUG
	MsgBox("进入消息循环成功！","Msg",1500);
#endif

	// 消息循环
	while(GetMessage(&msg,NULL,0,0))//从消息队列中取得消息，有阻塞
	{
		if(!TranslateAccelerator(msg.hwnd,hAccTable,&msg))//把快捷键消息转换成字符消息
		{
			TranslateMessage(&msg);//如果没有快捷键消息则转换其它消息
			DispatchMessage(&msg);//派发消息
		}
	}

	//删除加速键表
	DestroyAcceleratorTable(hAccTable);
}

/*
 * PrepareForApp - 程序准备
 */
BOOL PrepareForApp(HINSTANCE hInstance)
{
	hInst=hInstance;//记录本应用程序的实例句柄

	if(!GetCurrentDirectory(sizeof(gameSet.CurDir),gameSet.CurDir))//获取程序的当前的目录，无论应用程序的当前目录怎么变，CurDir始终不变
	{
		ErrorBox("Get programing current directory failed!");
		return FALSE;
	}

	// 获得MessageBoxTimeout系统调用
	HMODULE hUser32=GetModuleHandle("user32.dll");
	if(hUser32)
	{
		MessageBoxTimeout=(MSGBOXAAPI)GetProcAddress(hUser32,"MessageBoxTimeoutA");
		if(MessageBoxTimeout==NULL)
			MessageBox(NULL,"Load MessageBoxTimeoutA fouction failed!","error",MB_OK);
	}

	// 获取程序相关资源
	if(GetChessTypeResourse()==FALSE)//获取棋种组件信息
		return FALSE;
	hMenu=CreateUserMenu();//创建菜单栏
	if (hMenu == NULL)
		return FALSE;
	CreateChessTypeMenu(hMenu);//向菜单栏中加入棋种选择菜单
	InitialWithIni();//使用配置文件初始化程序设置

	// 创建依赖文件夹
	CreateFolder("PrintScr");//创建截图文件夹

	return TRUE;
}


