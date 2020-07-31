
#include "includes.h"



#define QRCODE_Y 	70		//TFT二维码显示坐标y


u8 Iot_Website[] = "https://m.poscn.com/iot/linkcode?id=1_";
u8 ID_DISPLAY_BUF[80];     //
char * iot_str_p;//字符串指针//
u8 iot_str_l = 0; //字符长度


#define ID_Show_Y	45
#define MAC_Show_Y 45

void DISPLAY_RENCODE_TO_TFT(u8 *qrcode_data, u16 color) {
	u8 i, j;
	u16 x, y, p;
	//u8 qrencode_buff[12];			//存放LCD ID字符串
	EncodeData((char *)qrcode_data);
	//LCD_Fill(10,110,15,115,BLACK);
	LCD_Fill(120, 20, 360, 310, color);
//	sprintf((char*)qrencode_buff,"size:%d",m_nSymbleSize);//将LCD ID打印到lcd_id数组。
//	LCD_ShowString(10,40,200,16,16,qrencode_buff);		//显示LCD ID
	if (m_nSymbleSize * 2 > 240) {
		LCD_ShowString(10, 60, 200, 16, 16, (u8 *)"The QR Code is too large!"); //太大显示不下
		return;
	}
	for (i = 0; i < 10; i++) {
		if ((m_nSymbleSize * i * 2) > 240)	break;
	}
	p = (i - 1) * 2; //点大小
	x = (480 - m_nSymbleSize * p) / 2;
	y = QRCODE_Y;
	//sprintf((char*)qrencode_buff,"piont:%d",p);//将LCD ID打印到lcd_id数组。
	//LCD_ShowString(10,60,200,16,16,qrencode_buff);
	for (i = 0; i < m_nSymbleSize; i++) {
		for (j = 0; j < m_nSymbleSize; j++) {
			//USART1_SendData(m_byModuleData[j][i]);
			if (m_byModuleData[i][j] == 1)
				LCD_Fill(x + p * i, y + p * j, x + p * (i + 1) - 1, y + p * (j + 1) - 1, BLACK);

		}

	}
}


void User_ID_Show(void) { //ID显示



		LCD_Clear(BLACK);





		iot_str_p = strcpy((void*)ID_DISPLAY_BUF, (void*)Iot_Website);
		iot_str_l = strlen((void*)Iot_Website);


		ID_DISPLAY_BUF[iot_str_l + 0] = HTOC((SYS_TYPE & 0xf000) >> 12);
		ID_DISPLAY_BUF[iot_str_l + 1] = HTOC((SYS_TYPE & 0xf00) >> 8);
		ID_DISPLAY_BUF[iot_str_l + 2] = HTOC((SYS_TYPE & 0xf0) >> 4);
		ID_DISPLAY_BUF[iot_str_l + 3] = HTOC((SYS_TYPE & 0xf));

		ID_DISPLAY_BUF[iot_str_l + 4] = HTOC((SYS_EDITION & 0xf0) >> 4);
		ID_DISPLAY_BUF[iot_str_l + 5] = HTOC((SYS_EDITION & 0xf));

		ID_DISPLAY_BUF[iot_str_l + 6] = HTOC((SYS_TYPE_NO & 0xf00000) >> 20);
		ID_DISPLAY_BUF[iot_str_l + 7] = HTOC((SYS_TYPE_NO & 0xf0000) >> 16);
		ID_DISPLAY_BUF[iot_str_l + 8] = HTOC((SYS_TYPE_NO & 0xf000) >> 12);
		ID_DISPLAY_BUF[iot_str_l + 9] = HTOC((SYS_TYPE_NO & 0xf00) >> 8);
		ID_DISPLAY_BUF[iot_str_l + 10] = HTOC((SYS_TYPE_NO & 0xf0) >> 4);
		ID_DISPLAY_BUF[iot_str_l + 11] = HTOC((SYS_TYPE_NO & 0xf));
		ID_DISPLAY_BUF[iot_str_l + 12] = 0;


		DISPLAY_RENCODE_TO_TFT(ID_DISPLAY_BUF, WHITE);


		POINT_COLOR = BLACK;
		BACK_COLOR = WHITE;


		LCD_ShowChar(176 - 40, ID_Show_Y, 'I', 16, 0);
		LCD_ShowChar(176 - 32, ID_Show_Y, 'D', 16, 0);
		LCD_ShowChar(176 - 24, ID_Show_Y, ':', 16, 0);


		LCD_ShowChar(176, ID_Show_Y, SHOW_HTOC((SYS_TYPE & 0xf000) >> 12), 16, 0);
		LCD_ShowChar(176 + 8, ID_Show_Y, SHOW_HTOC((SYS_TYPE & 0xf00) >> 8), 16, 0);

		LCD_ShowChar(176 + 24, ID_Show_Y, SHOW_HTOC((SYS_TYPE & 0xf0) >> 4), 16, 0);
		LCD_ShowChar(176 + 32, ID_Show_Y, SHOW_HTOC((SYS_TYPE & 0xf) >> 0), 16, 0);

		LCD_ShowChar(176 + 48, ID_Show_Y, SHOW_HTOC((SYS_EDITION & 0xf0) >> 4), 16, 0);
		LCD_ShowChar(176 + 56, ID_Show_Y, SHOW_HTOC((SYS_EDITION & 0xf) >> 0), 16, 0);

		LCD_ShowChar(176 + 72, ID_Show_Y, SHOW_HTOC((SYS_TYPE_NO & 0xf00000) >> 20), 16, 0);
		LCD_ShowChar(176 + 80, ID_Show_Y, SHOW_HTOC((SYS_TYPE_NO & 0xf0000) >> 16), 16, 0);

		LCD_ShowChar(176 + 96, ID_Show_Y, SHOW_HTOC((SYS_TYPE_NO & 0xf000) >> 12), 16, 0);
		LCD_ShowChar(176 + 104, ID_Show_Y, SHOW_HTOC((SYS_TYPE_NO & 0xf00) >> 8), 16, 0);

		LCD_ShowChar(176 + 120, ID_Show_Y, SHOW_HTOC((SYS_TYPE_NO & 0xf0) >> 4), 16, 0);
		LCD_ShowChar(176 + 128, ID_Show_Y, SHOW_HTOC((SYS_TYPE_NO & 0xf) >> 0), 16, 0);



	while (f_Sys_Check == 2) {
		
		if (CMachine.UI_Show_No != 8)	break;

		if (CMachine.Key2_vale == 1) {	//IN键按下
			CMachine.Key2_vale = 0; //清零
			CMachine.UI_Show_No = 2; //进入下一界面
			break;//下一页，退出
		}
		User_Public_Show();//公共显示部分

		delay_ms(20);

	}

}

void User_MAC_Show(void) { //MAC显示












}
