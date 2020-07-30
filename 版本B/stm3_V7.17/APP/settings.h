#ifndef __SETTINGS_H
#define __SETTINGS_H
#include "sys.h"
#include "includes.h" 	   	 
#include "common.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-设置 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//list结构体.链表结构
__packed typedef struct 
{
	u8 syslanguage;			//默认系统语言
	u8 lcdbklight;			//LED背光亮度 10~110.10代表最暗;110代表最亮
	u8 picmode;				//图片浏览模式:0,顺序循环播放;1,随机播放.
	u8 mp3mode;				//MP3播放模式:0,顺序循环播放;1,随机播放;2,单曲循环播放.
	u8 fmtxmode;	   		//FM发射模式:0,关闭;1,开启;
	u8 fmrxmode;	   		//FM播放模式:0,关闭后台播放;1,开启后台播放;
	u16 fmtxfreq;			//FM发射频率:单位0.1Mhz
	u8 audiosel;			//默认的音频选择输入,不参与变动保存.
 	u8 t3dxoff;				//3D传感器的3个校准值
	u8 t3dyoff;
	u8 t3dzoff;

	u8 saveflag;			//保存标志,0X0A,保存过了;其他,还从未保存	   
}_system_setings;

extern _system_setings systemset;	//在settings.c里面设置 

u8 sysset_time_set(u16 x,u16 y,u8 *hour,u8 *min,u8*caption);
u8 sysset_date_set(u16 x,u16 y,u16 *year,u8 *month,u8 *date,u8*caption);
u8 sysset_fmtx_freq_set(u16 x,u16 y,u16 *freq,u8*caption);
void sysset_mp3_effict_show(u16 x,u16 y,u8 mask,u8 vsval);
u8 sysset_mp3_effict_set(u16 x,u16 y,_vs10xx_obj *vsset,u8* caption);
u8 sysset_bklight_set(u16 x,u16 y,u8* caption,u16 *bkval);
u8 sysset_system_update_cpymsg(u8*pname,u8 pct,u8 mode);
void sysset_system_update(u8*caption);
void sysset_system_info(u16 x,u16 y,u8*caption);
void sysset_system_status(u16 x,u16 y,u8* caption);
void sysset_system_about(u16 x,u16 y,u8*caption);
u8 * set_search_caption(const u8 *mcaption);	 
void sysset_read_para(_system_setings * sysset);
void sysset_save_para(_system_setings * sysset);

u8 sysset_play(void);
										   
#endif












