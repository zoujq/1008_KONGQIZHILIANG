#ifndef __LIS3DH_H__
#define __LIS3DH_H__

#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO��������
#define SDA_IN()  {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=8<<28;}
#define SDA_OUT() {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=3<<28;}

//IO��������	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 



extern u8 uLIS3DH_X;//X������ֵ
extern u8 uLIS3DH_Y;//Y������ֵ
extern u8 uLIS3DH_Z;//Z������ֵ





//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  



u8 User_LIS3DH_Init(void);//3����ٶȴ��������ƿڳ�ʼ��

//u8 User_LIS3DH_Task(void);//3����ٶȴ���������
u8 User_LIS3DH_Read(void);//��ȡ3����ٶȴ�����������ֵ
	



#endif





