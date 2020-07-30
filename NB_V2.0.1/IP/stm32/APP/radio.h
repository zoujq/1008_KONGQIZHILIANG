#ifndef __RADIO_H
#define __RADIO_H
#include "sys.h"
#include "includes.h" 	   	 
#include "common.h"		
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-收音机 代码	   
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
extern const u8* RADIO_DEMO_PIC;			//demo图片路径	    
extern const u8* RADIO_RSSI_PIC;			//RSSI图片路径	      
extern const u8* RADIO_STERO_PIC;			//STERO图片路径	      
extern const u8* RADIO_MONO_PIC;			//MONO图片路径

extern const u8* RADIO_PLAYR_PIC;			//播放 松开
extern const u8* RADIO_PLAYP_PIC;			//播放 按下
extern const u8* RADIO_PAUSER_PIC;			//暂停 松开
extern const u8* RADIO_PAUSEP_PIC;			//暂停 按下
extern const u8* RADIO_BTN_PIC_TBL[2][5];	//5个图片按钮的路径
////////////////////////////////////////////////////////////////////////////////////////////
#define RADIO_MAX_CH_VAL	50	//最大允许存储的电台数目

//RADIO控制管理器
__packed typedef struct 
{			 						 
	u8 vol; 					//音量
	u8 sta;						//模式及状态
								//[7]:单声道/立体声;0,立体声;1,强制单声道
								//[6:4]:保留
								//[3:2]:00,手动调台模式;01,半自动调台模式;10,全自动搜台;
								//[1:0]:频段选择;00,87~108,欧美频段;01:76~91,日本频段;10:76~108,扩展频段.
 	u8 curch;					//当前电台编号	
	u8 totalch;					//存储的电台总数目	
	u16 chtbl[RADIO_MAX_CH_VAL];//电台频率表(10Khz为单位),当不是全自动搜台的时候,默认频道0保存为当前频率.									    
 
	u8 saveflag;				//保存标志,0X0A,保存过了;其他,还从未保存	   
}_m_radiodev;	

void radio_load_ui(void);
void radio_show_freq(u8 *bfbase,u16 freq);
void radio_show_freqband(u8 min,u8 max);
void radio_band_process(_progressbar_obj *fmprgb,u8 band,u16 *fmin,u16 *fmax);
void radio_read_para(_m_radiodev * radio);
void radio_save_para(_m_radiodev * radio);

u8 radio_play(void);

#endif












