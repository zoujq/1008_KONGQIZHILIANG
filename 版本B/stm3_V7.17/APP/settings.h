#ifndef __SETTINGS_H
#define __SETTINGS_H
#include "sys.h"
#include "includes.h" 	   	 
#include "common.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//list�ṹ��.����ṹ
__packed typedef struct 
{
	u8 syslanguage;			//Ĭ��ϵͳ����
	u8 lcdbklight;			//LED�������� 10~110.10�����;110��������
	u8 picmode;				//ͼƬ���ģʽ:0,˳��ѭ������;1,�������.
	u8 mp3mode;				//MP3����ģʽ:0,˳��ѭ������;1,�������;2,����ѭ������.
	u8 fmtxmode;	   		//FM����ģʽ:0,�ر�;1,����;
	u8 fmrxmode;	   		//FM����ģʽ:0,�رպ�̨����;1,������̨����;
	u16 fmtxfreq;			//FM����Ƶ��:��λ0.1Mhz
	u8 audiosel;			//Ĭ�ϵ���Ƶѡ������,������䶯����.
 	u8 t3dxoff;				//3D��������3��У׼ֵ
	u8 t3dyoff;
	u8 t3dzoff;

	u8 saveflag;			//�����־,0X0A,�������;����,����δ����	   
}_system_setings;

extern _system_setings systemset;	//��settings.c�������� 

u8 sysset_time_set(u16 x,u16 y,u8 *hour,u8 *min,u8*caption);
u8 sysset_date_set(u16 x,u16 y,u16 *year,u8 *month,u8 *date,u8*caption);
u8 sysset_fmtx_freq_set(u16 x,u16 y,u16 *freq,u8*caption);
void sysset_mp3_effict_show(u16 x,u16 y,u8 mask,u8 vsval);
u8 sysset_mp3_effict_set(u16 x,u16 y,_vs10xx_obj *vsset,u8* caption);
u8 sysset_bklight_set(u16 x,u16 y,u8* caption,u16 *bkval);
u8 sysset_system_update_cpymsg(u8*pname,u8 pct,u8 mode);
void sysset_system_update(u8*caption);
void sysset_system_info(u16 x,u16 y,u8*caption);
void sysset_system_status(u16 x,u16 y,u8* caption);
void sysset_system_about(u16 x,u16 y,u8*caption);
u8 * set_search_caption(const u8 *mcaption);	 
void sysset_read_para(_system_setings * sysset);
void sysset_save_para(_system_setings * sysset);

u8 sysset_play(void);
										   
#endif












