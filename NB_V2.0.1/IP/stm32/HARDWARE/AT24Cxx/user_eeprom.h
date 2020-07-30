/********************************************************
***��������ͷ�ļ���user_uart.h
***
***����ʱ�䣺2011-01-29			�����ˣ�������
***�޸�ʱ�䣺               �޸��ˣ�
********************************************************/
#ifndef __USER_EEPROM_H
#define __USER_EEPROM_H


#define I2C_PORT (GPIOB)
#define SDA_PIN  (GPIO_PIN_5) /*ģ��I2C���ݴ���λ*/
#define SCL_PIN  (GPIO_PIN_4)/*ģ��I2Cʱ�ӿ���λ*/
#define		BS_SCL_PIN				GPIO_WriteHigh(I2C_PORT,SCL_PIN)
#define		BC_SCL_PIN				GPIO_WriteLow(I2C_PORT,SCL_PIN)
#define		BS_SDA_PIN				GPIO_WriteHigh(I2C_PORT,SDA_PIN)
#define		BC_SDA_PIN				GPIO_WriteLow(I2C_PORT,SDA_PIN)


static void 	delay(u16 de_time);										//��ʱ
void	_I2C_Init(void);											//��ʼ�� io
void	IIC_ack(void);												//�ȴ��ӻ�Ӧ���ź�
void 	IIC_sendack(void);										//��������ackӦ���ź�
void 	IIC_start(u8 slave_add);							//��ʼ�ź�	
void 	IIC_stop(void);												//�����ź�
void 	IIC_write_byte(u8 buffer);						//�ֽ�д
u8  	IIC_read(void);												//�ֽڶ�

//extern	void	I2C_Init_Simulation(void);// ��IIC��ʼ��
extern	void IIC_SendByte(u8 addr_base,u8 buffer);	//���дһ���ֽ�
extern	void IIC_RcvByte(u8 addr_base,u8* buffer);	//�����һ���ֽ�
//extern	void IIC_write_page(u8 add_base);		//ҳд
//extern	void IIC_read_page(u8 add_base);		//ҳ��


#endif

