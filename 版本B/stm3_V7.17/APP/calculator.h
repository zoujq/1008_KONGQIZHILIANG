#ifndef __CALCULATOR_H
#define __CALCULATOR_H 	
#include "common.h"	   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-��ѧ������ ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

////////////////////////////////////////////////////////////////////////////////////////////
//��ͼ��/ͼƬ·��
extern const u8* CALC_UP_BTN_TBL[29];	//�ɿ�ʱ����ͼ��·��
extern const u8* CALC_DOWN_BTN_TBL[29];	//����ʱ����ͼ��·��
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























