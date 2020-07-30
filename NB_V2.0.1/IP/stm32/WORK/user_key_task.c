#include "includes.h"



u16  Key0_No = 0; //按键0,计数值
u8  Key1_No = 0; //按键1
u8  Key2_No = 0; //按键2
u8  Key3_No = 0; //按键3
u16  Key4_No = 0; //按键4，组合键
u8  Key5_No = 0; //按键5，组合键
u8  Key6_No = 0; //按键6，组合键


u8  fKey0_Short = 0; //按键0长按标志
u8  fKey1_Short = 0; //按键1长按标志
u8  fKey2_Short = 0; //按键2长按标志
u8  fKey3_Short = 0; //按键3长按标志
u8  fKey4_Short = 0; //按键4长按标志
u8  fKey5_Short = 0; //按键5长按标志
u8  fKey6_Short = 0; //按键6长按标志



#define KEY_DELAY	5 //延时（10ms）
#define KEY_DELAY_Long	200 //按键长按几秒后有效，延时(2S)


u8 KeyPress = 0;



u16 KeyPress_Double_Time = 0;//双击计时
u16 KeyPress_Double_No = 0;//双击计数

#define DOUBLE_TIME	50 //双击时间



void User_Key_Scan_Task(void *pdata) {

	
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	printf(" User_Key_Scan_Task start \n\r");

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE); //使能PORTA,PORTE时钟




	//按键口中断唤醒
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 //PA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化IO
	//使用外部中断方式
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//中断线0连接GPIOA.0

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	// 初始化外部中断

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器





	//初始化 按键	  浮动输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮动输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA11,GPIOA13




	CMachine.Key1_vale = 0;
	CMachine.Key2_vale = 0;
	CMachine.Key3_vale = 0;


	CMachine.UI_Show_No = 0; //重启初始化为0



	CMachine.uSys_User_Reset = 0; //系统用户重置
	CMachine.uSys_User_Reset_Time = 0; //系统用户重置,计时
	CMachine.uSys_User_Reset_Time_LED_Blink_No = 0; //系统用户重置,计时闪烁




	printf("f_Sys_Check=%d\n\r",f_Sys_Check);
	while (f_Sys_Check == 2) {
		if (Bit_RESET != GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))		KeyPress |= 0x01; //开关按键//开关键，上升沿唤醒的，需平时低
		else	KeyPress &= (~0x01);

		if (Bit_RESET == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))		KeyPress |= 0x04; //模式机键
		else	KeyPress &= (~0x04);

		if (Bit_RESET == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))		KeyPress |= 0x02; //配网按键
		else	KeyPress &= (~0x02);



		if (KeyPress_Double_Time > 0)	KeyPress_Double_Time--;
		else {

			KeyPress_Double_No = 0;
		}


		if (CMachine.uID_Show_Time_No > 0)	CMachine.uID_Show_Time_No--;
		
		//printf("KeyPress=%d\n\r",KeyPress);
		if (KeyPress == 0x00) {
		
			
			if (fKey0_Short == 0 && CMachine.UI_Show_No == 1 && (CMachine.fCharge == 0))	 SYS_POWER_OFF(); //系统关闭





			if (fKey4_Short != 0) {
				fKey4_Short = 0;



			} else if (fKey5_Short != 0) {
				fKey5_Short = 0;



			}
			if (fKey6_Short != 0) {
				fKey6_Short = 0;



			} else	if (fKey0_Short == 1) {
				fKey0_Short = 0;

				CMachine.Key1_vale = 1;



				if (KeyPress_Double_Time > 0) {
					KeyPress_Double_No++;
					if (KeyPress_Double_No >= 4) { //连续击5次
						KeyPress_Double_No = 0;

						if (CMachine.UI_Show_No == 5) {

							CMachine.uCO2_Test_Mode = 5; //CO2较准模式

						}

					}


				}
				KeyPress_Double_Time = DOUBLE_TIME;





			} else if (fKey1_Short == 1) {
				fKey1_Short = 0;

				CMachine.Key2_vale = 1;

			} else if (fKey2_Short == 1) {
				fKey2_Short = 0;

				CMachine.Key3_vale = 1;

				if (CMachine.UI_Show_No > 1) {
					if (uTime_Set_Mode == 0) { //非时间设置模式下
						//		if(CMachine.fRF_Mode==0)	CMachine.fRF_Mode=1;
						//		else	CMachine.fRF_Mode=0;
					}
				}




			} else if (fKey3_Short == 1) {
				fKey3_Short = 0;

				if (CMachine.UI_Show_No == 7) {
					CMachine.uID_Show_Time_No = 200; //2秒
					CMachine.uID_Show_Time_No_B = 1; //
				}

			}





			//长按放开清零
			if (CMachine.Key1_vale == 2)	CMachine.Key1_vale = 0;
			if (CMachine.Key2_vale == 2) CMachine.Key2_vale = 0;
			if (CMachine.Key3_vale == 2) CMachine.Key3_vale = 0;





			Key0_No = 0; //清零
			Key1_No = 0; //清零
			Key2_No = 0; //清零
			Key3_No = 0; //清零
			Key4_No = 0; //清零
			Key5_No = 0; //清零
			Key6_No = 0; //清零

			fKey0_Short = 0; //清零
			fKey1_Short = 0; //清零
			fKey2_Short = 0; //清零
			fKey3_Short = 0; //清零
			fKey4_Short = 0; //清零
			fKey5_Short = 0; //清零
			fKey6_Short = 0; //清零

		} 
		else {

			switch (KeyPress) {
			case 0x01:	//按键0//开关机键
				if (fKey0_Short != 2) Key0_No++;
				if (Key0_No == (KEY_DELAY - 1)) {
					fKey0_Short = 1; //按键1短按

					if (CMachine.UI_Show_No == 0) {
						//SystemInit();	//系统从停止模式唤醒后，需重新初始化时钟
						CMachine.UI_Show_No = 1; //
					}



				}



				if (Key0_No >= 300 && fKey0_Short != 2) { //长按2秒到
					fKey0_Short = 2; //按键没有短按

					//按键长按函数

					if (CMachine.UI_Show_No == 1) {
						//LCD_BACK_ON	//打开背光
						CMachine.fLED_Back = 1;
						CMachine.UI_Show_No = 2; //
						
						printf("LCD_BACK_ON\n\r");

					} else {
						if (CMachine.fCharge != 0) { //充电状态
							CMachine.UI_Show_No = 1; //
						} else {
							CMachine.UI_Show_No = 0; //

							CMachine.uSys_User_Reset = 0; //系统用户重置//清零
							CMachine.uSys_User_Reset_Time = 0; //系统用户重置,计时//清零

							//LCD_BACK_OFF	//关闭背光
							CMachine.fLED_Back = 0;
							printf("LCD_BACK_OFF\n\r");
						}
					}




				}
				break;
			case 0x02:	//按键2//
				if (fKey1_Short != 2) Key1_No++;
				if (Key1_No == (KEY_DELAY - 1)) {
					fKey1_Short = 1; //按键2短按




				}
				if (Key1_No > 200 && fKey1_Short != 2) { //长按2秒到
					fKey1_Short = 2; //按键没有短按

					//按键长按函数
					CMachine.Key2_vale = 2;

				}
				break;
			case 0x04:	//按键3//界面切换键
				if (fKey2_Short != 2) Key2_No++;
				if (Key2_No == (KEY_DELAY - 1)) {
					fKey2_Short = 1; //按键3短按



				}
				if (Key2_No > 200 && fKey2_Short != 2) { //长按2秒到
					fKey2_Short = 2; //按键没有短按

					if (CMachine.UI_Show_No > 1 && uTime_Set_Mode == 0) {	//非时间设置模式下
						CMachine.uSys_User_Reset = 1; //系统用户重置
						CMachine.uSys_User_Reset_Time = 12000; //系统用户重置,计时
						//	Sys_Cntrl.uSys_User_Reset_Time_LED_Blink=300;//系统用户重置,计时闪烁，3秒
					}


				}
				break;
			case 0x07:	//按键4//开关机和模式组合键
				if (fKey3_Short != 2) Key3_No++;
				if (Key3_No == (KEY_DELAY - 1)) {
					fKey3_Short = 1; //按键4短按

				}
				if (Key3_No > 200 && fKey3_Short != 2) { //长按2秒到
					fKey3_Short = 2; //按键没有短按

					//按键长按函数

				}
				break;
			case 0x06:	//语言改变
				if (fKey4_Short != 2) Key4_No++;
				if (Key4_No == (KEY_DELAY - 1)) {
					fKey4_Short = 1; //按键4短按


				}
				if (Key4_No > 400 && fKey4_Short != 2) { //长按4秒到
					fKey4_Short = 2; //按键没有短按

					if (CMachine.UI_Show_No > 1) {
						if (uTime_Set_Mode == 0) { //非时间设置模式下
							if (CMachine.uLanguage_Mode == 0) {
								CMachine.uLanguage_Mode = 0x01;
								AT24CXX_Write((uint8_t*)&CMachine.uLanguage_Mode, SYS_LANGUAGE_ADDR, 3);
							} else {
								CMachine.uLanguage_Mode = 0x00;
								AT24CXX_Write((uint8_t*)&CMachine.uLanguage_Mode, SYS_LANGUAGE_ADDR, 3);
							}
						}
					}
				}

				break;

			case 0x05:	//屏背光
				if (fKey5_Short != 2) Key5_No++;
				if (Key5_No == (KEY_DELAY - 1)) {
					fKey5_Short = 1; //按键5短按


				}
				if (Key5_No > 200 && fKey5_Short != 2) { //长按2秒到
					fKey5_Short = 2; //按键没有短按
					if (CMachine.UI_Show_No > 1) {
						if (CMachine.uLED_Back_No == 0)	CMachine.uLED_Back_No = 1;
						else	CMachine.uLED_Back_No = 0;

						AT24CXX_Write((uint8_t*)&CMachine.uLED_Back_No, SYS_BACK_LIGHT_ADDR, 3);
					}
				}

				break;
			case 0x03:	//重置用户
				if (fKey6_Short != 2) Key6_No++;
				if (Key6_No == (KEY_DELAY - 1)) {
					fKey6_Short = 1; //按键6短按


				}
				if (Key6_No > 200 && fKey6_Short != 2) { //长按2秒到
					fKey6_Short = 2; //按键没有短按



				}

				break;


			default:
				break;
			}

		}

		delay_ms(10);
	}
}








