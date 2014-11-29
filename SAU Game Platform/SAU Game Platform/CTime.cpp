#include "CTime.h"
#include "GameType.h"


INT CTime::UpdateTime(char *strBlcTime,char *strWhtTime)
{
	BYTE player;//当前轮到哪个选手就计哪个选手的时间
	player=CT_GetCurPlayer();
	char *time;
	int len=strlen("Time: ");
	if(player==BLACK)//计算黑方时间
	{
		time=strBlcTime;
		strcpy(time,"Time: ");
		blcTime.sumTime=GetTickCount()-startTime-whtTime.sumTime;
		timeDeal(BLACK);
		if(blcTime.hours>0)
		{
			itoa(blcTime.hours,&time[len],10);
			len=strlen(time);
			time[len++]=':';
		}
		if(blcTime.minutes<10)//分数小于10，则补零
		{			
			time[len++]='0';
			time[len++]='0'+blcTime.minutes;
		}
		else
		{
			itoa(blcTime.minutes,&time[len],10);
			len=strlen(time);
		}
		time[len++]=':';
		if(blcTime.seconds<10)//秒数小于10，则补零
		{			
			time[len++]='0';
			time[len++]='0'+blcTime.seconds;
		}
		else
		{
			itoa(blcTime.seconds,&time[len],10);
			len=strlen(time);
		}
		time[len]='\0';
	}
	else//计算白方时间									
	{
		time=strWhtTime;
		strcpy(time,"Time: ");
		whtTime.sumTime=GetTickCount()-startTime-blcTime.sumTime;
		timeDeal(WHITE);
		if(whtTime.hours>0)
		{
			itoa(whtTime.hours,&time[len],10);
			len=strlen(time);
			time[len++]=':';
		}
		if(whtTime.minutes<10)//分数小于10，则补零
		{			
			time[len++]='0';
			time[len++]='0'+whtTime.minutes;
		}
		else
		{
			itoa(whtTime.minutes,&time[len],10);
			len=strlen(time);
		}
		time[len++]=':';
		if(whtTime.seconds<10)//秒数小于10，则补零
		{			
			time[len++]='0';
			time[len++]='0'+whtTime.seconds;
		}
		else
		{
			itoa(whtTime.seconds,&time[len],10);
			len=strlen(time);
		}
		time[len]='\0';
	}		
	return player;
}

VOID CTime::timeDeal(int player)	//时间转换
{
	if(player==BLACK)
	{
		blcTime.seconds=blcTime.sumTime/1000%60;
		blcTime.minutes=blcTime.sumTime/60000%60;
		blcTime.hours=blcTime.sumTime/3600000;
	}
	else if(player==WHITE)
	{
		whtTime.seconds=whtTime.sumTime/1000%60;
		whtTime.minutes=whtTime.sumTime/60000%60;
		whtTime.hours=whtTime.sumTime/3600000;
	}
}

VOID CTime::StartTimer(HWND hTimerWnd)
{
	hSetWnd=hTimerWnd;
	blcTime.sumTime=whtTime.sumTime=0;//时间清零
	killed=false;
	SetTimer(hSetWnd,0,250,NULL);//启动计时器	（秒级计时器在双方交换行棋时可能并不准确）
	startTime=GetTickCount();
}

//停止计时
bool CTime::StopTimer()
{	
	if(killed)
		return false;
	if(!KillTimer(hSetWnd,0))//先判断KillTimer函数返回值（KillTimer函数已执行）
	{
		ErrorBox("KillTimer failed");		
		return false;
	}
	killed=true;
	return true;
}

LastTime CTime::GetTime(int side)
{
	if(side==BLACK)
		return blcTime;
	else
		return whtTime;
}