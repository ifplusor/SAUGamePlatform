/**
 * file: CEngine.h
 * date: 2014/9/10
 * version: 1.2
 * description: SAU_GamePalform 程序引擎控制类头文件，内容包括：CEngine类的定义及其成员函数的声明。
 */


#ifndef CENGINE_H_GAMEPLATFORM
#define CENGINE_H_GAMEPLATFORM

#include "Game Platform.h"

#define BUFSIZE 256
#define FULLBUFSIZE 512
#define UNNAMEDPIPE 0
#define NAMEDPIPE 1


typedef struct _PROCESSDESCRIBE
{
	HANDLE engine_read, platform_write;//引擎读平台写匿名管道
	HANDLE platform_read, engine_write;//引擎写平台读匿名管道
	HANDLE hPipe;//命名管道
	HANDLE hEProcess;
	HWND console_hwnd;//引擎信息窗口句柄
	HANDLE console_write, console_read;//引擎信息窗口管道
	HANDLE hCProcess;
}PROCESSDESCRIBE;

class CEngine
{
public:
	CEngine();
	~CEngine();

	void SetType(int type){linkType=type;}
	bool GetLoaded(){return linked;}
	void SetName(char *nameCmd);
	char* GetName(){return name;}
	//加载引擎
	bool LoadEngine();
	//卸载引擎
	bool UnloadEngine();
	//读取引擎消息
	DWORD ReadMsg(char *msg,int size);
	//向引擎写入消息
	DWORD WriteMsg(char *msg);
	void GetCommand(char *cmd,char *CMD);
	VOID ShowEngineWindow(int nCmdShow);

	VOID CreatePipeAndConnectClient();

private:
	int linkType;//连接类型
	bool linked;//是否已连接引擎
	char name[256];//引擎名
	LPCTSTR path;//引擎程序路径
	OVERLAPPED ol;

	char readBuffer[FULLBUFSIZE];//读引擎信息缓存区
	char indexBuf;

	PROCESSDESCRIBE pde;

	//通过匿名管道连接引擎引擎
	bool LinkEngineWithUnnamed();
	//通过命名管道连接引擎
	bool LinkEngineWithNamed();
	//创建信息显示窗口
	bool CreateEngineInfoBoard();
};
#endif