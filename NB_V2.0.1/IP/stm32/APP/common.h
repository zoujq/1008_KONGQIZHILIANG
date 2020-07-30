#ifndef __COMMON_H
#define __COMMON_H 	
#include "sys.h"
#include "touch.h"	 
#include "includes.h"
#include "gui.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APPͨ�� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   


//Ӳ��ƽ̨��Ӳ���汾����	   	
#define HARDWARE_VERSION	   		18		//Ӳ���汾,�Ŵ�10��,��1.0��ʾΪ10
#define SOFTWARE_VERSION	    	224		//����汾,�Ŵ�100��,��1.00,��ʾΪ100

//ϵͳ���ݱ����ַ			  
#define SYSTEM_PARA_SAVE_BASE 		100		//ϵͳ��Ϣ�����׵�ַ.��100��ʼ.			    

////////////////////////////////////////////////////////////////////////////////////////////
//��ͼ��/ͼƬ·��
extern const u8* APP_OK_PIC;			//ȷ��ͼ��
extern const u8* APP_CANCEL_PIC;		//ȡ��ͼ��
extern const u8* APP_UNSELECT_PIC;		//δѡ��ͼ��
extern const u8* APP_SELECT_PIC;		//ѡ��ͼ��
extern const u8* APP_ASCII_60;			//����������·��
extern const u8* APP_ASCII_28;			//����������·��
extern const u8* APP_VOL_PIC;			//����ͼƬ·��

extern const u8 APP_ALIENTEK_ICO[72];	//��������ͼ��,�����flash
////////////////////////////////////////////////////////////////////////////////////////////
//APP���ܹ�����Ŀ
#define APP_FUNS_NUM	18

//app��Ҫ���ܽ������
extern const u8* APP_MFUNS_CAPTION_TBL[APP_FUNS_NUM][GUI_LANGUAGE_NUM];
extern const u8* APP_DISK_NAME_TBL[2][GUI_LANGUAGE_NUM];

extern const u8* APP_MODESEL_CAPTION_TBL[GUI_LANGUAGE_NUM];
extern const u8* APP_REMIND_CAPTION_TBL[GUI_LANGUAGE_NUM];
extern const u8* APP_SAVE_CAPTION_TBL[GUI_LANGUAGE_NUM];
extern const u8* APP_CREAT_ERR_MSG_TBL[GUI_LANGUAGE_NUM];
//ƽ���ߵ���ֹ��ɫ����
#define WIN_SMOOTH_LINE_SEC	0XB1FFC4	//��ֹ��ɫ
#define WIN_SMOOTH_LINE_MC	0X1600B1	//�м���ɫ

//��������ѡ����Ŀ��������Ϣ
#define APP_ITEM_BTN1_WIDTH		60	  		//��2������ʱ�Ŀ��
#define APP_ITEM_BTN2_WIDTH		100			//ֻ��1������ʱ�Ŀ��
#define APP_ITEM_BTN_HEIGHT		30			//�����߶�
#define APP_ITEM_ICO_SIZE		32			//ICOͼ��ĳߴ�

#define APP_ITEM_SEL_BKCOLOR	0X0EC3		//ѡ��ʱ�ı���ɫ
#define APP_WIN_BACK_COLOR	 	0XC618		//���屳��ɫ

/////////////////////////////////////////////////////////////////////////
																			    
u32  app_get_rand(u32 max);
void app_srand(u32 seed);
void app_read_bkcolor(u16 x,u16 y,u16 width,u16 height,u16 *ctbl);
void app_recover_bkcolor(u16 x,u16 y,u16 width,u16 height,u16 *ctbl);
void app_gui_tcbar(u16 x,u16 y,u16 width,u16 height,u8 mode);
u8 app_get_numlen(long long num,u8 dir);
void app_show_float(u16 x,u16 y,long long num,u8 flen,u8 clen,u8 font,u16 color,u16 bkcolor);
void app_filebrower(u8 *topname,u8 mode);		 
void app_showbigchar(u8 *fontbase,u16 x,u16 y,u8 chr,u8 size,u16 color,u16 bkcolor);
void app_showbigstring(u8 *fontbase,u16 x,u16 y,const u8 *p,u8 size,u16 color,u16 bkcolor);
void app_showbignum(u8 *fontbase,u16 x,u16 y,u32 num,u8 len,u8 size,u16 color,u16 bkcolor);
void app_showbig2num(u8 *fontbase,u16 x,u16 y,u8 num,u8 size,u16 color,u16 bkcolor);
void app_show_nummid(u16 x,u16 y,u16 width,u16 height,u32 num,u8 len,u8 size,u16 ptcolor,u16 bkcolor);
void app_draw_smooth_line(u16 x,u16 y,u16 width,u16 height,u32 sergb,u32 mrgb);

u8 app_tp_is_in_area(_m_tp_dev *tp,u16 x,u16 y,u16 width,u16 height);
void app_show_items(u16 x,u16 y,u16 itemwidth,u16 itemheight,u8*name,u8*icopath,u16 color,u16 bkcolor);
u8 * app_get_icopath(u8 mode,u8 *selpath,u8 *unselpath,u8 selx,u8 index);
u8 app_items_sel(u16 x,u16 y,u16 width,u16 height,u8 *items[],u8 itemsize,u8 *selx,u8 mode,u8*caption);
u8 app_listbox_select(u8 *sel,u8 *top,u8 * caption,u8 *items[],u8 itemsize);
void app_show_mono_icos(u16 x,u16 y,u8 width,u8 height,u8 *icosbase,u16 color,u16 bkcolor);

u8 app_system_file_check(void);//ϵͳ�ļ����
u8 app_boot_cpdmsg(u8*pname,u8 pct,u8 mode);
void app_boot_cpdmsg_set(u16 x,u16 y);
u8 app_system_update(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode));
void app_getstm32_sn(u32 *sn0,u32 *sn1,u32 *sn2);
void app_get_version(u8*buf,u32 ver,u8 len);//�õ��汾��

void app_usmart_getsn(void);//USMARTר��.
u8 app_system_parameter_init(void);//ϵͳ��Ϣ��ʼ��
#endif




























































