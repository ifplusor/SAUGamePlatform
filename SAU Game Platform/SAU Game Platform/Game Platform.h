/**
* file: Game Platform.h
* date: 2014/7/15
* version: 2.0
* description: 
*/


#ifndef MAIN_H_GAMEPLATFORM
#define MAIN_H_GAMEPLATFORM


//动态库静态链接
//============================================================================
#pragma comment(lib,"WSOCK32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"Lib\\CommFunc.lib")
#pragma comment(lib,"Lib\\FileDlg.lib")


#include <Windows.h>
#include <stdio.h>
#include <algorithm>
#include <map>
#include "resource.h"
#include "re_function.h"
#include "Set.h"
#include "Include\\CommFunc.h"
#include "Include\\FileDlg.h"

using namespace std;


//宏
#define BLACK 0
#define WHITE 1

#define MsgBox(lpText,lpCaption,dwMilliseconds) \
	{ if(MessageBoxTimeout==NULL||dwMilliseconds==0) MessageBox(NULL,lpText,lpCaption,MB_OK); \
	  else MessageBoxTimeout(NULL,lpText,lpCaption,MB_OK,0,dwMilliseconds); }
//错误提示框
#define ErrorBox(ErrorInfo) \
	{ CHAR error1[50],error2[20]; \
	  strcpy(error1,ErrorInfo); \
	  sprintf(error2,"\n\nerror: %d",GetLastError()); \
	  strcat(error1,error2); \
	  MessageBox(NULL,error1,"error",MB_OK); }
//设置文本框文本内容
#define SetText(hWnd,str) \
	{ SendMessage(hWnd,WM_SETTEXT,(WPARAM)0,(LPARAM)str); /*添加内容*/  \
	  SendMessage(hWnd,EM_SETSEL,(WPARAM)-2,(LPARAM)-1); /*设置焦点到末尾*/ \
	  SendMessage(hWnd,WM_VSCROLL,SB_BOTTOM,0); }
//获取文本框文本内容
#define GetText(hWnd,str,size) \
	{ SendMessage(hWnd,WM_GETTEXT,size,(LPARAM)str); }


//自定义消息
#define GM_NAME			0X1011
#define GM_SHOWSTEP		0X1012
#define GM_WINLOSE		0X1013
#define GM_CREATECHAT   0X1014
#define GM_DESTROYCHAT  0X1015


typedef int (__stdcall *MSGBOXAAPI)(IN HWND hWnd,IN LPCSTR lpText,IN LPCSTR lpCaption,IN UINT uType,IN WORD wLanguageID,IN DWORD dwMilliseconds);
extern MSGBOXAAPI MessageBoxTimeout;

typedef int (*FUNCTION)();//函数指针结构


//全局变量
extern HINSTANCE hInst;//程序实例句柄



#endif