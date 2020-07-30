#ifndef __MP3PLAYER_H
#define __MP3PLAYER_H
#include "sys.h"
#include "includes.h" 	   	 
#include "common.h"
#include "ff.h"	
#include "gui.h"
#include "lyric.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-���ֲ�����ʵ�� ����	   
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
extern const u8 *FLAC_PATCH_PATH;			//FLAC���ŵ�PATCH�Ĵ��λ��
extern const u8 *SPEC_PATCH_PATH;			//Ƶ�׷�����PATCH�Ĵ��λ��
extern const u8* MP3_DEMO_PIC;				//demoͼƬ·��
extern const u8* MP3_BTN_PIC_TBL[2][5];  	//5��ͼƬ��ť��·��
extern const u8* MP3_PLAYR_PIC;				//���� �ɿ�
extern const u8* MP3_PLAYP_PIC;				//���� ����
extern const u8* MP3_PAUSER_PIC;			//��ͣ �ɿ�
extern const u8* MP3_PAUSEP_PIC;			//��ͣ ����
////////////////////////////////////////////////////////////////////////////////////////////
#define MP3_TITLE_COLOR   	0XFFFF		//������������ɫ	
#define MP3_TITLE_BKCOLOR   0X0000		//���������ⱳ��ɫ	

#define MP3_INFO_COLOR   	0X8410		//��Ϣ�������ɫ	
#define MP3_MAIN_BKCOLOR    0X18E3		//������ɫ	
/////////////////////////////////////////////////////
#define FFT_TOP_COLOR		0XF800		//���Ӷ�������ɫ
#define FFT_BANDS_COLOR		0X07FF		//���ӵ���ɫ


/////////////////////////////////////////////
#define MP3_LRC_MCOLOR       0XF810//0XFFE0		//��ǰ�����ɫΪ��ɫ
#define MP3_LRC_SCOLOR       0X07FF//0X07FF		//ǰһ��ͺ�һ������ɫΪ��ɫ			    

//FFT
#define FFT_BANDS			14			//14��Ƶ��

//MP3���ƹ�����
__packed typedef struct 
{			 						 
	vu8 sta; 			//������״̬
						//bit7:0,����;1,MP3������(������ͣ״̬) 
						//bit6:0,û���и�;1,�Ѿ��и���.
						//bit5:0,��������;1,��ͣ����;
						//bit4:0,��ͨ�ļ�,1,flac�ļ�	  			 
						//bit3~1:����.
						//bit0:0,������ֹ����;1,������
	u8 mode;			//����ģʽ
						//0,ȫ��ѭ��;1,����ѭ��;2,�������;
	u8 *path;			//��ǰ�ļ���·��
	u8 *name;			//��ǰ���ŵ�MP3��������
	u16 namelen;		//name��ռ�ĵ���.
	u16 curnamepos;		//��ǰ��ƫ��

	u16 playtime;		//����ʱ��	
	u16 kbps;			//������
	u16 curindex;		//��ǰ���ŵ���Ƶ�ļ�����
	u16 mfilenum;		//�����ļ���Ŀ	    
	u16 *mfindextbl;	//��Ƶ�ļ�������

	u8 fft_topval_tbl[FFT_BANDS];	//Ƶ�׶�ֵ��
	u8 fft_curval_tbl[FFT_BANDS];	//Ƶ�׵�ǰֵ��
	u8 fft_time_tbl[FFT_BANDS];		//��ֵͣ��ʱ���

	FIL* fmp3;			//MP3�ļ�	 
}_m_mp3dev;	
			  
extern _m_mp3dev *mp3dev;
						  	    													  
//extern OS_EVENT * mp3mbox;//�¼����ƿ�	 
extern u8 mp3sta;//MP3״̬	  


void music_play_task(void *pdata);
void mp3_stop(_m_mp3dev *mp3devx);
u8 mp3_filelist(_m_mp3dev *mp3devx);
void mp3_load_ui(void);
void mp3_show_vol(u8 pctx);
void mp3_time_show(u16 sx,u16 sy,u16 sec);
void fft_show_oneband(u16 x,u16 y,u16 width,u16 height,u16 curval,u16 topval);
void mp3_specalz_show(_m_mp3dev *mp3devx,u16 *pdt);
//void mp3_info_upd(_m_mp3dev *mp3devx,_progressbar_obj* mp3prgbx,_progressbar_obj* volprgbx,_lyric_obj* lrcx);
//void mp3_lrc_show(_m_mp3dev *mp3devx,_lyric_obj* lrcx);
u8 mp3_play(void);	    

#endif












