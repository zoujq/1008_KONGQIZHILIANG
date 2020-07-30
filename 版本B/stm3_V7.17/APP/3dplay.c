#include "3dplay.h"
#include "includes.h" 	   	 
#include "common.h"
#include "3d.h"     
#include "settings.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-3D ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//�Ƕ����ֱ�
const u8 *t3dplay_axisname_tbl[3][GUI_LANGUAGE_NUM]=
{
{"X��Ƕ�:     ��","X�S�Ƕ�:     ��"," X Axis:     ��",},
{"Y��Ƕ�:     ��","Y�S�Ƕ�:     ��"," Y Axis:     ��",},
{"Z��Ƕ�:     ��","Z�S�Ƕ�:     ��"," Z Axis:     ��",},	  
};
//���}
const u8 *t3dplay_captions_tbl[GUI_LANGUAGE_NUM]=
{
"3D��ʾ","3D��ʾ","3D SHOW",    	  
};
	   
//��������ı߿�
//t3dx:3D��
//color:������ɫ
void t3dplay_draw_cube_rim(_t3d_obj* t3dx,u16 color)
{
   	gui_draw_line(t3dx->p2dbuf[0].x,t3dx->p2dbuf[0].y,t3dx->p2dbuf[1].x,t3dx->p2dbuf[1].y,color);//����
  	gui_draw_line(t3dx->p2dbuf[0].x,t3dx->p2dbuf[0].y,t3dx->p2dbuf[3].x,t3dx->p2dbuf[3].y,color);//����
 	gui_draw_line(t3dx->p2dbuf[0].x,t3dx->p2dbuf[0].y,t3dx->p2dbuf[4].x,t3dx->p2dbuf[4].y,color);//����
 	gui_draw_line(t3dx->p2dbuf[2].x,t3dx->p2dbuf[2].y,t3dx->p2dbuf[1].x,t3dx->p2dbuf[1].y,color);//����
 	gui_draw_line(t3dx->p2dbuf[2].x,t3dx->p2dbuf[2].y,t3dx->p2dbuf[3].x,t3dx->p2dbuf[3].y,color);//����
 	gui_draw_line(t3dx->p2dbuf[2].x,t3dx->p2dbuf[2].y,t3dx->p2dbuf[6].x,t3dx->p2dbuf[6].y,color);//����
 	gui_draw_line(t3dx->p2dbuf[5].x,t3dx->p2dbuf[5].y,t3dx->p2dbuf[1].x,t3dx->p2dbuf[1].y,color);//����
 	gui_draw_line(t3dx->p2dbuf[5].x,t3dx->p2dbuf[5].y,t3dx->p2dbuf[6].x,t3dx->p2dbuf[6].y,color);//����
 	gui_draw_line(t3dx->p2dbuf[5].x,t3dx->p2dbuf[5].y,t3dx->p2dbuf[4].x,t3dx->p2dbuf[4].y,color);//����
  	gui_draw_line(t3dx->p2dbuf[7].x,t3dx->p2dbuf[7].y,t3dx->p2dbuf[3].x,t3dx->p2dbuf[3].y,color);//����
 	gui_draw_line(t3dx->p2dbuf[7].x,t3dx->p2dbuf[7].y,t3dx->p2dbuf[6].x,t3dx->p2dbuf[6].y,color);//����
 	gui_draw_line(t3dx->p2dbuf[7].x,t3dx->p2dbuf[7].y,t3dx->p2dbuf[4].x,t3dx->p2dbuf[4].y,color);//����
}  
//��һ��3d������
//t3dx:3D��
//color:������ɫ.
//bkcolor:������ɫ.
void t3dplay_draw_cube(_t3d_obj* t3dx,u16 color,u16 bkcolor)
{	 		 
	t3dplay_draw_cube_rim(t3dx,bkcolor);
	t3d_show_obj(10,80,220,220,t3dx,bkcolor);	//����ʾ��.
	t3dplay_draw_cube_rim(t3dx,color);
}	  

//ϵͳ���� 
u8 t3d_play(void)
{	
   	
	u8 rval=0;
	u8 time=0;
  	_t3d_obj* t3d_obj;				//3D
	short angxbase=0,angybase=0;	//�����Ƕ�,ȫ��Ϊ0
	short angx,angy,angz;			//������������������ĽǶ�
	short x=0,y=0,z=0;
					 
	u16 lastx=0;					//����һ��ӛ䛵�xֵ
	u16 lasty=0;					//����һ��ӛ䛵�yֵ
	u8  keysta=0;					//���I״̬
   	t3d_obj=t3d_creat(120,180,50,8);//����һ��8�����ص��3D��
	if(t3d_obj)
	{
	  	t3d_draw_point(t3d_obj,0,-25,-25,-25,RED);	//�����������8��������
		t3d_draw_point(t3d_obj,1,25,-25,-25,RED);
		t3d_draw_point(t3d_obj,2,25,25,-25,RED);
		t3d_draw_point(t3d_obj,3,-25,25,-25,RED);
		t3d_draw_point(t3d_obj,4,-25,-25,25,RED);
		t3d_draw_point(t3d_obj,5,25,-25,25,RED);
		t3d_draw_point(t3d_obj,6,25,25,25,RED);
		t3d_draw_point(t3d_obj,7,-25,25,25,RED);
		t3d_obj->pixsize=8;
 	}else rval=1;
	LCD_Clear(BLACK);	//����
	app_filebrower((u8*)t3dplay_captions_tbl[gui_phy.language],0X05);//��ʾ����
   	if(rval==0)
	{
 		gui_show_string((u8*)t3dplay_axisname_tbl[0][gui_phy.language],10,32,190,16,16,YELLOW);			//X��Ƕ�
 		gui_show_string((u8*)t3dplay_axisname_tbl[1][gui_phy.language],10,32+16,190,16,16,YELLOW);		//Y��Ƕ�
		gui_show_string((u8*)t3dplay_axisname_tbl[2][gui_phy.language],10,32+32,190,16,16,YELLOW);		//Z��Ƕ�
 		//д��У׼ֵ
 	 	ADXL345_WR_Reg(OFSX,systemset.t3dxoff);
		ADXL345_WR_Reg(OFSY,systemset.t3dyoff);
		ADXL345_WR_Reg(OFSZ,systemset.t3dzoff);	
	 	while(1)
		{   						   
			tp_dev.scan(0);    
			if(tp_dev.sta&TP_PRES_DOWN)	//����������
			{
				if((keysta&0X80)==0&&tp_dev.y<300)//��һ�ΰ���
				{
					keysta|=1<<7;		//���֮ǰ�ǰ��µ�
					lastx=tp_dev.x;		//��¼��һ�ΰ��µ�����
					lasty=tp_dev.y;		//��¼��һ�ΰ��µ�����	   
				}else
				{	
 					if((tp_dev.x!=lastx)||(tp_dev.y!=lasty))//�ж���
					{
						angx=tp_dev.x;
						angx=angx-lastx;
						lastx=tp_dev.x;
						angy=tp_dev.y;
						angy=angy-lasty;
						lasty=tp_dev.y;			//�õ��ƶ��ĽǶ�ֵ
						angxbase+=angy*5;
 						angybase+=angx*5;
						angxbase=angxbase%3600;	   
						angybase=angybase%3600;	   
 					} 
				}
			}else keysta=0;//�ɿ���,��ֱ�����û�����Ч  
		 	if(system_task_return)break;//TPAD����
 			ADXL345_RD_XYZ(&angx,&angy,&angz); 
			x+=angx;
			y+=angy;
			z+=angz;
 			time++;
			if(time==10)
			{
				x/=10;//ȡƽ��ֵ
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





























