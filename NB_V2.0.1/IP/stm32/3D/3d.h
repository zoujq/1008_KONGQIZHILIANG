#ifndef __3D_H
#define __3D_H	   
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//����ת��MacroXu <kk20y@yahoo.com.cn>,AVR S65 3D Project
//��ԭ���ľ��󲿷ָ��������Ϊ��������,����ԭ����sin,cos������Ϊ���,������ٶ�
//�Ż���,�ٶ�����Լ12��	
//ALIENTEKս��STM32������
//3D ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0		       								  
////////////////////////////////////////////////////////////////////////////////// 	   


extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];

//�û�����ʾ�����ӽǹ�ϵ����	
//���ﵥλΪ0.1��  
#define     USRX    0
#define     USRY    0  
#define     USRZ    388   // OFFSETZ*0.707*10,1:1������ʾ

//extern u16 	USRX;
//extern u16 	USRY;
//extern u16 	USRZ;
		 
///////////////////////////////////////////////////////////////////////////////////////////////
//����һ��2D���ص�  
__packed typedef struct _TYPE_2D_PIXEL{
    u16    x;
    u16    y;
	u16    color;
}type_2d_pixel;
//����һ��3D���ص� 
__packed typedef struct _TYPE_3D_PIXEL{
    s16    x;
    s16    y;
    s16    z;
	u16    color;
}type_3d_pixel;

//������ת�Ƕ� 
//��λ0.1��
__packed typedef struct _TYPE_ROTATION_ANGLE{
    s16   x;
    s16   y;
	s16   z; 
}type_rot_ang;

//�����ƶ�λ��
__packed typedef struct _TYPE_TRANSLATION_DIST{
    s16    x;
	s16    y;
	s16    z;
}type_tran_dis;

//����һ��3Dͼ�ι�����
__packed typedef struct 
{
	type_3d_pixel *p3dbuf;		//3d���ص�buf
	type_2d_pixel *p2dbuf; 	    //2d���ص�buf
	type_rot_ang  ang;			//�Ƕ�ֵ
	type_tran_dis dis;	 		//λ��ֵ
	u16 pixsize;				//��ǰ3d����
	u16 maxpixsize;				//���3d����ֵ
	u16 offx;					//x����ƫ��
	u16 offy;					//y����ƫ��
	u16 offz;					//z����ƫ��
}_t3d_obj;

signed short my_sin(s16 arg);
signed short my_cos(s16 arg);  

_t3d_obj* t3d_creat(u16 offsetx,u16 offsety,u16 offsetz,u16 size);//����3Dͼ�ι����� 
void t3d_delete(_t3d_obj* t3d_del);//ɾ��һ��3D������
void t3d_pixel_transform(_t3d_obj* t3dx,u16 pixindex);
void t3d_show_char(_t3d_obj *t3dx,short x,short y,short z,u8 num,u8 size,u16 color);
void t3d_show_str(_t3d_obj *t3dx,short x,short y,short z,const u8 *p,u8 size,u16 *ctbl,u8 mode);
void t2d_draw_point(u16 x,u16 y,u16 color);
void t3d_draw_point(_t3d_obj *t3dx,u16 pixindex,s16 x,s16 y,s16 z,u16 color);
 
void t3d_show_obj(u16 x,u16 y,u16 width,u16 height,_t3d_obj *t3dx,u16 bkcolor);
void t3d_clear_obj(u16 x,u16 y,u16 width,u16 height,_t3d_obj *t3dx,u16 bkcolor);

#endif



























