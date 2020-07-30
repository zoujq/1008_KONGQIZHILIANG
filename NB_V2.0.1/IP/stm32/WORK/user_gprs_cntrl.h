#ifndef __USER_GPRS_CNTRL_H__
#define __USER_GPRS_CNTRL_H__

#define	SYS_TYPE 0xF005
#define	SYS_TYPE_NO 0x10  //3




#define CON_NO_MAX	50


#define CON_NO_START0	CON_NO_MAX
#define CON_NO_START1	CON_NO_MAX-1
#define CON_NO_START2	CON_NO_MAX-2
#define CON_NO_START3	CON_NO_MAX-3
#define CON_NO_START4	CON_NO_MAX-4
#define CON_NO_START5	CON_NO_MAX-5
#define CON_NO_ST0	CON_NO_MAX-6
#define CON_NO_ST1	CON_NO_MAX-7
#define CON_NO_ST2	CON_NO_MAX-8
#define CON_NO_ST3	CON_NO_MAX-9
#define CON_NO_ST4	CON_NO_MAX-10
#define CON_NO_ST5	CON_NO_MAX-11
#define CON_NO_ST6	CON_NO_MAX-12
#define CON_NO_ST7	CON_NO_MAX-13
#define CON_NO_ST8	CON_NO_MAX-14
#define CON_NO_ST9	CON_NO_MAX-15
#define CON_NO_ST10	CON_NO_MAX-16
#define CON_NO_ST11	CON_NO_MAX-17
#define CON_NO_ST12	CON_NO_MAX-18
#define CON_NO_ST13	CON_NO_MAX-19
#define CON_NO_ST14	CON_NO_MAX-20
#define CON_NO_ST15	CON_NO_MAX-21
#define CON_NO_ST16	CON_NO_MAX-22
//#define CON_NO_ST17	CON_NO_MAX-23
//#define CON_NO_ST18	CON_NO_MAX-24



void User_Uart_Handle(void);//串口处理函数 
void User_Gprs_Task(void);//通讯定位控制任务






#endif
