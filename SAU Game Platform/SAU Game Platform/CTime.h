#ifndef CTIME_H_GAMEPLATFORM
#define CTIME_H_GAMEPLATFORM

#include "Game Platform.h"

typedef struct _LastTime{
	int hours;
	int minutes;
	int seconds;
	DWORD sumTime;
}LastTime;

class CTime
{
public:
	CTime(){killed=true;};
	~CTime(){};

	INT UpdataTime(char *strBlcTime,char *strWhtTime);
	VOID StartTimer(HWND hTimerWnd);
	bool StopTimer();
	VOID timeDeal(int player);	//时间转换
	LastTime GetTime(int side);
private:
	HWND hSetWnd;
	bool killed;//计时器是否已销毁
	DWORD startTime;
	LastTime blcTime,whtTime;
};

#endif