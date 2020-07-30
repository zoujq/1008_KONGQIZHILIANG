#include "myiic2.h"
#include "includes.h"	   
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
 
 static void delay_us(u32 dly)
 {
	 
	     int i,j;
    for (i=0;i<dly;i++)   for (j=0;j<40;j++) ;


}
 
//IO��������
void MYIIC2_SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��

}
void MYIIC2_SDA_OUT(void) 
{

	
    GPIO_InitTypeDef GPIO_InitStructure;
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��



}



//��ʼ��IIC
void MYIIC2_Init(void)
{					     

	
    GPIO_InitTypeDef GPIO_InitStructure;


		

	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��

		GPIO_SetBits(GPIOB,GPIO_Pin_8);
		GPIO_SetBits(GPIOB,GPIO_Pin_9);			
	
}


void MYIIC2_UnInit(void)
{					     

    GPIO_InitTypeDef GPIO_InitStructure;


		

	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��

		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);				

}




//����IIC��ʼ�ź�
void MYIIC2_Start(void)
{
	MYIIC2_SDA_OUT();     //sda�����
//	IIC_SDA=1;	  	
  MYIIC2_SDIN_Set;
	//IIC_SCL=1;
	MYIIC2_SCLK_Set;
	delay_us(4);
 	//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	MYIIC2_SDIN_Clr;
	delay_us(4);
	//IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	MYIIC2_SCLK_Clr;
}	  
//����IICֹͣ�ź�
void MYIIC2_Stop(void)
{
	MYIIC2_SDA_OUT();//sda�����
	//IIC_SCL=0;
	//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	MYIIC2_SCLK_Clr;
	MYIIC2_SDIN_Clr;
	
 	delay_us(4);
//	IIC_SCL=1; 
//	IIC_SDA=1;//����I2C���߽����ź�
	MYIIC2_SCLK_Set;
	MYIIC2_SDIN_Set;
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
unsigned char MYIIC2_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MYIIC2_SDA_IN();      //SDA����Ϊ����  
	//IIC_SDA=1;
	MYIIC2_SDIN_Set;	
	delay_us(1);	   
	//IIC_SCL=1;
	MYIIC2_SCLK_Set;	
	delay_us(1);	 
	while(MYIIC2_SDIN_PIN)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MYIIC2_Stop();
			return 1;
		}
	}
	//IIC_SCL=0;//ʱ�����0 	
		MYIIC2_SCLK_Clr;
	
	return 0;  
} 
//����ACKӦ��
void MYIIC2_Ack(void)
{
	//IIC_SCL=0;
		MYIIC2_SCLK_Clr;	
	MYIIC2_SDA_OUT();
	//IIC_SDA=0;
	MYIIC2_SDIN_Clr;	
	delay_us(2);
	//IIC_SCL=1;
	MYIIC2_SCLK_Set;		
	delay_us(2);
	//IIC_SCL=0;
		MYIIC2_SCLK_Clr;		
}
//������ACKӦ��		    
void MYIIC2_NAck(void)
{
//	IIC_SCL=0;
		MYIIC2_SCLK_Clr;		
	MYIIC2_SDA_OUT();
//	IIC_SDA=1;
	MYIIC2_SDIN_Set;		
	delay_us(2);
	//IIC_SCL=1;
		MYIIC2_SCLK_Set;		
	delay_us(2);
	//IIC_SCL=0;
			MYIIC2_SCLK_Clr;		
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MYIIC2_Send_Byte(unsigned char txd)
{                        
    u8 t;   
	MYIIC2_SDA_OUT(); 	    
  //  IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
			MYIIC2_SCLK_Clr;		
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
		//	IIC_SDA=1;
			MYIIC2_SDIN_Set;		
		else
	//		IIC_SDA=0;
		MYIIC2_SDIN_Clr;		
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
	//	IIC_SCL=1;
				MYIIC2_SCLK_Set;		
		delay_us(2); 
	//	IIC_SCL=0;	
				MYIIC2_SCLK_Clr;			
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
unsigned char MYIIC2_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MYIIC2_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
     //   IIC_SCL=0; 
					MYIIC2_SCLK_Clr;		
        delay_us(2);
		//IIC_SCL=1;
						MYIIC2_SCLK_Set;		
        receive<<=1;
        if(MYIIC2_SDIN_PIN)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        MYIIC2_NAck();//����nACK
    else
        MYIIC2_Ack(); //����ACK   
    return receive;
}


























