#ifndef __MYIIC2_H
#define __MYIIC2_H












//-----------------端口定义----------------  	


 
  

 
 
//IO操作函数	 
#define MYIIC2_SCLK_Clr GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define MYIIC2_SCLK_Set GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define MYIIC2_SDIN_Clr GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define MYIIC2_SDIN_Set GPIO_SetBits(GPIOB,GPIO_Pin_9)


#define	MYIIC2_SDIN_PIN	 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

 
     






//IIC所有操作函数
void MYIIC2_UnInit(void);
void MYIIC2_Init(void);                //初始化IIC的IO口				 
void MYIIC2_Start(void);				//发送IIC开始信号
void MYIIC2_Stop(void);	  			//发送IIC停止信号
void MYIIC2_Send_Byte(unsigned char txd);			//IIC发送一个字节
unsigned char MYIIC2_Read_Byte(unsigned char ack);//IIC读取一个字节
unsigned char MYIIC2_Wait_Ack(void); 				//IIC等待ACK信号
void MYIIC2_Ack(void);					//IIC发送ACK信号
void MYIIC2_NAck(void);				//IIC不发送ACK信号

//void MYIIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 MYIIC1_Read_One_Byte(u8 daddr,u8 addr);	  
#endif




