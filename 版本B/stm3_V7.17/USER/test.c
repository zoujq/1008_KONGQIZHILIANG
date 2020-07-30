#include "includes.h"
#include "stdlib.h"




//////////////////////////////////////////////////////////////////////////////

//系统初始化
void system_init(void) {

	NVIC_Configuration();
	delay_init(64);			//延时初始化

#ifdef SEGGER
	SEGGER_RTT_printf(0, "UART init...\r\n");
#endif

	f_Sys_Check = 2; //赋值
	//UID_READ();
	Sys_RTC_Init();//实时时钟初始化
	User_Sys_Data_Get();
	delay_ms(100);
	
}




extern char User_Key_Scan_Thread(thread_t* pt);
extern char User_Menu_Show_Thread(thread_t* pt);//菜单图标显示任务
extern char User_All_Sensor_Thread(thread_t* pt);//所有传感器检测任务
extern char User_NBIOT_Thread(thread_t* pt);
extern char User_WIFI_Thread(thread_t* pt);
extern char User_SI4432_Thread(thread_t* pt);
extern char user_watch_Thread(thread_t* pt);

extern char Charge_Off_Show_Thread(thread_t* pt);//关机充电界面显示
extern char All_Show_Thread(thread_t* pt);//所有参数界面显示
extern char PM25_Show_Thread(thread_t* pt);//PM2.5界面显示
extern char TVOC_Show_Thread(thread_t* pt);//TVOC界面显示
extern char HCHO_Show_Thread(thread_t* pt);//甲醛界面显示
extern char CO2_Show_Thread(thread_t* pt);//TVOC界面显示
extern char Time_Show_Thread(thread_t* pt);//时间界面显示


extern char Uart_10ms_Thread(thread_t* pt);//串口10ms任务
extern char User_NBIOT_Register_Thread(thread_t* pt);//注册任务
extern char User_NBIOT_OneNET_Thread(thread_t* pt);//OneNET任务
extern char User_NBIOT_OneNET_Scan_Thread(thread_t* pt);//OneNET任务,常监测
extern char User_NBIOT_OneSELF_Thread(thread_t* pt);//自己平台任务
extern char User_NBIOT_OneSELF_Scan_Thread(thread_t* pt);//自己平台,常监测
extern char User_NBIOT_Register_Scan_Thread(thread_t* pt);//注册时,常监测
extern char User_Print_SYS_Status_Thread(thread_t* pt);//心跳

void init_system_printf();
//main函数
int main(void)

{

	/* Flash读保护使能 */
	/*
	if (FLASH_GetReadOutProtectionStatus() == RESET)
	{
	FLASH_Unlock();
	FLASH_ReadOutProtection(ENABLE);
	}*/

	Sys_Cntrl.uRes_Flag = 0;
	if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET) {
		//这是软件复位
		Sys_Cntrl.uRes_Flag = 1;
	}

	RCC_ClearFlag();//清除RCC中复位标志

	SystemInit();
	system_init();		//系统初始化

	SysTickInit();
	TaskInit();
	
	init_system_printf();
	printf("zoujq-V2.0.2\n\r");

    Thread_Start(User_Key_Scan_Thread, &threadInfo.status);
    Thread_Start(User_Menu_Show_Thread, &threadInfo.status);
    Thread_Start(User_All_Sensor_Thread, &threadInfo.status);
//    Thread_Start(User_WIFI_Thread, &threadInfo.status);
    Thread_Start(User_SI4432_Thread, &threadInfo.status);



    Thread_Start(Uart_10ms_Thread, &threadInfo.status);



//    Thread_Start(User_NBIOT_Register_Thread, &threadInfo.status);
//    Thread_Start(User_NBIOT_Register_Scan_Thread, &threadInfo.status);


//    Thread_Start(User_NBIOT_OneNET_Thread, &threadInfo.status);
//    Thread_Start(User_NBIOT_OneNET_Scan_Thread, &threadInfo.status);


//    Thread_Start(User_NBIOT_OneSELF_Thread, &threadInfo.status);
//    Thread_Start(User_NBIOT_OneSELF_Scan_Thread, &threadInfo.status);



    Thread_Start(Charge_Off_Show_Thread, &threadInfo.status);
    Thread_Start(All_Show_Thread, &threadInfo.status);
    Thread_Start(PM25_Show_Thread, &threadInfo.status);
		Thread_Start(TVOC_Show_Thread, &threadInfo.status);
    Thread_Start(HCHO_Show_Thread, &threadInfo.status);
    Thread_Start(CO2_Show_Thread, &threadInfo.status);
    Thread_Start(Time_Show_Thread, &threadInfo.status);
		
		Thread_Start(User_Print_SYS_Status_Thread, &threadInfo.status);//
//    Thread_Start(user_watch_Thread, &threadInfo.status);//放最后，有进入低功耗模式

		
    //run
    TaskRun();

	
	while (1) {

		printf("zoujq 904087449@qq.com V2.0.1\n\r");
		delay_ms(1000);
	}

}
/**************************************Z-1-start powered by zoujq 904087449@qq.com Implementation of system printf function***********************************/
// us UART5 as printf Tx 
void init_system_printf()
{
	uart5_init(115200);//
	USART_ClearFlag(UART5,USART_FLAG_TC);
}
struct __FILE
{
 int handle;
} __stdout;

int fputc(int ch, FILE *f)                     
{             
				
	USART_SendData(UART5,ch);
	while(USART_GetFlagStatus(UART5, USART_FLAG_TC)==RESET);
	USART_ClearFlag(UART5,USART_FLAG_TC);
  return ch;
}
/**************************************Z-1-end powered by zoujq 904087449@qq.com Implementation of system printf function***********************************/








