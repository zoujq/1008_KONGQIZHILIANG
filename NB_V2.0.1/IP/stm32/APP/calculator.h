#ifndef __CALCULATOR_H
#define __CALCULATOR_H 	
#include "common.h"	   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-科学计算器 代码	   
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
extern const u8* CALC_UP_BTN_TBL[29];	//松开时按键图标路径
extern const u8* CALC_DOWN_BTN_TBL[29];	//按下时按键图标路径
////////////////////////////////////////////////////////////////////////////////////////////
 
u8 calc_play(void);
u8 calc_show_res(u8 *fontbase,double res);
void calc_input_fresh(u8 *fontbase,u8* calc_sta,u8 *inbuf,u8 len);
void calc_show_flag(u8 *fontbase,u8 fg);
void calc_show_exp(short exp);
u8 calc_exe(u8 *fontbase,double *x1,double *x2,u8 *buf,u8 ctype,u8 *calc_sta);
void calc_show_inbuf(u8 *fontbase,u8 *buf);
void calc_load_ui(void);
					   
#endif























