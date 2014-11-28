#ifndef CHINESE_H
#define CHINESE_H

#include <windows.h>
#include <stdio.h>
#include <ocidl.h>
#include <olectl.h>

#pragma comment(lib,"winmm.lib")

VOID __cdecl DelFile(char *dir);
BOOL __cdecl DisplayImage(HDC hDC, LPCTSTR szImagePath,RECT *rt);//展示bmp、jpg、gif（第一帧）图片
VOID __cdecl PlayMusic(char *filename,bool circle,int vol);
VOID __cdecl StopMusic(char *filename);
VOID __cdecl ErrorBox(LPTSTR ErrorInfo);//错误提示框
VOID __cdecl MsgBox(LPTSTR msg);//消息框
BOOL __cdecl PrintScreen(char *filename,int left,int top,int width,int height);//截图
BOOL __cdecl PrintScreenEx(char *filename,HWND hWnd,bool (*DrawBoard)(HDC hDC,HDC hMemDC));
HWND __cdecl GetProcessMainWnd(DWORD dwProcessId);
VOID __stdcall GetSystemTimeEx(SYSTEMTIME *st,int region);

#endif