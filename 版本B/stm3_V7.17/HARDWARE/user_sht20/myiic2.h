#ifndef __MYIIC2_H
#define __MYIIC2_H












//-----------------�˿ڶ���----------------  	


 
  

 
 
//IO��������	 
#define MYIIC2_SCLK_Clr GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define MYIIC2_SCLK_Set GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define MYIIC2_SDIN_Clr GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define MYIIC2_SDIN_Set GPIO_SetBits(GPIOB,GPIO_Pin_9)


#define	MYIIC2_SDIN_PIN	 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

 
     






//IIC���в�������
void MYIIC2_UnInit(void);
void MYIIC2_Init(void);                //��ʼ��IIC��IO��				 
void MYIIC2_Start(void);				//����IIC��ʼ�ź�
void MYIIC2_Stop(void);	  			//����IICֹͣ�ź�
void MYIIC2_Send_Byte(unsigned char txd);			//IIC����һ���ֽ�
unsigned char MYIIC2_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char MYIIC2_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MYIIC2_Ack(void);					//IIC����ACK�ź�
void MYIIC2_NAck(void);				//IIC������ACK�ź�

//void MYIIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 MYIIC1_Read_One_Byte(u8 daddr,u8 addr);	  
#endif




