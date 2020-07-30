#ifndef __3D_H
#define __3D_H	   
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//代码转自MacroXu <kk20y@yahoo.com.cn>,AVR S65 3D Project
//把原来的绝大部分浮点运算改为整形运算,并将原来的sin,cos函数换为查表法,以提高速度
//优化后,速度提升约12倍	
//ALIENTEK战舰STM32开发板
//3D 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/3
//版本：V1.0		       								  
////////////////////////////////////////////////////////////////////////////////// 	   


extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];

//用户和显示屏的视角关系定义	
//这里单位为0.1°  
#define     USRX    0
#define     USRY    0  
#define     USRZ    388   // OFFSETZ*0.707*10,1:1比例显示

//extern u16 	USRX;
//extern u16 	USRY;
//extern u16 	USRZ;
		 
///////////////////////////////////////////////////////////////////////////////////////////////
//描述一个2D像素点  
__packed typedef struct _TYPE_2D_PIXEL{
    u16    x;
    u16    y;
	u16    color;
}type_2d_pixel;
//描述一个3D像素点 
__packed typedef struct _TYPE_3D_PIXEL{
    s16    x;
    s16    y;
    s16    z;
	u16    color;
}type_3d_pixel;

//描述旋转角度 
//单位0.1°
__packed typedef struct _TYPE_ROTATION_ANGLE{
    s16   x;
    s16   y;
	s16   z; 
}type_rot_ang;

//描述移动位置
__packed typedef struct _TYPE_TRANSLATION_DIST{
    s16    x;
	s16    y;
	s16    z;
}type_tran_dis;

//定义一个3D图形管理器
__packed typedef struct 
{
	type_3d_pixel *p3dbuf;		//3d像素点buf
	type_2d_pixel *p2dbuf; 	    //2d像素点buf
	type_rot_ang  ang;			//角度值
	type_tran_dis dis;	 		//位移值
	u16 pixsize;				//当前3d点数
	u16 maxpixsize;				//最大3d像素值
	u16 offx;					//x方向偏移
	u16 offy;					//y方向偏移
	u16 offz;					//z方向偏移
}_t3d_obj;

signed short my_sin(s16 arg);
signed short my_cos(s16 arg);  

_t3d_obj* t3d_creat(u16 offsetx,u16 offsety,u16 offsetz,u16 size);//创建3D图形管理器 
void t3d_delete(_t3d_obj* t3d_del);//删除一个3D管理器
void t3d_pixel_transform(_t3d_obj* t3dx,u16 pixindex);
void t3d_show_char(_t3d_obj *t3dx,short x,short y,short z,u8 num,u8 size,u16 color);
void t3d_show_str(_t3d_obj *t3dx,short x,short y,short z,const u8 *p,u8 size,u16 *ctbl,u8 mode);
void t2d_draw_point(u16 x,u16 y,u16 color);
void t3d_draw_point(_t3d_obj *t3dx,u16 pixindex,s16 x,s16 y,s16 z,u16 color);
 
void t3d_show_obj(u16 x,u16 y,u16 width,u16 height,_t3d_obj *t3dx,u16 bkcolor);
void t3d_clear_obj(u16 x,u16 y,u16 width,u16 height,_t3d_obj *t3dx,u16 bkcolor);

#endif



























