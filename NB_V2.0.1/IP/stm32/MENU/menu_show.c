#include "includes.h"




u8 uCharge_Blink_No = 0; //�����˸����


u8 uBattery_Show = 0;

u8 uRF_Blink_No = 0; //RF��˸����



void User_Public_Show(void) { //������ʾ����




	/*
	if(CMachine.fRF_Mode==0)
	{
		icon_show(300, 15, 45, 30, gImage_RF4,1);//��ʾ
		uRF_Blink_No=0;
	}
	else
	{
		uRF_Blink_No++;
		if(uRF_Blink_No>99)	uRF_Blink_No=0;
		if(uRF_Blink_No==1)		 icon_show(300, 15, 45, 30, gImage_RF1,1);//��ʾ
		else if(uRF_Blink_No==25)		 icon_show(300, 15, 45, 30, gImage_RF2,1);//��ʾ
		else if(uRF_Blink_No==50)		 icon_show(300, 15, 45, 30, gImage_RF3,1);//��ʾ
		else if(uRF_Blink_No==75)		 icon_show(300, 15, 45, 30, gImage_RF4,1);//��ʾ

	}
	*/

	if (CMachine.uLine_Work != 0) {
		icon_show(150, 15, 79, 29, gImage_NB_033_03, 1); //��ʾ
		uRF_Blink_No = 0;
	} else {
		uRF_Blink_No++;
		if (uRF_Blink_No >= 75)	uRF_Blink_No = 0;
		if (uRF_Blink_No == 1)		 icon_show(150, 15, 79, 29, gImage_NB_031_03, 1); //��ʾ
		else if (uRF_Blink_No == 25)		 icon_show(150, 15, 79, 29, gImage_NB_032_03, 1); //��ʾ
		else if (uRF_Blink_No == 50)		 icon_show(150, 15, 79, 29, gImage_NB_033_03, 1); //��ʾ


	}


	if (CMachine.uSys_User_Reset == 1) {

		CMachine.uSys_User_Reset_Time_LED_Blink_No++;
		if (CMachine.uSys_User_Reset_Time_LED_Blink_No > 80)	CMachine.uSys_User_Reset_Time_LED_Blink_No = 0;

		if (CMachine.uSys_User_Reset_Time_LED_Blink_No == 1)		 icon_show(36, 20, 60, 25, gImage_RHT_03, 1); //��ʾ
		else	if (CMachine.uSys_User_Reset_Time_LED_Blink_No == 40)	 icon_show(36, 20, 60, 25, gImage_RHT_03, 0); //��ʾ


	} else {
		CMachine.uSys_User_Reset_Time_LED_Blink_No = 0; //
		icon_show(36, 20, 60, 25, gImage_RHT_03, 1); //��ʾ
	}





	if (CMachine.UI_Show_No != CMachine_B.UI_Show_No) {


		CMachine_B.UI_Show_No = CMachine.UI_Show_No;



// icon_show(36, 20, 60, 25, gImage_RHT_03,1);//��ʾ


//icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ

		//	icon_show(150, 15, 79, 29, gImage_NB_033_03,1);//��ʾ




		uCharge_Blink_No = 1;

		uBattery_Show = 0xff;


		CMachine.Battery_Low_Power_Off_No = 0; //����



	}

	if (CMachine.fCharge == 1) {

		uBattery_Show = 0xff; //ˢ��
		if (CMachine.Battery == 100) {
			if (uCharge_Blink_No != 0)	icon_show(394, 16, 49, 26, gImage_battery_03, 1); //��ʾ
			uCharge_Blink_No = 0;
		} else {
			if (uCharge_Blink_No == 1) {
				icon_show(394, 16, 49, 26, gImage_battery_00, 1); //��ʾ
			}
			if (uCharge_Blink_No == 20) {
				icon_show(394, 16, 49, 26, gImage_battery_01, 1); //��ʾ
			}
			if (uCharge_Blink_No == 40) {
				icon_show(394, 16, 49, 26, gImage_battery_02, 1); //��ʾ
			}
			if (uCharge_Blink_No == 60) {
				icon_show(394, 16, 49, 26, gImage_battery_03, 1); //��ʾ
			}
			uCharge_Blink_No++;
			if (uCharge_Blink_No > 80)	uCharge_Blink_No = 1;
		}
	} else {

		//	if(fCharge_b==1)	 icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ


		if (CMachine.Battery < 10) {
			if (uCharge_Blink_No == 1) {
				icon_show(394, 16, 49, 26, gImage_battery_00, 1); //��ʾ
			}
			if (uCharge_Blink_No == 20) {
				icon_show(394, 16, 49, 26, gImage_battery_03, 0); //��ʾ
			}
			if (uCharge_Blink_No == 40) {
				icon_show(394, 16, 49, 26, gImage_battery_00, 1); //��ʾ
			}
			if (uCharge_Blink_No == 60) {
				icon_show(394, 16, 49, 26, gImage_battery_03, 0); //��ʾ
			}
			uCharge_Blink_No++;
			if (uCharge_Blink_No > 80)	uCharge_Blink_No = 1;
			uBattery_Show = CMachine.Battery;

			if (CMachine.Battery == 0) {
				CMachine.Battery_Low_Power_Off_No++;//����
				if (CMachine.Battery_Low_Power_Off_No >= 160) { //�͵�ػ�
					SYS_POWER_OFF();//ϵͳ�ر�
				}
			}
		} else {
			if (uBattery_Show != CMachine.Battery) {
				uBattery_Show = CMachine.Battery;
				if (CMachine.Battery > 80) {
					icon_show(394, 16, 49, 26, gImage_battery_03, 1); //��ʾ
				} else if (CMachine.Battery > 60) {
					icon_show(394, 16, 49, 26, gImage_battery_02, 1); //��ʾ
				} else if (CMachine.Battery > 40) {
					icon_show(394, 16, 49, 26, gImage_battery_01, 1); //��ʾ
				} else if (CMachine.Battery > 20) {
					icon_show(394, 16, 49, 26, gImage_battery_00, 1); //��ʾ
				}
			}
		}





	}










}



unsigned short UI_Show_No_B_Show_Task = 0xff; //

void User_Menu_Show_Task(void *pdata) { //�˵�ͼ����ʾ����
//	dprintf_WORD16_16(192,20,"�����Ⱦ",3,0);
	while (1) {

		if (CMachine.UI_Show_No < 1) {

			if (UI_Show_No_B_Show_Task != 0) {
				LCD_UnInit();
				UI_Show_No_B_Show_Task = 0;
			}

			delay_ms(10);
			continue;//�˳�


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
			Charge_Off_Show();//�ػ���������ʾ
			break;
		case 2:
			All_Show();//���в���������ʾ

			break;
		case 3:

			PM25_Show();//PM2.5������ʾ
			break;
		case 4:

			TVOC_Show();//TVOC������ʾ
			break;
		case 5:

			CO2_Show();//TVOC������ʾ
			break;
		case 6:
			HCHO_Show();//��ȩ������ʾ

			break;
		case 7:
			Time_Show();//ʱ�������ʾ

			break;
		default:
			break;
		}
		delay_ms(20);

	}











}

























