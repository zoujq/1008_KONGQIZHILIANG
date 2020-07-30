#include "3dplay.h"
#include "includes.h" 	   	 
#include "common.h"
#include "3d.h"     
#include "settings.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-3D 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//角度名字表
const u8 *t3dplay_axisname_tbl[3][GUI_LANGUAGE_NUM]=
{
{"X轴角度:     °","X軸角度:     °"," X Axis:     °",},
{"Y轴角度:     °","Y軸角度:     °"," Y Axis:     °",},
{"Z轴角度:     °","Z軸角度:     °"," Z Axis:     °",},	  
};
//標題
const u8 *t3dplay_captions_tbl[GUI_LANGUAGE_NUM]=
{
"3D演示","3D演示","3D SHOW",    	  
};
	   
//画立方体的边框
//t3dx:3D体
//color:线条颜色
void t3dplay_draw_cube_rim(_t3d_obj* t3dx,u16 color)
{
   	gui_draw_line(t3dx->p2dbuf[0].x,t3dx->p2dbuf[0].y,t3dx->p2dbuf[1].x,t3dx->p2dbuf[1].y,color);//画线
  	gui_draw_line(t3dx->p2dbuf[0].x,t3dx->p2dbuf[0].y,t3dx->p2dbuf[3].x,t3dx->p2dbuf[3].y,color);//画线
 	gui_draw_line(t3dx->p2dbuf[0].x,t3dx->p2dbuf[0].y,t3dx->p2dbuf[4].x,t3dx->p2dbuf[4].y,color);//画线
 	gui_draw_line(t3dx->p2dbuf[2].x,t3dx->p2dbuf[2].y,t3dx->p2dbuf[1].x,t3dx->p2dbuf[1].y,color);//画线
 	gui_draw_line(t3dx->p2dbuf[2].x,t3dx->p2dbuf[2].y,t3dx->p2dbuf[3].x,t3dx->p2dbuf[3].y,color);//画线
 	gui_draw_line(t3dx->p2dbuf[2].x,t3dx->p2dbuf[2].y,t3dx->p2dbuf[6].x,t3dx->p2dbuf[6].y,color);//画线
 	gui_draw_line(t3dx->p2dbuf[5].x,t3dx->p2dbuf[5].y,t3dx->p2dbuf[1].x,t3dx->p2dbuf[1].y,color);//画线
 	gui_draw_line(t3dx->p2dbuf[5].x,t3dx->p2dbuf[5].y,t3dx->p2dbuf[6].x,t3dx->p2dbuf[6].y,color);//画线
 	gui_draw_line(t3dx->p2dbuf[5].x,t3dx->p2dbuf[5].y,t3dx->p2dbuf[4].x,t3dx->p2dbuf[4].y,color);//画线
  	gui_draw_line(t3dx->p2dbuf[7].x,t3dx->p2dbuf[7].y,t3dx->p2dbuf[3].x,t3dx->p2dbuf[3].y,color);//画线
 	gui_draw_line(t3dx->p2dbuf[7].x,t3dx->p2dbuf[7].y,t3dx->p2dbuf[6].x,t3dx->p2dbuf[6].y,color);//画线
 	gui_draw_line(t3dx->p2dbuf[7].x,t3dx->p2dbuf[7].y,t3dx->p2dbuf[4].x,t3dx->p2dbuf[4].y,color);//画线
}  
//画一个3d立方体
//t3dx:3D体
//color:线条颜色.
//bkcolor:背景颜色.
void t3dplay_draw_cube(_t3d_obj* t3dx,u16 color,u16 bkcolor)
{	 		 
	t3dplay_draw_cube_rim(t3dx,bkcolor);
	t3d_show_obj(10,80,220,220,t3dx,bkcolor);	//先显示点.
	t3dplay_draw_cube_rim(t3dx,color);
}	  

//系统设置 
u8 t3d_play(void)
{	
   	
	u8 rval=0;
	u8 time=0;
  	_t3d_obj* t3d_obj;				//3D
	short angxbase=0,angybase=0;	//基本角度,全部为0
	short angx,angy,angz;			//由重力传感器测出来的角度
	short x=0,y=0,z=0;
					 
	u16 lastx=0;					//最後一次記錄的x值
	u16 lasty=0;					//最後一次記錄的y值
	u8  keysta=0;					//按鍵状态
   	t3d_obj=t3d_creat(120,180,50,8);//创建一个8个像素点的3D体
	if(t3d_obj)
	{
	  	t3d_draw_point(t3d_obj,0,-25,-25,-25,RED);	//画出正方体的8个基本点
		t3d_draw_point(t3d_obj,1,25,-25,-25,RED);
		t3d_draw_point(t3d_obj,2,25,25,-25,RED);
		t3d_draw_point(t3d_obj,3,-25,25,-25,RED);
		t3d_draw_point(t3d_obj,4,-25,-25,25,RED);
		t3d_draw_point(t3d_obj,5,25,-25,25,RED);
		t3d_draw_point(t3d_obj,6,25,25,25,RED);
		t3d_draw_point(t3d_obj,7,-25,25,25,RED);
		t3d_obj->pixsize=8;
 	}else rval=1;
	LCD_Clear(BLACK);	//清屏
	app_filebrower((u8*)t3dplay_captions_tbl[gui_phy.language],0X05);//显示标题
   	if(rval==0)
	{
 		gui_show_string((u8*)t3dplay_axisname_tbl[0][gui_phy.language],10,32,190,16,16,YELLOW);			//X轴角度
 		gui_show_string((u8*)t3dplay_axisname_tbl[1][gui_phy.language],10,32+16,190,16,16,YELLOW);		//Y轴角度
		gui_show_string((u8*)t3dplay_axisname_tbl[2][gui_phy.language],10,32+32,190,16,16,YELLOW);		//Z轴角度
 		//写入校准值
 	 	ADXL345_WR_Reg(OFSX,systemset.t3dxoff);
		ADXL345_WR_Reg(OFSY,systemset.t3dyoff);
		ADXL345_WR_Reg(OFSZ,systemset.t3dzoff);	
	 	while(1)
		{   						   
			tp_dev.scan(0);    
			if(tp_dev.sta&TP_PRES_DOWN)	//触摸按下了
			{
				if((keysta&0X80)==0&&tp_dev.y<300)//第一次按下
				{
					keysta|=1<<7;		//标记之前是按下的
					lastx=tp_dev.x;		//记录第一次按下的坐标
					lasty=tp_dev.y;		//记录第一次按下的坐标	   
				}else
				{	
 					if((tp_dev.x!=lastx)||(tp_dev.y!=lasty))//有动作
					{
						angx=tp_dev.x;
						angx=angx-lastx;
						lastx=tp_dev.x;
						angy=tp_dev.y;
						angy=angy-lasty;
						lasty=tp_dev.y;			//得到移动的角度值
						angxbase+=angy*5;
 						angybase+=angx*5;
						angxbase=angxbase%3600;	   
						angybase=angybase%3600;	   
 					} 
				}
			}else keysta=0;//松开了,则直接设置滑动无效  
		 	if(system_task_return)break;//TPAD返回
 			ADXL345_RD_XYZ(&angx,&angy,&angz); 
			x+=angx;
			y+=angy;
			z+=angz;
 			time++;
			if(time==10)
			{
				x/=10;//取平均值
				y/=10;
				z/=10;
				angx=ADXL345_Get_Angle(x,y,z,1);    
				angy=ADXL345_Get_Angle(x,y,z,2);   
				angz=ADXL345_Get_Angle(x,y,z,0); 
				t3d_obj->ang.x=-angx+angxbase;
				t3d_obj->ang.y=-angy+angybase;
		 		app_show_float(10+104,32,angx,1,5,16,YELLOW,BLACK);
				app_show_float(10+104,32+16,angy,1,5,16,YELLOW,BLACK);
				app_show_float(10+104,32+32,angz,1,5,16,YELLOW,BLACK);	  
 				t3dplay_draw_cube(t3d_obj,RED,BLACK);
				time=0;
 			}
			delay_ms(10);	   		   
		}	   
	}
	return rval;			   							  
}





























