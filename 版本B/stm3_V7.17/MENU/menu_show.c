#include "includes.h"	   
#include "qr_encode.h"



u8 uCharge_Blink_No=0;//�����˸����


u8 uBattery_Show=0;

u8 uRF_Blink_No=0;//RF��˸����
u8 uNB_Blink_No=0;//NB��˸����




u8	fRF_Mode_Show=0;//RFͼ����ʾ֮ǰ


void User_Public_Show(void)//������ʾ����
{

	
	if(CMachine.UI_Show_No!=CMachine_B.UI_Show_No)
	{
	
		
		CMachine_B.UI_Show_No=CMachine.UI_Show_No;
		
		
		
		
		

// icon_show(36, 20, 60, 25, gImage_RHT_03,1);//��ʾ
		

 //icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ		
	
			//	icon_show(150, 15, 79, 29, gImage_NB_033_03,1);//��ʾ	

		
		
		
		uCharge_Blink_No=1;
		
		uBattery_Show=0xff;
		
		
		CMachine.Battery_Low_Power_Off_No=0;//����
		
		
	
		Sys_Cntrl_B.uLine_Work=0xff;
		Sys_Cntrl_B.uSys_User_Reset=0xff;	
			fRF_Mode_Show=0xff;				
		
		
		
	}

	
	
	

	

	
		
	
	
if(CMachine.uID_Show_Time_No==0)	
{
	
	
	
//	if(CMachine.fRF_Mode==0)		
//	{
//		if(fRF_Mode_Show!=CMachine.fRF_Mode)
//		{
//			fRF_Mode_Show=CMachine.fRF_Mode;				
//			icon_show(300, 15, 45, 30, gImage_RF4,1);//��ʾ	
//		}								
//		uRF_Blink_No=0;
//	}
//	else	
//	{
//			fRF_Mode_Show=CMachine.fRF_Mode;					
//		uRF_Blink_No++;
//		if(uRF_Blink_No>99)	uRF_Blink_No=0;
//		if(uRF_Blink_No==1)		 icon_show(300, 15, 45, 30, gImage_RF1,1);//��ʾ		
//		else if(uRF_Blink_No==25)		 icon_show(300, 15, 45, 30, gImage_RF4,1);//��ʾ		
//		else if(uRF_Blink_No==50)		 icon_show(300, 15, 45, 30, gImage_RF1,1);//��ʾ				
//		else if(uRF_Blink_No==75)		 icon_show(300, 15, 45, 30, gImage_RF4,1);//��ʾ				
//	
//	}		
	
	
	
						if(Sys_Cntrl.uSys_User_Reset==1)
						{
							
							Sys_Cntrl_B.uSys_User_Reset=Sys_Cntrl.uSys_User_Reset;
							
							Sys_Cntrl.uSys_User_Reset_Time_LED_Blink_No++;
							if(Sys_Cntrl.uSys_User_Reset_Time_LED_Blink_No>80)	Sys_Cntrl.uSys_User_Reset_Time_LED_Blink_No=0;
							
							if(Sys_Cntrl.uSys_User_Reset_Time_LED_Blink_No==1)		 
							{
								///if(CMachine.uLOGO_Mode==0)	icon_show(36, 20, 60, 25, gImage_RHT_03,1);//��ʾ
								//else	icon_show(36, 20, 60, 25, gImage_MOLA_03,1);//��ʾ
								
								if(CMachine.uLOGO_Mode==0)	
								{
								  if(CMachine.uLOGO_Mode_Clear==0)  
									{
									   LCD_Fill(36,18,36+58,17+28,BLACK);
									}	
									icon_show(42, 20, 46, 22, gImage_AIR,1);//��ʾ	
									CMachine.uLOGO_Mode_Clear=1;
	
								}
								else{
									if(CMachine.uLOGO_Mode_Clear==1)  
									{
									   LCD_Fill(42,20,42+46,22,BLACK);
									}
								icon_show(36, 17, 58, 28, gImage_PiaoAi,1);
								CMachine.uLOGO_Mode_Clear=0;		
								}//��ʾ
							}
							else	if(Sys_Cntrl.uSys_User_Reset_Time_LED_Blink_No==40)	 
							{
								//if(CMachine.uLOGO_Mode==0)	icon_show(36, 21, 60, 25, gImage_RHT_03,0);//��ʾ
								//else	icon_show(36, 20, 60, 25, gImage_MOLA_03,0);//��ʾ
								//if(CMachine.uLOGO_Mode==0)	icon_show(42, 21, 46, 22, gImage_AIR,1);//��ʾ
								//else	icon_show(36, 18, 58, 28, gImage_PiaoAi,1);//��ʾ
								
								if(CMachine.uLOGO_Mode==0)	
								{
								  if(CMachine.uLOGO_Mode_Clear==0)  
									{
									   LCD_Fill(36,17,36+58,17+28,BLACK);
									}	
									icon_show(42, 20, 46, 22, gImage_AIR,1);//��ʾ	
									CMachine.uLOGO_Mode_Clear=1;
	
								}
								else{
									if(CMachine.uLOGO_Mode_Clear==1)  
									{
									   LCD_Fill(42,20,42+46,22,BLACK);
									}
								icon_show(36, 17, 58, 28, gImage_PiaoAi,1);
								CMachine.uLOGO_Mode_Clear=0;		
								}//��ʾ
								
							}								
	
						}else
						{
							
							if(Sys_Cntrl_B.uSys_User_Reset!=Sys_Cntrl.uSys_User_Reset)
							{
								Sys_Cntrl_B.uSys_User_Reset=Sys_Cntrl.uSys_User_Reset;							
								Sys_Cntrl.uSys_User_Reset_Time_LED_Blink_No=0;//
								//if(CMachine.uLOGO_Mode==0)	icon_show(36, 20, 60, 25, gImage_RHT_03,1);//��ʾ
								//else	icon_show(36, 20, 60, 25, gImage_MOLA_03,1);//��ʾ	
               // if(CMachine.uLOGO_Mode==0)	icon_show(42, 21, 46, 22, gImage_AIR,1);//��ʾ
								//else	icon_show(36, 17, 58, 28, gImage_PiaoAi,1);//��ʾ
								
								if(CMachine.uLOGO_Mode==0)	
								{
								  if(CMachine.uLOGO_Mode_Clear==0)  
									{
									   LCD_Fill(36,17,36+58,17+28,BLACK);
									}	
									icon_show(42, 20, 46, 22, gImage_AIR,1);//��ʾ	
									CMachine.uLOGO_Mode_Clear=1;
	
								}
								else{
									if(CMachine.uLOGO_Mode_Clear==1)  
									{
									   LCD_Fill(42,20,42+46,22,BLACK);
									}
								icon_show(36, 17, 58, 28, gImage_PiaoAi,1);
								CMachine.uLOGO_Mode_Clear=0;		
								}//��ʾ
							}
							if(CMachine.uLOGO_Mode_B!=CMachine.uLOGO_Mode)
							{
								CMachine.uLOGO_Mode_B=CMachine.uLOGO_Mode;						
								Sys_Cntrl.uSys_User_Reset_Time_LED_Blink_No=0;//
								//if(CMachine.uLOGO_Mode==0)	icon_show(36, 20, 60, 25, gImage_RHT_03,1);//��ʾ
								//else	icon_show(36, 20, 60, 25, gImage_MOLA_03,1);//��ʾ
								//if(CMachine.uLOGO_Mode==0)	icon_show(42, 21, 46, 22, gImage_AIR,1);//��ʾ
								//else	icon_show(36, 17, 58, 28, gImage_PiaoAi,1);//��ʾ
								
								if(CMachine.uLOGO_Mode==0)	
								{
								  if(CMachine.uLOGO_Mode_Clear==0)  
									{
									   LCD_Fill(36,17,36+58,17+28,BLACK);
									}	
									icon_show(42, 20, 46, 22, gImage_AIR,1);//��ʾ	
									CMachine.uLOGO_Mode_Clear=1;
	
								}
								else{
									if(CMachine.uLOGO_Mode_Clear==1)  
									{
									   LCD_Fill(42,20,42+46,22,BLACK);
									}
								icon_show(36, 17, 58, 28, gImage_PiaoAi,1);
								CMachine.uLOGO_Mode_Clear=0;		
								}//��ʾ
							}
						}					
	
	
if(Sys_Cntrl.uLine_Work!=0)		
	{
		
		
		if(Sys_Cntrl_B.uLine_Work!=Sys_Cntrl.uLine_Work)
		{
			Sys_Cntrl_B.uLine_Work=Sys_Cntrl.uLine_Work;
		icon_show(150, 15, 79, 29, gImage_NB_033_03,1);//��ʾ	
		}
		
		uNB_Blink_No=0;
	}
	else	
	{
		Sys_Cntrl_B.uLine_Work=Sys_Cntrl.uLine_Work;
		
		uNB_Blink_No++;
		if(uNB_Blink_No>=50)	uNB_Blink_No=0;
		if(uNB_Blink_No==1)		 icon_show(150, 15, 79, 29, gImage_NB_031_03,1);//��ʾ	
		else if(uNB_Blink_No==25)		 icon_show(150, 15, 79, 29, gImage_NB_033_03,1);//��ʾ	
		
		
		
	}	
}else
{

		Sys_Cntrl_B.uLine_Work=0xff;
		Sys_Cntrl_B.uSys_User_Reset=0xff;	
			fRF_Mode_Show=0xff;				
}
	
	

	

	
	
	
	

	
	if(CMachine.fCharge==1)
	{

				uBattery_Show=0xff;//ˢ��
		if(CMachine.Battery==100)	
		{	
			//if(uCharge_Blink_No!=0)	icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ					
			icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ
			LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
			LCD_Fill(394+9+11,16+8,394+15+11,16+19,WHITE);
			LCD_Fill(394+9+11+11,16+8,394+15+11+11,16+19,WHITE);
			
			uCharge_Blink_No=0;			
		}else
		{
			if(uCharge_Blink_No==1)	 
			{
				icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ		
			}
			if(uCharge_Blink_No==20)	
			{
				//icon_show(394, 16, 49, 26, gImage_battery_01,1);//��ʾ				  		 		
			  
				icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ

				LCD_Fill(394+9,16+8,394+15,16+19,WHITE);			
			
			}
			if(uCharge_Blink_No==40)		
			{
				//icon_show(394, 16, 49, 26, gImage_battery_02,1);//��ʾ				  	 			 		
			  
			  					icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ
//					LCD_Fill(394+8+5+5,16+7,394+8+8-1+5+5,16+13+7-1,WHITE);	//��ʾ
					LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
					LCD_Fill(394+9+11,16+8,394+15+11,16+19,WHITE);
//					LCD_Fill(394+9+11+11,16+8,394+15+11+11,16+19,WHITE);	
			
			}
			if(uCharge_Blink_No==60)	
			{
				//icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ				  			
			
			  	icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ
					LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
					LCD_Fill(394+9+11,16+8,394+15+11,16+19,WHITE);
					LCD_Fill(394+9+11+11,16+8,394+15+11+11,16+19,WHITE);
			}			
			uCharge_Blink_No++;
			if(uCharge_Blink_No>80)	uCharge_Blink_No=1;
		}
	}else
	{
	
	//	if(fCharge_b==1)	 icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ		
	
		
			if(CMachine.Battery<10)		
			{
				if(uCharge_Blink_No==1)	 
				{
					icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ						
				}
				if(uCharge_Blink_No==20)	
				{
					//icon_show(394, 16, 49, 26, gImage_battery_03,0);//��ʾ				  	
				
				    icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ
						LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
						LCD_Fill(394+9+11,16+8,394+15+11,16+19,WHITE);
						LCD_Fill(394+9+11+11,16+8,394+15+11+11,16+19,WHITE);
				}
				if(uCharge_Blink_No==40)		
				{
					icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ								
				}
				if(uCharge_Blink_No==60)	
				{
				//	icon_show(394, 16, 49, 26, gImage_battery_03,0);//��ʾ				  			
				    icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ
						LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
						LCD_Fill(394+9+11,16+8,394+15+11,16+19,WHITE);
						LCD_Fill(394+9+11+11,16+8,394+15+11+11,16+19,WHITE);	
				
				}			
				uCharge_Blink_No++;
				if(uCharge_Blink_No>80)	uCharge_Blink_No=1;
				uBattery_Show=CMachine.Battery;
				
				if(CMachine.Battery==0)		
				{							
					CMachine.Battery_Low_Power_Off_No++;//����
					if(CMachine.Battery_Low_Power_Off_No>=160)//�͵�ػ�
					{
						printf("SYS_POWER_OFF_3\n\r");
						SYS_POWER_OFF();//ϵͳ�ر�		
					}
				}
			}else
			{
				if(uBattery_Show!=CMachine.Battery)
				{
					uBattery_Show=CMachine.Battery;
					if(CMachine.Battery>80)	 
					{
						//icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ		
					  icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ
						LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
						LCD_Fill(394+9+11,16+8,394+15+11,16+19,WHITE);
						LCD_Fill(394+9+11+11,16+8,394+15+11+11,16+19,WHITE);
					
					}else
					if(CMachine.Battery>60)	
					{
						//icon_show(394, 16, 49, 26, gImage_battery_02,1);//��ʾ				   		  	 		 		
					
					  icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ
						LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
						LCD_Fill(394+9+11,16+8,394+15+11,16+19,WHITE);
						
					}else
					if(CMachine.Battery>40)		
					{
						//icon_show(394, 16, 49, 26, gImage_battery_01,1);//��ʾ				  	  			 							
					
					  icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ

						LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
					}else
					if(CMachine.Battery>20)		
					{
						icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ				 					
					}		
				}				
			}
		
		
		


	}
		






	
	
	
}



unsigned short UI_Show_No_B_Show_Task=0xff;//
char User_Menu_Show_Thread(thread_t* pt)//�˵�ͼ����ʾ����
{
	
	
	
     Thread_BEGIN
    {
				
	

	
//	dprintf_WORD16_16(192,20,"�����Ⱦ",3,0);

	



 







			 
		while(1)
	{
		
		
		
		
		
		
if(CMachine.UI_Show_No<1)
		{
			
			if(UI_Show_No_B_Show_Task!=0)
			{


					
 LCD_UnInit();
				
			UI_Show_No_B_Show_Task=0;
			}
			
			 		Thread_Sleep(10);
					continue;//�˳�
			
			
		}else
		{
		
			
			if(UI_Show_No_B_Show_Task==0)
			{
			
				
		LCD_Init();
		
		
	//	CMachine.UI_Show_No=2;//
CMachine_B.UI_Show_No=0xff;//
			
				
			
			UI_Show_No_B_Show_Task=0xff;
			}
						
		
		
		
		}
				
		
		
		
		
		
		
		
		
//		
//	switch(CMachine.UI_Show_No)
//	{
//		case 0:	 

//			break;
//		case 1:	 
//				Charge_Off_Show();//�ػ���������ʾ
//			break;		
//		case 2:	
// All_Show();//���в���������ʾ			

//			break;			
//		case 3:	
//			
//	 PM25_Show();//PM2.5������ʾ		
//			break;	
//		case 4:	

// TVOC_Show();//TVOC������ʾ
//			break;
//		case 5:	
//			
// CO2_Show();//TVOC������ʾ
//			break;			
//		case 6:	
// HCHO_Show();//��ȩ������ʾ			

//			break;			
//		case 7:				
//				Time_Show();//ʱ�������ʾ		

//			break;				
//		default:
//			break;
//	}
//		
//		
//		
		
		
		

		
		
		
		
		
		
		
		
		
		
		Thread_Sleep(10);

	}
	



    }
    Thread_END






}


u8 SHOW_HTOC(u8	Pdata)//
{
	u8 ctoh_b = 0;
	
	switch((Pdata&0x0f))
	{
		case	0xf:
			ctoh_b='F';
			break;
		case	0xe:
			ctoh_b='E';			
			break;
		case	0xd:
			ctoh_b='D';			
			break;		
		case	0xc:
			ctoh_b='C';			
			break;
		case	0xb:
			ctoh_b='B';			
			break;
		case	0xa:
			ctoh_b='A';			
			break;		
		case	0x9:
			ctoh_b='9';			
			break;
		case	0x8:
			ctoh_b='8';			
			break;
		case	0x7:
			ctoh_b='7';			
			break;		
		case	0x6:
			ctoh_b='6';			
			break;
		case	0x5:
			ctoh_b='5';			
			break;
		case	0x4:
			ctoh_b='4';			
			break;				
		case	0x3:
			ctoh_b='3';			
			break;		
		case	0x2:
			ctoh_b='2';			
			break;
		case	0x1:
			ctoh_b='1';			
			break;
		case	0x0:
			ctoh_b='0';			
			break;				
		default:
			break;
		
	}	
	
	return ctoh_b;
}






#define QRCODE_Y 	70		//TFT��ά����ʾ����y


u8 Iot_Website[]="https://m.poscn.com/iot/linkcode?id=1_";
u8 ID_DISPLAY_BUF[80];     //
char * iot_str_p;//�ַ���ָ��//
u8 iot_str_l=0;//�ַ�����


#define ID_Show_Y	45
#define MAC_Show_Y 45

void DISPLAY_RENCODE_TO_TFT(u8 *qrcode_data,u16 color)
{
	u8 i,j;
	u16 x,y,p;
	//u8 qrencode_buff[12];			//���LCD ID�ַ���
	EncodeData((char *)qrcode_data);
	//LCD_Fill(10,110,15,115,BLACK);
	LCD_Fill(120,20,360,310,color);
//	sprintf((char*)qrencode_buff,"size:%d",m_nSymbleSize);//��LCD ID��ӡ��lcd_id���顣
//	LCD_ShowString(10,40,200,16,16,qrencode_buff);		//��ʾLCD ID	 
	if(m_nSymbleSize*2>240)	
	{
		LCD_ShowString(10,60,200,16,16,(u8 *)"The QR Code is too large!");//̫����ʾ����
		return;
	}
	for(i=0;i<10;i++)
	{
		if((m_nSymbleSize*i*2)>240)	break;
	}
	p=(i-1)*2;//���С
	x=(480-m_nSymbleSize*p)/2;
	y=QRCODE_Y;
	//sprintf((char*)qrencode_buff,"piont:%d",p);//��LCD ID��ӡ��lcd_id���顣
	//LCD_ShowString(10,60,200,16,16,qrencode_buff);
	for(i=0;i<m_nSymbleSize;i++)
	{
		for(j=0;j<m_nSymbleSize;j++)
		{
			//USART1_SendData(m_byModuleData[j][i]);
			if(m_byModuleData[i][j]==1)
				LCD_Fill(x+p*i,y+p*j,x+p*(i+1)-1,y+p*(j+1)-1,BLACK);

		}
			
	}
}


void User_ID_Show(void)//ID��ʾ
{



			
			if(CMachine.uID_Show_Time_No_B==1)
			{
				CMachine.uID_Show_Time_No_B=2;
			
			 	 LCD_Clear(BLACK);
		
				


				
			iot_str_p=strcpy((void*)ID_DISPLAY_BUF,(void*)Iot_Website);	
			iot_str_l=strlen((void*)Iot_Website);
				
				
				ID_DISPLAY_BUF[iot_str_l+0]=HTOC((SYS_TYPE&0xf000)>>12);
				ID_DISPLAY_BUF[iot_str_l+1]=HTOC((SYS_TYPE&0xf00)>>8);
				ID_DISPLAY_BUF[iot_str_l+2]=HTOC((SYS_TYPE&0xf0)>>4);
				ID_DISPLAY_BUF[iot_str_l+3]=HTOC((SYS_TYPE&0xf));

				ID_DISPLAY_BUF[iot_str_l+4]=HTOC((SYS_EDITION&0xf0)>>4);
				ID_DISPLAY_BUF[iot_str_l+5]=HTOC((SYS_EDITION&0xf));				

				ID_DISPLAY_BUF[iot_str_l+6]=HTOC((SYS_TYPE_NO&0xf00000)>>20);
				ID_DISPLAY_BUF[iot_str_l+7]=HTOC((SYS_TYPE_NO&0xf0000)>>16);				
				ID_DISPLAY_BUF[iot_str_l+8]=HTOC((SYS_TYPE_NO&0xf000)>>12);
				ID_DISPLAY_BUF[iot_str_l+9]=HTOC((SYS_TYPE_NO&0xf00)>>8);
				ID_DISPLAY_BUF[iot_str_l+10]=HTOC((SYS_TYPE_NO&0xf0)>>4);
				ID_DISPLAY_BUF[iot_str_l+11]=HTOC((SYS_TYPE_NO&0xf));
				ID_DISPLAY_BUF[iot_str_l+12]=0;
				
				
				DISPLAY_RENCODE_TO_TFT(ID_DISPLAY_BUF,WHITE);

 
 				POINT_COLOR=BLACK;
				BACK_COLOR=WHITE;	
 

			 LCD_ShowChar(176-40,ID_Show_Y,'I',16,0);
			 LCD_ShowChar(176-32,ID_Show_Y,'D',16,0);
			 LCD_ShowChar(176-24,ID_Show_Y,':',16,0); 
 
 
			 LCD_ShowChar(176,ID_Show_Y,SHOW_HTOC((SYS_TYPE&0xf000)>>12),16,0);
			 LCD_ShowChar(176+8,ID_Show_Y,SHOW_HTOC((SYS_TYPE&0xf00)>>8),16,0);			
	
			 LCD_ShowChar(176+24,ID_Show_Y,SHOW_HTOC((SYS_TYPE&0xf0)>>4),16,0);
			 LCD_ShowChar(176+32,ID_Show_Y,SHOW_HTOC((SYS_TYPE&0xf)>>0),16,0);									
				
			 LCD_ShowChar(176+48,ID_Show_Y,SHOW_HTOC((SYS_EDITION&0xf0)>>4),16,0);
			 LCD_ShowChar(176+56,ID_Show_Y,SHOW_HTOC((SYS_EDITION&0xf)>>0),16,0);							
				
			 LCD_ShowChar(176+72,ID_Show_Y,SHOW_HTOC((SYS_TYPE_NO&0xf00000)>>20),16,0);
			 LCD_ShowChar(176+80,ID_Show_Y,SHOW_HTOC((SYS_TYPE_NO&0xf0000)>>16),16,0);			
		
			 LCD_ShowChar(176+96,ID_Show_Y,SHOW_HTOC((SYS_TYPE_NO&0xf000)>>12),16,0);
			 LCD_ShowChar(176+104,ID_Show_Y,SHOW_HTOC((SYS_TYPE_NO&0xf00)>>8),16,0);		
					
			 LCD_ShowChar(176+120,ID_Show_Y,SHOW_HTOC((SYS_TYPE_NO&0xf0)>>4),16,0);
			 LCD_ShowChar(176+128,ID_Show_Y,SHOW_HTOC((SYS_TYPE_NO&0xf)>>0),16,0);			
												
				
				
			}
			








}

void User_MAC_Show(void)//MAC��ʾ
{












}















