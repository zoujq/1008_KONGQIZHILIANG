#include "includes.h"
#include "stdlib.h"

void init_system_printf();
//#include "IMG.h"

//ALIENTEKս��STM32������ʵ��56
//ս��STM32�������ۺϲ���ʵ��
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			16 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				128
//�����ջ��8�ֽڶ���
__align(8) static OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);


/*
//��������
//�����������ȼ�
#define USART_TASK_PRIO       			7
//���������ջ��С
#define USART_STK_SIZE  		    	64
//�����ջ��8�ֽڶ���
__align(8) static OS_STK USART_TASK_STK[USART_STK_SIZE];
//������
void usart_task(void *pdata);
	*/

//������
//�����������ȼ�
#define MAIN_TASK_PRIO       			6
//���������ջ��С
#define MAIN_STK_SIZE  					64
//�����ջ��8�ֽڶ���
__align(8) static OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void main_task(void *pdata);

/*//���ڼ�������
//�����������ȼ�
#define WATCH_TASK_PRIO       			3
//���������ջ��С
#define WATCH_STK_SIZE  		   		64
//�����ջ��8�ֽڶ���
__align(8) static OS_STK WATCH_TASK_STK[WATCH_STK_SIZE];
//������
*/


/*
//MP3����
//�����������ȼ�
#define MUSIC_PLAY_TASK_PRIO       		2
//���������ջ��С
#define MUSIC_PLAY_STK_SIZE  		    256
//�����ջ��8�ֽڶ���
__align(8) static OS_STK MUSIC_PLAY_TASK_STK[MUSIC_PLAY_STK_SIZE];
//������
*/


//���ڼ�������
//�����������ȼ�
#define Uart_TASK_PRIO      			3
//���������ջ��С
#define Uart_STK_SIZE  				512
//�����ջ��8�ֽڶ���
__align(8) static OS_STK Uart_TASK_STK[Uart_STK_SIZE];
//������
void User_Uart_Control_Task(void *pdata);



//KEY ����
//�����������ȼ�
#define KEY_TASK_PRIO      			5
//���������ջ��С
#define KEY_STK_SIZE  				512
//�����ջ��8�ֽڶ���
__align(8) static OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//������
extern void User_Key_Scan_Task(void *pdata);

//ʱ�� ����
//�����������ȼ�
#define CLOCK_TASK_PRIO      			12
//���������ջ��С
#define CLOCK_STK_SIZE  				256
//�����ջ��8�ֽڶ���
__align(8) static OS_STK CLOCK_TASK_STK[CLOCK_STK_SIZE];
//������
extern void User_Clock_Show_Task(void *pdata);

//�˵� ����
//�����������ȼ�
#define Menu_TASK_PRIO      			13
//���������ջ��С
#define Menu_STK_SIZE  				256
//�����ջ��8�ֽڶ���
__align(8) static OS_STK Menu_TASK_STK[Menu_STK_SIZE];
//������
extern void User_Menu_Show_Task(void *pdata);



//������ ����
//�����������ȼ�
#define SENSOR_TASK_PRIO      			9
//���������ջ��С
#define SENSOR_STK_SIZE  				64
//�����ջ��8�ֽڶ���
__align(8) static OS_STK SENSOR_TASK_STK[SENSOR_STK_SIZE];
//������
extern void User_All_Sensor_Task(void *pdata);


//��س����� ����
//�����������ȼ�
#define POWER_TASK_PRIO      			11
//���������ջ��С
#define POWER_STK_SIZE  				512
//�����ջ��8�ֽڶ���
__align(8) static OS_STK POWER_TASK_STK[POWER_STK_SIZE];
//������
extern void User_GPRS_Task(void *pdata);


//�������� ����
//�����������ȼ�
#define OTHER_TASK_PRIO      			15
//���������ջ��С
#define OTHER_STK_SIZE  				128
//�����ջ��8�ֽڶ���
__align(8) static OS_STK OTHER_TASK_STK[OTHER_STK_SIZE];
//������
extern void User_OTHER_Control_Task(void *pdata);



//�Ʋ����� ����
//�����������ȼ�
#define ACTION3D_TASK_PRIO      			4
//���������ջ��С
#define ACTION3D_STK_SIZE  				256
//�����ջ��8�ֽڶ���
__align(8) static OS_STK ACTION3D_TASK_STK[ACTION3D_STK_SIZE];
//������
extern void User_Action3D_Control_Task(void *pdata);









//////////////////////////////////////////////////////////////////////////////

//ϵͳ��ʼ��
void system_init(void) {



	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_Configuration();

	delay_init(64);			//��ʱ��ʼ��

	//User_Other_Init();//��������IO�ڳ�ʼ��,�ȷ���ߡ�


#ifdef SEGGER
	SEGGER_RTT_printf(0, "UART init...\r\n");
#endif

	// uart1_init(9600); 	//����1��ʼ��

	init_system_printf();
	uart2_init(9600); 	//����2��ʼ��

	// uart3_init(9600); 	//����3��ʼ��
//	uart4_init(115200); 	//����4��ʼ��


	UID_READ();

	Sys_RTC_Init();//ʵʱʱ�ӳ�ʼ��


	User_Sys_Data_Get();




	//��ʼ��IIC�ӿ�
//AT24CXX_Init();

//		SPI_Flash_Init();  		//SPI FLASH ��ʼ��



// 	KEY_Init();				//������ʼ��
//Adc_Init();			  	//ADC��ʼ��,�ڲ��¶ȴ�����
//	BEEP_Init();

//////////////////////////////////////////////////////////////////////////
//��ʼӲ������ʼ��


// SSD1351_Init();

	//Initial_ssd1351();

//OLED_Init();//OLED��ʼ��

//User_BLE_Init();//����,��ʼ��




//	RTC_Init();	  			//RTC��ʼ��


//User_Clock_Init();//RTC��ʼ��


	//�������̽�,��ʾ��������
	//BEEP=1;

	//���Ը�λ

	/*	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//ʹ��PORTA,B,C,D,E,G�Լ�AFIO���ù���ʱ��


	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PA11     ��ѹ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);

	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD12     ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_12);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PC7 ������� ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
		delay_ms(1000);
		*/
	//delay_ms(100);
	//BEEP=0;


}
void main_task(void *pdata);
//main����
int main(void)

{

	/* Flash������ʹ�� */
	/*
	if (FLASH_GetReadOutProtectionStatus() == RESET)
	{
	FLASH_Unlock();
	FLASH_ReadOutProtection(ENABLE);
	}*/




	SystemInit();
	system_init();		//ϵͳ��ʼ��
	printf("System start V2.0.1 \n\r");
	OSInit();
	OSTaskCreate(start_task, (void *)0, (OS_STK *)&START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO ); //������ʼ����
	OSStart();
}
//��ʼ����
void start_task(void *pdata) {
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OSStatInit();		//��ʼ��ͳ������.�������ʱ1��������
// 	app_srand(OSTime);
	



	OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��)
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
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();	//�˳��ٽ���(���Ա��жϴ��)
	
}




//������
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











