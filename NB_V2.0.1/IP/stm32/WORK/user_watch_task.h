#ifndef __USER_WATCH_TASK_H__
#define __USER_WATCH_TASK_H__
#include "includes.h"	   






typedef struct
{

	unsigned short UI_Show_No;//显示的界面号
	


	
	unsigned char	Key1_vale;
	unsigned char	Key2_vale;
	unsigned char	Key3_vale;		
	
	unsigned short temp;//温度
	unsigned short humidity;//湿度
	unsigned short hcho;//甲醛
	unsigned short tvoc;//TVOC
	unsigned short	co2;//CO2
	unsigned short	pm25;//

	
	unsigned char	fTemp_Correct;//温度较正标志
	char	 				temp_Correct_data;//温度
	
	unsigned char	fHumidity_Correct;//湿度较正标志
	char	 				humidity_Correct_data;//湿度	
	

	unsigned char	co2_show_flash;//CO2显示刷新
	unsigned char	pm25_show_flash;//PM2.5显示刷新	


	unsigned char	fRF_Mode;//RF发送模式，0正常1配对
	
	unsigned char	Battery;//电量	
	unsigned short	Battery_Low_Power_Off_No;//低电计数

	unsigned char	fCharge;//充电标志
	unsigned char	uWifi;//WIFI

	unsigned char	uLine_Work;//联网状态



	unsigned char	uBlink0;//闪烁0
	unsigned char	uBlink1;//闪烁0	
	unsigned char	uBlink2;//闪烁0		
	
	
	
	unsigned char uTVOC_Res_Start_Time_No;//重新上电时间计数
	
	
	unsigned char uCO2_Test_Mode;//进入较准模式


	unsigned char uID_Show_Time_No;//显示ID号
	unsigned char uID_Show_Time_No_B;//显示ID号

		
	unsigned char uLanguage_Mode;//进入较准模式		
	unsigned char uLanguage_Mode_B;//进入较准模式				
		

	unsigned char fLED_Back_B;//背光标志	,之前
	unsigned char fLED_Back;//背光标志	
	
	unsigned char uLED_Back_No_B;//背光号，	
	unsigned char uLED_Back_No;//背光号，		
	
	
	unsigned char uSys_User_Reset;//系统用户重置
	unsigned short uSys_User_Reset_Time;//系统用户重置,计时	
	unsigned short uSys_User_Reset_Time_LED_Blink_No;//系统用户重置,LED闪烁显示	
	
		
		
}Machine;
extern Machine CMachine;
extern Machine CMachine_B;//之前，刷新用





void SYS_POWER_OFF(void);//系统关闭

void user_watch_task(void *pdata);

#endif
