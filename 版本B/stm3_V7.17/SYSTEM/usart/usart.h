#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_UART4_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_UART5_RX 			1		//使能（1）/禁止（0）串口1接收

#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收	 
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收	 
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收	 
//extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义


#define	UART_RX_BUF_MAX	200
#define	UART_TX_BUF_MAX	400



extern 	u8  uUart4_Recive_Data[50];//接收缓存
extern  u8 	uUart4_Recive_Length;//接收长度
extern u8 	uUart4_Recive_End;//接收完成
//extern u8  	uUart4_Send_Data[400];//发送缓存
extern u8 	uUart4_Send_Delay;//延时发送


extern 	u8 volatile 	uUart5_Recive_Data[50];//接收缓存
extern  u8 	uUart5_Recive_Length;//接收长度
extern u8 	uUart5_Recive_End;//接收完成
//extern u8 volatile 	uUart5_Send_Data[250];//发送缓存
extern u8 	uUart5_Send_Delay;//延时发送

extern 	u8 volatile 	uUart1_Recive_Data[50];//接收缓存
extern  u8 	uUart1_Recive_Length;//接收长度
extern u8 	uUart1_Recive_End;//接收完成
//extern u8 volatile 	uUart1_Send_Data[100];//发送缓存
extern u8 	uUart1_Send_Delay;//延时发送


extern  u8 fUart_Send_End;//发送完成 标志，数据分段发送
extern 	char 	uUart2_Recive_Data[UART_RX_BUF_MAX];//接收缓存
extern  u8 	uUart2_Recive_Length;//接收长度
extern u8 	uUart2_Recive_End;//接收完成
extern char 	uUart2_Send_Data[UART_TX_BUF_MAX];//发送缓存
extern u8 	uUart2_Send_Length;//发送长度
extern u8 	uUart2_Send_Length_No;//发送长度,计数



extern 	u8 volatile 	uUart3_Recive_Data[50];//接收缓存
extern  u8 	uUart3_Recive_Length;//接收长度
extern u8 	uUart3_Recive_End;//接收完成
extern u8 volatile 	uUart3_Send_Data[20];//发送缓存
extern u8 	uUart3_Send_Delay;//延时发送


void uart4_init(u32 bound);
void uart5_init(u32 bound);
void uart5_uninit(void);
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart2_uninit(void);
void uart3_init(u32 bound);
u8 User_UART4_SendStringx(char *pString,u16 Length);
//u8 User_UART2_SendStringx(char *pString,u16 Length);
void User_UART_Send(void);
u8 User_UART5_SendStringx(char *pString,u8 Length);
u8 User_UART3_SendStringx(char *pString,u8 Length);	

typedef struct
{
	u16	time_out;	
	u16	rxbufmax;	
	u16	txbufmax;		
	char *rxbuf;u16 rxlen;
	char *txbuf;u16 txlen;
	void (*Init)(u8 *prx,u16 rxlen,u8 *ptx,u16 txlen);
	u8 (*Connect)(char *ClientID,char *Username,char *Password);
	u8 (*SubscribeTopic)(char *topic,u8 qos,u8 whether);
	u8 (*PublishData)(char *topic, char *message, u8 qos);
	void (*SendHeart)(void);
	void (*Disconnect)(void);
}_typdef_mqtt;

extern _typdef_mqtt _mqtt;


typedef struct
{
	u16	time_out;
	u16	rxbufmax;	
	u16	txbufmax;		
	char *rxbuf;u16 rxlen;
	char *txbuf;u16 txlen;
	
	u8 (*Check)(void);
	u8 (*Init)(u8 *prx,u16 rxlen,u8 *ptx,u16 txlen);
	void (*Restore)(void);
	u8 (*ConnectAP)(char *ssid,char *pswd);
	u8 (*ConnectServer)(char* mode,char *ip,u16 port);
	u8 (*DisconnectServer)(void);
	u8 (*OpenTransmission)(void);
	void (*CloseTransmission)(void);		
	void (*SendString)(char *str);
	void (*SendBuf)(u8 *buf,u16 len);
}_typdef_net;

extern _typdef_net _net;





#endif	   
















