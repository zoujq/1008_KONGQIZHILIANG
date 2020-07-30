#ifndef __USER_DATA_SAVE_H__
#define __USER_DATA_SAVE_H__
#include "includes.h"	   





void User_Sys_Data_Get(void);



#define	DATA_SAVE_DOUBLE_ADD_ADDR	0x80  //数据双重存储地址//一半


#define SYS_LANGUAGE_ADDR	0	//语言版本保存地址


#define SYS_LOGO_ADDR	5	//图标版本保存地址



#define SYS_BACK_LIGHT_ADDR	10	//背光状态保存地址


#define SYS_TEMP_CORRECT_ADDR	20	//温度较正值保存地址

#define SYS_HUMIDITY_CORRECT_ADDR	30	//湿度较正值保存地址





#endif
