#include "includes.h"	   
#include "crc32.h"




u16 GetCrC(unsigned char *CmmBuf, unsigned char Len)
{
    unsigned short crc = 0xFFFF;
    unsigned short crc_res = 0;	
	
    unsigned char i;
    unsigned char j;
	
	
	
    if(Len > 78) return 0;
    j = 0;
    while(j < Len)
    {
        crc ^= CmmBuf[j];
        for(i=0; i < 8; i++)
        {
            if(crc & 0x01)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
        j++;
    }
//    CmmBuf[j] = crc % 0x100; //У��ʹӳ������𣬳���Ϊ���ȼ�3
 //   CmmBuf[j + 1] = crc / 0x100;
		
		
		crc_res=(crc&0xff)<<8;
		crc_res|=(crc&0xff00)>>8;
return crc_res;
}


u16 crc_check=0;
u16 crc_check_send=0;


u16 uSensor_Data_Read_Addr_Start=0;//����ʼ��ַ
u16 uSensor_Data_Read_Addr_No=0;//���ļĴ�������
u16 uSensor_Data_Read_i=0;//���ļĴ�������,����


 unsigned char	uSensor_Data_Buf_Little_Endian[80];//С��ģʽ


void Uart2_Decode(void)
{


	
	
/*
	
	
	
ע��Ҫ4�ֽڶ��룬��Ȼ�������еĲ������ȡ����
	
	

	
		*/
	
	
	if(uUart2_Recive_Data[0]==0x12&&CPU_ID[3]==UID_KEY[3])
	{
	
		
		
		//����

	/*	uSensorData.sensor_data_struct.uSun_Radiation=0x2245;//̫���ܷ���

		uSensorData.sensor_data_struct.uAnion=0x22448855;//������ 
		uSensorData.sensor_data_struct.uIllumination=0x05;//����
		uSensorData.sensor_data_struct.uRadiation_Dosage=0x55667788;//�������
		uSensorData.sensor_data_struct.uDevice_Address=0x11223344;//�豸������ַ
		
		*/
		
		

		crc_check=GetCrC((unsigned char *)&uUart2_Recive_Data[0],(uUart2_Recive_Length-2));
		
		//С��ģʽ�������ܼ���Э��
		memcpy(&uSensor_Data_Buf_Little_Endian[0],&uSensorData.uSensor_Data_Buf[0],56);
		
		uSensor_Data_Buf_Little_Endian[56]=uSensorData.uSensor_Data_Buf[58];
		uSensor_Data_Buf_Little_Endian[57]=uSensorData.uSensor_Data_Buf[59];
		uSensor_Data_Buf_Little_Endian[58]=uSensorData.uSensor_Data_Buf[56];
		uSensor_Data_Buf_Little_Endian[59]=uSensorData.uSensor_Data_Buf[57];
		
		uSensor_Data_Buf_Little_Endian[60]=uSensorData.uSensor_Data_Buf[62];
		uSensor_Data_Buf_Little_Endian[61]=uSensorData.uSensor_Data_Buf[63];
		uSensor_Data_Buf_Little_Endian[62]=uSensorData.uSensor_Data_Buf[60];
		uSensor_Data_Buf_Little_Endian[63]=uSensorData.uSensor_Data_Buf[61];		
		
		uSensor_Data_Buf_Little_Endian[64]=uSensorData.uSensor_Data_Buf[66];
		uSensor_Data_Buf_Little_Endian[65]=uSensorData.uSensor_Data_Buf[67];
		uSensor_Data_Buf_Little_Endian[66]=uSensorData.uSensor_Data_Buf[64];
		uSensor_Data_Buf_Little_Endian[67]=uSensorData.uSensor_Data_Buf[65];		
		
		uSensor_Data_Buf_Little_Endian[68]=uSensorData.uSensor_Data_Buf[70];
		uSensor_Data_Buf_Little_Endian[69]=uSensorData.uSensor_Data_Buf[71];
		uSensor_Data_Buf_Little_Endian[70]=uSensorData.uSensor_Data_Buf[68];
		uSensor_Data_Buf_Little_Endian[71]=uSensorData.uSensor_Data_Buf[69];				
		
		
		memcpy(&uSensor_Data_Buf_Little_Endian[72],&uSensorData.uSensor_Data_Buf[72],2);		


		
		
		if(((uUart2_Recive_Data[uUart2_Recive_Length-2]<<8)|(uUart2_Recive_Data[uUart2_Recive_Length-1]))==crc_check)
		{
			
			
		if(uUart2_Recive_Data[1]==0x03&&uUart2_Recive_Data[2]==0x00&&uUart2_Recive_Data[4]==0x00)	
		{
			 uSensor_Data_Read_Addr_Start=uUart2_Recive_Data[3];//����ʼ��ַ
 uSensor_Data_Read_Addr_No=uUart2_Recive_Data[5];//���ļĴ�������
			
			
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
			uUart2_Send_Data[0]=0x12;
			uUart2_Send_Data[1]=0x03;
			uUart2_Send_Data[2]=uSensor_Data_Read_Addr_No*2;
			
			for(uSensor_Data_Read_i=0;uSensor_Data_Read_i<(uSensor_Data_Read_Addr_No*2);uSensor_Data_Read_i+=2) 
			{
				uUart2_Send_Data[3+uSensor_Data_Read_i]=uSensor_Data_Buf_Little_Endian[2*uSensor_Data_Read_Addr_Start+uSensor_Data_Read_i+1];
				uUart2_Send_Data[3+uSensor_Data_Read_i+1]=uSensor_Data_Buf_Little_Endian[2*uSensor_Data_Read_Addr_Start+uSensor_Data_Read_i];				
			}
	
			crc_check_send=GetCrC((unsigned char *)&uUart2_Send_Data[0],(uSensor_Data_Read_Addr_No*2)+3);
			uUart2_Send_Data[(uSensor_Data_Read_Addr_No*2)+3]=(crc_check_send&0xff00)>>8;
			uUart2_Send_Data[(uSensor_Data_Read_Addr_No*2)+3+1]=(crc_check_send&0xff);
			User_UART2_SendStringx((char *)&uUart2_Send_Data[0],(uSensor_Data_Read_Addr_No*2)+3+2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);					
			
			
		
		
		
	
	
		
	}
		
	
}
	
	}
	
	
	
		uUart2_Recive_Length=0;
	
	
	
	memset(uUart2_Recive_Data,0,50);
	
	
	
}


void User_Modbus_Task(void *pdata)//���д������������
{





		while(1)
	{
		


			if(uUart2_Recive_End>0)	
		{
		
		uUart2_Recive_End--;
		if(uUart2_Recive_End==0) 		  Uart2_Decode();//���ڽ��ս��� 

		
		}
		
		


		delay_ms(10);

	}




}

