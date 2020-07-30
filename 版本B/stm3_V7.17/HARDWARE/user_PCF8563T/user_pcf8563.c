



//#include "i2c.h"
#include "myiic1.h"
#include "includes.h"	   





_calendar_obj calendar;//时钟结构体 







//在PCF8563指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 PCF8563_ReadOneByte(unsigned int ReadAddr)
{				  
	u8 temp=0;		  	    																 
  MYIIC1_Start();  
	MYIIC1_Send_Byte(0XA2);   //发送器件地址0XA2,写数据 	   
	MYIIC1_Wait_Ack(); 
    MYIIC1_Send_Byte(ReadAddr%256);   //发送低地址
	MYIIC1_Wait_Ack();	    
	MYIIC1_Start();  	 	   
	MYIIC1_Send_Byte(0xA3);           //进入接收模式			   
	MYIIC1_Wait_Ack();	 
    temp=MYIIC1_Read_Byte(0);		   
    MYIIC1_Stop();//产生一个停止条件	    
	return temp;	
	

}
//在PCF8563指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void PCF8563_WriteOneByte(unsigned int WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
  
  MYIIC1_Start();  
	MYIIC1_Send_Byte(0XA2);   //发送器件地址0XA2,写数据 	 
	MYIIC1_Wait_Ack();	   
    MYIIC1_Send_Byte(WriteAddr%256);   //发送低地址
	MYIIC1_Wait_Ack(); 	 										  		   
	MYIIC1_Send_Byte(DataToWrite);     //发送字节							   
	MYIIC1_Wait_Ack();  		    	   
    MYIIC1_Stop();//产生一个停止条件 
//	delay(10000);//10ms左右
	
}
//在PCF8563指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void PCF8563_WriteOneByte_Delay(unsigned int WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
  
  MYIIC1_Start();  
	MYIIC1_Send_Byte(0XA2);   //发送器件地址0XA2,写数据 	 
	MYIIC1_Wait_Ack();	   
    MYIIC1_Send_Byte(WriteAddr%256);   //发送低地址
	MYIIC1_Wait_Ack(); 	 										  		   
	MYIIC1_Send_Byte(DataToWrite);     //发送字节							   
	MYIIC1_Wait_Ack();  		    	   
    MYIIC1_Stop();//产生一个停止条件 
//	delay(10000);//10ms左右
	
}

//在PCF8563里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void PCF8563_Read(unsigned int ReadAddr,unsigned char *pBuffer,unsigned int NumToRead)
{

	while(NumToRead)
	{
		 *pBuffer++=PCF8563_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}

	
}  
//在PCF8563里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void PCF8563_Write(unsigned int WriteAddr,unsigned char *pBuffer,unsigned int NumToWrite)//
{
	

	
	while(NumToWrite--)
	{
		PCF8563_WriteOneByte_Delay(WriteAddr,*pBuffer);

		WriteAddr++;
		pBuffer++;
	}
	

}


//----------------BCD转10进制-----------
static unsigned char bcd_decimal(unsigned char bcd)
{
	return bcd-(bcd >> 4)*6;
}
//----------------10进制转BCD-----------
static unsigned char decimal_bcd(unsigned char decimal)
{
	return (unsigned char) (decimal+(decimal / 10)*6);
}




//初始化PCF8563
void PCF8563_Init(void)
{

 //MYIIC1_Init();                //初始化IIC的IO口			


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



//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 	

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日 
//返回值：星期号																						 
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





//得到当前的时间
//返回值:0,成功;其他:错误代码.
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


//设置时钟
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

