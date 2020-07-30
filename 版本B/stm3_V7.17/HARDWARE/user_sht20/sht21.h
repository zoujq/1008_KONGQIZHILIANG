#ifndef _SHT21_H_
#define _SHT21_H_








 extern u8 uTime_No;//分时复用


//初始化IIC接口
void SHT21_UnInit(void);
void SHT21_Init(void);
void SHT21_Read(u16 ReadAddr,u8 *pBuffer);
void SHT21_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);
void User_SHT20_Task(void);//获取温湿度数据任务







#endif




