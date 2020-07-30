/*******************************************************************************
 * 文件名  ：i2c.c
 * 描述    ：I2C程序
 * 实验平台：
 * 库版本  ：V1.5.1
 * 作者    ：liuyong
 *修改时间 ：2014-05-07
*******************************************************************************/
#include "includes.h"	   

#include "i2c0.h"


#if 1

/*******************************************************************************
 * 名称: void delayus(uint i)
 * 功能: 延时函数
 * 形参: 无
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void delayusi2c(u32 dly)
{  
	     int i,j;
    for (i=0;i<dly;i++)   for (j=0;j<40;j++) ;


}

/*******************************************************************************
 * 名称: void I2C0_Init(void)
 * 功能: I2C初始化函数
 * 形参: 无
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void I2C0_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;


		

	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOE, &GPIO_InitStructure); //初始化

		GPIO_SetBits(GPIOE,GPIO_Pin_0);
		GPIO_SetBits(GPIOE,GPIO_Pin_1);			
			
}


void I2C0_UnInit(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;


		

	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOE, &GPIO_InitStructure); //初始化

		GPIO_ResetBits(GPIOE,GPIO_Pin_0);
		GPIO_ResetBits(GPIOE,GPIO_Pin_1);			
	
	
	
}




void I2C0_SDA_OUT(void)
{

	
    GPIO_InitTypeDef GPIO_InitStructure;
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOE, &GPIO_InitStructure); //初始化




}

void  I2C0_SDA_IN(void)
{

	
    GPIO_InitTypeDef GPIO_InitStructure;
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//
    GPIO_Init(GPIOE, &GPIO_InitStructure); //初始化



}
 
 
 

/*******************************************************************************
 * 名称: void I2C0_Start(void)
 * 功能: I2C启动函数
 * 形参: 无
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void I2C0_Start(void)
{
  I2C0_SDA_OUT();     //sda线输出
	delayusi2c(2);	
  SDA_SET;	  	  
  SCL_SET;
  delayusi2c(4);
  SDA_RESET;//START:when CLK is high,DATA change form high to low 
  delayusi2c(4);
  SCL_RESET;//钳住I2C总线，准备发送或接收数据 
	  delayusi2c(4);
}
/*******************************************************************************
 * 名称: void I2C0_stop(void)
 * 功能: I2C停止函数
 * 形参: 无
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void I2C0_Stop(void)
{
  I2C0_SDA_OUT();//sda线输出
	delayusi2c(2);	
  SCL_RESET;
  SDA_RESET;//STOP:when CLK is high DATA change form low to high
  delayusi2c(4);
  SCL_SET; 
  SDA_SET;//发送I2C总线结束信号
  delayusi2c(4);					
}
/*******************************************************************************
 * 名称: bool I2C0_WaitACK(void)
 * 功能: I2C等待应答信号到来
 * 形参: 无
 * 返回: TRUE：接收应答失败
         FALSE：接收应答成功
 * 说明: 无 
 ******************************************************************************/

u8 I2C0_WaitACK(void)
{
  u16 ucErrTime=0;
  I2C0_SDA_IN();      //SDA设置为输入  
delayusi2c(2);	
  SDA_SET;
  delayusi2c(1);	   
  SCL_SET;
  delayusi2c(5);	 
  while(SDA_PIN==1)
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      I2C0_Stop();
      return TRUE;
    }
  }
  SCL_RESET;//时钟输出0 	   
  return FALSE;  
}
/*******************************************************************************
 * 名称: void I2C0_ACK(void)
 * 功能: I2C应答函数
 * 形参: 无
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void I2C0_ACK(void)
{
  SCL_RESET;
  I2C0_SDA_OUT();
	delayusi2c(2);	
  SDA_RESET;
  delayusi2c(2);
  SCL_SET;
  delayusi2c(2);
  SCL_RESET;
}
/*******************************************************************************
 * 名称: void I2C0_NoACK(void)
 * 功能: I2C非应答函数
 * 形参: 无
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void I2C0_NoACK(void)
{
  SCL_RESET;
  I2C0_SDA_OUT();
	delayusi2c(2);	
  SDA_SET;
  delayusi2c(2);
  SCL_SET;
  delayusi2c(2);
  SCL_RESET;
}
/*******************************************************************************
 * 名称: void I2C0_WriteByte(INT8U dat)
 * 功能: I2C写字节函数
 * 形参: dat：待写入的字节
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void I2C0_WriteByte(INT8U dat)
{	
  INT8U i;
  I2C0_SDA_OUT();
delayusi2c(2);		
  SCL_RESET;//拉低时钟线开始数据传输
	      delayusi2c(2);	
  for(i=0;i<8;i++){	
    if(dat&0x80) SDA_SET; 	   
    else SDA_RESET;		        
      dat <<= 1;
      delayusi2c(2);
      SCL_SET;
      delayusi2c(2);
      SCL_RESET;
      delayusi2c(2);	
  }	
}
/*******************************************************************************
 * 名称: INT8U I2C0_ReadByte(INT8U ack)
 * 功能: I2C读字节函数
 * 形参: ack：ack = 1时，发送ACK,ack=0，发送NoACK
 * 返回: 返回读取到的数据
 * 说明: 无 
 ******************************************************************************/
INT8U I2C0_ReadByte(INT8U ack)
{
  INT8U i,temp = 0;
  
  I2C0_SDA_IN();
	      delayusi2c(2);	
  for(i=0;i<8;i++){
		
		/*  temp <<= 1; 
       SCL_SET;           //拉高时钟线
         delayus(5);             //延时
      if(GPIO_ReadInputPin(PORTD,SDA)) temp++;           //读数据               
       SCL_RESET;               //拉低时钟线
         delayus(5);            //延时
				*/
				
    SCL_RESET;
    delayusi2c(2);
    SCL_SET;
    temp <<= 1; 
	    delayusi2c(2);	
    if(SDA_PIN==1) temp++; 
    delayusi2c(1);         		
  }
  if(!ack) I2C0_NoACK();
  else I2C0_ACK();
	
	

	
  return temp;
}

#endif


