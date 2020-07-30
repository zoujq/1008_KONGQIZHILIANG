#ifndef __TIME_SHOW_H__
#define __TIME_SHOW_H__


extern _calendar_obj Sys_Time_Save;//时钟结构体 
extern _calendar_obj Sys_Time_WIFI;//网络获得的时间


extern u8 uTime_Set_Mode;//时间设置模式



void Sys_RTC_Init(void);//实时时钟初始化

void Time_Show(void);//时间界面显示


#endif
