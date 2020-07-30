#include "includes.h"	  






char HCHO_Show_Thread(thread_t* pt)//甲醛界面显示
{
	
	
     Thread_BEGIN
    {	

	
	
	while(CPU_ID[2]==UID_KEY[2])
	{

	
		if(CMachine.UI_Show_No==6)	
			{
		
		
				
				if(CMachine_B.UI_Show_No!=CMachine.UI_Show_No)
				{
				
 	 LCD_Clear(BLACK);
	
POINT_COLOR=WHITE;
	BACK_COLOR=BLACK;			 
			 

			 
			 
			 
  icon_show(393, 194, 37, 51, gImage_humy_03,1);//显示
  icon_show(395, 64, 27, 57, gImage_temp_03,1);//显示
 
 icon_show(440, 160, 18, 16, gImage_tempc_03,1);//显示

 icon_show(440, 291, 17, 16, gImage_humyc_03,1);//显示				 
		 			 




  LCD_DrawLine(356,54, 356, 304); 

		

LCD_Fill(110,208,110+12,208+12,WHITE);//甲醛小数点





 icon_show(235, 263, 107, 36, gImage_mgm3b_03,1);//显示		
 

	
				CMachine.uLanguage_Mode_B=0xff;

CMachine_B.temp=0xff;
CMachine_B.humidity=0xff;
CMachine_B.hcho=0xffff;
CMachine_B.tvoc=0xffff;
CMachine_B.co2=0xffff;
CMachine_B.pm25=0xffff;

/*
CMachine.pm25=100;
CMachine.hcho=123;
CMachine.tvoc=889;
CMachine.co2=900;
CMachine.temp=32;
CMachine.humidity=56;	
*/		
				
				}
				
				
		
		if(CMachine.Key2_vale==1)	//IN键按下
		{
			CMachine.Key2_vale=0;//清零
			CMachine.UI_Show_No=7;//进入下一界面
						continue;//下一页，退出
		}		
 User_Public_Show();//公共显示部分
		
	if(CMachine.uID_Show_Time_No>0)
		{
		
			
			
			 User_ID_Show();//ID显示
				

			
			
		}else
		{
			
			
			
			if(CMachine.uID_Show_Time_No_B!=0)
			{
				CMachine.uID_Show_Time_No_B=0;			
				
				CMachine_B.UI_Show_No=0xff;
				
				continue;//退出界面重新刷新
				
			}					
		

		
					if(CMachine.uLanguage_Mode_B!=CMachine.uLanguage_Mode)
			{
				CMachine.uLanguage_Mode_B=CMachine.uLanguage_Mode;
			
			if(CMachine.uLanguage_Mode==0)	 		 	
			{
			    if(CMachine.fHcho_b03_Clear!=0)
					{
					  LCD_Fill(151,78,151+84,78+25,BLACK);	//位数有变，清底色	  
					}	
					CMachine.fHcho_b03_Clear=0;
					icon_show(160, 78, 59, 31, gImage_HCHOb_03,1);//显示	
			}
			else	 		 
			{
			    if(CMachine.fHcho_b03_Clear!=1)
					{
					  LCD_Fill(160,78,160+59,78+31,BLACK);	//位数有变，清底色	  
					}	
					CMachine.fHcho_b03_Clear=1;
					
					icon_show(151, 78, 84, 25, gImage_HCHOb_03EG,1);//显示	
			}
				
CMachine_B.temp=0xff;
CMachine_B.humidity=0xff;
CMachine_B.hcho=0xffff;
CMachine_B.tvoc=0xffff;
CMachine_B.co2=0xffff;
CMachine_B.pm25=0xffff;				
				
			
			}
		////////////////////////////////////////////////////////////////////////// 甲醛      ///////////////////////////////////////////////						
		if(CMachine_B.hcho!=CMachine.hcho)
		{
			CMachine_B.hcho=CMachine.hcho;
			POINT_COLOR=WHITE;
			BACK_COLOR=BLACK;			
			Show_NUM100_72(37,128,CMachine.hcho/1000,1);			
			Show_NUM100_72(129,128,CMachine.hcho%1000/100,1);					
			Show_NUM100_72(129+72,128,CMachine.hcho%100/10,1);							
			Show_NUM100_72(129+144,128,CMachine.hcho%10,1);									
		
			if(CMachine.hcho<=50)	
			{
				if(CMachine.uLanguage_Mode==0) icon_show(68, 265, 154, 30, gImage_excellentb_03,1);//		
				else icon_show(68, 265, 154, 30, gImage_excellentbEG_03,1);//			
			}
			else if(CMachine.hcho<=100)	
			{
				if(CMachine.uLanguage_Mode==0) icon_show(68, 265, 154, 30, gImage_goodb_03,1);//		
				else icon_show(68, 265, 154, 30, gImage_goodbEG_03,1);//						
			}				
			else if(CMachine.hcho<=200)	
			{
				if(CMachine.uLanguage_Mode==0) icon_show(68, 265, 154, 30, gImage_moderateb_03,1);//		
				else icon_show(68, 265, 154, 30, gImage_moderatebEG_03,1);//					
			}
			else
			{
				if(CMachine.uLanguage_Mode==0) icon_show(68, 265, 154, 30, gImage_poorb_03,1);//		
				else icon_show(68, 265, 154, 30, gImage_poorbEG_03,1);//									
			}				
		}


			
//////////////////////////////////////////////////////////////////////////温度       ///////////////////////////////////////////////		
		if(CMachine_B.temp!=CMachine.temp)
		{
			CMachine_B.temp=CMachine.temp;
			POINT_COLOR=WHITE;
			BACK_COLOR=BLACK;			
			
			if(CMachine.temp<=30)
			{
				Show_NUM44_32(374,126,(40-CMachine.temp)%100/10,1);					 
				Show_NUM44_32(374+32,126,(40-CMachine.temp)%10,1);						
				LCD_Fill(362,146,372,150,WHITE);	
				
			}else
			if(CMachine.temp<40)
			{
				LCD_Fill(374,126,406,170,BLACK);	
				LCD_Fill(362,146,372,150,BLACK);			
				
				Show_NUM44_32(374+32,126,(40-CMachine.temp)%10,1);		
				LCD_Fill(382,146,392,150,WHITE);			
			}else
			if(CMachine.temp<50)
			{
				LCD_Fill(374,126,406,170,BLACK);	
				LCD_Fill(362,146,372,150,BLACK);		
			 
				Show_NUM44_32(374+32,126,(CMachine.temp-40)%10,1);		
			}else
			{
				LCD_Fill(362,146,372,150,BLACK);		
				
				Show_NUM44_32(374,126,(CMachine.temp-40)%100/10,1);					 
				Show_NUM44_32(374+32,126,(CMachine.temp-40)%10,1);				
			}
		}	
//////////////////////////////////////////////////////////////////////////湿度        ///////////////////////////////////////////////		
		if(CMachine_B.humidity!=CMachine.humidity)
		{
			CMachine_B.humidity=CMachine.humidity;	
			POINT_COLOR=WHITE;
			BACK_COLOR=BLACK;						
			Show_NUM44_32(374,255,CMachine.humidity%100/10,1);		
			Show_NUM44_32(374+32,255,CMachine.humidity%10,1);					
		}			
				
		
	}
		}	
		
		Thread_Sleep(20);
 

}
	
    }
    Thread_END	
	






}

