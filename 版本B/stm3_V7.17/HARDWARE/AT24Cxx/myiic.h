#ifndef __MYIIC_H
#define __MYIIC_H





//#define		BS_SCL_PIN				GPIO_WriteHigh(I2C_PORT,SCL_PIN)
//#define		BC_SCL_PIN				GPIO_WriteLow(I2C_PORT,SCL_PIN)
//#define		BS_SDA_PIN				GPIO_WriteHigh(I2C_PORT,SDA_PIN)
//#define		BC_SDA_PIN				GPIO_WriteLow(I2C_PORT,SDA_PIN)





//-----------------OLED端口定义----------------  	

 //#define	MYIIC_SCLK_PIN		(GPIO_P24)
 //#define	MYIIC_SDIN_PIN		(GPIO_P23)
 
  
//IO方向设置

 
 
//IO操作函数	 
#define MYIIC_SCLK_Clr GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define MYIIC_SCLK_Set GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define MYIIC_SDIN_Clr GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define MYIIC_SDIN_Set GPIO_SetBits(GPIOB, GPIO_Pin_13)


#define MYIIC_SDA_PIN		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
 
 




//IIC所有操作函数
void MYIIC_Init(void);                //初始化IIC的IO口				 
void MYIIC_Start(void);				//发送IIC开始信号
void MYIIC_Stop(void);	  			//发送IIC停止信号
void MYIIC_Send_Byte(unsigned char txd);			//IIC发送一个字节
unsigned char MYIIC_Read_Byte(unsigned char ack);//IIC读取一个字节
unsigned char MYIIC_Wait_Ack(void); 				//IIC等待ACK信号
void MYIIC_Ack(void);					//IIC发送ACK信号
void MYIIC_NAck(void);				//IIC不发送ACK信号

//void MYIIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 MYIIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















