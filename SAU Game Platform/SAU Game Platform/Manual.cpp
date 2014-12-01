/**
* file: Manual.cpp
* date: 2014/11/29
* version: 1.0
* description:
*/


#include "Manual.h"


//截图模块
//=================================================================================================================

/**
 * PSDrawBoard - 绘制棋盘
 * @hDC:	目标设备窗口DC
 * description:	PSDrawBoard对CMainWnd类中棋盘部分绘图函数的封装，用以绕过私有成员调用实现函数指针的传递。PSDrawBoard函数是CMainWnd类的友元函数。
 */
void PSDrawBoard(HDC hDC)
{
	MainWnd->FillBkAndBoard(hDC);
	CT_DrawBoard(hDC);
}

/**
 * PrintScreenOnChess - 对棋盘进行截图
 * @filename:	目标文件名
 * return:		TRUE:保存成功；FALSE:保存失败
 */
BOOL PrintScreenOnChess(char *filename)
{
	return PrintScreenEx(filename, MainWnd->hWnd, MainWnd->GetBoardPos(), PSDrawBoard);
}

/**
 * PrintScrOnTemp - 快捷截图
 */
VOID PrintScrOnTemp()
{
	char filename[MAX_PATH] = { 0 };
	char time[100] = { 0 };

	SYSTEMTIME st;
	GetSystemTimeEx(&st, 8);//获取系统时间
	sprintf(time, "\\%d-%d-%d-%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	strcpy(filename, ".");//gameSet.CurDir);
	strcat(filename, "\\PrintScr");
	strcat(filename, time);
	strcat(filename, ".bmp");
	PrintScreenOnChess(filename);
	return;
}

/**
 * SavePrintScreen - 保存行棋截图
 * description:	实现SavePrintScreen和PrintScrOnTemp的核心技术是完全一致的，只是他们在实现功能的定位上不同，PrintScrOnTemp用来支持用户手动操作的快捷截图，
 *				SavePrintScreen用来支持引擎根据用户设定完成的自动截图。
 *				自动截图的原理：当棋种支持模块处理一个有效的着法输入时，会以SendMessage调用向MainWnd窗口发送GM_SHOWSTEP消息，告知引擎着法输入有效以及输入的着法（wParam参数）。
 *								平台在处理GM_SHOWSTEP消息时会调用SavePrintScreen函数完成保存行棋截图操作。
 */
VOID SavePrintScreen()
{
	SYSTEMTIME st;
	char filename[MAX_PATH] = { 0 };
	char time[100] = { 0 };

	if (gameSet.PrintScr)
	{
		GetSystemTimeEx(&st, 8);//获取系统时间
		sprintf(time, "\\%d-%d-%d-%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		char PrintScrFileName[MAX_PATH] = { 0 };
		if (gameSet.PrintScrDir[0] == 0)
		{
			strcpy(gameSet.PrintScrDir, ".");//gameSet.CurDir);
			strcat(gameSet.PrintScrDir, "\\chess manual\\");
			strcat(gameSet.PrintScrDir, chessType[chesstype].chessStr);
			strcat(gameSet.PrintScrDir, "\\bmp");
		}
		strcpy(PrintScrFileName, gameSet.PrintScrDir);
		strcat(PrintScrFileName, time);
		strcat(PrintScrFileName, ".bmp");
		PrintScreenOnChess(PrintScrFileName);
	}
}

/**
 * SaveChessManual - 保存棋谱
 * @step:			当前有效输入着法
 * description:		功能定位和实现原理同SavePrintScreen函数相同。
 */
VOID SaveChessManual(char *step)
{
	char filename[MAX_PATH];
	char time[100];
	HANDLE hFile;
	DWORD dwWrite;
	SYSTEMTIME st;

	if (gameSet.cmDir[0] == 0)
	{
		strcpy(gameSet.cmDir, ".");//gameSet.CurDir);
		strcat(gameSet.cmDir, "\\chess manual\\");
		strcat(gameSet.cmDir, chessType[chesstype].chessStr);
		strcat(gameSet.cmDir, "\\sgf");
	}
	strcpy(filename, gameSet.cmDir);
	strcat(filename, "\\");

	if (gameSet.SingleStep)//单步保存
	{
		::GetSystemTimeEx(&st, 8);//获取系统时间  
		sprintf(time, "%d-%d-%d-%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		strcat(filename, time);
		strcat(filename, ".sgf");

		hFile = CreateFile(
			filename,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			ErrorBox("CreateFile failed");
			return;
		}
		if (!WriteFile(hFile, step, strlen(step), &dwWrite, NULL))
		{
			ErrorBox("WriteFile failed");
			CloseHandle(hFile);
			return;
		}
		CloseHandle(hFile);
	}

	if (gameSet.AllStep)//保存全部棋谱
	{
		if (gameSet.cmFileName[0] == 0)
		{
			strcpy(gameSet.cmFileName, "Untitled");
		}
		strcat(filename, gameSet.cmFileName);
		strcat(filename, ".sgf");

		hFile = CreateFile(
			filename,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			ErrorBox("CreateFile failed");
			return;
		}
		SetFilePointer(hFile, 0, NULL, FILE_END);//设置文件操作指针为文件结尾
		if (!WriteFile(hFile, step, strlen(step), &dwWrite, NULL) || !WriteFile(hFile, "\r\n", 2, &dwWrite, NULL))
		{
			ErrorBox("WriteFile failed");
			CloseHandle(hFile);
			return;
		}
		CloseHandle(hFile);
	}
	return;
}