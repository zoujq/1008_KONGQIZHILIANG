#include "includes.h"




u8 uCharge_Blink_No = 0; //充电闪烁计数


u8 uBattery_Show = 0;

u8 uRF_Blink_No = 0; //RF闪烁计数

extern void icon_show_monochrome(unsigned short  x, unsigned short  y, unsigned short  width, unsigned short  height, const unsigned char* pstr,u16 colors);
void User_Public_Show(void) { //公共显示部分



//logo
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


	
//show logo

//	if (CMachine.uSys_User_Reset == 1) {

//		CMachine.uSys_User_Reset_Time_LED_Blink_No++;
//		if (CMachine.uSys_User_Reset_Time_LED_Blink_No > 80)	CMachine.uSys_User_Reset_Time_LED_Blink_No = 0;

//		if (CMachine.uSys_User_Reset_Time_LED_Blink_No == 1)		 icon_show(36, 20, 60, 25, gImage_RHT_03, 1); //显示
//		else	if (CMachine.uSys_User_Reset_Time_LED_Blink_No == 40)	 icon_show(36, 20, 60, 25, gImage_RHT_03, 0); //显示


//	} else {
//		CMachine.uSys_User_Reset_Time_LED_Blink_No = 0; //
//		icon_show(36, 20, 60, 25, gImage_RHT_03, 1); //显示
//	}
	

//start logo my
	icon_show_monochrome(36,20,32,32,gImage_zhongguo_df,0xffff);

	LCD_ShowChar(140, 300,'W',16,0);
	LCD_ShowChar(150, 300,'W',16,0);
	LCD_ShowChar(160, 300,'W',16,0);
	LCD_ShowChar(170, 300,'.',16,0);
	LCD_ShowChar(180, 300,'H',16,0);
	LCD_ShowChar(190, 300,'U',16,0);
	LCD_ShowChar(200, 300,'O',16,0);
	LCD_ShowChar(210, 300,'T',16,0);
	LCD_ShowChar(220, 300,'I',16,0);
	LCD_ShowChar(230, 300,'A',16,0);
	LCD_ShowChar(240, 300,'N',16,0);
	LCD_ShowChar(250, 300,'T',16,0);
	LCD_ShowChar(260, 300,'E',16,0);
	LCD_ShowChar(270, 300,'C',16,0);
	LCD_ShowChar(280, 300,'H',16,0);
	LCD_ShowChar(290, 300,'.',16,0);
	LCD_ShowChar(300, 300,'C',16,0);
	LCD_ShowChar(310, 300,'O',16,0);
	LCD_ShowChar(320, 300,'M',16,0);
	
//end logo my	



	if (CMachine.UI_Show_No != CMachine_B.UI_Show_No) {


		CMachine_B.UI_Show_No = CMachine.UI_Show_No;

		uCharge_Blink_No = 1;
		uBattery_Show = 0xff;
		CMachine.Battery_Low_Power_Off_No = 0; //清零
	}
	

	if (CMachine.fCharge == 1) {

		uBattery_Show = 0xff; //刷新
		if (CMachine.Battery == 100) {
			if (uCharge_Blink_No != 0)	icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_03_z,0xffff); //显示
			uCharge_Blink_No = 0;
		} else {
			if (uCharge_Blink_No == 1) {
				icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_00_z,0xffff);
			}
			if (uCharge_Blink_No == 20) {
				icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_01_z,0xffff);
			}
			if (uCharge_Blink_No == 40) {
				icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_02_z,0xffff);
			}
			if (uCharge_Blink_No == 60) {
				icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_03_z,0xffff);
			}
			uCharge_Blink_No++;
			if (uCharge_Blink_No > 80)	uCharge_Blink_No = 1;
		}
	} else {

		//	if(fCharge_b==1)	 icon_show(394, 16, 49, 26, gImage_battery_03,1);//显示


		if (CMachine.Battery < 10) {
			if (uCharge_Blink_No == 1) {
				icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_00_z,0xffff);
			}
			if (uCharge_Blink_No == 20) {
				icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_01_z,0xffff);
			}
			if (uCharge_Blink_No == 40) {
				icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_02_z,0xffff);
			}
			if (uCharge_Blink_No == 60) {
				icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_03_z,0xffff);
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
		} 
		else 
		{
			if (uBattery_Show != CMachine.Battery) {
				uBattery_Show = CMachine.Battery;
				if (CMachine.Battery > 80) {
					icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_03_z,0xffff);
				} else if (CMachine.Battery > 60) {
					icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_02_z,0xffff);
				} else if (CMachine.Battery > 40) {
					icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_01_z,0xffff);
				} else if (CMachine.Battery > 20) {
					icon_show_monochrome(394,16,0X30,0X1A,gImage_battery_00_z,0xffff);
				}
			}
		}





	}










}


extern void User_ID_Show(void);
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
		case 8:
			
			User_ID_Show();//QR_CODE显示

			break;
		default:
			break;
		}
		delay_ms(20);

	}











}

























