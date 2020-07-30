#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/13 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
										  

void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI2_Init(void);			 //��ʼ��SPI2��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�  		 
void SPI2_SetSpeed(u8 SpeedSet); //����SPI2�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�
u8 SPI2_ReadWriteByte(u8 TxData);//SPI2���߶�дһ���ֽ�
#endif

