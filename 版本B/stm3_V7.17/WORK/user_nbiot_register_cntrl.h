#ifndef __USER_NBIOT_REGISTER_CNTRL_H__
#define __USER_NBIOT_REGISTER_CNTRL_H__




#define	SYS_TYPE 0x010D		//连网方式+型号
#define	SYS_EDITION 0x01	//版本号
//例01：硬件版本1，软件版本1
//
//

#define	SYS_TYPE_NO1 0x000506  //3
//////////////////////////////////////////////////////////////
extern	uint64_t SYS_TYPE_NO;



#define SYS_TYPE_NO_ADDR (0x807FED0)//807ffe0

#define CON_ASK_MAX	1000	//10秒

#define ASK_RES_NEXT_DELAY 20 //延时发送下一个




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


extern  u8  uCon_No;//控制号。每执行一种动作，加1
//extern xdata u8  uCon_No_B;//限制或重复执行动作标志
//extern xdata u16  uCon_Time_Delay;//每种动作之间延迟时间
//extern xdata u16  uCon_Ask_Time;//应答时间定时，到时间没回复，再次发送

extern  char * str_p;//字符串指针//
extern u8 str_p_i;//数据查找计数
//extern  char * str_p_nest;//字符串指针//


extern  u16 crc_check;
extern  u16 crc_check_res;






extern const char	st1[];//开启射频全功能
extern const char	st2[];//下行数据通知 
extern const char	st3[];//自动搜网 
extern const char	st4[];//关闭低功耗功能
extern const char	st5[];//查询分配的ip 地址，可以以此判断是否连接到网络
extern const char	st6[];//查询UE 状态，包括rsrp,sinr,覆盖等级，小区信息等，可选 
 
extern const char  st7[];//查询IMEI 号
extern const char  st8[];//查询SIM卡信息

extern const char	st29[];//查询NB信号强度 




extern const char    st15[];//1 为socket 号，如果有回复OK说明创建成功,UDP


extern const char    st160[];//阿里的DNS
extern const char    st161[];//谷歌的DNS
extern const char    st162[];//114的DNS，流氓


extern  char uDNS_No;//DNS序号
extern  u16 uDNS_Res_Time;//DNS等待应答时间
extern  u8 uDNS_Res_Time_No;//DNS等待应答时间//次数


extern  char    st17[20];	//{"AT+NSORF=1,256\r\n\0"};//




struct SYSCNTRL_typedef
{
	
	
	u8 uSys_User_Reset;//系统用户重置
	u16 uSys_User_Reset_Time;//系统用户重置,计时		
	u16 uSys_User_Reset_Time_LED_Blink_No;//系统用户重置,LED闪烁显示
	
	
 u8 	uLine_Work;//网络状态	
 u8 	uLine_Work_No_Blink_No;//网络状态//闪烁计数		
	
	

//  u8  uStart_Send_Data_5;//开机启动，未连接状态，每10秒发送一次数据，连续发5次

 u8  uData_Send_No_Add;//数据发送次数计数，返回清零，没返回超过重启模块	
	
	u8 fSys_User_Other_Data;//其他信息上传			
	


 u16  uData_Send_Time;//数据发送定时

 //u8  fData_Send_Services_Ask;//返回数据给服务器标志 
 u8  fData_Send_Services_Ask_B;//返回数据给服务器标志 
 u8  fData_Send_Services_Ask_B1;//返回数据给服务器标志 
 
 
 u32 		uTime_Heard_No;//心跳计数
 
 u16 		uTime_Heard_Send_No;//发送数据心跳计数 
 
 
 
 u8		uSend_Data_Type_No;//发送数据类型号，1：定位数据（只有重启时才发送一次）

 u8		uPlatform_No;//平台号,0:注册平台   1：自己平台   2：OneNET平台
 u8		uDNS_IP_Get_No;//获取IP号,0:注册平台IP   1：自己平台IP   2：OneNET平台IP
 u8 uDNS_IP_GET[4];//获取的服务器IP
 char uSocket_No;//连接的socket号 
	

//union Var_typedef	Data_Buf;	//利用联合体及结构体，减少内存占用
u8  	Data_Buf_i;//数据缓存	
u8  	Data_Buf_strlen;//数据缓存		

	signed long	NB_RSSI;	//NB信号强度
u8 fNB_Open;//打开平台标志	
		



	u8  	uUart_Data_De_Coding[110];//数据编解码
//NB位置信息			
	signed long	Signal_power;
	signed long	Total_power;
	signed long	TX_power;
	signed long	TX_time;
	signed long	RX_time;
	signed long	Cell_ID;
	signed long	ECL;
	signed long	SNR;
	signed long	EARFCN;
	signed long	PCI;
	signed long	RSRQ;
	signed long	OPERATOR_MODE;
	signed long	CURRENT_BAND;	


//NB模块信息	
	char CGSN[15];//IMEI 号	
	char CIMI[15];//SIM卡信息	
	
	u8 uRes_Flag;//重启标志位
	
 u32 uRegister_Use_Time;//注册用时


	
};
extern  struct  SYSCNTRL_typedef Sys_Cntrl;
extern  struct  SYSCNTRL_typedef Sys_Cntrl_B;




u8 CTOH(char *Pdata);
u8 HTOC(u8	Pdata);//

void uart_sendstring(char const *str);








#endif
