#include "includes.h"
#include "stdlib.h"

void init_system_printf();
//#include "IMG.h"

//ALIENTEK战舰STM32开发板实验56
//战舰STM32开发板综合测试实验
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			16 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				128
//任务堆栈，8字节对齐
__align(8) static OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);


/*
//串口任务
//设置任务优先级
#define USART_TASK_PRIO       			7
//设置任务堆栈大小
#define USART_STK_SIZE  		    	64
//任务堆栈，8字节对齐
__align(8) static OS_STK USART_TASK_STK[USART_STK_SIZE];
//任务函数
void usart_task(void *pdata);
	*/

//主任务
//设置任务优先级
#define MAIN_TASK_PRIO       			6
//设置任务堆栈大小
#define MAIN_STK_SIZE  					64
//任务堆栈，8字节对齐
__align(8) static OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//任务函数
void main_task(void *pdata);

/*//串口监视任务
//设置任务优先级
#define WATCH_TASK_PRIO       			3
//设置任务堆栈大小
#define WATCH_STK_SIZE  		   		64
//任务堆栈，8字节对齐
__align(8) static OS_STK WATCH_TASK_STK[WATCH_STK_SIZE];
//任务函数
*/


/*
//MP3任务
//设置任务优先级
#define MUSIC_PLAY_TASK_PRIO       		2
//设置任务堆栈大小
#define MUSIC_PLAY_STK_SIZE  		    256
//任务堆栈，8字节对齐
__align(8) static OS_STK MUSIC_PLAY_TASK_STK[MUSIC_PLAY_STK_SIZE];
//任务函数
*/


//串口监视任务
//设置任务优先级
#define Uart_TASK_PRIO      			3
//设置任务堆栈大小
#define Uart_STK_SIZE  				512
//任务堆栈，8字节对齐
__align(8) static OS_STK Uart_TASK_STK[Uart_STK_SIZE];
//任务函数
void User_Uart_Control_Task(void *pdata);



//KEY 任务
//设置任务优先级
#define KEY_TASK_PRIO      			5
//设置任务堆栈大小
#define KEY_STK_SIZE  				512
//任务堆栈，8字节对齐
__align(8) static OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务函数
extern void User_Key_Scan_Task(void *pdata);

//时钟 任务
//设置任务优先级
#define CLOCK_TASK_PRIO      			12
//设置任务堆栈大小
#define CLOCK_STK_SIZE  				256
//任务堆栈，8字节对齐
__align(8) static OS_STK CLOCK_TASK_STK[CLOCK_STK_SIZE];
//任务函数
extern void User_Clock_Show_Task(void *pdata);

//菜单 任务
//设置任务优先级
#define Menu_TASK_PRIO      			13
//设置任务堆栈大小
#define Menu_STK_SIZE  				256
//任务堆栈，8字节对齐
__align(8) static OS_STK Menu_TASK_STK[Menu_STK_SIZE];
//任务函数
extern void User_Menu_Show_Task(void *pdata);



//传感器 任务
//设置任务优先级
#define SENSOR_TASK_PRIO      			9
//设置任务堆栈大小
#define SENSOR_STK_SIZE  				64
//任务堆栈，8字节对齐
__align(8) static OS_STK SENSOR_TASK_STK[SENSOR_STK_SIZE];
//任务函数
extern void User_All_Sensor_Task(void *pdata);


//电池充电管理 任务
//设置任务优先级
#define POWER_TASK_PRIO      			11
//设置任务堆栈大小
#define POWER_STK_SIZE  				512
//任务堆栈，8字节对齐
__align(8) static OS_STK POWER_TASK_STK[POWER_STK_SIZE];
//任务函数
extern void User_GPRS_Task(void *pdata);


//其他管理 任务
//设置任务优先级
#define OTHER_TASK_PRIO      			15
//设置任务堆栈大小
#define OTHER_STK_SIZE  				128
//任务堆栈，8字节对齐
__align(8) static OS_STK OTHER_TASK_STK[OTHER_STK_SIZE];
//任务函数
extern void User_OTHER_Control_Task(void *pdata);



//计步管理 任务
//设置任务优先级
#define ACTION3D_TASK_PRIO      			4
//设置任务堆栈大小
#define ACTION3D_STK_SIZE  				256
//任务堆栈，8字节对齐
__align(8) static OS_STK ACTION3D_TASK_STK[ACTION3D_STK_SIZE];
//任务函数
extern void User_Action3D_Control_Task(void *pdata);









//////////////////////////////////////////////////////////////////////////////

//系统初始化
void system_init(void) {



	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_Configuration();

	delay_init(64);			//延时初始化

	//User_Other_Init();//其他控制IO口初始化,先放这边。


#ifdef SEGGER
	SEGGER_RTT_printf(0, "UART init...\r\n");
#endif

	// uart1_init(9600); 	//串口1初始化

	init_system_printf();
	uart2_init(9600); 	//串口2初始化

	// uart3_init(9600); 	//串口3初始化
//	uart4_init(115200); 	//串口4初始化


	UID_READ();

	Sys_RTC_Init();//实时时钟初始化


	User_Sys_Data_Get();




	//初始化IIC接口
//AT24CXX_Init();

//		SPI_Flash_Init();  		//SPI FLASH 初始化



// 	KEY_Init();				//按键初始化
//Adc_Init();			  	//ADC初始化,内部温度传感器
//	BEEP_Init();

//////////////////////////////////////////////////////////////////////////
//开始硬件检测初始化


// SSD1351_Init();

	//Initial_ssd1351();

//OLED_Init();//OLED初始化

//User_BLE_Init();//蓝牙,初始化




//	RTC_Init();	  			//RTC初始化


//User_Clock_Init();//RTC初始化


	//蜂鸣器短叫,提示正常启动
	//BEEP=1;

	//测试复位

	/*	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//使能PORTA,B,C,D,E,G以及AFIO复用功能时钟


	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PA11     升压
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);

	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD12     供电
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_12);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PC7 推挽输出 背光
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
		delay_ms(1000);
		*/
	//delay_ms(100);
	//BEEP=0;


}
void main_task(void *pdata);
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




	SystemInit();
	system_init();		//系统初始化
	printf("System start V2.0.1 \n\r");
	OSInit();
	OSTaskCreate(start_task, (void *)0, (OS_STK *)&START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO ); //创建起始任务
	OSStart();
}
//开始任务
void start_task(void *pdata) {
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OSStatInit();		//初始化统计任务.这里会延时1秒钟左右
// 	app_srand(OSTime);
	



	OS_ENTER_CRITICAL();//进入临界区(无法被中断打断)
	OSTaskCreate(main_task, (void *)0, (OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE - 1], MAIN_TASK_PRIO);

	OSTaskCreate(User_Key_Scan_Task, (void *)0, (OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE - 1], KEY_TASK_PRIO);
//	OSTaskCreate(User_Uart_Control_Task,(void *)0,(OS_STK*)&Uart_TASK_STK[Uart_STK_SIZE-1],Uart_TASK_PRIO);
//	OSTaskCreate(User_Modbus_Task,(void *)0,(OS_STK*)&CLOCK_TASK_STK[CLOCK_STK_SIZE-1],CLOCK_TASK_PRIO);
	OSTaskCreate(User_Menu_Show_Task, (void *)0, (OS_STK*)&Menu_TASK_STK[Menu_STK_SIZE - 1], Menu_TASK_PRIO);
	OSTaskCreate(User_All_Sensor_Task, (void *)0, (OS_STK*)&SENSOR_TASK_STK[SENSOR_STK_SIZE - 1], SENSOR_TASK_PRIO);
	OSTaskCreate(User_NBIOT_Task, (void *)0, (OS_STK*)&POWER_TASK_STK[POWER_STK_SIZE - 1], POWER_TASK_PRIO);
//	OSTaskCreate(User_SI4432_Task, (void *)0, (OS_STK*)&OTHER_TASK_STK[OTHER_STK_SIZE - 1], OTHER_TASK_PRIO);
	OSTaskCreate(user_watch_task, (void *)0, (OS_STK*)&ACTION3D_TASK_STK[ACTION3D_STK_SIZE - 1], CLOCK_TASK_PRIO);


	printf("start_task \n\r");
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();	//退出临界区(可以被中断打断)
	
}




//主任务
void main_task(void *pdata) {


	while (1) {
		printf("Heart beat \n\r");
		delay_ms(5000);
	}
}



/**************************************Z-1-start powered by zoujq 904087449@qq.com Implementation of system printf function***********************************/
// us UART5 as printf Tx
void init_system_printf() {
	uart5_init(115200);//
	USART_ClearFlag(UART5, USART_FLAG_TC);
}
struct __FILE {
	int handle;
} __stdout;

int fputc(int ch, FILE *f) {

	USART_SendData(UART5, ch);
	while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
	USART_ClearFlag(UART5, USART_FLAG_TC);
	return ch;
}
/**************************************Z-1-end powered by zoujq 904087449@qq.com Implementation of system printf function***********************************/











