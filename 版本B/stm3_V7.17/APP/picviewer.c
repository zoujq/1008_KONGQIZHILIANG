#include "picviewer.h"
#include "piclib.h"
#include "settings.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-�������ʵ�� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
 
//ͼƬ���
u8 picviewer_play(void)
{				 	 
	DIR picdir;	//picdirר��	  
	FILINFO picinfo;   		   
	u8 rval=0;			//����ֵ	  
	u8 *pname=0;
	u8 *fn=0;
	u8 picsta=0;		//ebook״̬
						//0,�����ļ����״̬    
						//1,˳�򲥷�ͼƬ
						//2,��ͣ״̬
						//3,˫��,�������ذ�ť��	  


	u16 curindex=0;	//��ǰ�����ͼƬ�ļ���������
	u8  endecode=0;	//ʹ�ܽ���
	u8 key;
	u8 tpccnt=0;	//�������´���
	u16 dtime=0;	//��ʱʱ��
	u8 keyup=1;		//�ɿ����
	u8 pictype=0;	//ͼƬ����
 
 	_btn_obj* rbtn;		//���ذ�ť�ؼ�
   	_filelistbox_obj * flistbox;
	_filelistbox_list * filelistx; 	//�ļ�

	app_filebrower((u8*)APP_MFUNS_CAPTION_TBL[1][gui_phy.language],0X07);//ѡ��Ŀ���ļ�,���õ�Ŀ������
   	flistbox=filelistbox_creat(0,20,240,280,1,12);		//����һ��filelistbox
	if(flistbox==NULL)rval=1;							//�����ڴ�ʧ��.
	else  
	{
		flistbox->fliter=FLBOX_FLT_PICTURE;	//ͼƬ�ļ�
 		filelistbox_addlist(flistbox,(u8*)APP_DISK_NAME_TBL[0][gui_phy.language],0);		//����0
		filelistbox_addlist(flistbox,(u8*)APP_DISK_NAME_TBL[1][gui_phy.language],0);		//����1
		filelistbox_draw_listbox(flistbox);
	} 	 

	//Ϊ���ļ������뻺����
 	picinfo.lfsize = _MAX_LFN * 2 + 1;
	picinfo.lfname = gui_memin_malloc(picinfo.lfsize);
	if(picinfo.lfname==NULL)rval=1;//�����ڴ�ʧ�� 
   	else gui_memset((u8*)picinfo.lfname,0,picinfo.lfsize);
 
	rbtn=btn_creat(199,300,40,19,0,0x03);//�������ְ�ť
	if(rbtn==NULL)rval=1;	//û���㹻�ڴ湻����
	else
	{
	 	rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];	//����
	 	rbtn->font=16;			//����		 
		rbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
		rbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
		btn_draw(rbtn);//����ť
	}
	LED1=1;//�ر�LED1
	while(rval==0)//��ѭ��
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
		delay_ms(5);
		if(system_task_return)break;			//TPAD����   	
		if(picsta==0)
		{
			filelistbox_check(flistbox,&in_obj);	//ɨ���ļ�
	 		if(flistbox->dbclick==0X81)				//˫���ļ���
			{	
				curindex=flistbox->selindex-flistbox->foldercnt;//�õ���ǰͼƬ������
				picsta=1;	//ͼƬ����״̬		 
				endecode=1;	//��һ��ͼƬ�Զ�����.
				LCD_Clear(0x0);//���� 	 
			}
		}
		key=btn_check(rbtn,&in_obj);
  		if(key&&((rbtn->sta&0X80)==0))
		{		  
			if(flistbox->dbclick!=0X81)//���ļ������ʱ���˷��ذ�ť,�򷵻���һ��Ŀ¼
			{
				filelistx=filelist_search(flistbox->list,flistbox->selindex);//�õ���ʱѡ�е�list����Ϣ
				if(filelistx->type==FICO_DISK)//�Ѿ�������������,���˳�ͼƬ���
				{				 
					break;
				}else filelistbox_back(flistbox);//�˻���һ��Ŀ¼	 
			} 
		} 
		if(endecode)
		{
     		rval=f_opendir(&picdir,(const TCHAR*)flistbox->path); //��ѡ�е�Ŀ¼
			if(rval)break;	 			   
			dir_sdi(&picdir,flistbox->findextbl[curindex]);
	 		rval=f_readdir(&picdir,&picinfo);//��ȡ�ļ���Ϣ
			if(rval)break;//�򿪳ɹ�    
			fn=(u8*)(*picinfo.lfname?picinfo.lfname:picinfo.fname);
			pname=gui_memin_malloc(strlen((const char*)fn)+strlen((const char*)flistbox->path)+2);//�����ڴ�
			if(pname==NULL)break;//����ʧ��
			{
				pname=gui_path_name(pname,flistbox->path,fn);	//�ļ�������·�� 
				pictype=f_typetell(pname); 
				if(pictype==T_GIF)gui_show_string(fn,5,5,lcddev.width-5,12,12,RED);//��ʾGIF����
 				ai_load_picfile(pname,0,0,240,320);				//�������ͼƬ
				if(pictype!=T_GIF)gui_show_string(fn,5,5,lcddev.width-5,12,12,RED);//��ʾͼƬ����		     								  
			}
			gui_memin_free(pname);			//�ͷ��ڴ� 
			pname=NULL;
			endecode=0;		    
		}
		keyup=1;
 		tpccnt=0;
		dtime=0;
		while(picsta)//����ɨ��ѭ��
		{     
			key=pic_tp_scan(picsta&0x02);   
			dtime++;
			if(dtime>400&&(picsta==1))key=3;//˳�򲥷�״̬��ʱ�����,�Զ�������һ��ͼƬ    
			if(key)//�а�������
			{ 
				dtime=0;
				if(keyup)
				{   
					keyup=0;
					if(key==1)//��һ��ͼƬ
					{
						if(curindex)curindex--;
						else curindex=flistbox->filecnt-1;
						endecode=1;
						LCD_Clear(0x0);//���� 	 
						break;
					}else if(key==2)//����ͣ�ͷ���֮ͣ���л�
					{
						if(tpccnt)//˫����
						{
							picsta=3;//˫����ʱ��,��������ͣ״̬
							LED1=0;	 
							app_gui_tcbar(0,300,240,20,0x01);	//�Ϸֽ���
							btn_draw(rbtn);//����ť	   
						}else 
						{
							tpccnt=1;//��ǰ�����һ��
							if(picsta==1)
							{
								picsta=2;
								LED1=0;	 	//��ʾ��ͣ
							}else 
							{
								picsta=1;
								LED1=1;	   	//��ͣ����
							}
						}   
					}else if(key==3)
					{
						if(systemset.picmode==0)//˳�򲥷�
						{
							if(curindex<(flistbox->filecnt-1))curindex++;
							else curindex=0;
						}else	//�������
						{   
							curindex=app_get_rand(flistbox->filecnt);//����õ���һ��ͼƬ�ı��   	 
						}
						endecode=1;
						LCD_Clear(0x0);//���� 	 
						break;	 
					}  
				}
			}else keyup=1;//��ǰ����ɿ�
			if(tpccnt)//˫����
			{
				if(dtime>20)tpccnt=0;//û�з���˫��
			}else if(pictype==T_GIF&&picsta==2&&dtime>3)//�����ɿ�,��ͣ״̬,GIF������
			{		 
				endecode=1;//�ٴν���
				break;//gif�����ʱ��,����ͣ��ʱ��,ѭ����ʾͼƬ.
			}	 
			if(picsta==3)//�������ذ�ť��
			{
				tp_dev.scan(0);    
				in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
				key=btn_check(rbtn,&in_obj);
		  		if(key&&((rbtn->sta&0X80)==0))picsta=0;	//�ص��ļ����״̬
			}
			delay_ms(10);  
			if(system_task_return)picsta=0;//TPAD����
			if(picsta==0)//�ص��ļ����״̬֮ǰ�Ĵ���
			{
				LED1=1;	   				//�ر�LED1
				flistbox->dbclick=0;	//���÷��ļ����״̬
				app_filebrower((u8*)APP_MFUNS_CAPTION_TBL[1][gui_phy.language],0X07);//ѡ��Ŀ���ļ�,���õ�Ŀ������
				btn_draw(rbtn);			//����ť
				filelistbox_rebuild_filelist(flistbox);//�ؽ�flistbox
 				system_task_return=0;	//�������˳�ͼƬ���
				break;
			}
		}
	}	
	LED1=1;//�ر�LED1
	filelistbox_delete(flistbox);	//ɾ��filelist
	btn_delete(rbtn);				//ɾ����ť	 	  
 	gui_memin_free(pname);			//�ͷ��ڴ�		  
 	gui_memin_free(picinfo.lfname);
 	return rval; 
}


//���ϰ벿��
//mode:0,û�е������ذ�ť.
//	   1,�������ذ�ť��
u8 pic_tp_scan(u8 mode)
{
	tp_dev.scan(0);    
	if(tp_dev.sta&TP_PRES_DOWN)
	{
		if(tp_dev.y<106)return 1;//��������
		if(tp_dev.y<212)return 2;//�����м�
		if(tp_dev.y<300)return 3;//��������
		if(mode==0)return 3; 	 //��������
	}
	return 0;
}






































