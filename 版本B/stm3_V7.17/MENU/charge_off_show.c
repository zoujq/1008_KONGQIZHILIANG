#include "includes.h"	  



u8 uCharge_Off_Blink_No=0;//�����˸����

char Charge_Off_Show_Thread(thread_t* pt)//�ػ���������ʾ
{
	
	
     Thread_BEGIN
    {

	
	
	while(f_Sys_Check==2)
	{

	
		if(CMachine.UI_Show_No==1)	
			{
		
		
				
				if(CMachine_B.UI_Show_No!=CMachine.UI_Show_No)
				{
				
	 	 LCD_Clear(BLACK);

	
				uCharge_Off_Blink_No=1;	//��ֹ���ػ���������ˣ�����ʾ				
				
				}
				
				
		
			CMachine_B.UI_Show_No=CMachine.UI_Show_No;//��ֹ����ػ��ٿ��������ͼ�겻��ʾ�����ʱ


		
		
		
	if(CMachine.fCharge==1)
	{
		
				//LCD_BACK_ON	//�򿪱���
					CMachine.fLED_Back=1;	
		

		if(CMachine.Battery==100)	
		{	
			//if(uCharge_Off_Blink_No!=0)	icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ					
			
			icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ
						LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
						LCD_Fill(394+9+11,16+8,394+15+11,16+19,WHITE);
						LCD_Fill(394+9+11+11,16+8,394+15+11+11,16+19,WHITE);	
			
			uCharge_Off_Blink_No=0;			
		}else
		{
			if(uCharge_Off_Blink_No==2)	 
			{
				icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ		
			}
			if(uCharge_Off_Blink_No==20)	
			{
			//	icon_show(394, 16, 49, 26, gImage_battery_01,1);//��ʾ		

			  icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ

				LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
				
			}
			if(uCharge_Off_Blink_No==40)		
			{
				//icon_show(394, 16, 49, 26, gImage_battery_02,1);//��ʾ				

				icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ
//						LCD_Fill(394+8+5+5,16+7,394+8+8-1+5+5,16+13+7-1,WHITE);	//��ʾ
				LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
				LCD_Fill(394+9+11,16+8,394+15+11,16+19,WHITE);		
			}
			if(uCharge_Off_Blink_No==60)	
			{
				//icon_show(394, 16, 49, 26, gImage_battery_03,1);//��ʾ	

        icon_show(394, 16, 49, 26, gImage_battery_00,1);//��ʾ
				LCD_Fill(394+9,16+8,394+15,16+19,WHITE);
				LCD_Fill(394+9+11,16+8,394+15+11,16+19,WHITE);
				LCD_Fill(394+9+11+11,16+8,394+15+11+11,16+19,WHITE);

				
			}			
			uCharge_Off_Blink_No++;
			if(uCharge_Off_Blink_No>80)	uCharge_Off_Blink_No=2;
	}
	}		
		
		
		
		
		
}
		
		

		Thread_Sleep(20);
	} 

    }
    Thread_END	
	
	
	






}
