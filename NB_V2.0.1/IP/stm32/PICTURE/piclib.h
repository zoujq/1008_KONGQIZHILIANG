#ifndef __PICLIB_H
#define __PICLIB_H	  		  
#include "sys.h" 
#include "lcd.h"
#include "malloc.h"
#include "ff.h"
#include "exfuns.h"
#include "bmp.h"
#include "jpeg.h"
#include "gif.h"
 //////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//图片解码 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/19
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


#define PIC_FORMAT_ERR		0x27	//格式错误
#define PIC_SIZE_ERR		0x28	//图片尺寸错误
#define PIC_WINDOW_ERR		0x29	//窗口设定错误
#define PIC_MEM_ERR			0x11	//内存错误
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//图片显示物理层接口  
//在移植的时候,必须由用户自己实现这几个函数
typedef struct 
{
	u16(*read_point)(u16,u16);			//u16 read_point(u16 x,u16 y)						读点函数
	void(*draw_point)(u16,u16,u16);		//void draw_point(u16 x,u16 y,u16 color)		    画点函数
 	void(*fill)(u16,u16,u16,u16,u16);	///void fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color) 单色填充函数 	 
 	void(*draw_hline)(u16,u16,u16,u16);	//void draw_hline(u16 x0,u16 y0,u16 len,u16 color)  画水平线函数	 
}_pic_phy; 
extern _pic_phy pic_phy;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//图像信息
typedef struct
{			 
	u32 ImgWidth; 	//图像的实际宽度和高度
	u32 ImgHeight;

	u32 Div_Fac;  	//缩放系数 (扩大了8192倍的)
	
	u32 S_Height; 	//设定的高度和宽度
	u32 S_Width;
	
	u32	S_XOFF;	  	//x轴和y轴的偏移量
	u32 S_YOFF;

	u32 staticx; 	//当前显示到的ｘｙ坐标
	u32 staticy;																 	
}_pic_info;
extern _pic_info picinfo;//图像信息
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void piclib_init(void);													//初始化画图
u16 piclib_alpha_blend(u16 src,u16 dst,u8 alpha);						//alphablend处理
void ai_draw_init(void);												//初始化智能画图
u8 is_element_ok(u16 x,u16 y,u8 chg);									//判断像素是否有效
u8 ai_load_picfile(const u8 *filename,u16 x,u16 y,u16 width,u16 height);//智能画图
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif






























