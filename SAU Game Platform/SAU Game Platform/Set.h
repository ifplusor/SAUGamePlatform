/**
 * file: Set.h
 * date: 2014/11/1
 * version: 1.0
 * description: SAU_GamePalform 程序设置管理头文件，内容包括：
 */


#ifndef SET_H_GAMEPLATFROM
#define SET_H_GAMEPLATFROM

#include "Game Platform.h"


//程序设置
typedef struct _GameSet{
	//路径设置
	char CurDir[MAX_PATH];//当前应用程序的目录
	char BoardFileName[MAX_PATH];//棋盘材质路径
	char BkPicFileName[MAX_PATH];//背景图片路径
	char BlcPerFileName[MAX_PATH];//黑方人物图片路径
	char WhtPerFileName[MAX_PATH];//白方人物图片路径
	char BkMusicFileName[MAX_PATH];//背景音乐路径
	char cmDir[MAX_PATH];//棋谱保存路径
	char PrintScrDir[MAX_PATH];//截图路径
	char cmFileName[MAX_PATH];//棋谱文件路径
	char EngineInitDir[MAX_PATH-100];//引擎初始目录

	//平台设置项标志
	bool fullScr;//全屏 标志
	bool SingleStep;//单步保存棋谱 标志
	bool AllStep;//一起保存棋谱 标志
	bool PrintScr;//截图 标志
	bool CirclePlay;//循环播放背景音乐 标志
	bool StopPlay;//停止播放背景音乐 标志
	bool swEngine;//显示引擎信息 标志
	int indexBdPic;//棋盘材质
	int indexBkPic;//背景图片
	int indexBlcPer;//人物图片
	int indexWhtPer;
	int volume;//音量
}GameSet;


VOID InitialWithIni();
VOID RecordUserOper();

extern GameSet gameSet;


#endif