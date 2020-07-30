

#ifndef __FLASH_CODE_H__
#define __FLASH_CODE_H__


unsigned char UID_READ(void);

extern unsigned char CPU_ID[12]; //MCU 的96 位UID
extern unsigned char UID_KEY[12]; //运算结果
extern unsigned char	f_Sys_Check;//系统检测 标志，制造错误

unsigned char UID_READ(void);

#endif
