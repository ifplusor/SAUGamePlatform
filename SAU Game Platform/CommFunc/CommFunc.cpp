#include "CommFunc.h"

HINSTANCE hInst;

BOOL APIENTRY DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			hInst=hModule;
			break;
		case DLL_THREAD_ATTACH:
			hInst=hModule;
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

//删除文件夹下的文件
VOID __cdecl DelFile(char *dir,bool all)
{
	char szPath[MAX_PATH]={0};
	char directroy[MAX_PATH];
	strcpy(szPath,dir);
	strcat(szPath,"/*");
	WIN32_FIND_DATA fData;
	HANDLE hFile=FindFirstFile(szPath,&fData);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		return;
	} 
	do
	{
		if(fData.dwFileAttributes==FILE_ATTRIBUTE_ARCHIVE)//文件
		{
			char filename[MAX_PATH]={0};
			strcpy(filename,dir);
			strcat(filename,"\\");
			strcat(filename,fData.cFileName);
			DeleteFile(filename);		
		}
		else if(fData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY&&all)//文件夹
		{
			if(strcmp(fData.cFileName,".")==0 ||strcmp(fData.cFileName,"..")==0)//代表本级目录的'.'和上级目录的'..'
				continue;
			strcpy(directroy,dir);
			strcat(directroy,"\\");
			strcat(directroy,fData.cFileName);
			DelFile(directroy,all);
		}
	}while(FindNextFile(hFile,&fData));
	return;
}

//显示图片
BOOL __cdecl DisplayImage(HDC hDC, LPCTSTR szImagePath,RECT *rt)//展示bmp、jpg、gif（第一帧）图片
{
	//首先打开szImagePath指定文件
	HANDLE hFile=CreateFile(szImagePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
	if(hFile==INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD dwFileSize=GetFileSize(hFile, NULL); //获得图片文件的大小，用来分配全局内存
	HGLOBAL hImageMemory=GlobalAlloc(GMEM_MOVEABLE, dwFileSize); //给图片分配全局内存
	void *pImageMemory=GlobalLock(hImageMemory); //锁定内存
	DWORD dwReadedSize; //保存实际读取的文件大小
	ReadFile(hFile, pImageMemory, dwFileSize, &dwReadedSize, NULL); //读取图片到全局内存当中
	GlobalUnlock(hImageMemory); //解锁内存
	CloseHandle(hFile); //关闭文件句柄
	IStream *pIStream;//创建一个IStream接口指针，用来保存图片流
	IPicture *pIPicture;//创建一个IPicture接口指针，表示图片对象
	CreateStreamOnHGlobal(hImageMemory, false, &pIStream); //用全局内存初使化IStream接口指针
	OleLoadPicture(pIStream, 0, false, IID_IPicture, (LPVOID*)&(pIPicture));//用OleLoadPicture获得IPicture接口指针
	//得到IPicture COM接口对象后，你就可以进行获得图片信息、显示图片等操作
	OLE_XSIZE_HIMETRIC hmWidth;
	OLE_YSIZE_HIMETRIC hmHeight;	
	pIPicture->get_Width(&hmWidth); //用接口方法获得图片的宽和高
	pIPicture->get_Height(&hmHeight);	
	//在指定的DC上绘出图片
	pIPicture->Render(hDC,rt->left,rt->top,rt->right-rt->left,rt->bottom-rt->top,0,hmHeight,hmWidth,-hmHeight,NULL); 
	GlobalFree(hImageMemory); //释放全局内存
	pIStream->Release(); //释放
	pIPicture->Release(); //释放pIPicture	
	/*HWND hWnd=::WindowFromDC(hDC);
	static CPictureEx m_flash(hWnd);
	m_flash.Load(szImagePath);
	m_flash.Draw();*/
	return TRUE;
}

//播放音乐
VOID __cdecl PlayMusic(char *filename,bool circle,int vol)
{	
	char shortname[MAX_PATH];
	char cmd[MAX_PATH+100];	
	memset(shortname,sizeof(shortname),0);
	memset(cmd,sizeof(cmd),0);
		
	GetShortPathName(filename,shortname,sizeof(shortname)/sizeof(char));//如果文件名含有空格，则去掉空格
	if(circle)
		sprintf(cmd,"play %s repeat",shortname);
	else
		sprintf(cmd,"play %s",shortname);

	mciSendString(cmd,NULL,0,NULL);	//播放音乐	

	memset(cmd,sizeof(cmd),0);
	sprintf(cmd,"setaudio %s volume to %d",shortname,vol*10);
	mciSendString(cmd,0,0,0);
	return;
}

//停止音乐播放
VOID __cdecl StopMusic(char *filename)
{	
	char shortname[MAX_PATH]={0};
	char cmd[MAX_PATH+100]={0};		
		
	GetShortPathName(filename,shortname,sizeof(shortname)/sizeof(char));//如果文件名含有空格，则去掉空格
	sprintf(cmd,"stop %s",shortname);

	mciSendString(cmd,NULL,0,NULL);	//停止音乐
	return;
}

//截屏
BOOL __cdecl PrintScreen(char *filename,int left,int top,int width,int height)
{
	HDC hDC=GetDC(NULL);
	HDC hMemDC=CreateCompatibleDC(hDC);
	HBITMAP hBitmap=CreateCompatibleBitmap(hDC,width,height);
	HBITMAP hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);

	BitBlt(hMemDC,0,0,width,height,hDC,left,top,SRCCOPY);
	
	BITMAPINFO BmpInfo;
	ZeroMemory(&BmpInfo,sizeof(BmpInfo));
	BmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	BmpInfo.bmiHeader.biWidth=width;
	BmpInfo.bmiHeader.biHeight=height;
	BmpInfo.bmiHeader.biPlanes=1;
	BmpInfo.bmiHeader.biBitCount=32;
	BmpInfo.bmiHeader.biCompression=BI_RGB;	
	
	int OffSize=sizeof(BITMAPFILEHEADER)+BmpInfo.bmiHeader.biSize;
	int BmpSize=BmpInfo.bmiHeader.biWidth*BmpInfo.bmiHeader.biHeight*4;
	int FileSize=OffSize+BmpSize;

	BITMAPFILEHEADER FileHeader;
	ZeroMemory(&FileHeader,sizeof(FileHeader));
	FileHeader.bfSize=FileSize;
	FileHeader.bfType='B'+('M'<<8);
	FileHeader.bfOffBits=OffSize;

	PVOID pBmpData;
	pBmpData=HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,BmpSize);
	ZeroMemory(pBmpData,BmpSize);	
		
	if(GetDIBits(hMemDC,hBitmap,0,BmpInfo.bmiHeader.biHeight,pBmpData,&BmpInfo,DIB_RGB_COLORS)==0)
	{
		ErrorBox("GetDIBits faied");
		return FALSE;
	}

	HANDLE hFile;
	DWORD dwWriten;
	hFile=CreateFile(
		filename,
		GENERIC_READ |GENERIC_WRITE,
		FILE_SHARE_READ |FILE_SHARE_WRITE,
		NULL,CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		ErrorBox("CreateFile faied");
		return FALSE;
	}
	BOOL Written[3];
	Written[0]=WriteFile(hFile,&FileHeader,sizeof(FileHeader),&dwWriten,NULL);
	Written[1]=WriteFile(hFile,&BmpInfo,sizeof(BmpInfo),&dwWriten,NULL);
	Written[2]=WriteFile(hFile,pBmpData,BmpSize,&dwWriten,NULL);
	if(!Written[0] ||!Written[1] ||!Written[2])
	{
		ErrorBox("WriteFile faied");
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);

	HeapFree(GetProcessHeap(),0,pBmpData);	
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	DeleteDC(hDC);
	return TRUE;
}

/**
 * PrintScreenEx - 保存指定窗口指定区域的图像
 * @filename:	目标文件名
 * @hWnd:		窗口句柄
 * @rtCut:		窗口裁剪矩形
 * @Draw:		绘图函数
 * return:		TRUE：保存成功；False：保存失败。
 * description：对棋盘客户区窗口进行，通过传递窗口的绘图函数指针，实现对窗口实际图像的获取，而不是窗口所在区域在屏幕上所呈现的图像
 */
BOOL __cdecl PrintScreenEx(char *filename,HWND hWnd,RECT rtCut,void (*Draw)(HDC hDC))
{
	RECT rt;
	GetClientRect(hWnd,&rt);

	HDC hDC=GetDC(hWnd);
	HDC hMemDC=CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, rt.right - rt.left, rt.bottom - rt.top);
	SelectObject(hMemDC,hBitmap);
	Draw(hMemDC);

	int width = rtCut.right - rtCut.left;
	int height = rtCut.bottom - rtCut.top;
	HDC hPicDC = CreateCompatibleDC(hDC); 
	HBITMAP hPicBitmap = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(hPicDC, hPicBitmap);
	BitBlt(hPicDC, 0, 0, width, height, hMemDC, rtCut.left, rtCut.top, SRCCOPY);
	
	BITMAPINFO BmpInfo;
	ZeroMemory(&BmpInfo,sizeof(BmpInfo));
	BmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	BmpInfo.bmiHeader.biWidth=width;
	BmpInfo.bmiHeader.biHeight=height;
	BmpInfo.bmiHeader.biPlanes=1;
	BmpInfo.bmiHeader.biBitCount=32;
	BmpInfo.bmiHeader.biCompression=BI_RGB;	
	
	int OffSize=sizeof(BITMAPFILEHEADER)+BmpInfo.bmiHeader.biSize;//文件头大小
	int BmpSize=BmpInfo.bmiHeader.biWidth*BmpInfo.bmiHeader.biHeight*4;//图像信息大小
	int FileSize=OffSize+BmpSize;//文件大小

	BITMAPFILEHEADER FileHeader;
	ZeroMemory(&FileHeader,sizeof(FileHeader));
	FileHeader.bfSize=FileSize;
	FileHeader.bfType='B'+('M'<<8);
	FileHeader.bfOffBits=OffSize;

	PVOID pBmpData;
	pBmpData=HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,BmpSize);
	ZeroMemory(pBmpData,BmpSize);	
		
	if(GetDIBits(hPicDC,hPicBitmap,0,BmpInfo.bmiHeader.biHeight,pBmpData,&BmpInfo,DIB_RGB_COLORS)==0)
	{
		ErrorBox("GetDIBits faied");
		return FALSE;
	}

	HANDLE hFile;
	DWORD dwWriten;
	hFile=CreateFile(
		filename,
		GENERIC_READ |GENERIC_WRITE,
		FILE_SHARE_READ |FILE_SHARE_WRITE,
		NULL,CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		ErrorBox("CreateFile faied");
		return FALSE;
	}
	BOOL Written[3];
	Written[0]=WriteFile(hFile,&FileHeader,sizeof(FileHeader),&dwWriten,NULL);
	Written[1]=WriteFile(hFile,&BmpInfo,sizeof(BmpInfo),&dwWriten,NULL);
	Written[2]=WriteFile(hFile,pBmpData,BmpSize,&dwWriten,NULL);
	if(!Written[0] ||!Written[1] ||!Written[2])
	{
		ErrorBox("WriteFile faied");
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);

	HeapFree(GetProcessHeap(),0,pBmpData);	
	DeleteObject(hPicBitmap);
	DeleteDC(hPicDC);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	DeleteDC(hDC);
	return TRUE;
}

VOID __cdecl ErrorBox(LPTSTR ErrorInfo)//错误提示框
{
	CHAR error1[50],error2[20];
	strcpy(error1,ErrorInfo);
	sprintf(error2,"\n\nerror: %d",GetLastError());	
	strcat(error1,error2);	
	MessageBox(NULL,error1,"error",MB_OK);
}

VOID __cdecl MsgBox(LPTSTR msg)//消息框
{
	MessageBox(NULL,msg,"OK",MB_OK);
}


//获取指定进程ID的主窗口句柄
//=================================================================================================
typedef struct tagWNDINFO 
{ 
	DWORD dwProcessId; 
	HWND hWnd; 
} WNDINFO, *LPWNDINFO; 
BOOL CALLBACK YourEnumProc(HWND hWnd,LPARAM lParam) //枚举窗口进程ID
{ 
	DWORD dwProcessId; 
	GetWindowThreadProcessId(hWnd, &dwProcessId); 
	LPWNDINFO pInfo = (LPWNDINFO)lParam; 
	if(dwProcessId == pInfo->dwProcessId) 
	{ 
		pInfo->hWnd = hWnd; 
		return FALSE; 
	} 
	return TRUE; 
} 
HWND __cdecl GetProcessMainWnd(DWORD dwProcessId) //获取进程窗口句柄
{ 
	WNDINFO wi; 
	wi.dwProcessId = dwProcessId; 
	wi.hWnd = NULL; 
	EnumWindows(YourEnumProc,(LPARAM)&wi); 
	while(GetParent(wi.hWnd))
		wi.hWnd=GetParent(wi.hWnd);
	return wi.hWnd; 
}


//获取系统时间
VOID __stdcall GetSystemTimeEx(SYSTEMTIME *st,int region)
{
	GetSystemTime(st);
	st->wHour+=region;//中国时区位于东八区(简单进行时区相加会造成问题)
	if(st->wHour>=24)
	{
		st->wHour-=24;
		st->wDay++;
		if(st->wMonth==1 ||st->wMonth==3 ||st->wMonth==5 ||st->wMonth==7 ||st->wMonth==8 ||st->wMonth==10 ||st->wMonth==12)//有31天的月份
		{
			if(st->wDay>31)
			{
				st->wDay=1;
				st->wMonth++;
				if(st->wMonth>12)
				{
					st->wMonth=1;
					st->wYear++;
				}
			}
		}
		else if(st->wMonth==4 ||st->wMonth==6 ||st->wMonth==9 ||st->wMonth==11)//有30天的月份
		{
			if(st->wDay>30)
			{
				st->wDay=1;
				st->wMonth++;
			}
		}
		else//二月
		{
			if(st->wYear%400==0||(st->wYear%4==0&&st->wYear%100!=0))//闰年
			{
				if(st->wDay>29)
				{
					st->wDay=1;
					st->wMonth++;
				}			
			}
			else
			{
				if(st->wDay>28)
				{
					st->wDay=1;
					st->wMonth++;
				}			
			}
		}
	}
	return;
}
