#include  "joypad.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��Ϸ�ֱ����� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//��ʼ���ֱ��ӿ�.	 
void JOYPAD_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_12;				 //
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��GPIO
 	GPIO_SetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_12);	//����

 	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
 	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_9);	//����
}

//��ȡ�ֱ�����ֵ.
//FC�ֱ����������ʽ:
//ÿ��һ������,���һλ����,���˳��:
//A->B->SELECT->START->UP->DOWN->LEFT->RIGHT.
//�ܹ�8λ,������C��ť���ֱ�,����C��ʵ�͵���A+Bͬʱ����.
//������0,�ɿ���1.
//����ֵ:
//[0]:��
//[1]:��
//[2]:��
//[3]:��
//[1]:Start
//[5]:Select
//[6]:B
//[7]:A
u8 JOYPAD_Read(void)
{
	u8 temp=0;
	u8 t;
	JOYPAD_LAT=1;					//���浱ǰ״̬
 	JOYPAD_LAT=0;
	for(t=0;t<8;t++)
	{
		temp<<=1;	 
		if(JOYPAD_DAT)temp|=0x01;	//LOAD֮�󣬾͵õ���һ������
		JOYPAD_CLK=1;			   	//ÿ��һ�����壬�յ�һ������
 		JOYPAD_CLK=0;	
	}
	return temp;
}





