#include "includes.h"



u16  Key0_No = 0; //����0,����ֵ
u8  Key1_No = 0; //����1
u8  Key2_No = 0; //����2
u8  Key3_No = 0; //����3
u16  Key4_No = 0; //����4����ϼ�
u8  Key5_No = 0; //����5����ϼ�
u8  Key6_No = 0; //����6����ϼ�


u8  fKey0_Short = 0; //����0������־
u8  fKey1_Short = 0; //����1������־
u8  fKey2_Short = 0; //����2������־
u8  fKey3_Short = 0; //����3������־
u8  fKey4_Short = 0; //����4������־
u8  fKey5_Short = 0; //����5������־
u8  fKey6_Short = 0; //����6������־



#define KEY_DELAY	5 //��ʱ��10ms��
#define KEY_DELAY_Long	200 //���������������Ч����ʱ(2S)


u8 KeyPress = 0;



u16 KeyPress_Double_Time = 0;//˫����ʱ
u16 KeyPress_Double_No = 0;//˫������

#define DOUBLE_TIME	50 //˫��ʱ��



void User_Key_Scan_Task(void *pdata) {

	
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	printf(" User_Key_Scan_Task start \n\r");

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PORTA,PORTEʱ��




	//�������жϻ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 //PA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��IO
	//ʹ���ⲿ�жϷ�ʽ
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//�ж���0����GPIOA.0

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//���ð������е��ⲿ��·
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	// ��ʼ���ⲿ�ж�

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���





	//��ʼ�� ����	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA11,GPIOA13




	CMachine.Key1_vale = 0;
	CMachine.Key2_vale = 0;
	CMachine.Key3_vale = 0;


	CMachine.UI_Show_No = 0; //������ʼ��Ϊ0



	CMachine.uSys_User_Reset = 0; //ϵͳ�û�����
	CMachine.uSys_User_Reset_Time = 0; //ϵͳ�û�����,��ʱ
	CMachine.uSys_User_Reset_Time_LED_Blink_No = 0; //ϵͳ�û�����,��ʱ��˸




	printf("f_Sys_Check=%d\n\r",f_Sys_Check);
	while (f_Sys_Check == 2) {
		if (Bit_RESET != GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))		KeyPress |= 0x01; //���ذ���//���ؼ��������ػ��ѵģ���ƽʱ��
		else	KeyPress &= (~0x01);

		if (Bit_RESET == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))		KeyPress |= 0x04; //ģʽ����
		else	KeyPress &= (~0x04);

		if (Bit_RESET == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))		KeyPress |= 0x02; //��������
		else	KeyPress &= (~0x02);



		if (KeyPress_Double_Time > 0)	KeyPress_Double_Time--;
		else {

			KeyPress_Double_No = 0;
		}


		if (CMachine.uID_Show_Time_No > 0)	CMachine.uID_Show_Time_No--;
		
		//printf("KeyPress=%d\n\r",KeyPress);
		if (KeyPress == 0x00) {
		
			
			if (fKey0_Short == 0 && CMachine.UI_Show_No == 1 && (CMachine.fCharge == 0))	 SYS_POWER_OFF(); //ϵͳ�ر�





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
					if (KeyPress_Double_No >= 4) { //������5��
						KeyPress_Double_No = 0;

						if (CMachine.UI_Show_No == 5) {

							CMachine.uCO2_Test_Mode = 5; //CO2��׼ģʽ

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
					if (uTime_Set_Mode == 0) { //��ʱ������ģʽ��
						//		if(CMachine.fRF_Mode==0)	CMachine.fRF_Mode=1;
						//		else	CMachine.fRF_Mode=0;
					}
				}




			} else if (fKey3_Short == 1) {
				fKey3_Short = 0;

				if (CMachine.UI_Show_No == 7) {
					CMachine.uID_Show_Time_No = 200; //2��
					CMachine.uID_Show_Time_No_B = 1; //
				}

			}





			//�����ſ�����
			if (CMachine.Key1_vale == 2)	CMachine.Key1_vale = 0;
			if (CMachine.Key2_vale == 2) CMachine.Key2_vale = 0;
			if (CMachine.Key3_vale == 2) CMachine.Key3_vale = 0;





			Key0_No = 0; //����
			Key1_No = 0; //����
			Key2_No = 0; //����
			Key3_No = 0; //����
			Key4_No = 0; //����
			Key5_No = 0; //����
			Key6_No = 0; //����

			fKey0_Short = 0; //����
			fKey1_Short = 0; //����
			fKey2_Short = 0; //����
			fKey3_Short = 0; //����
			fKey4_Short = 0; //����
			fKey5_Short = 0; //����
			fKey6_Short = 0; //����

		} 
		else {

			switch (KeyPress) {
			case 0x01:	//����0//���ػ���
				if (fKey0_Short != 2) Key0_No++;
				if (Key0_No == (KEY_DELAY - 1)) {
					fKey0_Short = 1; //����1�̰�

					if (CMachine.UI_Show_No == 0) {
						//SystemInit();	//ϵͳ��ֹͣģʽ���Ѻ������³�ʼ��ʱ��
						CMachine.UI_Show_No = 1; //
					}



				}



				if (Key0_No >= 300 && fKey0_Short != 2) { //����2�뵽
					fKey0_Short = 2; //����û�ж̰�

					//������������

					if (CMachine.UI_Show_No == 1) {
						//LCD_BACK_ON	//�򿪱���
						CMachine.fLED_Back = 1;
						CMachine.UI_Show_No = 2; //
						
						printf("LCD_BACK_ON\n\r");

					} else {
						if (CMachine.fCharge != 0) { //���״̬
							CMachine.UI_Show_No = 1; //
						} else {
							CMachine.UI_Show_No = 0; //

							CMachine.uSys_User_Reset = 0; //ϵͳ�û�����//����
							CMachine.uSys_User_Reset_Time = 0; //ϵͳ�û�����,��ʱ//����

							//LCD_BACK_OFF	//�رձ���
							CMachine.fLED_Back = 0;
							printf("LCD_BACK_OFF\n\r");
						}
					}




				}
				break;
			case 0x02:	//����2//
				if (fKey1_Short != 2) Key1_No++;
				if (Key1_No == (KEY_DELAY - 1)) {
					fKey1_Short = 1; //����2�̰�




				}
				if (Key1_No > 200 && fKey1_Short != 2) { //����2�뵽
					fKey1_Short = 2; //����û�ж̰�

					//������������
					CMachine.Key2_vale = 2;

				}
				break;
			case 0x04:	//����3//�����л���
				if (fKey2_Short != 2) Key2_No++;
				if (Key2_No == (KEY_DELAY - 1)) {
					fKey2_Short = 1; //����3�̰�



				}
				if (Key2_No > 200 && fKey2_Short != 2) { //����2�뵽
					fKey2_Short = 2; //����û�ж̰�

					if (CMachine.UI_Show_No > 1 && uTime_Set_Mode == 0) {	//��ʱ������ģʽ��
						CMachine.uSys_User_Reset = 1; //ϵͳ�û�����
						CMachine.uSys_User_Reset_Time = 12000; //ϵͳ�û�����,��ʱ
						//	Sys_Cntrl.uSys_User_Reset_Time_LED_Blink=300;//ϵͳ�û�����,��ʱ��˸��3��
					}


				}
				break;
			case 0x07:	//����4//���ػ���ģʽ��ϼ�
				if (fKey3_Short != 2) Key3_No++;
				if (Key3_No == (KEY_DELAY - 1)) {
					fKey3_Short = 1; //����4�̰�

				}
				if (Key3_No > 200 && fKey3_Short != 2) { //����2�뵽
					fKey3_Short = 2; //����û�ж̰�

					//������������

				}
				break;
			case 0x06:	//���Ըı�
				if (fKey4_Short != 2) Key4_No++;
				if (Key4_No == (KEY_DELAY - 1)) {
					fKey4_Short = 1; //����4�̰�


				}
				if (Key4_No > 400 && fKey4_Short != 2) { //����4�뵽
					fKey4_Short = 2; //����û�ж̰�

					if (CMachine.UI_Show_No > 1) {
						if (uTime_Set_Mode == 0) { //��ʱ������ģʽ��
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

			case 0x05:	//������
				if (fKey5_Short != 2) Key5_No++;
				if (Key5_No == (KEY_DELAY - 1)) {
					fKey5_Short = 1; //����5�̰�


				}
				if (Key5_No > 200 && fKey5_Short != 2) { //����2�뵽
					fKey5_Short = 2; //����û�ж̰�
					if (CMachine.UI_Show_No > 1) {
						if (CMachine.uLED_Back_No == 0)	CMachine.uLED_Back_No = 1;
						else	CMachine.uLED_Back_No = 0;

						AT24CXX_Write((uint8_t*)&CMachine.uLED_Back_No, SYS_BACK_LIGHT_ADDR, 3);
					}
				}

				break;
			case 0x03:	//�����û�
				if (fKey6_Short != 2) Key6_No++;
				if (Key6_No == (KEY_DELAY - 1)) {
					fKey6_Short = 1; //����6�̰�


				}
				if (Key6_No > 200 && fKey6_Short != 2) { //����2�뵽
					fKey6_Short = 2; //����û�ж̰�



				}

				break;


			default:
				break;
			}

		}

		delay_ms(10);
	}
}








