#ifndef _PCF8563_H_
#define _PCF8563_H_






//时间结构体
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体
 


//初始化IIC接口
void PCF8563_Init(void);
void PCF8563_Read(unsigned int ReadAddr,unsigned char *pBuffer,unsigned int NumToRead);
void PCF8563Write(unsigned int WriteAddr,unsigned char *pBuffer,unsigned int NumToWrite);

void PCF8563_WriteOneByte(unsigned int WriteAddr,unsigned char DataToWrite);

void	PCF8563_WriteOneByte_Delay(unsigned int WriteAddr,unsigned char DataToWrite);



void PCF8563_Set_Time(void);
void PCF8563_Get_Time(void);


u8 Is_Leap_Year(u16 year);//平年,闰年判断

//u8 RTC_Init(void);        //初始化RTC,返回0,失败;1,成功;
u8 RTC_Get(void);         //更新时间   
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间		


u8 RTC_Get_Week(u16 year,u8 month,u8 day);


#endif




