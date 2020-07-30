#include "myiic.h"
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
 


void MYIIC_SDA_IN(void)
{	

    GPIO_InitTypeDef GPIO_InitStructure;
 
		   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��

}
void MYIIC_SDA_OUT(void)
{


	
    GPIO_InitTypeDef GPIO_InitStructure;
 
		   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��




}


void MYIIC_SCL_OUT(void)
{


    GPIO_InitTypeDef GPIO_InitStructure;
 
		   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��

	



}








//����IIC��ʼ�ź�
void MYIIC_Start(void)
{
	
  MYIIC_SCL_OUT();	
	MYIIC_SDA_OUT();     //sda�����
//	IIC_SDA=1;	  	
  MYIIC_SDIN_Set;
	//IIC_SCL=1;
	MYIIC_SCLK_Set;
	delay_us(4);
 	//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	MYIIC_SDIN_Clr;
	delay_us(4);
	//IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	MYIIC_SCLK_Clr;
}	  
//����IICֹͣ�ź�
void MYIIC_Stop(void)
{
	MYIIC_SDA_OUT();//sda�����
	//IIC_SCL=0;
	//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	MYIIC_SCLK_Clr;
	MYIIC_SDIN_Clr;
	
 	delay_us(4);
//	IIC_SCL=1; 
//	IIC_SDA=1;//����I2C���߽����ź�
	MYIIC_SCLK_Set;
	MYIIC_SDIN_Set;
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MYIIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MYIIC_SDA_IN();      //SDA����Ϊ����  
	//IIC_SDA=1;
	MYIIC_SDIN_Set;	
	delay_us(1);	   
	//IIC_SCL=1;
	MYIIC_SCLK_Set;	
	delay_us(1);	 
	while( 0!=MYIIC_SDA_PIN)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MYIIC_Stop();
			return 1;
		}
	}
	//IIC_SCL=0;//ʱ�����0 	
		MYIIC_SCLK_Clr;
	
	return 0;  
} 
//����ACKӦ��
void MYIIC_Ack(void)
{
	//IIC_SCL=0;
		MYIIC_SCLK_Clr;	
	MYIIC_SDA_OUT();
	//IIC_SDA=0;
	MYIIC_SDIN_Clr;	
	delay_us(2);
	//IIC_SCL=1;
	MYIIC_SCLK_Set;		
	delay_us(2);
	//IIC_SCL=0;
		MYIIC_SCLK_Clr;		
}
//������ACKӦ��		    
void MYIIC_NAck(void)
{
//	IIC_SCL=0;
		MYIIC_SCLK_Clr;		
	MYIIC_SDA_OUT();
//	IIC_SDA=1;
	MYIIC_SDIN_Set;		
	delay_us(2);
	//IIC_SCL=1;
		MYIIC_SCLK_Set;		
	delay_us(2);
	//IIC_SCL=0;
			MYIIC_SCLK_Clr;		
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MYIIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MYIIC_SDA_OUT(); 	    
  //  IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
			MYIIC_SCLK_Clr;		
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
		//	IIC_SDA=1;
			MYIIC_SDIN_Set;		
		else
	//		IIC_SDA=0;
		MYIIC_SDIN_Clr;		
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
	//	IIC_SCL=1;
				MYIIC_SCLK_Set;		
		delay_us(2); 
	//	IIC_SCL=0;	
				MYIIC_SCLK_Clr;			
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 MYIIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MYIIC_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
     //   IIC_SCL=0; 
					MYIIC_SCLK_Clr;		
        delay_us(2);
		//IIC_SCL=1;
						MYIIC_SCLK_Set;		
        receive<<=1;
        if( 0!=MYIIC_SDA_PIN)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        MYIIC_NAck();//����nACK
    else
        MYIIC_Ack(); //����ACK   
    return receive;
}



























