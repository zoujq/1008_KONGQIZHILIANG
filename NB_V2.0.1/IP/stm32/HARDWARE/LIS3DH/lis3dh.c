#include "lis3dh.h"
#include "delay.h"
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
 
 
 
 
u8 uLIS3DH_X=0;//X������ֵ
u8 uLIS3DH_Y=0;//Y������ֵ
u8 uLIS3DH_Z=0;//Z������ֵ
 
 
 
 
//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB10,PB11 �����
}



//����IIC��ʼ�ź�
void IIC_Start_1(u8 aaa)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	//IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	 IIC_Send_Byte(aaa);
	
	
}	 


//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
			delay_us(2);
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
			delay_us(2);
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}



u8 User_LIS3DH_Init(void)//3����ٶȴ��������ƿڳ�ʼ��
{



 IIC_Init();


 //User_LIS3DH_Task();//3����ٶȴ���������
	
	
	
	IIC_Start_1(0x32);
IIC_Wait_Ack();
IIC_Send_Byte(0x20);
IIC_Wait_Ack();
//IIC_Send_Byte(0x47);
	IIC_Send_Byte(0x4f);
	
IIC_Wait_Ack(); 	
IIC_Stop();
				
	IIC_Start_1(0x32);
IIC_Wait_Ack();
IIC_Send_Byte(0x21);
IIC_Wait_Ack();
IIC_Send_Byte(0x00);
IIC_Wait_Ack(); 	
IIC_Stop();		

IIC_Start_1(0x32);
IIC_Wait_Ack();
IIC_Send_Byte(0x22);
IIC_Wait_Ack();
IIC_Send_Byte(0x00);
IIC_Wait_Ack(); 	
IIC_Stop();

IIC_Start_1(0x32);
IIC_Wait_Ack();
IIC_Send_Byte(0x23);
IIC_Wait_Ack();
IIC_Send_Byte(0x00);
IIC_Wait_Ack(); 	
IIC_Stop();






}





u8 User_LIS3DH_Read(void)//��ȡ3����ٶȴ�����������ֵ
{

	
//	_I2C_Init();
	



IIC_Start_1(0x32);
IIC_Wait_Ack();
IIC_Send_Byte(0x29);
IIC_Wait_Ack();
IIC_Start_1(0x33);
IIC_Wait_Ack(); 	
uLIS3DH_X=IIC_Read_Byte(0);
IIC_Stop();

	
	
IIC_Start_1(0x32);
IIC_Wait_Ack();
IIC_Send_Byte(0x2B);
IIC_Wait_Ack();
IIC_Start_1(0x33);
IIC_Wait_Ack(); 	
uLIS3DH_Y=IIC_Read_Byte(0);
IIC_Stop();

	
IIC_Start_1(0x32);
IIC_Wait_Ack();
IIC_Send_Byte(0x2D);
IIC_Wait_Ack();
IIC_Start_1(0x33);
IIC_Wait_Ack(); 	
uLIS3DH_Z=IIC_Read_Byte(0);
IIC_Stop();






}







