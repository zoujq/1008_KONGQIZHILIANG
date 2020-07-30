#include "myiic1.h"
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
 
 static void myiic1_delay_us(unsigned char dly)
 {
	 
	     int i,j;
    for (i=0;i<dly;i++)   for (j=0;j<40;j++) ;


}
 

void MYIIC1_SDA_IN(void)
{	

    GPIO_InitTypeDef GPIO_InitStructure;
 
		   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��

}
void MYIIC1_SDA_OUT(void)
{


	
    GPIO_InitTypeDef GPIO_InitStructure;
 
		   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��




}

void MYIIC1_SCL_OUT(void)
{


    GPIO_InitTypeDef GPIO_InitStructure;
 
		   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ


}



//��ʼ��IIC
void MYIIC1_Init(void)
{					     

	
	
	//while(1);
}
//����IIC��ʼ�ź�
void MYIIC1_Start(void)
{
	MYIIC1_SDA_OUT();     //sda�����
	MYIIC1_SCL_OUT();     //scl�����
//	IIC_SDA=1;	  	
  MYIIC1_SDIN_Set;
	//IIC_SCL=1;
	MYIIC1_SCLK_Set;
	myiic1_delay_us(4);
 	//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	MYIIC1_SDIN_Clr;
	myiic1_delay_us(4);
	//IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	MYIIC1_SCLK_Clr;
}	  
//����IICֹͣ�ź�
void MYIIC1_Stop(void)
{
	MYIIC1_SDA_OUT();//sda�����
	//IIC_SCL=0;
	//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	MYIIC1_SCLK_Clr;
	MYIIC1_SDIN_Clr;
	
 	myiic1_delay_us(4);
//	IIC_SCL=1; 
//	IIC_SDA=1;//����I2C���߽����ź�
	MYIIC1_SCLK_Set;
	MYIIC1_SDIN_Set;
	myiic1_delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
unsigned char MYIIC1_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MYIIC1_SDA_IN();      //SDA����Ϊ����  
	//IIC_SDA=1;
	MYIIC1_SDIN_Set;	
	myiic1_delay_us(1);	   
	//IIC_SCL=1;
	MYIIC1_SCLK_Set;	
	myiic1_delay_us(1);	 
	while(MYIIC1_SDA_PIN)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MYIIC1_Stop();
			return 1;
		}
	}
	//IIC_SCL=0;//ʱ�����0 	
		MYIIC1_SCLK_Clr;
	
	return 0;  
} 
//����ACKӦ��
void MYIIC1_Ack(void)
{
	//IIC_SCL=0;
		MYIIC1_SCLK_Clr;	
	MYIIC1_SDA_OUT();
	//IIC_SDA=0;
	MYIIC1_SDIN_Clr;	
	myiic1_delay_us(2);
	//IIC_SCL=1;
	MYIIC1_SCLK_Set;		
	myiic1_delay_us(2);
	//IIC_SCL=0;
		MYIIC1_SCLK_Clr;		
}
//������ACKӦ��		    
void MYIIC1_NAck(void)
{
//	IIC_SCL=0;
		MYIIC1_SCLK_Clr;		
	MYIIC1_SDA_OUT();
//	IIC_SDA=1;
	MYIIC1_SDIN_Set;		
	myiic1_delay_us(2);
	//IIC_SCL=1;
		MYIIC1_SCLK_Set;		
	myiic1_delay_us(2);
	//IIC_SCL=0;
			MYIIC1_SCLK_Clr;		
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MYIIC1_Send_Byte(unsigned char txd)
{                        
    u8 t;   
	MYIIC1_SDA_OUT(); 	    
  //  IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
			MYIIC1_SCLK_Clr;		
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
		//	IIC_SDA=1;
			MYIIC1_SDIN_Set;		
		else
	//		IIC_SDA=0;
		MYIIC1_SDIN_Clr;		
		txd<<=1; 	  
		myiic1_delay_us(2);   //��TEA5767��������ʱ���Ǳ����
	//	IIC_SCL=1;
				MYIIC1_SCLK_Set;		
		myiic1_delay_us(2); 
	//	IIC_SCL=0;	
				MYIIC1_SCLK_Clr;			
		myiic1_delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
unsigned char MYIIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MYIIC1_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
     //   IIC_SCL=0; 
					MYIIC1_SCLK_Clr;		
        myiic1_delay_us(2);
		//IIC_SCL=1;
						MYIIC1_SCLK_Set;		
        receive<<=1;
        if(MYIIC1_SDA_PIN)receive++;   
		myiic1_delay_us(1); 
    }					 
    if (!ack)
        MYIIC1_NAck();//����nACK
    else
        MYIIC1_Ack(); //����ACK   
    return receive;
}



























