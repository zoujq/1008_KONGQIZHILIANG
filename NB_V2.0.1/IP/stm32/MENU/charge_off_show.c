#include "includes.h"	  



u8 uCharge_Off_Blink_No=0;//�����˸����

void Charge_Off_Show(void)//�ػ���������ʾ
{

 	 LCD_Clear(BLACK);

	
				uCharge_Off_Blink_No=1;	//��ֹ���ػ���������ˣ�����ʾ	
	
	
	while(f_Sys_Check==2)
	{

	
		if(CMachine.UI_Show_No!=1)	break;
		
		
		
			CMachine_B.UI_Show_No=CMachine.UI_Show_No;//��ֹ����ػ��ٿ��������ͼ�겻��ʾ�����ʱ
		
		
	if(CMachine.fCharge==1)
	{
		
				//LCD_BACK_ON	//�򿪱���
					CMachine.fLED_Back=1;	
		

		if(CMachine.Battery==100)	
		{	
			if(uCharge_Off_Blink_No!=0)	icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ					
			uCharge_Off_Blink_No=0;			
		}else
		{
			if(uCharge_Off_Blink_No==2)	 
			{
				icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ		
			}
			if(uCharge_Off_Blink_No==20)	
			{
				icon_show(394, 16, 49, 26, gImage_battery_01,1);//��ʾ				  		 		
			}
			if(uCharge_Off_Blink_No==40)		
			{
				icon_show(394, 16, 49, 26, gImage_battery_02,1);//��ʾ				  	 			 		
			}
			if(uCharge_Off_Blink_No==60)	
			{
				icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ				  			
			}			
			uCharge_Off_Blink_No++;
			if(uCharge_Off_Blink_No>80)	uCharge_Off_Blink_No=2;
	}
	}		
		
		
		
		
		
		
		
		

		delay_ms(20);
	} 

	
	
	
	






}
