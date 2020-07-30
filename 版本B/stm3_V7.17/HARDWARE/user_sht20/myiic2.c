#include "myiic2.h"
#include "includes.h"	   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
 static void delay_us(u32 dly)
 {
	 
	     int i,j;
    for (i=0;i<dly;i++)   for (j=0;j<40;j++) ;


}
 
//IO方向设置
void MYIIC2_SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化

}
void MYIIC2_SDA_OUT(void) 
{

	
    GPIO_InitTypeDef GPIO_InitStructure;
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化



}



//初始化IIC
void MYIIC2_Init(void)
{					     

	
    GPIO_InitTypeDef GPIO_InitStructure;


		

	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化

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
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化

		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);				

}




//产生IIC起始信号
void MYIIC2_Start(void)
{
	MYIIC2_SDA_OUT();     //sda线输出
//	IIC_SDA=1;	  	
  MYIIC2_SDIN_Set;
	//IIC_SCL=1;
	MYIIC2_SCLK_Set;
	delay_us(4);
 	//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	MYIIC2_SDIN_Clr;
	delay_us(4);
	//IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	MYIIC2_SCLK_Clr;
}	  
//产生IIC停止信号
void MYIIC2_Stop(void)
{
	MYIIC2_SDA_OUT();//sda线输出
	//IIC_SCL=0;
	//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	MYIIC2_SCLK_Clr;
	MYIIC2_SDIN_Clr;
	
 	delay_us(4);
//	IIC_SCL=1; 
//	IIC_SDA=1;//发送I2C总线结束信号
	MYIIC2_SCLK_Set;
	MYIIC2_SDIN_Set;
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
unsigned char MYIIC2_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MYIIC2_SDA_IN();      //SDA设置为输入  
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
	//IIC_SCL=0;//时钟输出0 	
		MYIIC2_SCLK_Clr;
	
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void MYIIC2_Send_Byte(unsigned char txd)
{                        
    u8 t;   
	MYIIC2_SDA_OUT(); 	    
  //  IIC_SCL=0;//拉低时钟开始数据传输
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
		delay_us(2);   //对TEA5767这三个延时都是必须的
	//	IIC_SCL=1;
				MYIIC2_SCLK_Set;		
		delay_us(2); 
	//	IIC_SCL=0;	
				MYIIC2_SCLK_Clr;			
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
unsigned char MYIIC2_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MYIIC2_SDA_IN();//SDA设置为输入
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
        MYIIC2_NAck();//发送nACK
    else
        MYIIC2_Ack(); //发送ACK   
    return receive;
}


























