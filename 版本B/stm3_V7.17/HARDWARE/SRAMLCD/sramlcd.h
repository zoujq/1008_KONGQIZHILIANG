#ifndef __SRAMLCD_H
#define	__SRAMLCD_H	   
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SRAM-LCD ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   


////////////////////////////////////////////////////////////////
 extern u16 LCD_GRAM_BUF[320][480];
////////////////////////////////////////////////////////////////

//SRAMLCD ������
typedef struct _m_sramlcd_dev
{															    
 	void (*show)(u16); 				//��ʾ֡ 
 	void (*draw_point)(u16,u16,u16);//����		
	void (*clear)(u16);				//��֡ 
	void (*move)(u8,u8,u16);		//�ƶ�   
	u8 pos;                        	//��ǰ֡��λ��(x����)
	u8 frame;				 		//��ǰ֡(0/1)
}m_sramlcd_dev;
extern m_sramlcd_dev SLCD;  

u16  slcd_read_point(u16 x,u16 y);
void slcd_draw_point(u16 x,u16 y,u16 color);
void slcd_frame_show(u16 x);			 
void slcd_frame_clear(u16 color);
void slcd_frame_move(u8 dir,u8 skips,u16 pos);
#endif






























