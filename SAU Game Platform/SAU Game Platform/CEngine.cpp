/**
 * file: CEngine.cpp
 * date: 2014/9/10
 * version: 1.2
 * description: SAU_GamePalform 程序引擎控制类文件，内容包括：CEngine类的声明的成员函数的定义。
 */


#include "CEngine.h"


/**
 * getcmd - 寻找命令字
 * @scr:	待查缓存区地址
 * @size:	待查缓存区长度
 * @cmd:	待查命令字首地址
 * @return:	命令字在缓存区中的偏移量，-1表示不存在
 * description：因为平台-引擎通信协议规定：引擎向平台发送的命令格式为"命令字+' '+参数"，故在命令字匹配过程中要检查是否以' '结束。
				引擎返回数据中的关键字只有“name”和“move”。
 */
int getcmd(char *scr,int size,char *cmd)
{
	int pos=-1,i,j,len;
	bool start = true;
	len=strlen(cmd);
	size -= len;
	for(i=0;i<=size;i++)
	{
		if (start)//匹配启动控制
		{
			for (j = 0; j<len; j++)
			{
				if (scr[i + j] != cmd[j])
					break;
			}
			if (j == len&&scr[i + j] == ' ')//引擎向平台发送的数据如果有意义，关键字后必有空格
			{
				pos = i;
				break;
			}
			else//匹配失败
			{
				i += j;
				start = false;
			}
		}
		if (scr[i] == ' ' || scr[i] == '\n' || scr[i] == '\t'|| scr[i] == '\0')//关键字前必为这些字符
			start = true;
	}
	return pos;
}

CEngine::CEngine()
{
	status=-1;//未连接引擎
	linkType=UNNAMEDPIPE;//匿名管道连接
	indexBuf=0;
}

CEngine::~CEngine()
{
}

void CEngine::SetName(char *nameCmd)
{
	strncpy(name, nameCmd + 5, 255);
}

//通过匿名管道连接引擎引擎
bool CEngine::LinkEngineWithUnnamed()
{
	status = 0;//引擎连接未就绪

	SECURITY_ATTRIBUTES sa;
	sa.nLength=sizeof(sa);
	sa.lpSecurityDescriptor=NULL;//Default security attributes
	sa.bInheritHandle=true;//handle can be inherited

	if(!CreatePipe(&pde.engine_read,&pde.platform_write,&sa,BUFSIZE) ||!CreatePipe(&pde.platform_read,&pde.engine_write,&sa,BUFSIZE))//创建两个平台与引擎之间互相通信的匿名管道
	{
		ErrorBox("CreatePipe failed");
		return false;
	}

	char Filter[]="(exe files)|*.exe|(all files)|*.*||";//文件滤镜	
	CFileDialog FileName(true,NULL,NULL,Filter,NULL,gameSet.EngineInitDir);//定义文件对话框类实例
	if(FileName.DoModal()==IDOK)
	{
		path=FileName.GetFilePath();
		LPCTSTR folder=FileName.GetFolderPath(path);
		char EngineDir[MAX_PATH]={0};
		strcpy(EngineDir,folder);
				
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si,sizeof(si));
		si.cb=sizeof(si);
		si.dwFlags=STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
		si.wShowWindow=SW_HIDE;
		si.hStdInput=pde.engine_read;
		si.hStdOutput=pde.engine_write;
		si.hStdError=pde.engine_write;
		if(!CreateProcess(path,"",NULL,NULL,true,0,NULL,EngineDir,&si,&pi))//打开引擎进程
		{
			ErrorBox("CreateProcess failed");
			status = -1;
			return false;
		}
		CloseHandle(pde.engine_read);
		CloseHandle(pde.engine_write);
		WaitForInputIdle(pi.hProcess,INFINITE);
		pde.hEProcess = pi.hProcess;
		CloseHandle(pi.hThread);
		SetCurrentDirectory(gameSet.CurDir);//恢复当前主应用程序的进程
		CreateEngineInfoBoard();
	}
	else
	{
		status = -1;
		return false;
	}

	return true;
}

//通过命名管道连接引擎
bool CEngine::LinkEngineWithNamed()
{
	status = 0;
	CreatePipeAndConnectClient();//创建命名管道并等待客户端连接

	char Filter[]="(exe files)|*.exe|(all files)|*.*||";//文件滤镜	
	CFileDialog FileName(true,NULL,NULL,Filter,NULL,gameSet.EngineInitDir);	
	if(FileName.DoModal()==IDOK)
	{
		path=FileName.GetFilePath();
		LPCTSTR folder=FileName.GetFolderPath(path);
		char EngineDir[MAX_PATH]={0};
		strcpy(EngineDir,folder);
				
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si,sizeof(si));
		si.cb=sizeof(si);
		si.dwFlags=STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
		si.wShowWindow=SW_SHOW;
		si.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
		si.hStdOutput=GetStdHandle(STD_OUTPUT_HANDLE);
		si.hStdError=GetStdHandle(STD_ERROR_HANDLE);
		if(!CreateProcess(path,"",NULL,NULL,true,0,NULL,EngineDir,&si,&pi))//打开引擎进程
		{
			ErrorBox("CreateProcess failed");
			status = -1;
			return false;
		}
		SetCurrentDirectory(gameSet.CurDir);//恢复当前主应用程序的进程
		CreateEngineInfoBoard();
	}
	else
	{
		status = -1;
		return false;
	}			

	WaitForSingleObject(ol.hEvent,INFINITE);//等待客户端成功建立连接

	WriteMsg("name?\n");//询问引擎名称	
	char rMsg[256];
	memset(rMsg,0,sizeof(rMsg));
	GetCommand("name",rMsg);

	strcpy(name,"Name: ");
	strcat(name,rMsg+strlen("name "));//设置引擎的名称
	return true;
}

//加载引擎
bool CEngine::LoadEngine()
{
	if(status!=-1)//引擎处于连接状态
	{
		MsgBox("Engine has loaded!","error",3000);
		return false;
	}
	indexBuf=0;
	if(linkType==UNNAMEDPIPE)
	{
		return LinkEngineWithUnnamed();
	}
	else if(linkType==NAMEDPIPE)
	{
		return LinkEngineWithNamed();
	}	
	return false;
}

VOID CEngine::ShowEngineWindow(int nCmdShow)
{
	if(status!=-1)
		ShowWindow(pde.console_hwnd,nCmdShow);
}

//创建引擎窗体
bool CEngine::CreateEngineInfoBoard()
{
	//为了保持对控制台类型引擎信息显示的原味性，程序创建了另外的一个控制台辅助显示程序，并通过匿名管道向辅助控制台写入引擎信息
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;
	sa.nLength=sizeof(sa);
	sa.lpSecurityDescriptor=NULL;//Default security attributes
	sa.bInheritHandle=true;//handle can be inherited

	if(!CreatePipe(&pde.console_read,&pde.console_write,&sa,BUFSIZE))//创建引擎与显示控制台之间互相通信的匿名管道
	{
		ErrorBox("CreatePipe failed");
		return false;
	}		
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	si.dwFlags=STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
	si.wShowWindow=SW_HIDE;
	si.hStdInput=pde.console_read;
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError=GetStdHandle(STD_ERROR_HANDLE);
	if(!CreateProcess("bin\\console1.exe",NULL,NULL,NULL,true,0,NULL,gameSet.CurDir,&si,&pi))//打开引擎进程
	{
		ErrorBox("CreateProcess console1 failed");
		return false;
	}
	CloseHandle(pde.console_read);
	pde.hCProcess = pi.hProcess;
	CloseHandle(pi.hThread);
	WaitForInputIdle(pi.hProcess,INFINITE);
	while((pde.console_hwnd=GetProcessMainWnd(pi.dwProcessId))==NULL)
	{//（由于进程创建函数是异步的）子进程刚建立时并不能立刻枚举窗口，需要等待
		Sleep(50);
	}
	//设置窗口标题为引擎路径
	SetWindowText(pde.console_hwnd,path);
	//获取窗口菜单句柄
	HMENU hMenu=GetSystemMenu(pde.console_hwnd,NULL);
	//使关闭按钮无效
	EnableMenuItem(hMenu,SC_CLOSE,MF_GRAYED);
	if(gameSet.swEngine==true)
		ShowWindow(pde.console_hwnd,SW_SHOW);
	return true;
}

//卸载引擎
bool CEngine::UnloadEngine()
{
	DWORD k;
	if(status == -1)//看是否已卸载
	{
		MsgBox("Engine has unloaded!","Msg",1500);
		return true;
	}
	if (!GetExitCodeProcess(pde.hEProcess, &k))//获取退出码
	{
		ErrorBox("Get exit code failed!");
	}
	else
	{
		if(k!=STILL_ACTIVE)//异常退出
		{
			MsgBox("Program has been ended for unknown reasons!","error",3000);
			if(linkType==UNNAMEDPIPE)
			{//关闭引擎的同时关闭显示窗口，释放依赖匿名管道
				TerminateProcess(pde.hCProcess,0);
				CloseHandle(pde.console_write);
				CloseHandle(pde.platform_write);
				CloseHandle(pde.platform_read);
			}
			status = -1;
			return true;
		}
	}
	if(TerminateProcess(pde.hEProcess,0))//卸载成功
	{
		if(linkType==UNNAMEDPIPE)
		{//关闭引擎的同时关闭显示窗口，释放依赖匿名管道
			TerminateProcess(pde.hCProcess,0);
			CloseHandle(pde.console_write);
			CloseHandle(pde.platform_write);
			CloseHandle(pde.platform_read);
		}
		MsgBox("UnLoadEngine succeed!","Msg",1500);
	}
	else
	{
		ErrorBox("UnLoadEngine failed");
		return false;
	}
	status = -1;
	return true;
}

//读取引擎消息
DWORD CEngine::ReadMsg(char *msg,int size)
{	
	DWORD dwRead;		
	HANDLE hFile=NULL;	
	if(linkType==UNNAMEDPIPE)
		hFile=pde.platform_read;
	else if(linkType==NAMEDPIPE)
		hFile=pde.hPipe;
	else
	{
		MsgBox("连接程序类型出错!","Error",0);
		return 0;
	}

	if(hFile==NULL)
	{
		MsgBox("与连接程序之间的通信管道出错!","Error",0);
		return 0;
	}
	if(!ReadFile(hFile,msg,size,&dwRead,NULL))//读取管道消息
	{
		ErrorBox("ReadMsg failed");
		return 0;
	}
	if(linkType==UNNAMEDPIPE)
		WriteFile(pde.console_write,msg,dwRead,&dwRead,NULL);
	return dwRead;
}

//往引擎写消息
DWORD CEngine::WriteMsg(char *msg)
{
	DWORD dwWrite;	
	HANDLE hFile=NULL;	
	if(linkType==UNNAMEDPIPE)
		hFile=pde.platform_write;
	else if(linkType==NAMEDPIPE)
		hFile=pde.hPipe;					
	else
	{
		MsgBox("连接程序类型出错!","Error",0);
		return 0;
	}

	if(hFile==NULL)
	{
		MsgBox("与连接程序之间的通信管道出错!","Error",0);
		return 0;
	}
	if(!WriteFile(hFile,msg,strlen(msg)+1,&dwWrite,NULL))//往管道写入消息
	{
		ErrorBox("WriteMsg failed");
		return 0;
	}
	if(linkType==UNNAMEDPIPE)
		WriteFile(pde.console_write,msg,dwWrite,&dwWrite,NULL);
	return dwWrite;
}

/**
 * GetCommand - 获取命令串
 * @cmd:	希望取得命令串的命令字
 * @CMD：	保存命令串的缓存区
 */
void CEngine::GetCommand(char *cmd,char *CMD)
{
	int dwSize,pos,len,i;
	len=strlen(cmd);
	while(1)
	{
		dwSize = ReadMsg(readBuffer + indexBuf, BUFSIZE);
		indexBuf+=dwSize;
		if(dwSize==BUFSIZE)//当 dwSize=BUFSIZE 时，具有字符截断的可能
		{
			pos=getcmd(readBuffer,indexBuf,cmd);
			if(pos==-1)//未找到命令字，进行防截断处理
			{
				for(i=indexBuf-1;indexBuf-i<len&&i>=0;i--)
				{
					if (readBuffer[i] == ' ' || readBuffer[i] == '\n' || readBuffer[i] == '\t' || readBuffer[i] == '\0')//向前寻找间隔符
						break;
				}
				if(indexBuf-i>=len)//最后串长度大于关键字长度，必不是关键字
					indexBuf=0;
				else
				{
					memmove(readBuffer, readBuffer + (i + 1), indexBuf - i - 1);
					indexBuf=indexBuf-i-1;
				}
			}
			else
			{
				for(i=pos+len+1;i<indexBuf;i++)
				{
					if (readBuffer[i] == ' ' || readBuffer[i] == '\r' || readBuffer[i] == '\n' || readBuffer[i] == '\t' || readBuffer[i] == '\0')//命令字后有完整参数  从引擎读入的数据中'\n'前包含'\r'
					{
						memcpy(CMD,readBuffer+pos,i-pos);
						CMD[i-pos]='\0';
						memmove(readBuffer, readBuffer + (i + 1), indexBuf - i - 1);
						indexBuf=indexBuf-i-1;
						return;
					}
				}
				//命令字后无完整参数，保留关键字并继续读取引擎信息
				memmove(readBuffer, readBuffer + pos, indexBuf - pos);
				indexBuf=indexBuf-pos;
			}
		}
		else
		{
			pos=getcmd(readBuffer,indexBuf,cmd);
			if(pos==-1)//未找到命令字，舍弃串
				indexBuf=0;
			else
			{
				for(i=pos+len+1;i<indexBuf;i++)
				{
					if (readBuffer[i] == ' ' || readBuffer[i] == '\r' || readBuffer[i] == '\n' || readBuffer[i] == '\t' || readBuffer[i] == '\0')//命令字后有完整参数
					{
						memcpy(CMD,readBuffer+pos,i-pos);
						CMD[i-pos]='\0';
						memmove(readBuffer, readBuffer + (i + 1), indexBuf - i - 1);
						indexBuf=indexBuf-i-1;
						return;
					}
				}
				//命令字后无完整参数，保留关键字并继续读取引擎信息
				memmove(readBuffer, readBuffer + pos, indexBuf - pos);
				indexBuf=indexBuf-pos;
			}
		}
	}
}

VOID CEngine::CreatePipeAndConnectClient()
{
	if(pde.hPipe!=NULL)return;//如果已创建命名管道，则不在重复创建

	char *PipeName="\\\\.\\pipe\\server";
	pde.hPipe=CreateNamedPipe(
		PipeName,
		PIPE_ACCESS_DUPLEX |FILE_FLAG_OVERLAPPED,//open mode
		PIPE_TYPE_MESSAGE |PIPE_READMODE_MESSAGE |PIPE_WAIT,//pipe mode
		PIPE_UNLIMITED_INSTANCES,//the maximum instance which can create the pipe having name specified by PipeName
		BUFSIZE,//the number of byte of the pipe buffer written by server process once
		BUFSIZE,//the number of byte of the pipe buffer read by server process once
		100000,//the time the client process can use to try to connect to server process
		NULL//default security attribute
		);
	if(pde.hPipe==INVALID_HANDLE_VALUE)
	{
		printf("CreateNamedPipe failed with %d\n",GetLastError());
		return;
	}
	ol.hEvent=CreateEvent(
		NULL,//default security attribute
		TRUE,//reset event state to nonsignaled automatically.
		FALSE,//Initial state is nonsignaled.
		NULL//The event is without a name.
		);
	if(!ConnectNamedPipe(pde.hPipe,&ol))//If a client process opens the pipe created by server process,the event specified by "ol.hEvent" will be signaled.
	{
		//printf("ConnectNamedPipe failed %d\n",GetLastError());
		//return;
	}
	return;
}
