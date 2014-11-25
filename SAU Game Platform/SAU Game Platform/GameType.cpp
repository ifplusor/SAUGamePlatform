/**
 * file: GameType.cpp
 * date: 2014/9/16
 * version: 1.0
 * description: 棋种管理：管理棋种包，维护棋种支持模块接口
 */


#include "GameType.h"
#include "CMainWnd.h"



//平台棋种支持模块接口函数指针
_CheckModule CT_CheckModule=NULL;
_InitModule CT_InitModule=NULL;
_OnSize CT_OnSize=NULL;
_DrawBoard CT_DrawBoard=NULL;
_OnLButtonDown CT_OnLButtonDown=NULL;
_ProcessMsg CT_ProcessMsg=NULL;
_OnRun CT_OnRun=NULL;
_GetCurPlayer CT_GetCurPlayer=NULL;


int chesstype=0;//棋种标志
int chessNum=0;//可用棋种包数量
struct _CHESSTYPE chessType[25];//棋种描述


/**
 * GetChessTypeResourse - 获取棋种包资源
 */
BOOL GetChessTypeResourse()
{
	HANDLE hFile;
	WIN32_FIND_DATA fData;
	char directory[MAX_PATH]={0};
	char filename[MAX_PATH]={0};
	char check[50];
	strcpy(directory,gameSet.CurDir);
	strcat(directory,"\\ChessType\\*.dll");
	//检索可用语言包
	hFile=FindFirstFile(directory,&fData);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		ErrorBox("Find first ChessType resourse failed!");
		return FALSE;
	}
	chessNum=0;
	do{
		strcpy(filename,gameSet.CurDir);
		strcat(filename,"\\ChessType\\");
		strcat(filename,fData.cFileName);
		chessType[chessNum].chessTP=(HINSTANCE)LoadLibrary(filename);
		if(chessType[chessNum].chessTP==NULL)
		{
			ErrorBox("Load ChessType resource failed !");
			return FALSE;
		}
		else
		{
			CT_CheckModule=(_CheckModule)GetProcAddress(chessType[chessNum].chessTP,"CheckModule");
			if(CT_CheckModule!=NULL)
			{
				memset(check,0,sizeof(check));
				CT_CheckModule(check,chessType[chessNum].chessStr,&chessType[chessNum].type);
				if(strcmp("SAU_GamePlatform chessType",check)==0)//校验组件正确性
					chessNum++;
				else
					FreeLibrary(chessType[chessNum].chessTP);//释放不正确组件
			}
			else
				FreeLibrary(chessType[chessNum].chessTP);//释放不正确组件
		}
	}while(FindNextFile(hFile,&fData));

	if(chessNum==0)
		return FALSE;
	return TRUE;
}

/*
 * CreateChessTypeMenu - 创建棋种子菜单
 */
VOID CreateChessTypeMenu(HMENU hMenu)
{
	char string[25];
	HMENU hChessType=NULL;
	MENUITEMINFO menuItem;
	menuItem.cbSize=sizeof(MENUITEMINFO);
	menuItem.fMask=MIIM_TYPE|MIIM_ID|MIIM_DATA|MIIM_STATE;
	menuItem.fType=MFT_STRING;
	menuItem.cch=25;
	menuItem.dwTypeData=string;
	hChessType=CreatePopupMenu();//创建子菜单
	if(hChessType==NULL)
	{
		ErrorBox("Create ChessType Menu failed !");
		return;
	}
	for(int i=0;i<chessNum;i++)
	{
		menuItem.fState=MFS_UNCHECKED;
		menuItem.dwItemData=CHESSTYPE+i;//指定唯一标识
		menuItem.wID=CHESSTYPE+i;
		strcpy(menuItem.dwTypeData,chessType[i].chessStr);
		menuItem.cch=strlen(menuItem.dwTypeData);
		if(!InsertMenuItem(hChessType,-1,true,&menuItem))//将棋种项插入子菜单
			ErrorBox("Insert MenuItem failed !");
	}
	menuItem.cch=25;
	if(!GetMenuItemInfo(hMenu,IDM_GAME_SELECT,false,&menuItem))//获取选择棋种菜单项信息
	{
		ErrorBox("Get MenuItem information failed !");
	}
	else
	{
		menuItem.fMask=MIIM_TYPE|MIIM_SUBMENU;//扩充菜单项打开子菜单
		menuItem.hSubMenu=hChessType;
		if(!SetMenuItemInfo(hMenu,IDM_GAME_SELECT,false,&menuItem))
			ErrorBox("Set MenuItem information failed !");
	}
//	CloseHandle(hChessType);//关闭棋种子菜单句柄有时会出问题，不知道是什么原因
}

/*
 * InitialChessType - 初始化棋种接口
 * @i	棋种索引
 * description：函数用于初始化棋种支持模块，在程序主窗口创建之前进行调用。
 *				在程序运行期间，棋种模块内部有关棋盘绘制的参数变化依赖于主窗口的状态，并在窗口处理响应消息时调用接口函数进行处理。
 *				MainWnd窗口创建先处理WM_CREATE消息，再WM_SIZE消息，后处理WM_PAINT消息。窗口销毁时处理WM_DESTROY消息
 *				CT_InitModule函数的调用在MainWnd创建后处理WM_CREATE消息时完成，用于传递窗口句柄和程序设置信息
 *				CT_OnSize函数的调用在MainWnd创建后处理WM_SIZE消息时完成，用于传递棋盘位置
 *				CT_DrawBoard函数的调用在MainWnd重绘处理WM_PAINT消息时完成，用于绘制棋盘内容
 *				CT_ExitModule函数的调用在MainWnd销毁处理WM_DESTROY消息时完成，用于释放棋种模块中的系统资源
 */
VOID InitialChessType(int i,HMENU hMenu)
{
	chesstype=i;//置新语言标志
	for(int i=0;i<chessNum;i++)
	{
		if(i==chesstype)
			CheckMenuItem(hMenu,CHESSTYPE+i,MF_CHECKED);
		else
			CheckMenuItem(hMenu,CHESSTYPE+i,MF_UNCHECKED);			
	}
	//动态获取DLL库中函数地址
	CT_InitModule=(_InitModule)GetProcAddress(chessType[chesstype].chessTP,"InitModule");
	CT_OnSize=(_OnSize)GetProcAddress(chessType[chesstype].chessTP,"OnSize");
	CT_DrawBoard=(_DrawBoard)GetProcAddress(chessType[chesstype].chessTP,"DrawBoard");
	CT_OnLButtonDown=(_OnLButtonDown)GetProcAddress(chessType[chesstype].chessTP,"OnLButtonDown");
	CT_ProcessMsg=(_ProcessMsg)GetProcAddress(chessType[chesstype].chessTP,"ProcessMsg");
	CT_OnRun=(_OnRun)GetProcAddress(chessType[chesstype].chessTP,"OnRun");
	CT_GetCurPlayer=(_GetCurPlayer)GetProcAddress(chessType[chesstype].chessTP,"GetCurPlayer");

	return;
}

/*
 * SetChessType - 设置棋种
 * @i:	棋种索引
 */
VOID SetChessType(int i,HMENU hMenu)
{	
	if(i==chesstype)//所选棋种非当前棋种
		return;
	chesstype=i;//置新语言标志
	for(int i=0;i<chessNum;i++)
	{
		if(i==chesstype)
			CheckMenuItem(hMenu,CHESSTYPE+i,MF_CHECKED);
		else
			CheckMenuItem(hMenu,CHESSTYPE+i,MF_UNCHECKED);			
	}
	//动态获取DLL库中函数地址
	CT_InitModule=(_InitModule)GetProcAddress(chessType[chesstype].chessTP,"InitModule");
	CT_OnSize=(_OnSize)GetProcAddress(chessType[chesstype].chessTP,"OnSize");
	CT_DrawBoard=(_DrawBoard)GetProcAddress(chessType[chesstype].chessTP,"DrawBoard");
	CT_OnLButtonDown=(_OnLButtonDown)GetProcAddress(chessType[chesstype].chessTP,"OnLButtonDown");
	CT_ProcessMsg=(_ProcessMsg)GetProcAddress(chessType[chesstype].chessTP,"ProcessMsg");
	CT_OnRun=(_OnRun)GetProcAddress(chessType[chesstype].chessTP,"OnRun");
	CT_GetCurPlayer=(_GetCurPlayer)GetProcAddress(chessType[chesstype].chessTP,"GetCurPlayer");

	CT_InitModule(MainWnd->hWnd,&gameSet);
	CT_OnSize(MainWnd->GetBoardPos());
	InvalidateRect(MainWnd->hWnd,NULL,false);
	//设置自动截图目录
	strcpy(gameSet.PrintScrDir,gameSet.CurDir);
	strcat(gameSet.PrintScrDir,"\\chess manual\\");
	strcat(gameSet.PrintScrDir,chessType[chesstype].chessStr);
	strcat(gameSet.PrintScrDir,"\\bmp");
	//设置棋谱保存目录
	strcpy(gameSet.cmDir,gameSet.CurDir);
	strcat(gameSet.cmDir,"\\chess manual\\");
	strcat(gameSet.cmDir,chessType[chesstype].chessStr);
	strcat(gameSet.cmDir,"\\sgf");

	return;
}
