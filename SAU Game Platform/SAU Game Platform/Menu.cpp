/**
 * file: Menu.cpp
 * date: 2014/9/10
 * version: 1.0
 * description: 用户菜单是构成SAU Game Platform图形用户界面的基本元素之一，其承担了用户对程序控制的全部操作。
 */


#include "Menu.h"
#include "Function.h"


HMENU hMenu;//菜单句柄
map<int,FUNCTION> menuFunction;//菜单项功能映射表


/*
 * mapFunction - 菜单项与操作功能映射
 * @menuID:		菜单项ID
 * @fp:			菜单格式文件指针
 */
void mapFunction(int menuID,FILE *fp)
{
	int type,index;
	fscanf(fp,"%d",&type);
	switch(type)
	{
	case 0:
		fscanf(fp,"%d",&index);
		switch(index)
		{
		case IDF_EXIT:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::Exit));
			break;
		case IDF_BLACK_LOAD:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::LoadBlackEngine));
			break;
		case IDF_BLACK_UNLOAD:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::UnloadBlackEngine));
			break;
		case IDF_WHITE_LOAD:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::LoadWhiteEngine));
			break;
		case IDF_WHITE_UNLOAD:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::UnloadWhiteEngine));
			break;
		case IDF_GAME_START:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::GameStart));
			break;
		case IDF_GAME_STOP:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::GameStop));
			break;
		case IDF_MANUAL_SHOT:
			menuFunction.insert(pair<int, FUNCTION>(menuID, Fuction::ShotBoard));
		default:
			break;
		}
		break;
	}
}


/**
 * CreateUserMenu - 创建用户菜单
 * @return：	返回菜单句柄
 */
HMENU CreateUserMenu()
{
	FILE *fp;
	char tag[30];
	HMENU hMenu=NULL;

	fp=fopen("default.menu","rt");
	if(fp==NULL)
	{
		MsgBox("Open menu script file fail!","error",0);
		return NULL;
	}
	while(fscanf(fp,"%s",tag)!=EOF)
	{
		if(strcmp(tag,"MAIN")==0)//进入主菜单
			hMenu=CreateMainMenu(fp);
	}

	return hMenu;
}

/**
 * CreateMainMenu - 创建主菜单
 * @return：	返回主菜单句柄
 */
HMENU CreateMainMenu(FILE *fp)
{
	char tag[30];
	char string[80],num[10];
	HMENU hMenu=NULL;
	MENUITEMINFO menuItem;

	menuItem.cbSize=sizeof(MENUITEMINFO);
	menuItem.fType=MFT_STRING;
	menuItem.dwTypeData=string;
	hMenu=CreateMenu();//创建菜单
	if(hMenu==NULL)
	{
		ErrorBox("Create Language Menu failed !");
		return NULL;
	}

	while(fscanf(fp,"%s",tag)!=EOF)
	{
		if(strcmp(tag,"ITEM")==0)//菜单项
		{
			menuItem.fMask=MIIM_TYPE|MIIM_ID|MIIM_DATA|MIIM_STATE;
			menuItem.fState=MFS_UNCHECKED;
			while(fscanf(fp,"%s",tag)!=EOF)//可选条目
			{
				if(strcmp(tag,"NAME")==0)
				{
					fscanf(fp,"%s",menuItem.dwTypeData);
					//fgets(menuItem.dwTypeData,sizeof(string)-1,fp);//名称
					menuItem.cch=strlen(menuItem.dwTypeData);
				}
				else if(strcmp(tag,"ID")==0)
				{
					fscanf(fp,"%d",&menuItem.dwItemData);//指定唯一标识
					menuItem.wID=menuItem.dwItemData;
				}
				else if(strcmp(tag,"FUNCTION")==0)
				{
					mapFunction(menuItem.dwItemData,fp);
				}
				else if(strcmp(tag,"ENDI")==0)
				{
					if(!InsertMenuItem(hMenu,-1,true,&menuItem))//插入菜单项
						ErrorBox("Insert MenuItem failed !");
					break;
				}
			}
		}
		else if(strcmp(tag,"SUB")==0)//子菜单
		{
			menuItem.fMask=MIIM_TYPE|MIIM_SUBMENU;
			fscanf(fp,"%s",tag);
			fscanf(fp,"%s",menuItem.dwTypeData);
			//fgets(menuItem.dwTypeData,sizeof(string)-1,fp);//名称
			menuItem.cch=strlen(menuItem.dwTypeData);
			menuItem.hSubMenu=CreateSubMenu(fp);
			if(!InsertMenuItem(hMenu,-1,true,&menuItem))//插入子菜单
				ErrorBox("Insert MenuItem failed !");
		}
		else if(strcmp(tag,"ENDM")==0)//主菜单结束
			break;
	}

	return hMenu;
}

/**
 * CreateSubMenu - 创建子菜单
 * @return：	返回子句柄
 */
HMENU CreateSubMenu(FILE *fp)
{
	char tag[30];
	char string[80];
	int type;
	HMENU hMenu=NULL;
	MENUITEMINFO menuItem;

	menuItem.cbSize=sizeof(MENUITEMINFO);
	menuItem.fType=MFT_STRING;
	menuItem.dwTypeData=string;
	hMenu=CreatePopupMenu();//创建菜单
	if(hMenu==NULL)
	{
		ErrorBox("Create Language Menu failed !");
		return NULL;
	}

	while(fscanf(fp,"%s",tag)!=EOF)
	{
		if(strcmp(tag,"ITEM")==0)//菜单项
		{
			menuItem.fMask=MIIM_TYPE|MIIM_ID|MIIM_DATA|MIIM_STATE;
			menuItem.fState=MFS_UNCHECKED;
			while(fscanf(fp,"%s",tag)!=EOF)//可选条目
			{
				if(strcmp(tag,"NAME")==0)
				{
					fscanf(fp,"%s",menuItem.dwTypeData);
					//fgets(menuItem.dwTypeData,sizeof(string)-1,fp);//名称
					menuItem.cch=strlen(menuItem.dwTypeData);
				}
				else if(strcmp(tag,"ID")==0)
				{
					fscanf(fp,"%d",&menuItem.dwItemData);//指定唯一标识
					menuItem.wID=menuItem.dwItemData;
				}
				else if(strcmp(tag,"FUNCTION")==0)
				{
					mapFunction(menuItem.dwItemData,fp);
				}
				else if(strcmp(tag,"ENDI")==0)
				{
					if(!InsertMenuItem(hMenu,-1,true,&menuItem))//插入菜单项
						ErrorBox("Insert MenuItem failed !");
					break;
				}
			}
		}
		else if(strcmp(tag,"SUB")==0)//子菜单
		{
			menuItem.fMask=MIIM_TYPE|MIIM_SUBMENU;
			fscanf(fp,"%s",tag);
			fscanf(fp,"%s",menuItem.dwTypeData);
			//fgets(menuItem.dwTypeData,sizeof(string)-1,fp);//名称
			menuItem.cch=strlen(menuItem.dwTypeData);
			menuItem.hSubMenu=CreateSubMenu(fp);
			if(!InsertMenuItem(hMenu,-1,true,&menuItem))//插入子菜单
				ErrorBox("Insert MenuItem failed !");
		}
		else if(strcmp(tag,"ENDS")==0)//子菜单结束
			break;
	}

	return hMenu;
}