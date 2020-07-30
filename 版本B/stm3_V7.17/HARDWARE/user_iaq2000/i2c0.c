/*******************************************************************************
 * �ļ���  ��i2c.c
 * ����    ��I2C����
 * ʵ��ƽ̨��
 * ��汾  ��V1.5.1
 * ����    ��liuyong
 *�޸�ʱ�� ��2014-05-07
*******************************************************************************/
#include "includes.h"	   

#include "i2c0.h"


#if 1

/*******************************************************************************
 * ����: void delayus(uint i)
 * ����: ��ʱ����
 * �β�: ��
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void delayusi2c(u32 dly)
{  
	     int i,j;
    for (i=0;i<dly;i++)   for (j=0;j<40;j++) ;


}

/*******************************************************************************
 * ����: void I2C0_Init(void)
 * ����: I2C��ʼ������
 * �β�: ��
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void I2C0_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;


		

	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��

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
    GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��

		GPIO_ResetBits(GPIOE,GPIO_Pin_0);
		GPIO_ResetBits(GPIOE,GPIO_Pin_1);			
	
	
	
}




void I2C0_SDA_OUT(void)
{

	
    GPIO_InitTypeDef GPIO_InitStructure;
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��




}

void  I2C0_SDA_IN(void)
{

	
    GPIO_InitTypeDef GPIO_InitStructure;
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//
    GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��



}
 
 
 

/*******************************************************************************
 * ����: void I2C0_Start(void)
 * ����: I2C��������
 * �β�: ��
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void I2C0_Start(void)
{
  I2C0_SDA_OUT();     //sda�����
	delayusi2c(2);	
  SDA_SET;	  	  
  SCL_SET;
  delayusi2c(4);
  SDA_RESET;//START:when CLK is high,DATA change form high to low 
  delayusi2c(4);
  SCL_RESET;//ǯסI2C���ߣ�׼�����ͻ�������� 
	  delayusi2c(4);
}
/*******************************************************************************
 * ����: void I2C0_stop(void)
 * ����: I2Cֹͣ����
 * �β�: ��
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void I2C0_Stop(void)
{
  I2C0_SDA_OUT();//sda�����
	delayusi2c(2);	
  SCL_RESET;
  SDA_RESET;//STOP:when CLK is high DATA change form low to high
  delayusi2c(4);
  SCL_SET; 
  SDA_SET;//����I2C���߽����ź�
  delayusi2c(4);					
}
/*******************************************************************************
 * ����: bool I2C0_WaitACK(void)
 * ����: I2C�ȴ�Ӧ���źŵ���
 * �β�: ��
 * ����: TRUE������Ӧ��ʧ��
         FALSE������Ӧ��ɹ�
 * ˵��: �� 
 ******************************************************************************/

u8 I2C0_WaitACK(void)
{
  u16 ucErrTime=0;
  I2C0_SDA_IN();      //SDA����Ϊ����  
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
  SCL_RESET;//ʱ�����0 	   
  return FALSE;  
}
/*******************************************************************************
 * ����: void I2C0_ACK(void)
 * ����: I2CӦ����
 * �β�: ��
 * ����: ��
 * ˵��: �� 
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
 * ����: void I2C0_NoACK(void)
 * ����: I2C��Ӧ����
 * �β�: ��
 * ����: ��
 * ˵��: �� 
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
 * ����: void I2C0_WriteByte(INT8U dat)
 * ����: I2Cд�ֽں���
 * �β�: dat����д����ֽ�
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void I2C0_WriteByte(INT8U dat)
{	
  INT8U i;
  I2C0_SDA_OUT();
delayusi2c(2);		
  SCL_RESET;//����ʱ���߿�ʼ���ݴ���
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
 * ����: INT8U I2C0_ReadByte(INT8U ack)
 * ����: I2C���ֽں���
 * �β�: ack��ack = 1ʱ������ACK,ack=0������NoACK
 * ����: ���ض�ȡ��������
 * ˵��: �� 
 ******************************************************************************/
INT8U I2C0_ReadByte(INT8U ack)
{
  INT8U i,temp = 0;
  
  I2C0_SDA_IN();
	      delayusi2c(2);	
  for(i=0;i<8;i++){
		
		/*  temp <<= 1; 
       SCL_SET;           //����ʱ����
         delayus(5);             //��ʱ
      if(GPIO_ReadInputPin(PORTD,SDA)) temp++;           //������               
       SCL_RESET;               //����ʱ����
         delayus(5);            //��ʱ
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


