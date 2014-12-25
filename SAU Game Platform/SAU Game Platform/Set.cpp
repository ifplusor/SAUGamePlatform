/**
 * file: Set.h
 * date: 2014/11/1
 * version: 1.0
 * description: SAU_GamePalform 程序设置管理文件，内容包括：
 */


#include "Set.h"


GameSet gameSet;//程序设置


/*
 * InitialWithIni - 根据设置文件进行初始化
 */
VOID InitialWithIni()
{
	//默认棋种选择
	GetPrivateProfileString("CHESS", "Default", "\0", gameSet.DefualtChess, sizeof(gameSet.DefualtChess), ".\\Config.ini");
	//引擎信息显示，默认显示
	gameSet.swEngine=GetPrivateProfileInt("SHOW","EngineInfo",TRUE,".\\Config.ini");
	//棋盘材质，默认无棋盘材质
	GetPrivateProfileString("BOARD","Material","\0",gameSet.BoardFileName,sizeof(gameSet.BoardFileName),".\\Config.ini");
	//棋谱保存位置
	GetPrivateProfileString("SAVE","sfgDir","\0",gameSet.cmDir,sizeof(gameSet.cmDir),".\\Config.ini");
	//是否单步保存棋谱，默认为否
	gameSet.SingleStep=GetPrivateProfileInt("SAVE","SingleStep",FALSE,".\\Config.ini");
	//是否一起保存棋谱，默认为否
	gameSet.AllStep=GetPrivateProfileInt("SAVE","AllStep",FALSE,".\\Config.ini");
	//是否截图，默认为否
	gameSet.PrintScr=GetPrivateProfileInt("SAVE","PrintScr",FALSE,".\\Config.ini");
	//截图位置
	GetPrivateProfileString("SAVE","PrintScrDir","\0",gameSet.PrintScrDir,sizeof(gameSet.PrintScrDir),".\\Config.ini");
	//背景图片
	GetPrivateProfileString("PICTURE","BackPicture","\0",gameSet.BkPicFileName,sizeof(gameSet.BkPicFileName),".\\Config.ini");
	//黑方人物图片*/
	GetPrivateProfileString("PICTURE","BlackPer","\0",gameSet.BlcPerFileName,sizeof(gameSet.BlcPerFileName),".\\Config.ini");
	//白方人物图片
	GetPrivateProfileString("PICTURE","WhitePer","\0",gameSet.WhtPerFileName,sizeof(gameSet.WhtPerFileName),".\\Config.ini");
	//背景音乐文件名
	GetPrivateProfileString("MUSIC","BackMuick","\0",gameSet.BkMusicFileName,sizeof(gameSet.BkMusicFileName),".\\Config.ini");
	//音量，默认30
	gameSet.volume=GetPrivateProfileInt("MUSIC","Volume",30,".\\Config.ini");
	//单曲循环，默认循环
	gameSet.CirclePlay=GetPrivateProfileInt("MUSIC","CirclePlay",TRUE,".\\Config.ini");
	//停止播放，默认停止
	gameSet.StopPlay=GetPrivateProfileInt("MUSIC","StopPlay",TRUE,".\\Config.ini");
	//读取图片索引值
	gameSet.indexBdPic=GetPrivateProfileInt("PICTURE","indexBdPic",0,".\\Config.ini");
	gameSet.indexBkPic=GetPrivateProfileInt("PICTURE","indexBkPic",0,".\\Config.ini");
	gameSet.indexBlcPer=GetPrivateProfileInt("PICTURE","indexBlcPer",4,".\\Config.ini");
	gameSet.indexWhtPer=GetPrivateProfileInt("PICTURE","indexWhtPer",4,".\\Config.ini");

	return;
}

/*
 * RecordUsrOper - 记录用户设置
 */
VOID RecordUserOper()
{
	char num[10];
	//默认棋种选择
	WritePrivateProfileString("CHESS", "Default", gameSet.DefualtChess, ".\\Config.ini");
	/*写入引擎信息显示*/
	WritePrivateProfileString("SHOW","EngineInfo",itoa(gameSet.swEngine,num,10),".\\Config.ini");
	/*写入棋盘材质*/
	WritePrivateProfileString("BOARD","Material",gameSet.BoardFileName,".\\Config.ini");
	/*写入棋谱保存位置*/
	WritePrivateProfileString("SAVE","sfgDir",gameSet.cmDir,".\\Config.ini");
	/*写入是否单步保存棋谱*/
	WritePrivateProfileString("SAVE","SingleStep",itoa(gameSet.SingleStep,num,10),".\\Config.ini");
	/*写入是否一起保存棋谱*/
	WritePrivateProfileString("SAVE","AllStep",itoa(gameSet.AllStep,num,10),".\\Config.ini");
	/*写入是否截图*/
	WritePrivateProfileString("SAVE","PrintScr",itoa(gameSet.PrintScr,num,10),".\\Config.ini");
	/*写入截图位置*/
	WritePrivateProfileString("SAVE","PrintScrDir",gameSet.PrintScrDir,".\\Config.ini");
	/*写入背景图片*/
	WritePrivateProfileString("PICTURE","BackPicture",gameSet.BkPicFileName,".\\Config.ini");
	/*写入黑方人物图片*/
	WritePrivateProfileString("PICTURE","BlackPer",gameSet.BlcPerFileName,".\\Config.ini");
	/*写入白方人物图片*/
	WritePrivateProfileString("PICTURE","WhitePer",gameSet.WhtPerFileName,".\\Config.ini");
	/*写入背景音乐文件名*/
	WritePrivateProfileString("MUSIC","BackMuick",gameSet.BkMusicFileName,".\\Config.ini");
	/*写入音量*/
	WritePrivateProfileString("MUSIC","Volume",itoa(gameSet.volume,num,10),".\\Config.ini");
	/*写入单曲循环*/
	WritePrivateProfileString("MUSIC","CirclePlay",itoa(gameSet.CirclePlay,num,10),".\\Config.ini");
	/*写入停止播放*/
	WritePrivateProfileString("MUSIC","StopPlay",itoa(gameSet.StopPlay,num,10),".\\Config.ini");
	/*写入图片索引值*/
	WritePrivateProfileString("PICTURE","indexBdPic",itoa(gameSet.indexBdPic,num,10),".\\Config.ini");
	WritePrivateProfileString("PICTURE","indexBkPic",itoa(gameSet.indexBkPic,num,10),".\\Config.ini");
	WritePrivateProfileString("PICTURE","indexBlcPer",itoa(gameSet.indexBlcPer,num,10),".\\Config.ini");
	WritePrivateProfileString("PICTURE","indexWhtPer",itoa(gameSet.indexWhtPer,num,10),".\\Config.ini");
	return;
}
