#ifndef _PCF8563_H_
#define _PCF8563_H_






//ʱ��ṹ��
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//������������
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��
 


//��ʼ��IIC�ӿ�
void PCF8563_Init(void);
void PCF8563_Read(unsigned int ReadAddr,unsigned char *pBuffer,unsigned int NumToRead);
void PCF8563Write(unsigned int WriteAddr,unsigned char *pBuffer,unsigned int NumToWrite);

void PCF8563_WriteOneByte(unsigned int WriteAddr,unsigned char DataToWrite);

void	PCF8563_WriteOneByte_Delay(unsigned int WriteAddr,unsigned char DataToWrite);



void PCF8563_Set_Time(void);
void PCF8563_Get_Time(void);


u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�

//u8 RTC_Init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
u8 RTC_Get(void);         //����ʱ��   
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��		


u8 RTC_Get_Week(u16 year,u8 month,u8 day);


#endif




