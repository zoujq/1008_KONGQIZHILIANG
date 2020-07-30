#include "recoder.h"
#include "vs10xx.h"
#include "settings.h"
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

#define RECODER_TITLE_COLOR   	0XFFFF		//¼����������ɫ	
#define RECODER_TITLE_BKCOLOR	0X0000		//¼�������ⱳ��ɫ	

#define RECODER_VU_BKCOLOR    	0X39C7		//VU Meter����ɫ	
#define RECODER_VU_L1COLOR    	0X07FF		//VU Meter L1ɫ	
#define RECODER_VU_L2COLOR    	0xFFE0		//VU Meter L2ɫ	
#define RECODER_VU_L3COLOR    	0xF800		//VU Meter L3ɫ	

#define RECODER_TIME_COLOR    	0X07FF		//ʱ����ɫ
#define RECODER_MAIN_BKCOLOR	0X18E3		//������ɫ

//������Ƕ����ɫ
#define RECODER_INWIN_FONT_COLOR		0X736C		//0XAD53		

#if ICOS_LOCATION==ICOS_SD_CARD//�����SD��   
const u8* RECODER_DEMO_PIC="0:/SYSTEM/APP/RECODER/Demo.bmp";		//demoͼƬ·�� 	      
const u8* RECODER_RECR_PIC="0:/SYSTEM/APP/RECODER/RecR.bmp";		//¼�� �ɿ�
const u8* RECODER_RECP_PIC="0:/SYSTEM/APP/RECODER/RecP.bmp";		//¼�� ����
const u8* RECODER_PAUSER_PIC="0:/SYSTEM/APP/RECODER/PauseR.bmp";	//��ͣ �ɿ�
const u8* RECODER_PAUSEP_PIC="0:/SYSTEM/APP/RECODER/PauseP.bmp";	//��ͣ ����
const u8* RECODER_STOPR_PIC="0:/SYSTEM/APP/RECODER/StopR.bmp";		//ֹͣ �ɿ�
const u8* RECODER_STOPP_PIC="0:/SYSTEM/APP/RECODER/StopP.bmp";		//ֹͣ ����
#else//����� FLASH DISK
const u8* RECODER_DEMO_PIC="1:/SYSTEM/APP/RECODER/Demo.bmp";		//demoͼƬ·�� 	      
const u8* RECODER_RECR_PIC="1:/SYSTEM/APP/RECODER/RecR.bmp";		//¼�� �ɿ�
const u8* RECODER_RECP_PIC="1:/SYSTEM/APP/RECODER/RecP.bmp";		//¼�� ����
const u8* RECODER_PAUSER_PIC="1:/SYSTEM/APP/RECODER/PauseR.bmp";	//��ͣ �ɿ�
const u8* RECODER_PAUSEP_PIC="1:/SYSTEM/APP/RECODER/PauseP.bmp";	//��ͣ ����
const u8* RECODER_STOPR_PIC="1:/SYSTEM/APP/RECODER/StopR.bmp";		//ֹͣ �ɿ�
const u8* RECODER_STOPP_PIC="1:/SYSTEM/APP/RECODER/StopP.bmp";		//ֹͣ ����
#endif 
const u8* recoder_set_tbl[GUI_LANGUAGE_NUM]={"��˷���������","�����L�����O��","MIC GAIN SET"};	   

//VS1053��WAV¼����bug,���plugin��������������� 							    
const u16 wav_plugin[40]=/* Compressed plugin */ 
{ 
0x0007, 0x0001, 0x8010, 0x0006, 0x001c, 0x3e12, 0xb817, 0x3e14, /* 0 */ 
0xf812, 0x3e01, 0xb811, 0x0007, 0x9717, 0x0020, 0xffd2, 0x0030, /* 8 */ 
0x11d1, 0x3111, 0x8024, 0x3704, 0xc024, 0x3b81, 0x8024, 0x3101, /* 10 */ 
0x8024, 0x3b81, 0x8024, 0x3f04, 0xc024, 0x2808, 0x4800, 0x36f1, /* 18 */ 
0x9811, 0x0007, 0x0001, 0x8028, 0x0006, 0x0002, 0x2a00, 0x040e,  
}; 
//����ADPCM ¼��ģʽ
//agc:0,�Զ�����.1024�൱��1��,512�൱��0.5��,���ֵ65535=64��		  
void recoder_enter_rec_mode(u16 agc)
{
	//�����IMA ADPCM,�����ʼ��㹫ʽ����:
 	//������=CLKI/256*d;	
	//����d=0,��2��Ƶ,�ⲿ����Ϊ12.288M.��ôFc=(2*12288000)/256*6=16Khz
	//���������PCM,������ֱ�Ӿ�д����ֵ 
   	VS_WR_Cmd(SPI_BASS,0x0000);    
 	VS_WR_Cmd(SPI_AICTRL0,8000);	//���ò�����,����Ϊ8Khz
 	VS_WR_Cmd(SPI_AICTRL1,agc);		//��������,0,�Զ�����.1024�൱��1��,512�൱��0.5��,���ֵ65535=64��	
 	VS_WR_Cmd(SPI_AICTRL2,0);		//�����������ֵ,0,�������ֵ65536=64X
 	VS_WR_Cmd(SPI_AICTRL3,6);		//��ͨ��(MIC����������)
	VS_WR_Cmd(SPI_CLOCKF,0X2000);	//����VS10XX��ʱ��,MULT:2��Ƶ;ADD:������;CLK:12.288Mhz
	VS_WR_Cmd(SPI_MODE,0x1804);		//MIC,¼������    
 	delay_ms(5);					//�ȴ�����1.35ms 
 	VS_Load_Patch((u16*)wav_plugin,40);//VS1053��WAV¼����Ҫplugin
}
//��ʼ��WAVͷ.
void recoder_wav_init(__WaveHeader* wavhead) //��ʼ��WAVͷ			   
{
	wavhead->riff.ChunkID=0X46464952;	//"RIFF"
	wavhead->riff.ChunkSize=0;			//��δȷ��,�����Ҫ����
	wavhead->riff.Format=0X45564157; 	//"WAVE"
	wavhead->fmt.ChunkID=0X20746D66; 	//"fmt "
	wavhead->fmt.ChunkSize=16; 			//��СΪ16���ֽ�
	wavhead->fmt.AudioFormat=0X01; 		//0X01,��ʾPCM;0X01,��ʾIMA ADPCM
 	wavhead->fmt.NumOfChannels=1;		//������
 	wavhead->fmt.SampleRate=8000;		//16Khz������ ��������
 	wavhead->fmt.ByteRate=wavhead->fmt.SampleRate*2;//16λ,��2���ֽ�
 	wavhead->fmt.BlockAlign=2;			//���С,2���ֽ�Ϊһ����
 	wavhead->fmt.BitsPerSample=16;		//16λPCM
   	wavhead->data.ChunkID=0X61746164;	//"data"
 	wavhead->data.ChunkSize=0;			//���ݴ�С,����Ҫ����  
}

//��ƽ��ֵ��
const u16 vu_val_tbl[10]={3000,4500,6500,9000,11000,14000,18000,22000,27000,32000};
//���źŵ�ƽ�õ�vu����ֵ
//signallevel:�źŵ�ƽ
//����ֵ:vuֵ
u8 recoder_vu_get(u16 signallevel)
{
	u8 i;
	for(i=10;i>0;i--)
	{
		if(signallevel>=vu_val_tbl[i-1])break;
	}
	return i;

}
//��ʾVU Meter
//level:0~10;
//x,y:����
void recoder_vu_meter(u16 x,u16 y,u8 level)
{
	u8 i;
	u16 vucolor=RECODER_VU_L1COLOR;
 	if(level>10)return ;
	if(level==0)
	{
		gui_fill_rectangle(x,y,218,10,RECODER_VU_BKCOLOR);	//��䱳��ɫ
		return;
	}   
	for(i=0;i<level;i++)
	{
		if(i==9)vucolor=RECODER_VU_L3COLOR;
		else if(i>5)vucolor=RECODER_VU_L2COLOR;
		gui_fill_rectangle(x+22*i,y,20,10,vucolor);	//��䱳��ɫ
	}
	if(level<10)gui_fill_rectangle(x+level*22,y,218-level*22,10,RECODER_VU_BKCOLOR);	//��䱳��ɫ	 
}   
//��ʾ¼��ʱ��
//��ʾ�ߴ�Ϊ:150*60
//x,y:��ַ
//bfbase:�������ַ
//tsec:������.
void recoder_show_time(u16 x,u16 y,u8 *bfbase,u32 tsec)
{
	u8 min;
	if(tsec>=60*100)min=99;
	else min=tsec/60;	 
	app_showbig2num(bfbase,x,y,min,60,RECODER_TIME_COLOR,RECODER_MAIN_BKCOLOR);
 	app_showbigchar(bfbase,x+60,y,':',60,RECODER_TIME_COLOR,RECODER_MAIN_BKCOLOR);	
	app_showbig2num(bfbase,x+90,y,tsec%60,60,RECODER_TIME_COLOR,RECODER_MAIN_BKCOLOR);	   	    
} 
//��ʾ����
//x,y:����(��Ҫ��0��ʼ)
//name:����
void recoder_show_name(u16 x,u16 y,u8 *name)
{
 	gui_fill_rectangle(x-1,y-1,240,12,RECODER_MAIN_BKCOLOR);	//��䱳��ɫ
 	gui_show_ptstrwhiterim(x,y,240,y+12,0,BLACK,WHITE,12,name); 	  
}
//����¼����������UI	  
void recoder_load_ui(void)
{	
 	gui_fill_rectangle(0,0,240,20,RECODER_TITLE_BKCOLOR);				//��䱳��ɫ
	gui_show_strmid(0,0,240,20,RECODER_TITLE_COLOR,16,(u8*)APP_MFUNS_CAPTION_TBL[13][gui_phy.language]);//��ʾ����
 	gui_fill_rectangle(0,20,240,300,RECODER_MAIN_BKCOLOR);				//����ɫ 
 	minibmp_decode((u8*)RECODER_DEMO_PIC,70,100,100,100,0,0);			//����100*100��ͼƬDEMO
	recoder_vu_meter(11,200+5,0);										//��ʾvu meter;
	app_gui_tcbar(0,300,240,20,0x01);									//�Ϸֽ���
}

//ͨ��ʱ���ȡ�ļ���
//������SD������,��֧��FLASH DISK����
//��ϳ�:����"0:RECORDER/REC20120321210633.wav"���ļ���
void recoder_new_pathname(u8 *pname)
{	 
	RTC_Get();//����ʱ��
 	sprintf((char*)pname,"0:RECORDER/REC%04d%02d%02d%02d%02d%02d.wav",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
}
//��ʾAGC��С
//x,y:����
//agc:����ֵ 1~64,��ʾ1~64��;0,��ʾ�Զ�����
void recoder_show_agc(u16 x,u16 y,u8 agc)
{
	gui_phy.back_color=APP_WIN_BACK_COLOR;					//���ñ���ɫΪ��ɫ
	gui_fill_rectangle(x,y,24,12,APP_WIN_BACK_COLOR);		//��䱳��ɫ 
	if(agc==0)
	{
   		gui_show_string("AUTO",x,y,24,12,12,RECODER_INWIN_FONT_COLOR);//��ʾagc
	}else
	{
		gui_show_num(x,y,2,RECODER_INWIN_FONT_COLOR,12,agc,0X80);	//��ʾ��ֵ	
	}
}	
//agc���ý���.�̶��ߴ�:180*122    
//x,y:���Ͻ�����
//agc:�Զ�����ָ��(0~15)0,�Զ�����;15,15������
//caption:��������				  
//����ֵ:0,�ɹ�����;
//    ����,����������
u8 recoder_agc_set(u16 x,u16 y,u8 *agc,u8*caption)
{
 	u8 rval=0,res;
   	_window_obj* twin=0;			//����
 	_btn_obj * rbtn=0;				//ȡ����ť		  
 	_btn_obj * okbtn=0;				//ȷ����ť		  
	_progressbar_obj * agcprgb;		//AGC���ý�����
 	u8 tempagc=*agc;

  	twin=window_creat(x,y,180,122,0,1|1<<5,16);			//�������� ��
 	agcprgb=progressbar_creat(x+10,y+52,160,15,0X20);	//����������
	if(agcprgb==NULL)rval=1;
  	okbtn=btn_creat(x+20,y+82,60,30,0,0x02);		//������ť
 	rbtn=btn_creat(x+20+60+20,y+82,60,30,0,0x02);	//������ť
	if(twin==NULL||rbtn==NULL||okbtn==NULL||rval)rval=1;
	else
	{
		//���ڵ����ֺͱ���ɫ
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
		//���ذ�ť����ɫ    
		rbtn->bkctbl[0]=0X8452;		//�߿���ɫ
		rbtn->bkctbl[1]=0XAD97;		//��һ�е���ɫ				
		rbtn->bkctbl[2]=0XAD97;		//�ϰ벿����ɫ
		rbtn->bkctbl[3]=0X8452;		//�°벿����ɫ
		okbtn->bkctbl[0]=0X8452;	//�߿���ɫ
		okbtn->bkctbl[1]=0XAD97;	//��һ�е���ɫ				
		okbtn->bkctbl[2]=0XAD97;	//�ϰ벿����ɫ
		okbtn->bkctbl[3]=0X8452;	//�°벿����ɫ

		agcprgb->totallen=15;		//���AGCΪ15
	   	agcprgb->curpos=tempagc;	//��ǰ�ߴ�


		rbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];	//����Ϊȡ��
 		okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];		//����Ϊȷ��
  		window_draw(twin);				//��������
		btn_draw(rbtn);					//����ť
		btn_draw(okbtn);				//����ť
		progressbar_draw_progressbar(agcprgb);
   		gui_show_string("AGC:",x+10,y+38,24,12,12,RECODER_INWIN_FONT_COLOR);//��ʾSIZE
		recoder_show_agc(x+10+24,y+38,tempagc);
 		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
			if(system_task_return)break;			//TPAD����
			res=btn_check(rbtn,&in_obj);			//ȡ����ť���
			if(res&&((rbtn->sta&0X80)==0))			//����Ч����
			{
				rval=1;
				break;//�˳�
			}
			res=btn_check(okbtn,&in_obj); 			//ȷ�ϰ�ť���
			if(res&&((okbtn->sta&0X80)==0))			//����Ч����
			{
				rval=0XFF;		   
				break;//ȷ����
			}	  	  
			res=progressbar_check(agcprgb,&in_obj);
 			if(res&&(tempagc!=agcprgb->curpos))	//�������Ķ���
			{
				tempagc=agcprgb->curpos;		//�������µĽ��  
 				recoder_show_agc(x+10+24,y+38,tempagc);
				if(tempagc<64)VS_WR_Cmd(SPI_AICTRL1,1024*tempagc);//��������,0,�Զ�����.1024�൱��1��,512�൱��0.5��,���ֵ65535=64��	
   				else VS_WR_Cmd(SPI_AICTRL1,65535);	//���õ��������
			} 	
 		}
	}
	window_delete(twin);							//ɾ������
	btn_delete(rbtn);								//ɾ����ť	
	progressbar_delete(agcprgb);//ɾ��������  
	system_task_return=0;		 
	if(rval==0XFF)
	{
		*agc=tempagc;
		return 0;
 	}
	return rval;
}
 		   

//¼����
//����¼���ļ�,��������SD��RECORDER�ļ�����.
u8 recoder_play(void)
{
	u8 res;
	u8 rval=0;
	__WaveHeader *wavhead=0;
	u32 sectorsize=0;
	FIL* f_rec=0;					//�ļ�
 	_btn_obj * rbtn=0;				//ȡ����ť		  
 	_btn_obj * mbtn=0;				//ѡ�ť	
 	_btn_obj * recbtn=0;			//¼����ť		  
 	_btn_obj * stopbtn=0;			//ֹͣ¼����ť
	u8 *recbuf;						//�����ڴ�	
		  
 	u16 w;
	u16 idx=0;
 
	u8 rec_sta=0;					//¼��״̬
									//[7]:0,û��¼��;1,��¼��;
									//[6:1]:����
									//[0]:0,����¼��;1,��ͣ¼��;

 	u16 *pset_bkctbl=0;				//����ʱ����ɫָ��
	u8 *pname=0;
	u8 timecnt=0;					//��ʱ��   
	u32 recsec=0;					//¼��ʱ��
 
	u8 vulevel=0;
	u8 temp;
	short tempval;
	u16 maxval=0;	   	   

	u8 recagc=4;//Ĭ������Ϊ4
//////////////////////////////////////////////////////////////////////////////
//�����ּ�������
 	u8 *bfbase=0;		//���ֿ�Ļ�ַ		 
  	f_rec=(FIL *)gui_memin_malloc(sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
	if(f_rec==NULL)rval=1;	//����ʧ��
 	else
	{
		res=f_open(f_rec,(const TCHAR*)APP_ASCII_60,FA_READ);//���ļ���
		if(res==FR_OK)
		{
			bfbase=(u8*)gui_memex_malloc(f_rec->fsize);	//Ϊ�����忪�ٻ����ַ
			if(bfbase==0)rval=1;
			else 
			{
				res=f_read(f_rec,bfbase,f_rec->fsize,(UINT*)&br);	//һ�ζ�ȡ�����ļ�
 			}
		} 
		if(res)rval=1;
	} 	    
/////////////////////////////////////////////////////////////////////////////////
 	wavhead=(__WaveHeader*)gui_memin_malloc(sizeof(__WaveHeader));//����__WaveHeader�ֽڵ��ڴ�����
	if(wavhead==NULL)rval=1; 
	recbuf=gui_memin_malloc(512);
	if(recbuf==NULL)rval=1;
	recoder_load_ui();						//װ��������
	rbtn=btn_creat(199,300,40,19,0,0x03);	//�������ְ�ť
	if(rbtn==NULL)rval=1;	//û���㹻�ڴ湻����
	else
	{																				
	 	rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//���� 
	 	rbtn->font=16;			//����		 
		rbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
		rbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
		btn_draw(rbtn);			//�ػ���ť
	}
	mbtn=btn_creat(0,300,40,19,0,0x03);		//�������ְ�ť
	if(mbtn==NULL)rval=1;					//û���㹻�ڴ湻����
	else
	{																				
	 	mbtn->caption=(u8*)GUI_OPTION_CAPTION_TBL[gui_phy.language];//���� 
	 	mbtn->font=16;			//����		 
		mbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
		mbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
		btn_draw(mbtn);			//�ػ���ť
	}
	recbtn=btn_creat(48,215+18,48,48,0,1);		//����ͼƬ��ť
	if(recbtn==NULL){rval=1;}				//����ʧ��.
	else
	{
		recbtn->picbtnpathu=(u8*)RECODER_RECR_PIC;
		recbtn->picbtnpathd=(u8*)RECODER_PAUSEP_PIC;
 		recbtn->sta=0;	 
	}
	stopbtn=btn_creat(48+96,215+18,48,48,0,1);	//����ͼƬ��ť
	if(stopbtn==NULL){rval=1;}				//����ʧ��.
	else
	{
		stopbtn->picbtnpathu=(u8*)RECODER_STOPR_PIC;
		stopbtn->picbtnpathd=(u8*)RECODER_STOPP_PIC;
 		recbtn->sta=0;	 
	}	   
	pname=gui_memin_malloc(40);	//����40���ֽ��ڴ�,����"0:RECORDER/REC20120321210633.wav"
	if(pname==NULL)rval=1;
	pset_bkctbl=gui_memex_malloc(180*122*2);//Ϊ����ʱ�ı���ɫ�������ڴ�
	if(pset_bkctbl==NULL)rval=1;			//�����ڴ����   
	if(rval==0)
	{    
		btn_draw(rbtn);	
		btn_draw(mbtn);	
		btn_draw(recbtn);
		recbtn->picbtnpathu=(u8*)RECODER_PAUSER_PIC;
		recbtn->picbtnpathd=(u8*)RECODER_RECP_PIC;	  
		btn_draw(stopbtn);
 		if(mp3dev!=NULL)mp3_stop(mp3dev);	//��ֹͣMP3����
  		Audiosel_Set(0);					//����Ϊ��Ƶ���Ϊ¼���������Ƶ
		recoder_enter_rec_mode(1024*recagc);				
   		while(VS_RD_Reg(SPI_HDAT1)>>8);		//�ȵ�buf ��Ϊ�����ٿ�ʼ  
  		recoder_show_time(45,40,bfbase,recsec);//��ʾʱ��
 	   	while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
 			if(system_task_return)break;			//TPAD����
			res=btn_check(rbtn,&in_obj); 			//��鷵�ذ�ť
			if(res&&((rbtn->sta&(1<<7))==0)&&(rbtn->sta&(1<<6)))break;//���ذ�ť
  			res=btn_check(mbtn,&in_obj); 			//������ð�ť
			if(res&&((mbtn->sta&(1<<7))==0)&&(mbtn->sta&(1<<6)))
			{   
				app_read_bkcolor(30,95,180,122,pset_bkctbl);//��ȡ����ɫ
				res=recoder_agc_set(30,95,&recagc,(u8*)recoder_set_tbl[gui_phy.language]);//����AGC
				if(res==0)
				{
					if(recagc<64)VS_WR_Cmd(SPI_AICTRL1,1024*recagc);//��������,0,�Զ�����.1024�൱��1��,512�൱��0.5��,���ֵ65535=64��	
	   				else VS_WR_Cmd(SPI_AICTRL1,65535);				//���õ��������
				}	
				app_recover_bkcolor(30,95,180,122,pset_bkctbl);//�ָ�����ɫ  				  
			}
  			res=btn_check(recbtn,&in_obj); 			//���¼����ť
			if(res&&((recbtn->sta&(1<<7))==0)&&(recbtn->sta&(1<<6)))
			{  
				if(rec_sta&0X01)//ԭ������ͣ,����¼��
				{
					rec_sta&=0XFE;//ȡ����ͣ
					recbtn->picbtnpathu=(u8*)RECODER_RECR_PIC;
					recbtn->picbtnpathd=(u8*)RECODER_PAUSEP_PIC;
				}else if(rec_sta&0X80)//�Ѿ���¼����,��ͣ
				{
					rec_sta|=0X01;//��ͣ
					recbtn->picbtnpathu=(u8*)RECODER_PAUSER_PIC;
					recbtn->picbtnpathd=(u8*)RECODER_RECP_PIC;
				}else	//��û��ʼ¼�� 
				{
 					rec_sta|=0X80;//��ʼ¼��
					recbtn->picbtnpathu=(u8*)RECODER_RECR_PIC;
					recbtn->picbtnpathd=(u8*)RECODER_PAUSEP_PIC;	 	 
			 		pname[0]='\0';					//��ӽ�����
					recoder_new_pathname(pname);	//�õ��µ�����
					recoder_show_name(2,24,pname);	//��ʾ����
			 		recoder_wav_init(wavhead);		//��ʼ��wav����	
 					res=f_open(f_rec,(const TCHAR*)pname, FA_CREATE_ALWAYS | FA_WRITE); 
					if(res)//�ļ�����ʧ��
					{
						rec_sta=0;//�����ļ�ʧ��,����¼��
						rval=0XFE;//��ʾ�Ƿ����SD��
					}else res=f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//д��ͷ����
 				}
								  
			}
  			res=btn_check(stopbtn,&in_obj); 			//���ֹͣ��ť
			if(res&&((recbtn->sta&(1<<7))==0)&&(recbtn->sta&(1<<6)))
			{
				if(rec_sta&0X80)//��¼��
				{
					wavhead->riff.ChunkSize=sectorsize*512+36;	//�����ļ��Ĵ�С-8;
			   		wavhead->data.ChunkSize=sectorsize*512;		//���ݴ�С
					f_lseek(f_rec,0);							//ƫ�Ƶ��ļ�ͷ.
			  		f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//д��ͷ����
					f_close(f_rec);
					sectorsize=0;

					recbtn->picbtnpathu=(u8*)RECODER_RECR_PIC;
					recbtn->picbtnpathd=(u8*)RECODER_PAUSEP_PIC;
					btn_draw(recbtn);	
					recbtn->picbtnpathu=(u8*)RECODER_PAUSER_PIC;
					recbtn->picbtnpathd=(u8*)RECODER_RECP_PIC;	  
				}
				rec_sta=0;
				recsec=0;
				recoder_show_time(45,40,bfbase,recsec);//��ʾʱ��
 			}
///////////////////////////////////////////////////////////
//��ȡ����			  
	  		w=VS_RD_Reg(SPI_HDAT1);	
			if((w>=256)&&(w<896))
			{
 				idx=0;				   	 
	  			while(idx<512) 	//һ�ζ�ȡ512�ֽ�
				{	 
		 			w=VS_RD_Reg(SPI_HDAT0);
					tempval=(short)w;
					if(tempval<0)tempval=-tempval;
					if(maxval<tempval)maxval=tempval;	    
	 				recbuf[idx++]=w&0XFF;
					recbuf[idx++]=w>>8;
				}	 				      
				timecnt++;
				if((timecnt%2)==0)
				{	 
					temp=recoder_vu_get(maxval);
					if(temp>vulevel)vulevel=temp;
					else if(vulevel)vulevel--;
					recoder_vu_meter(11,200+5,vulevel);//��ʾvu meter;
					maxval=0;							  
	 			}  
				if(rec_sta==0X80)//�Ѿ���¼����
				{
	 				res=f_write(f_rec,recbuf,512,&bw);//д���ļ�
					if(res)
					{
						printf("err:%d\r\n",res);
						printf("bw:%d\r\n",bw);
						break;//д�����.	  
					}
					sectorsize++;//����������1,ԼΪ32ms	 
				}			
			}
/////////////////////////////////////////////////////////////
 			if(recsec!=(sectorsize*4/125))//¼��ʱ����ʾ
			{
				recsec=sectorsize*4/125;
				recoder_show_time(45,40,bfbase,recsec);//��ʾʱ��
			}
		}					   
	}
	if(rval==0XFE)//�����ļ�ʧ����,��Ҫ��ʾ�Ƿ����SD��
	{
		window_msg_box(20,110,200,100,(u8*)APP_CREAT_ERR_MSG_TBL[gui_phy.language],(u8*)APP_REMIND_CAPTION_TBL[gui_phy.language],12,0,0);//��ʾSD���Ƿ����
		delay_ms(2000);//�ȴ�2����
 	}
	if(rec_sta&0X80)//�������¼��,����ʾ�������¼���ļ�
	{
 		res=window_msg_box(20,120,200,80,"",(u8*)APP_SAVE_CAPTION_TBL[gui_phy.language],12,0,0X03);
		if(res==1)//��Ҫ����
		{
			wavhead->riff.ChunkSize=sectorsize*512+36;	//�����ļ��Ĵ�С-8;
	   		wavhead->data.ChunkSize=sectorsize*512;		//���ݴ�С
			f_lseek(f_rec,0);							//ƫ�Ƶ��ļ�ͷ.
	  		f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//д��ͷ����
			f_close(f_rec);
		}
	}
	Audiosel_Set(3);//�л�Ϊ����  
	VS_HD_Reset();	//Ӳ��λVS1003
	Audiosel_Set(systemset.audiosel);				//���û�ԭ��������  
	gui_memex_free(bfbase);	
	gui_memex_free(pset_bkctbl);	
	gui_memin_free(wavhead);
	gui_memin_free(recbuf);	  
 	gui_memin_free(f_rec);
	btn_delete(rbtn);	 
	btn_delete(mbtn);	 
	btn_delete(recbtn);	 
	btn_delete(stopbtn);	 
	return rval;
}
			   			   


































