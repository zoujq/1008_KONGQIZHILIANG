#include "includes.h"	 
#include "3d.h"
#include "lcd.h"  
#include "sintbl.h" 
#include "string.h"
#include "usart.h"
#include "malloc.h"	 
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

//����3Dͼ�ι����� 
//offsetx:x�����ƫ��
//offsety:y�����ƫ��
//offsetz:z�����ƫ��
//size:3D���ص�ĸ���(ÿ��ռ��14���ֽ�)
_t3d_obj* t3d_creat(u16 offsetx,u16 offsety,u16 offsetz,u16 size) 
{
	u8 res=0;
	_t3d_obj* t3d_crt;	  	
	t3d_crt=(_t3d_obj*)mymalloc(SRAMIN,sizeof(_t3d_obj));//������������ڴ�
	if(t3d_crt==NULL)return NULL;//�ڴ���䲻��.

	t3d_crt->p3dbuf=(type_3d_pixel*)mymalloc(SRAMEX,sizeof(type_3d_pixel)*size);//����3D���ص���ڴ�
	if(t3d_crt->p3dbuf==NULL)res=1;//�ڴ���䲻��.
	t3d_crt->p2dbuf=(type_2d_pixel*)mymalloc(SRAMEX,sizeof(type_2d_pixel)*size);//����2D���ص���ڴ�
	if(t3d_crt->p2dbuf==NULL)res=1;//�ڴ���䲻��.
	t3d_crt->ang.x=0;
	t3d_crt->ang.y=0;
	t3d_crt->ang.z=0;
	t3d_crt->dis.x=0;
	t3d_crt->dis.y=0;
	t3d_crt->dis.z=0;
	t3d_crt->pixsize=0;	  
	t3d_crt->maxpixsize=size;
	//����ϵƫ��
	t3d_crt->offx=offsetx;
	t3d_crt->offy=offsety;
	t3d_crt->offz=offsetz;

	if(res)//����ʧ��
	{
		t3d_delete(t3d_crt);
		return NULL;
	}else return t3d_crt;//�����ɹ�	  
}
//ɾ��һ��3D������
void t3d_delete(_t3d_obj* t3d_del)
{
	myfree(SRAMEX,t3d_del->p2dbuf);
	myfree(SRAMEX,t3d_del->p3dbuf);
	myfree(SRAMIN,t3d_del);
}
/////////////////////////////////////////////�㷨����/////////////////////////////////
//����������ֵ,����ֵ����1024��ȡ��.
//arg:�Ƕ�   (��λ:0.1��)
//����ֵ:����ֵ(����1024��ȡ��)
signed short my_sin(s16 arg)
{
	if(arg<0)
	{	   	
		arg=-arg;
		arg=arg%3600;//��������0~360��
		if(arg<=900)return -sintbl[arg];
		else if(arg<=1800)return -sintbl[1800-arg];
		else if(arg<=2700)return sintbl[arg-1800];
		else return sintbl[3600-arg];
	}else
	{
		arg=arg%3600;//��������0~360��
		if(arg<=900)return sintbl[arg];
		else if(arg<=1800)return sintbl[1800-arg];
		else if(arg<=2700)return -sintbl[arg-1800];
		else return -sintbl[3600-arg];
	}
}
//����������ֵ,����ֵ����1024��ȡ��.
//arg:�Ƕ�   (��λ:0.1��)
//����ֵ:����ֵ(����1024��ȡ��)
signed short my_cos(s16 arg)
{
	if(arg<0)arg=-arg;//ת��Ϊ����
	arg=arg%3600;//��������0~360��
	if(arg<=900)return sintbl[900-arg];
	else if(arg<=1800)return -sintbl[arg-900];
	else if(arg<=2700)return -sintbl[2700-arg];
	else return sintbl[arg-2700];
} 
//�����㷨����
//3dת2D����, pangle-��ת�Ƕȣ�ptrandis-�ƶ�λ��, ppixel��Ҫת��������, ppreturn-ת�����2Dֵ���浽��
//pangle:�Ƕ�.��λΪ����,�����ǡ�.���Ҫ�öȱ�ʾ�Ļ�,��Ҫ���Ԧ�/180
//ptrandis:�ƶ�λ��(��ǰ����Χ�Ʋ����ĵ������,3D���ص�)
//ppixel:Ҫת�������ص�(3D���ص�)
//ppreturn:ת����ɺ�����ص�(2D���ص�)		 
void t3d_pixel_transform(_t3d_obj* t3dx,u16 pixindex)
{		 
	signed long lsinx,lcosx,lsiny,lcosy,lsinz,lcosz;
	signed long ltempx,ltempy,ltempz;
	signed long ldx,ldy,ldz; 	  
    float tdx,tdy;//����Ҫ��2������������,������x/y���ϻ��е�����.	  
	lsinx=my_sin(t3dx->ang.x);
 	lcosx=my_cos(t3dx->ang.x);
	lsiny=my_sin(t3dx->ang.y);
	lcosy=my_cos(t3dx->ang.y);
	lsinz=my_sin(t3dx->ang.z);
 	lcosz=my_cos(t3dx->ang.z); 
    //��ʼ�����ת�� 
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
    ldz = ltempz/10000;//�õ�����100���Ľ��
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
//����ϵ��������,���ϵ���.	    

//��ָ��λ����ʾһ���ַ�
//�����������3D����
//t3dx:3d����
//x,y,z:3d����ϵ����.
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16	
//color:������ɫ			  
void t3d_show_char(_t3d_obj *t3dx,short x,short y,short z,u8 num,u8 size,u16 color)
{  		    
    u8 temp;
    u8 t,t1; 	   					      				  
	short y0=y;
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ  
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=asc2_1206[num][t];  //����1206����
		else temp=asc2_1608[num][t];		 //����1608���� 	                          
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
//��ָ��λ����ʾһ���ַ���
//�����������3D����
//t3dx:3d����
//x,y,z:3d����ϵ����.
//p:�ַ���ָ��
//size:�����С 12/16	
//ctbl:������ɫ��,ÿ���ַ�һ����ɫ
//mode:ģʽ.1,����ɫ��.0,������ɫ��,���еĶ���һ����ɫ.
void t3d_show_str(_t3d_obj *t3dx,short x,short y,short z,const u8 *p,u8 size,u16 *ctbl,u8 mode)
{         
    while(*p!='\0')
    {       
        if(x>239)break;		
		t3d_show_char(t3dx,x,y,z,*p,size,*ctbl);							 
        if(mode)ctbl++;//����ɫ��	  
		x+=size/2;
        p++;
    }  
}   

//��2ά����ϵ����
//�����������LCD����,����3D����
//x,y:����
//color:�����ɫ
void t2d_draw_point(u16 x,u16 y,u16 color)
{
	POINT_COLOR=color;
	LCD_DrawPoint(x,y); 
} 
//��3d�ռ仭һ����
//�����������3D����
//t3dx:3d����
//pixindex:���ر��
//x,y,z:��ά�ռ������
//color:�����ɫ
void t3d_draw_point(_t3d_obj *t3dx,u16 pixindex,s16 x,s16 y,s16 z,u16 color)
{	  
 	t3dx->p3dbuf[pixindex].x=x;
	t3dx->p3dbuf[pixindex].y=y; 
	t3dx->p3dbuf[pixindex].z=z;
	t3dx->p3dbuf[pixindex].color=color; 

 	t3d_pixel_transform(t3dx,pixindex);//����ת��
 	t2d_draw_point(t3dx->p2dbuf[pixindex].x,t3dx->p2dbuf[pixindex].y,t3dx->p2dbuf[pixindex].color);
}
//��ָ��������ʾһ��3D����.
//�����������LCD����,����3D����
//x,y,width,height:��ʾ�����趨
//t3dx:3d����
//bkcolor:����ɫ.
//��ʾʱ,�������һ�ε���ʾ,Ȼ������ʾ�µ�
void t3d_show_obj(u16 x,u16 y,u16 width,u16 height,_t3d_obj *t3dx,u16 bkcolor)
{
	u16 i;
	width+=x;
	height+=y;
	for(i=0;i<t3dx->pixsize;i++)
	{																								 
		if(t3dx->p2dbuf[i].x<width&&t3dx->p2dbuf[i].x>=x&&t3dx->p2dbuf[i].y<height&&t3dx->p2dbuf[i].y>=y)//���趨������.
		t2d_draw_point(t3dx->p2dbuf[i].x, t3dx->p2dbuf[i].y,bkcolor);					//�����һ�ε�ͼ������
	    t3d_pixel_transform(t3dx,i);	//����ת��		  
		if(t3dx->p2dbuf[i].x<width&&t3dx->p2dbuf[i].x>=x&&t3dx->p2dbuf[i].y<height&&t3dx->p2dbuf[i].y>=y)//���趨������.
		t2d_draw_point(t3dx->p2dbuf[i].x,t3dx->p2dbuf[i].y,t3dx->p2dbuf[i].color);		//���µ�ͼ������
	}
}  
//�����һ�ε�ͼ��
//�����������LCD����,����3D����
//x,y,width,height:��ʾ�����趨
//t3dx:3d����
//bkcolor:����ɫ.
void t3d_clear_obj(u16 x,u16 y,u16 width,u16 height,_t3d_obj *t3dx,u16 bkcolor)
{
	u16 i;
	width+=x;
	height+=y;
	for(i=0;i<t3dx->pixsize;i++)
	{
		if(t3dx->p2dbuf[i].x<width&&t3dx->p2dbuf[i].x>=x&&t3dx->p2dbuf[i].y<height&&t3dx->p2dbuf[i].y>=y)//���趨������.
		t2d_draw_point(t3dx->p2dbuf[i].x, t3dx->p2dbuf[i].y,bkcolor);					//�����һ�ε�ͼ������
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
































