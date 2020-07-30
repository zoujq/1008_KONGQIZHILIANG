#include "includes.h"





char All_Show_Thread(thread_t* pt) { //所有参数界面显示


	Thread_BEGIN {




		while (f_Sys_Check == 2) {

			if (CMachine.UI_Show_No == 2) {



				if (CMachine_B.UI_Show_No != CMachine.UI_Show_No) {

					LCD_Clear(BLACK);




					icon_show(357, 206, 59, 16, gImage_CO2_03, 1); //显示
					icon_show(60, 77, 44, 16, gImage_PM25_03, 1); //显示
					icon_show(206, 76, 52, 20, gImage_HCHO_03, 1); //显示
					icon_show(366, 77, 50, 18, gImage_TVOC_03, 1); //显示

					icon_show(177, 224, 37, 51, gImage_humy_03, 1); //显示
					icon_show(23, 221, 27, 57, gImage_temp_03, 1); //显示

					icon_show(130, 264, 18, 16, gImage_tempc_03, 1); //显示

					icon_show(284, 262, 17, 16, gImage_humyc_03, 1); //显示

					icon_show(419, 276, 35, 15, gImage_ppm_03, 1); //显示


					icon_show(413, 164, 52, 18, gImage_mgm3_03, 1); //显示
					icon_show(255, 164, 52, 18, gImage_mgm3_03, 1); //显示

					icon_show(101, 164, 46, 18, gImage_ugm3_03, 1); //显示


					LCD_DrawDottedLine(16, 190, 464, 190);
					LCD_DrawDottedLine(150, 72, 150, 182);
					LCD_DrawDottedLine(312, 72, 312, 182);
					LCD_DrawDottedLine(150, 204, 150, 292);
					LCD_DrawDottedLine(312, 204, 312, 292);



					POINT_COLOR = WHITE;
					BACK_COLOR = BLACK;



					LCD_Fill(159 + 32 + 4, 104 + 35, 159 + 32 + 4 + 4, 104 + 35 + 4, WHITE);	//甲醛的小数点




					LCD_Fill(162 + 162 + 32 + 4, 104 + 35, 162 + 162 + 32 + 4 + 4, 104 + 35 + 4, WHITE);	//TVOC的小数点












					//dprintf_NUM16_16(200,161,"良",3,0);





					CMachine.uLanguage_Mode_B = 0xff;


					CMachine_B.temp = 0xff;
					CMachine_B.humidity = 0xff;
					CMachine_B.hcho = 0xffff;
					CMachine_B.tvoc = 0xffff;
					CMachine_B.co2 = 0xffff;
					CMachine_B.pm25 = 0xffff;


					CMachine.co2_show_flash = 0xff;
					CMachine.pm25_show_flash = 0xff;

					/*
					CMachine.pm25=100;
					CMachine.hcho=123;
					CMachine.tvoc=889;
					CMachine.co2=900;
					CMachine.temp=32;
					CMachine.humidity=56;

					*/

					CMachine.fTemp_Correct = 0; //温度较正标志
					CMachine.fHumidity_Correct = 0; //湿度较正标志

				}


				if (CMachine.Key2_vale == 1) {	//IN键按下
					CMachine.Key2_vale = 0; //清零
					CMachine.UI_Show_No = 3; //进入下一界面
					continue;//下一页，退出
				}
				User_Public_Show();//公共显示部分




				if (CMachine.uID_Show_Time_No > 0) {



					User_ID_Show();//ID显示




				} else {



					if (CMachine.uID_Show_Time_No_B != 0) {
						CMachine.uID_Show_Time_No_B = 0;

						CMachine_B.UI_Show_No = 0xff;

						continue;//退出界面重新刷新

					}


					if (CMachine.uLanguage_Mode_B != CMachine.uLanguage_Mode) {
						CMachine.uLanguage_Mode_B = CMachine.uLanguage_Mode;

						if (CMachine.uLanguage_Mode == 0) {
							icon_show(357, 206, 59, 16, gImage_CO2_03, 1); //显示
							icon_show(206, 76, 52, 20, gImage_HCHO_03, 1); //显示

							CMachine_B.temp = 0xff;
							CMachine_B.humidity = 0xff;
							CMachine_B.hcho = 0xffff;
							CMachine_B.tvoc = 0xffff;
							CMachine_B.co2 = 0xffff;
							CMachine_B.pm25 = 0xffff;


							CMachine.co2_show_flash = 0xff;
							CMachine.pm25_show_flash = 0xff;
						} else {
							icon_show(357, 206, 59, 16, gImage_CO2_03EG, 1); //显示
							icon_show(206, 76, 52, 20, gImage_HCHO_03EG, 1); //显示

							CMachine_B.temp = 0xff;
							CMachine_B.humidity = 0xff;
							CMachine_B.hcho = 0xffff;
							CMachine_B.tvoc = 0xffff;
							CMachine_B.co2 = 0xffff;
							CMachine_B.pm25 = 0xffff;


							CMachine.co2_show_flash = 0xff;
							CMachine.pm25_show_flash = 0xff;
						}

					}

////////////////////////////////////////////////////////////////////////// PM2.5         ///////////////////////////////////////////////
					if (CMachine_B.pm25 != CMachine.pm25) {
						CMachine_B.pm25 = CMachine.pm25;
						POINT_COLOR = WHITE;
						BACK_COLOR = BLACK;


						if (CMachine.pm25 > 999) {
							if (CMachine.pm25_show_flash != 3)					LCD_Fill(14, 104, 14 + 128, 104 + 45, BLACK);	//位数有变，清底色
							Show_NUM44_32(14, 104, CMachine.pm25 / 1000, 1);
							Show_NUM44_32(14 + 32, 104, CMachine.pm25 % 1000 / 100, 1);
							Show_NUM44_32(14 + 64, 104, CMachine.pm25 % 100 / 10, 1);
							Show_NUM44_32(14 + 96, 104, CMachine.pm25 % 10, 1);
							CMachine.pm25_show_flash = 3;
						} else if (CMachine.pm25 > 99) {
							if (CMachine.pm25_show_flash != 2)					LCD_Fill(14, 104, 14 + 128, 104 + 45, BLACK);	//位数有变，清底色
							Show_NUM44_32(30, 104, CMachine.pm25 % 1000 / 100, 1);
							Show_NUM44_32(62, 104, CMachine.pm25 % 100 / 10, 1);
							Show_NUM44_32(94, 104, CMachine.pm25 % 10, 1);
							CMachine.pm25_show_flash = 2;
						} else if (CMachine.pm25 > 9) {
							if (CMachine.pm25_show_flash != 1)					LCD_Fill(14, 104, 14 + 128, 104 + 45, BLACK);	//位数有变，清底色
							Show_NUM44_32(14 + 32, 104, CMachine.pm25 % 100 / 10, 1);
							Show_NUM44_32(14 + 64, 104, CMachine.pm25 % 10, 1);
							CMachine.pm25_show_flash = 1;
						} else {
							if (CMachine.pm25_show_flash != 0)					LCD_Fill(14, 104, 14 + 128, 104 + 45, BLACK);	//位数有变，清底色
							Show_NUM44_32(14 + 16 + 32, 104, CMachine.pm25 % 10, 1);
							CMachine.pm25_show_flash = 0;
						}


						if (CMachine.pm25 <= 35) {
							if (CMachine.uLanguage_Mode == 0) icon_show(16, 165, 82, 16, gImage_excellent_03, 1); //
							else icon_show(16, 165, 82, 16, gImage_excellent_03EG, 1); //
						} else if (CMachine.pm25 <= 75) {
							if (CMachine.uLanguage_Mode == 0) icon_show(16, 165, 82, 16, gImage_good_03, 1); //
							else icon_show(16, 165, 82, 16, gImage_good_03EG, 1); //
						} else if (CMachine.pm25 <= 115) {
							if (CMachine.uLanguage_Mode == 0) icon_show(16, 165, 82, 16, gImage_moderate_03, 1); //
							else icon_show(16, 165, 82, 16, gImage_moderate_03EG, 1); //
						} else {
							if (CMachine.uLanguage_Mode == 0) icon_show(16, 165, 82, 16, gImage_poor_03, 1); //
							else icon_show(16, 165, 82, 16, gImage_poor_03EG, 1); //
						}
					}
////////////////////////////////////////////////////////////////////////// 甲醛         ///////////////////////////////////////////////
					if (CMachine_B.hcho != CMachine.hcho) {
						CMachine_B.hcho = CMachine.hcho;
						POINT_COLOR = WHITE;
						BACK_COLOR = BLACK;
						Show_NUM44_32(162, 104, CMachine.hcho / 1000, 1);
						Show_NUM44_32(203, 104, CMachine.hcho % 1000 / 100, 1);
						Show_NUM44_32(203 + 32, 104, CMachine.hcho % 100 / 10, 1);
						Show_NUM44_32(203 + 64, 104, CMachine.hcho % 10, 1);

						if (CMachine.hcho <= 50) {
							if (CMachine.uLanguage_Mode == 0) icon_show(163, 165, 82, 16, gImage_excellent_03, 1); //
							else icon_show(163, 165, 82, 16, gImage_excellent_03EG, 1); //
						} else if (CMachine.hcho <= 100) {
							if (CMachine.uLanguage_Mode == 0) icon_show(163, 165, 82, 16, gImage_good_03, 1); //
							else icon_show(163, 165, 82, 16, gImage_good_03EG, 1); //
						} else if (CMachine.hcho <= 200) {
							if (CMachine.uLanguage_Mode == 0) icon_show(163, 165, 82, 16, gImage_moderate_03, 1); //
							else icon_show(163, 165, 82, 16, gImage_moderate_03EG, 1); //
						} else {
							if (CMachine.uLanguage_Mode == 0) icon_show(163, 165, 82, 16, gImage_poor_03, 1); //
							else icon_show(163, 165, 82, 16, gImage_poor_03EG, 1); //
						}
					}

////////////////////////////////////////////////////////////////////////// CO2        ///////////////////////////////////////////////
					if (CMachine_B.tvoc != CMachine.tvoc) {
						CMachine_B.tvoc = CMachine.tvoc;
						POINT_COLOR = WHITE;
						BACK_COLOR = BLACK;

						Show_NUM44_32(324, 104, CMachine.tvoc / 1000, 1);
						Show_NUM44_32(369, 104, CMachine.tvoc % 1000 / 100, 1);
						Show_NUM44_32(369 + 32, 104, CMachine.tvoc % 100 / 10, 1);
						Show_NUM44_32(369 + 64, 104, CMachine.tvoc % 10, 1);


						if (CMachine.tvoc <= 400) {
							if (CMachine.uLanguage_Mode == 0) icon_show(325, 165, 82, 16, gImage_excellent_03, 1); //
							else icon_show(325, 165, 82, 16, gImage_excellent_03EG, 1); //
						} else if (CMachine.tvoc <= 600) {
							if (CMachine.uLanguage_Mode == 0) icon_show(325, 165, 82, 16, gImage_good_03, 1); //
							else icon_show(325, 165, 82, 16, gImage_good_03EG, 1); //
						} else if (CMachine.tvoc <= 1000) {
							if (CMachine.uLanguage_Mode == 0) icon_show(325, 165, 82, 16, gImage_moderate_03, 1); //
							else icon_show(325, 165, 82, 16, gImage_moderate_03EG, 1); //
						} else {
							if (CMachine.uLanguage_Mode == 0) icon_show(325, 165, 82, 16, gImage_poor_03, 1); //
							else icon_show(325, 165, 82, 16, gImage_poor_03EG, 1); //
						}
					}
////////////////////////////////////////////////////////////////////////// TVOC         ///////////////////////////////////////////////
					if (CMachine_B.co2 != CMachine.co2) {
						CMachine_B.co2 = CMachine.co2;
						POINT_COLOR = WHITE;
						BACK_COLOR = BLACK;

						if (CMachine.co2 > 999) {
							if (CMachine.co2_show_flash != 3)					LCD_Fill(329, 227, 329 + 128, 227 + 45, BLACK);	//位数有变，清底色
							Show_NUM44_32(329, 227, CMachine.co2 / 1000, 1);
							Show_NUM44_32(329 + 32, 227, CMachine.co2 % 1000 / 100, 1);
							Show_NUM44_32(329 + 64, 227, CMachine.co2 % 100 / 10, 1);
							Show_NUM44_32(329 + 96, 227, CMachine.co2 % 10, 1);
							CMachine.co2_show_flash = 3;
						} else if (CMachine.co2 > 99) {
							if (CMachine.co2_show_flash != 2)					LCD_Fill(329, 227, 329 + 128, 227 + 45, BLACK);	//位数有变，清底色
							Show_NUM44_32(329 + 16, 227, CMachine.co2 % 1000 / 100, 1);
							Show_NUM44_32(329 + 16 + 32, 227, CMachine.co2 % 100 / 10, 1);
							Show_NUM44_32(329 + 16 + 64, 227, CMachine.co2 % 10, 1);
							CMachine.co2_show_flash = 2;
						} else if (CMachine.co2 > 9) {
							if (CMachine.co2_show_flash != 1)					LCD_Fill(329, 227, 329 + 128, 227 + 45, BLACK);	//位数有变，清底色
							Show_NUM44_32(329 + 32, 227, CMachine.co2 % 100 / 10, 1);
							Show_NUM44_32(329 + 64, 227, CMachine.co2 % 10, 1);
							CMachine.co2_show_flash = 1;
						} else {
							if (CMachine.co2_show_flash != 0)					LCD_Fill(329, 227, 329 + 128, 227 + 45, BLACK);	//位数有变，清底色
							Show_NUM44_32(329 + 16 + 32, 227, CMachine.co2 % 10, 1);
							CMachine.co2_show_flash = 0;
						}


						if (CMachine.co2 <= 700) {
							if (CMachine.uLanguage_Mode == 0) icon_show(325, 276, 82, 16, gImage_excellent_03, 1); //
							else icon_show(325, 276, 82, 16, gImage_excellent_03EG, 1); //
						} else if (CMachine.co2 <= 1000) {
							if (CMachine.uLanguage_Mode == 0) icon_show(325, 276, 82, 16, gImage_good_03, 1); //
							else icon_show(325, 276, 82, 16, gImage_good_03EG, 1); //
						} else if (CMachine.co2 <= 1500) {
							if (CMachine.uLanguage_Mode == 0) icon_show(325, 276, 82, 16, gImage_moderate_03, 1); //
							else icon_show(325, 276, 82, 16, gImage_moderate_03EG, 1); //
						} else {
							if (CMachine.uLanguage_Mode == 0) icon_show(325, 276, 82, 16, gImage_poor_03, 1); //
							else icon_show(325, 276, 82, 16, gImage_poor_03EG, 1); //
						}
					}
//////////////////////////////////////////////////////////////////////////温度       ///////////////////////////////////////////////
					if (CMachine_B.temp != CMachine.temp) {
						CMachine_B.temp = CMachine.temp;
						POINT_COLOR = WHITE;
						BACK_COLOR = BLACK;

						if (CMachine.temp <= 30) {
							Show_NUM44_32(62, 227, (40 - CMachine.temp) % 100 / 10, 1);
							Show_NUM44_32(62 + 32, 227, (40 - CMachine.temp) % 10, 1);
							LCD_Fill(50, 247, 60, 251, WHITE);
						} else if (CMachine.temp < 40) {
							LCD_Fill(62, 227, 94, 271, BLACK);
							LCD_Fill(50, 247, 60, 251, BLACK);

							Show_NUM44_32(62 + 32, 227, (40 - CMachine.temp) % 10, 1);
							LCD_Fill(70, 247, 80, 251, WHITE);
						} else if (CMachine.temp < 50) {
							LCD_Fill(62, 227, 94, 271, BLACK);
							LCD_Fill(50, 247, 60, 251, BLACK);

							Show_NUM44_32(62 + 32, 227, (CMachine.temp - 40) % 10, 1);
						} else {
							LCD_Fill(50, 247, 60, 251, BLACK);

							Show_NUM44_32(62, 227, (CMachine.temp - 40) % 100 / 10, 1);
							Show_NUM44_32(62 + 32, 227, (CMachine.temp - 40) % 10, 1);
						}
					}
//////////////////////////////////////////////////////////////////////////湿度        ///////////////////////////////////////////////
					if (CMachine_B.humidity != CMachine.humidity) {
						CMachine_B.humidity = CMachine.humidity;
						POINT_COLOR = WHITE;
						BACK_COLOR = BLACK;
						Show_NUM44_32(218, 227, CMachine.humidity % 100 / 10, 1);
						Show_NUM44_32(218 + 32, 227, CMachine.humidity % 10, 1);
					}

				}


			}
			Thread_Sleep(20);



		}

	}
	Thread_END






}



