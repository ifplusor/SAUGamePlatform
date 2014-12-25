#ifndef CHINESE_H
#define CHINESE_H

#include <windows.h>
#include <stdio.h>
#include <ocidl.h>
#include <olectl.h>

#pragma comment(lib,"winmm.lib")

BOOL __cdecl CreateFolder(char *Dir);//创建新目录
VOID __cdecl DelFile(char *dir,bool all);//删除指定目录下的文件，dll控制子目录下文件的删除
BOOL __cdecl DisplayImage(HDC hDC, LPCTSTR szImagePath,RECT *rt);//展示bmp、jpg、gif（第一帧）图片
VOID __cdecl PlayMusic(char *filename,bool circle,int vol);
VOID __cdecl StopMusic(char *filename);
VOID __cdecl ErrorBox(LPTSTR ErrorInfo);//错误提示框
VOID __cdecl MsgBox(LPTSTR msg);//消息框
BOOL __cdecl PrintScreen(char *filename,int left,int top,int width,int height);//截图
BOOL __cdecl PrintScreenEx(char *filename, HWND hWnd, RECT rtCut, void(*Draw)(HDC hDC));
HWND __cdecl GetProcessMainWnd(DWORD dwProcessId);
VOID __stdcall GetSystemTimeEx(SYSTEMTIME *st,int region);

#endif