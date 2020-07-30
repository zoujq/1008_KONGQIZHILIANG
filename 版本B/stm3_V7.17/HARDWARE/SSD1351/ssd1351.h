#ifndef __SSD1351_H__
#define __SSD1351_H__

#include "sys.h"


 struct ascii{
 unsigned char index;
 unsigned char msk[64];
};
//extern struct ascii ascii_3030[];
//extern struct ascii ascii_2020[];


//初始化SSD1351
//void SSD1351_Init(void);


//void Initial_ssd1351(void);

//OLED初始化
void OLED_Init(void);
//OLED显示打开
void OLED_On(void);
	
//OLED显示关闭
void OLED_Off(void);
	
//清屏函数
//void OLED_Clear(u16 color)    ;
void OLCD_Clear(u16 colora);//清屏





//画一条粗线(方法1)
//x0,y0:起点 x1,y1:终点
//size:线粗细,仅支持:0~2.
//color:颜色
void gui_draw_bline1(u16 x0,u16 y0,u16 x1,u16 y1,u8 size,u16 color);


//画三角形，开始按键
//x0,y0:坐标
//r半径
//color:颜色
void gui_fill_keyStart(u16 x0,u16 y0,u16 r,u16 color,u16 back_color);
//画三角形，暂停按键
//x0,y0:坐标
//r半径
//color:颜色
void gui_fill_keyPause(u16 x0,u16 y0,u16 r,u16 color,u16 back_color);
	
//画三角形，左进按键
//x0,y0:坐标
//r半径
//color:颜色
void gui_fill_keyLeft(u16 x0,u16 y0,u16 r,u16 color,u16 back_color);
	//画三角形，右进按键
//x0,y0:坐标
//r半径
//color:颜色
void gui_fill_keyRight(u16 x0,u16 y0,u16 r,u16 color,u16 back_color)	;


//填充图标颜色
//x:
//y:
//iconame:图标
void OLED_DrawICO_Full(u8 x,u8 y,u16 color);
	
//画图标
//x:
//y:
//iconame:图标
void OLED_DrawICO(u8 x,u8 y,const unsigned char bmp[]);


//画图标,缩小一半的图标
//x:
//y:
//iconame:图标
void OLED_DrawICO_Small(u8 x,u8 y,const unsigned char bmp[]);

//画图标,大小30*30
//x:
//y:
//iconame:图标
void OLED_DrawICO_3030(u8 x,u8 y,const unsigned char bmp[]);
	



//画图标,选中，换颜色
//x:
//y:
//iconame:图标
///void OLED_DrawICO_Choose(u8 x,u8 y,const unsigned char bmp[]);
//在指定位置显示一个字符,包括部分字符
//x:0~(127-15)
//y:0~(95-30)
//ch:字符			 
//color_front:字体颜色
//color_back:背景颜色
void OLED_ShowChar(u8 x,u8 y,unsigned char ch,u16 color_front, u16 color_back);



//在指定位置显示一个字符,包括部分字符
//x:0~(127-15)
//y:0~(95-30)
//ch:字符			 
//color_front:字体颜色
//color_back:背景颜色
void OLED_ShowChar2020(u8 x,u8 y,unsigned char ch,u16 color_front, u16 color_back);








//画圆形指针表盘
//x,y:坐标中心点
//size:表盘大小(直径)
//d:表盘分割,秒钟的高度
void calendar_circle_clock_drawpanel(u16 x,u16 y,u16 size,u16 d);
//显示时间
//x,y:坐标中心点
//size:表盘大小(直径)
//d:表盘分割,秒钟的高度
//hour:时钟
//min:分钟
//sec:秒钟
void calendar_circle_clock_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec);	






#endif
