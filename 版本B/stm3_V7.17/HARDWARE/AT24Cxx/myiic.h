#ifndef __MYIIC_H
#define __MYIIC_H





//#define		BS_SCL_PIN				GPIO_WriteHigh(I2C_PORT,SCL_PIN)
//#define		BC_SCL_PIN				GPIO_WriteLow(I2C_PORT,SCL_PIN)
//#define		BS_SDA_PIN				GPIO_WriteHigh(I2C_PORT,SDA_PIN)
//#define		BC_SDA_PIN				GPIO_WriteLow(I2C_PORT,SDA_PIN)





//-----------------OLED�˿ڶ���----------------  	

 //#define	MYIIC_SCLK_PIN		(GPIO_P24)
 //#define	MYIIC_SDIN_PIN		(GPIO_P23)
 
  
//IO��������

 
 
//IO��������	 
#define MYIIC_SCLK_Clr GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define MYIIC_SCLK_Set GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define MYIIC_SDIN_Clr GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define MYIIC_SDIN_Set GPIO_SetBits(GPIOB, GPIO_Pin_13)


#define MYIIC_SDA_PIN		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
 
 




//IIC���в�������
void MYIIC_Init(void);                //��ʼ��IIC��IO��				 
void MYIIC_Start(void);				//����IIC��ʼ�ź�
void MYIIC_Stop(void);	  			//����IICֹͣ�ź�
void MYIIC_Send_Byte(unsigned char txd);			//IIC����һ���ֽ�
unsigned char MYIIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char MYIIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MYIIC_Ack(void);					//IIC����ACK�ź�
void MYIIC_NAck(void);				//IIC������ACK�ź�

//void MYIIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 MYIIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















