



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
//    CmmBuf[j] = crc % 0x100; //У��ʹӳ������𣬳���Ϊ���ȼ�3
 //   CmmBuf[j + 1] = crc / 0x100;
		
		
		crc_res=(crc&0xff)<<8;
		crc_res|=(crc&0xff00)>>8;
return crc_res;
}


//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	   //����д����
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}else MYIIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
	MYIIC_Wait_Ack(); 
    MYIIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	MYIIC_Wait_Ack();	    
	MYIIC_Start();  	 	   
	MYIIC_Send_Byte(0xA1);           //�������ģʽ			   
	MYIIC_Wait_Ack();	 
    temp=MYIIC_Read_Byte(0);		   
    MYIIC_Stop();//����һ��ֹͣ����	    
	return temp;	
	

}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(unsigned short WriteAddr,u8 DataToWrite)
{				   	  	    																 
  
    MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	    //����д����
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}else MYIIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	MYIIC_Wait_Ack();	   
    MYIIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	MYIIC_Wait_Ack(); 	 										  		   
	MYIIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	MYIIC_Wait_Ack();  		    	   
    MYIIC_Stop();//����һ��ֹͣ���� 
//	delay(10000);//10ms����
	
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte_Delay(unsigned short WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
  
    MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	    //����д����
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}else MYIIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	MYIIC_Wait_Ack();	   
    MYIIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	MYIIC_Wait_Ack(); 	 										  		   
	MYIIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	MYIIC_Wait_Ack();  		    	   
    MYIIC_Stop();//����һ��ֹͣ���� 
	delay(10000);//10ms����
	
}





//˫�����ݴ洢
//AT24C512
//64K
//0x8000

u16 crc_check_data=0;//���ݽ���
u16 crc_check_data_read=0;//���ݽ���
static uint8_t crc_check_buf[50];	//�洢




//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
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





 AT24CXX_Read_NByte(crc_check_buf,ReadAddr,(NumToRead));//��һ����
	
	
	crc_check_data_read=((crc_check_buf[NumToRead-2]&0xff)<<8);
	crc_check_data_read|=((crc_check_buf[NumToRead-1]&0xff));	
	
			crc_check_data=GetCrC(crc_check_buf,NumToRead-2);



/*
SFLASH_FastReadNByte(crc_check_buf,ReadAddr+DATA_SAVE_DOUBLE_ADD_ADDR,(nByte+2));//�ڶ�����	
	
	
	crc_check_data_read=((crc_check_buf[nByte]&0xff)<<8);
	crc_check_data_read|=((crc_check_buf[nByte+1]&0xff));	
	
			crc_check_data=GetCrC(crc_check_buf,nByte);
	
if(crc_check_data==crc_check_data_read)//������󣬶�ȡ�ڶ�������
{

memcpy(pBuffer,crc_check_buf,nByte);//���ݶ���

}
	*/
if(crc_check_data==crc_check_data_read)//������󣬶�ȡ�ڶ�������
{


memcpy(pBuffer,crc_check_buf,NumToRead-2);//���ݶ���





}else
{

AT24CXX_Read_NByte(crc_check_buf,ReadAddr+DATA_SAVE_DOUBLE_ADD_ADDR,(NumToRead));//�ڶ�����	
	
	
	crc_check_data_read=((crc_check_buf[NumToRead-2]&0xff)<<8);
	crc_check_data_read|=((crc_check_buf[NumToRead-1]&0xff));	
	
			crc_check_data=GetCrC(crc_check_buf,NumToRead-2);
	
if(crc_check_data==crc_check_data_read)//������󣬶�ȡ�ڶ�������
{

memcpy(pBuffer,crc_check_buf,NumToRead-2);//���ݶ���

}





}






}



//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write_NByte(unsigned char *pBuffer,unsigned short WriteAddr,unsigned short NumToWrite)//
{
	


	unsigned short uWrite_Length=0;//����
	unsigned short uWrite_Addr_Point=0;	//��ǰд���ַ
	
	
	
uWrite_Length=NumToWrite;
uWrite_Addr_Point=WriteAddr;


   MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	    //����д����
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(uWrite_Addr_Point>>8);//���͸ߵ�ַ	  
	}else MYIIC_Send_Byte(0XA0+((uWrite_Addr_Point/256)<<1));   //����������ַ0XA0,д���� 	 
	MYIIC_Wait_Ack();	   
    MYIIC_Send_Byte(uWrite_Addr_Point%256);   //���͵͵�ַ
	MYIIC_Wait_Ack(); 

while(uWrite_Length!= 0) 
{ 
				uWrite_Length--;
	
	
		 										  		   
	MYIIC_Send_Byte(*pBuffer++);     //�����ֽ�							   
	MYIIC_Wait_Ack();  		    	 	

			uWrite_Addr_Point++; //д���ַ��1 

			if(uWrite_Addr_Point % Page_Byte == 0) //�µ�һҳ
			{ 
						MYIIC_Stop();//����һ��ֹͣ���� 
						delay(10000);//10ms����// AT24C02��д��������5ms,��ʱһ��Ҫ���ڻ��ߵ���5ms 

  MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	    //����д����
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(uWrite_Addr_Point>>8);//���͸ߵ�ַ	  
	}else MYIIC_Send_Byte(0XA0+((uWrite_Addr_Point/256)<<1));   //����������ַ0XA0,д���� 	 
	MYIIC_Wait_Ack();	   
    MYIIC_Send_Byte(uWrite_Addr_Point%256);   //���͵͵�ַ
	MYIIC_Wait_Ack(); 

} 

} 

						MYIIC_Stop();//����һ��ֹͣ���� 
						delay(10000);//10ms����// AT24C02��д��������5ms,��ʱһ��Ҫ���ڻ��ߵ���5ms 


}



void AT24CXX_Write(unsigned char *pBuffer,unsigned short WriteAddr,unsigned short NumToWrite)//
{




memcpy(crc_check_buf,pBuffer,NumToWrite);
	
		crc_check_data=GetCrC(crc_check_buf,NumToWrite-2);

crc_check_buf[NumToWrite-2]=((crc_check_data&0xff00)>>8);
crc_check_buf[NumToWrite-1]=((crc_check_data&0xff));
	
	

 AT24CXX_Write_NByte(crc_check_buf,WriteAddr,(NumToWrite));//��һ����

 AT24CXX_Write_NByte(crc_check_buf,(WriteAddr+DATA_SAVE_DOUBLE_ADD_ADDR),(NumToWrite));//�ڶ�����	






}




//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���

void AT24CXX_Clear(void)//
{
	
	
	u16 uWrite_Addr_One=0;
	u16 uWrite_Addr_Point=32;	//
	u16 uWrite_Addr_Start=0;	//������ַ
	
	

	
	while(uWrite_Addr_Point<2048)
	{

//	uWrite_Addr_One=WriteAddr+uWrite_Addr_Point;
	if((uWrite_Addr_Point+16)>2048)	uWrite_Addr_One=(2048-uWrite_Addr_Point);
		else uWrite_Addr_One=16;
		

	uWrite_Addr_Start=0+uWrite_Addr_Point;
	    MYIIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		MYIIC_Send_Byte(0XA0);	    //����д����
		MYIIC_Wait_Ack();
		MYIIC_Send_Byte(uWrite_Addr_Start>>8);//���͸ߵ�ַ	  
	}else MYIIC_Send_Byte(0XA0+((uWrite_Addr_Start/256)<<1));   //����������ַ0XA0,д���� 	 
	MYIIC_Wait_Ack();	   
    MYIIC_Send_Byte(uWrite_Addr_Start%256);   //���͵͵�ַ
	MYIIC_Wait_Ack(); 
	while(uWrite_Addr_One--)
	{
		
		 										  		   
	MYIIC_Send_Byte(0x00);     //�����ֽ�							   
	MYIIC_Wait_Ack();  		    	 	
		


		
	}	
    MYIIC_Stop();//����һ��ֹͣ���� 
	delay(10000);//10ms����


	uWrite_Addr_Point+=16;

	}
	
	
	
	

}






























