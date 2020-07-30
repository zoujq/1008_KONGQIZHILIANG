



#include "myiic2.h"
#include "includes.h"	   


u8 uTime_No=0;//��ʱ����
unsigned short temp_charge_time_10min_no;//�¶Ƚ��������ϳ��ʱ��	
unsigned short temp_charge_time_30min_no;//�¶Ƚ��������ϳ��ʱ��	
unsigned short temp_charge_time_out_10min_no;//�¶Ƚ�����������Ƴ�ʱ��

unsigned short temp_buf;//�¶Ȼ���
	
	
	
//��ʼ��IIC�ӿ�
void SHT21_Init(void)
{

	
	 uTime_No=0;//����
	temp_charge_time_10min_no=0;//����
	temp_charge_time_30min_no=0;//����	
	temp_charge_time_out_10min_no=0;//����	
	
	CMachine.temp=40;//�տ�ʼ��ʾ0��
	
 MYIIC2_Init();                //��ʼ��IIC��IO��			


}

//��ʼ��IIC�ӿ�
void SHT21_UnInit(void)
{

 MYIIC2_UnInit();                //��ʼ��IIC��IO��			


}



//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 SHT21_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    MYIIC2_Start();  
	MYIIC2_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
	MYIIC2_Wait_Ack(); 
    MYIIC2_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	MYIIC2_Wait_Ack();	    
	MYIIC2_Start();  	 	   
	MYIIC2_Send_Byte(0XA1);           //�������ģʽ			   
	MYIIC2_Wait_Ack();	 
    temp=MYIIC2_Read_Byte(0);		   
    MYIIC2_Stop();//����һ��ֹͣ����	    
	return temp;	
	

}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void SHT21_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
  
    MYIIC2_Start();  
	MYIIC2_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	MYIIC2_Wait_Ack();	   
    MYIIC2_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	MYIIC2_Wait_Ack(); 	 										  		   
	MYIIC2_Send_Byte(DataToWrite);     //�����ֽ�							   
	MYIIC2_Wait_Ack();  		    	   
    MYIIC2_Stop();//����һ��ֹͣ���� 
//	delay(10000);//10ms����
	
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���

void SHT21_Read(u16 ReadAddr,u8 *pBuffer)
{


	if(uTime_No==0||uTime_No==2)  
	{		
    MYIIC2_Start();  
	MYIIC2_Send_Byte(0x80);   //
	MYIIC2_Wait_Ack(); 	
	MYIIC2_Send_Byte(ReadAddr);   //
	MYIIC2_Wait_Ack(); 
	}


	if(uTime_No==1||uTime_No==3)  
	{		
	MYIIC2_Start();  	 	   
	MYIIC2_Send_Byte(0X81);           //�������ģʽ			   
	MYIIC2_Wait_Ack();	 

    (*pBuffer++)=MYIIC2_Read_Byte(1);		
	   (*pBuffer++)=MYIIC2_Read_Byte(1);	
	   (*pBuffer++)=MYIIC2_Read_Byte(0);	
    MYIIC2_Stop();//����һ��ֹͣ����	    
	}
	

	
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void SHT21_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)//
{
	

	
	while(NumToWrite--)
	{
		SHT21_WriteOneByte(WriteAddr,*pBuffer);

		WriteAddr++;
		pBuffer++;
	}
	

}









signed int   uHumidity_Inside=0;//ʪ��
signed int    uTemp_Inside=0;//�¶�

u8  f_SHT20_Init=0;//




void User_SHT20_Task(void)
{

u8 buf[3]={0};

	

if(CMachine.uTVOC_Res_Start_Time_No!=0)	
{
	
 uTime_No=0;//����

	
		temp_charge_time_10min_no=0;//����
	temp_charge_time_30min_no=0;//����
	temp_charge_time_out_10min_no=0;//����	
	
 SHT21_UnInit();	
	
	return;
}else
{




}


/*
1����ʪ��У׼��ʽ��



S=�¶����ֵ��

X=�¶���ʾֵ��


X=10/11*(S-3)


�豸���ʱ���¶Ȼ���������������¶Ȼ��½���


�趨��ʼ���10���Ӻ�X=10/11*(S-4),  ���30���Ӻ�X=10/11*(S-5) ��

�����������߳����ֹ�󣬹�10���ӣ�X=10/11*(S-3)��


usb ��ͷ���ڳ��ͷ�ϣ��豸���ܴ��ڳ��״̬��Ҳ������δ���״̬��Ҫ�����豸���������ж��Ƿ��ڳ�硣




ʪ�������� ԭ����+ 18�������޸�Ϊ+8 ���ɣ�



��ʪ��У׼��
�¶�У׼��������6��1��ʾ���棬������5�¿��ؼ����¶���������M������1�ȣ�������������¶�1��

ʪ��У׼��������6��1��ʾ���棬������6�¿��ؼ���ʪ����������M������ʪ��1%�������������ʪ��1%



*/



	if(CMachine.fCharge==1&&CMachine.Battery<100)//���״̬
	{
		if(temp_charge_time_10min_no>600)	
		{
			temp_charge_time_10min_no=600;
			temp_charge_time_out_10min_no=600;//�ѵ�10����
		}else	temp_charge_time_10min_no++;
		

	}else
	{

		if(temp_charge_time_30min_no>0)	temp_charge_time_30min_no--;		
		if(temp_charge_time_10min_no>0)	temp_charge_time_10min_no--;
		if(temp_charge_time_out_10min_no>0)	temp_charge_time_out_10min_no--;	
	
	
	}





	if(uTime_No==0||uTime_No==1)
	{
	SHT21_Read(0xf5,buf);//ʪ��
if(uTime_No==1)	
{
	uHumidity_Inside=(-6+125*((buf[0]*256+buf[1])/65536.0))*10;
		CMachine.humidity=uHumidity_Inside;//ʪ��
	
		CMachine.humidity+=80;
		CMachine.humidity/=10;	
}
	}
		if(uTime_No==2||uTime_No==3)
	{
	SHT21_Read(0xf3,buf);//�¶�
if(uTime_No==3)	
{
	uTemp_Inside=(-46.85+175.72*((buf[0]*256+buf[1])/65536.0))*10;
	if(uTemp_Inside<-400)	uTemp_Inside=(-400);	
  temp_buf=(uTemp_Inside+400);//�¶�	

//	CMachine.pm25=temp_buf;
	
	
	
	if(temp_buf>=650)	temp_buf-=40;
	else	if(temp_buf>=600)	temp_buf-=45;
	else	if(temp_buf>=550)	temp_buf-=50;		
	else	if(temp_buf>=450)	temp_buf-=55;
	else		temp_buf-=60;
	
	


		if(temp_charge_time_out_10min_no>0)		
		{
			if(temp_buf>10) CMachine.temp=(temp_buf-10);//�¶�
			else	CMachine.temp=0;//�¶�		
		}else
		{
			
			if(temp_charge_time_10min_no>=600)
			{
				if(temp_buf>10) CMachine.temp=(temp_buf-10);//�¶�
				else	CMachine.temp=0;//�¶�						
			}else
			{
				CMachine.temp=temp_buf;//�¶�			
			}

			
		
		}	

		
  CMachine.temp/=10;
	
	
}
	}
	
	

/*
1����ʪ��У׼��ʽ��



S=�¶����ֵ��

X=�¶���ʾֵ��


X=10/11*(S-7)


�豸���ʱ���¶Ȼ���������������¶Ȼ��½���


�趨��ʼ���10���Ӻ�X=10/11*(S-8),  ���30���Ӻ�X=10/11*(S-9) ��

�����������߳����ֹ�󣬹�10���ӣ�X=10/11*(S-7)��


usb ��ͷ���ڳ��ͷ�ϣ��豸���ܴ��ڳ��״̬��Ҳ������δ���״̬��Ҫ�����豸���������ж��Ƿ��ڳ�硣




ʪ�������� ԭ����+ 18�������޸�Ϊ+8 ���ɣ�





	if(CMachine.fCharge==1&&CMachine.Battery<100)//���״̬
	{
		if(temp_charge_time_10min_no>600)	
		{
			temp_charge_time_10min_no=600;
			temp_charge_time_out_10min_no=600;//�ѵ�10����
		}else	temp_charge_time_10min_no++;
		
		if(temp_charge_time_30min_no>1800)	temp_charge_time_30min_no=1800;		
		else	temp_charge_time_30min_no++;
		
	}else
	{

		if(temp_charge_time_30min_no>0)	temp_charge_time_30min_no--;		
		if(temp_charge_time_10min_no>0)	temp_charge_time_10min_no--;
		if(temp_charge_time_out_10min_no>0)	temp_charge_time_out_10min_no--;	
	
	
	}





	if(uTime_No==0||uTime_No==1)
	{
	SHT21_Read(0xf5,buf);//ʪ��
if(uTime_No==1)	
{
	uHumidity_Inside=(-6+125*((buf[0]*256+buf[1])/65536.0))*10;
		CMachine.humidity=uHumidity_Inside;//ʪ��
	
		CMachine.humidity+=80;
		CMachine.humidity/=10;	
}
	}
		if(uTime_No==2||uTime_No==3)
	{
	SHT21_Read(0xf3,buf);//�¶�
if(uTime_No==3)	
{
	uTemp_Inside=(-46.85+175.72*((buf[0]*256+buf[1])/65536.0))*10;
	if(uTemp_Inside<-400)	uTemp_Inside=(-400);	
  temp_buf=(uTemp_Inside+400);//�¶�	

	CMachine.pm25=temp_buf;
	
	if(temp_charge_time_10min_no<600)	
	{
		if(temp_charge_time_out_10min_no>0)		
		{
			if(temp_buf>80) CMachine.temp=10/11.0*(temp_buf-80);//�¶�
			else	CMachine.temp=0;//�¶�		
		}else
		{
			if(temp_buf>70) CMachine.temp=10/11.0*(temp_buf-70);//�¶�
			else	CMachine.temp=0;//�¶�
		
		}	
	}else
	{
		if(temp_charge_time_30min_no<1800)		
		{
			if(temp_buf>80) CMachine.temp=10/11.0*(temp_buf-80);//�¶�
			else	CMachine.temp=0;//�¶�		
		}else
		{
			if(temp_buf>90) CMachine.temp=10/11.0*(temp_buf-90);//�¶�
			else	CMachine.temp=0;//�¶�
		
		}
	}
		
  CMachine.temp/=10;
	
	
}
	}



	
*/
		


	
	


	

	


	
	
	


	
	
	/*
if(uSensorData.sensor_data_struct.uTemp>=400&&uSensorData.sensor_data_struct.uTemp<=500)//�¶�0-10��      �¶�-1  ʪ��+3
{
	uSensorData.sensor_data_struct.uTemp-=10;
	uSensorData.sensor_data_struct.uHumidity+=30;
}else
if(uSensorData.sensor_data_struct.uTemp>500&&uSensorData.sensor_data_struct.uTemp<=600)//�¶�10-20��     �¶�-2  ʪ��+4
{
	uSensorData.sensor_data_struct.uTemp-=20;
	uSensorData.sensor_data_struct.uHumidity+=40;
}else	
if(uSensorData.sensor_data_struct.uTemp>600&&uSensorData.sensor_data_struct.uTemp<=650)//�¶� 20-25��      �¶�-2.5 ʪ��+5
{
	uSensorData.sensor_data_struct.uTemp-=25;
	uSensorData.sensor_data_struct.uHumidity+=50;
}else	
if(uSensorData.sensor_data_struct.uTemp>650&&uSensorData.sensor_data_struct.uTemp<=700)//�¶� 25-30��      �¶�-3  ʪ��+6
{
	uSensorData.sensor_data_struct.uTemp-=30;
	uSensorData.sensor_data_struct.uHumidity+=60;
}else	
if(uSensorData.sensor_data_struct.uTemp>700)//�¶ȴ���30��    �¶�-4��ʪ��+7
{
	uSensorData.sensor_data_struct.uTemp-=40;
	uSensorData.sensor_data_struct.uHumidity+=70;
}	

	*/






			uTime_No++;
			if(uTime_No>3)	uTime_No=0;
			





}


















