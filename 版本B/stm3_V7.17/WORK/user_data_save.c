#include "includes.h"	   






void User_Sys_Data_Get(void)
{





	

	
	

	  AT24CXX_Read((uint8_t*)&CMachine.uLanguage_Mode, SYS_LANGUAGE_ADDR, 3); 	


	if(CMachine.uLanguage_Mode>1)
	{
	
		CMachine.uLanguage_Mode=0x00;
	  AT24CXX_Write((uint8_t*)&CMachine.uLanguage_Mode, SYS_LANGUAGE_ADDR, 3); 		
	
	}
	
		CMachine.uLanguage_Mode_B=CMachine.uLanguage_Mode;//	
	



	
	 AT24CXX_Read((uint8_t*)&CMachine.uLOGO_Mode, SYS_LOGO_ADDR, 3); 	
	if(CMachine.uLOGO_Mode>1)
	{
	
		CMachine.uLOGO_Mode=0x00;
	  AT24CXX_Write((uint8_t*)&CMachine.uLOGO_Mode, SYS_LOGO_ADDR, 3); 		
	
	}
	
		CMachine.uLOGO_Mode_B=CMachine.uLOGO_Mode;//	
			
	
	
	
	


	
	
	
	  AT24CXX_Read((uint8_t*)&CMachine.uLED_Back_No, SYS_BACK_LIGHT_ADDR, 3); 	


	if(CMachine.uLED_Back_No>1)
	{
	
		CMachine.uLED_Back_No=0x00;
	  AT24CXX_Write((uint8_t*)&CMachine.uLED_Back_No, SYS_BACK_LIGHT_ADDR, 3); 		
	
	}
	
	//温度修正值
	 // AT24CXX_Read((uint8_t*)&CMachine.temp_Correct_data, SYS_TEMP_CORRECT_ADDR, 3); 	

	
		//CMachine.uLED_Back_No=0x00;
	 // AT24CXX_Write((uint8_t*)&CMachine.temp_Correct_data, SYS_TEMP_CORRECT_ADDR, 3); 		
	
	
	
	//湿度修正值	
	//  AT24CXX_Read((uint8_t*)&CMachine.humidity_Correct_data, SYS_HUMIDITY_CORRECT_ADDR, 3); 	

	
		//CMachine.uLED_Back_No=0x00;
	 // AT24CXX_Write((uint8_t*)&CMachine.humidity_Correct_data, SYS_HUMIDITY_CORRECT_ADDR, 3); 		
	
	
	
	


}














































