#include "includes.h"


u8 uTest_Mode_Time = 0; //���ģʽʱ�䣬1��

void Uart1_Decode(void) {
	uUart1_Recive_Length = 0;


	if (uUart1_Recive_Data[0] == 0x42 && uUart1_Recive_Data[1] == 0x4d) {

		CMachine.pm25 = ((uUart1_Recive_Data[12] << 8) | uUart1_Recive_Data[13]);

	}


}


void Uart3_Decode(void) {
	uUart3_Recive_Length = 0;
	if (uUart3_Recive_Data[0] == 0xFE && uUart3_Recive_Data[1] == 0x04 && uUart3_Recive_Data[2] == 0x02) {
		CMachine.co2 = ((uUart3_Recive_Data[3] << 8) | uUart3_Recive_Data[4]);;

	}
}


unsigned int	uHCHO_S_Buf = 0; //

void Uart4_Decode(void) {

	uUart4_Recive_Length = 0;

// CMachine.pm25=uUart4_Recive_Data[0];
// CMachine.temp=uUart4_Recive_Data[1];
// CMachine.humidity=uUart4_Recive_Data[2];
//
//
// CMachine.hcho=uUart4_Recive_Data[3];

//CMachine.tvoc=uUart4_Recive_Data[5];
//CMachine.co2=uUart4_Recive_Data[8];
//

	if (uUart4_Recive_Data[0] == 0xff && uUart4_Recive_Data[1] == 0x17) {
//uHCHO_S_Buf=(uUart4_Recive_Data[4]<<8);
//uHCHO_S_Buf+=uUart4_Recive_Data[5];

		uHCHO_S_Buf = ((uUart4_Recive_Data[4] << 8) | uUart4_Recive_Data[5]);

		//ת��//
		uHCHO_S_Buf *= 134;
		uHCHO_S_Buf /= 100;

		//uHCHO_S_Buf/=3;	//���ҽ������3


//uHCHO_S_Former_TVOC=uHCHO_S_Buf;//

		CMachine.hcho = uHCHO_S_Buf; //


	}


}



u8  fInit_PM25 = 0; //��һ�γ�ʼ����־
unsigned short UI_Show_No_B_Sensor_Task = 0xff; //
char User_All_Sensor_Thread(thread_t* pt) { //���д������������
	Thread_BEGIN {
		GPIO_InitTypeDef GPIO_InitStructure;


		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
		while (1) {



			if (CMachine.UI_Show_No <= 1) {

				if (UI_Show_No_B_Sensor_Task != 0) {


					//USART3_TX   PB.10    //USART3_RX	  PB.11
					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //PB.10
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
					GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PA2
					GPIO_ResetBits(GPIOB, GPIO_Pin_10);
					GPIO_ResetBits(GPIOB, GPIO_Pin_11);

					//USART1_TX   PA.9    //USART1_RX	  PA.10
					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //PA.9
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
					GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
					GPIO_ResetBits(GPIOA, GPIO_Pin_9);
					GPIO_ResetBits(GPIOA, GPIO_Pin_10);


					//USART4_TX   PC.10    //USART4_RX	  PC.11
					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //PC.10
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
					GPIO_Init(GPIOC, &GPIO_InitStructure);
					GPIO_ResetBits(GPIOC, GPIO_Pin_10);
					GPIO_ResetBits(GPIOC, GPIO_Pin_11);



					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
					GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
					GPIO_ResetBits(GPIOA, GPIO_Pin_8); 	//�µ�


					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
					GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB15
					TVOC_POWER_OFF  	//��TVOC��ѹ


					UI_Show_No_B_Sensor_Task = 0;
				}

				Thread_Sleep(10);
				continue;//�˳�


			} else {


				if (UI_Show_No_B_Sensor_Task == 0) {



					uart3_init(9600);//���CO2

					uart1_init(9600);//���ٷ۳�������
					uart2_init(9600);//���ٷ۳�������

					//uart5_init(9600);//
					uart4_init(9600);//��ȩ

//     //USART4_TX   PC.10
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11; //PC.10
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
//    GPIO_Init(GPIOC, &GPIO_InitStructure);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_10); 	//
//		GPIO_ResetBits(GPIOC,GPIO_Pin_11); 	//



					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
					GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
					GPIO_SetBits(GPIOA, GPIO_Pin_8); 	//�ϵ�



					SHT21_Init();
					User_IAQ2000_Init();//��������

					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
					GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB15

					TVOC_POWER_ON //������ѹ


					CMachine.uTVOC_Res_Start_Time_No = 0; //TVOC��ѹ����ʱ�����


					UI_Show_No_B_Sensor_Task = 0xff;

					CMachine.uCO2_Test_Mode = 0; //



				}




			}








			if (uUart1_Recive_End > 0) {

				uUart1_Recive_End--;
				if (uUart1_Recive_End == 0) 		  Uart1_Decode(); //���ڽ��ս���


			}

			if (uUart3_Recive_End > 0) {

				uUart3_Recive_End--;
				if (uUart3_Recive_End == 0) 		  Uart3_Decode(); //���ڽ��ս���


			}

			if (uUart4_Recive_End > 0) {

				uUart4_Recive_End--;
				if (uUart4_Recive_End == 0) 		  Uart4_Decode(); //���ڽ��ս���


			}


			uTest_Mode_Time++;
			if (uTest_Mode_Time > 99)	uTest_Mode_Time = 0;







			if (uTest_Mode_Time == 0) {





				if (Sys_Cntrl.uLine_Work == 1) { //����״̬��
					if (Sys_Time_WIFI.w_year != (calendar.w_year - 2000) || Sys_Time_WIFI.w_month != calendar.w_month || Sys_Time_WIFI.w_date != calendar.w_date || Sys_Time_WIFI.hour != calendar.hour || Sys_Time_WIFI.min != calendar.min) { //����2����
						if (uTime_Set_Mode == 0) {

							if (((calendar.w_year - 2000) != Sys_Time_WIFI.w_year) ||
							        (calendar.w_month != Sys_Time_WIFI.w_month) ||
							        (calendar.w_date != Sys_Time_WIFI.w_date) ||
							        (calendar.hour != Sys_Time_WIFI.hour) ||
							        ((Sys_Time_WIFI.min > calendar.min) && (Sys_Time_WIFI.min - calendar.min > 2)) ||
							        ((calendar.min > Sys_Time_WIFI.min) && (calendar.min - Sys_Time_WIFI.min > 2))
							   ) {


								Sys_Time_Save.w_year = Sys_Time_WIFI.w_year + 2000;
								Sys_Time_Save.w_month = Sys_Time_WIFI.w_month;
								Sys_Time_Save.w_date = Sys_Time_WIFI.w_date;
								Sys_Time_Save.hour = Sys_Time_WIFI.hour;
								Sys_Time_Save.min = Sys_Time_WIFI.min;
								Sys_Time_Save.sec = 0;

								RTC_Set(Sys_Time_Save.w_year, Sys_Time_Save.w_month, Sys_Time_Save.w_date, Sys_Time_Save.hour, Sys_Time_Save.min, Sys_Time_Save.sec);
//	 User_Test_Data_Check();//���ݽ�׼������ʱ��
							}

						}

					}
				}

				if (uTime_Set_Mode == 0) {
					RTC_Get();//����ʱ��
				}






				if (calendar.hour == 6 && calendar.min == 0 && CMachine.uTVOC_Res_Start_Time_No == 0)	CMachine.uTVOC_Res_Start_Time_No = 180; //����

///////////////////////////////////////  TVOC ///////////////////////////////////////////




				if (CMachine.uTVOC_Res_Start_Time_No == 180) {
					TVOC_POWER_OFF//��TVOC��ѹ

				}
				if (CMachine.uTVOC_Res_Start_Time_No == 10) {

					TVOC_POWER_ON//��TVOC��ѹ

				}
				if (CMachine.uTVOC_Res_Start_Time_No == 0) {



				}


				if (CMachine.uTVOC_Res_Start_Time_No > 0)	CMachine.uTVOC_Res_Start_Time_No--;





				User_SHT20_Task();//��ȡ��ʪ����������
				User_IAQ2000_Read();//��������




///////////////////////////////////////  PM2.5 ///////////////////////////////////////////
				/*	if(fInit_PM25==0)
					{
						fInit_PM25=1;

				uUart3_Send_Data[0]=0x11;
				uUart3_Send_Data[1]=0x03;
				uUart3_Send_Data[2]=0x0C;
				uUart3_Send_Data[3]=0x02;
				uUart3_Send_Data[4]=0x1E;
				uUart3_Send_Data[5]=0xC0;


				User_UART3_SendStringx(&uUart3_Send_Data[0],6);

					}else
					{


				uUart3_Send_Data[0]=0x11;
				uUart3_Send_Data[1]=0x02;
				uUart3_Send_Data[2]=0x0B;
				uUart3_Send_Data[3]=0x01;
				uUart3_Send_Data[4]=0xE1;


				User_UART3_SendStringx(&uUart3_Send_Data[0],5);


				}*/

///////////////////////////////////////  CO2 ///////////////////////////////////////////
				if (CMachine.uCO2_Test_Mode == 5) {

					uUart3_Send_Data[0] = 0xFE;
					uUart3_Send_Data[1] = 0x06;
					uUart3_Send_Data[2] = 0x00;
					uUart3_Send_Data[3] = 0x00;
					uUart3_Send_Data[4] = 0x00;
					uUart3_Send_Data[5] = 0x00;
					uUart3_Send_Data[6] = 0x9D;
					uUart3_Send_Data[7] = 0xC5;

					User_UART3_SendStringx((char *)&uUart3_Send_Data[0], 8);


				} else	if (CMachine.uCO2_Test_Mode == 4) {

					uUart3_Send_Data[0] = 0xFE;
					uUart3_Send_Data[1] = 0x06;
					uUart3_Send_Data[2] = 0x00;
					uUart3_Send_Data[3] = 0x01;
					uUart3_Send_Data[4] = 0x7c;
					uUart3_Send_Data[5] = 0x06;
					uUart3_Send_Data[6] = 0x6c;
					uUart3_Send_Data[7] = 0xc7;

					User_UART3_SendStringx((char *)&uUart3_Send_Data[0], 8);

				} else	if (CMachine.uCO2_Test_Mode == 1) {

					uUart3_Send_Data[0] = 0xFE;
					uUart3_Send_Data[1] = 0x03;
					uUart3_Send_Data[2] = 0x00;
					uUart3_Send_Data[3] = 0x00;
					uUart3_Send_Data[4] = 0x00;
					uUart3_Send_Data[5] = 0x01;
					uUart3_Send_Data[6] = 0x90;
					uUart3_Send_Data[7] = 0x05;

					User_UART3_SendStringx((char *)&uUart3_Send_Data[0], 8);


				} else	if (CMachine.uCO2_Test_Mode == 0) {
					uUart3_Send_Data[0] = 0xFE;
					uUart3_Send_Data[1] = 0x04;
					uUart3_Send_Data[2] = 0x00;
					uUart3_Send_Data[3] = 0x03;
					uUart3_Send_Data[4] = 0x00;
					uUart3_Send_Data[5] = 0x01;
					uUart3_Send_Data[6] = 0xD5;
					uUart3_Send_Data[7] = 0xC5;

					User_UART3_SendStringx((char *)&uUart3_Send_Data[0], 8);
				}


				if (CMachine.uCO2_Test_Mode > 0) CMachine.uCO2_Test_Mode--;


///////////////////////////////////////  ��ȩ ///////////////////////////////////////////








			}





			Thread_Sleep(10);
		}
	}
	Thread_END

///////////////////////////////////////  �ж���Ⱦ�ȼ�     ///////////////////////////////////////////


	/*
	��ȩ��0-0.03 ��ɫ  0.03-0.1 ��ɫ 0.1�����Ǻ�ɫ


	TVOC: 0-0.3 ��ɫ  0.3-0.6 ��ɫ 0.6���� ��ɫ


	pm2.5��0-15 ��ɫ 15-75 ��ɫ 75���Ϻ�ɫ


	CO2��280-800 ��ɫ 800-1000 ��ɫ 1000���� ��ɫ
	*/


	/*		 	uAir_Strength=0;
		 if(uSensorData.sensor_data_struct.uHCHO<=30)	uAir_Strength=0;
		 if(uSensorData.sensor_data_struct.uTVOC<=300)	uAir_Strength=0;
		 if(uSensorData.sensor_data_struct.uCO2<=800)	uAir_Strength=0;
		 if(uSensorData.sensor_data_struct.uPM25<=15)uAir_Strength=0;

		 if(uSensorData.sensor_data_struct.uHCHO>30&&uSensorData.sensor_data_struct.uHCHO<=100) uAir_Strength=1;
		 if(uSensorData.sensor_data_struct.uTVOC>300&&uSensorData.sensor_data_struct.uTVOC<=600)	uAir_Strength=1;
		 if(uSensorData.sensor_data_struct.uCO2>800&&uSensorData.sensor_data_struct.uCO2<=1000)	uAir_Strength=1;
		 if(uSensorData.sensor_data_struct.uPM25>15&&uSensorData.sensor_data_struct.uPM25<=75)	uAir_Strength=1;

		 if(uSensorData.sensor_data_struct.uHCHO>100)	uAir_Strength=2;
		 if(uSensorData.sensor_data_struct.uTVOC>600) uAir_Strength=2;
		 if(uSensorData.sensor_data_struct.uCO2>1000)	uAir_Strength=2;
		 if(uSensorData.sensor_data_struct.uPM25>75)	uAir_Strength=2;
	*/

}












