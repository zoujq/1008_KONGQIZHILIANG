#include "includes.h"	   
#include "stdlib.h"	




u8 uTime_World_Buf[6]={12,5,25,13,45,6}; //年月日时分周,系统时间计算缓存
u8 uRang_Time[2]={10,02};//闹铃时间
u8 uRang_Time_Buf[2]={0,0};//闹铃时间,缓存

u8 f_Rang_Run=1;//闹钟使能标志位

u16 uRang_Times=0;//闹铃总时间

u16 uRang_Times_30S=0;//闹铃,30秒闹铃，每3分钟一次，连续3次


//u8 uClock_Hour_Set;//时钟设置小时缓存
//u8 uClock_Min_Set;//时钟设置分缓存
u8 uClock_Set_Point=0;//时钟设置位指示
u8 uRang_Set_Point=0;//闹钟设置位指示
u8 uMusic_Set_Point=0;//按键位指示


u8 f_Music_Set_Start=0;//暂停或播放按键


	 u16	uMode_Old=0xffff;//之前的菜单，判断表盘刷新用


	static u8 uSec_Blink_Suspended=0;//秒闪计数,暂停
	static u8 uSec_Blink_Suspended_Rang=0;//秒闪计数,暂停


u8 f_User_Sleep=0;//用户睡眠标志、0不睡眠1睡眠






u8 uData_Buf_Music[10]={0};//



u8	f_Search_Phone_Show=0;//动态图标显示
u8	uSearch_Phone_Run=0;//动态图标显示,图标号






void User_Clock_Init(void)//RTC初始化
{



	u8 res;
			res=RTC_Init();	  			//RTC初始化
		//while(RTC_Init())	

		//	rtccnt=RTC->CNTH;//得到闹钟值
//	rtccnt<<=16;
	//rtccnt|=RTC->CNTL;
	//ALARM_CNT_Set(rtccnt+1);//设置8秒钟后，闹钟唤醒
//	RTC_SetAlarm(RTC_GetCounter());
//		SPI_Flash_Read(Data_Save_Buf,SYSTIME_ADDRESS_SAVE,7);		//掉电，保存时间
	AT24CXX_Read(SYSTIME_ADDRESS_SAVE,Data_Save_Buf,7);
	
	
	

calendar.w_year=	Data_Save_Buf[0]*256+Data_Save_Buf[1];
calendar.w_month=Data_Save_Buf[2];
calendar.w_date=Data_Save_Buf[3];
calendar.week=Data_Save_Buf[4];
calendar.hour=Data_Save_Buf[5];		
calendar.min=Data_Save_Buf[6];		

	if(!(calendar.w_year>2014&&calendar.w_year<2099))
	{
	
		res=RTC_Set(2015,1,1,0,0,0);
		
		calendar.w_year=	2015;
calendar.w_month=1;
calendar.w_date=1;
calendar.week=4;
calendar.hour=0;		
calendar.min=0;	
		
		
	
	}	else
	res=RTC_Set((Data_Save_Buf[0]*256+Data_Save_Buf[1]),Data_Save_Buf[2],Data_Save_Buf[3],Data_Save_Buf[5],Data_Save_Buf[6],0);	
	
	
		

	
	
	
//res=RTC_Set(2014,11,18,11,11,11);



}


void Sys_Time_Save(void)//系统时间保存
{
	static u16 uYear_Before=255;//年，年有变化才刷新
	static u8 uMonth_Before=255;//月，月有变化才刷新	
	static u8 uDay_Before=255;//日，日有变化才刷新
	static u8 uHour_Before=255;//时，时有变化才刷新
	static u8 uMin_Before=255;//分，分有变化才刷新

	
		//	u8 Data_Save_Buf0[10]={0};//保存缓存
			u8 Data_Save_Buf1[10]={0};//保存缓存
	
	
	if(uYear_Before!=calendar.w_year||uMonth_Before!=calendar.w_month||uDay_Before!=calendar.w_date||uHour_Before!=calendar.hour||uMin_Before!=calendar.min)
	{
	uYear_Before=calendar.w_year;
		uMonth_Before=calendar.w_month;
		uDay_Before=calendar.w_date;
		uHour_Before=calendar.hour;
		uMin_Before=calendar.min;
		
Data_Save_Buf1[0]=calendar.w_year/256;
Data_Save_Buf1[1]=calendar.w_year%256;
Data_Save_Buf1[2]=calendar.w_month;
Data_Save_Buf1[3]=calendar.w_date;
Data_Save_Buf1[4]=calendar.week;
Data_Save_Buf1[5]=calendar.hour;		
Data_Save_Buf1[6]=calendar.min;		
		
///SPI_Flash_Init();//调用这个，防止数据发生变化。睡眠时，时钟可能发生变化，没这个，数据会出错
		///SPI_Flash_Write(Data_Save_Buf,SYSTIME_ADDRESS_SAVE,7);		//掉电，保存时间

	 AT24CXX_Write(SYSTIME_ADDRESS_SAVE,Data_Save_Buf1,7);
	//	AT24CXX_Read(SYSTIME_ADDRESS_SAVE,Data_Save_Buf0,7);
	}
}


void Clock_Show_Home(void)//首页显示数字时间
{
	static u8 uSec_Before=255;//秒，秒有变化才刷新
	static u8 uSec_Blink_No=0;//秒闪计数
	

	


	if(uSec_Before!=calendar.sec)
	{
		uSec_Before=calendar.sec;
		OLED_ShowChar(22+5,15,(calendar.hour/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(37+5,15,(calendar.hour%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(52+5,15,':',0xffff,0xffff);//显示ASCII字符	  
		OLED_ShowChar(67+5,15,(calendar.min/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(82+5,15,(calendar.min%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		uSec_Blink_No=6;
	}	
	if(uSec_Blink_No>0)
	{
		uSec_Blink_No--;
		if(uSec_Blink_No==0) 	
		OLED_ShowChar(52+5,15,':',0x0000,0x0000);//显示ASCII字符	  
	}

	
	
	
	
	
	
	
}







void User_Music_Set_Into(void)//进入音乐播放
{

	

	
uMenu_No=40;//音乐播放设置
	

 OLCD_Clear(0x0000);//清蓝色
	
	
	
	


	
		 gui_fill_keyLeft(9,33,31,0xffff,0x0000);//左
	
		 gui_fill_keyStart(49,33,31,0xffff,0x0000);//播放
	
		 gui_fill_keyRight(89,33,31,0xffff,0x0000);//右
	
	switch(uMusic_Set_Point)
	{
		case 0:
					 gui_fill_keyLeft(9,33,31,0x0000,0xffff);//左
				break;
		case 1:
			if(f_Music_Set_Start==0)
				 gui_fill_keyStart(49,33,31,0x0000,0xffff);//播放
			else gui_fill_keyPause(49,33,31,0x0000,0xffff);//暂停
				break;		
		case 2:
					 gui_fill_keyRight(89,33,31,0x0000,0xffff);//右
				break;	
		default:
		break;
	}
	
		uMenu_Not_Change_Time=0;//定时睡眠，清零
}





	
	
	
	


void User_Music_Set_Exit(void)//退出音乐播放
{


	uMenu_No=4;//音乐播放
	
	uMenu_Not_Change_Time=0;//定时睡眠，清零

}


	
void User_Music_Set_Bit_Next(void)//音乐播放,下一个按键
{

		uMusic_Set_Point++;
	if(uMusic_Set_Point>2)	uMusic_Set_Point=0;//
		
		 gui_fill_keyLeft(9,33,31,0xffff,0x0000);//左
	
		if(f_Music_Set_Start==0) gui_fill_keyStart(49,33,31,0xffff,0x0000);//播放
		else gui_fill_keyPause(49,33,31,0xffff,0x0000);//暂停
	
		 gui_fill_keyRight(89,33,31,0xffff,0x0000);//右
	
	switch(uMusic_Set_Point)
	{
		case 0:
					 gui_fill_keyLeft(9,33,31,0x0000,0xffff);//左
				break;
		case 1:
			if(f_Music_Set_Start==0)
				 gui_fill_keyStart(49,33,31,0x0000,0xffff);//播放
			else gui_fill_keyPause(49,33,31,0x0000,0xffff);//暂停
				break;		
		case 2:
					 gui_fill_keyRight(89,33,31,0x0000,0xffff);//右
				break;	
		default:
		break;
	}
	
	
	uMenu_Not_Change_Time=0;//定时睡眠，清零
	
	
}



	
void User_Music_Set_Bit_Before(void)//音乐播放,上一个按键
{
	uMusic_Set_Point--;
	if(uMusic_Set_Point>200)	uMusic_Set_Point=2;//
		
		 gui_fill_keyLeft(9,33,31,0xffff,0x0000);//左
	
		if(f_Music_Set_Start==0) gui_fill_keyStart(49,33,31,0xffff,0x0000);//播放
		else gui_fill_keyPause(49,33,31,0xffff,0x0000);//暂停
	
		 gui_fill_keyRight(89,33,31,0xffff,0x0000);//右
	
	switch(uMusic_Set_Point)
	{
		case 0:
					 gui_fill_keyLeft(9,33,31,0x0000,0xffff);//左
				break;
		case 1:
			if(f_Music_Set_Start==0)
				 gui_fill_keyStart(49,33,31,0x0000,0xffff);//播放
			else gui_fill_keyPause(49,33,31,0x0000,0xffff);//暂停
				break;		
		case 2:
					 gui_fill_keyRight(89,33,31,0x0000,0xffff);//右
				break;	
		default:
		break;
	}
	
	
		uMenu_Not_Change_Time=0;//定时睡眠，清零
}


void User_Music_Set_Bit_In(void)//音乐播放，按键选中
{

//选中图标操作


	

	
		
		 gui_fill_keyLeft(9,33,31,0xffff,0x0000);//左
	
			if(f_Music_Set_Start==0) gui_fill_keyStart(49,33,31,0xffff,0x0000);//播放
		else gui_fill_keyPause(49,33,31,0xffff,0x0000);//暂停
	
		 gui_fill_keyRight(89,33,31,0xffff,0x0000);//右
	
	switch(uMusic_Set_Point)
	{
		case 0:
					 gui_fill_keyLeft(9,33,31,0x0000,0xffff);//左
		
								uData_Buf_Music[0]=	0xF6;
	uData_Buf_Music[1]=	0x01;
	uData_Buf_Music[2]=	0x02;
	uData_Buf_Music[3]=	0x07;		
	uData_Buf_Music[4]=  0x1D;
	uData_Buf_Music[5]=	0x00;
	uData_Buf_Music[6]=	0x00;
	uData_Buf_Music[7]=	0x45;		
		
				break;
		case 1:
				if(f_Music_Set_Start==1) f_Music_Set_Start=0;//
	else f_Music_Set_Start=1;
		
			if(f_Music_Set_Start==0)
			{ 
				gui_fill_keyStart(49,33,31,0x0000,0xffff);//播放

						uData_Buf_Music[0]=	0xF6;
	uData_Buf_Music[1]=	0x01;
	uData_Buf_Music[2]=	0x01;
	uData_Buf_Music[3]=	0x07;		
	uData_Buf_Music[4]=  0x1D;
	uData_Buf_Music[5]=	0x00;
	uData_Buf_Music[6]=	0x00;
	uData_Buf_Music[7]=	0x45;		
			}
			
			else 
			{
				gui_fill_keyPause(49,33,31,0x0000,0xffff);//暂停
									uData_Buf_Music[0]=	0xF6;
	uData_Buf_Music[1]=	0x01;
	uData_Buf_Music[2]=	0x01;
	uData_Buf_Music[3]=	0x07;		
	uData_Buf_Music[4]=  0x1D;
	uData_Buf_Music[5]=	0x00;
	uData_Buf_Music[6]=	0x00;
	uData_Buf_Music[7]=	0x45;			

			}
				break;		
		case 2:
					 gui_fill_keyRight(89,33,31,0x0000,0xffff);//右
								uData_Buf_Music[0]=	0xF6;
	uData_Buf_Music[1]=	0x01;
	uData_Buf_Music[2]=	0x03;
	uData_Buf_Music[3]=	0x07;		
	uData_Buf_Music[4]=  0x1D;
	uData_Buf_Music[5]=	0x00;
	uData_Buf_Music[6]=	0x00;
	uData_Buf_Music[7]=	0x45;		
				break;	
		default:
		break;
	}
	
	


	
			BLE_WAKEUP;//唤醒蓝牙
delay_ms(10);

			User_UART1_SendStringx(uData_Buf_Music,8);
		BLE_SLEEP;//睡眠蓝牙
	
	uMenu_Not_Change_Time=0;//定时睡眠，清零
	
	
	
	
}






void User_Step_Into(void)//进入计步界面
{
	uMenu_No=50;//计步
	OLCD_Clear(0x0000);//清蓝色
	OLED_ShowChar(19,32,'S',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar(34,32,'t',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar(49,32,'e',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar(64,32,'p',0xffff,0x0000);//显示ASCII字符	
	OLED_ShowChar(79,32,'s',0xffff,0x0000);//显示ASCII字符		
	OLED_ShowChar2020(34,64,uStep_Count_No/10000+0x30,0xffff,0x0000);
	OLED_ShowChar2020(44,64,uStep_Count_No%10000/1000+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(54,64,uStep_Count_No%1000/100+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(64,64,uStep_Count_No%100/10+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(74,64,uStep_Count_No%10+0x30,0xffff,0x0000);		
	uMenu_Not_Change_Time=0;//定时睡眠，清零
}

void User_Step_Show(void)//显示步数界面
{
	static u16 uStep_Count_No_B=0;
	if(uStep_Count_No_B!=uStep_Count_No)
	{
		uStep_Count_No_B=uStep_Count_No;
		OLED_ShowChar2020(34,64,uStep_Count_No/10000+0x30,0xffff,0x0000);
		OLED_ShowChar2020(44,64,uStep_Count_No%10000/1000+0x30,0xffff,0x0000);	
		OLED_ShowChar2020(54,64,uStep_Count_No%1000/100+0x30,0xffff,0x0000);	
		OLED_ShowChar2020(64,64,uStep_Count_No%100/10+0x30,0xffff,0x0000);	
		OLED_ShowChar2020(74,64,uStep_Count_No%10+0x30,0xffff,0x0000);		
	}
}

void User_Step_Exit(void)//退出计步界面
{
	uMenu_No=5;//计步
	uMenu_Not_Change_Time=0;//定时睡眠，清零
}

void User_Step_Clear(void)//计步清零
{
	uStep_Count_No=0;//清零
	uMenu_Not_Change_Time=0;//定时睡眠，清零
}











void User_Distance_Into(void)//进入米距界面
{
	/*u32 uDistance_B=0;//缓存
	
	
	
	uMenu_No=60;//计步
	OLCD_Clear(0x0000);//清蓝色
	uDistance_B=(uStep_Count_No*u_User_Stride)/100;
	
	
	OLED_ShowChar2020(24,32,'D',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar2020(34,32,'i',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar2020(44,32,'s',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar2020(54,32,'t',0xffff,0x0000);//显示ASCII字符	
	OLED_ShowChar2020(64,32,'a',0xffff,0x0000);//显示ASCII字符		
	OLED_ShowChar2020(74,32,'n',0xffff,0x0000);//显示ASCII字符		
	OLED_ShowChar2020(84,32,'c',0xffff,0x0000);//显示ASCII字符		
	OLED_ShowChar2020(94,32,'e',0xffff,0x0000);//显示ASCII字符		
	
	
	OLED_ShowChar2020(34,64,uDistance_B/10000+0x30,0xffff,0x0000);
	OLED_ShowChar2020(44,64,uDistance_B%10000/1000+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(54,64,uDistance_B%1000/100+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(64,64,uDistance_B%100/10+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(74,64,uDistance_B%10+0x30,0xffff,0x0000);		
	uMenu_Not_Change_Time=0;//定时睡眠，清零
	*/
	
		float uCalories_B=0;//缓存
	u32 uCalories_C=0;//缓存
	u32 uDistance_B=0;//缓存
	
	
	uMenu_No=60;//米距和卡路里
	OLCD_Clear(0x0000);//清蓝色
	uDistance_B=(uStep_Count_No*u_User_Stride)/100;
	uCalories_B=(uStep_Count_No*u_User_Stride)/100/1000.0;
	uCalories_B=uCalories_B*u_User_Weight*1.036;
	uCalories_C=uCalories_B;
	
	
		OLED_ShowChar2020(24,10,'D',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar2020(34,10,'i',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar2020(44,10,'s',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar2020(54,10,'t',0xffff,0x0000);//显示ASCII字符	
	OLED_ShowChar2020(64,10,'a',0xffff,0x0000);//显示ASCII字符		
	OLED_ShowChar2020(74,10,'n',0xffff,0x0000);//显示ASCII字符		
	OLED_ShowChar2020(84,10,'c',0xffff,0x0000);//显示ASCII字符		
	OLED_ShowChar2020(94,10,'e',0xffff,0x0000);//显示ASCII字符		
	
	
	OLED_ShowChar2020(34,30,uDistance_B/10000+0x30,0xffff,0x0000);
	OLED_ShowChar2020(44,30,uDistance_B%10000/1000+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(54,30,uDistance_B%1000/100+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(64,30,uDistance_B%100/10+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(74,30,uDistance_B%10+0x30,0xffff,0x0000);		
	
	
	OLED_ShowChar2020(24,50,'C',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar2020(34,50,'a',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar2020(44,50,'l',0xffff,0x0000);//显示ASCII字符	 	
	OLED_ShowChar2020(54,50,'o',0xffff,0x0000);//显示ASCII字符	
	OLED_ShowChar2020(64,50,'r',0xffff,0x0000);//显示ASCII字符		
	OLED_ShowChar2020(74,50,'i',0xffff,0x0000);//显示ASCII字符		
	OLED_ShowChar2020(84,50,'e',0xffff,0x0000);//显示ASCII字符		
	OLED_ShowChar2020(94,50,'s',0xffff,0x0000);//显示ASCII字符		
	
	
	OLED_ShowChar2020(34,70,uCalories_C/10000+0x30,0xffff,0x0000);
	OLED_ShowChar2020(44,70,uCalories_C%10000/1000+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(54,70,uCalories_C%1000/100+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(64,70,uCalories_C%100/10+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(74,70,uCalories_C%10+0x30,0xffff,0x0000);		
	uMenu_Not_Change_Time=0;//定时睡眠，清零
	
	
	
	
	
}

void User_Distance_Show(void)//显示米距界面
{
	u32 uDistance_B=0;//缓存
	static u16 uDistance_Count_No_B=0;
		
		static u16 uCalories_Count_No_B=0;
	
		float uCalories_B=0;//缓存
	u32 uCalories_C=0;//缓存
	
	
	
	if(uDistance_Count_No_B!=uStep_Count_No)
	{
		uDistance_Count_No_B=uStep_Count_No;
		
			uDistance_B=(uStep_Count_No*u_User_Stride)/100;
		
	OLED_ShowChar2020(34,30,uDistance_B/10000+0x30,0xffff,0x0000);
	OLED_ShowChar2020(44,30,uDistance_B%10000/1000+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(54,30,uDistance_B%1000/100+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(64,30,uDistance_B%100/10+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(74,30,uDistance_B%10+0x30,0xffff,0x0000);		
	
	}
	

	
	
	if(uCalories_Count_No_B!=uStep_Count_No)
	{
		uCalories_Count_No_B=uStep_Count_No;
		
	uCalories_B=(uStep_Count_No*u_User_Stride)/100/1000.0;
	uCalories_B=uCalories_B*u_User_Weight*1.036;
	uCalories_C=uCalories_B;
		
	OLED_ShowChar2020(34,64,uCalories_C/10000+0x30,0xffff,0x0000);
	OLED_ShowChar2020(44,64,uCalories_C%10000/1000+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(54,64,uCalories_C%1000/100+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(64,64,uCalories_C%100/10+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(74,64,uCalories_C%10+0x30,0xffff,0x0000);		
	}
	
	
	
	
	
	
	
}

void User_Distance_Exit(void)//退出米距界面
{
	uMenu_No=6;//
	uMenu_Not_Change_Time=0;//定时睡眠，清零
}




void User_Search_Phone_Into(void)//进入查找手机界面
{
	u8 i;

		uMenu_No=70;//
		OLCD_Clear(0x0000);//清蓝色
		OLED_DrawICO_Small(36, 40,gImage_4);//查找手机
			f_Search_Phone_Show=0;//清零
		uSearch_Phone_Run=0;//清零
	
	
		for(i=0;i<7;i++)
	{
		gui_draw_bline1(64+i,68-i,64+i,68,0,0xffff); 
	}
	

					uMenu_Not_Change_Time=0;//定时睡眠，清零
	

}

u8 uData_Buf_Search_Phone[10]={0};
void User_Search_Phone_Able(void)//选择查找手机或关闭手机界面
{
	u8 i;
	
	
	if(f_Search_Phone_Show==0) 
	{
		f_Search_Phone_Show=1;
		
		uData_Buf_Search_Phone[0]=	0xF3;
		uData_Buf_Search_Phone[1]=	0x01;
		uData_Buf_Search_Phone[2]=	0x01;
		uData_Buf_Search_Phone[3]=	0x07;		
		uData_Buf_Search_Phone[4]=  0x1D;
		uData_Buf_Search_Phone[5]=	0x00;
		uData_Buf_Search_Phone[6]=	0x00;
		uData_Buf_Search_Phone[7]=	0x45;		
	
		BLE_WAKEUP;//唤醒蓝牙
		delay_ms(10);
		User_UART1_SendStringx(uData_Buf_Search_Phone,8);
		BLE_SLEEP;//睡眠蓝牙
	}
	else 
	{
		f_Search_Phone_Show=0;
		uSearch_Phone_Run=0;
		
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+i,68-10-i,64+10+i,68,0,0x0000); 
			}	
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+i,68-10-8-i,64+10+8+i,68,0,0x0000); 
			}
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+8+i,68-10-8-8-i,64+10+8+8+i,68,0,0x0000); 
			}
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+8+8+i,68-10-8-8-8-i,64+10+8+8+8+i,68,0,0x0000); 
			}	
		
		uData_Buf_Search_Phone[0]=	0xF3;
		uData_Buf_Search_Phone[1]=	0x01;
		uData_Buf_Search_Phone[2]=	0xA1;
		uData_Buf_Search_Phone[3]=	0x07;		
		uData_Buf_Search_Phone[4]=  0x1D;
		uData_Buf_Search_Phone[5]=	0x00;
		uData_Buf_Search_Phone[6]=	0x00;
		uData_Buf_Search_Phone[7]=	0x45;		
	
		BLE_WAKEUP;//唤醒蓝牙
		delay_ms(10);
		User_UART1_SendStringx(uData_Buf_Search_Phone,8);
		BLE_SLEEP;//睡眠蓝牙
	}
	
	uMenu_Not_Change_Time=0;//定时睡眠，清零
}

void User_Search_Phone_Show(void)//显示查找手机动态界面
{
u8 i,j;
	static u8 time_show=0;
	
	
	if(f_Search_Phone_Show==1)
	{
		
		time_show++;
		if(time_show>=6)		time_show=0;
		else return;
		
		uSearch_Phone_Run++;
		if(uSearch_Phone_Run>5) uSearch_Phone_Run=1;
		if(uSearch_Phone_Run==1)
		{
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+i,68-10-i,64+10+i,68,0,0xffff); 
			}
		}
		if(uSearch_Phone_Run==2)
		{
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+i,68-10-8-i,64+10+8+i,68,0,0xffff); 
			}
		}
		if(uSearch_Phone_Run==3)
		{			
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+8+i,68-10-8-8-i,64+10+8+8+i,68,0,0xffff); 
			}
		}
		if(uSearch_Phone_Run==4)
		{					
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+8+8+i,68-10-8-8-8-i,64+10+8+8+8+i,68,0,0xffff); 
			}
		}
		if(uSearch_Phone_Run==5)	
		{
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+i,68-10-i,64+10+i,68,0,0x0000); 
			}	
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+i,68-10-8-i,64+10+8+i,68,0,0x0000); 
			}
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+8+i,68-10-8-8-i,64+10+8+8+i,68,0,0x0000); 
			}
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+8+8+i,68-10-8-8-8-i,64+10+8+8+8+i,68,0,0x0000); 
			}
		}
	}else
	{
		if(uSearch_Phone_Run>0)
		{
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+i,68-10-i,64+10+i,68,0,0x0000); 
			}	
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+i,68-10-8-i,64+10+8+i,68,0,0x0000); 
			}
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+8+i,68-10-8-8-i,64+10+8+8+i,68,0,0x0000); 
			}
			for(i=0;i<5;i++)
			{
				gui_draw_bline1(64+10+8+8+8+i,68-10-8-8-8-i,64+10+8+8+8+i,68,0,0x0000); 
			}	
		}
	}
	

	
	

}



void User_Calories_Exit(void)//退出查找手机界面
{
	uSearch_Phone_Run=0;
	f_Search_Phone_Show=0;
	uMenu_No=7;//
	
	//f_Search_Phone_Show=0;
	//	uSearch_Phone_Run=0;
		uData_Buf_Search_Phone[0]=	0xF3;
		uData_Buf_Search_Phone[1]=	0x01;
		uData_Buf_Search_Phone[2]=	0xA1;
		uData_Buf_Search_Phone[3]=	0x07;		
		uData_Buf_Search_Phone[4]=  0x1D;
		uData_Buf_Search_Phone[5]=	0x00;
		uData_Buf_Search_Phone[6]=	0x00;
		uData_Buf_Search_Phone[7]=	0x45;		
	
		BLE_WAKEUP;//唤醒蓝牙
delay_ms(10);
		User_UART1_SendStringx(uData_Buf_Search_Phone,8);
		BLE_SLEEP;//睡眠蓝牙
	
	
	
	uMenu_Not_Change_Time=0;//定时睡眠，清零
}




void User_Sleep_Test_Fun(void)//睡眠检测显示
{
	
	u8 i;
	
for(i=0;i<30;i++)//清屏
	{
		gui_draw_bline1(35,39+i,90,39+i,0,0x0000); 
	}
	
	if(f_User_Sleep==1)
	{

		
		OLED_ShowChar(49,37,'O',0xffff,0x0000);//显示ASCII字符	 
	OLED_ShowChar(64,37,'N',0xffff,0x0000);//显示ASCII字符	 

		gui_draw_bline1(37,39,88,39,0,0xffff); 
	gui_draw_bline1(37,40,88,40,0,0xffff); 
	gui_draw_bline1(37,65,88,65,0,0xffff); 
	gui_draw_bline1(37,66,88,66,0,0xffff); 	
	gui_draw_bline1(39,36,39,66,0,0xffff); 
	gui_draw_bline1(40,36,40,66,0,0xffff); 
	gui_draw_bline1(89,36,89,66,0,0xffff); 
	gui_draw_bline1(90,36,90,66,0,0xffff); 
		
	}else
	{

		
	OLED_ShowChar(42,37,'O',0xffff,0x0000);//显示ASCII字符	 
	OLED_ShowChar(57,37,'F',0xffff,0x0000);//显示ASCII字符	 
	OLED_ShowChar(72,37,'F',0xffff,0x0000);//显示ASCII字符	
	
	gui_draw_bline1(37,39,88,39,0,0xffff); 
	gui_draw_bline1(37,40,88,40,0,0xffff); 
	gui_draw_bline1(37,65,88,65,0,0xffff); 
	gui_draw_bline1(37,66,88,66,0,0xffff); 	
	gui_draw_bline1(39,36,39,66,0,0xffff); 
	gui_draw_bline1(40,36,40,66,0,0xffff); 
	gui_draw_bline1(89,36,89,66,0,0xffff); 
	gui_draw_bline1(90,36,90,66,0,0xffff); 
	
	}





}

void User_Sleep_Test_Into(void)//进入睡眠检测界面
{
	u8 i;
	
	
	
	uMenu_No=90;//睡眠检测
	OLCD_Clear(0x0000);//清蓝色
		User_Sleep_Test_Fun();//睡眠检测显示

	
	uMenu_Not_Change_Time=0;//定时睡眠，清零
}

void User_Sleep_Test_Change(void)//睡眠检测界面切换
{
	
	

	if(f_User_Sleep==1)	
	{
		f_User_Sleep=0;//非用户睡眠
		uSleep_Count_No_10min=0;//数据清零，开始重新计数		
	}	else//用户睡眠
	{		
		f_User_Sleep=1;	
		uSleep_Count_No_10min=0;//数据清零，开始重新计数
	}
	User_Sleep_Test_Fun();//睡眠检测显示
	
//SPI_Flash_Init();//调用这个，防止数据发生变化。睡眠时，时钟可能发生变化，没这个，数据会出错	
///		SPI_Flash_Write(&f_User_Sleep,USER_SLEEP_ADDRESS_SAVE,1);		//保存用户数据，用户睡眠状态
	AT24CXX_Write(USER_SLEEP_ADDRESS_SAVE,&f_User_Sleep,1);
	
	
	
}

void User_Sleep_Test_Exit(void)//退出睡眠检测界面
{
	uMenu_No=9;//睡眠检测
	uMenu_Not_Change_Time=0;//定时睡眠，清零
}










void User_Rang_Pause(void)//闹铃暂停
{

//uRang_Times_30S=0;

	u_Rang_Remind_No=0;//复位

}

void User_Rang_Stop(void)//闹铃停止
{

//uRang_Times_30S=0;//复位
//uRang_Times=0;//复位
	u_Rang_Remind_No=0;//复位

}


/*
void User_Rang_Fun(void)//闹铃扫描
{

	static u8 uTime_500MS_No=0;//500ms计时
	
	
	
	uTime_500MS_No++;
	if(uTime_500MS_No<5) 
	{
		uTime_500MS_No++;
		return;
	}
	else 
	{
		uTime_500MS_No=0;
		
	}

	if(uRang_Times>0)
	{
		uRang_Times--;
	if(uRang_Times%360==0)
	{
	
	
	uRang_Times_30S=60;//30秒
		
	
	}
	
	}
	
		if(uRang_Times_30S>0)
	{
	
	
		if(uRang_Times_30S%2==0)   Buzzer_Run(1);//声音响,一声
		
		
	uRang_Times_30S--;
		
		
	
	
	}
	

if((uRang_Time[0]==calendar.hour)&&(uRang_Time[1]==calendar.min)&&calendar.sec==0&&f_Rang_Run==1)
{


uRang_Times=721;//闹钟响3次







}












}
	
	*/
	
	
	
	
	
	



void User_Rang_Set_Into(void)//进入闹铃设置
{

uMenu_No=80;//闹铃设置
	
	

	
		uRang_Time_Buf[0]=uRang_Time[0];
		uRang_Time_Buf[1]=uRang_Time[1];




 OLCD_Clear(0x0000);//清蓝色
	
  
			OLED_ShowChar(22+5,20,'R',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(37+5,20,'a',0xffff,0x0000);//显示ASCII字符	  

		OLED_ShowChar(52+5,20,'n',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(67+5,20,'g',0xffff,0x0000);//显示ASCII字符	  
	
			OLED_ShowChar(22+5,32+20,(uRang_Time_Buf[0]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(37+5,32+20,(uRang_Time_Buf[0]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(52+5,32+20,':',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(67+5,32+20,(uRang_Time_Buf[1]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(82+5,32+20,(uRang_Time_Buf[1]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
}

/*
void User_Rang_Set_Fun(void)//闹铃保存，独立出来，供给时间重新设置时用
{
	u32 tempCountValue = 0;//设置闹钟时间
	
	u16 uNow_Time_B=0,uRang_Time_B=0,uTime_B=0;
	
	
	
	uNow_Time_B=calendar.hour*60+calendar.min;
	uRang_Time_B=uRang_Time[0]*60+uRang_Time[1];
	
	if(uNow_Time_B>=uRang_Time_B) uTime_B=(1440-uNow_Time_B)+uRang_Time_B;//闹铃时间差
	else uTime_B=uRang_Time_B-uNow_Time_B;//闹铃时间差
		
	

		tempCountValue=RTC_GetCounter();
				tempCountValue = tempCountValue + uTime_B*60;
	
		RTC_WaitForLastTask();
			RTC_SetAlarm(tempCountValue);	//设置新的闹钟值
			RTC_WaitForLastTask();
	





}
*/
/*
void Action3D_10Min_Save_Rang_Choose1(void)//选择闹铃或10分钟保存一次数据
{
		u32 tempCountValue = 0;//设置闹钟时间
			u32 uSUB_Buf0=0;//缓存
		u32 uSUB_Buf1=0;//缓存	
	
	
	
	
	
if(f_Rang_Run==1)//有开启闹铃
	{
		if((calendar.hour*60+calendar.min)<=(uRang_Time[0]*60+uRang_Time[1]))//如果现实时间小于闹铃时间 
		{
			uSUB_Buf0=((uRang_Time[0]*60+uRang_Time[1])-(calendar.hour*60+calendar.min))*60;//现实时间和闹铃时间差
			uSUB_Buf1=(10-calendar.min%10)*60;//距离下次保存数据的时间差
			
			if((uSUB_Buf0>uSUB_Buf1)||(uSUB_Buf0==0))//闹铃比(10分保存一次数据)晚到
			{
				tempCountValue=RTC_GetCounter();
				tempCountValue = tempCountValue + uSUB_Buf1;//N分钟后再闹铃
				//				tempCountValue = tempCountValue + 20;//N分钟后再闹铃
				RTC_WaitForLastTask();
				RTC_SetAlarm(tempCountValue);	//设置新的闹钟值
				RTC_WaitForLastTask();
			}else
			{
				tempCountValue=RTC_GetCounter();
				tempCountValue = tempCountValue + uSUB_Buf0;//N分钟后再闹铃
				//tempCountValue = tempCountValue + 20;//N分钟后再闹铃
				RTC_WaitForLastTask();
				RTC_SetAlarm(tempCountValue);	//设置新的闹钟值
				RTC_WaitForLastTask();
			}
		}else
		{
				uSUB_Buf1=(10-calendar.min%10)*60;
		tempCountValue=RTC_GetCounter();
		tempCountValue = tempCountValue + uSUB_Buf1;//N分钟后再闹铃
			//				tempCountValue = tempCountValue + 20;//N分钟后再闹铃
		RTC_WaitForLastTask();
		RTC_SetAlarm(tempCountValue);	//设置新的闹钟值
		RTC_WaitForLastTask();
		
		
		}
	}else
	{
		uSUB_Buf1=(10-calendar.min%10)*60;
		tempCountValue=RTC_GetCounter();
		tempCountValue = tempCountValue + uSUB_Buf1;//N分钟后再闹铃
		RTC_WaitForLastTask();
		RTC_SetAlarm(tempCountValue);	//设置新的闹钟值
		RTC_WaitForLastTask();
	}
			 



}*/

void User_Rang_Set_Exit(void)//退出闹铃设置，保存
{


	
	
	
			uRang_Time[0]=uRang_Time_Buf[0];
		uRang_Time[1]=uRang_Time_Buf[1];


	f_Rang_Run=1;//开始闹铃
	

	
	
	 //User_Rang_Set_Fun();//闹铃保存，独立出来，供给时间重新设置时用
		 Action3D_10Min_Save_Rang_Choose();//选择闹铃或10分钟保存一次数据
	
 //OLCD_Clear(0xf800);//清蓝色
		uMenu_No=8;//时间设置

}
void User_Rang_Set_Exit_NotSave(void)//退出闹铃设置，不保存
{

	
	//RTC_Set(2014,11,18,uClock_Hour_Set,uClock_Min_Set,0);

	uMenu_No=8;//时间设置
	f_Rang_Run=0;//停止闹铃
	u_Rang_Remind_No=0;//复位
 //OLCD_Clear(0xf800);//清蓝色
	

}

void User_Rang_Set_Bit_Next(void)//闹铃设置,下一位
{

		uRang_Set_Point++;
	if(uRang_Set_Point>1)	uRang_Set_Point=0;//
	
	uMenu_Not_Change_Time=0;//定时睡眠，清零
	
	
}

void User_Rang_Set_Bit_Before(void)//闹铃设置,上一位
{
	uRang_Set_Point--;
	if(uRang_Set_Point>200)	uClock_Set_Point=1;//
	
		uMenu_Not_Change_Time=0;//定时睡眠，清零
}


void User_Rang_Set_Bit_Add(void)//闹铃设置,位加
{

		if(uRang_Set_Point==0)
		{
		uRang_Time_Buf[0]++;
			if(uRang_Time_Buf[0]>23)		uRang_Time_Buf[0]=0;
			
			
			
		}else 
		if(uRang_Set_Point==1)
		{
			uRang_Time_Buf[1]++;
			if(uRang_Time_Buf[1]>59)		uRang_Time_Buf[1]=0;
		}		
		uSec_Blink_Suspended_Rang=10;//不闪
		
		uMenu_Not_Change_Time=0;//定时睡眠，清零
	
}

void Rang_Show_Set(void)//设置闹铃时，显示数字时间，闪烁
{
	//static u8 uSec_Before=255;//秒，秒有变化才刷新
	static u8 uSec_Blink_No=0;//秒闪计数

	
	
	
	
	uSec_Blink_No++;
	if(uSec_Blink_No>=5) uSec_Blink_No=0;
	
	
	if(uSec_Blink_Suspended_Rang>0)
	{
	uSec_Blink_Suspended_Rang--;
		
	uSec_Blink_No=0;//清零，暂停闪烁
	
	
	}
	
	
	if(uSec_Blink_No==0)
	{


			OLED_ShowChar(22+5,32+20,(uRang_Time_Buf[0]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(37+5,32+20,(uRang_Time_Buf[0]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(52+5,32+20,':',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(67+5,32+20,(uRang_Time_Buf[1]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(82+5,32+20,(uRang_Time_Buf[1]%10+0x30),0xffff,0x0000);//显示ASCII字符	  

	}	


		if(uSec_Blink_No==3) 	
		{
			
			
			switch(uRang_Set_Point)
			{
				case	0:
			OLED_ShowChar(22+5,32+20,(uRang_Time_Buf[0]/10+0x30),0x0000,0x0000);//显示ASCII字符	  
		OLED_ShowChar(37+5,32+20,(uRang_Time_Buf[0]%10+0x30),0x0000,0x0000);//显示ASCII字符	  
				break;
				case	1:
		OLED_ShowChar(67+5,32+20,(uRang_Time_Buf[1]/10+0x30),0x0000,0x0000);//显示ASCII字符	  
		OLED_ShowChar(82+5,32+20,(uRang_Time_Buf[1]%10+0x30),0x0000,0x0000);//显示ASCII字符	
				break;			
				default:
					break;
			
			}
 
  
  
	  

			
			

		}
	
	
}








void User_Clock_Set_Into(void)//进入时间设置
{

uMenu_No=20;//时间设置
	
	

	
		uTime_World_Buf[0]=calendar.w_year-2000;
		uTime_World_Buf[1]=calendar.w_month;
		uTime_World_Buf[2]=calendar.w_date;
	uTime_World_Buf[3]=calendar.hour;

uTime_World_Buf[4]=calendar.min;	
	
	

 OLCD_Clear(0x0000);//清蓝色
	
		OLED_ShowChar2020(2+12,28,(2+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(12+12,28,(0+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(22+12,28,(uTime_World_Buf[0]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(32+12,28,(uTime_World_Buf[0]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(42+12,28,'-',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(52+12,28,(uTime_World_Buf[1]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(62+12,28,(uTime_World_Buf[1]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(72+12,28,'-',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(82+12,28,(uTime_World_Buf[2]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(92+12,28,(uTime_World_Buf[2]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
	
	
			OLED_ShowChar(22+5,32+20,(uTime_World_Buf[3]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(37+5,32+20,(uTime_World_Buf[3]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(52+5,32+20,':',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(67+5,32+20,(uTime_World_Buf[4]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(82+5,32+20,(uTime_World_Buf[4]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
}

void User_Clock_Set_Exit(void)//退出时间设置，保存
{

	
	RTC_Set(2000+uTime_World_Buf[0],uTime_World_Buf[1],uTime_World_Buf[2],uTime_World_Buf[3],uTime_World_Buf[4],0);

	uMenu_No=2;//时间设置
	
		RTC_Get();//更新时间   
	
	//	 User_Rang_Set_Fun();//闹铃保存，独立出来，供给时间重新设置时用
		 Action3D_10Min_Save_Rang_Choose();//选择闹铃或10分钟保存一次数据
	
 //OLCD_Clear(0xf800);//清蓝色
	

}
void User_Clock_Set_Exit_NotSave(void)//退出时间设置，不保存
{

	
	//RTC_Set(2014,11,18,uClock_Hour_Set,uClock_Min_Set,0);

	uMenu_No=2;//时间设置
	
	
 //OLCD_Clear(0xf800);//清蓝色
	

}


void User_Clock_Set_Bit_Next(void)//时间设置,下一位
{

		uClock_Set_Point++;
	if(uClock_Set_Point>4)	uClock_Set_Point=0;//
	
	uMenu_Not_Change_Time=0;//定时睡眠，清零
	
	
}

void User_Clock_Set_Bit_Before(void)//时间设置,上一位
{
	uClock_Set_Point--;
	if(uClock_Set_Point>200)	uClock_Set_Point=4;//
	
		uMenu_Not_Change_Time=0;//定时睡眠，清零
}


void User_Clock_Set_Bit_Add(void)//时间设置,位加
{

		switch(uClock_Set_Point)
		{
		case 0:
	uTime_World_Buf[0]++;
	if(uTime_World_Buf[0]>99) 	uTime_World_Buf[0]=0;//年
	
					if(uTime_World_Buf[1]==2)//2月
	{
	
		if(((2000+uTime_World_Buf[0])%4==0&&(2000+uTime_World_Buf[0])%100!=0)||((2000+uTime_World_Buf[0])%400==0))//闰年
		{
			
			if(uTime_World_Buf[2]>29) uTime_World_Buf[2]=29;
			
		}
		else//平年
		{
		
		if(uTime_World_Buf[2]>28) uTime_World_Buf[2]=28;
		}
	
	
	}
	
	
	break;
case 1:
		uTime_World_Buf[1]++;
		if(uTime_World_Buf[1]>12) uTime_World_Buf[1]=1;//月
			if((uTime_World_Buf[1]==4||uTime_World_Buf[1]==6||uTime_World_Buf[1]==9||uTime_World_Buf[1]==11)&&uTime_World_Buf[2]>30)//小月
uTime_World_Buf[2]=30;
				if(uTime_World_Buf[1]==2)//2月
	{
	
		if(((2000+uTime_World_Buf[0])%4==0&&(2000+uTime_World_Buf[0])%100!=0)||((2000+uTime_World_Buf[0])%400==0))//闰年
		{
			
			if(uTime_World_Buf[2]>29) uTime_World_Buf[2]=29;
			
		}
		else//平年
		{
		
		if(uTime_World_Buf[2]>28) uTime_World_Buf[2]=28;
		}
	
	
	}
		

	break;	
case 2:
	uTime_World_Buf[2]++;//日
		if(uTime_World_Buf[1]==1||uTime_World_Buf[1]==3||uTime_World_Buf[1]==5||uTime_World_Buf[1]==7||uTime_World_Buf[1]==8||uTime_World_Buf[1]==10||uTime_World_Buf[1]==12)//大月
	{
	if(uTime_World_Buf[2]>31) uTime_World_Buf[2]=1;
	
	}
			if(uTime_World_Buf[1]==4||uTime_World_Buf[1]==6||uTime_World_Buf[1]==9||uTime_World_Buf[1]==11)//小月
		{
		
		if(uTime_World_Buf[2]>30) uTime_World_Buf[2]=1;
		
		}
		if(uTime_World_Buf[1]==2)//2月
	{
	
		if(((2000+uTime_World_Buf[0])%4==0&&(2000+uTime_World_Buf[0])%100!=0)||((2000+uTime_World_Buf[0])%400==0))//闰年
		{
			
			if(uTime_World_Buf[2]>29) uTime_World_Buf[2]=1;
			
		}
		else//平年
		{
		
		if(uTime_World_Buf[2]>28) uTime_World_Buf[2]=1;
		}
	
	
	}
	
	break;
case 3:
	uTime_World_Buf[3]++;//时
	if(	uTime_World_Buf[3]>23) 	uTime_World_Buf[3]=0;

	break;
case 4:
	uTime_World_Buf[4]++;//分
	if(uTime_World_Buf[4]>59) 	uTime_World_Buf[4]=0;

	break;
default :
	break;
	}
		uSec_Blink_Suspended=10;//位加，不闪
		uMenu_Not_Change_Time=0;//定时睡眠，清零
	
}

void Clock_Show_Set(void)//设置时间时，显示数字时间，闪烁
{
	//static u8 uSec_Before=255;//秒，秒有变化才刷新
	static u8 uSec_Blink_No=0;//秒闪计数

	
	
	
	
	uSec_Blink_No++;
	if(uSec_Blink_No>=5) uSec_Blink_No=0;
	
	
	if(uSec_Blink_Suspended>0)
	{
	uSec_Blink_Suspended--;
		
	uSec_Blink_No=0;//清零，暂停闪烁
	
	
	}
	
	
	if(uSec_Blink_No==0)
	{

		OLED_ShowChar2020(2+12,28,(2+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(12+12,28,(0+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(22+12,28,(uTime_World_Buf[0]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(32+12,28,(uTime_World_Buf[0]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(42+12,28,'-',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(52+12,28,(uTime_World_Buf[1]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(62+12,28,(uTime_World_Buf[1]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(72+12,28,'-',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(82+12,28,(uTime_World_Buf[2]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(92+12,28,(uTime_World_Buf[2]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
	
	
			OLED_ShowChar(22+5,32+20,(uTime_World_Buf[3]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(37+5,32+20,(uTime_World_Buf[3]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(52+5,32+20,':',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(67+5,32+20,(uTime_World_Buf[4]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar(82+5,32+20,(uTime_World_Buf[4]%10+0x30),0xffff,0x0000);//显示ASCII字符	  

	}	


		if(uSec_Blink_No==3) 	
		{
			
			
			switch(uClock_Set_Point)
			{
				case	0:
							OLED_ShowChar2020(22+12,28,(uTime_World_Buf[0]/10+0x30),0x0000,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(32+12,28,(uTime_World_Buf[0]%10+0x30),0x0000,0x0000);//显示ASCII字符	
				break;
				case	1:
							OLED_ShowChar2020(52+12,28,(uTime_World_Buf[1]/10+0x30),0x0000,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(62+12,28,(uTime_World_Buf[1]%10+0x30),0x0000,0x0000);//显示ASCII字符
				break;		
				case	2:
					
		OLED_ShowChar2020(82+12,28,(uTime_World_Buf[2]/10+0x30),0x0000,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(92+12,28,(uTime_World_Buf[2]%10+0x30),0x0000,0x0000);//显示ASCII字符	 
				break;		
				case	3:
										OLED_ShowChar(22+5,52,(uTime_World_Buf[3]/10+0x30),0x0000,0x0000);//显示ASCII字符	
		OLED_ShowChar(37+5,52,(uTime_World_Buf[3]%10+0x30),0x0000,0x0000);//显示ASCII字符	
				break;		
				case	4:
								OLED_ShowChar(67+5,52,(uTime_World_Buf[4]/10+0x30),0x0000,0x0000);//显示ASCII字符	  
		OLED_ShowChar(82+5,52,(uTime_World_Buf[4]%10+0x30),0x0000,0x0000);//显示ASCII字符	 
				break;
				default:
					break;
			
			}
 
  
  
	  

			
			

		}
	
	
}

void Clock_Show_Watch(void)//表盘显示
{
	
			//	OLCD_Clear(0x0000);
	
	/*static u8 uStep_Count_No_Bef=255;
		static u8 uRiss_Count_No_Bef=255;
	
	if(uStep_Count_No_Bef!=uStep_Count_No)
	{
	
	uStep_Count_No_Bef=uStep_Count_No;
		
		OLED_ShowChar2020(12+12,28,(uStep_Count_No/100+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(22+12,28,(uStep_Count_No%100/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(32+12,28,(uStep_Count_No%10+0x30),0xffff,0x0000);//显示ASCII字符	 
	
	}
	
	
	
		if(uRiss_Count_No_Bef!=uRiss)
	{
		uRiss_Count_No_Bef=uRiss;
		
				OLED_ShowChar2020(22+12,50,(uRiss/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(32+12,50,(uRiss%10+0x30),0xffff,0x0000);//显示ASCII字符	 
		
		
		
	}
	*/
	
	

	static u8 uSec_Before=255;//秒，秒有变化才刷新
	static u8 uSec_Blink_No=0;//秒闪计数

	if(uSec_Before!=calendar.sec)
	{
	uSec_Before=calendar.sec;
		
			if(uMode_Old!=uMenu_No)
			{
				uMode_Old=uMenu_No;//赋值
			OLCD_Clear(0x0000);
			calendar_circle_clock_drawpanel(63,53,84,2);//显示指针时钟表盘 
			}
			calendar_circle_clock_showtime(64,53,80,3,calendar.hour%12,calendar.min,calendar.sec);
		
		}
	
	
	
	
}
void Day_Show_Watch(void)//日期显示
{
	
	
		u8 data111=0;
	
	// AT24CXX_WriteOneByte(0,11);
	
	
// data111=AT24CXX_ReadOneByte(0);
	

			if(uMode_Old!=uMenu_No)
			{
				uMode_Old=uMenu_No;//赋值
				
			OLCD_Clear(0x0000);//清

						uTime_World_Buf[0]=calendar.w_year-2000;
		uTime_World_Buf[1]=calendar.w_month;
		uTime_World_Buf[2]=calendar.w_date;
	uTime_World_Buf[3]=calendar.hour;
	//	uTime_World_Buf[2]=		data111;
				
				

uTime_World_Buf[4]=calendar.min;	
				
		OLED_ShowChar2020(2+12,28,(2+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(12+12,28,(0+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(22+12,28,(uTime_World_Buf[0]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(32+12,28,(uTime_World_Buf[0]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(42+12,28,'-',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(52+12,28,(uTime_World_Buf[1]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(62+12,28,(uTime_World_Buf[1]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(72+12,28,'-',0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(82+12,28,(uTime_World_Buf[2]/10+0x30),0xffff,0x0000);//显示ASCII字符	  
		OLED_ShowChar2020(92+12,28,(uTime_World_Buf[2]%10+0x30),0xffff,0x0000);//显示ASCII字符	  
		
	 
		

		
		
	if((uTemp_Dat_Flag&0x80)==0x80) OLED_ShowChar2020(2+12,32+29,' ',0xffff,0x0000);//显示ASCII字符	  
	else OLED_ShowChar2020(2+12,32+29,'-',0xffff,0x0000);//显示ASCII字符	  
			
			OLED_ShowChar2020(2+12+7,32+29,uTemp_Dat/10+0x30,0xffff,0x0000);//显示ASCII字符	
			OLED_ShowChar2020(2+12+17,32+29,uTemp_Dat%10+0x30,0xffff,0x0000);//显示ASCII字符	
			
	if((uTemp_Dat_Flag&0x40)==0x40)
	{
		//显示℃
			OLED_ShowChar2020(2+12+27,32+29,'<',0xffff,0x0000);//显示ASCII字符	  
			OLED_ShowChar2020(2+12+37,32+29,'>',0xffff,0x0000);//显示ASCII字符	 
	}else
	{
		//℉
			OLED_ShowChar2020(2+12+27,32+29,'[',0xffff,0x0000);//显示ASCII字符	  
			OLED_ShowChar2020(2+12+8+27,32+29,'F',0xffff,0x0000);//显示ASCII字符		
	
	}



	//	u8 uTemp_Dat=0x16;//温度数据
//u8 uTemp_Dat_Flag=0x00;//温度数据,温度用
		
		
		
		
		
			switch(calendar.week)
			{
				case 1:
					OLED_ShowChar(22+40,32+20,'M',0xffff,0x0000);//显示ASCII字符	  
					OLED_ShowChar(37+40,32+20,'o',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(52+40,32+20,'n',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(67+40,32+20,'n',0x0000,0x0000);//显示ASCII字符	 	
						break;
				case 2:
					OLED_ShowChar(22+40,32+20,'T',0xffff,0x0000);//显示ASCII字符	  
					OLED_ShowChar(37+40,32+20,'u',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(52+40,32+20,'e',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(67+40,32+20,'s',0xffff,0x0000);//显示ASCII字符	 			
						break;
				case 3:
					OLED_ShowChar(22+40,32+20,'W',0xffff,0x0000);//显示ASCII字符	  
					OLED_ShowChar(37+40,32+20,'e',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(52+40,32+20,'d',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(67+40,32+20,'d',0x0000,0x0000);//显示ASCII字符	 			
						break;
				case 4:
					OLED_ShowChar(22+40,32+20,'T',0xffff,0x0000);//显示ASCII字符	  
					OLED_ShowChar(37+40,32+20,'h',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(52+40,32+20,'u',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(67+40,32+20,'r',0xffff,0x0000);//显示ASCII字符	 			
						break;
				case 5:
					OLED_ShowChar(22+40,32+20,'F',0xffff,0x0000);//显示ASCII字符	  
					OLED_ShowChar(37+40,32+20,'r',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(52+40,32+20,'i',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(67+40,32+20,'i',0x0000,0x0000);//显示ASCII字符	 			
						break;
				case 6:
					OLED_ShowChar(22+40,32+20,'S',0xffff,0x0000);//显示ASCII字符	  
					OLED_ShowChar(37+40,32+20,'a',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(52+40,32+20,'t',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(67+40,32+20,'t',0x0000,0x0000);//显示ASCII字符	 			
						break;
				case 7:
					OLED_ShowChar(22+40,32+20,'S',0xffff,0x0000);//显示ASCII字符	  
					OLED_ShowChar(37+40,32+20,'u',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(52+40,32+20,'n',0xffff,0x0000);//显示ASCII字符	 
					OLED_ShowChar(67+40,32+20,'n',0x0000,0x0000);//显示ASCII字符	 			
						break;	
				default:
					
					break;

			}
			}

}
	
	
	
void Remind_Show(void)//提醒显示
{
//static u8 f_Call_B=0;//来电标志
//static u8 f_Misscall_B=0;//未接来电标志
//static u8 f_MSM_B=0;//短信标志
	
	static u8 f_Misscall_Before=0;//未接来电标志保存，才能动态更新
	
		static u8 f_Call_Before=0;//来电标志保存，才能动态更新
	
	
				if(uMode_Old!=uMenu_No||f_Misscall_Before!=f_Misscall||f_Call_Before!=f_Call)
			{
				f_Misscall_Before=f_Misscall;//更新值
				f_Call_Before=f_Call;//更新值
				uMode_Old=uMenu_No;//赋值
			//	f_Call_B=f_Call;
			//	f_Misscall_B=f_Misscall;
			//	f_MSM_B=f_MSM;
				
			OLCD_Clear(0x0000);//清
				
				//u8 f_Call=0;//来电标志
//u8 f_Misscall=0;//未接来电标志
//u8 f_MSM=0;//短信标志
			
		if(f_Call==1)		
			{
			
				OLED_DrawICO_3030(36, 23,gImage_2);//来电
		}else
			{
			if(f_Misscall==1)	OLED_DrawICO_3030(36, 23,gImage_12);//未接来电
			
			
			}
			if(f_MSM==1)			OLED_DrawICO_3030(36, 53,gImage_10);//短信
				if(u_Rang_Remind_No!=0)		OLED_DrawICO_Small(66, 23,gImage_8);//闹钟
			
			
			
	//	OLED_DrawICO_Small(36, 23,gImage_2);//来电
	//	OLED_DrawICO_Small(66, 23,gImage_12);//未接来电
	//	OLED_DrawICO_Small(36, 53,gImage_10);//短信
	
			}
	
	
	
	
	
	
}
	
	
	
	
	

void User_Clock_Show_Task(void *pdata)//时钟显示任务
{

	//static u8	f_Clock_to_Num=0;//从唤醒，时钟。切换进时间调节界面，标志
//	static u8	f_Rang_to_Num=0;//从唤醒，时钟。切换进时间调节界面，标志
	
	
	 
	
	
		      //OLCD_Clear(0x0000);
	//calendar_circle_clock_drawpanel(63,53,84,2);//显示指针时钟表盘 
	//	calendar_circle_clock_showtime(64,53,80,3,10,0,r);
	
	
	
		while(1)
	{

		
		
		// User_Rang_Fun();//闹铃扫描
		
		 Sys_Time_Save();//系统时间保存
		
		
	//	if(uMenu_No==1)f_Clock_to_Num=1;
		//else 	if(uMenu_No!=20)f_Clock_to_Num=0;
		
		if(uMenu_No!=102&&uMenu_No!=101&&uMenu_No!=1&&uMenu_No!=20)	uMode_Old=255;//防止切换，不刷新表盘
		
		switch(uMenu_No)
		{
		case 0:
		
			break;			
		case 1:
			Clock_Show_Watch();//表盘显示
			break;
		case 101:
			 Day_Show_Watch();//日期显示		
			break;
		case 102:
			 Remind_Show();//提醒显示
			break;		
		case 50:
					 User_Step_Show();//显示步数界面
			break;	
	case 60:
					 User_Distance_Show();//显示米距界面
			break;			
		case 70:
					User_Search_Phone_Show();//显示查找手机动态界面
			break;		
	 
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:			
			//uMode_Old=uMenu_No;//赋值
			//Clock_Show_Home();//首页显示数字时间	
			break;
		case 20:
			
		// if(f_Clock_to_Num==1)
		// {			
		//	 f_Clock_to_Num=0;
		//	 OLCD_Clear(0x0000);//清蓝色,防止残留，从时钟唤醒切换过来的
		// }
			uMode_Old=uMenu_No;//赋值
			Clock_Show_Set();//设置时间时，显示数字时间，闪烁
			break;
		case 80:
			
			//	 if(f_Clock_to_Num==1)
		// {			
			// f_Clock_to_Num=0;
			// OLCD_Clear(0x0000);//清蓝色,防止残留，从时钟唤醒切换过来的
		 //}
			uMode_Old=uMenu_No;//赋值
			Rang_Show_Set();//设置时间时，显示数字时间，闪烁
			
			break;
		default:
			break;

}
		

		
	
		
		
 
		
		
		
		//calendar.sec
		

		
		
		
		delay_ms(100);

	}
	





}
























