#include "includes.h"




u8 uCharge_Blink_No = 0; //充电闪烁计数


u8 uBattery_Show = 0;

u8 uRF_Blink_No = 0; //RF闪烁计数



void User_Public_Show(void) { //公共显示部分




	/*
	if(CMachine.fRF_Mode==0)
	{
		icon_show(300, 15, 45, 30, gImage_RF4,1);//显示
		uRF_Blink_No=0;
	}
	else
	{
		uRF_Blink_No++;
		if(uRF_Blink_No>99)	uRF_Blink_No=0;
		if(uRF_Blink_No==1)		 icon_show(300, 15, 45, 30, gImage_RF1,1);//显示
		else if(uRF_Blink_No==25)		 icon_show(300, 15, 45, 30, gImage_RF2,1);//显示
		else if(uRF_Blink_No==50)		 icon_show(300, 15, 45, 30, gImage_RF3,1);//显示
		else if(uRF_Blink_No==75)		 icon_show(300, 15, 45, 30, gImage_RF4,1);//显示

	}
	*/

	if (CMachine.uLine_Work != 0) {
		icon_show(150, 15, 79, 29, gImage_NB_033_03, 1); //显示
		uRF_Blink_No = 0;
	} else {
		uRF_Blink_No++;
		if (uRF_Blink_No >= 75)	uRF_Blink_No = 0;
		if (uRF_Blink_No == 1)		 icon_show(150, 15, 79, 29, gImage_NB_031_03, 1); //显示
		else if (uRF_Blink_No == 25)		 icon_show(150, 15, 79, 29, gImage_NB_032_03, 1); //显示
		else if (uRF_Blink_No == 50)		 icon_show(150, 15, 79, 29, gImage_NB_033_03, 1); //显示


	}


	if (CMachine.uSys_User_Reset == 1) {

		CMachine.uSys_User_Reset_Time_LED_Blink_No++;
		if (CMachine.uSys_User_Reset_Time_LED_Blink_No > 80)	CMachine.uSys_User_Reset_Time_LED_Blink_No = 0;

		if (CMachine.uSys_User_Reset_Time_LED_Blink_No == 1)		 icon_show(36, 20, 60, 25, gImage_RHT_03, 1); //显示
		else	if (CMachine.uSys_User_Reset_Time_LED_Blink_No == 40)	 icon_show(36, 20, 60, 25, gImage_RHT_03, 0); //显示


	} else {
		CMachine.uSys_User_Reset_Time_LED_Blink_No = 0; //
		icon_show(36, 20, 60, 25, gImage_RHT_03, 1); //显示
	}





	if (CMachine.UI_Show_No != CMachine_B.UI_Show_No) {


		CMachine_B.UI_Show_No = CMachine.UI_Show_No;



// icon_show(36, 20, 60, 25, gImage_RHT_03,1);//显示


//icon_show(394, 16, 49, 26, gImage_battery_03,1);//显示

		//	icon_show(150, 15, 79, 29, gImage_NB_033_03,1);//显示




		uCharge_Blink_No = 1;

		uBattery_Show = 0xff;


		CMachine.Battery_Low_Power_Off_No = 0; //清零



	}

	if (CMachine.fCharge == 1) {

		uBattery_Show = 0xff; //刷新
		if (CMachine.Battery == 100) {
			if (uCharge_Blink_No != 0)	icon_show(394, 16, 49, 26, gImage_battery_03, 1); //显示
			uCharge_Blink_No = 0;
		} else {
			if (uCharge_Blink_No == 1) {
				icon_show(394, 16, 49, 26, gImage_battery_00, 1); //显示
			}
			if (uCharge_Blink_No == 20) {
				icon_show(394, 16, 49, 26, gImage_battery_01, 1); //显示
			}
			if (uCharge_Blink_No == 40) {
				icon_show(394, 16, 49, 26, gImage_battery_02, 1); //显示
			}
			if (uCharge_Blink_No == 60) {
				icon_show(394, 16, 49, 26, gImage_battery_03, 1); //显示
			}
			uCharge_Blink_No++;
			if (uCharge_Blink_No > 80)	uCharge_Blink_No = 1;
		}
	} else {

		//	if(fCharge_b==1)	 icon_show(394, 16, 49, 26, gImage_battery_03,1);//显示


		if (CMachine.Battery < 10) {
			if (uCharge_Blink_No == 1) {
				icon_show(394, 16, 49, 26, gImage_battery_00, 1); //显示
			}
			if (uCharge_Blink_No == 20) {
				icon_show(394, 16, 49, 26, gImage_battery_03, 0); //显示
			}
			if (uCharge_Blink_No == 40) {
				icon_show(394, 16, 49, 26, gImage_battery_00, 1); //显示
			}
			if (uCharge_Blink_No == 60) {
				icon_show(394, 16, 49, 26, gImage_battery_03, 0); //显示
			}
			uCharge_Blink_No++;
			if (uCharge_Blink_No > 80)	uCharge_Blink_No = 1;
			uBattery_Show = CMachine.Battery;

			if (CMachine.Battery == 0) {
				CMachine.Battery_Low_Power_Off_No++;//清零
				if (CMachine.Battery_Low_Power_Off_No >= 160) { //低电关机
					SYS_POWER_OFF();//系统关闭
				}
			}
		} else {
			if (uBattery_Show != CMachine.Battery) {
				uBattery_Show = CMachine.Battery;
				if (CMachine.Battery > 80) {
					icon_show(394, 16, 49, 26, gImage_battery_03, 1); //显示
				} else if (CMachine.Battery > 60) {
					icon_show(394, 16, 49, 26, gImage_battery_02, 1); //显示
				} else if (CMachine.Battery > 40) {
					icon_show(394, 16, 49, 26, gImage_battery_01, 1); //显示
				} else if (CMachine.Battery > 20) {
					icon_show(394, 16, 49, 26, gImage_battery_00, 1); //显示
				}
			}
		}





	}










}



unsigned short UI_Show_No_B_Show_Task = 0xff; //

void User_Menu_Show_Task(void *pdata) { //菜单图标显示任务
//	dprintf_WORD16_16(192,20,"轻度污染",3,0);
	while (1) {

		if (CMachine.UI_Show_No < 1) {

			if (UI_Show_No_B_Show_Task != 0) {
				LCD_UnInit();
				UI_Show_No_B_Show_Task = 0;
			}

			delay_ms(10);
			continue;//退出


		} else {
			if (UI_Show_No_B_Show_Task == 0) {
				LCD_Init();
				//CMachine.UI_Show_No=2;//
				CMachine_B.UI_Show_No = 0xff; //

				UI_Show_No_B_Show_Task = 0xff;
			}

		}
		printf("CMachine.UI_Show_No=%d\n\r",CMachine.UI_Show_No);
		switch (CMachine.UI_Show_No) {
			
		case 0:

			break;
		case 1:
			Charge_Off_Show();//关机充电界面显示
			break;
		case 2:
			All_Show();//所有参数界面显示

			break;
		case 3:

			PM25_Show();//PM2.5界面显示
			break;
		case 4:

			TVOC_Show();//TVOC界面显示
			break;
		case 5:

			CO2_Show();//TVOC界面显示
			break;
		case 6:
			HCHO_Show();//甲醛界面显示

			break;
		case 7:
			Time_Show();//时间界面显示

			break;
		default:
			break;
		}
		delay_ms(20);

	}











}

























