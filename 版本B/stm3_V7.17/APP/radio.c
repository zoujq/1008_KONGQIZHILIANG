#include "radio.h"
#include "settings.h"
#include "calendar.h"
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

#define RADIO_TITLE_COLOR   	0XFFFF		//������������ɫ	
#define RADIO_TITLE_BKCOLOR     0X0000		//���������ⱳ��ɫ	
#define RADIO_INFO_COLOR   	   	0X8410	 	//��Ϣ�������ɫ	
#define RADIO_MAIN_BKCOLOR    	0X18E3		//������ɫ	
#define RADIO_FREQ_COLOR    	0X07FF		//Ƶ����ɫ	


#if ICOS_LOCATION==ICOS_SD_CARD//�����SD��  
const u8* RADIO_DEMO_PIC="0:/SYSTEM/APP/RADIO/Demo.bmp";		//demoͼƬ·��	    
const u8* RADIO_RSSI_PIC="0:/SYSTEM/APP/RADIO/RSSI.bmp";		//RSSIͼƬ·��	      
const u8* RADIO_STERO_PIC="0:/SYSTEM/APP/RADIO/STERO.bmp";		//STEROͼƬ·��	      
const u8* RADIO_MONO_PIC="0:/SYSTEM/APP/RADIO/MONO.bmp";		//MONOͼƬ·��
	      
const u8* RADIO_PLAYR_PIC="0:/SYSTEM/APP/RADIO/PlayR.bmp";		//���� �ɿ�
const u8* RADIO_PLAYP_PIC="0:/SYSTEM/APP/RADIO/PlayP.bmp";		//���� ����
const u8* RADIO_PAUSER_PIC="0:/SYSTEM/APP/RADIO/PauseR.bmp";	//��ͣ �ɿ�
const u8* RADIO_PAUSEP_PIC="0:/SYSTEM/APP/RADIO/PauseP.bmp";	//��ͣ ����
//5��ͼƬ��ť��·��
const u8* RADIO_BTN_PIC_TBL[2][5]=  						
{
{
	"0:/SYSTEM/APP/RADIO/MenuR.bmp",
	"0:/SYSTEM/APP/RADIO/PrevR.bmp", 
	"0:/SYSTEM/APP/RADIO/PauseR.bmp", 
	"0:/SYSTEM/APP/RADIO/NextR.bmp",
	"0:/SYSTEM/APP/RADIO/BackR.bmp", 
},
{
	"0:/SYSTEM/APP/RADIO/MenuP.bmp",  
	"0:/SYSTEM/APP/RADIO/PrevP.bmp",  
	"0:/SYSTEM/APP/RADIO/PlayP.bmp",  
	"0:/SYSTEM/APP/RADIO/NextP.bmp", 
	"0:/SYSTEM/APP/RADIO/BackP.bmp",  
},
};
#else//����� FLASH DISK
const u8* RADIO_DEMO_PIC="1:/SYSTEM/APP/RADIO/Demo.bmp";		//demoͼƬ·��	    
const u8* RADIO_RSSI_PIC="1:/SYSTEM/APP/RADIO/RSSI.bmp";		//RSSIͼƬ·��	      
const u8* RADIO_STERO_PIC="1:/SYSTEM/APP/RADIO/STERO.bmp";		//STEROͼƬ·��	      
const u8* RADIO_MONO_PIC="1:/SYSTEM/APP/RADIO/MONO.bmp";		//MONOͼƬ·��	      
	      
const u8* RADIO_PLAYR_PIC="1:/SYSTEM/APP/RADIO/PlayR.bmp";		//���� �ɿ�
const u8* RADIO_PLAYP_PIC="1:/SYSTEM/APP/RADIO/PlayP.bmp";		//���� ����
const u8* RADIO_PAUSER_PIC="1:/SYSTEM/APP/RADIO/PauseR.bmp";	//��ͣ �ɿ�
const u8* RADIO_PAUSEP_PIC="1:/SYSTEM/APP/RADIO/PauseP.bmp";	//��ͣ ����
//5��ͼƬ��ť��·��
const u8* RADIO_BTN_PIC_TBL[2][5]=  						
{
{
	"1:/SYSTEM/APP/RADIO/MenuR.bmp",
	"1:/SYSTEM/APP/RADIO/PrevR.bmp", 
	"1:/SYSTEM/APP/RADIO/PauseR.bmp", 
	"1:/SYSTEM/APP/RADIO/NextR.bmp",
	"1:/SYSTEM/APP/RADIO/BackR.bmp", 
},
{
	"1:/SYSTEM/APP/RADIO/MenuP.bmp",  
	"1:/SYSTEM/APP/RADIO/PrevP.bmp",  
	"1:/SYSTEM/APP/RADIO/PlayP.bmp",  
	"1:/SYSTEM/APP/RADIO/NextP.bmp", 
	"1:/SYSTEM/APP/RADIO/BackP.bmp",  
},
}; 
#endif

//��������������
const u8* radio_set_tbl[GUI_LANGUAGE_NUM][2]=
{
{"ģʽ����","Ƶ��ѡ��",},
{"ģʽ�O��","�l���x��",},		 
{"Mode Set","Band Set",},
};
//��������̨ģʽ
const u8* radio_mode_tbl[GUI_LANGUAGE_NUM][3]=
{
{"�ֶ���̨","���Զ���̨","ȫ�Զ���̨",},
{"�ք����_","���Ԅ����_","ȫ�Ԅ����_",},
{"Manual","Semi-Auto","Auto",},
};		 
//������Ƶ��ѡ��
const u8* radio_band_tbl[GUI_LANGUAGE_NUM][3]=
{
{"ŷ��Ƶ��","�ձ�Ƶ��","��չƵ��",},
{"�W���l��","�ձ��l��","�Uչ�l��",},
{"US/Europe","Japan","Japan Wide",},
};



//����������������UI	  
void radio_load_ui(void)
{	
 	gui_fill_rectangle(0,0,240,20,RADIO_TITLE_BKCOLOR);					//����ɫ 
	gui_show_strmid(0,0,240,20,RADIO_TITLE_COLOR,16,(u8*)APP_MFUNS_CAPTION_TBL[7][gui_phy.language]);//��ʾ����
 	gui_fill_rectangle(0,20,240,300,RADIO_MAIN_BKCOLOR);				//����ɫ 
 	minibmp_decode((u8*)RADIO_DEMO_PIC,20,165,100,64,0,0);				//����64*64��ͼƬDEMO
 	minibmp_decode((u8*)APP_VOL_PIC,103,165+10,16,16,0,0);				//��������ͼ��
 
 	minibmp_decode((u8*)RADIO_STERO_PIC,22,32,16,16,0,0);				//����������ͼ��
 	minibmp_decode((u8*)RADIO_RSSI_PIC,240-28-20,32,16,16,0,0);			//�����ź�ǿ��ͼ��
     
 	gui_show_string("CH:",120,165+45,66,12,12,RADIO_INFO_COLOR);  		//��ʾʱ��
   	gui_show_string("Mhz",30+135,70+36,24,16,16,RADIO_FREQ_COLOR);   	//��ʾʱ��
}
//��������ʾ�����ٷֱ�
//pctx:0~100;
void radio_show_vol(u8 pctx)
{
	gui_phy.back_color=RADIO_MAIN_BKCOLOR;//���ñ���ɫΪ��ɫ
 	gui_fill_rectangle(120,195,24,12,RADIO_MAIN_BKCOLOR);//��䱳��ɫ 
	if(pctx==100)
	{
		gui_show_num(120,195,3,RADIO_INFO_COLOR,12,pctx,0x80);//��ʾ�����ٷֱ�   
		gui_show_ptchar(120+18,195,120+18+6,210+12,0,RADIO_INFO_COLOR,12,'%',0);	//��ʾ�ٷֺ�
	}else 
	{
		gui_show_num(120,195,2,RADIO_INFO_COLOR,12,pctx,0x80);//��ʾ�����ٷֱ�   
 		gui_show_ptchar(120+12,195,120+12+6,210+12,0,RADIO_INFO_COLOR,12,'%',0);	//��ʾ�ٷֺ�
  	}	 
}
//��ʾƵ��
//bfbase:�������ַ
//freq:760~1080.
void radio_show_freq(u8 *bfbase,u16 freq)
{
 	app_showbignum(bfbase,30,70,freq/10,3,60,RADIO_FREQ_COLOR,RADIO_MAIN_BKCOLOR);
	app_showbigchar(bfbase,30+90,70,'.',60,RADIO_FREQ_COLOR,RADIO_MAIN_BKCOLOR);	
	app_showbignum(bfbase,30+102,70,freq%10,1,60,RADIO_FREQ_COLOR,RADIO_MAIN_BKCOLOR);	    
}  
//��ʾƵ�η�Χ
void radio_show_freqband(u8 min,u8 max)
{
  	gui_phy.back_color=RADIO_MAIN_BKCOLOR;							//���ñ���ɫΪ��ɫ
  	gui_show_num(21,140+11,2,RADIO_INFO_COLOR,12,min,0x00);			//��ʾ��СƵ��
 	gui_show_num(240-20-18,140+11,3,RADIO_INFO_COLOR,12,max,0x00);	//��ʾ���Ƶ��  
}
//��ʾƵ�η�Χ
void radio_show_freqch(u8 chx)
{
  	gui_phy.back_color=RADIO_MAIN_BKCOLOR;							//���ñ���ɫΪ��ɫ
  	gui_show_num(120+18,165+45,2,RADIO_INFO_COLOR,12,chx,0x80);    	//��ʾ�ǰƵ�ʵ�̨���
}
 
//Ƶ�����ô���
void radio_band_process(_progressbar_obj *fmprgb,u8 band,u16 *fmin,u16 *fmax)
{
	switch(band)
	{
		case 0://����Ϊŷ��Ƶ��	    
	 		fmprgb->totallen=210;		//�ܳ���
			fmprgb->curpos=0;			//��ǰλ��	  
			*fmin=870;
			*fmax=1080;		  
			break;
		case 1://����Ϊ�ձ�Ƶ��
	 		fmprgb->totallen=150;		//�ܳ���
			fmprgb->curpos=0;			//��ǰλ��	  
			*fmin=760;
			*fmax=910;		  
			break;
		case 2://����Ϊ��չƵ��
	 		fmprgb->totallen=320;		//�ܳ���
			fmprgb->curpos=0;			//��ǰλ��	  
			*fmin=760;
			*fmax=1080;		  
			break;	
	}							  	
	radio_show_freqband(*fmin/10,*fmax/10);
 	progressbar_draw_progressbar(fmprgb);	//��������	
 	RDA5820_Band_Set(band);					//����Ƶ��	 
	RDA5820_Freq_Set(*fmin*10);				//���õ�ǰƵ��
}
//��ָ����һ��Ƶ�ʿ�ʼ���µ�Ƶ,ֱ���ҵ���һ��̨���ߵ������Ƶ����.
//bfbase:��������ֿ��ַ
//fmprgb:Ƶ����ʾ������
//startfreq:��ʼƵ��(��λ:100Khz)
//minfreq,maxfreq:��ֹƵ��(��λ:100Khz)
//mode:
//[7]:0,һ��Ҫ����һ��;1,�����������Ͳ�������.
//[6:1]:����
//[0]:��Ѱ�ķ���;0,�˲�����;1,��������;
//����ֵ:0,û�ҵ������ĵ�̨;1,�ҵ���һ����̨;
u8 radio_auto_tune(u8 *bfbase,_progressbar_obj *fmprgb,u16 *startfreq,u16 minfreq,u16 maxfreq,u8 mode)
{
	u8 res=0;
	u16 temp;
	u8 rssi=0;
	u16 tempfreq=*startfreq;//������ʼƵ��
 	while(1)
	{
		if(mode&0X01)	//��ǰ��̨
		{	  
			if(*startfreq<maxfreq)(*startfreq)++;   	//Ƶ������100Khz
			else if(mode&0X80)break;
			else *startfreq=minfreq;   
		}else			//�����̨
 		{
		 	if(*startfreq>minfreq)(*startfreq)--;   	//Ƶ������100Khz
			else if(mode&0X80)break;
			else *startfreq=maxfreq;   	    
		}		     
		RDA5820_Freq_Set(*startfreq*10);	   	//����Ƶ��
		delay_ms(20);							//�ȴ���Ƶ�ź��ȶ�
 		fmprgb->curpos=*startfreq-minfreq;
 		progressbar_draw_progressbar(fmprgb);	//��������		 
		radio_show_freq(bfbase,*startfreq);		//��ʾ��ǰƵ��
		temp=RDA5820_RD_Reg(0X0B);
		rssi=temp>>9;
		if(temp&(1<<8)||rssi>70)				//��һ����Ч��̨/�źŵ�ƽ�ϴ�. 
		{
			res=1;
			break;		 
		}
		if(tempfreq==*startfreq)			//���˸���Ҳû�ҵ��ϸ�ĵ�̨
		{		 
			res=0;								
 			break;
		}
	}
	return res;
}
//���������ݱ�����:SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj)+sizeof(_alarm_obj)
//��ȡ����������
//radio:��������Ϣ
void radio_read_para(_m_radiodev * radio)
{
	AT24CXX_Read(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj)+sizeof(_alarm_obj),(u8*)radio,sizeof(_m_radiodev));
}
//д������������
//radio:��������Ϣ 
void radio_save_para(_m_radiodev * radio)
{
   	OS_CPU_SR cpu_sr=0;
	OS_ENTER_CRITICAL();	//�����ٽ���(�޷����жϴ��) 
	AT24CXX_Write(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj)+sizeof(_alarm_obj),(u8*)radio,sizeof(_m_radiodev));
	OS_EXIT_CRITICAL();		//�˳��ٽ���(���Ա��жϴ��)
}

//������
u8 radio_play(void)
{
	u8 i;
	u8 res;
	u8 tcnt=0;
	u8 rval=0;		//1,�ڴ����;2,����,MP3��������;3,����,ֹͣMP3����.
	u8 stero=0;		//��������־ 
	u8 mode=0;		//ģʽ,Ĭ��Ϊ�ֶ���̨ģʽ
	u16 temp;
	u8 savemask=0;	//������
	static u16 curfreq=0;//��ǰƵ��
   
	_progressbar_obj* fmprgb,*volprgb;	//Ƶ�ʽ�����������������
	_btn_obj* tbtn[5];					//5������ť	
	_m_radiodev *fmdev;

	u8 pause=0;							//��ͣ��־.0,����ͣ;1,��ͣ.
	u16 fmfreq=0;						//��ǰFMƵ��
	u16 fmin=0,fmax=0;					//��ǰFMƵ�ʵ������Сֵ
	u16 *fmbkctbl=0;								
					  
//////////////////////////////////////////////////////////////////////////////
//�����ּ�������
	FIL* f_radio=0;	 
	u8 *bfbase=0;		//���ֿ�Ļ�ַ		 

	fmdev=(_m_radiodev*)gui_memin_malloc(sizeof(_m_radiodev));//����_m_radiodev�ֽڵ��ڴ����� 
	if(fmdev==NULL)rval=1;
	else
	{
 		radio_read_para(fmdev);			//��ȡ����������
 	}
  	f_radio=(FIL *)gui_memin_malloc(sizeof(FIL));//����FIL�ֽڵ��ڴ����� 
	if(f_radio==NULL)rval=1;			//����ʧ��
	else
	{
		res=f_open(f_radio,(const TCHAR*)APP_ASCII_60,FA_READ);//���ļ���
		if(res==FR_OK)
		{
			bfbase=(u8*)gui_memex_malloc(f_radio->fsize);	//Ϊ�����忪�ٻ����ַ
			if(bfbase==0)rval=1;
			else 
			{
				res=f_read(f_radio,bfbase,f_radio->fsize,(UINT*)&br);	//һ�ζ�ȡ�����ļ�
 			}
		} 
		if(res)rval=1;
	} 	    
/////////////////////////////////////////////////////////////////////////////////
  	if(curfreq==0)//��һ�ο�������������,��Ҫ��ʼ��һ��
	{
		if(RDA5820_Init())rval=1;						//��ʼ��FMģ��ʧ��
	}
	fmprgb=progressbar_creat(20,140,200,10,0X40);		//FM���Ž�����
	if(fmprgb==NULL)rval=1;
	volprgb=progressbar_creat(120,165+15,100,6,0X20);	//������С������				    
  	if(volprgb==NULL)rval=1;
	fmbkctbl=(u16*)gui_memex_malloc(180*192*2);			//Ϊ�����洢�����ڴ�
  	if(fmbkctbl==NULL)rval=1;
   
 	for(i=0;i<5;i++)//ѭ������5����ť
	{
		tbtn[i]=btn_creat(0+i*48,271,48,48,0,1);			//����ͼƬ��ť
		if(tbtn[i]==NULL){rval=1;break;}					//����ʧ��.
		tbtn[i]->picbtnpathu=(u8*)RADIO_BTN_PIC_TBL[0][i];
		tbtn[i]->picbtnpathd=(u8*)RADIO_BTN_PIC_TBL[1][i];
 		tbtn[i]->sta=0;	 
	}
	if(rval==0)//û�д���
	{
 	   	fmprgb->inbkcolora=0x738E;				//Ĭ��ɫ
	 	fmprgb->inbkcolorb=RADIO_INFO_COLOR;	//Ĭ��ɫ    
		fmprgb->prgbarlen=3;					//���곤��2����λ.
 		
		fmprgb->totallen=320;					//�ܳ���
		fmprgb->curpos=0;			 			//��ǰλ��
 		fmprgb->btncolor=BLUE;					//Ĭ��ɫ  
 
	   	volprgb->inbkcolora=RADIO_INFO_COLOR;	//Ĭ��ɫ
	 	volprgb->inbkcolorb=RADIO_INFO_COLOR; 	//Ĭ��ɫ 
	 	volprgb->infcolora=0X75D;				//Ĭ��ɫ
	 	volprgb->infcolorb=0X596;				//Ĭ��ɫ 	   						    						  		
		volprgb->curpos=fmdev->vol;				//�趨�����λ��
 		volprgb->totallen=15;					//������Χ:0~15	
							 	  					   
		mode=(fmdev->sta>>2)&0X03;				//�õ�֮ǰ���úõ�ģʽ

		radio_load_ui();//����������
		radio_show_vol((volprgb->curpos*100)/volprgb->totallen);	//��ʾ�����ٷֱ� 
		for(i=0;i<5;i++)btn_draw(tbtn[i]);			//����ť
		tbtn[2]->picbtnpathu=(u8*)RADIO_PLAYR_PIC;	//����һ��֮���Ϊ�����ɿ�״̬

 		RDA5820_RX_Mode();					//���õ�����ģʽ
		RDA5820_Rssi_Set(fmdev->vol);	  	//��������
		RDA5820_Space_Set(0);				//���ò���Ƶ��Ϊ100Khz
 		radio_band_process(fmprgb,fmdev->sta&0X03,&fmin,&fmax);//����Ƶ��,����ʾ������Ϣ
		if(mode!=2)//��ȫ�Զ���̨ģʽ,ֱ�����õ����Ƶ��
		{
			if(curfreq<fmin||curfreq>fmax)curfreq=fmin;//�ж�curfreq���ݺϷ���
			fmfreq=curfreq;
		}else//ȫ�Զ���̨,ֱ�����õ��ϴ�ѡ�е�Ƶ��
		{
			fmfreq=fmdev->chtbl[fmdev->curch];
		}
		RDA5820_Freq_Set(fmfreq*10);			//���õ�ǰƵ��
		radio_show_freq(bfbase,fmfreq);			//��ʾƵ��
 		fmprgb->curpos=fmfreq-fmin;				//������λ��
  		progressbar_draw_progressbar(fmprgb);	//��������	 
		progressbar_draw_progressbar(volprgb);	//��������
		tcnt=0;									 
  		radio_show_vol((volprgb->curpos*100)/volprgb->totallen);//��ʾ�����ٷֱ� 
  		Audiosel_Set(1);					//����Ϊ��Ƶ���Ϊ��������Ƶ
		radio_show_freqch(fmdev->curch);	//��ʾ��̨���
  		while(rval==0)
		{
			tcnt++;//��ʱ����.
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(10);							//��ʱ10ms
 			if(system_task_return)break;			//TPAD����
	 		for(i=0;i<5;i++)
			{
				res=btn_check(tbtn[i],&in_obj);
				if(res&&((tbtn[i]->sta&(1<<7))==0)&&(tbtn[i]->sta&(1<<6)))//�а����������ɿ�,����TP�ɿ���
				{
					switch(i)
					{
						case 0://��������ѡ��
	 						app_read_bkcolor(30,40,180,192,fmbkctbl);//��ȡ����ɫ
							i=0;
							//�����Ի���	 
							res=app_items_sel(30,60,180,72+40*2,(u8**)radio_set_tbl[gui_phy.language],2,(u8*)&i,0X90,(u8*)APP_MODESEL_CAPTION_TBL[gui_phy.language]);//2��ѡ��
	 						if(res==0)//ȷ�ϼ�����
							{
								if(i==0)	//����ģʽ
								{
									i=(fmdev->sta>>2)&0X03;
									temp=i;//��¼��ǰģʽ
									res=app_items_sel(30,40,180,72+40*3,(u8**)radio_mode_tbl[gui_phy.language],3,(u8*)&i,0X90,(u8*)radio_set_tbl[gui_phy.language][0]);//3��ѡ��
									app_recover_bkcolor(30,40,180,192,fmbkctbl);//�ָ�����ɫ  
	 								if(res==0)
									{
 										mode=i;					//����ģʽ
										fmdev->sta&=0XF3;		//���֮ǰ������
										fmdev->sta|=0X0C&(i<<2);//����ģʽ
 										if(mode==2)//ȫ�Զ���̨
										{
											savemask=1;			//������ݱ仯��
											fmdev->curch=0;		//���
											fmdev->totalch=0;	//���
											fmfreq=fmin;		
 											while(1)
											{				    	   
												res=radio_auto_tune(bfbase,fmprgb,&fmfreq,fmin,fmax,0X81);//��ǰ��̨.�ѵ�����ͽ���
												if(res)
												{
													fmdev->chtbl[fmdev->totalch]=fmfreq;//��¼���Ƶ��
													if(fmdev->totalch<RADIO_MAX_CH_VAL-1)fmdev->totalch++;//��������1
												}
												if(fmfreq==fmax)
												{			 
													if(fmdev->totalch)
													{
														fmdev->curch=fmdev->totalch-1;
						 								fmfreq=fmdev->chtbl[fmdev->curch];
													}else fmfreq=fmin;
													radio_show_freq(bfbase,fmfreq);	    
													fmprgb->curpos=fmfreq-fmin;
													progressbar_draw_progressbar(fmprgb);	//��������		 
													RDA5820_Freq_Set(fmfreq*10);			//���ñ�Ƶ��
													radio_show_freqch(fmdev->curch);						 
													break; 	
												}		   
	 										}
										}else if(temp!=mode)//ģʽ�ı���
										{
											savemask=1;				//������ݱ仯��
											radio_show_freqch(0);	//CH����Ϊ0
										} 
									}
								}else 		//����Ƶ��
								{
									i=fmdev->sta&0X03;//ȡ���ϴε�����
									temp=i;//��¼֮ǰ��Ƶ��
 									res=app_items_sel(30,40,180,72+40*3,(u8**)radio_band_tbl[gui_phy.language],3,(u8*)&i,0X90,(u8*)radio_set_tbl[gui_phy.language][1]);//3��ѡ��
									app_recover_bkcolor(30,40,180,192,fmbkctbl);//�ָ�����ɫ  
	 								if(res==0&&temp!=i)//Ƶ�α����
									{
										savemask=1;		 	//������ݱ仯��
										fmdev->curch=0;		//��ǰƵ�ʺ�����
										fmdev->totalch=0;	//��ЧƵ��������
										fmdev->sta&=0XFC;
 										fmdev->sta|=i;		//����Ƶ��	  
										radio_band_process(fmprgb,i,&fmin,&fmax);//����Ƶ��,����ʾ������Ϣ
										fmfreq=fmin;
										radio_show_freq(bfbase,fmfreq);	//��ʾƵ��
 									}
								}
							}else app_recover_bkcolor(30,40,180,192,fmbkctbl);//�ָ�����ɫ  
 							break;
						case 1://����Ƶ��
						case 3:	
							switch(mode)
							{   
								case 0://�ֶ���̨ģʽ	 
									if(i==1)
									{
										if(fmfreq>fmin)fmfreq--;
										else fmfreq=fmax;
									}else 
									{
										fmfreq++;
										if(fmfreq>fmax)fmfreq=fmin;
									}
									break;
								case 1://���Զ���̨ģʽ
									if(i==1)//����̨
									{				    	   
										res=radio_auto_tune(bfbase,fmprgb,&fmfreq,fmin,fmax,0);		   
 									}else	//��ǰ��̨
									{
										res=radio_auto_tune(bfbase,fmprgb,&fmfreq,fmin,fmax,1);		   
									}	    
									break;
								case 2://ȫ�Զ���̨ģʽ
									if(fmdev->totalch>0)//����Ҫ��һ����̨.
									{
										savemask=1;//������ݱ仯��
										if(i==1)//����̨
										{		
 											if(fmdev->curch)fmdev->curch--;
											else fmdev->curch=fmdev->totalch-1; 
	 									}else	//��ǰ��̨
										{
											if(fmdev->curch<(fmdev->totalch-1))fmdev->curch++;
											else fmdev->curch=0;		    	   
										}
										fmfreq=fmdev->chtbl[fmdev->curch];	  
										radio_show_freqch(fmdev->curch);  
									}
								 	break;
							}	 
 							radio_show_freq(bfbase,fmfreq);
							fmprgb->curpos=fmfreq-fmin;
							progressbar_draw_progressbar(fmprgb);	//��������		 
							RDA5820_Freq_Set(fmfreq*10);			//���õ�ǰƵ��
							break;
						case 2://����/��ͣ
							if(pause)//�л�������
							{
								pause=0;//ȡ����ͣ	    
								tbtn[2]->picbtnpathd=(u8*)RADIO_PLAYP_PIC; 
								tbtn[2]->picbtnpathu=(u8*)RADIO_PAUSER_PIC;
							}else	//�л�����ͣ״̬
							{
								pause=1;//�����ͣ
								tbtn[2]->picbtnpathd=(u8*)RADIO_PAUSEP_PIC; 
								tbtn[2]->picbtnpathu=(u8*)RADIO_PLAYR_PIC;
							}
							RDA5820_Mute_Set(pause);
							break;
						case 4://ֹͣ����/����	
							rval=1;  
							break;

					}   
				}
			}
			res=progressbar_check(volprgb,&in_obj);	//�������������
			if(res&&fmdev->vol!=volprgb->curpos)	//��������,��λ�ñ仯��.ִ����������
			{	
				savemask=1;		 					//������ݱ仯��	  
				fmdev->vol=volprgb->curpos&0X0F;	   								 
				RDA5820_Vol_Set(fmdev->vol);		//��������						   
  				radio_show_vol((volprgb->curpos*100)/volprgb->totallen);	//��ʾ�����ٷֱ�   
 			}	  
			if((tcnt%250)==0)//������ʾ��Ϣ,ÿ2.5sִ��һ��
			{
				tcnt=0;
 				temp=RDA5820_RD_Reg(0X0A);		//��ȡ0X0A������
 				temp=(temp>>10)&0X01;
				if(stero!=temp)
				{
					stero=temp;
			 		gui_fill_rectangle(22,32,16,16,RADIO_MAIN_BKCOLOR);//��䱳��ɫ 
					if(stero)//��������
					{
				 		minibmp_decode((u8*)RADIO_STERO_PIC,22,32,16,16,0,0);	//����������ͼ��
					}else
					{
				 		minibmp_decode((u8*)RADIO_MONO_PIC,22,32,16,16,0,0);	//���뵥����ͼ��
					}
				}
				temp=RDA5820_RD_Reg(0X0B);		//��ȡ0X0B������
			    temp>>=9;
			  	gui_phy.back_color=RADIO_MAIN_BKCOLOR;//���ñ���ɫΪ��ɫ
			 	gui_fill_rectangle(240-12-20,36,12,12,RADIO_MAIN_BKCOLOR);	//��䱳��ɫ 
				gui_show_num(240-12-20,36,2,RADIO_INFO_COLOR,12,temp,0x80);	//��ʾ�����ٷֱ�
			}
 		}
	}
	if(savemask)
	{
		printf("fm save!\r\n");
		radio_save_para(fmdev);			//�������������� 
	}
	if(systemset.fmrxmode==0)//��֧�ֺ�̨����
	{
		if(systemset.fmtxmode==1)//��Ҫ���÷���ģʽ
		{
	 		RDA5820_TX_Mode();							//����Ϊ����ģʽ 
			RDA5820_Freq_Set(systemset.fmtxfreq*10);	//���õ�systemset.fmtxfreq��Ƶ��ֵ
			RDA5820_TxPGA_Set(3); 						//��������Ϊ3
 		}
		if(systemset.audiosel==1)systemset.audiosel=3;	//����ͨ��
		Audiosel_Set(systemset.audiosel);				//���û�ԭ��������  
	}else systemset.audiosel=1;							//Ĭ��ͨ����Ϊ������
 	curfreq=fmfreq;										//�����˳�ʱ��Ƶ��
	for(i=0;i<5;i++)btn_delete(tbtn[i]);//ɾ����ť	
	progressbar_delete(fmprgb);
	progressbar_delete(volprgb);
	gui_memin_free(f_radio);
	gui_memin_free(fmdev);
	gui_memex_free(bfbase);
	gui_memex_free(fmbkctbl);
 	return rval;
}
































