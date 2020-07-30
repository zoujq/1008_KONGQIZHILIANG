#ifndef __RECODER_H
#define __RECODER_H 	
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-¼���� ����	   
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
extern const u8* RECODER_DEMO_PIC;			//demoͼƬ·�� 	      
extern const u8* RECODER_RECR_PIC;			//¼�� �ɿ�
extern const u8* RECODER_RECP_PIC;			//¼�� ����
extern const u8* RECODER_PAUSER_PIC;		//��ͣ �ɿ�
extern const u8* RECODER_PAUSEP_PIC;		//��ͣ ����
extern const u8* RECODER_STOPR_PIC;			//ֹͣ �ɿ�
extern const u8* RECODER_STOPP_PIC;			//ֹͣ ����
////////////////////////////////////////////////////////////////////////////////////////////
 //RIFF��
typedef __packed struct
{
    u32 ChunkID;		   	//chunk id;����̶�Ϊ"RIFF",��0X46464952
    u32 ChunkSize ;		   	//���ϴ�С;�ļ��ܴ�С-8
    u32 Format;	   			//��ʽ;WAVE,��0X45564157
}ChunkRIFF ;
//fmt��
typedef __packed struct
{
    u32 ChunkID;		   	//chunk id;����̶�Ϊ"fmt ",��0X20746D66
    u32 ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:20.
    u16 AudioFormat;	  	//��Ƶ��ʽ;0X10,��ʾ����PCM;0X11��ʾIMA ADPCM
	u16 NumOfChannels;		//ͨ������;1,��ʾ������;2,��ʾ˫����;
	u32 SampleRate;			//������;0X1F40,��ʾ8Khz
	u32 ByteRate;			//�ֽ�����; 
	u16 BlockAlign;			//�����(�ֽ�); 
	u16 BitsPerSample;		//�����������ݴ�С;4λADPCM,����Ϊ4
//	u16 ByteExtraData;		//���ӵ������ֽ�;2��; ����PCM,û���������
//	u16 ExtraData;			//���ӵ�����,�����������ݿ��С;0X1F9:505�ֽ�  ����PCM,û���������
}ChunkFMT;	   
//fact�� 
typedef __packed struct 
{
    u32 ChunkID;		   	//chunk id;����̶�Ϊ"fact",��0X74636166;
    u32 ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:4.
    u32 NumOfSamples;	  	//����������; 
}ChunkFACT;
//data�� 
typedef __packed struct 
{
    u32 ChunkID;		   	//chunk id;����̶�Ϊ"data",��0X61746164
    u32 ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);�ļ���С-60.
}ChunkDATA;

//wavͷ
typedef __packed struct
{ 
	ChunkRIFF riff;	//riff��
	ChunkFMT fmt;  	//fmt��
	//ChunkFACT fact;	//fact�� ����PCM,û������ṹ��	 
	ChunkDATA data;	//data��		 
}__WaveHeader; 
	 		
void recoder_enter_rec_mode(u16 agc);
void recoder_wav_init(__WaveHeader* wavhead);//��ʼ��WAVͷ	
void recoder_show_agc(u16 x,u16 y,u8 agc);
u8 recoder_vu_get(u16 signallevel);
void recoder_vu_meter(u16 x,u16 y,u8 level);
u8 recoder_play(void);		   			   

#endif























