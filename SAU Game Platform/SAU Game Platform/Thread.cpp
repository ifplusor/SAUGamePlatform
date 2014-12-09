#include "Thread.h"


CThread::CThread()
{
	hThread = NULL;
}

CThread::~CThread()
{

}

/***启动线程***/
HANDLE CThread::StartThread(
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	DWORD dwStackSize,
	DWORD dwCreationFlags,
	LPDWORD lpThreadId
	)
{
	hThread = CreateThread(
		lpThreadAttributes,
		dwStackSize,
		lpStartAddress,
		lpParameter,
		dwCreationFlags,
		lpThreadId
		);
	if (hThread == NULL)
	{
		ErrorBox("CreateThread failed");
	}
	return hThread;
}

/***终止线程***/
BOOL CThread::StopThread()
{
	if (!TerminateThread(hThread, 0))
	{
		ErrorBox("TerminateThread failed");
		return FALSE;
	}
	hThread = NULL;
	return TRUE;
}