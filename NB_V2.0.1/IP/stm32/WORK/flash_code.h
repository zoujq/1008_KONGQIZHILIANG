

#ifndef __FLASH_CODE_H__
#define __FLASH_CODE_H__


unsigned char UID_READ(void);

extern unsigned char CPU_ID[12]; //MCU ��96 λUID
extern unsigned char UID_KEY[12]; //������
extern unsigned char	f_Sys_Check;//ϵͳ��� ��־���������

unsigned char UID_READ(void);

#endif
