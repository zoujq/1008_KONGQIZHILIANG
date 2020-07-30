#include "includes.h"
#include "sramlcd.h"
#include "dma.h"
#include "guix.h"
#include "spb.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPBЧ��ʵ�� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   


//SPB ������
m_spb_dev spbdev;
#if ICOS_LOCATION==ICOS_SD_CARD//�����SD��  
//����ͼ·��
const u8* spb_bkpic_path_tbl[2]=
{
	"0:/SYSTEM/SPB/BACKPIC/LEFT.jpg",
	"0:/SYSTEM/SPB/BACKPIC/RIGHT.jpg",	
};	  
//icos��·����
const u8 * spb_icos_path_tbl[2][9]=
{
	{
		"0:/SYSTEM/SPB/ICOS/ebook.bmp",
	 	"0:/SYSTEM/SPB/ICOS/picture.bmp",	    
	  	"0:/SYSTEM/SPB/ICOS/music.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/appstore.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/time.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/set.bmp",	    
	  	"0:/SYSTEM/SPB/ICOS/game.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/radio.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/notebook.bmp",
	},	    
 	{
	 	"0:/SYSTEM/SPB/ICOS/setup.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/3d.bmp",	    
		"0:/SYSTEM/SPB/ICOS/paint.bmp",
	 	"0:/SYSTEM/SPB/ICOS/camera.bmp",	    
	  	"0:/SYSTEM/SPB/ICOS/recorder.bmp",	    
	  	"0:/SYSTEM/SPB/ICOS/usb.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/tomcat.bmp",
	  	"0:/SYSTEM/SPB/ICOS/wireless.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/calculator.bmp",
	},	 	    
};
#else//����� FLASH DISK
//����ͼ·��
const u8* spb_bkpic_path_tbl[2]=
{
	"1:/SYSTEM/SPB/BACKPIC/LEFT.jpg",
	"1:/SYSTEM/SPB/BACKPIC/RIGHT.jpg",	
};	  
//icos��·����
const u8 * spb_icos_path_tbl[2][9]=
{
	{
		"1:/SYSTEM/SPB/ICOS/ebook.bmp",
	 	"1:/SYSTEM/SPB/ICOS/picture.bmp",	    
	  	"1:/SYSTEM/SPB/ICOS/music.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/appstore.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/time.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/set.bmp",	    
	  	"1:/SYSTEM/SPB/ICOS/game.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/radio.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/notebook.bmp",
	},	    
 	{
	 	"1:/SYSTEM/SPB/ICOS/setup.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/3d.bmp",	    
		"1:/SYSTEM/SPB/ICOS/paint.bmp",
	 	"1:/SYSTEM/SPB/ICOS/camera.bmp",	    
	  	"1:/SYSTEM/SPB/ICOS/recorder.bmp",	    
	  	"1:/SYSTEM/SPB/ICOS/usb.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/tomcat.bmp",
	  	"1:/SYSTEM/SPB/ICOS/wireless.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/calculator.bmp",
	},	 	    
};
#endif


//����ICOS�Ķ�Ӧ��������
const u8* icos_name_tbl[GUI_LANGUAGE_NUM][2][9]=
{
	{
		{
			"����ͼ��","�������","���ֲ���",
			"Ӧ������","ʱ��","ϵͳ����",
			"FC��Ϸ��","������","���±�",	   
		},
		{
			"������","3D","��д����",
			"�����","¼����","USB ����",
			"TOM è","���ߴ���","������",
		},
	},
	{
		{
			"��ӈD��","���a���","��������",
			"��������","�r�","ϵ�y�O��",
			"FC�[��C","�����C","ӛ�±�",	   
		},
		{
			"�\����","3D","�֌����P",
			"�����","����C ","USB �B��",
			"TOM ؈","�o������","Ӌ����",
		},
	},
	{
		{
			"EBOOK","PHOTOS","MUSIC",
			"APP","TIME","SETTINGS",
			"FC GAMES","RADIO","NOTEPAD",	   
		},
		{
			"EXE","3D","PAINT",
			"CAMERA","RECODER","USB",
			"TOM CAT","WIRELESS","CALC",
		},
	},
};						  

//��ʼ��spb��������			  
void spb_init(void)
{
	u16 i,j;
	spbdev.oldxpos=0;
	spbdev.curxpos=0;
	spbdev.curypos=0;
	spbdev.spbsta=0;
	spbdev.selico=0xff;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			spbdev.icos[0][i*3+j].x=5+j*80;
			spbdev.icos[0][i*3+j].y=10+i*105;
			spbdev.icos[0][i*3+j].width=70;
			spbdev.icos[0][i*3+j].height=90;
			spbdev.icos[0][i*3+j].path=(u8*)spb_icos_path_tbl[0][i*3+j];
			spbdev.icos[0][i*3+j].name=(u8*)icos_name_tbl[gui_phy.language][0][i*3+j];
		
			spbdev.icos[1][i*3+j].x=5+j*80;
			spbdev.icos[1][i*3+j].y=10+i*105;
			spbdev.icos[1][i*3+j].width=70;
			spbdev.icos[1][i*3+j].height=90;
			spbdev.icos[1][i*3+j].path=(u8*)spb_icos_path_tbl[1][i*3+j]; 
			spbdev.icos[1][i*3+j].name=(u8*)icos_name_tbl[gui_phy.language][1][i*3+j];
		}
	} 
 	//����sram->lcd��dma����
	LCD_DMA_Coinfig();
	//ָ��sramlcd
	gui_phy.read_point=slcd_read_point;
	gui_phy.draw_point=slcd_draw_point;	 
	pic_phy.read_point=slcd_read_point;
	pic_phy.draw_point=slcd_draw_point;	 
	 
	SLCD.frame=0;						 
	ai_load_picfile(spb_bkpic_path_tbl[0],0,0,240,320);//����һ��ͼƬ
	spb_load_icos(SLCD.frame);
// 	SLCD.frame=1;	//֡����������
//	ai_load_picfile(spb_bkpic_path_tbl[1],0,0,240,320);//���ڶ���ͼƬ
//	spb_load_icos(SLCD.frame);
	//ָ��lcd
	gui_phy.read_point=LCD_ReadPoint;
	gui_phy.draw_point=LCD_Fast_DrawPoint;	 
	pic_phy.read_point=LCD_ReadPoint;
	pic_phy.draw_point=LCD_Fast_DrawPoint;	 
	SLCD.pos=0;  		//���õ�ǰ��ʾ��λ��			 
	SLCD.frame=0;
	SLCD.show(SLCD.pos);//��ʾ������
}
//װ��ICOS
void spb_load_icos(u8 frame)
{
	u8 i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			minibmp_decode(spbdev.icos[frame][i*3+j].path,spbdev.icos[frame][i*3+j].x+5,spbdev.icos[frame][i*3+j].y,spbdev.icos[frame][i*3+j].width,spbdev.icos[frame][i*3+j].height,0,0);
 			gui_show_strmid(spbdev.icos[frame][i*3+j].x,spbdev.icos[frame][i*3+j].y+67,spbdev.icos[frame][i*3+j].width,16,SPB_FONT_COLOR,16,spbdev.icos[frame][i*3+j].name);//��ʾ����  
		}
	}
}
//����ѡ���ĸ�ͼ��
//sel:0~8����ǰҳ��ѡ��ico
void spb_set_sel(u8 sel)
{
	spbdev.selico=sel;
	SLCD.show(SLCD.pos);//ˢ�±���
	delay_ms(30);     
	gui_alphablend_area(spbdev.icos[SLCD.frame][sel].x,spbdev.icos[SLCD.frame][sel].y,spbdev.icos[SLCD.frame][sel].width,spbdev.icos[SLCD.frame][sel].height,SPB_ALPHA_COLOR,SPB_ALPHA_VAL);
	minibmp_decode(spbdev.icos[SLCD.frame][sel].path,spbdev.icos[SLCD.frame][sel].x+5,spbdev.icos[SLCD.frame][sel].y,spbdev.icos[SLCD.frame][sel].width,spbdev.icos[SLCD.frame][sel].height,0,0);
 	gui_show_strmid(spbdev.icos[SLCD.frame][sel].x,spbdev.icos[SLCD.frame][sel].y+67,spbdev.icos[SLCD.frame][sel].width,16,SPB_FONT_COLOR,16,spbdev.icos[SLCD.frame][sel].name);//��ʾ����
}
//��Ļ�������������
//����ֵ:0~8,��˫����ͼ����.		    
//       0xff,û���κ�ͼ�걻˫�����߰���
u8 spb_move_chk(void)
{		 
	u8 i=0xff;
	u8 movecnt=0;	//�õ���������
	tp_dev.scan(0);//ɨ��																		 
	if(tp_dev.sta&TP_PRES_DOWN)//�а���������
	{
		if(spbdev.spbsta&0X80)//�Ѿ��������
		{
			movecnt=spbdev.spbsta&0X3F;//�õ���������
			if(gui_disabs(spbdev.curxpos,tp_dev.x)>=SPB_MOVE_WIN)//�ƶ����ڵ���SPB_MOVE_WIN����
			{
				if(movecnt<SPB_MOVE_MIN/SPB_MOVE_WIN)spbdev.spbsta++;//��������1	
			}
		}			  		    
	 	spbdev.curxpos=tp_dev.x;				//��¼��ǰ����
	 	spbdev.curypos=tp_dev.y;				//��¼��ǰ����	
 		if((spbdev.spbsta&0X80)==0)				//������һ�α�����
		{
			spbdev.spbsta=0;				 	//״̬����
			spbdev.spbsta|=1<<7;				//��ǰ���	    
			spbdev.oldxpos=tp_dev.x;			//��¼����ʱ������ 
		}else if(spbdev.spbsta&0X40)			//�л���
		{
			if(spbdev.oldxpos>tp_dev.x) 		//x����,��Ļpos����
			{
				if(SLCD.pos<240)SLCD.pos+=spbdev.oldxpos-tp_dev.x; 
				if(SLCD.pos>240)SLCD.pos=240;	//�����. 
			}else 								//����,��Ļpos����
			{			    
				if(SLCD.pos>0)SLCD.pos-=tp_dev.x-spbdev.oldxpos;
				if(SLCD.pos>240)SLCD.pos=0;		//�����.	   		  
			}	    
			spbdev.oldxpos=tp_dev.x;
			SLCD.show(SLCD.pos);
 		}else if((gui_disabs(spbdev.curxpos,spbdev.oldxpos)>=SPB_MOVE_MIN)&&(movecnt>=SPB_MOVE_MIN/SPB_MOVE_WIN))//�������볬��SPB_MOVE_MIN,���Ҽ�⵽����Ч������������SPB_MOVE_MIN/SPB_MOVE_WIN.
		{
			spbdev.spbsta|=1<<6;//��ǻ���	
		}   
	}else //�����ɿ���
	{
		if(spbdev.spbsta&0x80)//֮ǰ�а���
		{	  
			if(spbdev.spbsta&0X40)//�л���
			{										    
				if(SLCD.frame==1)//ԭ���ڵڶ�֡,ֻ������ 
				{	
					if((240-SLCD.pos)>SPB_MOVE_ACT)
					{
						SLCD.move(1,5,SLCD.pos);
						SLCD.pos=0;
					}else 
					{
						SLCD.move(0,5,SLCD.pos);
						SLCD.pos=240;
					}			 
				}else	   //ԭ���ڵ�һ֡,ֻ������  0��ʼ
				{
					if(SLCD.pos>SPB_MOVE_ACT)
					{
						SLCD.move(0,5,SLCD.pos);
						SLCD.pos=240;
					}else 
					{	
						SLCD.move(1,5,SLCD.pos);
						SLCD.pos=0;
					}		   

				}
 				spbdev.selico=0xff;//ȡ��spbdev.selicoԭ�ȵ�����  
			}else	//���ڵ㰴.
			{
				for(i=0;i<9;i++)
				{
					if((spbdev.curxpos>spbdev.icos[SLCD.frame][i].x)&&(spbdev.curxpos<spbdev.icos[SLCD.frame][i].x+spbdev.icos[SLCD.frame][i].width)&&(spbdev.curxpos>spbdev.icos[SLCD.frame][i].x)&&
					   (spbdev.curypos<spbdev.icos[SLCD.frame][i].y+spbdev.icos[SLCD.frame][i].height))
					{
						break;//�õ�ѡ�еı��	
					}
				}
				if(i<9)
				{
					if(i!=spbdev.selico)//ѡ���˲�ͬ��ͼ��,�л�ͼ��
					{
						spb_set_sel(i);
						i=0xff;
					}else
					{
						spbdev.selico=0XFF;//������˫��,���¸�λselico.
					}
				}else i=0xff;//��Ч�ĵ㰴.
			}
		} 							
		spbdev.spbsta=0;//��ձ�־
	}
	return i;

}







