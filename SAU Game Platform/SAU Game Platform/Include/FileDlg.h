#ifndef CFILEDIALOG_H_GAMEPLATFORM
#define CFILEDIALOG_H_GAMEPLATFORM

#include "windows.h"
#include <shlobj.h>

#define IMPORT __declspec(dllimport)

class IMPORT CFileDialog
{
public:	
	OPENFILENAME FileName;
	BOOL m_Open;
	char m_path[MAX_PATH];
	char m_folder[MAX_PATH-100];

	CFileDialog(bool Open,LPCTSTR DefExt,LPTSTR DefFileName,LPTSTR Filter,HWND hWnd,LPCTSTR InitDir=NULL);
	~CFileDialog();
	INT DoModal();
	LPCTSTR GetFilePath();
	LPCTSTR GetFolderPath(LPCTSTR path);
};
#endif
