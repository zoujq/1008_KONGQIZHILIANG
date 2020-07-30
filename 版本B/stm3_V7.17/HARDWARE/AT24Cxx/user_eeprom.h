/********************************************************
***串口配置头文件：user_uart.h
***
***创建时间：2011-01-29			创建人：邹勇坤
***修改时间：               修改人：
********************************************************/
#ifndef __USER_EEPROM_H
#define __USER_EEPROM_H


#define I2C_PORT (GPIOB)
#define SDA_PIN  (GPIO_PIN_5) /*模拟I2C数据传送位*/
#define SCL_PIN  (GPIO_PIN_4)/*模拟I2C时钟控制位*/
#define		BS_SCL_PIN				GPIO_WriteHigh(I2C_PORT,SCL_PIN)
#define		BC_SCL_PIN				GPIO_WriteLow(I2C_PORT,SCL_PIN)
#define		BS_SDA_PIN				GPIO_WriteHigh(I2C_PORT,SDA_PIN)
#define		BC_SDA_PIN				GPIO_WriteLow(I2C_PORT,SDA_PIN)


static void 	delay(u16 de_time);										//延时
void	_I2C_Init(void);											//初始化 io
void	IIC_ack(void);												//等待从机应答信号
void 	IIC_sendack(void);										//主机发送ack应答信号
void 	IIC_start(u8 slave_add);							//开始信号	
void 	IIC_stop(void);												//结束信号
void 	IIC_write_byte(u8 buffer);						//字节写
u8  	IIC_read(void);												//字节读

//extern	void	I2C_Init_Simulation(void);// 软IIC初始化
extern	void IIC_SendByte(u8 addr_base,u8 buffer);	//随机写一个字节
extern	void IIC_RcvByte(u8 addr_base,u8* buffer);	//随机读一个字节
//extern	void IIC_write_page(u8 add_base);		//页写
//extern	void IIC_read_page(u8 add_base);		//页读


#endif

