#ifndef __NESPLAY_H
#define __NESPLAY_H 	
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-NESģ���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

extern u8 nesruning ;	//�˳�NES�ı�־
extern u8 frame_cnt;	//ͳ��֡��
 
void nes_clock_set(u8 PLL);		   
void load_nes(u8* path);   
u8 nes_play(void);
#endif























