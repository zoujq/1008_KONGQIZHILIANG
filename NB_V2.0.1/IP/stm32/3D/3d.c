#include "includes.h"	 
#include "3d.h"
#include "lcd.h"  
#include "sintbl.h" 
#include "string.h"
#include "usart.h"
#include "malloc.h"	 
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

//创建3D图形管理器 
//offsetx:x轴向的偏移
//offsety:y轴向的偏移
//offsetz:z轴向的偏移
//size:3D像素点的个数(每个占用14个字节)
_t3d_obj* t3d_creat(u16 offsetx,u16 offsety,u16 offsetz,u16 size) 
{
	u8 res=0;
	_t3d_obj* t3d_crt;	  	
	t3d_crt=(_t3d_obj*)mymalloc(SRAMIN,sizeof(_t3d_obj));//分配管理器的内存
	if(t3d_crt==NULL)return NULL;//内存分配不够.

	t3d_crt->p3dbuf=(type_3d_pixel*)mymalloc(SRAMEX,sizeof(type_3d_pixel)*size);//分配3D像素点的内存
	if(t3d_crt->p3dbuf==NULL)res=1;//内存分配不够.
	t3d_crt->p2dbuf=(type_2d_pixel*)mymalloc(SRAMEX,sizeof(type_2d_pixel)*size);//分配2D像素点的内存
	if(t3d_crt->p2dbuf==NULL)res=1;//内存分配不够.
	t3d_crt->ang.x=0;
	t3d_crt->ang.y=0;
	t3d_crt->ang.z=0;
	t3d_crt->dis.x=0;
	t3d_crt->dis.y=0;
	t3d_crt->dis.z=0;
	t3d_crt->pixsize=0;	  
	t3d_crt->maxpixsize=size;
	//坐标系偏移
	t3d_crt->offx=offsetx;
	t3d_crt->offy=offsety;
	t3d_crt->offz=offsetz;

	if(res)//创建失败
	{
		t3d_delete(t3d_crt);
		return NULL;
	}else return t3d_crt;//创建成功	  
}
//删除一个3D管理器
void t3d_delete(_t3d_obj* t3d_del)
{
	myfree(SRAMEX,t3d_del->p2dbuf);
	myfree(SRAMEX,t3d_del->p3dbuf);
	myfree(SRAMIN,t3d_del);
}
/////////////////////////////////////////////算法部分/////////////////////////////////
//查表发获得正弦值,正弦值扩大1024倍取整.
//arg:角度   (单位:0.1度)
//返回值:正弦值(扩大1024倍取整)
signed short my_sin(s16 arg)
{
	if(arg<0)
	{	   	
		arg=-arg;
		arg=arg%3600;//周期总是0~360°
		if(arg<=900)return -sintbl[arg];
		else if(arg<=1800)return -sintbl[1800-arg];
		else if(arg<=2700)return sintbl[arg-1800];
		else return sintbl[3600-arg];
	}else
	{
		arg=arg%3600;//周期总是0~360°
		if(arg<=900)return sintbl[arg];
		else if(arg<=1800)return sintbl[1800-arg];
		else if(arg<=2700)return -sintbl[arg-1800];
		else return -sintbl[3600-arg];
	}
}
//查表发获得余弦值,余弦值扩大1024倍取整.
//arg:角度   (单位:0.1度)
//返回值:余弦值(扩大1024倍取整)
signed short my_cos(s16 arg)
{
	if(arg<0)arg=-arg;//转换为正数
	arg=arg%3600;//周期总是0~360°
	if(arg<=900)return sintbl[900-arg];
	else if(arg<=1800)return -sintbl[arg-900];
	else if(arg<=2700)return -sintbl[2700-arg];
	else return sintbl[arg-2700];
} 
//核心算法部分
//3d转2D函数, pangle-旋转角度，ptrandis-移动位置, ppixel－要转换的像素, ppreturn-转换后的2D值保存到这
//pangle:角度.单位为弧度,而不是°.如果要用度表示的话,需要乘以π/180
//ptrandis:移动位置(当前点所围绕操作的点的坐标,3D像素点)
//ppixel:要转换的像素点(3D像素点)
//ppreturn:转换完成后的像素点(2D像素点)		 
void t3d_pixel_transform(_t3d_obj* t3dx,u16 pixindex)
{		 
	signed long lsinx,lcosx,lsiny,lcosy,lsinz,lcosz;
	signed long ltempx,ltempy,ltempz;
	signed long ldx,ldy,ldz; 	  
    float tdx,tdy;//必须要的2个个浮点数据,否则在x/y轴上会有点问题.	  
	lsinx=my_sin(t3dx->ang.x);
 	lcosx=my_cos(t3dx->ang.x);
	lsiny=my_sin(t3dx->ang.y);
	lcosy=my_cos(t3dx->ang.y);
	lsinz=my_sin(t3dx->ang.z);
 	lcosz=my_cos(t3dx->ang.z); 
    //开始计算和转换 
    ldx = t3dx->p3dbuf[pixindex].x;
	ldy = t3dx->p3dbuf[pixindex].y;
	ldz = t3dx->p3dbuf[pixindex].z;
    ltempy = ldy*lcosx - ldz*lsinx;
    ltempz = ldy*lsinx + ldz*lcosx;	
    ldy = ltempy;
    ldz = ltempz;
    ltempx = ldx*lcosy*1000 - ldz*lsiny;	
    ltempz = ldx*lsiny*1000 + ldz*lcosy;	
    ldx = ltempx/1000;			  
    ldz = ltempz/10000;//得到扩大100倍的结果
    ltempx = ldx * lcosz - ldy * lsinz;	
    ltempy = ldx * lsinz + ldy * lcosz;	
    ldx = ltempx/100000;
    ldy = ltempy/100000;	  
    tdx = ldx + t3dx->dis.x*10;
	tdy = ldy + t3dx->dis.y*10;
	ldz = ldz - t3dx->dis.z*100 + t3dx->offz*100;	   
 	t3dx->p2dbuf[pixindex].x = (((tdx-USRX)*USRZ)/ldz) + t3dx->offx;
	t3dx->p2dbuf[pixindex].y = (((tdy-USRY)*USRZ)/ldz) + t3dx->offy;    
	t3dx->p2dbuf[pixindex].color=t3dx->p3dbuf[pixindex].color;    
  
}  
/////////////////////////////////////////////////////////////////////////////////////
//坐标系从做到右,从上到下.	    

//在指定位置显示一个字符
//这里的坐标是3D坐标
//t3dx:3d物体
//x,y,z:3d坐标系坐标.
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16	
//color:文字颜色			  
void t3d_show_char(_t3d_obj *t3dx,short x,short y,short z,u8 num,u8 size,u16 color)
{  		    
    u8 temp;
    u8 t,t1; 	   					      				  
	short y0=y;
	//设置窗口		   
	num=num-' ';//得到偏移后的值  
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=asc2_1206[num][t];  //调用1206字体
		else temp=asc2_1608[num][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{			    
	        if(temp&0x80)
			{	   
				t3dx->p3dbuf[t3dx->pixsize].x=x;
				t3dx->p3dbuf[t3dx->pixsize].y=y; 
				t3dx->p3dbuf[t3dx->pixsize].z=z;	  
				t3dx->p3dbuf[t3dx->pixsize].color=color;
				t3dx->pixsize++;    
			}
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 					     	   	 	  
}
//在指定位置显示一个字符串
//这里的坐标是3D坐标
//t3dx:3d物体
//x,y,z:3d坐标系坐标.
//p:字符串指针
//size:字体大小 12/16	
//ctbl:文字颜色表,每个字符一个颜色
//mode:模式.1,用颜色表.0,不用颜色表,所有的都是一个颜色.
void t3d_show_str(_t3d_obj *t3dx,short x,short y,short z,const u8 *p,u8 size,u16 *ctbl,u8 mode)
{         
    while(*p!='\0')
    {       
        if(x>239)break;		
		t3d_show_char(t3dx,x,y,z,*p,size,*ctbl);							 
        if(mode)ctbl++;//用颜色表	  
		x+=size/2;
        p++;
    }  
}   

//在2维坐标系画点
//这里的坐标是LCD坐标,不是3D坐标
//x,y:坐标
//color:点的颜色
void t2d_draw_point(u16 x,u16 y,u16 color)
{
	POINT_COLOR=color;
	LCD_DrawPoint(x,y); 
} 
//在3d空间画一个点
//这里的坐标是3D坐标
//t3dx:3d物体
//pixindex:像素编号
//x,y,z:三维空间的坐标
//color:点的颜色
void t3d_draw_point(_t3d_obj *t3dx,u16 pixindex,s16 x,s16 y,s16 z,u16 color)
{	  
 	t3dx->p3dbuf[pixindex].x=x;
	t3dx->p3dbuf[pixindex].y=y; 
	t3dx->p3dbuf[pixindex].z=z;
	t3dx->p3dbuf[pixindex].color=color; 

 	t3d_pixel_transform(t3dx,pixindex);//坐标转换
 	t2d_draw_point(t3dx->p2dbuf[pixindex].x,t3dx->p2dbuf[pixindex].y,t3dx->p2dbuf[pixindex].color);
}
//在指定区域显示一个3D物体.
//这里的坐标是LCD坐标,不是3D坐标
//x,y,width,height:显示区域设定
//t3dx:3d物体
//bkcolor:背景色.
//显示时,先清除上一次的显示,然后再显示新的
void t3d_show_obj(u16 x,u16 y,u16 width,u16 height,_t3d_obj *t3dx,u16 bkcolor)
{
	u16 i;
	width+=x;
	height+=y;
	for(i=0;i<t3dx->pixsize;i++)
	{																								 
		if(t3dx->p2dbuf[i].x<width&&t3dx->p2dbuf[i].x>=x&&t3dx->p2dbuf[i].y<height&&t3dx->p2dbuf[i].y>=y)//在设定区域内.
		t2d_draw_point(t3dx->p2dbuf[i].x, t3dx->p2dbuf[i].y,bkcolor);					//清除上一次的图形像素
	    t3d_pixel_transform(t3dx,i);	//坐标转换		  
		if(t3dx->p2dbuf[i].x<width&&t3dx->p2dbuf[i].x>=x&&t3dx->p2dbuf[i].y<height&&t3dx->p2dbuf[i].y>=y)//在设定区域内.
		t2d_draw_point(t3dx->p2dbuf[i].x,t3dx->p2dbuf[i].y,t3dx->p2dbuf[i].color);		//画新的图形像素
	}
}  
//清除上一次的图像
//这里的坐标是LCD坐标,不是3D坐标
//x,y,width,height:显示区域设定
//t3dx:3d物体
//bkcolor:背景色.
void t3d_clear_obj(u16 x,u16 y,u16 width,u16 height,_t3d_obj *t3dx,u16 bkcolor)
{
	u16 i;
	width+=x;
	height+=y;
	for(i=0;i<t3dx->pixsize;i++)
	{
		if(t3dx->p2dbuf[i].x<width&&t3dx->p2dbuf[i].x>=x&&t3dx->p2dbuf[i].y<height&&t3dx->p2dbuf[i].y>=y)//在设定区域内.
		t2d_draw_point(t3dx->p2dbuf[i].x, t3dx->p2dbuf[i].y,bkcolor);					//清除上一次的图形像素
	}
}
/////////////////////////////////////////////////////////////////////
   

#include "led.h"
#include "key.h"

u16 frame=0;

//void test_3d(u16 x,u16 y,u8 *str)
//{
//	u16 i;
//	_t3d_obj *t3dx;
////	u8 pause=0;
//	u8 key;
//	short slen=strlen((const char*)str);
//	LCD_Clear(0);
// 	slen*=-4;
//	
//	t3d_show_str(t3dx,slen+x,y,0,str,16,RED);
//
//	while(1)		
//	{			   	 
//		 frame++;
// 		 //t3dx->ang.x+=1;
// 		 //t3dx->ang.z+=1;
// 		 //t3dx->ang.y+=1;				   
//		 while(0)
//		 {				 
//		 	key=KEY_Scan(1);
//		 	if(key==1)
//			{
//				t3dx->ang.x+=10;
//				printf("x:%d\r\n",(short)t3dx->ang.x);
//				break;
//			}		    
//		 	if(key==2)
//			{
//				t3dx->ang.y+=10;
//				printf("y:%d\r\n",(short)t3dx->ang.y);
//				break;
//			}
//		 	if(key==3)
//			{
//				t3dx->ang.z+=10;
//				printf("z:%d\r\n",(short)t3dx->ang.z);
//				break;
//			}
//		 }
//
//		
//	}
//} 
































