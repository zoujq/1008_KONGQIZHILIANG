#ifndef __MYIIC1_H
#define __MYIIC1_H












//-----------------端口定义----------------  	


 
  
//IO方向设置
//#define MYIIC1_SDA_IN() {P0M1&=0xf0;}
//#define MYIIC1_SDA_OUT() 	{P0M1|=0x08;}
 
//#define MYIIC1_SDA P0_2
 
//IO操作函数	 
#define MYIIC1_SCLK_Clr GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define MYIIC1_SCLK_Set GPIO_SetBits(GPIOB, GPIO_Pin_6)

#define MYIIC1_SDIN_Clr GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define MYIIC1_SDIN_Set GPIO_SetBits(GPIOB, GPIO_Pin_7)


#define MYIIC1_SDA_PIN		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
 
 






//IIC所有操作函数
void MYIIC1_Init(void);                //初始化IIC的IO口				 
void MYIIC1_Start(void);				//发送IIC开始信号
void MYIIC1_Stop(void);	  			//发送IIC停止信号
void MYIIC1_Send_Byte(unsigned char txd);			//IIC发送一个字节
unsigned char MYIIC1_Read_Byte(unsigned char ack);//IIC读取一个字节
unsigned char MYIIC1_Wait_Ack(void); 				//IIC等待ACK信号
void MYIIC1_Ack(void);					//IIC发送ACK信号
void MYIIC1_NAck(void);				//IIC不发送ACK信号

//void MYIIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 MYIIC1_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















