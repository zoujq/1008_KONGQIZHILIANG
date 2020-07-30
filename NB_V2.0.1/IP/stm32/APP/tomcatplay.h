#ifndef __TOMCATPLAY_H
#define __TOMCATPLAY_H 	
#include "common.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-TOM猫 代码	   
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
extern const u8* TOMCAT_DEMO_PIC;//demo图片路径 	      
////////////////////////////////////////////////////////////////////////////////////////////

void tomcat_load_ui(void);
void tomcat_show_spd(u16 x,u16 y,u16 spd);
u8 tomcat_agcspd_set(u16 x,u16 y,u8 *agc,u16 *speed,u8*caption);
void tomcat_rec_mode(u8 agc);
void tomcat_play_wav(u8 *buf,u32 len);
void tomcat_data_move(u8* buf,u16 size,u16 dx);
u8 tomcat_play(void);



#endif























