#ifndef  __I2C0_H__
#define  __I2C0_H__


 
//#define PORTD GPIOE
//#define SDA GPIO_PIN_2
//#define SCL	GPIO_PIN_1


#define SDA_SET       GPIO_SetBits(GPIOE,GPIO_Pin_1)
#define SDA_RESET     GPIO_ResetBits(GPIOE,GPIO_Pin_1)
#define SCL_SET       GPIO_SetBits(GPIOE,GPIO_Pin_0)
#define SCL_RESET     GPIO_ResetBits(GPIOE,GPIO_Pin_0)

#define SDA_PIN     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)



#define	INT8U	u8


void delayusi2c(u32 dly);
void I2C0_UnInit(void);
void I2C0_Init(void);
void I2C0_Start(void);
void I2C0_Stop(void);
u8 I2C0_WaitACK(void);
void I2C0_ACK(void);
void I2C0_NoACK(void);
void I2C0_WriteByte(unsigned char dat);
unsigned char I2C0_ReadByte(unsigned char ack);

#endif
