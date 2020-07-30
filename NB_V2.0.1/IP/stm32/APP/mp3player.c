#include "includes.h"		 
#include "mp3player.h"
#include "settings.h"
#include "ucos_ii.h"
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

//mp3���ſ�����
_m_mp3dev  *mp3dev=NULL;
_lyric_obj *lrcdev=NULL;	//��ʿ�����

////////////////////////////////////////////////////////
#if ICOS_LOCATION==ICOS_SD_CARD//�����SD��  
const u8 *FLAC_PATCH_PATH="0:/SYSTEM/APP/MP3/FLAC.BIN";			//FLAC���ŵ�PATCH�Ĵ��λ��
const u8 *SPEC_PATCH_PATH="0:/SYSTEM/APP/MP3/SPCALZ.BIN";		//Ƶ�׷�����PATCH�Ĵ��λ��
const u8* MP3_DEMO_PIC="0:/SYSTEM/APP/MP3/Demo.bmp";			//demoͼƬ·��
const u8* MP3_BTN_PIC_TBL[2][5]=  								//5��ͼƬ��ť��·��
{
{
	"0:/SYSTEM/APP/MP3/ListR.bmp",
	"0:/SYSTEM/APP/MP3/PrevR.bmp", 
	"0:/SYSTEM/APP/MP3/PauseR.bmp", 
	"0:/SYSTEM/APP/MP3/NextR.bmp",
	"0:/SYSTEM/APP/MP3/BackR.bmp", 
},
{
	"0:/SYSTEM/APP/MP3/ListP.bmp",  
	"0:/SYSTEM/APP/MP3/PrevP.bmp",  
	"0:/SYSTEM/APP/MP3/PlayP.bmp",  
	"0:/SYSTEM/APP/MP3/NextP.bmp", 
	"0:/SYSTEM/APP/MP3/BackP.bmp",  
},
};
const u8* MP3_PLAYR_PIC="0:/SYSTEM/APP/MP3/PlayR.bmp";		//���� �ɿ�
const u8* MP3_PLAYP_PIC="0:/SYSTEM/APP/MP3/PlayP.bmp";		//���� ����
const u8* MP3_PAUSER_PIC="0:/SYSTEM/APP/MP3/PauseR.bmp";	//��ͣ �ɿ�
const u8* MP3_PAUSEP_PIC="0:/SYSTEM/APP/MP3/PauseP.bmp";	//��ͣ ����
#else//�����FLASH DISK
const u8 *FLAC_PATCH_PATH="1:/SYSTEM/APP/MP3/FLAC.BIN";			//FLAC���ŵ�PATCH�Ĵ��λ��
const u8 *SPEC_PATCH_PATH="1:/SYSTEM/APP/MP3/SPCALZ.BIN";		//Ƶ�׷�����PATCH�Ĵ��λ��
const u8* MP3_DEMO_PIC="1:/SYSTEM/APP/MP3/Demo.bmp";			//demoͼƬ·��
const u8* MP3_BTN_PIC_TBL[2][5]=  								//5��ͼƬ��ť��·��
{
{
	"1:/SYSTEM/APP/MP3/ListR.bmp",
	"1:/SYSTEM/APP/MP3/PrevR.bmp", 
	"1:/SYSTEM/APP/MP3/PauseR.bmp", 
	"1:/SYSTEM/APP/MP3/NextR.bmp",
	"1:/SYSTEM/APP/MP3/BackR.bmp", 
},
{
	"1:/SYSTEM/APP/MP3/ListP.bmp",  
	"1:/SYSTEM/APP/MP3/PrevP.bmp",  
	"1:/SYSTEM/APP/MP3/PlayP.bmp",  
	"1:/SYSTEM/APP/MP3/NextP.bmp", 
	"1:/SYSTEM/APP/MP3/BackP.bmp",  
},
};
const u8* MP3_PLAYR_PIC="1:/SYSTEM/APP/MP3/PlayR.bmp";		//���� �ɿ�
const u8* MP3_PLAYP_PIC="1:/SYSTEM/APP/MP3/PlayP.bmp";		//���� ����
const u8* MP3_PAUSER_PIC="1:/SYSTEM/APP/MP3/PauseR.bmp";	//��ͣ �ɿ�
const u8* MP3_PAUSEP_PIC="1:/SYSTEM/APP/MP3/PauseP.bmp";	//��ͣ ����
#endif 

//������������
OS_EVENT * mp3mbox;//�¼����ƿ�	   	   
void music_play_task(void *pdata)
{	 
   	DIR mp3dir;			//mp3dirר��	  
 	FILINFO mp3info;	 	   
    u16 br;
	u8 res,rval;	  
	u8 *databuf;
	u8 *patchbuf=0;	  		   
	u16 i=0;   
	u8 *pname=0;		   
 		    
 	mp3mbox=OSMboxCreate((void*) 0);//��������
//	VS_HD_Reset();
//	VS_Soft_Reset();  	//��λVS1053
 	while(1)
	{
		mp3dev->curindex=(u32)OSMboxPend(mp3mbox,0,&rval)-1;//��������,Ҫ��ȥ1,��Ϊ���͵�ʱ��������1
		rval=0;
		databuf=(u8*)mymalloc(SRAMIN,4096);		//����512�ֽڵ��ڴ�����
		if(databuf==NULL)rval=1 ;				//�ڴ�����ʧ��.
 	
		//Ϊ���ļ������뻺����
	 	mp3info.lfsize = _MAX_LFN * 2 + 1;
		mp3info.lfname = gui_memin_malloc(mp3info.lfsize);
		if(mp3info.lfname==NULL)rval=1;//�����ڴ�ʧ�� 
	   	else gui_memset((u8 *)mp3info.lfname,0,mp3info.lfsize);
		if(rval==0)rval=f_opendir(&mp3dir,(const TCHAR*)mp3dev->path); //��ѡ�е�Ŀ¼
  		while(rval==0)
		{	  	 			   
			dir_sdi(&mp3dir,mp3dev->mfindextbl[mp3dev->curindex]);
	 		rval=f_readdir(&mp3dir,&mp3info);//��ȡ�ļ���Ϣ
			if(rval)break;//�򿪳ɹ�    
 			mp3dev->name=(u8*)(*mp3info.lfname?mp3info.lfname:mp3info.fname);
			pname=gui_memin_malloc(strlen((const char*)mp3dev->name)+strlen((const char*)mp3dev->path)+2);//�����ڴ�
			if(pname==NULL)break;//����ʧ��	    
			pname=gui_path_name(pname,mp3dev->path,mp3dev->name);	//�ļ�������·��  			 			  	 
 			VS_Restart_Play();  	//�������� 
			VS_Set_All();        	//������������Ϣ 			 
			VS_Reset_DecodeTime();	//��λ����ʱ�� 
			res=f_typetell(pname);
///////////////////////////////////////////////////////////////////////////////////
			if(res==0x4c)//flac
			{	
				res=f_open(mp3dev->fmp3,(const TCHAR*)FLAC_PATCH_PATH,FA_READ);//���ļ�
				mp3dev->sta|=1<<4;//���Ϊflac�ļ�
			}else
			{
				res=f_open(mp3dev->fmp3,(const TCHAR*)SPEC_PATCH_PATH,FA_READ);//���ļ�
				mp3dev->sta&=~(1<<4);//���Ϊ��ͨ�ļ�
			}	 
			if(res)break;//��flac patch����
			patchbuf=(u8*)mymalloc(SRAMEX,mp3dev->fmp3->fsize);	//����fsize�ֽڵ��ڴ�����
			if(patchbuf==NULL)break;//�����ڴ�ʧ��
			res=f_read(mp3dev->fmp3,patchbuf,mp3dev->fmp3->fsize,(UINT*)&br);	//һ�ζ�ȡ�����ļ�
		   	if(res==0)
			{
				VS_Load_Patch((u16*)patchbuf,mp3dev->fmp3->fsize/2);
			}
			myfree(SRAMEX,patchbuf);
			patchbuf=NULL;
			f_close(mp3dev->fmp3);	 
///////////////////////////////////////////////////////////////////////////////////
			res=f_open(mp3dev->fmp3,(const TCHAR*)pname,FA_READ);//���ļ�	 
			gui_memin_free(pname);
 			pname=NULL;
			if(res==0)//�򿪳ɹ�.
			{ 
				VS_SPI_SpeedHigh();	//����
				mp3dev->sta|=1<<7;	//��ǿ�ʼ����MP3
 				mp3dev->sta|=1<<6;	//���ִ����һ�θ������л�	   
				while(1)
				{
					res=f_read(mp3dev->fmp3,databuf,4096,(UINT*)&br);	//����readlen���ֽ�  
					i=0;
					do//������ѭ��
				    {  	
						if(VS_Send_MusicData(databuf+i)==0)//��VS10XX������Ƶ����
						{
							i+=32;
						}else   
						{
							delay_ms(5);//�������	   
							while(mp3dev->sta&(1<<5))//���������ͣ��
							{
								delay_ms(5);//�������,ͬʱ�ȴ�����ͣ
								if((mp3dev->sta&0x01)==0)break;//������ֹ		 
							}
							if((mp3dev->sta&0x01)==0)break;//������ֹ
								
						}	    	    
					}while(i<4096);//ѭ������4096���ֽ� 
					if(br!=4096||res!=0)
					{
						break;//������.
					}
					if((mp3dev->sta&0x01)==0)break;//������ֹ
				}
				f_close(mp3dev->fmp3);
			}	  
			if(mp3dev->sta&0x01)//�����ŵ�������,ѭ������
			{
				if(systemset.mp3mode==0)//˳�򲥷�
				{
					if(mp3dev->curindex<(mp3dev->mfilenum-1))mp3dev->curindex++;
					else mp3dev->curindex=0;
				}else if(systemset.mp3mode==1)//�������
				{						    
					mp3dev->curindex=app_get_rand(mp3dev->mfilenum);//�õ���һ�׸���������	  
				}else mp3dev->curindex=mp3dev->curindex;//����ѭ��				
 			}else break;	  
		}
		gui_memin_free(pname);
	    gui_memin_free(mp3info.lfname);
		myfree(SRAMEX,patchbuf);	 
		myfree(SRAMIN,databuf);	  	  
 		mp3dev->sta=0;//����ֹͣ
	}	 	    
}
///////////////////////////////////////////////////////////////////////////////////////////
//ֹͣMP3����
//mp3devx:MP3�ṹ��
void mp3_stop(_m_mp3dev *mp3devx)
{
	mp3devx->sta&=~(1<<0);		//������ֹ����
	while(mp3devx->sta&0X80);	//�ȴ�����ֹͣ
	mp3devx->sta|=1<<0;			//������  
}  

//�����б�
const u8* MUSIC_LIST[GUI_LANGUAGE_NUM]=
{
	"�����б�","�����б�","MUSIC LIST",
};
 
//MP3�ļ����,���ļ��洢����
//mp3devx:MP3�ṹ��
//����ֵ:0,��������/�����˳���ť.
//		 1,�ڴ����ʧ��		 
u8 mp3_filelist(_m_mp3dev *mp3devx)
{
	FILINFO mp3info;	 	   
	u8 res;
	u8 rval=0;			//����ֵ	  
  	u16 i;	    						   
 
 	_btn_obj* rbtn;		//���ذ�ť�ؼ�
 	_btn_obj* qbtn;		//�˳���ť�ؼ�

   	_filelistbox_obj * flistbox;
	_filelistbox_list * filelistx; 	//�ļ�
  			    
	app_filebrower((u8*)MUSIC_LIST[gui_phy.language],0X07);	//ѡ��Ŀ���ļ�,���õ�Ŀ������
   	flistbox=filelistbox_creat(0,20,240,280,1,12);		//����һ��filelistbox
	if(flistbox==NULL)rval=1;							//�����ڴ�ʧ��.
	else if(mp3devx->path==NULL)  
	{
		flistbox->fliter=FLBOX_FLT_MUSIC;	//���������ļ�
 		filelistbox_addlist(flistbox,(u8*)APP_DISK_NAME_TBL[0][gui_phy.language],0);		//����0
		filelistbox_addlist(flistbox,(u8*)APP_DISK_NAME_TBL[1][gui_phy.language],0);		//����1
		filelistbox_draw_listbox(flistbox);
	}else
	{
		flistbox->fliter=FLBOX_FLT_MUSIC;		//���������ļ�	 
		flistbox->path=(u8*)gui_memin_malloc(strlen((const char*)mp3devx->path)+1);//Ϊ·�������ڴ�
		strcpy((char *)flistbox->path,(char *)mp3devx->path);//����·��	    
		filelistbox_scan_filelist(flistbox);	//����ɨ���б� 
		flistbox->selindex=flistbox->foldercnt+mp3devx->curindex;//ѡ����ĿΪ��ǰ���ڲ��ŵ���Ŀ
		if(flistbox->scbv->totalitems>flistbox->scbv->itemsperpage)flistbox->scbv->topitem=flistbox->selindex;  
		filelistbox_draw_listbox(flistbox);		//�ػ� 		 
	} 	 		 
	//Ϊ���ļ������뻺����
 	mp3info.lfsize = _MAX_LFN * 2 + 1;
	mp3info.lfname = gui_memin_malloc(mp3info.lfsize);
	if(mp3info.lfname==NULL)rval=1;//�����ڴ�ʧ�� 
   	else gui_memset((u8 *)mp3info.lfname,0,mp3info.lfsize);
 
	rbtn=btn_creat(199,300,40,19,0,0x03);	//�����������ְ�ť
 	qbtn=btn_creat(0,300,40,19,0,0x03);		//�����˳����ְ�ť
	if(rbtn==NULL||qbtn==NULL)rval=1;	//û���㹻�ڴ湻����
	else
	{
	 	rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];	//����
	 	rbtn->font=16;			//����		 
		rbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
		rbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
		btn_draw(rbtn);//����ť

	 	qbtn->caption=(u8*)GUI_QUIT_CAPTION_TBL[gui_phy.language];	//����
	 	qbtn->font=16;			//����		 
		qbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
		qbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
		btn_draw(qbtn);//����ť
	}	   
   	while(rval==0)
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
		delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
		if(system_task_return)break;			//TPAD����
		filelistbox_check(flistbox,&in_obj);	//ɨ���ļ�
		res=btn_check(rbtn,&in_obj);
		if(res)
		{
			if(((rbtn->sta&0X80)==0))//��ť״̬�ı���
			{
				if(flistbox->dbclick!=0X81)
				{
 					filelistx=filelist_search(flistbox->list,flistbox->selindex);//�õ���ʱѡ�е�list����Ϣ
					if(filelistx->type==FICO_DISK)//�Ѿ�������������
					{				 
						break;
					}else filelistbox_back(flistbox);//�˻���һ��Ŀ¼	 
				} 
 			}	 
		}
		res=btn_check(qbtn,&in_obj);
		if(res)
		{
			if(((qbtn->sta&0X80)==0))//��ť״̬�ı���
			{ 
				break;//�˳�
 			}	 
		}   
		if(flistbox->dbclick==0X81)//˫���ļ���
		{											 
			mp3_stop(mp3devx);
			gui_memin_free(mp3devx->path);		//�ͷ��ڴ�
			gui_memex_free(mp3devx->mfindextbl);	//�ͷ��ڴ�
			mp3devx->path=(u8*)gui_memin_malloc(strlen((const char*)flistbox->path)+1);//Ϊ�µ�·�������ڴ�
			if(mp3devx->path==NULL){rval=1;break;}
			mp3devx->path[0]='\0';//���ʼ���������.
 			strcpy((char *)mp3devx->path,(char *)flistbox->path);
			mp3devx->mfindextbl=(u16*)gui_memex_malloc(flistbox->filecnt*2);//Ϊ�µ�tbl�����ڴ�
			if(mp3devx->mfindextbl==NULL){rval=1;break;}
		    for(i=0;i<flistbox->filecnt;i++)mp3devx->mfindextbl[i]=flistbox->findextbl[i];//����

			mp3devx->mfilenum=flistbox->filecnt;		//��¼�ļ�����	  
			OSMboxPost(mp3mbox,(void*)(flistbox->selindex-flistbox->foldercnt+1));//��������,��Ϊ���䲻��Ϊ��,������������1
 			flistbox->dbclick=0;
			break;	 							   			   
		}
	}	
	filelistbox_delete(flistbox);	//ɾ��filelist
	btn_delete(qbtn);				//ɾ����ť	  	 
	btn_delete(rbtn);				//ɾ����ť	   
  	gui_memin_free(mp3info.lfname);	
	if(rval)
	{
		gui_memin_free(mp3devx->path);		 //�ͷ��ڴ�
		gui_memex_free(mp3devx->mfindextbl); //�ͷ��ڴ�
		gui_memin_free(mp3devx);
 	}	    
	return rval; 
}

const u16 MP3_SPEC_BASE_CTBL[6]={0X6B6D,0X632C,0X5ACB,0X4A69,0X31A6,0X18E3};//Ƶ����ʾ�ײ�����ɫ��
//MP3����������
void mp3_load_ui(void)
{	
	u8 i;									   	  
	gui_fill_rectangle(0,0,240,20,MP3_TITLE_BKCOLOR);					//����ɫ 
	gui_show_strmid(0,0,240,20,MP3_TITLE_COLOR,16,(u8*)APP_MFUNS_CAPTION_TBL[2][gui_phy.language]);//��ʾ����
 	gui_fill_rectangle(0,20,240,300,MP3_MAIN_BKCOLOR);					//����ɫ 
 	minibmp_decode((u8*)MP3_DEMO_PIC,20,150,100,64,0,0);				//����64*64��ͼƬDEMO
 	minibmp_decode((u8*)APP_VOL_PIC,103,150+10,16,16,0,0);				//��������ͼ��
	gui_show_string("kbps",178+18,133,24,12,12,MP3_INFO_COLOR);			//��ʾkbps
 
  	gui_show_string("00%",120,178,66,12,12,MP3_INFO_COLOR);		//��ʾʱ��
	gui_show_string("00:00/00:00",21,133,66,12,12,MP3_INFO_COLOR);		//��ʾʱ��
 
 	gui_fill_colorblock(20,105,200,6,(u16*)MP3_SPEC_BASE_CTBL,0);		//�������
	for(i=3;i<200;i+=4)
	{
		gui_draw_vline(20+i,105,6,MP3_MAIN_BKCOLOR);//�������
	}															  
}
//��ʾ�����ٷֱ� 
void mp3_show_vol(u8 pctx)
{
	gui_phy.back_color=MP3_MAIN_BKCOLOR;//���ñ���ɫΪ��ɫ
 	gui_fill_rectangle(120,178,24,12,MP3_MAIN_BKCOLOR);//��䱳��ɫ 
	if(pctx==100)
	{
		gui_show_num(120,178,3,MP3_INFO_COLOR,12,pctx,0x80);//��ʾ�����ٷֱ�   
		gui_show_ptchar(120+18,178,120+18+6,178+12,0,MP3_INFO_COLOR,12,'%',0);	//��ʾ�ٷֺ�
	}else 
	{
		gui_show_num(120,178,2,MP3_INFO_COLOR,12,pctx,0x80);//��ʾ�����ٷֱ�   
 		gui_show_ptchar(120+12,178,120+12+6,178+12,0,MP3_INFO_COLOR,12,'%',0);	//��ʾ�ٷֺ�
  	}	 
}  
//��ʾMP3����ʱ��
//sx,sy:��ʼ����
//sec:ʱ��
void mp3_time_show(u16 sx,u16 sy,u16 sec)
{
	u16 min;
	min=sec/60;//�õ�������
	if(min>99)min=99;
	sec=sec%60;//�õ�������

	gui_phy.back_color=MP3_MAIN_BKCOLOR;//���ñ���ɫΪ��ɫ
	gui_show_num(sx,sy,2,MP3_INFO_COLOR,12,min,0x80);//��ʾʱ��
	gui_show_ptchar(sx+12,sy,239,319,0,MP3_INFO_COLOR,12,':',0);//��ʾð��
	gui_show_num(sx+18,sy,2,MP3_INFO_COLOR,12,sec,0x80);//��ʾʱ��	  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʾһ������
//x,y,width,height:λ�úͳߴ�
//curval:��ֵ
//topval:���ֵ
void fft_show_oneband(u16 x,u16 y,u16 width,u16 height,u16 curval,u16 topval)
{  
	gui_fill_rectangle(x,y,width,height-curval,MP3_MAIN_BKCOLOR);					//��䱳��ɫ
	gui_fill_rectangle(x,y+height-curval,width,curval,FFT_BANDS_COLOR);	//�����״ɫ
	gui_draw_hline(x,y+height-topval-1,width,FFT_TOP_COLOR);	
}
//��ʾFFT_BANDS������
//mp3devx:MP3�ṹ��
//pdt:Ƶ������
void mp3_specalz_show(_m_mp3dev *mp3devx,u16 *pdt)
{
	u8 i=0;
	u8 temp;								   
	for(i=0;i<FFT_BANDS;i++)	//��ʾ����Ƶ��
	{
		temp=(pdt[i]&0X3F)*2;	//�õ�curvalֵ,����2��
		if(mp3devx->fft_curval_tbl[i]<temp)//��ǰֵС��temp��
		{
			mp3devx->fft_curval_tbl[i]=temp;
 		}else
		{
			if(mp3devx->fft_curval_tbl[i]>1)mp3devx->fft_curval_tbl[i]-=2;
			else mp3devx->fft_curval_tbl[i]=0;
		}
		if(mp3devx->fft_curval_tbl[i]>mp3devx->fft_topval_tbl[i])//���·�ֵ
		{
			mp3devx->fft_topval_tbl[i]=mp3devx->fft_curval_tbl[i];
			mp3devx->fft_time_tbl[i]=10;//�����ֵͣ��ʱ��
		}
		if(mp3devx->fft_time_tbl[i])mp3devx->fft_time_tbl[i]--;
		else 
		{
			if(mp3devx->fft_topval_tbl[i])mp3devx->fft_topval_tbl[i]--;//��ֵ��С1
		}
		if(mp3devx->fft_curval_tbl[i]>63)mp3devx->fft_curval_tbl[i]=63;
		if(mp3devx->fft_topval_tbl[i]>63)mp3devx->fft_topval_tbl[i]=63; 
		fft_show_oneband(20+2+i*14,105-64,12,63,mp3devx->fft_curval_tbl[i],mp3devx->fft_topval_tbl[i]);//��ʾ����	   
	}   	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�µ�Ƶ��ֵ
const u16 VS_NEW_BANDS_FREQ_TBL[14]={80,300,800,1270,2016,3200,4500,6000,7500,9000,11000,13000,15000,20000};
//����MP3���ŵ���Ϣ
//mp3devx:MP3������
//mp3prgbx:������
//lrcx:��ʿ�����
void mp3_info_upd(_m_mp3dev *mp3devx,_progressbar_obj* mp3prgbx,_progressbar_obj* volprgbx,_lyric_obj* lrcx)
{
	u16 temp;			  
	u8 i;				 
 	if(mp3devx->sta&(1<<6))//ִ����һ�θ����л�,���¸������ֺ͵�ǰ������Ŀ����,mp3prgb���ȵ���Ϣ
	{
		mp3devx->sta&=~(1<<6);//��ձ�־λ
		gui_fill_rectangle(0,20+4,240,14,MP3_MAIN_BKCOLOR);//���¸������һ��,���֮ǰ����ʾ 
		gui_show_ptstrwhiterim(5,20+5,239-5,319,0,0X0000,0XFFFF,12,mp3devx->name);	//��ʾ�µ�����
		
		mp3devx->namelen=strlen((const char*)mp3devx->name);//�õ���ռ�ַ��ĸ���
		mp3devx->namelen*=6;//�õ�����
		mp3devx->curnamepos=0;//�õ�����

  		gui_phy.back_color=MP3_MAIN_BKCOLOR;//���ñ���ɫΪ��ɫ
  		mp3_show_vol((volprgbx->curpos*100)/volprgbx->totallen);				//��ʾ�����ٷֱ�   
 		gui_show_num(120,193,3,MP3_INFO_COLOR,12,mp3devx->curindex,0x80);		//�ǵ���
		gui_show_ptchar(120+18,193,120+18+6,193+12,0,MP3_INFO_COLOR,12,'/',0);	//��ʾ�ָ���
		gui_show_num(120+24,193,3,MP3_INFO_COLOR,12,mp3devx->mfilenum,0x80);	//�ǵ���
		//��ʱ��������
 		mp3devx->kbps=0;
		mp3devx->playtime=0;gui_fill_rectangle(20,133,66,12,MP3_MAIN_BKCOLOR);	//���֮ǰ����ʾ 
		gui_show_string("00:00/00:00",21,133,66,12,12,MP3_INFO_COLOR);			//ʱ������
  		gui_show_num(178,133,3,MP3_INFO_COLOR,12,mp3devx->kbps,0x80);			//�ǵ�����ʾ����
 		mp3prgbx->totallen=mp3devx->fmp3->fsize;	//�����ܳ���	
		mp3prgbx->curpos=0;	  	
		//���Ƶ������
		for(i=0;i<FFT_BANDS;i++)
		{
			mp3devx->fft_topval_tbl[i]=0;
 			mp3devx->fft_curval_tbl[i]=0;
  			mp3devx->fft_time_tbl[i]=0;	    
			fft_show_oneband(20+2+i*14,105-64,12,63,0,0);//ÿ�����Ӷ�����	   
		}				 
		VS_Set_Bands((u16*)VS_NEW_BANDS_FREQ_TBL,14);//����Ƶ��
		if(lrcx)
		{
			gui_fill_rectangle(20,216,220,36+10,MP3_MAIN_BKCOLOR);//���֮ǰ����ʾ
			lrc_read(lrcx,mp3devx->path,mp3devx->name);
		}
	}
	if(mp3devx->namelen>240-10)//������Ļ����
	{
		gui_fill_rectangle(0,20+4,240,14,MP3_MAIN_BKCOLOR);//���¸������һ��,���֮ǰ����ʾ 
		gui_show_ptstrwhiterim(5,20+5,239-5,319,mp3devx->curnamepos,0X0000,0XFFFF,12,mp3devx->name);	//��ʾ�µ�����
		mp3devx->curnamepos++;
		if(mp3devx->curnamepos+240-10>mp3devx->namelen+110)mp3devx->curnamepos=0;//ѭ����ʾ		
	}
	if(mp3devx->sta&(1<<7))//MP3���ڲ���
	{
	 	mp3prgbx->curpos=f_tell(mp3devx->fmp3);//�õ���ǰ�Ĳ���λ��
		progressbar_draw_progressbar(mp3prgbx);//���½�����λ��
		temp=VS_Get_DecodeTime();
		if(temp!=mp3devx->playtime)
		{
 			mp3devx->playtime=temp;
			mp3_time_show(21,133,mp3devx->playtime);//��ʾ����ʱ��
			temp=VS_Get_HeadInfo();//�õ�����
			if(mp3devx->kbps!=temp)
			{
				mp3devx->kbps=temp;
				gui_phy.back_color=MP3_MAIN_BKCOLOR;//���ñ���ɫΪ��ɫ
				gui_show_num(178,133,3,MP3_INFO_COLOR,12,mp3devx->kbps,0x80);//�ǵ�����ʾ����	   
				temp=(mp3devx->fmp3->fsize/temp)/125;  	//������ʱ��,��λs
				mp3_time_show(21+6*6,133,temp);			//��ʾ��ʱ��
			}   
		}	    
	}
}
//��ʾ���
//mp3devx:MP3������
//lrcx:��ʿ�����
void mp3_lrc_show(_m_mp3dev *mp3devx,_lyric_obj* lrcx)
{
	u8 t;
	u16 temp,temp1;	   	  
	if(lrcx->oldostime!=GUI_TIMER_10MS)//ÿ10ms����һ��
	{
		t=gui_disabs(GUI_TIMER_10MS,lrcx->oldostime);//��ֹ�ܾ�û�н����������µ�©��
		lrcx->oldostime=GUI_TIMER_10MS;
		if(t>10)t=1;
		lrcx->curtime+=t;//����10ms	 
		if(lrcx->indexsize)//�и�ʴ��� 
		{
			lrcx->detatime+=t;//��־ʱ��������10ms
			if(lrcx->curindex<lrcx->indexsize)//��û��ʾ��
			{
	 			if((lrcx->curtime%100)>80)//1���Ӻ����800ms,��Ҫ��ѯVS10XX�Ľ���ʱ��Ĵ�����ͬ�����
				{							 	 
					lrcx->curtime=(u32)VS_Get_DecodeTime()*100;//��������
				}
				if(lrcx->curtime>=lrcx->time_tbl[lrcx->curindex])//��ǰʱ�䳬����,��Ҫ���¸��
				{
					gui_fill_rectangle(20,216,200,36+10,MP3_MAIN_BKCOLOR);//���֮ǰ����ʾ
	 				if(lrcx->curindex)//��Ҫ��ʾ��һ��
					{
						lrcx->curindex--;
						lrcx->color=MP3_LRC_SCOLOR;
						lrc_show_linelrc(lrcx,20,216,200,12);//��ʾ��һ����
						lrcx->curindex++;//�ָ�.
					}
					if(lrcx->curindex<lrcx->indexsize-1)//��������ʾ��һ��
					{
						lrcx->curindex++;
						lrcx->color=MP3_LRC_SCOLOR;
						lrc_show_linelrc(lrcx,20,216+24+10,200,12);//��ʾ��һ����
						lrcx->curindex--;//�ָ�.
	 				}
					lrcx->color=MP3_LRC_MCOLOR;
					lrc_show_linelrc(lrcx,20,216+12+5,200,12);		//��ʾ������	    
					lrcx->curindex++;   
					if(lrcx->namelen>200)//��Ҫ�������
					{
						if(lrcx->curindex<lrcx->indexsize)//�������һ���ʻ��Ǵ��ڵ�.
						{
							temp=lrcx->time_tbl[lrcx->curindex-1];//��ǰ��ʵ�ʱ��
							temp1=lrcx->time_tbl[lrcx->curindex]; //��һ���ʵ�ʱ�� 	 	   
							lrcx->updatetime=(temp1-temp)/(lrcx->namelen-150);//����õ��������ʱ��,�������50����λ,��Ϊǰ��ĳ���ִ��ʱ��Ӱ��.
							if(lrcx->updatetime>20)lrcx->updatetime=20;//��󲻳���200ms;
 						}else lrcx->updatetime=5;//Ĭ�Ϲ���ʱ��.50ms	  
					}													   
				}
			}
			if(lrcx->detatime>=lrcx->updatetime)//ÿlrcx->updatetime*10ms������ʾ��ǰ���(�����Ҫ�����Ļ�)
			{			 
				if(lrcx->namelen>200)//��������ʾ����,��Ҫ������ʾ������
				{
 					gui_fill_rectangle(20,216+12+5,200,12,MP3_MAIN_BKCOLOR);//���֮ǰ����ʾ
					gui_show_ptstr(20,216+12+5,239-20,319,lrcx->curnamepos,MP3_LRC_MCOLOR,lrcx->font,lrcx->buf,1);//������ʾ������
					lrcx->curnamepos++;
					if(lrcx->curnamepos+200>lrcx->namelen+50)lrcx->curnamepos=0;//ѭ����ʾ		
				}
				lrcx->detatime=0;		  
			}
		}   
	}
}	  
//MP3����
u8 mp3_play(void)
{
	u8 i;
	u8 res;
	u8 tcnt=0;
	u8 rval=0;//1,�ڴ����;2,����,MP3��������;3,����,ֹͣMP3����.
	u16 specbuf[15];
	u8 lastvolpos;
 
	_progressbar_obj* mp3prgb,*volprgb;
	_btn_obj* tbtn[5];		    
 	if(mp3dev==NULL)//��һ�δ�,�����Ҫ���ڴ�
	{
		mp3dev=(_m_mp3dev*)gui_memin_malloc(sizeof(_m_mp3dev));
		if(mp3dev==NULL)return 1;							//�����ڴ�ʧ��
		gui_memset((u8 *)mp3dev,0,sizeof(_m_mp3dev));			//����	   
		mp3dev->fmp3=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
		if(mp3dev->fmp3==NULL)								//�ڴ�����ʧ��.	
		{
			gui_memin_free(mp3dev);	 						//�ͷ�֮ǰ���뵽���ڴ�
			return 1;	
		}	 
		lrcdev=lrc_creat();									//������ʹ���ṹ��
		if(lrcdev)
		{
			lrcdev->font=12;
 			lrcdev->bkcolor=MP3_MAIN_BKCOLOR;		    
		}
 		gui_memset((u8 *)mp3dev->fmp3,0,sizeof(FIL));			//����	   
	}   
	if((mp3dev->sta&0x80)==0)	//��ǰû�в���MP3
	{
		VS_HD_Reset();
		VS_Soft_Reset();		//��λ
		mp3_filelist(mp3dev);
		if((mp3dev->sta&0x80)==0)return 0;//����û��MP3�ڲ���,���˳�����
	}else mp3dev->sta|=1<<6;//ģ��һ���и�,�ø������ֵ���Ϣ��ʾ���� 
/////////////////////////////////////////////////////////////////////////////////
	mp3prgb=progressbar_creat(20,120,200,10,0X20);		//mp3���Ž�����
	if(mp3prgb==NULL)rval=1;
	volprgb=progressbar_creat(120,150+15,100,6,0X20);	//������С������
  	if(volprgb==NULL)rval=1;	   
 	volprgb->totallen=150;	//�����ܳ���,��Ƶ��100~250.ƫ��Ϊ100.	
	
	if(vsset.mvol>=100&&vsset.mvol<=250)volprgb->curpos=vsset.mvol-100;
	else//��������� 
	{
		vsset.mvol=0;
		volprgb->curpos=0;
	}	  
	lastvolpos=volprgb->curpos;//�趨�����λ��

	for(i=0;i<5;i++)//ѭ������5����ť
	{
		tbtn[i]=btn_creat(0+i*48,271,48,48,0,1);//����ͼƬ��ť
		if(tbtn[i]==NULL){rval=1;break;}//����ʧ��.
		tbtn[i]->picbtnpathu=(u8*)MP3_BTN_PIC_TBL[0][i];
		tbtn[i]->picbtnpathd=(u8*)MP3_BTN_PIC_TBL[1][i];
 		tbtn[i]->sta=0;	 
	}
	if(rval==0)//û�д���
	{
	   	mp3prgb->inbkcolora=0x738E;			//Ĭ��ɫ
	 	mp3prgb->inbkcolorb=MP3_INFO_COLOR;	//Ĭ��ɫ 
	 	mp3prgb->infcolora=0X75D;			//Ĭ��ɫ
	 	mp3prgb->infcolorb=0X596;			//Ĭ��ɫ 
	
	   	volprgb->inbkcolora=MP3_INFO_COLOR;	//Ĭ��ɫ
	 	volprgb->inbkcolorb=MP3_INFO_COLOR;	//Ĭ��ɫ 
	 	volprgb->infcolora=0X75D;			//Ĭ��ɫ
	 	volprgb->infcolorb=0X596;			//Ĭ��ɫ 	   						    						  		
							 	  					   
	
		mp3_load_ui();//����������
		mp3_show_vol((volprgb->curpos*100)/volprgb->totallen);	//��ʾ�����ٷֱ� 
		for(i=0;i<5;i++)btn_draw(tbtn[i]);		//����ť
		tbtn[2]->picbtnpathu=(u8*)MP3_PLAYR_PIC;//����һ��֮���Ϊ�����ɿ�״̬
		progressbar_draw_progressbar(mp3prgb);	//��������	 
		progressbar_draw_progressbar(volprgb);	//��������
		tcnt=0;
 		if(systemset.fmtxmode)//��������˷���,���FM����.
		{
			RDA5820_TX_Mode();	//����Ϊ����ģʽ 
			RDA5820_Freq_Set(systemset.fmtxfreq*10);	//���õ�systemset.fmtxfreq��Ƶ��ֵ
			RDA5820_TxPGA_Set(3); 						//��������Ϊ3
		}
		Audiosel_Set(0);		//����Ϊ��Ƶ���ΪMP3��Ƶ
		while(rval==0)
		{
			tcnt++;//��ʱ����.
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
	 		for(i=0;i<5;i++)
			{
				res=btn_check(tbtn[i],&in_obj);	 
				if((res&&((tbtn[i]->sta&(1<<7))==0)&&(tbtn[i]->sta&(1<<6)))||system_task_return)//�а����������ɿ�,����TP�ɿ��˻���TPAD����
				{
					if(system_task_return)i=4;//TPAD����
					switch(i)
					{
						case 0://file list
							mp3_filelist(mp3dev);
							mp3dev->sta|=1<<6;//ģ��һ���и�,�ø������ֵ���Ϣ��ʾ����
							//////////////////////////////////////////////////////////////////
 							mp3_load_ui();//���¼���������
							if(mp3dev->sta&(1<<5))//����ͣ
							{
								tbtn[2]->picbtnpathu=(u8*)MP3_PLAYR_PIC;
							}else tbtn[2]->picbtnpathu=(u8*)MP3_PAUSER_PIC;
							for(i=0;i<5;i++)btn_draw(tbtn[i]);		//����ť
							if(mp3dev->sta&(1<<5))//����ͣ
							{
								tbtn[2]->picbtnpathu=(u8*)MP3_PAUSER_PIC;
							}else tbtn[2]->picbtnpathu=(u8*)MP3_PLAYR_PIC;
							progressbar_draw_progressbar(mp3prgb);	//��������	 
							progressbar_draw_progressbar(volprgb);	//������
							if(system_task_return)//�ո��˳��ļ����
							{
								delay_ms(100);
								system_task_return=0;
							}		 
 							break;
						case 1://��һ��������һ��
						case 3:
				    		mp3_stop(mp3dev);
							if(systemset.mp3mode==1)//�������
							{						    
								mp3dev->curindex=app_get_rand(mp3dev->mfilenum);//�õ���һ�׸���������	  
							}else
							{
								if(i==1)//��һ��
								{
									if(mp3dev->curindex)mp3dev->curindex--;
									else mp3dev->curindex=mp3dev->mfilenum-1;
								}else
								{
									if(mp3dev->curindex<(mp3dev->mfilenum-1))mp3dev->curindex++;
									else mp3dev->curindex=0;
								}
							}
							OSMboxPost(mp3mbox,(void*)(mp3dev->curindex+1));//��������,��Ϊ���䲻��Ϊ��,������������1
							break;
						case 2://����/��ͣ
							if(mp3dev->sta&(1<<5))//����ͣ
							{
								mp3dev->sta&=~(1<<5);//ȡ����ͣ
								tbtn[2]->picbtnpathd=(u8*)MP3_PAUSEP_PIC; 
								tbtn[2]->picbtnpathu=(u8*)MP3_PLAYR_PIC;
							}else//��ͣ״̬
							{
								mp3dev->sta|=1<<5;//�����ͣ
								tbtn[2]->picbtnpathd=(u8*)MP3_PLAYP_PIC; 
								tbtn[2]->picbtnpathu=(u8*)MP3_PAUSER_PIC;
							}
							break;
						case 4://ֹͣ����/����
							if(mp3dev->sta&(1<<5))//��ͣ״̬�°��˷���
							{		   
								rval=3;//�˳�����,MP3ֹͣ����
								mp3_stop(mp3dev);//ֹͣ����
							}else//��ͣ״̬�°�����,�Ǿ͹ر�MP3���Ź���.
							{
								rval=2;//�˳����Ž���,MP3��������
 							}	 
							break;

					}   
				}
			}
			res=progressbar_check(volprgb,&in_obj);//�������������
			if(res&&lastvolpos!=volprgb->curpos)//��������,��λ�ñ仯��.ִ����������
			{		  
				lastvolpos=volprgb->curpos;
				if(volprgb->curpos)vsset.mvol=100+volprgb->curpos;//��������
				else vsset.mvol=0;									 
				VS_Set_Vol(vsset.mvol);//��������						   
  				mp3_show_vol((volprgb->curpos*100)/volprgb->totallen);	//��ʾ�����ٷֱ�   
 			}	  
			res=progressbar_check(mp3prgb,&in_obj);
			if(res&&((mp3prgb->sta&&PRGB_BTN_DOWN)==0))//��������,�����ɿ���,ִ�п������
			{
				f_lseek(mp3dev->fmp3,mp3prgb->curpos);//���ٶ�λ
			}	  
			if((tcnt%20)==0)mp3_info_upd(mp3dev,mp3prgb,volprgb,lrcdev);//������ʾ��Ϣ,ÿ100msִ��һ��
 			if(((mp3dev->sta&(1<<4))==0)&&(tcnt%5)==0)		//����flac,ִ��Ƶ����ʾ
			{
				res=VS_Get_Spec(specbuf);
				if(res)mp3_specalz_show(mp3dev,specbuf);	//��ʾƵ��
			}
			if(lrcdev!=NULL&&((mp3dev->sta&(1<<5))==0)&&(mp3dev->sta&(1<<7)))//���ڲ���,������ͣ,�Ҹ�ʽṹ������
			{
				mp3_lrc_show(mp3dev,lrcdev);	//������ʾ���	  
			}
		}
	}
 	for(i=0;i<5;i++)btn_delete(tbtn[i]);//ɾ����ť	
	progressbar_delete(mp3prgb);
	progressbar_delete(volprgb);
	if(rval==3)//�˳�MP3����.�Ҳ���̨����
	{
		if(systemset.audiosel==0)systemset.audiosel=3;//����ͨ��
		Audiosel_Set(systemset.audiosel);	//���û�ԭ��������  
		gui_memin_free(mp3dev->path);		//�ͷ��ڴ�
		gui_memex_free(mp3dev->mfindextbl);	//�ͷ��ڴ�
		myfree(SRAMIN,mp3dev->fmp3);		//�ͷ��ڴ�
		gui_memin_free(mp3dev);				//�ͷ��ڴ�	 
		lrc_delete(lrcdev);					//�ͷŸ����ʾ������ڴ�
		lrcdev=NULL;//ָ��յ�ַ
		mp3dev=NULL;//ָ��յ�ַ
	}else systemset.audiosel=0;				//Ĭ��ͨ����ΪMP3ͨ��  			  
	return rval;
}















