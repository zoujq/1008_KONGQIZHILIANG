#include "lis3dh.h"
#include "delay.h"
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
 
 
 
 
u8 uLIS3DH_X=0;//X轴座标值
u8 uLIS3DH_Y=0;//Y轴座标值
u8 uLIS3DH_Z=0;//Z轴座标值
 
 
 
 
//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB10,PB11 输出高
}



//产生IIC起始信号
void IIC_Start_1(u8 aaa)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	//IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	 IIC_Send_Byte(aaa);
	
	
}	 


//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
			delay_us(2);
    IIC_SCL=0;//拉低时钟开始数据传输
			delay_us(2);
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}



u8 User_LIS3DH_Init(void)//3轴加速度传感器控制口初始化
{



 IIC_Init();


 //User_LIS3DH_Task();//3轴加速度传感器任务
	
	
	
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





u8 User_LIS3DH_Read(void)//读取3轴加速度传感器的座标值
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







