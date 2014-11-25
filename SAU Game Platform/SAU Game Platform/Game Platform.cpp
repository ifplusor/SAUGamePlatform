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
#include "NetWork.h"


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
	PrepareForApp(hInstance);
	InitialChessType(0,hMenu);

	MainWnd=new CMainWnd;
	if(MainWnd->RegisterWnd(hInstance)==FALSE||MainWnd->CreateWnd(NULL,hMenu)==FALSE)
	{
		ErrorBox("Create MainWnd failed!");
		return -1;
	}
//	if(!NetWnd.RegisterWnd(hInstance)||!ChatWnd.RegisterWnd(hInstance))
//	{
//		ErrorBox("Register NetWnd or ChatWnd failed");
//		return 0;
//	}

	GetAndDispatchMessage();

	return 0;
}

/**
 * GetAndDispatchMessage - 消息循环
 */
VOID GetAndDispatchMessage()
{
	HACCEL hAccTable;
	MSG msg;
	hAccTable=LoadAccelerators(hInst,MAKEINTRESOURCE(IDR_ACCELERATOR));//载入快捷键
#ifdef _DEBUG
	MsgBox("进入消息循环成功！","Msg",1500);
#endif

	while(GetMessage(&msg,NULL,0,0))//从消息队列中取得消息
	{
		if(!TranslateAccelerator(msg.hwnd,hAccTable,&msg))//把快捷键消息转换成字符消息
		{
			TranslateMessage(&msg);//如果没有快捷键消息则转换其它消息
			DispatchMessage(&msg);//派发消息
		}
	}
	DestroyAcceleratorTable(hAccTable);//删除加速键表
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

	HMODULE hUser32=GetModuleHandle("user32.dll");
	if(hUser32)
	{
		MessageBoxTimeout=(MSGBOXAAPI)GetProcAddress(hUser32,"MessageBoxTimeoutA");
		if(MessageBoxTimeout==NULL)
			MessageBox(NULL,"Load MessageBoxTimeoutA fouction failed!","error",MB_OK);
	}

	GetChessTypeResourse();
	hMenu=CreateUserMenu();
	CreateChessTypeMenu(hMenu);
	InitialWithIni();

	return TRUE;
}


