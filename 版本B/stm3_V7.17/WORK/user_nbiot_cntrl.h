#ifndef __USER_NBIOT_CNTRL_H__
#define __USER_NBIOT_CNTRL_H__




#define	SYS_TYPE 0x030D		//连网方式+型号
#define	SYS_EDITION 0x01	//版本号
//例01：硬件版本1，软件版本1
//
//

//#define	SYS_TYPE_NO 0x000001  //3
extern	uint64_t SYS_TYPE_NO;

#define SYS_TYPE_NO_ADDR (0x807FED0)//807ffe0





#define CON_NO_MAX	60


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
#define CON_NO_ST17	CON_NO_MAX-23
#define CON_NO_ST18	CON_NO_MAX-24
#define CON_NO_ST19	CON_NO_MAX-25
#define CON_NO_ST20	CON_NO_MAX-26
#define CON_NO_ST21	CON_NO_MAX-27
#define CON_NO_ST22	CON_NO_MAX-28
#define CON_NO_ST23	CON_NO_MAX-29
#define CON_NO_ST24	CON_NO_MAX-30
#define CON_NO_ST25	CON_NO_MAX-31
#define CON_NO_ST26	CON_NO_MAX-32
#define CON_NO_ST27	CON_NO_MAX-33
#define CON_NO_ST28	CON_NO_MAX-34
#define CON_NO_ST29	CON_NO_MAX-35
#define CON_NO_ST30	CON_NO_MAX-36
#define CON_NO_ST31	CON_NO_MAX-37
#define CON_NO_ST32	CON_NO_MAX-38
#define CON_NO_ST33	CON_NO_MAX-39
#define CON_NO_ST34	CON_NO_MAX-40
#define CON_NO_ST35	CON_NO_MAX-41
#define CON_NO_ST36	CON_NO_MAX-42
#define CON_NO_ST37	CON_NO_MAX-43
#define CON_NO_ST38	CON_NO_MAX-44
#define CON_NO_ST39	CON_NO_MAX-45
#define CON_NO_ST40	CON_NO_MAX-46
#define CON_NO_ST41	CON_NO_MAX-47
#define CON_NO_ST42	CON_NO_MAX-48
#define CON_NO_ST43	CON_NO_MAX-49
#define CON_NO_ST44	CON_NO_MAX-50
#define CON_NO_ST45	CON_NO_MAX-51
#define CON_NO_ST46	CON_NO_MAX-52
#define CON_NO_ST47	CON_NO_MAX-53
#define CON_NO_ST48	CON_NO_MAX-54
#define CON_NO_ST49	CON_NO_MAX-55






void Uart2_Decode(void);//串口处理函数 


void User_NBIOT_Task(void *pdata);//NBIOT任务


u8 HTOC(u8	Pdata);//



#endif
