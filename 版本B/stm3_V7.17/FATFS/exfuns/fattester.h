#ifndef __FATTESTER_H
#define __FATTESTER_H 			   
#include "sys.h"	   
#include "ff.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//FATFS ���Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/03
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.1�޸�˵�� 20121003
//1,����mf_cpymsg��mf_fdsize��mf_fcopy��mf_fdcopy�Ⱥ���				  									  
//////////////////////////////////////////////////////////////////////////////////

 
u8 mf_mount(u8 drv);
u8 mf_open(u8*path,u8 mode);
u8 mf_close(void);
u8 mf_read(u16 len);
u8 mf_write(u8*dat,u16 len);
u8 mf_opendir(u8* path);
u8 mf_readdir(void);
u8 mf_scan_files(u8 * path);
u32 mf_showfree(u8 *drv);
u8 mf_lseek(u32 offset);
u32 mf_tell(void);
u32 mf_size(void);
u8 mf_mkdir(u8*pname);
u8 mf_fmkfs(u8 drv,u8 mode,u16 au);
u8 mf_unlink(u8 *pname);
u8 mf_rename(u8 *oldname,u8* newname);
void mf_gets(u16 size);
u8 mf_putc(u8 c);
u8 mf_puts(u8*c);
u8 mf_cpymsg(u8*pname,u8 pct,u8 mode);
u32 mf_fdsize(u8*pname);
u8 mf_fcopy(u8 *psrc,u8 *pdst);
u8 mf_fdcopy(u8 *psrc,u8 *pdst);
#endif





























