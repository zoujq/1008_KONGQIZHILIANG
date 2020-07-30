#include "includes.h"




u8 uCO2_Correct_Blink_No = 0; //CO2较准闪烁计数


void CO2_Show(void) { //TVOC界面显示
	LCD_Clear(BLACK);

	POINT_COLOR = WHITE;
	BACK_COLOR = BLACK;





	icon_show(393, 194, 37, 51, gImage_humy_03, 1); //显示
	icon_show(395, 64, 27, 57, gImage_temp_03, 1); //显示

	icon_show(440, 160, 18, 16, gImage_tempc_03, 1); //显示

	icon_show(440, 291, 17, 16, gImage_humyc_03, 1); //显示




	LCD_DrawLine(356, 54, 356, 304);






	icon_show(235, 268, 70, 28, gImage_ppmb_03, 1); //显示



	CMachine.uLanguage_Mode_B = 0xff;

	CMachine_B.temp = 0xff;
	CMachine_B.humidity = 0xff;
	CMachine_B.hcho = 0xffff;
	CMachine_B.tvoc = 0xffff;
	CMachine_B.co2 = 0xffff;
	CMachine_B.pm25 = 0xffff;


	CMachine.co2_show_flash = 0xff;

	uCO2_Correct_Blink_No = 0; //


	// while (CPU_ID[0] == UID_KEY[0]) {
	while (f_Sys_Check == 2) {
		if (CMachine.UI_Show_No != 5)	break;

		if (CMachine.Key2_vale == 1) {	//IN键按下
			CMachine.Key2_vale = 0; //清零
			CMachine.UI_Show_No = 6; //进入下一界面
			break;//下一页，退出
		}


		User_Public_Show();//公共显示部分



		if (CMachine.uLanguage_Mode_B != CMachine.uLanguage_Mode) {
			CMachine.uLanguage_Mode_B = CMachine.uLanguage_Mode;

			if (CMachine.uLanguage_Mode == 0)	icon_show(107, 77, 124, 35, gImage_co2b_03, 1); //显示
			else	icon_show(107, 77, 124, 35, gImage_co2b_03EG, 1); //显示


			CMachine_B.temp = 0xff;
			CMachine_B.humidity = 0xff;
			CMachine_B.hcho = 0xffff;
			CMachine_B.tvoc = 0xffff;
			CMachine_B.co2 = 0xffff;
			CMachine_B.pm25 = 0xffff;


			CMachine.co2_show_flash = 0xff;

			uCO2_Correct_Blink_No = 0; //



		}


////////////////////////////////////////////////////////////////////////// CO2        ///////////////////////////////////////////////


		if (CMachine.uCO2_Test_Mode != 0) {


			if (uCO2_Correct_Blink_No == 0)		LCD_Fill(37, 128, 37 + 260, 128 + 90, BLACK);	//位数有变，清底色
			if (uCO2_Correct_Blink_No == 20) {
				POINT_COLOR = WHITE;
				BACK_COLOR = BLACK;

				if (CMachine.co2 > 999) {
					Show_NUM100_72(37, 128, CMachine.co2 / 1000, 1);
					Show_NUM100_72(37 + 72, 128, CMachine.co2 % 1000 / 100, 1);
					Show_NUM100_72(37 + 144, 128, CMachine.co2 % 100 / 10, 1);
					Show_NUM100_72(37 + 216, 128, CMachine.co2 % 10, 1);
				} else if (CMachine.co2 > 99) {
					Show_NUM100_72(37 + 32, 128, CMachine.co2 % 1000 / 100, 1);
					Show_NUM100_72(37 + 72 + 32, 128, CMachine.co2 % 100 / 10, 1);
					Show_NUM100_72(37 + 144 + 32, 128, CMachine.co2 % 10, 1);
				} else if (CMachine.co2 > 9) {
					Show_NUM100_72(37 + 72, 128, CMachine.co2 % 100 / 10, 1);
					Show_NUM100_72(37 + 144, 128, CMachine.co2 % 10, 1);
				} else {
					Show_NUM100_72(37 + 72 + 32, 128, CMachine.co2 % 10, 1);
				}

			}




			uCO2_Correct_Blink_No++;//
			if (uCO2_Correct_Blink_No > 39)	uCO2_Correct_Blink_No = 0;

			CMachine_B.co2 = 0xffff;
			CMachine.co2_show_flash = 0xff;
		} else {
			uCO2_Correct_Blink_No = 0; //
			if (CMachine_B.co2 != CMachine.co2) {
				CMachine_B.co2 = CMachine.co2;
				POINT_COLOR = WHITE;
				BACK_COLOR = BLACK;

				if (CMachine.co2 > 999) {
					if (CMachine.pm25_show_flash != 3)					LCD_Fill(37, 128, 37 + 260, 128 + 90, BLACK);	//位数有变，清底色
					Show_NUM100_72(37, 128, CMachine.co2 / 1000, 1);
					Show_NUM100_72(37 + 72, 128, CMachine.co2 % 1000 / 100, 1);
					Show_NUM100_72(37 + 144, 128, CMachine.co2 % 100 / 10, 1);
					Show_NUM100_72(37 + 216, 128, CMachine.co2 % 10, 1);
					CMachine.pm25_show_flash = 3;
				} else if (CMachine.co2 > 99) {
					if (CMachine.pm25_show_flash != 2)					LCD_Fill(37, 128, 37 + 260, 128 + 90, BLACK);	//位数有变，清底色
					Show_NUM100_72(37 + 32, 128, CMachine.co2 % 1000 / 100, 1);
					Show_NUM100_72(37 + 72 + 32, 128, CMachine.co2 % 100 / 10, 1);
					Show_NUM100_72(37 + 144 + 32, 128, CMachine.co2 % 10, 1);
					CMachine.pm25_show_flash = 2;
				} else if (CMachine.co2 > 9) {
					if (CMachine.pm25_show_flash != 1)					LCD_Fill(37, 128, 37 + 260, 128 + 90, BLACK);	//位数有变，清底色
					Show_NUM100_72(37 + 72, 128, CMachine.co2 % 100 / 10, 1);
					Show_NUM100_72(37 + 144, 128, CMachine.co2 % 10, 1);
					CMachine.pm25_show_flash = 1;
				} else {
					if (CMachine.pm25_show_flash != 0)					LCD_Fill(37, 128, 37 + 260, 128 + 90, BLACK);	//位数有变，清底色
					Show_NUM100_72(37 + 72 + 32, 128, CMachine.co2 % 10, 1);
					CMachine.pm25_show_flash = 0;
				}


				if (CMachine.co2 <= 700) {
					if (CMachine.uLanguage_Mode == 0) icon_show(68, 265, 154, 30, gImage_excellentb_03, 1); //
					else icon_show(68, 265, 154, 30, gImage_excellentbEG_03, 1); //
				} else if (CMachine.co2 <= 1000) {
					if (CMachine.uLanguage_Mode == 0) icon_show(68, 265, 154, 30, gImage_goodb_03, 1); //
					else icon_show(68, 265, 154, 30, gImage_goodbEG_03, 1); //
				} else if (CMachine.co2 <= 1500) {
					if (CMachine.uLanguage_Mode == 0) icon_show(68, 265, 154, 30, gImage_moderateb_03, 1); //
					else icon_show(68, 265, 154, 30, gImage_moderatebEG_03, 1); //
				} else {
					if (CMachine.uLanguage_Mode == 0) icon_show(68, 265, 154, 30, gImage_poorb_03, 1); //
					else icon_show(68, 265, 154, 30, gImage_poorbEG_03, 1); //
				}
			}
		}


//////////////////////////////////////////////////////////////////////////温度       ///////////////////////////////////////////////
		if (CMachine_B.temp != CMachine.temp) {
			CMachine_B.temp = CMachine.temp;
			POINT_COLOR = WHITE;
			BACK_COLOR = BLACK;

			if (CMachine.temp <= 30) {
				Show_NUM44_32(374, 126, (40 - CMachine.temp) % 100 / 10, 1);
				Show_NUM44_32(374 + 32, 126, (40 - CMachine.temp) % 10, 1);
				LCD_Fill(362, 146, 372, 150, WHITE);

			} else if (CMachine.temp < 40) {
				LCD_Fill(374, 126, 406, 170, BLACK);
				LCD_Fill(362, 146, 372, 150, BLACK);

				Show_NUM44_32(374 + 32, 126, (40 - CMachine.temp) % 10, 1);
				LCD_Fill(382, 146, 392, 150, WHITE);
			} else if (CMachine.temp < 50) {
				LCD_Fill(374, 126, 406, 170, BLACK);
				LCD_Fill(362, 146, 372, 150, BLACK);

				Show_NUM44_32(374 + 32, 126, (CMachine.temp - 40) % 10, 1);
			} else {
				LCD_Fill(362, 146, 372, 150, BLACK);

				Show_NUM44_32(374, 126, (CMachine.temp - 40) % 100 / 10, 1);
				Show_NUM44_32(374 + 32, 126, (CMachine.temp - 40) % 10, 1);
			}
		}
//////////////////////////////////////////////////////////////////////////湿度        ///////////////////////////////////////////////
		if (CMachine_B.humidity != CMachine.humidity) {
			CMachine_B.humidity = CMachine.humidity;
			POINT_COLOR = WHITE;
			BACK_COLOR = BLACK;
			Show_NUM44_32(374, 255, CMachine.humidity % 100 / 10, 1);
			Show_NUM44_32(374 + 32, 255, CMachine.humidity % 10, 1);
		}




		delay_ms(20);
	}











}




