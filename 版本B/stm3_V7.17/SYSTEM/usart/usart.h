#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_UART4_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_UART5_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����	 
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����	 
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����	 
//extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��


#define	UART_RX_BUF_MAX	200
#define	UART_TX_BUF_MAX	400



extern 	u8  uUart4_Recive_Data[50];//���ջ���
extern  u8 	uUart4_Recive_Length;//���ճ���
extern u8 	uUart4_Recive_End;//�������
//extern u8  	uUart4_Send_Data[400];//���ͻ���
extern u8 	uUart4_Send_Delay;//��ʱ����


extern 	u8 volatile 	uUart5_Recive_Data[50];//���ջ���
extern  u8 	uUart5_Recive_Length;//���ճ���
extern u8 	uUart5_Recive_End;//�������
//extern u8 volatile 	uUart5_Send_Data[250];//���ͻ���
extern u8 	uUart5_Send_Delay;//��ʱ����

extern 	u8 volatile 	uUart1_Recive_Data[50];//���ջ���
extern  u8 	uUart1_Recive_Length;//���ճ���
extern u8 	uUart1_Recive_End;//�������
//extern u8 volatile 	uUart1_Send_Data[100];//���ͻ���
extern u8 	uUart1_Send_Delay;//��ʱ����


extern  u8 fUart_Send_End;//������� ��־�����ݷֶη���
extern 	char 	uUart2_Recive_Data[UART_RX_BUF_MAX];//���ջ���
extern  u8 	uUart2_Recive_Length;//���ճ���
extern u8 	uUart2_Recive_End;//�������
extern char 	uUart2_Send_Data[UART_TX_BUF_MAX];//���ͻ���
extern u8 	uUart2_Send_Length;//���ͳ���
extern u8 	uUart2_Send_Length_No;//���ͳ���,����



extern 	u8 volatile 	uUart3_Recive_Data[50];//���ջ���
extern  u8 	uUart3_Recive_Length;//���ճ���
extern u8 	uUart3_Recive_End;//�������
extern u8 volatile 	uUart3_Send_Data[20];//���ͻ���
extern u8 	uUart3_Send_Delay;//��ʱ����


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
















