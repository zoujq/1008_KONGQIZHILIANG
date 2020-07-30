



//#include "i2c.h"
#include "myiic1.h"
#include "includes.h"	   





_calendar_obj calendar;//ʱ�ӽṹ�� 







//��PCF8563ָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 PCF8563_ReadOneByte(unsigned int ReadAddr)
{				  
	u8 temp=0;		  	    																 
  MYIIC1_Start();  
	MYIIC1_Send_Byte(0XA2);   //����������ַ0XA2,д���� 	   
	MYIIC1_Wait_Ack(); 
    MYIIC1_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	MYIIC1_Wait_Ack();	    
	MYIIC1_Start();  	 	   
	MYIIC1_Send_Byte(0xA3);           //�������ģʽ			   
	MYIIC1_Wait_Ack();	 
    temp=MYIIC1_Read_Byte(0);		   
    MYIIC1_Stop();//����һ��ֹͣ����	    
	return temp;	
	

}
//��PCF8563ָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void PCF8563_WriteOneByte(unsigned int WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
  
  MYIIC1_Start();  
	MYIIC1_Send_Byte(0XA2);   //����������ַ0XA2,д���� 	 
	MYIIC1_Wait_Ack();	   
    MYIIC1_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	MYIIC1_Wait_Ack(); 	 										  		   
	MYIIC1_Send_Byte(DataToWrite);     //�����ֽ�							   
	MYIIC1_Wait_Ack();  		    	   
    MYIIC1_Stop();//����һ��ֹͣ���� 
//	delay(10000);//10ms����
	
}
//��PCF8563ָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void PCF8563_WriteOneByte_Delay(unsigned int WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
  
  MYIIC1_Start();  
	MYIIC1_Send_Byte(0XA2);   //����������ַ0XA2,д���� 	 
	MYIIC1_Wait_Ack();	   
    MYIIC1_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	MYIIC1_Wait_Ack(); 	 										  		   
	MYIIC1_Send_Byte(DataToWrite);     //�����ֽ�							   
	MYIIC1_Wait_Ack();  		    	   
    MYIIC1_Stop();//����һ��ֹͣ���� 
//	delay(10000);//10ms����
	
}

//��PCF8563�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void PCF8563_Read(unsigned int ReadAddr,unsigned char *pBuffer,unsigned int NumToRead)
{

	while(NumToRead)
	{
		 *pBuffer++=PCF8563_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}

	
}  
//��PCF8563�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void PCF8563_Write(unsigned int WriteAddr,unsigned char *pBuffer,unsigned int NumToWrite)//
{
	

	
	while(NumToWrite--)
	{
		PCF8563_WriteOneByte_Delay(WriteAddr,*pBuffer);

		WriteAddr++;
		pBuffer++;
	}
	

}


//----------------BCDת10����-----------
static unsigned char bcd_decimal(unsigned char bcd)
{
	return bcd-(bcd >> 4)*6;
}
//----------------10����תBCD-----------
static unsigned char decimal_bcd(unsigned char decimal)
{
	return (unsigned char) (decimal+(decimal / 10)*6);
}




//��ʼ��PCF8563
void PCF8563_Init(void)
{

 //MYIIC1_Init();                //��ʼ��IIC��IO��			


	//secc=PCF8563_ReadOneByte(0x02);
	//minn=PCF8563_ReadOneByte(0x03);
	
}


void PCF8563_Set_Time(void)
{
/*
Sys_Time.year=17;	
Sys_Time.month=7;	
Sys_Time.day=26;		
Sys_Time.hour=3;	
Sys_Time.min=1;	
Sys_Time.sec=10;	
	*/

	/*PCF8563_WriteOneByte(0x04,decimal_bcd(Sys_Time.hour));
PCF8563_WriteOneByte(0x03,decimal_bcd(Sys_Time.min));
PCF8563_WriteOneByte(0x02,decimal_bcd(Sys_Time.sec));
	
PCF8563_WriteOneByte(0x08,decimal_bcd(Sys_Time.year));
PCF8563_WriteOneByte(0x07,decimal_bcd(Sys_Time.month));
PCF8563_WriteOneByte(0x05,decimal_bcd(Sys_Time.day));	
	*/
	

}

void PCF8563_Get_Time(void)
{
/*
Sys_Time.hour=bcd_decimal(0x3f&PCF8563_ReadOneByte(0x04));	
Sys_Time.min=bcd_decimal(0x7f&PCF8563_ReadOneByte(0x03));	
Sys_Time.sec=bcd_decimal(0x7f&PCF8563_ReadOneByte(0x02));	
	
Sys_Time.year=bcd_decimal(0xff&PCF8563_ReadOneByte(0x08));		
Sys_Time.month=bcd_decimal(0x1f&PCF8563_ReadOneByte(0x07));	
Sys_Time.day=bcd_decimal(0x3f&PCF8563_ReadOneByte(0x05));	
	*/
	
}



//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 	

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	
	u16 temp2;
	
	u8 week_month,week_year,week_day;

week_month=month;
week_year=year-2000;
week_day=day;
if(week_month == 1 | week_month == 2)
{
week_month += 12;
--week_year;
}

temp2= (week_day + 2 * week_month + 3 * (week_month + 1) / 5 + week_year + week_year / 4 -week_year / 100 +week_year / 400) % 7 + 1;

return temp2;

	
}			  





//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
u8 RTC_Get(void)
{
	
	
calendar.hour=bcd_decimal(0x3f&PCF8563_ReadOneByte(0x04));	
calendar.min=bcd_decimal(0x7f&PCF8563_ReadOneByte(0x03));	
calendar.sec=bcd_decimal(0x7f&PCF8563_ReadOneByte(0x02));	
	
calendar.w_year=bcd_decimal(0xff&PCF8563_ReadOneByte(0x08))+2000;		
calendar.w_month=bcd_decimal(0x1f&PCF8563_ReadOneByte(0x07));	
calendar.w_date=bcd_decimal(0x3f&PCF8563_ReadOneByte(0x05));		
	
	
	return 0;
}	


//����ʱ��
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{

PCF8563_WriteOneByte(0x04,decimal_bcd(hour));
PCF8563_WriteOneByte(0x03,decimal_bcd(min));
PCF8563_WriteOneByte(0x02,decimal_bcd(sec));
	
PCF8563_WriteOneByte(0x08,decimal_bcd(syear-2000));
PCF8563_WriteOneByte(0x07,decimal_bcd(smon));
PCF8563_WriteOneByte(0x05,decimal_bcd(sday));	
	
		
	
	return 0;	    
}

