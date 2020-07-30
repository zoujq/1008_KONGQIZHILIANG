#ifndef __MP3PLAYER_H
#define __MP3PLAYER_H
#include "sys.h"
#include "includes.h" 	   	 
#include "common.h"
#include "ff.h"	
#include "gui.h"
#include "lyric.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-音乐播放器实现 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   


////////////////////////////////////////////////////////////////////////////////////////////
//各图标/图片路径
extern const u8 *FLAC_PATCH_PATH;			//FLAC播放的PATCH的存放位置
extern const u8 *SPEC_PATCH_PATH;			//频谱分析的PATCH的存放位置
extern const u8* MP3_DEMO_PIC;				//demo图片路径
extern const u8* MP3_BTN_PIC_TBL[2][5];  	//5个图片按钮的路径
extern const u8* MP3_PLAYR_PIC;				//播放 松开
extern const u8* MP3_PLAYP_PIC;				//播放 按下
extern const u8* MP3_PAUSER_PIC;			//暂停 松开
extern const u8* MP3_PAUSEP_PIC;			//暂停 按下
////////////////////////////////////////////////////////////////////////////////////////////
#define MP3_TITLE_COLOR   	0XFFFF		//播放器标题颜色	
#define MP3_TITLE_BKCOLOR   0X0000		//播放器标题背景色	

#define MP3_INFO_COLOR   	0X8410		//信息字体的颜色	
#define MP3_MAIN_BKCOLOR    0X18E3		//主背景色	
/////////////////////////////////////////////////////
#define FFT_TOP_COLOR		0XF800		//柱子顶部的颜色
#define FFT_BANDS_COLOR		0X07FF		//柱子的颜色


/////////////////////////////////////////////
#define MP3_LRC_MCOLOR       0XF810//0XFFE0		//当前歌词颜色为黄色
#define MP3_LRC_SCOLOR       0X07FF//0X07FF		//前一句和后一句歌词颜色为青色			    

//FFT
#define FFT_BANDS			14			//14个频段

//MP3控制管理器
__packed typedef struct 
{			 						 
	vu8 sta; 			//播放器状态
						//bit7:0,空闲;1,MP3播放中(包括暂停状态) 
						//bit6:0,没有切歌;1,已经切歌了.
						//bit5:0,正常播放;1,暂停播放;
						//bit4:0,普通文件,1,flac文件	  			 
						//bit3~1:保留.
						//bit0:0,请求终止播放;1,允许播放
	u8 mode;			//播放模式
						//0,全部循环;1,单曲循环;2,随机播放;
	u8 *path;			//当前文件夹路径
	u8 *name;			//当前播放的MP3歌曲名字
	u16 namelen;		//name所占的点数.
	u16 curnamepos;		//当前的偏移

	u16 playtime;		//播放时长	
	u16 kbps;			//比特率
	u16 curindex;		//当前播放的音频文件索引
	u16 mfilenum;		//音乐文件数目	    
	u16 *mfindextbl;	//音频文件索引表

	u8 fft_topval_tbl[FFT_BANDS];	//频谱顶值表
	u8 fft_curval_tbl[FFT_BANDS];	//频谱当前值表
	u8 fft_time_tbl[FFT_BANDS];		//顶值停留时间表

	FIL* fmp3;			//MP3文件	 
}_m_mp3dev;	
			  
extern _m_mp3dev *mp3dev;
						  	    													  
//extern OS_EVENT * mp3mbox;//事件控制块	 
extern u8 mp3sta;//MP3状态	  


void music_play_task(void *pdata);
void mp3_stop(_m_mp3dev *mp3devx);
u8 mp3_filelist(_m_mp3dev *mp3devx);
void mp3_load_ui(void);
void mp3_show_vol(u8 pctx);
void mp3_time_show(u16 sx,u16 sy,u16 sec);
void fft_show_oneband(u16 x,u16 y,u16 width,u16 height,u16 curval,u16 topval);
void mp3_specalz_show(_m_mp3dev *mp3devx,u16 *pdt);
//void mp3_info_upd(_m_mp3dev *mp3devx,_progressbar_obj* mp3prgbx,_progressbar_obj* volprgbx,_lyric_obj* lrcx);
//void mp3_lrc_show(_m_mp3dev *mp3devx,_lyric_obj* lrcx);
u8 mp3_play(void);	    

#endif












