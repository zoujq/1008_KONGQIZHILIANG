#include "includes.h"	   
#include "stdlib.h"	




u8 uTime_World_Buf[6]={12,5,25,13,45,6}; //������ʱ����,ϵͳʱ����㻺��
u8 uRang_Time[2]={10,02};//����ʱ��
u8 uRang_Time_Buf[2]={0,0};//����ʱ��,����

u8 f_Rang_Run=1;//����ʹ�ܱ�־λ

u16 uRang_Times=0;//������ʱ��

u16 uRang_Times_30S=0;//����,30�����壬ÿ3����һ�Σ�����3��


//u8 uClock_Hour_Set;//ʱ������Сʱ����
//u8 uClock_Min_Set;//ʱ�����÷ֻ���
u8 uClock_Set_Point=0;//ʱ������λָʾ
u8 uRang_Set_Point=0;//��������λָʾ
u8 uMusic_Set_Point=0;//����λָʾ


u8 f_Music_Set_Start=0;//��ͣ�򲥷Ű���


	 u16	uMode_Old=0xffff;//֮ǰ�Ĳ˵����жϱ���ˢ����


	static u8 uSec_Blink_Suspended=0;//��������,��ͣ
	static u8 uSec_Blink_Suspended_Rang=0;//��������,��ͣ


u8 f_User_Sleep=0;//�û�˯�߱�־��0��˯��1˯��






u8 uData_Buf_Music[10]={0};//



u8	f_Search_Phone_Show=0;//��̬ͼ����ʾ
u8	uSearch_Phone_Run=0;//��̬ͼ����ʾ,ͼ���






void User_Clock_Init(void)//RTC��ʼ��
{



	u8 res;
			res=RTC_Init();	  			//RTC��ʼ��
		//while(RTC_Init())	

		//	rtccnt=RTC->CNTH;//�õ�����ֵ
//	rtccnt<<=16;
	//rtccnt|=RTC->CNTL;
	//ALARM_CNT_Set(rtccnt+1);//����8���Ӻ����ӻ���
//	RTC_SetAlarm(RTC_GetCounter());
//		SPI_Flash_Read(Data_Save_Buf,SYSTIME_ADDRESS_SAVE,7);		//���磬����ʱ��
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


void Sys_Time_Save(void)//ϵͳʱ�䱣��
{
	static u16 uYear_Before=255;//�꣬���б仯��ˢ��
	static u8 uMonth_Before=255;//�£����б仯��ˢ��	
	static u8 uDay_Before=255;//�գ����б仯��ˢ��
	static u8 uHour_Before=255;//ʱ��ʱ�б仯��ˢ��
	static u8 uMin_Before=255;//�֣����б仯��ˢ��

	
		//	u8 Data_Save_Buf0[10]={0};//���滺��
			u8 Data_Save_Buf1[10]={0};//���滺��
	
	
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
		
///SPI_Flash_Init();//�����������ֹ���ݷ����仯��˯��ʱ��ʱ�ӿ��ܷ����仯��û��������ݻ����
		///SPI_Flash_Write(Data_Save_Buf,SYSTIME_ADDRESS_SAVE,7);		//���磬����ʱ��

	 AT24CXX_Write(SYSTIME_ADDRESS_SAVE,Data_Save_Buf1,7);
	//	AT24CXX_Read(SYSTIME_ADDRESS_SAVE,Data_Save_Buf0,7);
	}
}


void Clock_Show_Home(void)//��ҳ��ʾ����ʱ��
{
	static u8 uSec_Before=255;//�룬���б仯��ˢ��
	static u8 uSec_Blink_No=0;//��������
	

	


	if(uSec_Before!=calendar.sec)
	{
		uSec_Before=calendar.sec;
		OLED_ShowChar(22+5,15,(calendar.hour/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(37+5,15,(calendar.hour%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(52+5,15,':',0xffff,0xffff);//��ʾASCII�ַ�	  
		OLED_ShowChar(67+5,15,(calendar.min/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(82+5,15,(calendar.min%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		uSec_Blink_No=6;
	}	
	if(uSec_Blink_No>0)
	{
		uSec_Blink_No--;
		if(uSec_Blink_No==0) 	
		OLED_ShowChar(52+5,15,':',0x0000,0x0000);//��ʾASCII�ַ�	  
	}

	
	
	
	
	
	
	
}







void User_Music_Set_Into(void)//�������ֲ���
{

	

	
uMenu_No=40;//���ֲ�������
	

 OLCD_Clear(0x0000);//����ɫ
	
	
	
	


	
		 gui_fill_keyLeft(9,33,31,0xffff,0x0000);//��
	
		 gui_fill_keyStart(49,33,31,0xffff,0x0000);//����
	
		 gui_fill_keyRight(89,33,31,0xffff,0x0000);//��
	
	switch(uMusic_Set_Point)
	{
		case 0:
					 gui_fill_keyLeft(9,33,31,0x0000,0xffff);//��
				break;
		case 1:
			if(f_Music_Set_Start==0)
				 gui_fill_keyStart(49,33,31,0x0000,0xffff);//����
			else gui_fill_keyPause(49,33,31,0x0000,0xffff);//��ͣ
				break;		
		case 2:
					 gui_fill_keyRight(89,33,31,0x0000,0xffff);//��
				break;	
		default:
		break;
	}
	
		uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}





	
	
	
	


void User_Music_Set_Exit(void)//�˳����ֲ���
{


	uMenu_No=4;//���ֲ���
	
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����

}


	
void User_Music_Set_Bit_Next(void)//���ֲ���,��һ������
{

		uMusic_Set_Point++;
	if(uMusic_Set_Point>2)	uMusic_Set_Point=0;//
		
		 gui_fill_keyLeft(9,33,31,0xffff,0x0000);//��
	
		if(f_Music_Set_Start==0) gui_fill_keyStart(49,33,31,0xffff,0x0000);//����
		else gui_fill_keyPause(49,33,31,0xffff,0x0000);//��ͣ
	
		 gui_fill_keyRight(89,33,31,0xffff,0x0000);//��
	
	switch(uMusic_Set_Point)
	{
		case 0:
					 gui_fill_keyLeft(9,33,31,0x0000,0xffff);//��
				break;
		case 1:
			if(f_Music_Set_Start==0)
				 gui_fill_keyStart(49,33,31,0x0000,0xffff);//����
			else gui_fill_keyPause(49,33,31,0x0000,0xffff);//��ͣ
				break;		
		case 2:
					 gui_fill_keyRight(89,33,31,0x0000,0xffff);//��
				break;	
		default:
		break;
	}
	
	
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
	
	
}



	
void User_Music_Set_Bit_Before(void)//���ֲ���,��һ������
{
	uMusic_Set_Point--;
	if(uMusic_Set_Point>200)	uMusic_Set_Point=2;//
		
		 gui_fill_keyLeft(9,33,31,0xffff,0x0000);//��
	
		if(f_Music_Set_Start==0) gui_fill_keyStart(49,33,31,0xffff,0x0000);//����
		else gui_fill_keyPause(49,33,31,0xffff,0x0000);//��ͣ
	
		 gui_fill_keyRight(89,33,31,0xffff,0x0000);//��
	
	switch(uMusic_Set_Point)
	{
		case 0:
					 gui_fill_keyLeft(9,33,31,0x0000,0xffff);//��
				break;
		case 1:
			if(f_Music_Set_Start==0)
				 gui_fill_keyStart(49,33,31,0x0000,0xffff);//����
			else gui_fill_keyPause(49,33,31,0x0000,0xffff);//��ͣ
				break;		
		case 2:
					 gui_fill_keyRight(89,33,31,0x0000,0xffff);//��
				break;	
		default:
		break;
	}
	
	
		uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}


void User_Music_Set_Bit_In(void)//���ֲ��ţ�����ѡ��
{

//ѡ��ͼ�����


	

	
		
		 gui_fill_keyLeft(9,33,31,0xffff,0x0000);//��
	
			if(f_Music_Set_Start==0) gui_fill_keyStart(49,33,31,0xffff,0x0000);//����
		else gui_fill_keyPause(49,33,31,0xffff,0x0000);//��ͣ
	
		 gui_fill_keyRight(89,33,31,0xffff,0x0000);//��
	
	switch(uMusic_Set_Point)
	{
		case 0:
					 gui_fill_keyLeft(9,33,31,0x0000,0xffff);//��
		
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
				gui_fill_keyStart(49,33,31,0x0000,0xffff);//����

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
				gui_fill_keyPause(49,33,31,0x0000,0xffff);//��ͣ
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
					 gui_fill_keyRight(89,33,31,0x0000,0xffff);//��
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
	
	


	
			BLE_WAKEUP;//��������
delay_ms(10);

			User_UART1_SendStringx(uData_Buf_Music,8);
		BLE_SLEEP;//˯������
	
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
	
	
	
	
}






void User_Step_Into(void)//����Ʋ�����
{
	uMenu_No=50;//�Ʋ�
	OLCD_Clear(0x0000);//����ɫ
	OLED_ShowChar(19,32,'S',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar(34,32,'t',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar(49,32,'e',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar(64,32,'p',0xffff,0x0000);//��ʾASCII�ַ�	
	OLED_ShowChar(79,32,'s',0xffff,0x0000);//��ʾASCII�ַ�		
	OLED_ShowChar2020(34,64,uStep_Count_No/10000+0x30,0xffff,0x0000);
	OLED_ShowChar2020(44,64,uStep_Count_No%10000/1000+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(54,64,uStep_Count_No%1000/100+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(64,64,uStep_Count_No%100/10+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(74,64,uStep_Count_No%10+0x30,0xffff,0x0000);		
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}

void User_Step_Show(void)//��ʾ��������
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

void User_Step_Exit(void)//�˳��Ʋ�����
{
	uMenu_No=5;//�Ʋ�
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}

void User_Step_Clear(void)//�Ʋ�����
{
	uStep_Count_No=0;//����
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}











void User_Distance_Into(void)//�����׾����
{
	/*u32 uDistance_B=0;//����
	
	
	
	uMenu_No=60;//�Ʋ�
	OLCD_Clear(0x0000);//����ɫ
	uDistance_B=(uStep_Count_No*u_User_Stride)/100;
	
	
	OLED_ShowChar2020(24,32,'D',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar2020(34,32,'i',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar2020(44,32,'s',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar2020(54,32,'t',0xffff,0x0000);//��ʾASCII�ַ�	
	OLED_ShowChar2020(64,32,'a',0xffff,0x0000);//��ʾASCII�ַ�		
	OLED_ShowChar2020(74,32,'n',0xffff,0x0000);//��ʾASCII�ַ�		
	OLED_ShowChar2020(84,32,'c',0xffff,0x0000);//��ʾASCII�ַ�		
	OLED_ShowChar2020(94,32,'e',0xffff,0x0000);//��ʾASCII�ַ�		
	
	
	OLED_ShowChar2020(34,64,uDistance_B/10000+0x30,0xffff,0x0000);
	OLED_ShowChar2020(44,64,uDistance_B%10000/1000+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(54,64,uDistance_B%1000/100+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(64,64,uDistance_B%100/10+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(74,64,uDistance_B%10+0x30,0xffff,0x0000);		
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
	*/
	
		float uCalories_B=0;//����
	u32 uCalories_C=0;//����
	u32 uDistance_B=0;//����
	
	
	uMenu_No=60;//�׾�Ϳ�·��
	OLCD_Clear(0x0000);//����ɫ
	uDistance_B=(uStep_Count_No*u_User_Stride)/100;
	uCalories_B=(uStep_Count_No*u_User_Stride)/100/1000.0;
	uCalories_B=uCalories_B*u_User_Weight*1.036;
	uCalories_C=uCalories_B;
	
	
		OLED_ShowChar2020(24,10,'D',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar2020(34,10,'i',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar2020(44,10,'s',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar2020(54,10,'t',0xffff,0x0000);//��ʾASCII�ַ�	
	OLED_ShowChar2020(64,10,'a',0xffff,0x0000);//��ʾASCII�ַ�		
	OLED_ShowChar2020(74,10,'n',0xffff,0x0000);//��ʾASCII�ַ�		
	OLED_ShowChar2020(84,10,'c',0xffff,0x0000);//��ʾASCII�ַ�		
	OLED_ShowChar2020(94,10,'e',0xffff,0x0000);//��ʾASCII�ַ�		
	
	
	OLED_ShowChar2020(34,30,uDistance_B/10000+0x30,0xffff,0x0000);
	OLED_ShowChar2020(44,30,uDistance_B%10000/1000+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(54,30,uDistance_B%1000/100+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(64,30,uDistance_B%100/10+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(74,30,uDistance_B%10+0x30,0xffff,0x0000);		
	
	
	OLED_ShowChar2020(24,50,'C',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar2020(34,50,'a',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar2020(44,50,'l',0xffff,0x0000);//��ʾASCII�ַ�	 	
	OLED_ShowChar2020(54,50,'o',0xffff,0x0000);//��ʾASCII�ַ�	
	OLED_ShowChar2020(64,50,'r',0xffff,0x0000);//��ʾASCII�ַ�		
	OLED_ShowChar2020(74,50,'i',0xffff,0x0000);//��ʾASCII�ַ�		
	OLED_ShowChar2020(84,50,'e',0xffff,0x0000);//��ʾASCII�ַ�		
	OLED_ShowChar2020(94,50,'s',0xffff,0x0000);//��ʾASCII�ַ�		
	
	
	OLED_ShowChar2020(34,70,uCalories_C/10000+0x30,0xffff,0x0000);
	OLED_ShowChar2020(44,70,uCalories_C%10000/1000+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(54,70,uCalories_C%1000/100+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(64,70,uCalories_C%100/10+0x30,0xffff,0x0000);	
	OLED_ShowChar2020(74,70,uCalories_C%10+0x30,0xffff,0x0000);		
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
	
	
	
	
	
}

void User_Distance_Show(void)//��ʾ�׾����
{
	u32 uDistance_B=0;//����
	static u16 uDistance_Count_No_B=0;
		
		static u16 uCalories_Count_No_B=0;
	
		float uCalories_B=0;//����
	u32 uCalories_C=0;//����
	
	
	
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

void User_Distance_Exit(void)//�˳��׾����
{
	uMenu_No=6;//
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}




void User_Search_Phone_Into(void)//��������ֻ�����
{
	u8 i;

		uMenu_No=70;//
		OLCD_Clear(0x0000);//����ɫ
		OLED_DrawICO_Small(36, 40,gImage_4);//�����ֻ�
			f_Search_Phone_Show=0;//����
		uSearch_Phone_Run=0;//����
	
	
		for(i=0;i<7;i++)
	{
		gui_draw_bline1(64+i,68-i,64+i,68,0,0xffff); 
	}
	

					uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
	

}

u8 uData_Buf_Search_Phone[10]={0};
void User_Search_Phone_Able(void)//ѡ������ֻ���ر��ֻ�����
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
	
		BLE_WAKEUP;//��������
		delay_ms(10);
		User_UART1_SendStringx(uData_Buf_Search_Phone,8);
		BLE_SLEEP;//˯������
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
	
		BLE_WAKEUP;//��������
		delay_ms(10);
		User_UART1_SendStringx(uData_Buf_Search_Phone,8);
		BLE_SLEEP;//˯������
	}
	
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}

void User_Search_Phone_Show(void)//��ʾ�����ֻ���̬����
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



void User_Calories_Exit(void)//�˳������ֻ�����
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
	
		BLE_WAKEUP;//��������
delay_ms(10);
		User_UART1_SendStringx(uData_Buf_Search_Phone,8);
		BLE_SLEEP;//˯������
	
	
	
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}




void User_Sleep_Test_Fun(void)//˯�߼����ʾ
{
	
	u8 i;
	
for(i=0;i<30;i++)//����
	{
		gui_draw_bline1(35,39+i,90,39+i,0,0x0000); 
	}
	
	if(f_User_Sleep==1)
	{

		
		OLED_ShowChar(49,37,'O',0xffff,0x0000);//��ʾASCII�ַ�	 
	OLED_ShowChar(64,37,'N',0xffff,0x0000);//��ʾASCII�ַ�	 

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

		
	OLED_ShowChar(42,37,'O',0xffff,0x0000);//��ʾASCII�ַ�	 
	OLED_ShowChar(57,37,'F',0xffff,0x0000);//��ʾASCII�ַ�	 
	OLED_ShowChar(72,37,'F',0xffff,0x0000);//��ʾASCII�ַ�	
	
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

void User_Sleep_Test_Into(void)//����˯�߼�����
{
	u8 i;
	
	
	
	uMenu_No=90;//˯�߼��
	OLCD_Clear(0x0000);//����ɫ
		User_Sleep_Test_Fun();//˯�߼����ʾ

	
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}

void User_Sleep_Test_Change(void)//˯�߼������л�
{
	
	

	if(f_User_Sleep==1)	
	{
		f_User_Sleep=0;//���û�˯��
		uSleep_Count_No_10min=0;//�������㣬��ʼ���¼���		
	}	else//�û�˯��
	{		
		f_User_Sleep=1;	
		uSleep_Count_No_10min=0;//�������㣬��ʼ���¼���
	}
	User_Sleep_Test_Fun();//˯�߼����ʾ
	
//SPI_Flash_Init();//�����������ֹ���ݷ����仯��˯��ʱ��ʱ�ӿ��ܷ����仯��û��������ݻ����	
///		SPI_Flash_Write(&f_User_Sleep,USER_SLEEP_ADDRESS_SAVE,1);		//�����û����ݣ��û�˯��״̬
	AT24CXX_Write(USER_SLEEP_ADDRESS_SAVE,&f_User_Sleep,1);
	
	
	
}

void User_Sleep_Test_Exit(void)//�˳�˯�߼�����
{
	uMenu_No=9;//˯�߼��
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}










void User_Rang_Pause(void)//������ͣ
{

//uRang_Times_30S=0;

	u_Rang_Remind_No=0;//��λ

}

void User_Rang_Stop(void)//����ֹͣ
{

//uRang_Times_30S=0;//��λ
//uRang_Times=0;//��λ
	u_Rang_Remind_No=0;//��λ

}


/*
void User_Rang_Fun(void)//����ɨ��
{

	static u8 uTime_500MS_No=0;//500ms��ʱ
	
	
	
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
	
	
	uRang_Times_30S=60;//30��
		
	
	}
	
	}
	
		if(uRang_Times_30S>0)
	{
	
	
		if(uRang_Times_30S%2==0)   Buzzer_Run(1);//������,һ��
		
		
	uRang_Times_30S--;
		
		
	
	
	}
	

if((uRang_Time[0]==calendar.hour)&&(uRang_Time[1]==calendar.min)&&calendar.sec==0&&f_Rang_Run==1)
{


uRang_Times=721;//������3��







}












}
	
	*/
	
	
	
	
	
	



void User_Rang_Set_Into(void)//������������
{

uMenu_No=80;//��������
	
	

	
		uRang_Time_Buf[0]=uRang_Time[0];
		uRang_Time_Buf[1]=uRang_Time[1];




 OLCD_Clear(0x0000);//����ɫ
	
  
			OLED_ShowChar(22+5,20,'R',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(37+5,20,'a',0xffff,0x0000);//��ʾASCII�ַ�	  

		OLED_ShowChar(52+5,20,'n',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(67+5,20,'g',0xffff,0x0000);//��ʾASCII�ַ�	  
	
			OLED_ShowChar(22+5,32+20,(uRang_Time_Buf[0]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(37+5,32+20,(uRang_Time_Buf[0]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(52+5,32+20,':',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(67+5,32+20,(uRang_Time_Buf[1]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(82+5,32+20,(uRang_Time_Buf[1]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
}

/*
void User_Rang_Set_Fun(void)//���屣�棬��������������ʱ����������ʱ��
{
	u32 tempCountValue = 0;//��������ʱ��
	
	u16 uNow_Time_B=0,uRang_Time_B=0,uTime_B=0;
	
	
	
	uNow_Time_B=calendar.hour*60+calendar.min;
	uRang_Time_B=uRang_Time[0]*60+uRang_Time[1];
	
	if(uNow_Time_B>=uRang_Time_B) uTime_B=(1440-uNow_Time_B)+uRang_Time_B;//����ʱ���
	else uTime_B=uRang_Time_B-uNow_Time_B;//����ʱ���
		
	

		tempCountValue=RTC_GetCounter();
				tempCountValue = tempCountValue + uTime_B*60;
	
		RTC_WaitForLastTask();
			RTC_SetAlarm(tempCountValue);	//�����µ�����ֵ
			RTC_WaitForLastTask();
	





}
*/
/*
void Action3D_10Min_Save_Rang_Choose1(void)//ѡ�������10���ӱ���һ������
{
		u32 tempCountValue = 0;//��������ʱ��
			u32 uSUB_Buf0=0;//����
		u32 uSUB_Buf1=0;//����	
	
	
	
	
	
if(f_Rang_Run==1)//�п�������
	{
		if((calendar.hour*60+calendar.min)<=(uRang_Time[0]*60+uRang_Time[1]))//�����ʵʱ��С������ʱ�� 
		{
			uSUB_Buf0=((uRang_Time[0]*60+uRang_Time[1])-(calendar.hour*60+calendar.min))*60;//��ʵʱ�������ʱ���
			uSUB_Buf1=(10-calendar.min%10)*60;//�����´α������ݵ�ʱ���
			
			if((uSUB_Buf0>uSUB_Buf1)||(uSUB_Buf0==0))//�����(10�ֱ���һ������)��
			{
				tempCountValue=RTC_GetCounter();
				tempCountValue = tempCountValue + uSUB_Buf1;//N���Ӻ�������
				//				tempCountValue = tempCountValue + 20;//N���Ӻ�������
				RTC_WaitForLastTask();
				RTC_SetAlarm(tempCountValue);	//�����µ�����ֵ
				RTC_WaitForLastTask();
			}else
			{
				tempCountValue=RTC_GetCounter();
				tempCountValue = tempCountValue + uSUB_Buf0;//N���Ӻ�������
				//tempCountValue = tempCountValue + 20;//N���Ӻ�������
				RTC_WaitForLastTask();
				RTC_SetAlarm(tempCountValue);	//�����µ�����ֵ
				RTC_WaitForLastTask();
			}
		}else
		{
				uSUB_Buf1=(10-calendar.min%10)*60;
		tempCountValue=RTC_GetCounter();
		tempCountValue = tempCountValue + uSUB_Buf1;//N���Ӻ�������
			//				tempCountValue = tempCountValue + 20;//N���Ӻ�������
		RTC_WaitForLastTask();
		RTC_SetAlarm(tempCountValue);	//�����µ�����ֵ
		RTC_WaitForLastTask();
		
		
		}
	}else
	{
		uSUB_Buf1=(10-calendar.min%10)*60;
		tempCountValue=RTC_GetCounter();
		tempCountValue = tempCountValue + uSUB_Buf1;//N���Ӻ�������
		RTC_WaitForLastTask();
		RTC_SetAlarm(tempCountValue);	//�����µ�����ֵ
		RTC_WaitForLastTask();
	}
			 



}*/

void User_Rang_Set_Exit(void)//�˳��������ã�����
{


	
	
	
			uRang_Time[0]=uRang_Time_Buf[0];
		uRang_Time[1]=uRang_Time_Buf[1];


	f_Rang_Run=1;//��ʼ����
	

	
	
	 //User_Rang_Set_Fun();//���屣�棬��������������ʱ����������ʱ��
		 Action3D_10Min_Save_Rang_Choose();//ѡ�������10���ӱ���һ������
	
 //OLCD_Clear(0xf800);//����ɫ
		uMenu_No=8;//ʱ������

}
void User_Rang_Set_Exit_NotSave(void)//�˳��������ã�������
{

	
	//RTC_Set(2014,11,18,uClock_Hour_Set,uClock_Min_Set,0);

	uMenu_No=8;//ʱ������
	f_Rang_Run=0;//ֹͣ����
	u_Rang_Remind_No=0;//��λ
 //OLCD_Clear(0xf800);//����ɫ
	

}

void User_Rang_Set_Bit_Next(void)//��������,��һλ
{

		uRang_Set_Point++;
	if(uRang_Set_Point>1)	uRang_Set_Point=0;//
	
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
	
	
}

void User_Rang_Set_Bit_Before(void)//��������,��һλ
{
	uRang_Set_Point--;
	if(uRang_Set_Point>200)	uClock_Set_Point=1;//
	
		uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}


void User_Rang_Set_Bit_Add(void)//��������,λ��
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
		uSec_Blink_Suspended_Rang=10;//����
		
		uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
	
}

void Rang_Show_Set(void)//��������ʱ����ʾ����ʱ�䣬��˸
{
	//static u8 uSec_Before=255;//�룬���б仯��ˢ��
	static u8 uSec_Blink_No=0;//��������

	
	
	
	
	uSec_Blink_No++;
	if(uSec_Blink_No>=5) uSec_Blink_No=0;
	
	
	if(uSec_Blink_Suspended_Rang>0)
	{
	uSec_Blink_Suspended_Rang--;
		
	uSec_Blink_No=0;//���㣬��ͣ��˸
	
	
	}
	
	
	if(uSec_Blink_No==0)
	{


			OLED_ShowChar(22+5,32+20,(uRang_Time_Buf[0]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(37+5,32+20,(uRang_Time_Buf[0]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(52+5,32+20,':',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(67+5,32+20,(uRang_Time_Buf[1]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(82+5,32+20,(uRang_Time_Buf[1]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  

	}	


		if(uSec_Blink_No==3) 	
		{
			
			
			switch(uRang_Set_Point)
			{
				case	0:
			OLED_ShowChar(22+5,32+20,(uRang_Time_Buf[0]/10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(37+5,32+20,(uRang_Time_Buf[0]%10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	  
				break;
				case	1:
		OLED_ShowChar(67+5,32+20,(uRang_Time_Buf[1]/10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(82+5,32+20,(uRang_Time_Buf[1]%10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	
				break;			
				default:
					break;
			
			}
 
  
  
	  

			
			

		}
	
	
}








void User_Clock_Set_Into(void)//����ʱ������
{

uMenu_No=20;//ʱ������
	
	

	
		uTime_World_Buf[0]=calendar.w_year-2000;
		uTime_World_Buf[1]=calendar.w_month;
		uTime_World_Buf[2]=calendar.w_date;
	uTime_World_Buf[3]=calendar.hour;

uTime_World_Buf[4]=calendar.min;	
	
	

 OLCD_Clear(0x0000);//����ɫ
	
		OLED_ShowChar2020(2+12,28,(2+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(12+12,28,(0+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(22+12,28,(uTime_World_Buf[0]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(32+12,28,(uTime_World_Buf[0]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(42+12,28,'-',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(52+12,28,(uTime_World_Buf[1]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(62+12,28,(uTime_World_Buf[1]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(72+12,28,'-',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(82+12,28,(uTime_World_Buf[2]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(92+12,28,(uTime_World_Buf[2]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
	
	
			OLED_ShowChar(22+5,32+20,(uTime_World_Buf[3]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(37+5,32+20,(uTime_World_Buf[3]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(52+5,32+20,':',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(67+5,32+20,(uTime_World_Buf[4]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(82+5,32+20,(uTime_World_Buf[4]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
}

void User_Clock_Set_Exit(void)//�˳�ʱ�����ã�����
{

	
	RTC_Set(2000+uTime_World_Buf[0],uTime_World_Buf[1],uTime_World_Buf[2],uTime_World_Buf[3],uTime_World_Buf[4],0);

	uMenu_No=2;//ʱ������
	
		RTC_Get();//����ʱ��   
	
	//	 User_Rang_Set_Fun();//���屣�棬��������������ʱ����������ʱ��
		 Action3D_10Min_Save_Rang_Choose();//ѡ�������10���ӱ���һ������
	
 //OLCD_Clear(0xf800);//����ɫ
	

}
void User_Clock_Set_Exit_NotSave(void)//�˳�ʱ�����ã�������
{

	
	//RTC_Set(2014,11,18,uClock_Hour_Set,uClock_Min_Set,0);

	uMenu_No=2;//ʱ������
	
	
 //OLCD_Clear(0xf800);//����ɫ
	

}


void User_Clock_Set_Bit_Next(void)//ʱ������,��һλ
{

		uClock_Set_Point++;
	if(uClock_Set_Point>4)	uClock_Set_Point=0;//
	
	uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
	
	
}

void User_Clock_Set_Bit_Before(void)//ʱ������,��һλ
{
	uClock_Set_Point--;
	if(uClock_Set_Point>200)	uClock_Set_Point=4;//
	
		uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
}


void User_Clock_Set_Bit_Add(void)//ʱ������,λ��
{

		switch(uClock_Set_Point)
		{
		case 0:
	uTime_World_Buf[0]++;
	if(uTime_World_Buf[0]>99) 	uTime_World_Buf[0]=0;//��
	
					if(uTime_World_Buf[1]==2)//2��
	{
	
		if(((2000+uTime_World_Buf[0])%4==0&&(2000+uTime_World_Buf[0])%100!=0)||((2000+uTime_World_Buf[0])%400==0))//����
		{
			
			if(uTime_World_Buf[2]>29) uTime_World_Buf[2]=29;
			
		}
		else//ƽ��
		{
		
		if(uTime_World_Buf[2]>28) uTime_World_Buf[2]=28;
		}
	
	
	}
	
	
	break;
case 1:
		uTime_World_Buf[1]++;
		if(uTime_World_Buf[1]>12) uTime_World_Buf[1]=1;//��
			if((uTime_World_Buf[1]==4||uTime_World_Buf[1]==6||uTime_World_Buf[1]==9||uTime_World_Buf[1]==11)&&uTime_World_Buf[2]>30)//С��
uTime_World_Buf[2]=30;
				if(uTime_World_Buf[1]==2)//2��
	{
	
		if(((2000+uTime_World_Buf[0])%4==0&&(2000+uTime_World_Buf[0])%100!=0)||((2000+uTime_World_Buf[0])%400==0))//����
		{
			
			if(uTime_World_Buf[2]>29) uTime_World_Buf[2]=29;
			
		}
		else//ƽ��
		{
		
		if(uTime_World_Buf[2]>28) uTime_World_Buf[2]=28;
		}
	
	
	}
		

	break;	
case 2:
	uTime_World_Buf[2]++;//��
		if(uTime_World_Buf[1]==1||uTime_World_Buf[1]==3||uTime_World_Buf[1]==5||uTime_World_Buf[1]==7||uTime_World_Buf[1]==8||uTime_World_Buf[1]==10||uTime_World_Buf[1]==12)//����
	{
	if(uTime_World_Buf[2]>31) uTime_World_Buf[2]=1;
	
	}
			if(uTime_World_Buf[1]==4||uTime_World_Buf[1]==6||uTime_World_Buf[1]==9||uTime_World_Buf[1]==11)//С��
		{
		
		if(uTime_World_Buf[2]>30) uTime_World_Buf[2]=1;
		
		}
		if(uTime_World_Buf[1]==2)//2��
	{
	
		if(((2000+uTime_World_Buf[0])%4==0&&(2000+uTime_World_Buf[0])%100!=0)||((2000+uTime_World_Buf[0])%400==0))//����
		{
			
			if(uTime_World_Buf[2]>29) uTime_World_Buf[2]=1;
			
		}
		else//ƽ��
		{
		
		if(uTime_World_Buf[2]>28) uTime_World_Buf[2]=1;
		}
	
	
	}
	
	break;
case 3:
	uTime_World_Buf[3]++;//ʱ
	if(	uTime_World_Buf[3]>23) 	uTime_World_Buf[3]=0;

	break;
case 4:
	uTime_World_Buf[4]++;//��
	if(uTime_World_Buf[4]>59) 	uTime_World_Buf[4]=0;

	break;
default :
	break;
	}
		uSec_Blink_Suspended=10;//λ�ӣ�����
		uMenu_Not_Change_Time=0;//��ʱ˯�ߣ�����
	
}

void Clock_Show_Set(void)//����ʱ��ʱ����ʾ����ʱ�䣬��˸
{
	//static u8 uSec_Before=255;//�룬���б仯��ˢ��
	static u8 uSec_Blink_No=0;//��������

	
	
	
	
	uSec_Blink_No++;
	if(uSec_Blink_No>=5) uSec_Blink_No=0;
	
	
	if(uSec_Blink_Suspended>0)
	{
	uSec_Blink_Suspended--;
		
	uSec_Blink_No=0;//���㣬��ͣ��˸
	
	
	}
	
	
	if(uSec_Blink_No==0)
	{

		OLED_ShowChar2020(2+12,28,(2+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(12+12,28,(0+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(22+12,28,(uTime_World_Buf[0]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(32+12,28,(uTime_World_Buf[0]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(42+12,28,'-',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(52+12,28,(uTime_World_Buf[1]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(62+12,28,(uTime_World_Buf[1]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(72+12,28,'-',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(82+12,28,(uTime_World_Buf[2]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(92+12,28,(uTime_World_Buf[2]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
	
	
			OLED_ShowChar(22+5,32+20,(uTime_World_Buf[3]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(37+5,32+20,(uTime_World_Buf[3]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(52+5,32+20,':',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(67+5,32+20,(uTime_World_Buf[4]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(82+5,32+20,(uTime_World_Buf[4]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  

	}	


		if(uSec_Blink_No==3) 	
		{
			
			
			switch(uClock_Set_Point)
			{
				case	0:
							OLED_ShowChar2020(22+12,28,(uTime_World_Buf[0]/10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(32+12,28,(uTime_World_Buf[0]%10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	
				break;
				case	1:
							OLED_ShowChar2020(52+12,28,(uTime_World_Buf[1]/10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(62+12,28,(uTime_World_Buf[1]%10+0x30),0x0000,0x0000);//��ʾASCII�ַ�
				break;		
				case	2:
					
		OLED_ShowChar2020(82+12,28,(uTime_World_Buf[2]/10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(92+12,28,(uTime_World_Buf[2]%10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	 
				break;		
				case	3:
										OLED_ShowChar(22+5,52,(uTime_World_Buf[3]/10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	
		OLED_ShowChar(37+5,52,(uTime_World_Buf[3]%10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	
				break;		
				case	4:
								OLED_ShowChar(67+5,52,(uTime_World_Buf[4]/10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar(82+5,52,(uTime_World_Buf[4]%10+0x30),0x0000,0x0000);//��ʾASCII�ַ�	 
				break;
				default:
					break;
			
			}
 
  
  
	  

			
			

		}
	
	
}

void Clock_Show_Watch(void)//������ʾ
{
	
			//	OLCD_Clear(0x0000);
	
	/*static u8 uStep_Count_No_Bef=255;
		static u8 uRiss_Count_No_Bef=255;
	
	if(uStep_Count_No_Bef!=uStep_Count_No)
	{
	
	uStep_Count_No_Bef=uStep_Count_No;
		
		OLED_ShowChar2020(12+12,28,(uStep_Count_No/100+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(22+12,28,(uStep_Count_No%100/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(32+12,28,(uStep_Count_No%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	 
	
	}
	
	
	
		if(uRiss_Count_No_Bef!=uRiss)
	{
		uRiss_Count_No_Bef=uRiss;
		
				OLED_ShowChar2020(22+12,50,(uRiss/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(32+12,50,(uRiss%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	 
		
		
		
	}
	*/
	
	

	static u8 uSec_Before=255;//�룬���б仯��ˢ��
	static u8 uSec_Blink_No=0;//��������

	if(uSec_Before!=calendar.sec)
	{
	uSec_Before=calendar.sec;
		
			if(uMode_Old!=uMenu_No)
			{
				uMode_Old=uMenu_No;//��ֵ
			OLCD_Clear(0x0000);
			calendar_circle_clock_drawpanel(63,53,84,2);//��ʾָ��ʱ�ӱ��� 
			}
			calendar_circle_clock_showtime(64,53,80,3,calendar.hour%12,calendar.min,calendar.sec);
		
		}
	
	
	
	
}
void Day_Show_Watch(void)//������ʾ
{
	
	
		u8 data111=0;
	
	// AT24CXX_WriteOneByte(0,11);
	
	
// data111=AT24CXX_ReadOneByte(0);
	

			if(uMode_Old!=uMenu_No)
			{
				uMode_Old=uMenu_No;//��ֵ
				
			OLCD_Clear(0x0000);//��

						uTime_World_Buf[0]=calendar.w_year-2000;
		uTime_World_Buf[1]=calendar.w_month;
		uTime_World_Buf[2]=calendar.w_date;
	uTime_World_Buf[3]=calendar.hour;
	//	uTime_World_Buf[2]=		data111;
				
				

uTime_World_Buf[4]=calendar.min;	
				
		OLED_ShowChar2020(2+12,28,(2+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(12+12,28,(0+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(22+12,28,(uTime_World_Buf[0]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(32+12,28,(uTime_World_Buf[0]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(42+12,28,'-',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(52+12,28,(uTime_World_Buf[1]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(62+12,28,(uTime_World_Buf[1]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(72+12,28,'-',0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(82+12,28,(uTime_World_Buf[2]/10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		OLED_ShowChar2020(92+12,28,(uTime_World_Buf[2]%10+0x30),0xffff,0x0000);//��ʾASCII�ַ�	  
		
	 
		

		
		
	if((uTemp_Dat_Flag&0x80)==0x80) OLED_ShowChar2020(2+12,32+29,' ',0xffff,0x0000);//��ʾASCII�ַ�	  
	else OLED_ShowChar2020(2+12,32+29,'-',0xffff,0x0000);//��ʾASCII�ַ�	  
			
			OLED_ShowChar2020(2+12+7,32+29,uTemp_Dat/10+0x30,0xffff,0x0000);//��ʾASCII�ַ�	
			OLED_ShowChar2020(2+12+17,32+29,uTemp_Dat%10+0x30,0xffff,0x0000);//��ʾASCII�ַ�	
			
	if((uTemp_Dat_Flag&0x40)==0x40)
	{
		//��ʾ��
			OLED_ShowChar2020(2+12+27,32+29,'<',0xffff,0x0000);//��ʾASCII�ַ�	  
			OLED_ShowChar2020(2+12+37,32+29,'>',0xffff,0x0000);//��ʾASCII�ַ�	 
	}else
	{
		//�H
			OLED_ShowChar2020(2+12+27,32+29,'[',0xffff,0x0000);//��ʾASCII�ַ�	  
			OLED_ShowChar2020(2+12+8+27,32+29,'F',0xffff,0x0000);//��ʾASCII�ַ�		
	
	}



	//	u8 uTemp_Dat=0x16;//�¶�����
//u8 uTemp_Dat_Flag=0x00;//�¶�����,�¶���
		
		
		
		
		
			switch(calendar.week)
			{
				case 1:
					OLED_ShowChar(22+40,32+20,'M',0xffff,0x0000);//��ʾASCII�ַ�	  
					OLED_ShowChar(37+40,32+20,'o',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(52+40,32+20,'n',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(67+40,32+20,'n',0x0000,0x0000);//��ʾASCII�ַ�	 	
						break;
				case 2:
					OLED_ShowChar(22+40,32+20,'T',0xffff,0x0000);//��ʾASCII�ַ�	  
					OLED_ShowChar(37+40,32+20,'u',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(52+40,32+20,'e',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(67+40,32+20,'s',0xffff,0x0000);//��ʾASCII�ַ�	 			
						break;
				case 3:
					OLED_ShowChar(22+40,32+20,'W',0xffff,0x0000);//��ʾASCII�ַ�	  
					OLED_ShowChar(37+40,32+20,'e',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(52+40,32+20,'d',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(67+40,32+20,'d',0x0000,0x0000);//��ʾASCII�ַ�	 			
						break;
				case 4:
					OLED_ShowChar(22+40,32+20,'T',0xffff,0x0000);//��ʾASCII�ַ�	  
					OLED_ShowChar(37+40,32+20,'h',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(52+40,32+20,'u',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(67+40,32+20,'r',0xffff,0x0000);//��ʾASCII�ַ�	 			
						break;
				case 5:
					OLED_ShowChar(22+40,32+20,'F',0xffff,0x0000);//��ʾASCII�ַ�	  
					OLED_ShowChar(37+40,32+20,'r',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(52+40,32+20,'i',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(67+40,32+20,'i',0x0000,0x0000);//��ʾASCII�ַ�	 			
						break;
				case 6:
					OLED_ShowChar(22+40,32+20,'S',0xffff,0x0000);//��ʾASCII�ַ�	  
					OLED_ShowChar(37+40,32+20,'a',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(52+40,32+20,'t',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(67+40,32+20,'t',0x0000,0x0000);//��ʾASCII�ַ�	 			
						break;
				case 7:
					OLED_ShowChar(22+40,32+20,'S',0xffff,0x0000);//��ʾASCII�ַ�	  
					OLED_ShowChar(37+40,32+20,'u',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(52+40,32+20,'n',0xffff,0x0000);//��ʾASCII�ַ�	 
					OLED_ShowChar(67+40,32+20,'n',0x0000,0x0000);//��ʾASCII�ַ�	 			
						break;	
				default:
					
					break;

			}
			}

}
	
	
	
void Remind_Show(void)//������ʾ
{
//static u8 f_Call_B=0;//�����־
//static u8 f_Misscall_B=0;//δ�������־
//static u8 f_MSM_B=0;//���ű�־
	
	static u8 f_Misscall_Before=0;//δ�������־���棬���ܶ�̬����
	
		static u8 f_Call_Before=0;//�����־���棬���ܶ�̬����
	
	
				if(uMode_Old!=uMenu_No||f_Misscall_Before!=f_Misscall||f_Call_Before!=f_Call)
			{
				f_Misscall_Before=f_Misscall;//����ֵ
				f_Call_Before=f_Call;//����ֵ
				uMode_Old=uMenu_No;//��ֵ
			//	f_Call_B=f_Call;
			//	f_Misscall_B=f_Misscall;
			//	f_MSM_B=f_MSM;
				
			OLCD_Clear(0x0000);//��
				
				//u8 f_Call=0;//�����־
//u8 f_Misscall=0;//δ�������־
//u8 f_MSM=0;//���ű�־
			
		if(f_Call==1)		
			{
			
				OLED_DrawICO_3030(36, 23,gImage_2);//����
		}else
			{
			if(f_Misscall==1)	OLED_DrawICO_3030(36, 23,gImage_12);//δ������
			
			
			}
			if(f_MSM==1)			OLED_DrawICO_3030(36, 53,gImage_10);//����
				if(u_Rang_Remind_No!=0)		OLED_DrawICO_Small(66, 23,gImage_8);//����
			
			
			
	//	OLED_DrawICO_Small(36, 23,gImage_2);//����
	//	OLED_DrawICO_Small(66, 23,gImage_12);//δ������
	//	OLED_DrawICO_Small(36, 53,gImage_10);//����
	
			}
	
	
	
	
	
	
}
	
	
	
	
	

void User_Clock_Show_Task(void *pdata)//ʱ����ʾ����
{

	//static u8	f_Clock_to_Num=0;//�ӻ��ѣ�ʱ�ӡ��л���ʱ����ڽ��棬��־
//	static u8	f_Rang_to_Num=0;//�ӻ��ѣ�ʱ�ӡ��л���ʱ����ڽ��棬��־
	
	
	 
	
	
		      //OLCD_Clear(0x0000);
	//calendar_circle_clock_drawpanel(63,53,84,2);//��ʾָ��ʱ�ӱ��� 
	//	calendar_circle_clock_showtime(64,53,80,3,10,0,r);
	
	
	
		while(1)
	{

		
		
		// User_Rang_Fun();//����ɨ��
		
		 Sys_Time_Save();//ϵͳʱ�䱣��
		
		
	//	if(uMenu_No==1)f_Clock_to_Num=1;
		//else 	if(uMenu_No!=20)f_Clock_to_Num=0;
		
		if(uMenu_No!=102&&uMenu_No!=101&&uMenu_No!=1&&uMenu_No!=20)	uMode_Old=255;//��ֹ�л�����ˢ�±���
		
		switch(uMenu_No)
		{
		case 0:
		
			break;			
		case 1:
			Clock_Show_Watch();//������ʾ
			break;
		case 101:
			 Day_Show_Watch();//������ʾ		
			break;
		case 102:
			 Remind_Show();//������ʾ
			break;		
		case 50:
					 User_Step_Show();//��ʾ��������
			break;	
	case 60:
					 User_Distance_Show();//��ʾ�׾����
			break;			
		case 70:
					User_Search_Phone_Show();//��ʾ�����ֻ���̬����
			break;		
	 
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:			
			//uMode_Old=uMenu_No;//��ֵ
			//Clock_Show_Home();//��ҳ��ʾ����ʱ��	
			break;
		case 20:
			
		// if(f_Clock_to_Num==1)
		// {			
		//	 f_Clock_to_Num=0;
		//	 OLCD_Clear(0x0000);//����ɫ,��ֹ��������ʱ�ӻ����л�������
		// }
			uMode_Old=uMenu_No;//��ֵ
			Clock_Show_Set();//����ʱ��ʱ����ʾ����ʱ�䣬��˸
			break;
		case 80:
			
			//	 if(f_Clock_to_Num==1)
		// {			
			// f_Clock_to_Num=0;
			// OLCD_Clear(0x0000);//����ɫ,��ֹ��������ʱ�ӻ����л�������
		 //}
			uMode_Old=uMenu_No;//��ֵ
			Rang_Show_Set();//����ʱ��ʱ����ʾ����ʱ�䣬��˸
			
			break;
		default:
			break;

}
		

		
	
		
		
 
		
		
		
		//calendar.sec
		

		
		
		
		delay_ms(100);

	}
	





}
























