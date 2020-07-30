



//#include "i2c.h"
#include "myiic.h"
#include "user_at24cxx.h"
#include "includes.h"	  




 
 static void delay(u32 dly)
 {
	 
	     int i,j;
    for (i=0;i<dly;i++)   for (j=0;j<40;j++) ;

    

}


u16 GetCrC(unsigned char *CmmBuf, unsigned char Len)
{
    unsigned short crc = 0xFFFF;
    unsigned short crc_res = 0;	
	
    unsigned char i;
    unsigned char j;
	
	
	

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
//    CmmBuf[j] = crc % 0x100; //校验和从长度算起，长度为长度减3
 //   CmmBuf[j + 1] = crc / 0x100;
		
		
		crc_res=(crc&0xff)<<8;
		crc_res|=(crc&0xff00)>>8;
return crc_res;
}


//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	   //发送写命令
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	}else MYIIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	MYIIC_Wait_Ack(); 
    MYIIC_Send_Byte(ReadAddr%256);   //发送低地址
	MYIIC_Wait_Ack();	    
	MYIIC_Start();  	 	   
	MYIIC_Send_Byte(0xA1);           //进入接收模式			   
	MYIIC_Wait_Ack();	 
    temp=MYIIC_Read_Byte(0);		   
    MYIIC_Stop();//产生一个停止条件	    
	return temp;	
	

}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(unsigned short WriteAddr,u8 DataToWrite)
{				   	  	    																 
  
    MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	    //发送写命令
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}else MYIIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	MYIIC_Wait_Ack();	   
    MYIIC_Send_Byte(WriteAddr%256);   //发送低地址
	MYIIC_Wait_Ack(); 	 										  		   
	MYIIC_Send_Byte(DataToWrite);     //发送字节							   
	MYIIC_Wait_Ack();  		    	   
    MYIIC_Stop();//产生一个停止条件 
//	delay(10000);//10ms左右
	
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte_Delay(unsigned short WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
  
    MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	    //发送写命令
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}else MYIIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	MYIIC_Wait_Ack();	   
    MYIIC_Send_Byte(WriteAddr%256);   //发送低地址
	MYIIC_Wait_Ack(); 	 										  		   
	MYIIC_Send_Byte(DataToWrite);     //发送字节							   
	MYIIC_Wait_Ack();  		    	   
    MYIIC_Stop();//产生一个停止条件 
	delay(10000);//10ms左右
	
}





//双重数据存储
//AT24C512
//64K
//0x8000

u16 crc_check_data=0;//数据较验
u16 crc_check_data_read=0;//数据较验
static uint8_t crc_check_buf[50];	//存储




//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read_NByte(unsigned char *pBuffer,unsigned short ReadAddr,unsigned short NumToRead)
{

	while(NumToRead)
	{
		 *pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}

	
}  


void AT24CXX_Read(unsigned char *pBuffer,unsigned short ReadAddr,unsigned short NumToRead)
{





 AT24CXX_Read_NByte(crc_check_buf,ReadAddr,(NumToRead));//第一区域
	
	
	crc_check_data_read=((crc_check_buf[NumToRead-2]&0xff)<<8);
	crc_check_data_read|=((crc_check_buf[NumToRead-1]&0xff));	
	
			crc_check_data=GetCrC(crc_check_buf,NumToRead-2);



/*
SFLASH_FastReadNByte(crc_check_buf,ReadAddr+DATA_SAVE_DOUBLE_ADD_ADDR,(nByte+2));//第二区域	
	
	
	crc_check_data_read=((crc_check_buf[nByte]&0xff)<<8);
	crc_check_data_read|=((crc_check_buf[nByte+1]&0xff));	
	
			crc_check_data=GetCrC(crc_check_buf,nByte);
	
if(crc_check_data==crc_check_data_read)//如果错误，读取第二个备份
{

memcpy(pBuffer,crc_check_buf,nByte);//数据读出

}
	*/
if(crc_check_data==crc_check_data_read)//如果错误，读取第二个备份
{


memcpy(pBuffer,crc_check_buf,NumToRead-2);//数据读出





}else
{

AT24CXX_Read_NByte(crc_check_buf,ReadAddr+DATA_SAVE_DOUBLE_ADD_ADDR,(NumToRead));//第二区域	
	
	
	crc_check_data_read=((crc_check_buf[NumToRead-2]&0xff)<<8);
	crc_check_data_read|=((crc_check_buf[NumToRead-1]&0xff));	
	
			crc_check_data=GetCrC(crc_check_buf,NumToRead-2);
	
if(crc_check_data==crc_check_data_read)//如果错误，读取第二个备份
{

memcpy(pBuffer,crc_check_buf,NumToRead-2);//数据读出

}





}






}



//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write_NByte(unsigned char *pBuffer,unsigned short WriteAddr,unsigned short NumToWrite)//
{
	


	unsigned short uWrite_Length=0;//长度
	unsigned short uWrite_Addr_Point=0;	//当前写入地址
	
	
	
uWrite_Length=NumToWrite;
uWrite_Addr_Point=WriteAddr;


   MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	    //发送写命令
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(uWrite_Addr_Point>>8);//发送高地址	  
	}else MYIIC_Send_Byte(0XA0+((uWrite_Addr_Point/256)<<1));   //发送器件地址0XA0,写数据 	 
	MYIIC_Wait_Ack();	   
    MYIIC_Send_Byte(uWrite_Addr_Point%256);   //发送低地址
	MYIIC_Wait_Ack(); 

while(uWrite_Length!= 0) 
{ 
				uWrite_Length--;
	
	
		 										  		   
	MYIIC_Send_Byte(*pBuffer++);     //发送字节							   
	MYIIC_Wait_Ack();  		    	 	

			uWrite_Addr_Point++; //写入地址加1 

			if(uWrite_Addr_Point % Page_Byte == 0) //新的一页
			{ 
						MYIIC_Stop();//产生一个停止条件 
						delay(10000);//10ms左右// AT24C02的写入周期是5ms,延时一定要大于或者等于5ms 

  MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	    //发送写命令
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(uWrite_Addr_Point>>8);//发送高地址	  
	}else MYIIC_Send_Byte(0XA0+((uWrite_Addr_Point/256)<<1));   //发送器件地址0XA0,写数据 	 
	MYIIC_Wait_Ack();	   
    MYIIC_Send_Byte(uWrite_Addr_Point%256);   //发送低地址
	MYIIC_Wait_Ack(); 

} 

} 

						MYIIC_Stop();//产生一个停止条件 
						delay(10000);//10ms左右// AT24C02的写入周期是5ms,延时一定要大于或者等于5ms 


}



void AT24CXX_Write(unsigned char *pBuffer,unsigned short WriteAddr,unsigned short NumToWrite)//
{




memcpy(crc_check_buf,pBuffer,NumToWrite);
	
		crc_check_data=GetCrC(crc_check_buf,NumToWrite-2);

crc_check_buf[NumToWrite-2]=((crc_check_data&0xff00)>>8);
crc_check_buf[NumToWrite-1]=((crc_check_data&0xff));
	
	

 AT24CXX_Write_NByte(crc_check_buf,WriteAddr,(NumToWrite));//第一区域

 AT24CXX_Write_NByte(crc_check_buf,(WriteAddr+DATA_SAVE_DOUBLE_ADD_ADDR),(NumToWrite));//第二区域	






}




//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数

void AT24CXX_Clear(void)//
{
	
	
	u16 uWrite_Addr_One=0;
	u16 uWrite_Addr_Point=32;	//
	u16 uWrite_Addr_Start=0;	//启动地址
	
	

	
	while(uWrite_Addr_Point<2048)
	{

//	uWrite_Addr_One=WriteAddr+uWrite_Addr_Point;
	if((uWrite_Addr_Point+16)>2048)	uWrite_Addr_One=(2048-uWrite_Addr_Point);
		else uWrite_Addr_One=16;
		

	uWrite_Addr_Start=0+uWrite_Addr_Point;
	    MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	    //发送写命令
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(uWrite_Addr_Start>>8);//发送高地址	  
	}else MYIIC_Send_Byte(0XA0+((uWrite_Addr_Start/256)<<1));   //发送器件地址0XA0,写数据 	 
	MYIIC_Wait_Ack();	   
    MYIIC_Send_Byte(uWrite_Addr_Start%256);   //发送低地址
	MYIIC_Wait_Ack(); 
	while(uWrite_Addr_One--)
	{
		
		 										  		   
	MYIIC_Send_Byte(0x00);     //发送字节							   
	MYIIC_Wait_Ack();  		    	 	
		


		
	}	
    MYIIC_Stop();//产生一个停止条件 
	delay(10000);//10ms左右


	uWrite_Addr_Point+=16;

	}
	
	
	
	

}






























