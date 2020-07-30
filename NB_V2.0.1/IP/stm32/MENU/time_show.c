#include "includes.h"



_calendar_obj Sys_Time_Save;//时钟结构体
_calendar_obj Sys_Time_Show;//显示的时间
_calendar_obj Sys_Time_WIFI;//网络获得的时间





void Sys_RTC_Init(void) { //实时时钟初始化


	RTC_Init();//RTC初始化


	RTC_Get();//更新时间

	if (!(calendar.w_year >= 2018 && calendar.w_year < 2099)) {
		Sys_Time_Save.w_year =	2018;
		Sys_Time_Save.w_month = 11;
		Sys_Time_Save.w_date = 1;
		Sys_Time_Save.hour = 1;
		Sys_Time_Save.min = 1;
		Sys_Time_Save.sec = 0;
	}



	RTC_Set(Sys_Time_Save.w_year, Sys_Time_Save.w_month, Sys_Time_Save.w_date, Sys_Time_Save.hour, Sys_Time_Save.min, Sys_Time_Save.sec);

	RTC_Get();//更新时间







}










u8 uTime_Set_Mode = 0; //时间设置模式
u8 uTime_Set_Mode_B = 0; //时间设置模式



u8 uTime_Sec_Blink_No = 0; //秒闪计时


u8 uTime_Set_Blink_No = 0; //设置闪计数
#define BLINK_TIME	40	//闪烁时间	
#define BLINK_PAUSE_TIME	100	//暂停时间	


u16 uSet_Time_Out_No = 0; //设置时间超时计数


u8 SHOW_HTOC(u8	Pdata) { //
	u8 ctoh_b = 0;

	switch ((Pdata & 0x0f)) {
	case	0xf:
		ctoh_b = 'F';
		break;
	case	0xe:
		ctoh_b = 'E';
		break;
	case	0xd:
		ctoh_b = 'D';
		break;
	case	0xc:
		ctoh_b = 'C';
		break;
	case	0xb:
		ctoh_b = 'B';
		break;
	case	0xa:
		ctoh_b = 'A';
		break;
	case	0x9:
		ctoh_b = '9';
		break;
	case	0x8:
		ctoh_b = '8';
		break;
	case	0x7:
		ctoh_b = '7';
		break;
	case	0x6:
		ctoh_b = '6';
		break;
	case	0x5:
		ctoh_b = '5';
		break;
	case	0x4:
		ctoh_b = '4';
		break;
	case	0x3:
		ctoh_b = '3';
		break;
	case	0x2:
		ctoh_b = '2';
		break;
	case	0x1:
		ctoh_b = '1';
		break;
	case	0x0:
		ctoh_b = '0';
		break;
	default:
		break;

	}

	return ctoh_b;
}


void Time_Show(void) { //时间界面显示



	LCD_Clear(BLACK);


	POINT_COLOR = WHITE;
	BACK_COLOR = BLACK;
// dprintf_NUM100_72(85,95,"零",3,0);
// dprintf_NUM100_72(85+72,95,"零",3,0);
//			 LCD_Fill(235,114,235+12,114+12,WHITE);
	//		 LCD_Fill(235,170,235+12,170+12,WHITE);

//dprintf_NUM100_72(254,95,"零",3,0);
//dprintf_NUM100_72(254+72,95,"零",3,0);



// dprintf_NUM32_32(90,220,"星期天",3,0);
// dprintf_NUM32_32(90+96,220,"，",3,0);
// dprintf_NUM21_16(90+128,220+5,"零八",3,0);
//	 	LCD_Fill(252,240,252+3,240+3,WHITE);
//dprintf_NUM21_16(252+6,220+5,"零八",3,0);
//dprintf_NUM32_32(252+6+32,220,"，",3,0);
//dprintf_NUM21_16(252+6+64,220+5,"二零一八",3,0);









	uTime_Set_Mode = 0; //时间设置模式，初始化
	uTime_Sec_Blink_No = 0; //









	Sys_Time_Show.w_year = 0xff;
	Sys_Time_Show.w_month = 0xff;
	Sys_Time_Show.w_date = 0xff;
	Sys_Time_Show.hour = 0xff;
	Sys_Time_Show.min = 0xff;
	Sys_Time_Show.week = 0xff;



	while (f_Sys_Check == 2) {

		if (CMachine.UI_Show_No != 7)	break;

		if (CMachine.Key2_vale == 1) {	//按下
			CMachine.Key2_vale = 0; //清零
			if (uTime_Set_Mode == 0) {
				CMachine.UI_Show_No = 8; //进入下一界面
				uSet_Time_Out_No = 0; //超时自动退出清零
				break;//下一页，退出
			} else {
				uTime_Set_Mode++;
				if (uTime_Set_Mode > 5)	uTime_Set_Mode = 1;
				uSet_Time_Out_No = 0; //超时自动退出清零
			}
		}
		if (CMachine.Key2_vale == 2) {	//长按下
			CMachine.Key2_vale = 0; //清零
			if (uTime_Set_Mode == 0) {
				uTime_Set_Mode = 1; //进入设置时间模式
				Sys_Time_Save.w_year = calendar.w_year;
				Sys_Time_Save.w_month = calendar.w_month;
				Sys_Time_Save.w_date = calendar.w_date;
				Sys_Time_Save.hour = calendar.hour;
				Sys_Time_Save.min = calendar.min;
			} else {
				uTime_Set_Mode = 0; //退出设置时间模式
				Sys_Time_Save.sec = 0;
				RTC_Set(Sys_Time_Save.w_year, Sys_Time_Save.w_month, Sys_Time_Save.w_date, Sys_Time_Save.hour, Sys_Time_Save.min, Sys_Time_Save.sec);
				//重新刷新
				Sys_Time_Show.w_year = 0xff;
				Sys_Time_Show.w_month = 0xff;
				Sys_Time_Show.w_date = 0xff;
				Sys_Time_Show.hour = 0xff;
				Sys_Time_Show.min = 0xff;
				Sys_Time_Show.week = 0xff;
			}
		}

		if (CMachine.Key1_vale == 1) {	//按下
			CMachine.Key1_vale = 0; //清零
			uSet_Time_Out_No = 0; //超时自动退出清零
			if (uTime_Set_Mode == 0) {


			} else if (uTime_Set_Mode == 1) {														////////////////////////////     年减     /////////////////////////
				uTime_Set_Blink_No = BLINK_PAUSE_TIME; //暂停闪烁
				if (Sys_Time_Save.w_year > 2018)	Sys_Time_Save.w_year--;
				if (0 == Is_Leap_Year(Sys_Time_Save.w_year)) { //非润年
					if (Sys_Time_Save.w_month == 2 && Sys_Time_Save.w_date > 28) {
						Sys_Time_Save.w_date = 28;
						if (CMachine.uLanguage_Mode != 0) {
							//更新
							Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
							Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);
						} else {
							//更新
							Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
							Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
						}
					}
				} else {
					if (Sys_Time_Save.w_month == 2 && Sys_Time_Save.w_date > 29) {
						Sys_Time_Save.w_date = 29;
						if (CMachine.uLanguage_Mode != 0) {
							//更新
							Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
							Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);
						} else {
							//更新
							Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
							Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
						}
					}
				}
			} else if (uTime_Set_Mode == 2) {														////////////////////////////     月减     ////////////////////////
				uTime_Set_Blink_No = BLINK_PAUSE_TIME; //暂停闪烁
				if (Sys_Time_Save.w_month > 0)Sys_Time_Save.w_month--;
				if (Sys_Time_Save.w_month == 0)	Sys_Time_Save.w_month = 12;
				if (0 == Is_Leap_Year(Sys_Time_Save.w_year)) { //非润年
					if (Sys_Time_Save.w_month == 2 && Sys_Time_Save.w_date > 28) {
						Sys_Time_Save.w_date = 28;
						if (CMachine.uLanguage_Mode != 0) {
							//更新
							Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
							Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);
						} else {
							//更新
							Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
							Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
						}
					}
				} else {
					if (Sys_Time_Save.w_month == 2 && Sys_Time_Save.w_date > 29) {
						Sys_Time_Save.w_date = 29;
						if (CMachine.uLanguage_Mode != 0) {
							//更新
							Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
							Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);
						} else {
							//更新
							Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
							Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
						}
					}
				}
				if ((Sys_Time_Save.w_month == 4 || Sys_Time_Save.w_month == 6 || Sys_Time_Save.w_month == 9 || Sys_Time_Save.w_month == 11) && Sys_Time_Save.w_date > 30) {
					Sys_Time_Save.w_date = 30;
					if (CMachine.uLanguage_Mode != 0) {
						//更新
						Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
						Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);
					} else {
						//更新
						Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
						Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
					}
				}
			} else if (uTime_Set_Mode == 3) {														////////////////////////////     日减     ////////////////////////
				uTime_Set_Blink_No = BLINK_PAUSE_TIME; //暂停闪烁
				if (Sys_Time_Save.w_date > 0)	Sys_Time_Save.w_date--;
				if (0 == Is_Leap_Year(Sys_Time_Save.w_year)) { //非润年
					if (Sys_Time_Save.w_month == 2) {
						if (Sys_Time_Save.w_date == 0)	Sys_Time_Save.w_date = 28;
					}
				} else {
					if (Sys_Time_Save.w_month == 2) {
						if (Sys_Time_Save.w_date == 0)	Sys_Time_Save.w_date = 29;
					}
				}
				if (Sys_Time_Save.w_month == 4 || Sys_Time_Save.w_month == 6 || Sys_Time_Save.w_month == 9 || Sys_Time_Save.w_month == 11) {
					if (Sys_Time_Save.w_date == 0)	Sys_Time_Save.w_date = 30;
				}
				if (Sys_Time_Save.w_month == 1 || Sys_Time_Save.w_month == 3 || Sys_Time_Save.w_month == 5 || Sys_Time_Save.w_month == 7 || Sys_Time_Save.w_month == 8 || Sys_Time_Save.w_month == 10 || Sys_Time_Save.w_month == 12) {
					if (Sys_Time_Save.w_date == 0)	Sys_Time_Save.w_date = 31;
				}
			} else if (uTime_Set_Mode == 4) {														////////////////////////////     时减     ////////////////////////
				uTime_Set_Blink_No = BLINK_PAUSE_TIME; //暂停闪烁
				if (Sys_Time_Save.hour > 0)Sys_Time_Save.hour--;
				else	Sys_Time_Save.hour = 23;
			} else if (uTime_Set_Mode == 5) {														////////////////////////////     分减     ////////////////////////
				uTime_Set_Blink_No = BLINK_PAUSE_TIME; //暂停闪烁
				if (Sys_Time_Save.min > 0)Sys_Time_Save.min--;
				else	Sys_Time_Save.min = 59;
			}
		}
		if (CMachine.Key1_vale == 2) {	//长按下

		}

		if (CMachine.Key3_vale == 1) {	//按下
			CMachine.Key3_vale = 0; //清零
			uSet_Time_Out_No = 0; //超时自动退出清零
			if (uTime_Set_Mode == 0) {


			} else if (uTime_Set_Mode == 1) {															////////////////////////////     年加     /////////////////////////////////////////////////////////////////////////////
				uTime_Set_Blink_No = BLINK_PAUSE_TIME; //暂停闪烁
				Sys_Time_Save.w_year++;
				if (Sys_Time_Save.w_year > 2099)	Sys_Time_Save.w_year = 2099;
				if (0 == Is_Leap_Year(Sys_Time_Save.w_year)) { //非润年
					if (Sys_Time_Save.w_month == 2 && Sys_Time_Save.w_date > 28) {
						Sys_Time_Save.w_date = 28;
						if (CMachine.uLanguage_Mode != 0) {
							//更新
							Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
							Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);
						} else {
							//更新
							Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
							Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
						}
					}
				} else {
					if (Sys_Time_Save.w_month == 2 && Sys_Time_Save.w_date > 29) {
						Sys_Time_Save.w_date = 29;
						if (CMachine.uLanguage_Mode != 0) {
							//更新
							Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
							Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);
						} else {
							//更新
							Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
							Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
						}
					}
				}
			} else if (uTime_Set_Mode == 2) {														////////////////////////////     月加     /////////////////////////
				uTime_Set_Blink_No = BLINK_PAUSE_TIME; //暂停闪烁
				Sys_Time_Save.w_month++;
				if (Sys_Time_Save.w_month > 12)	Sys_Time_Save.w_month = 1;
				if (0 == Is_Leap_Year(Sys_Time_Save.w_year)) { //非润年
					if (Sys_Time_Save.w_month == 2 && Sys_Time_Save.w_date > 28) {
						Sys_Time_Save.w_date = 28;
						if (CMachine.uLanguage_Mode != 0) {
							//更新
							Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
							Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);
						} else {
							//更新
							Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
							Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
						}
					}
				} else {
					if (Sys_Time_Save.w_month == 2 && Sys_Time_Save.w_date > 29) {
						Sys_Time_Save.w_date = 29;
						if (CMachine.uLanguage_Mode != 0) {
							//更新
							Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
							Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);
						} else {
							//更新
							Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
							Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
						}
					}
				}
				if ((Sys_Time_Save.w_month == 4 || Sys_Time_Save.w_month == 6 || Sys_Time_Save.w_month == 9 || Sys_Time_Save.w_month == 11) && Sys_Time_Save.w_date > 30) {
					Sys_Time_Save.w_date = 30;
					if (CMachine.uLanguage_Mode != 0) {
						//更新
						Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
						Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);
					} else {
						//更新
						Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
						Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
					}
				}
			} else if (uTime_Set_Mode == 3) {														////////////////////////////     日加     /////////////////////////
				uTime_Set_Blink_No = BLINK_PAUSE_TIME; //暂停闪烁
				Sys_Time_Save.w_date++;
				if (0 == Is_Leap_Year(Sys_Time_Save.w_year)) { //非润年
					if (Sys_Time_Save.w_month == 2) {
						if (Sys_Time_Save.w_date > 28)	Sys_Time_Save.w_date = 1;
					}
				} else {
					if (Sys_Time_Save.w_month == 2) {
						if (Sys_Time_Save.w_date > 29)	Sys_Time_Save.w_date = 1;
					}
				}
				if (Sys_Time_Save.w_month == 4 || Sys_Time_Save.w_month == 6 || Sys_Time_Save.w_month == 9 || Sys_Time_Save.w_month == 11) {
					if (Sys_Time_Save.w_date > 30)	Sys_Time_Save.w_date = 1;
				}
				if (Sys_Time_Save.w_month == 1 || Sys_Time_Save.w_month == 3 || Sys_Time_Save.w_month == 5 || Sys_Time_Save.w_month == 7 || Sys_Time_Save.w_month == 8 || Sys_Time_Save.w_month == 10 || Sys_Time_Save.w_month == 12) {
					if (Sys_Time_Save.w_date > 31)	Sys_Time_Save.w_date = 1;
				}
			} else if (uTime_Set_Mode == 4) {														////////////////////////////     时加     /////////////////////////
				uTime_Set_Blink_No = BLINK_PAUSE_TIME; //暂停闪烁
				Sys_Time_Save.hour++;
				if (Sys_Time_Save.hour > 23)	Sys_Time_Save.hour = 0;
			} else if (uTime_Set_Mode == 5) {														////////////////////////////     分加     /////////////////////////
				uTime_Set_Blink_No = BLINK_PAUSE_TIME; //暂停闪烁
				Sys_Time_Save.min++;
				if (Sys_Time_Save.min > 59)	Sys_Time_Save.min = 0;
			}
		}
		if (CMachine.Key3_vale == 2) {	//长按下




		}







		User_Public_Show();//公共显示部分

		if (uTime_Set_Mode != uTime_Set_Mode_B) {
			uTime_Set_Mode_B = uTime_Set_Mode;
			uTime_Set_Blink_No = 25;


			LCD_Fill(232, 114 - 5, 232 + 12, 114 + 12 - 5, WHITE);
			LCD_Fill(232, 170 - 5, 232 + 12, 170 + 12 - 5, WHITE);


			Show_NUM21_16(322, 234, Sys_Time_Save.w_year / 1000, 1);
			Show_NUM21_16(322 + 16, 234, Sys_Time_Save.w_year % 1000 / 100, 1);
			Show_NUM21_16(322 + 32, 234, Sys_Time_Save.w_year % 100 / 10, 1);
			Show_NUM21_16(322 + 48, 234, Sys_Time_Save.w_year % 10, 1);


			if (CMachine.uLanguage_Mode != 0) {

				//显示
				Show_NUM21_16(190, 234, Sys_Time_Save.w_date / 10, 1);
				Show_NUM21_16(190 + 16, 234, Sys_Time_Save.w_date % 10, 1);


				if (Sys_Time_Save.w_month == 1)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jan_03, 1); //
				else	if (Sys_Time_Save.w_month == 2)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Feb_03, 1); //
				else	if (Sys_Time_Save.w_month == 3)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Mar_03, 1); //
				else	if (Sys_Time_Save.w_month == 4)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Apr_03, 1); //
				else	if (Sys_Time_Save.w_month == 5)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_May_03, 1); //
				else	if (Sys_Time_Save.w_month == 6)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jun_03, 1); //
				else	if (Sys_Time_Save.w_month == 7)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jul_03, 1); //
				else	if (Sys_Time_Save.w_month == 8)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Aug_03, 1); //
				else	if (Sys_Time_Save.w_month == 9)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Sep_03, 1); //
				else	if (Sys_Time_Save.w_month == 10)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Oct_03, 1); //
				else	if (Sys_Time_Save.w_month == 11)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Nov_03, 1); //
				else	if (Sys_Time_Save.w_month == 12)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Dec_03, 1); //
			} else {

				//显示
				Show_NUM21_16(190, 234, Sys_Time_Save.w_month / 10, 1);
				Show_NUM21_16(190 + 16, 234, Sys_Time_Save.w_month % 10, 1);

				Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
				Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);


			}


			Show_NUM100_72(85, 95, Sys_Time_Save.hour / 10, 1);
			Show_NUM100_72(85 + 72, 95, Sys_Time_Save.hour % 10, 1);


			Show_NUM100_72(254, 95, Sys_Time_Save.min / 10, 1);
			Show_NUM100_72(254 + 72, 95, Sys_Time_Save.min % 10, 1);

			//不显示星期
			icon_show(90, 232, 52, 26, gImage_Week_03, 0);
			icon_show(143, 232, 27, 26, gImage_Mon_03, 0); //

			icon_show(90, 235, 62, 21, gImage_eMon_03, 0); //清，不显示





		}







		if (uTime_Set_Mode == 1) { //年设置
			if (uTime_Set_Blink_No == 20) {
				Show_NUM21_16(322, 234, Sys_Time_Save.w_year / 1000, 0);
				Show_NUM21_16(322 + 16, 234, Sys_Time_Save.w_year % 1000 / 100, 0);
				Show_NUM21_16(322 + 32, 234, Sys_Time_Save.w_year % 100 / 10, 0);
				Show_NUM21_16(322 + 48, 234, Sys_Time_Save.w_year % 10, 0);
			}
			if (uTime_Set_Blink_No == BLINK_TIME || uTime_Set_Blink_No == BLINK_PAUSE_TIME) {
				Show_NUM21_16(322, 234, Sys_Time_Save.w_year / 1000, 1);
				Show_NUM21_16(322 + 16, 234, Sys_Time_Save.w_year % 1000 / 100, 1);
				Show_NUM21_16(322 + 32, 234, Sys_Time_Save.w_year % 100 / 10, 1);
				Show_NUM21_16(322 + 48, 234, Sys_Time_Save.w_year % 10, 1);
			}
		} else if (uTime_Set_Mode == 2) { //月设置
			if (CMachine.uLanguage_Mode != 0) {
				if (uTime_Set_Blink_No == 20) {
					if (Sys_Time_Save.w_month == 1)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jan_03, 0); //
					else	if (Sys_Time_Save.w_month == 2)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Feb_03, 0); //
					else	if (Sys_Time_Save.w_month == 3)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Mar_03, 0); //
					else	if (Sys_Time_Save.w_month == 4)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Apr_03, 0); //
					else	if (Sys_Time_Save.w_month == 5)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_May_03, 0); //
					else	if (Sys_Time_Save.w_month == 6)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jun_03, 0); //
					else	if (Sys_Time_Save.w_month == 7)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jul_03, 0); //
					else	if (Sys_Time_Save.w_month == 8)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Aug_03, 0); //
					else	if (Sys_Time_Save.w_month == 9)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Sep_03, 0); //
					else	if (Sys_Time_Save.w_month == 10)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Oct_03, 0); //
					else	if (Sys_Time_Save.w_month == 11)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Nov_03, 0); //
					else	if (Sys_Time_Save.w_month == 12)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Dec_03, 0); //
				}
				if (uTime_Set_Blink_No == BLINK_TIME || uTime_Set_Blink_No == BLINK_PAUSE_TIME) {
					if (Sys_Time_Save.w_month == 1)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jan_03, 1); //
					else	if (Sys_Time_Save.w_month == 2)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Feb_03, 1); //
					else	if (Sys_Time_Save.w_month == 3)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Mar_03, 1); //
					else	if (Sys_Time_Save.w_month == 4)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Apr_03, 1); //
					else	if (Sys_Time_Save.w_month == 5)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_May_03, 1); //
					else	if (Sys_Time_Save.w_month == 6)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jun_03, 1); //
					else	if (Sys_Time_Save.w_month == 7)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jul_03, 1); //
					else	if (Sys_Time_Save.w_month == 8)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Aug_03, 1); //
					else	if (Sys_Time_Save.w_month == 9)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Sep_03, 1); //
					else	if (Sys_Time_Save.w_month == 10)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Oct_03, 1); //
					else	if (Sys_Time_Save.w_month == 11)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Nov_03, 1); //
					else	if (Sys_Time_Save.w_month == 12)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Dec_03, 1); //
				}
			} else {
				if (uTime_Set_Blink_No == 20) {
					Show_NUM21_16(190, 234, Sys_Time_Save.w_month / 10, 0);
					Show_NUM21_16(190 + 16, 234, Sys_Time_Save.w_month % 10, 0);
				}
				if (uTime_Set_Blink_No == BLINK_TIME || uTime_Set_Blink_No == BLINK_PAUSE_TIME) {
					Show_NUM21_16(190, 234, Sys_Time_Save.w_month / 10, 1);
					Show_NUM21_16(190 + 16, 234, Sys_Time_Save.w_month % 10, 1);
				}
			}
		} else if (uTime_Set_Mode == 3) { //日设置
			if (CMachine.uLanguage_Mode != 0) {
				if (uTime_Set_Blink_No == 20) {
					Show_NUM21_16(190, 234, Sys_Time_Save.w_date / 10, 0);
					Show_NUM21_16(190 + 16, 234, Sys_Time_Save.w_date % 10, 0);
				}
				if (uTime_Set_Blink_No == BLINK_TIME || uTime_Set_Blink_No == BLINK_PAUSE_TIME) {
					Show_NUM21_16(190, 234, Sys_Time_Save.w_date / 10, 1);
					Show_NUM21_16(190 + 16, 234, Sys_Time_Save.w_date % 10, 1);
				}

			} else {
				if (uTime_Set_Blink_No == 20) {
					Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 0);
					Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 0);
				}
				if (uTime_Set_Blink_No == BLINK_TIME || uTime_Set_Blink_No == BLINK_PAUSE_TIME) {
					Show_NUM21_16(190 + 32 + 25, 234, Sys_Time_Save.w_date / 10, 1);
					Show_NUM21_16(190 + 48 + 25, 234, Sys_Time_Save.w_date % 10, 1);
				}
			}
		} else if (uTime_Set_Mode == 4) { //时设置
			if (uTime_Set_Blink_No == 20) {
				Show_NUM100_72(85, 95, Sys_Time_Save.hour / 10, 0);
				Show_NUM100_72(85 + 72, 95, Sys_Time_Save.hour % 10, 0);
			}
			if (uTime_Set_Blink_No == BLINK_TIME || uTime_Set_Blink_No == BLINK_PAUSE_TIME) {
				Show_NUM100_72(85, 95, Sys_Time_Save.hour / 10, 1);
				Show_NUM100_72(85 + 72, 95, Sys_Time_Save.hour % 10, 1);
			}
		} else if (uTime_Set_Mode == 5) { //分设置
			if (uTime_Set_Blink_No == 20) {
				Show_NUM100_72(254, 95, Sys_Time_Save.min / 10, 0);
				Show_NUM100_72(254 + 72, 95, Sys_Time_Save.min % 10, 0);
			}
			if (uTime_Set_Blink_No == BLINK_TIME || uTime_Set_Blink_No == BLINK_PAUSE_TIME) {
				Show_NUM100_72(254, 95, Sys_Time_Save.min / 10, 1);
				Show_NUM100_72(254 + 72, 95, Sys_Time_Save.min % 10, 1);
			}
		} else if (uTime_Set_Mode == 0) { //

			if (CMachine.uID_Show_Time_No > 0) {

				if (CMachine.uID_Show_Time_No_B == 1) {
					CMachine.uID_Show_Time_No_B = 2;

					LCD_Clear(BLACK);
					POINT_COLOR = WHITE;
					BACK_COLOR = BLACK;
					LCD_ShowChar(120, 100, SHOW_HTOC((SYS_TYPE & 0xf000) >> 12), 16, 0);
					LCD_ShowChar(120 + 8, 100, SHOW_HTOC((SYS_TYPE & 0xf00) >> 8), 16, 0);
					LCD_ShowChar(120 + 16, 100, ':', 16, 0);
					LCD_ShowChar(120 + 24, 100, SHOW_HTOC((SYS_TYPE & 0xf0) >> 4), 16, 0);
					LCD_ShowChar(120 + 32, 100, SHOW_HTOC((SYS_TYPE & 0xf) >> 0), 16, 0);
					LCD_ShowChar(120 + 40, 100, ':', 16, 0);


					LCD_ShowChar(120 + 48, 100, SHOW_HTOC((SYS_EDITION & 0xf0) >> 4), 16, 0);
					LCD_ShowChar(120 + 56, 100, SHOW_HTOC((SYS_EDITION & 0xf) >> 0), 16, 0);
					LCD_ShowChar(120 + 64, 100, ':', 16, 0);

					LCD_ShowChar(120 + 72, 100, SHOW_HTOC((SYS_TYPE_NO & 0xf0) >> 20), 16, 0);
					LCD_ShowChar(120 + 80, 100, SHOW_HTOC((SYS_TYPE_NO & 0xf) >> 16), 16, 0);
					LCD_ShowChar(120 + 88, 100, ':', 16, 0);
					LCD_ShowChar(120 + 96, 100, SHOW_HTOC((SYS_TYPE_NO & 0xf0) >> 12), 16, 0);
					LCD_ShowChar(120 + 104, 100, SHOW_HTOC((SYS_TYPE_NO & 0xf) >> 8), 16, 0);


					LCD_ShowChar(120 + 112, 100, ':', 16, 0);
					LCD_ShowChar(120 + 120, 100, SHOW_HTOC((SYS_TYPE_NO & 0xf0) >> 4), 16, 0);
					LCD_ShowChar(120 + 128, 100, SHOW_HTOC((SYS_TYPE_NO & 0xf) >> 0), 16, 0);





				}



			} else {

				if (CMachine.uLanguage_Mode_B != CMachine.uLanguage_Mode) {
					CMachine.uLanguage_Mode_B = CMachine.uLanguage_Mode;

					Sys_Time_Show.week = 0xff;

				}



				if (CMachine.uID_Show_Time_No_B != 0) {
					CMachine.uID_Show_Time_No_B = 0;
					LCD_Clear(BLACK);
					POINT_COLOR = WHITE;
					BACK_COLOR = BLACK;
					dprintf_NUM32_32(90 + 96, 220, "，", 3, 0);
					LCD_Fill(252, 240, 252 + 3, 240 + 3, WHITE);
					dprintf_NUM32_32(252 + 6 + 32, 220, "，", 3, 0);
					uTime_Set_Mode = 0; //时间设置模式，初始化
					uTime_Sec_Blink_No = 0; //
					Sys_Time_Show.w_year = 0xff;
					Sys_Time_Show.w_month = 0xff;
					Sys_Time_Show.w_date = 0xff;
					Sys_Time_Show.hour = 0xff;
					Sys_Time_Show.min = 0xff;
					Sys_Time_Show.week = 0xff;

					CMachine_B.UI_Show_No = 0xff;
					// icon_show(36, 20, 60, 25, gImage_RHT_03,1);//显示
					//	 icon_show(394, 16, 49, 26, gImage_battery_03,1);//显示

				}

				//RTC_Get();//更新时间  //放到TVOC重启控制的地方
				calendar.week = RTC_Get_Week(calendar.w_year, calendar.w_month, calendar.w_date); //获取星期
				uTime_Sec_Blink_No++;
				if (uTime_Sec_Blink_No > 49)	uTime_Sec_Blink_No = 0;
				if (uTime_Sec_Blink_No == 25) {
					LCD_Fill(232, 114 - 5, 232 + 12, 114 + 12 - 5, BLACK);
					LCD_Fill(232, 170 - 5, 232 + 12, 170 + 12 - 5, BLACK);
				}
				if (uTime_Sec_Blink_No == 0) {
					LCD_Fill(232, 114 - 5, 232 + 12, 114 + 12 - 5, WHITE);
					LCD_Fill(232, 170 - 5, 232 + 12, 170 + 12 - 5, WHITE);
				}
				if (Sys_Time_Show.w_year != calendar.w_year) {

					Sys_Time_Show.w_year = calendar.w_year;
					Show_NUM21_16(322, 234, calendar.w_year / 1000, 1);
					Show_NUM21_16(322 + 16, 234, calendar.w_year % 1000 / 100, 1);
					Show_NUM21_16(322 + 32, 234, calendar.w_year % 100 / 10, 1);
					Show_NUM21_16(322 + 48, 234, calendar.w_year % 10, 1);
				}
				if (Sys_Time_Show.w_month != calendar.w_month || Sys_Time_Show.w_date != calendar.w_date) {
					if (CMachine.uLanguage_Mode != 0) {
						//清，不显示
						Show_NUM21_16(190, 234, calendar.w_month / 10, 0);
						Show_NUM21_16(190 + 16, 234, calendar.w_month % 10, 0);
						Show_NUM21_16(190 + 32 + 25, 234, calendar.w_date / 10, 0);
						Show_NUM21_16(190 + 48 + 25, 234, calendar.w_date % 10, 0);
						icon_show(190 + 32, 232, 25, 26, gImage_Month_03, 0); //月
						icon_show(190 + 64 + 25, 232, 25, 26, gImage_Day_03, 0); //日
						//显示
						Sys_Time_Show.w_date = calendar.w_date;
						Show_NUM21_16(190, 234, calendar.w_date / 10, 1);
						Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 1);

						Sys_Time_Show.w_month = calendar.w_month;
						if (Sys_Time_Show.w_month == 1)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jan_03, 1); //
						else	if (Sys_Time_Show.w_month == 2)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Feb_03, 1); //
						else	if (Sys_Time_Show.w_month == 3)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Mar_03, 1); //
						else	if (Sys_Time_Show.w_month == 4)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Apr_03, 1); //
						else	if (Sys_Time_Show.w_month == 5)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_May_03, 1); //
						else	if (Sys_Time_Show.w_month == 6)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jun_03, 1); //
						else	if (Sys_Time_Show.w_month == 7)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jul_03, 1); //
						else	if (Sys_Time_Show.w_month == 8)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Aug_03, 1); //
						else	if (Sys_Time_Show.w_month == 9)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Sep_03, 1); //
						else	if (Sys_Time_Show.w_month == 10)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Oct_03, 1); //
						else	if (Sys_Time_Show.w_month == 11)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Nov_03, 1); //
						else	if (Sys_Time_Show.w_month == 12)	icon_show(190 + 32 + 15, 235, 58, 26, gImage_Dec_03, 1); //
					} else {
						//清，不显示
						Show_NUM21_16(190, 234, calendar.w_date / 10, 0);
						Show_NUM21_16(190 + 16, 234, calendar.w_date % 10, 0);
						icon_show(190 + 32 + 15, 235, 58, 26, gImage_Jan_03, 0); //
						//显示
						Sys_Time_Show.w_month = calendar.w_month;
						Show_NUM21_16(190, 234, calendar.w_month / 10, 1);
						Show_NUM21_16(190 + 16, 234, calendar.w_month % 10, 1);

						Sys_Time_Show.w_date = calendar.w_date;
						Show_NUM21_16(190 + 32 + 25, 234, calendar.w_date / 10, 1);
						Show_NUM21_16(190 + 48 + 25, 234, calendar.w_date % 10, 1);

						icon_show(190 + 32, 232, 25, 26, gImage_Month_03, 1); //月
						icon_show(190 + 64 + 25, 232, 25, 26, gImage_Day_03, 1); //日
					}
				}
				if (Sys_Time_Show.hour != calendar.hour) {
					Sys_Time_Show.hour = calendar.hour;
					Show_NUM100_72(85, 95, calendar.hour / 10, 1);
					Show_NUM100_72(85 + 72, 95, calendar.hour % 10, 1);
				}
				if (Sys_Time_Show.min != calendar.min) {
					Sys_Time_Show.min = calendar.min;
					Show_NUM100_72(254, 95, calendar.min / 10, 1);
					Show_NUM100_72(254 + 72, 95, calendar.min % 10, 1);
				}
				if (Sys_Time_Show.week != calendar.week) {
					Sys_Time_Show.week = calendar.week;




					if (CMachine.uLanguage_Mode != 0) {
						//清，不显示
						icon_show(90, 232, 52, 26, gImage_Week_03, 0); //
						icon_show(143, 232, 27, 26, gImage_Mon_03, 0); //
						//显示
						if (calendar.week == 1)			 				 icon_show(90, 235, 62, 21, gImage_eMon_03, 1); //
						else		if (calendar.week == 2)			 icon_show(90, 235, 62, 21, gImage_eTue_03, 1); //
						else		if (calendar.week == 3)			 icon_show(90, 235, 62, 21, gImage_eWed_03, 1); //
						else		if (calendar.week == 4)			 icon_show(90, 235, 62, 21, gImage_eThu_03, 1); //
						else		if (calendar.week == 5)			 icon_show(90, 235, 62, 21, gImage_eFri_03, 1); //
						else		if (calendar.week == 6)			 icon_show(90, 235, 62, 21, gImage_eSat_03, 1); //
						else		if (calendar.week == 7)			 icon_show(90, 235, 62, 21, gImage_eSun_03, 1); //

					} else {
						//清，不显示
						icon_show(90, 235, 62, 21, gImage_eMon_03, 0); //
						//显示
						icon_show(90, 232, 52, 26, gImage_Week_03, 1); //星期
						if (calendar.week == 1)							 icon_show(143, 232, 27, 26, gImage_Mon_03, 1); //
						else		if (calendar.week == 2)			 icon_show(143, 232, 27, 26, gImage_Tues_03, 1); //
						else		if (calendar.week == 3)			 icon_show(143, 232, 27, 26, gImage_Wed_03, 1); //
						else		if (calendar.week == 4)			 icon_show(143, 232, 27, 26, gImage_Thur_03, 1); //
						else		if (calendar.week == 5)			 icon_show(143, 232, 27, 26, gImage_Fri_03, 1); //
						else		if (calendar.week == 6)			 icon_show(143, 232, 27, 26, gImage_Sat_03, 1); //
						else		if (calendar.week == 7)			 icon_show(143, 232, 27, 26, gImage_Sun_03, 1); //
					}
				}
			}
		}



		if (uTime_Set_Mode != 0) {
			if (uTime_Set_Blink_No > 0) {
				uTime_Set_Blink_No--;
				if (uTime_Set_Blink_No == 0)	uTime_Set_Blink_No = BLINK_TIME;
			}

			uSet_Time_Out_No++;
			if (uSet_Time_Out_No > 1500) { //如果设置时间超过30秒无动作，自动不保存退出
				uSet_Time_Out_No = 0; //
				uTime_Set_Mode = 0; //退出设置时间模式
				//重新刷新
				Sys_Time_Show.w_year = 0xff;
				Sys_Time_Show.w_month = 0xff;
				Sys_Time_Show.w_date = 0xff;
				Sys_Time_Show.hour = 0xff;
				Sys_Time_Show.min = 0xff;
				Sys_Time_Show.week = 0xff;
			}



		}





		delay_ms(20);
	}











}
