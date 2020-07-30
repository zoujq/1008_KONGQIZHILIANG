/********************************************************
***��ʾ��user_i2c.c
***
***����ʱ�䣺2011-04-26			�����ˣ�������
***�޸�ʱ�䣺               �޸��ˣ�
********************************************************/
#include "user_lib.h"
//**********************************************************
// ��������
//**********************************************************
//**********************************************************
// λ����
//**********************************************************
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define write_bit              	0x00                      //write
#define read_bit               	0x01                      //read
#define eep_slaveadd       			0xA2                      //IIC address:24c02,0xA6/24c04,0xA0
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


BitStatus	fi2c_ack;							//Ӧ���ź�
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief Initializes the I2C peripheral registers to communicate the EEPROM
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * None
  */
	
/******************************************************
***��ʱ
******************************************************/
static void delay(u16 de_time)
{
	u16	i;
	for(i=0;i<de_time;i++)
		{	
			nop(); 
		}
}
/******************************************************
***��ʼ��io��
*******************************************************/
void	_I2C_Init(void)
{

GPIO_Init(I2C_PORT,SCL_PIN|SDA_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
		
	//	BS_SCL_PIN;
//	BS_SDA_PIN;
}
/******************************************************
***�ȴ��ӻ�Ӧ��
*******************************************************/
void IIC_ack(void)			//ack	�ȴ��ӻ�Ӧ���ź�
{
	u8	i=0;
  BC_SCL_PIN;
	nop();
  BS_SDA_PIN;
	nop();
	nop();
	nop();
	nop();
	nop();
	nop();
	nop();
	nop();
	nop();
	BS_SCL_PIN;
	nop();
	nop();
	while((GPIOB->IDR & 0X20 )==0X20)		//PB5
	{
		i++;
		delay(15);
		if(i>3)
		{
			i=0;
			break;
		}
	}
	nop();
	nop();
	BC_SCL_PIN;
	nop();
	BS_SDA_PIN;
}
/******************************************************
***����Ӧ��
*******************************************************/
void IIC_sendack(void)	//��������Ӧ���ź�ack
{
	BC_SCL_PIN;	
	nop();
	nop();
	nop();
	nop();
  BC_SDA_PIN;	
	nop();
	nop();
	nop();
	nop();
  BS_SCL_PIN;	
	nop();
	nop();
	nop();
	nop();
  //*****************additional***
  BC_SCL_PIN;	
	nop();
  BS_SDA_PIN;	
}
/******************************************************
***��ʼ�ź�
*******************************************************/
void IIC_start(u8 slave_add)   //start condition & send slave address
{
  BS_SCL_PIN;
	nop();
  BS_SDA_PIN;
	nop();
	nop();
	nop();
	nop();
	nop();
  BC_SDA_PIN;	
	nop();
	nop();
	nop();
	nop();
	nop();
	IIC_write_byte(slave_add);
}
/******************************************************
***�����ź�
*******************************************************/
void IIC_stop(void) 			//stop condition
{
  BC_SCL_PIN;
	nop();
	nop();
	BC_SDA_PIN;
	nop();
	nop();
  BS_SCL_PIN;
	nop();
	nop();
	nop();
	nop();
  BS_SDA_PIN;
}
/******************************************************
***дһ���ֽ�
*******************************************************/
void IIC_write_byte(u8 buffer)  //write a byte 
{
	u8 i; 
	for(i=0;i<=7;i++)
	{  		
		BC_SCL_PIN;
		nop();
		nop();
		nop();
		nop();
		if(((buffer<<i)&0x80)==0)
		BC_SDA_PIN;
		else
		BS_SDA_PIN;
		nop();
		nop();
		nop();
		nop();
		nop();
		BS_SCL_PIN;
		nop();
		nop();
		nop();
		nop();
		nop();
	}
}
/******************************************************
***��һ���ֽ�
*******************************************************/
u8  IIC_read(void)
{
  u8 i,dat=0;
	BS_SDA_PIN;
  for(i=0;i<=7;i++)
	{
		BC_SCL_PIN;
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		nop();
		BS_SCL_PIN;
		nop();
		nop();
		dat=(dat<<1);
		if((GPIOB->IDR & 0X20 )==0X20)
		dat|=0x01;
		nop();
		nop();
		nop();
	}
  return dat;
}
/******************************************************
***���дһ���ֽ�
*******************************************************/
void IIC_SendByte(u8 addr_base,u8 buffer)
{
	_I2C_Init();
	IIC_start(eep_slaveadd|write_bit);
	IIC_ack();
	IIC_write_byte(addr_base);
	IIC_ack(); 
	IIC_write_byte(buffer); 
	IIC_ack();	
	IIC_stop(); 
	delay(5000);
}
/******************************************************
***�����һ���ֽ�
*******************************************************/
void IIC_RcvByte(u8 addr_base,u8* buffer)
{
	_I2C_Init();
	IIC_start(eep_slaveadd|write_bit);
	IIC_ack();
	IIC_write_byte(addr_base);	
	IIC_ack(); 
	IIC_start(eep_slaveadd|read_bit);
	IIC_ack();
	*buffer=IIC_read();
	IIC_stop();
	delay(5000);
}
/******************************************************
***ҳд
*******************************************************/
/*void IIC_write_page(u8 add_base)  //write page
{
	_I2C_Init();
	IIC_start(eep_slaveadd|write_bit);
	IIC_ack();
	IIC_write_byte(add_base);
	IIC_ack(); 
	IIC_write_byte(dSYS_X_axis);          	
	IIC_ack();	
	IIC_write_byte(dSYS_Y_axis);          	
	IIC_ack();	
	IIC_stop();   
	delay(1000);
}*/
/******************************************************
***ҳ��
*******************************************************/
/*void  IIC_read_page(u8 add_base)
{
	_I2C_Init();
	IIC_start(eep_slaveadd|write_bit);
	IIC_ack();
	IIC_write_byte(add_base);	
	IIC_ack(); 
	IIC_start(eep_slaveadd|read_bit);
	IIC_ack();
	
	data_x0=IIC_read();
	IIC_sendack();
	data_x1=IIC_read();
	IIC_sendack();
	data_y0=IIC_read();
	IIC_sendack();
	data_y1=IIC_read();
	IIC_sendack();
	data_z0=IIC_read();
	IIC_sendack();
	data_z1=IIC_read();
	IIC_stop();
	//delay(1000);
}*/
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
