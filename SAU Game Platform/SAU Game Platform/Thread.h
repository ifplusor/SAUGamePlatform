#ifndef CTHREAD_H_GAMEPLATFORM
#define CTHREAD_H_GAMEPLATFORM

#include "Game Platform.h"

class CThread
{
public:
	HANDLE hThread;

	CThread();
	~CThread();
	HANDLE StartThread(LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL,
		DWORD dwStackSize = 0,
		DWORD dwCreationFlags = 0,
		LPDWORD lpThreadId = 0
		);
	BOOL StopThread();
};
#endif