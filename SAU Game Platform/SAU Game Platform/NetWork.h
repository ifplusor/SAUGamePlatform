#ifndef NETWORK_H_NETSHELL
#define NETWORK_H_NETSHELL


#include "Game Platform.h"
#include "CNetWnd.h"
#include "CChatWnd.h"


#define WM_SOCKET		0X1016
#define MAJOR_VERSION	1 
#define MINOR_VERSION	2 
#define WSA_MAKEWORD(x,y)  ((y)*256+(x)) 
#define MTU				1024					//最大传输单元


typedef struct _NetAddress{
	u_long ip;
	u_short port;
}NetAddress;

typedef struct _ServerInfo{
	NetAddress ad;
	SOCKET s;
}ServerInfo;

typedef struct _ClientInfo{
	char IP[30];
	char chessType[30];
	char state[30];
	char code[30];
}ClientInfo;


typedef struct _MessageBuf{//缓存
	char MessageBuffer[MTU];
	int size;
	SOCKET ns;
}MessageBuf;
typedef struct _MessageQueue{
	MessageBuf *MB[100];
	int head,tail;
}MessageQueue;


extern ServerInfo CLIENTINFO,SERVERINFO;
extern bool ConnectMode;
extern int NetWork;
extern int GameMode_2;

VOID NetShell(HWND hWnd,SOCKET ns,LPSTR command,INT size,BOOL send_tag);


#endif