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






extern 	u8  	uUart4_Recive_Data[50];//���ջ���
extern  u8 	uUart4_Recive_Length;//���ճ���
extern u8 	uUart4_Recive_End;//�������
extern u8  	uUart4_Send_Data[400];//���ͻ���
extern u8 	uUart4_Send_Delay;//��ʱ����


extern 	u8 volatile 	uUart5_Recive_Data[50];//���ջ���
extern  u8 	uUart5_Recive_Length;//���ճ���
extern u8 	uUart5_Recive_End;//�������
extern u8 volatile 	uUart5_Send_Data[250];//���ͻ���
extern u8 	uUart5_Send_Delay;//��ʱ����

extern 	u8 volatile 	uUart1_Recive_Data[50];//���ջ���
extern  u8 	uUart1_Recive_Length;//���ճ���
extern u8 	uUart1_Recive_End;//�������
extern u8 volatile 	uUart1_Send_Data[100];//���ͻ���
extern u8 	uUart1_Send_Delay;//��ʱ����



extern 	u8 volatile 	uUart2_Recive_Data[200];//���ջ���
extern  u8 	uUart2_Recive_Length;//���ճ���
extern u8 	uUart2_Recive_End;//�������
extern u8 volatile 	uUart2_Send_Data[200];//���ͻ���
extern u8 	uUart2_Send_Delay;//��ʱ����



extern 	u8 volatile 	uUart3_Recive_Data[50];//���ջ���
extern  u8 	uUart3_Recive_Length;//���ճ���
extern u8 	uUart3_Recive_End;//�������
extern u8 volatile 	uUart3_Send_Data[20];//���ͻ���
extern u8 	uUart3_Send_Delay;//��ʱ����


void uart4_init(u32 bound);
void uart5_init(u32 bound);
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart2_uninit(void);
void uart3_init(u32 bound);
u8 User_UART4_SendStringx(char *pString,u16 Length);
u8 User_UART2_SendStringx(char *pString,u8 Length);
u8 User_UART5_SendStringx(char *pString,u8 Length);
u8 User_UART3_SendStringx(char *pString,u8 Length);	


#endif	   
















