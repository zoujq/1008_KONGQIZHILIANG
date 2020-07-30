#ifndef __USER_WATCH_TASK_H__
#define __USER_WATCH_TASK_H__
#include "includes.h"	   






typedef struct
{

	unsigned short UI_Show_No;//��ʾ�Ľ����
	


	
	unsigned char	Key1_vale;
	unsigned char	Key2_vale;
	unsigned char	Key3_vale;		
	
	unsigned short temp;//�¶�
	unsigned short humidity;//ʪ��
	unsigned short hcho;//��ȩ
	unsigned short tvoc;//TVOC
	unsigned short	co2;//CO2
	unsigned short	pm25;//

	
	unsigned char	fTemp_Correct;//�¶Ƚ�����־
	char	 				temp_Correct_data;//�¶�
	
	unsigned char	fHumidity_Correct;//ʪ�Ƚ�����־
	char	 				humidity_Correct_data;//ʪ��	
	

	unsigned char	co2_show_flash;//CO2��ʾˢ��
	unsigned char	pm25_show_flash;//PM2.5��ʾˢ��	


	unsigned char	fRF_Mode;//RF����ģʽ��0����1���
	
	unsigned char	Battery;//����	
	unsigned short	Battery_Low_Power_Off_No;//�͵����

	unsigned char	fCharge;//����־
	unsigned char	uWifi;//WIFI

	unsigned char	uLine_Work;//����״̬



	unsigned char	uBlink0;//��˸0
	unsigned char	uBlink1;//��˸0	
	unsigned char	uBlink2;//��˸0		
	
	
	
	unsigned char uTVOC_Res_Start_Time_No;//�����ϵ�ʱ�����
	
	
	unsigned char uCO2_Test_Mode;//�����׼ģʽ


	unsigned char uID_Show_Time_No;//��ʾID��
	unsigned char uID_Show_Time_No_B;//��ʾID��

		
	unsigned char uLanguage_Mode;//�����׼ģʽ		
	unsigned char uLanguage_Mode_B;//�����׼ģʽ				
		

	unsigned char fLED_Back_B;//�����־	,֮ǰ
	unsigned char fLED_Back;//�����־	
	
	unsigned char uLED_Back_No_B;//����ţ�	
	unsigned char uLED_Back_No;//����ţ�		
	
	
	unsigned char uSys_User_Reset;//ϵͳ�û�����
	unsigned short uSys_User_Reset_Time;//ϵͳ�û�����,��ʱ	
	unsigned short uSys_User_Reset_Time_LED_Blink_No;//ϵͳ�û�����,LED��˸��ʾ	
	
		
		
}Machine;
extern Machine CMachine;
extern Machine CMachine_B;//֮ǰ��ˢ����





void SYS_POWER_OFF(void);//ϵͳ�ر�

void user_watch_task(void *pdata);

#endif
