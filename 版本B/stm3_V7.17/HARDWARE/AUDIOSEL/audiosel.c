#include "audiosel.h"	  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��Ƶѡ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


//������ʼ��
void Audiosel_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PB�˿�ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//PB.7 ������� 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure); //PD.7 �������
}
//����4052��ѡ��ͨ��
//����ͨ��ѡ��
//0	//MP3ͨ��
//1	//������ͨ��
//2	//PWM��Ƶͨ��
//3	//����  
void Audiosel_Set(u8 ch)
{
	AUDIO_SELA=ch&0X01;
 	AUDIO_SELB=(ch>>1)&0X01;	 
}




