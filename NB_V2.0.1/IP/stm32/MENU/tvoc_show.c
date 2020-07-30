#include "includes.h"





void TVOC_Show(void) { //TVOC界面显示
	LCD_Clear(BLACK);

	POINT_COLOR = WHITE;
	BACK_COLOR = BLACK;

	icon_show(134, 78, 109, 31, gImage_tvocb_03, 1); //显示



	icon_show(393, 194, 37, 51, gImage_humy_03, 1); //显示
	icon_show(395, 64, 27, 57, gImage_temp_03, 1); //显示

	icon_show(440, 160, 18, 16, gImage_tempc_03, 1); //显示

	icon_show(440, 291, 17, 16, gImage_humyc_03, 1); //显示





	LCD_DrawLine(356, 54, 356, 304);





	LCD_Fill(110, 208, 110 + 12, 208 + 12, WHITE);	//TVOC的小数点




	icon_show(235, 263, 107, 36, gImage_mgm3b_03, 1); //显示



	CMachine.uLanguage_Mode_B = 0xff;

	CMachine_B.temp = 0xff;
	CMachine_B.humidity = 0xff;
	CMachine_B.hcho = 0xffff;
	CMachine_B.tvoc = 0xffff;
	CMachine_B.co2 = 0xffff;
	CMachine_B.pm25 = 0xffff;






	// while (CPU_ID[1] == UID_KEY[1]) {
	while (f_Sys_Check == 2) {

		if (CMachine.UI_Show_No != 4)	break;

		if (CMachine.Key2_vale == 1) {	//IN键按下
			CMachine.Key2_vale = 0; //清零
			CMachine.UI_Show_No = 5; //进入下一界面
			break;//下一页，退出
		}
		User_Public_Show();//公共显示部分


		if (CMachine.uLanguage_Mode_B != CMachine.uLanguage_Mode) {
			CMachine.uLanguage_Mode_B = CMachine.uLanguage_Mode;

			CMachine_B.temp = 0xff;
			CMachine_B.humidity = 0xff;
			CMachine_B.hcho = 0xffff;
			CMachine_B.tvoc = 0xffff;
			CMachine_B.co2 = 0xffff;
			CMachine_B.pm25 = 0xffff;


		}
////////////////////////////////////////////////////////////////////////// TVOC         ///////////////////////////////////////////////
		if (CMachine_B.tvoc != CMachine.tvoc) {
			CMachine_B.tvoc = CMachine.tvoc;
			POINT_COLOR = WHITE;
			BACK_COLOR = BLACK;

			Show_NUM100_72(37, 128, CMachine.tvoc / 1000, 1);
			Show_NUM100_72(129, 128, CMachine.tvoc % 1000 / 100, 1);
			Show_NUM100_72(129 + 72, 128, CMachine.tvoc % 100 / 10, 1);
			Show_NUM100_72(129 + 144, 128, CMachine.tvoc % 10, 1);

			if (CMachine.tvoc <= 400) {
				if (CMachine.uLanguage_Mode == 0) icon_show(68, 265, 154, 30, gImage_excellentb_03, 1); //
				else icon_show(68, 265, 154, 30, gImage_excellentbEG_03, 1); //
			} else if (CMachine.tvoc <= 600) {
				if (CMachine.uLanguage_Mode == 0) icon_show(68, 265, 154, 30, gImage_goodb_03, 1); //
				else icon_show(68, 265, 154, 30, gImage_goodbEG_03, 1); //
			} else if (CMachine.tvoc <= 1000) {
				if (CMachine.uLanguage_Mode == 0) icon_show(68, 265, 154, 30, gImage_moderateb_03, 1); //
				else icon_show(68, 265, 154, 30, gImage_moderatebEG_03, 1); //
			} else {
				if (CMachine.uLanguage_Mode == 0) icon_show(68, 265, 154, 30, gImage_poorb_03, 1); //
				else icon_show(68, 265, 154, 30, gImage_poorbEG_03, 1); //
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




