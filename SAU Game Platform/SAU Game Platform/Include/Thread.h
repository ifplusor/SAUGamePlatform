#ifndef CTHREAD_H_GAMEPLATFORM
#define CTHREAD_H_GAMEPLATFORM

#include <windows.h>
#include <stdio.h>

#define EXPORT __declspec(dllexport)

class EXPORT CThread
{
public:
	HANDLE hThread;

	CThread();
	~CThread();
	HANDLE StartThread(LPTHREAD_START_ROUTINE lpStartAddress,
				   LPVOID lpParameter,
				   LPSECURITY_ATTRIBUTES lpThreadAttributes=NULL,
				   DWORD dwStackSize=0,
				   DWORD dwCreationFlags=0,
				   LPDWORD lpThreadId=0
				   );
	BOOL StopThread();
};
#endif