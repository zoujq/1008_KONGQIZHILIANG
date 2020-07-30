#ifndef __RADIO_H
#define __RADIO_H
#include "sys.h"
#include "includes.h" 	   	 
#include "common.h"		
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-������ ����	   
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
extern const u8* RADIO_DEMO_PIC;			//demoͼƬ·��	    
extern const u8* RADIO_RSSI_PIC;			//RSSIͼƬ·��	      
extern const u8* RADIO_STERO_PIC;			//STEROͼƬ·��	      
extern const u8* RADIO_MONO_PIC;			//MONOͼƬ·��

extern const u8* RADIO_PLAYR_PIC;			//���� �ɿ�
extern const u8* RADIO_PLAYP_PIC;			//���� ����
extern const u8* RADIO_PAUSER_PIC;			//��ͣ �ɿ�
extern const u8* RADIO_PAUSEP_PIC;			//��ͣ ����
extern const u8* RADIO_BTN_PIC_TBL[2][5];	//5��ͼƬ��ť��·��
////////////////////////////////////////////////////////////////////////////////////////////
#define RADIO_MAX_CH_VAL	50	//�������洢�ĵ�̨��Ŀ

//RADIO���ƹ�����
__packed typedef struct 
{			 						 
	u8 vol; 					//����
	u8 sta;						//ģʽ��״̬
								//[7]:������/������;0,������;1,ǿ�Ƶ�����
								//[6:4]:����
								//[3:2]:00,�ֶ���̨ģʽ;01,���Զ���̨ģʽ;10,ȫ�Զ���̨;
								//[1:0]:Ƶ��ѡ��;00,87~108,ŷ��Ƶ��;01:76~91,�ձ�Ƶ��;10:76~108,��չƵ��.
 	u8 curch;					//��ǰ��̨���	
	u8 totalch;					//�洢�ĵ�̨����Ŀ	
	u16 chtbl[RADIO_MAX_CH_VAL];//��̨Ƶ�ʱ�(10KhzΪ��λ),������ȫ�Զ���̨��ʱ��,Ĭ��Ƶ��0����Ϊ��ǰƵ��.									    
 
	u8 saveflag;				//�����־,0X0A,�������;����,����δ����	   
}_m_radiodev;	

void radio_load_ui(void);
void radio_show_freq(u8 *bfbase,u16 freq);
void radio_show_freqband(u8 min,u8 max);
void radio_band_process(_progressbar_obj *fmprgb,u8 band,u16 *fmin,u16 *fmax);
void radio_read_para(_m_radiodev * radio);
void radio_save_para(_m_radiodev * radio);

u8 radio_play(void);

#endif












