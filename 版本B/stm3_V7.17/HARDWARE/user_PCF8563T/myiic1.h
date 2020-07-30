#ifndef __MYIIC1_H
#define __MYIIC1_H












//-----------------�˿ڶ���----------------  	


 
  
//IO��������
//#define MYIIC1_SDA_IN() {P0M1&=0xf0;}
//#define MYIIC1_SDA_OUT() 	{P0M1|=0x08;}
 
//#define MYIIC1_SDA P0_2
 
//IO��������	 
#define MYIIC1_SCLK_Clr GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define MYIIC1_SCLK_Set GPIO_SetBits(GPIOB, GPIO_Pin_6)

#define MYIIC1_SDIN_Clr GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define MYIIC1_SDIN_Set GPIO_SetBits(GPIOB, GPIO_Pin_7)


#define MYIIC1_SDA_PIN		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
 
 






//IIC���в�������
void MYIIC1_Init(void);                //��ʼ��IIC��IO��				 
void MYIIC1_Start(void);				//����IIC��ʼ�ź�
void MYIIC1_Stop(void);	  			//����IICֹͣ�ź�
void MYIIC1_Send_Byte(unsigned char txd);			//IIC����һ���ֽ�
unsigned char MYIIC1_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char MYIIC1_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MYIIC1_Ack(void);					//IIC����ACK�ź�
void MYIIC1_NAck(void);				//IIC������ACK�ź�

//void MYIIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 MYIIC1_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















