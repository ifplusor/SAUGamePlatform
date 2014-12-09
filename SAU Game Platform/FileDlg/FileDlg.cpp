#include "FileDlg.h"

HWND hDlg;

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

//把易用的滤镜转换成系统能识别的滤镜
void ConvertFilter(LPTSTR Filter)
{//滤镜指向一对以空字符结束的过滤字符串的一个缓冲。缓冲中的最后一个字符串必须以两个NULL字符结束
	do
	{
		if((*Filter)=='|')
			*Filter='\0';
	}
	while(*(++Filter));
}

//转换文件全路径
void ConvertFileName(char* str)
{
	char path[MAX_PATH];
	memset(path,0,MAX_PATH);
	int i=3;
	strcpy(path,str);
	str+=3;
	*str='\\';	
	do
	{	
		str++;
		if(*str=='\\')
		{
			*str=path[i];
			*++str='/';
			i++;
		}			
		else
			*str=path[i];		
		i++;

	}while(path[i]!='\0');
	str-=i;
}

UINT CALLBACK OFNHookProc(
  HWND hdlg,      // handle to child dialog window
  UINT uiMsg,     // message identifier
  WPARAM wParam,  // message parameter
  LPARAM lParam   // message parameter
)
{
//	hDlg=hdlg;
	return 0;
}

//构造函数,在初始化文件名时总是出错
CFileDialog::CFileDialog(bool Open,LPCTSTR DefExt,LPTSTR DefFileName,LPTSTR Filter,HWND hWnd,LPCTSTR InitDir)
{
	CHAR m_FileName[MAX_PATH];
	ZeroMemory(m_FileName,sizeof(m_FileName));
	ZeroMemory(&FileName,sizeof(FileName));
	FileName.lStructSize=sizeof(FileName);
	FileName.hwndOwner=hWnd;
	FileName.lpstrInitialDir=InitDir;//default is Current directory
	FileName.Flags=OFN_EXPLORER |OFN_ENABLESIZING |OFN_OVERWRITEPROMPT;// |OFN_ENABLEHOOK ;
	//FileName.lpfnHook=(LPOFNHOOKPROC)OFNHookProc;
	m_FileName[0]='\0';
	FileName.lpstrFile=m_FileName;
	FileName.nMaxFile=MAX_PATH;					

	ConvertFilter(Filter);
	FileName.lpstrFilter=Filter;						

	if(DefExt!=NULL)
	{		
		FileName.lpstrDefExt=DefExt;
	}								
	m_Open=Open;
}

CFileDialog::~CFileDialog()
{

}

//模式选择（打开或保存）
INT CFileDialog::DoModal()
{
	INT ID;
	if(m_Open)//模式标志
	{				
		//FileName.lpstrInitialDir="F:\\GamePlatform\\GamePlatform\\BoardMaterial\\";
		ID=GetOpenFileName(&FileName);//打开文件对话框
	}
	else
	{
		ID=GetSaveFileName(&FileName);//保存文件对话框
	}	
	return (ID?IDOK:IDCANCEL);
}

//获取文件的全路径
LPCTSTR CFileDialog::GetFilePath()
{			
	memset(m_path,0,MAX_PATH);
	strcpy(m_path,FileName.lpstrFile);
	//ConvertFileName(m_path);
	return m_path;
}

//获取文件夹路径
LPCTSTR CFileDialog::GetFolderPath(LPCTSTR path)
{			
	char *p=m_folder;
	lstrcpyn(m_folder,path,strlen(path));//复制路径
	while((*++p));
	while(1)//回查，找到文件夹路径
	{
		if((*p)=='\\')
		{
			(*p)='\0';
			break;
		}		
		(*p)='\0';//清零
		p--;
	}
	return m_folder;
}
