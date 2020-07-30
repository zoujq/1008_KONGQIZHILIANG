#ifndef _AT24CXX_H_
#define _AT24CXX_H_




#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
#define AT24C512	65535  
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
//AT24C512
#define EE_TYPE AT24C512
#define Page_Byte	128 //页字节128个





 


//初始化IIC接口
void AT24CXX_Init(void);
void AT24CXX_Read(unsigned char *pBuffer,unsigned short ReadAddr,unsigned short NumToRead);
void AT24CXX_Write(unsigned char *pBuffer,unsigned short WriteAddr,unsigned short NumToWrite);

void AT24CXX_WriteOneByte(unsigned short WriteAddr,unsigned char DataToWrite);

void	AT24CXX_WriteOneByte_Delay(unsigned short WriteAddr,unsigned char DataToWrite);
//void AT24CXX_Write_More(unsigned short WriteAddr,unsigned char *pBuffer,unsigned short NumToWrite);//
void AT24CXX_Clear(void);//

unsigned short GetCrC(unsigned char *CmmBuf, unsigned char Len);







#endif




