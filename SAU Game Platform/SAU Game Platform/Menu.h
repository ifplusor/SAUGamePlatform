/**
 * file: Menu.h
 * date: 2014/9/10
 * version: 1.0
 * description: 
 */


#ifndef MENU_H_GAMEPLATFORM
#define MMNU_H_GAMEPLATFORM


#include "Game Platform.h"


void mapFunction(int menuID,FILE *fp);//菜单项与操作功能映射
HMENU CreateUserMenu();//创建用户菜单
HMENU CreateMainMenu(FILE *fp);//创建主菜单
HMENU CreateSubMenu(FILE *fp);//创建子菜单


extern HMENU hMenu;//菜单句柄
extern map<int,FUNCTION> menuFunction;//菜单项功能映射表


#endif