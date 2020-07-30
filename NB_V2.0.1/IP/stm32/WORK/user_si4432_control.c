#include "includes.h"	   




// 0321 ע��1��SPI ����ʱΪ�͵�ƽ���ڶ���������Ч			 //ע��2��SDNֱ�ӽ�DGND����
//      ע��3��SI4432��STM32Ҫ��ͬһ��VCC3.3����Ļ��յ�ȫΪ0
#define	SI4432_NSEL   PCout(5)  //SI4432_NSEL  ʹ��
#define	SI4432_SDN    PBout(1)  //SI4432_SDN   ʹ��
#define	SI4432_NIRQ   PBin(0)  //SI4432_NIRQ  �ж�״̬



// ��ȡ�Ĵ���
u8  SI4432_ReadReg(u8  addr)   //��ȡ�Ĵ�����ֵ
{
	uint8_t temp=0;
	
	SI4432_NSEL=0;
	SPI1_ReadWriteByte(addr);    //���Ͷ�ȡ�Ĵ�����ַ
	temp=SPI1_ReadWriteByte(0Xff);
	SI4432_NSEL=1;
	
	return temp;
}
//д�Ĵ���
void SI4432_WriteReg(u8 addr, u8 value)  //д�Ĵ���
{
	SI4432_NSEL=0;
	SPI1_ReadWriteByte(addr|0x80);  
	SPI1_ReadWriteByte(value);    
	SI4432_NSEL=1;
}



u8 ItStatus1,ItStatus2,res;
void SI4432_Init(void)
{

	
	
  	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );//A,Cʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  // PC5 = NSEL ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,GPIO_Pin_5);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  // PB1 =SI_SDN    ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	//GPIO_SetBits(GPIOB,GPIO_Pin_1);	
 	GPIO_ResetBits(GPIOB,GPIO_Pin_1);//���ر�ģʽ�� SDN=0,�� SDN=1 оƬ����ȫ�رգ��Ĵ����������ݽ���ʧ

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // PB0=SI_NIRQ �����ж�ʱΪ��   
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //��������	  //���գ� GPIO_Mode_IN_FLOATING
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI1_Init();		   	//��ʼ��SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_256); 	
	

/////////////////////////////////////////////////
	SI4432_NSEL=0;  
	delay_ms(600);	

	res = SI4432_ReadReg(0x00);
	delay_ms(20); 
	res = SI4432_ReadReg(0x01); 
	delay_ms(20); 
	res = SI4432_ReadReg(0x02); 	
	delay_ms(20); 	
	
	delay_ms(1000);

//��ȡ�ж�״̬ 	 ����ж�  �ͷ�	 SI4432_NIRQ
	ItStatus1 = SI4432_ReadReg(0x03);  
	ItStatus2 = SI4432_ReadReg(0x04);  	 		
//�����λ
	SI4432_WriteReg(0x07, 0x80); //��0X07��ַ  д��0X80  �����λ
	 while ( SI4432_NIRQ == 1);       
	//��ȡ�ж�״̬ 	 ����ж�  �ͷ�	 SI4432_NIRQ
	ItStatus1 = SI4432_ReadReg(0x03);  
	ItStatus2 = SI4432_ReadReg(0x04);  
/////////////////////////��ʼ����
// Ƶ������ 434			
	//SI4432_WriteReg(0x75, 0x53);  
	//SI4432_WriteReg(0x76, 0x64);  // 
	//SI4432_WriteReg(0x77, 0x00); 
	SI4432_WriteReg(0x75, 0x57);  //470
	SI4432_WriteReg(0x76, 0x00);  
	SI4432_WriteReg(0x77, 0x00); 	
	
// 1.2K bps ��������	
//	SI4432_WriteReg(0x2a, 0x14);
	//SI4432_WriteReg(0x6e, 0x09);//1.2
	//SI4432_WriteReg(0x6f, 0xd5);
	SI4432_WriteReg(0x6e, 0x27);//4.8
	SI4432_WriteReg(0x6f, 0x52);	
	
	SI4432_WriteReg(0x70, 0x2d);// 

//SpiWriteRegister(0x72, 0x48);	//(9.6kbps)	
  //  SI4432_WriteReg(0x72, 0x38);	//Ƶ��ƫ��(1.2kbps)	
	SI4432_WriteReg(0x72, 0x30);	//Ƶ��ƫ��(30kbps)		
// ��������ø���Silabs ��Excel	(9.6 kbps, deviation: 45 kHz, channel filter BW: 102.2 kHz
	/*
	SI4432_WriteReg(0x1C, 0x1b);															//write 0x1E to the IF Filter Bandwidth register		
	SI4432_WriteReg(0x20, 0x83);															//write 0xD0 to the Clock Recovery Oversampling Ratio register		
	SI4432_WriteReg(0x21, 0xc0);															//write 0x00 to the Clock Recovery Offset 2 register		
	SI4432_WriteReg(0x22, 0x13);															//write 0x9D to the Clock Recovery Offset 1 register		
	SI4432_WriteReg(0x23, 0xa9);															//write 0x49 to the Clock Recovery Offset 0 register		
	SI4432_WriteReg(0x24, 0x00);															//write 0x00 to the Clock Recovery Timing Loop Gain 1 register		
	SI4432_WriteReg(0x25, 0x03);															//write 0x24 to the Clock Recovery Timing Loop Gain 0 register		
	SI4432_WriteReg(0x1D, 0x40);															//write 0x40 to the AFC Loop Gearshift Override register		
	SI4432_WriteReg(0x1E, 0x0A);															//write 0x0A to the AFC Timing Control register		
	SI4432_WriteReg(0x2A, 0x14);															//write 0x20 to the AFC Limiter register				
	*/
	
	SI4432_WriteReg(0x1C, 0x1A);															//write 0x1E to the IF Filter Bandwidth register		
	SI4432_WriteReg(0x20, 0xA1);															//write 0xD0 to the Clock Recovery Oversampling Ratio register		
	SI4432_WriteReg(0x21, 0x20);															//write 0x00 to the Clock Recovery Offset 2 register		
	SI4432_WriteReg(0x22, 0x4E);															//write 0x9D to the Clock Recovery Offset 1 register		
	SI4432_WriteReg(0x23, 0xA5);															//write 0x49 to the Clock Recovery Offset 0 register		
	SI4432_WriteReg(0x24, 0x00);															//write 0x00 to the Clock Recovery Timing Loop Gain 1 register		
	SI4432_WriteReg(0x25, 0x1B);															//write 0x24 to the Clock Recovery Timing Loop Gain 0 register		
	SI4432_WriteReg(0x1D, 0x44);															//write 0x40 to the AFC Loop Gearshift Override register		
	SI4432_WriteReg(0x1E, 0x0A);															//write 0x0A to the AFC Timing Control register		
	SI4432_WriteReg(0x1F, 0x03);		
	SI4432_WriteReg(0x2A, 0x21);															//write 0x20 to the AFC Limiter register			
	
	
//ǰ���� ͬ����
	/*
	SI4432_WriteReg(0x34, 0X0A);  // ����5�ֽڵ�Preamble
	SI4432_WriteReg(0x35, 0x2A);  // ��Ҫ��� Preamble							
	SI4432_WriteReg(0x33, 0x02);// ͬ����3,2 ��ͬ����
	SI4432_WriteReg(0x36, 0x2d);  // ͬ����Ϊ 0x2dd4
	SI4432_WriteReg(0x37, 0xd4);
	SI4432_WriteReg(0x30, 0x8D);   // ʹ��PH+ FIFOģʽ����λ��ǰ�棬ʹ��CRCУ��	CCITTT 	
    SI4432_WriteReg(0x32, 0x00 );//	��ֹ֡ͷ
	SI4432_WriteReg(0x71, 0x63); // ���䲻��Ҫ CLK��FiFo �� FSKģʽ	
*/
//	SI4432_WriteReg(0x34, 0X08);  // ����4�ֽڵ�Preamble
//	SI4432_WriteReg(0x35, 0x22);  // ��Ҫ��� Preamble							
//	SI4432_WriteReg(0x33, 0x22);// ͬ����3,2 ��ͬ����
//	SI4432_WriteReg(0x36, 0x53);  // ͬ����Ϊ 0x2dd4


 /*set the packet structure and the modulation type*/
  //set the preamble length to 4bytes
  SI4432_WriteReg(0x34, 0x08);                 
   
  //set preamble detection threshold to 16bits
  SI4432_WriteReg(0x35, 0x22);
  
  //Set the header Header 3 and 2, set variable packet length (the length of the payload is defined by the
  //received packet length field of the packet), set the synch word Synchronization Word 3 and 2;
  SI4432_WriteReg(0x33, 0x42); 
  //Set the sync word pattern to 0x5352
  SI4432_WriteReg(0x36, 0x53);


	SI4432_WriteReg(0x37, 0x52);
	SI4432_WriteReg(0x38, 0x00);
	SI4432_WriteReg(0x39, 0x00);	
	
	SI4432_WriteReg(0x3F, (SYS_TYPE&0xff));	
	SI4432_WriteReg(0x40, ((SYS_TYPE_NO&0xff0000)>>16));	
	SI4432_WriteReg(0x41, ((SYS_TYPE_NO&0xff00)>>8));	
	SI4432_WriteReg(0x42, ((SYS_TYPE_NO&0xff)>>0));		
	
	SI4432_WriteReg(0x43, 0xFF);			
	SI4432_WriteReg(0x44, 0xFF);			
	SI4432_WriteReg(0x45, 0xFF);			
	SI4432_WriteReg(0x46, 0xFF);		
	
	
	SI4432_WriteReg(0x3A, (SYS_TYPE&0xff));		//����֡ͷ
	SI4432_WriteReg(0x3B, ((SYS_TYPE_NO&0xff0000)>>16));		
	SI4432_WriteReg(0x3C, ((SYS_TYPE_NO&0xff00)>>8));	
	SI4432_WriteReg(0x3D, ((SYS_TYPE_NO&0xff)>>0));		
	
	


	SI4432_WriteReg(0x30, 0x8D);   // ʹ��PH+ FIFOģʽ����λ��ǰ�棬ʹ��CRCУ��	CCITTT 	
  SI4432_WriteReg(0x32, 0x0F);//	��ֹ֡ͷ
	SI4432_WriteReg(0x71, 0x23); // ���䲻��Ҫ CLK��FiFo �� FSKģʽ	


//GPIO
	SI4432_WriteReg(0x0C, 0x12); //  
	SI4432_WriteReg(0x0D, 0x15); // 
//��������
	SI4432_WriteReg(0x09, 0xD7);  //  ���ص��� 
	SI4432_WriteReg(0x69, 0x60);  //AGC����	 
	
	SI4432_WriteReg(0x58, 0x80);	
	
//���书��
	SI4432_WriteReg(0x6d, 0x07);
//�ֶ��򿪽���
//	SI4432_WriteReg(0x07, 0x05);
//�� �����ж�
	SI4432_WriteReg(0x05, 0x03); 
	SI4432_WriteReg(0x06, 0x00); 
//���ж�
	ItStatus1 = SI4432_ReadReg(0x03);	  //read the Interrupt Status1 register
	ItStatus2 = SI4432_ReadReg(0x04);	  //read the Interrupt Status2 register		
	
//��������
	SI4432_WriteReg(0x08, 0x03); //�巢�䡢���ջ�����
	SI4432_WriteReg(0x08, 0x00);	//�巢�䡢���ջ�����
	SI4432_WriteReg(0x07, 0x05);//�򿪽���	
//	SI4432_WriteReg(0x0d, 0xf4);//GPIO2��������





}
void User_SI4432_Pair_ID(void)//�ı�Ϊ����ͷ��
{
	SI4432_WriteReg(0x3F, 0x21);	
	SI4432_WriteReg(0x40, 0x21);	
	SI4432_WriteReg(0x41, 0x21);	
	SI4432_WriteReg(0x42, 0x21);		
	
	SI4432_WriteReg(0x3A, 0x21);	//����֡ͷ
	SI4432_WriteReg(0x3B, 0x21);	
	SI4432_WriteReg(0x3C, 0x21);	
	SI4432_WriteReg(0x3D, 0x21);			
	
}


void User_SI4432_Data_ID(void)//�ı�Ϊ���ݷ���ͷ��
{
	SI4432_WriteReg(0x3F, (SYS_TYPE&0xff));		//����֡ͷ
	SI4432_WriteReg(0x40, ((SYS_TYPE_NO&0xff0000)>>16));		
	SI4432_WriteReg(0x41, ((SYS_TYPE_NO&0xff00)>>8));	
	SI4432_WriteReg(0x42, ((SYS_TYPE_NO&0xff)>>0));		
	
	SI4432_WriteReg(0x3A, (SYS_TYPE&0xff));		//����֡ͷ
	SI4432_WriteReg(0x3B, ((SYS_TYPE_NO&0xff0000)>>16));		
	SI4432_WriteReg(0x3C, ((SYS_TYPE_NO&0xff00)>>8));	
	SI4432_WriteReg(0x3D, ((SYS_TYPE_NO&0xff)>>0));		
	
}





u8 User_CheckSum_Make(u8*pstr,u8 length)
{
u8 uCheck=0;
	u8 uNo=0;
	for(uNo=0;uNo<length;uNo++)
	{
		uCheck+=*pstr;
		pstr++;
	}
	uCheck=0x100-uCheck;
	return uCheck;
}



unsigned short UI_Show_No_B_SI4432_Task=0xff;//

unsigned short uSI4432_Work_Delay=0;//RF������ʱ��������Ϊ����ʹ�����һ��Ҫ������ �ط� ��
unsigned short uSI4432_Data_Send_Time_1S_No=0;//�������ݷ���ʱ��������



unsigned short uSI4432_Pair_Data_Send_Time_1S_No=0;//�������ʱ�䶨ʱ��1����


u8 uMaster_Send_Data[25]={0};//���͵�����


void User_SI4432_Task(void *pdata)
{







    GPIO_InitTypeDef  GPIO_InitStructure;  		  

	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTCʱ��

	
	
	
	
				
		//��Դʹ�ܽ�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��
		//GPIO_ResetBits(GPIOA,GPIO_Pin_12);    		
		GPIO_SetBits(GPIOA,GPIO_Pin_12);  //��				
				
				
				

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��
		GPIO_ResetBits(GPIOC,GPIO_Pin_4);		//MCU-SPI_SCLK    				
		GPIO_ResetBits(GPIOC,GPIO_Pin_5);		//MCU-SPI_NSEL-CS1


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);		//MCU-SPI_NIRQ_INT-OUT
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);		//MCU-SPI_SDN					
				
				
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);		//MCU-SPI_SDO-MISO
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);		//MCU-SPI_SDI-MOSI
								
	


	
	
	while(1)
	{			  
		
		
					 		delay_ms(10);
					continue;//�˳�
		
		
		////////////////////////////////////////    ����RF    ////////////////////////////////////////
		
		
		
		if(CMachine.UI_Show_No<=1)
		{
			
			if(UI_Show_No_B_SI4432_Task!=0)
			{

				

				
		//��Դʹ�ܽ�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��
		//GPIO_ResetBits(GPIOA,GPIO_Pin_12);    		
		GPIO_SetBits(GPIOA,GPIO_Pin_12);  //��				
				
				
				

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��
		GPIO_ResetBits(GPIOC,GPIO_Pin_4);		//MCU-SPI_SCLK    				
		GPIO_ResetBits(GPIOC,GPIO_Pin_5);		//MCU-SPI_NSEL-CS1


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);		//MCU-SPI_NIRQ_INT-OUT
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);		//MCU-SPI_SDN					
				
				
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);		//MCU-SPI_SDO-MISO
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);		//MCU-SPI_SDI-MOSI
								
				
				

					
				
			uSI4432_Work_Delay=200;//2��	
				
				
			UI_Show_No_B_SI4432_Task=0;
			}
			 		delay_ms(10);
					continue;//�˳�
			
			
		}else
		{
		
			if(uSI4432_Work_Delay>0)
			{
					uSI4432_Work_Delay--;
					 		delay_ms(10);
					continue;//�˳�			
			
			}
			
			if(UI_Show_No_B_SI4432_Task==0&&uSI4432_Work_Delay==0)
			{
			
		
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);  //��Դ��		
				
				 SI4432_Init();
				
				
					CMachine.fRF_Mode=0;//				
					CMachine_B.fRF_Mode=0;//				
				
				
				
			
			UI_Show_No_B_SI4432_Task=0xff;
			}
						
		
		
		
		}
		
		

		

		
		
		
			if(UI_Show_No_B_SI4432_Task!=0)
			{
				uSI4432_Data_Send_Time_1S_No++;
				if(uSI4432_Data_Send_Time_1S_No>100)
				{
					uSI4432_Data_Send_Time_1S_No=0;//
				
				
				
					
			if(CMachine.fRF_Mode==1)
			{		
				if(CMachine_B.fRF_Mode!=CMachine.fRF_Mode)
				{
					CMachine_B.fRF_Mode=CMachine.fRF_Mode;//		
				
					User_SI4432_Pair_ID();//�ı�Ϊ����ͷ��
					uSI4432_Pair_Data_Send_Time_1S_No=0;					
				}
				
				
								
				uSI4432_Pair_Data_Send_Time_1S_No++;
				if(uSI4432_Pair_Data_Send_Time_1S_No>3)	//3���Զ��˳�
				{
					uSI4432_Pair_Data_Send_Time_1S_No=0;
					CMachine.fRF_Mode=0;//�˳����ģʽ
				}
				
				
				
				
	uMaster_Send_Data[0]=0x11;
	uMaster_Send_Data[1]=0x08;
	uMaster_Send_Data[2]=0x50;
	uMaster_Send_Data[3]=(SYS_TYPE&0xff);		//����֡ͷ
	uMaster_Send_Data[4]=((SYS_TYPE_NO&0xff0000)>>16);		
	uMaster_Send_Data[5]=((SYS_TYPE_NO&0xff00)>>8);	
	uMaster_Send_Data[6]=((SYS_TYPE_NO&0xff)>>0);		
	uMaster_Send_Data[7]=User_CheckSum_Make(&uMaster_Send_Data[0],7);
	
		//	res = SI4432_ReadReg(0x02);
	//delay_ms(20);
		SI4432_WriteReg(0x07, 0x01);	// rf ģ�����Ready ģʽ	//ģʽת��ǰ������𳬹�15ms				
		delay_ms(20);		// ��ʱ 5ms, ��ϵͳ�ȶ�	
		
		SI4432_WriteReg(0x3e, 0x08);  // �ܹ�����8���ֽڵ�����
		SI4432_WriteReg(0x7F, uMaster_Send_Data[0]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[1]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[2]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[3]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[4]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[5]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[6]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[7]);

		SI4432_WriteReg(0x05, 0x04);	// �������ݷ�����󣬲����ж�	
		SI4432_WriteReg(0x06, 0x00);
		ItStatus1 = SI4432_ReadReg(0x03);//����ж�	  
		ItStatus2 = SI4432_ReadReg(0x04);
		SI4432_WriteReg(0x07, 0x09);//�򿪷���	   
		 while ( SI4432_NIRQ == 1);     
	     
		//read interrupt status registers to release the interrupt flags
		ItStatus1 = SI4432_ReadReg(0x03);	  	 	
		ItStatus2 = SI4432_ReadReg(0x04);	     	
	//	delay_ms(1000);						
				

			}else
{			
					
				if(CMachine_B.fRF_Mode!=CMachine.fRF_Mode)
				{
					CMachine_B.fRF_Mode=CMachine.fRF_Mode;//		
					User_SI4432_Data_ID();//�ı�Ϊ���ݷ���ͷ��
				}		


				
				
	res = SI4432_ReadReg(0x02);
	delay_ms(20);
		
		SI4432_WriteReg(0x07, 0x01);	// rf ģ�����Ready ģʽ					
		delay_ms(20);		// ��ʱ 5ms, ��ϵͳ�ȶ�	
	
				
				
				
			/*	if(CMachine.UI_Show_No==3)
				{
				
				
CMachine.pm25=20;
CMachine.tvoc=0;				
CMachine.co2=0;
CMachine.hcho=0;				
				
				}else
				if(CMachine.UI_Show_No==4)
				{
				
CMachine.pm25=50;
CMachine.tvoc=0;				
CMachine.co2=0;
CMachine.hcho=0;							
				
				}else				
				if(CMachine.UI_Show_No==5)
				{
CMachine.pm25=80;
CMachine.tvoc=0;				
CMachine.co2=0;
CMachine.hcho=0;							
				
				
				}else				
				if(CMachine.UI_Show_No==6)
				{
				
CMachine.pm25=120;
CMachine.tvoc=0;				
CMachine.co2=0;
CMachine.hcho=0;							
				
				}
*/



				
uMaster_Send_Data[0]=0x11;
uMaster_Send_Data[1]=0x0E;
uMaster_Send_Data[2]=0x10;	
uMaster_Send_Data[3]=(SYS_TYPE&0Xff00>>8);
uMaster_Send_Data[4]=SYS_TYPE&0xff;
uMaster_Send_Data[5]=(SYS_TYPE_NO&0Xff0000>>16);
uMaster_Send_Data[6]=(SYS_TYPE_NO&0Xff00>>8);
uMaster_Send_Data[7]=SYS_TYPE_NO&0xff;
uMaster_Send_Data[8]=((CMachine.pm25&0xff00)>>8);
uMaster_Send_Data[9]=CMachine.pm25&0xff;					
uMaster_Send_Data[10]=((CMachine.co2&0xff00)>>8);
uMaster_Send_Data[11]=CMachine.co2&0xff;										
uMaster_Send_Data[12]=((CMachine.hcho&0xff00)>>8);
uMaster_Send_Data[13]=CMachine.hcho&0xff;							
uMaster_Send_Data[14]=((CMachine.tvoc&0xff00)>>8);
uMaster_Send_Data[15]=CMachine.tvoc&0xff;		
				
uMaster_Send_Data[16]=((CMachine.temp&0xff00)>>8);
uMaster_Send_Data[17]=CMachine.temp&0xff;
uMaster_Send_Data[18]=((CMachine.humidity&0xff00)>>8);
uMaster_Send_Data[19]=CMachine.humidity&0xff;

uMaster_Send_Data[20]=0;
uMaster_Send_Data[21]=0;


uMaster_Send_Data[22]=User_CheckSum_Make(&uMaster_Send_Data[0],22);
//uMaster_Send_Data[14]=User_CheckSum_Make(&uMaster_Send_Data[0],14);






	
		SI4432_WriteReg(0x3e, 0x17);  // �ܹ�����23���ֽڵ�����
		SI4432_WriteReg(0x7F, uMaster_Send_Data[0]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[1]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[2]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[3]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[4]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[5]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[6]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[7]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[8]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[9]);		
		SI4432_WriteReg(0x7F, uMaster_Send_Data[10]);		
		SI4432_WriteReg(0x7F, uMaster_Send_Data[11]);		
		SI4432_WriteReg(0x7F, uMaster_Send_Data[12]);		
		SI4432_WriteReg(0x7F, uMaster_Send_Data[13]);				
		SI4432_WriteReg(0x7F, uMaster_Send_Data[14]);					
		SI4432_WriteReg(0x7F, uMaster_Send_Data[15]);					
		SI4432_WriteReg(0x7F, uMaster_Send_Data[16]);					
		SI4432_WriteReg(0x7F, uMaster_Send_Data[17]);			
		SI4432_WriteReg(0x7F, uMaster_Send_Data[18]);			
		SI4432_WriteReg(0x7F, uMaster_Send_Data[19]);			
		SI4432_WriteReg(0x7F, uMaster_Send_Data[20]);			
		SI4432_WriteReg(0x7F, uMaster_Send_Data[21]);			
		SI4432_WriteReg(0x7F, uMaster_Send_Data[22]);			





		SI4432_WriteReg(0x05, 0x04);	// �������ݷ�����󣬲����ж�	
		SI4432_WriteReg(0x06, 0x80);
		ItStatus1 = SI4432_ReadReg(0x03);//����ж�	  
		ItStatus2 = SI4432_ReadReg(0x04);
		SI4432_WriteReg(0x07, 0x09);//�򿪷���	   
		 while ( SI4432_NIRQ == 1);     
	     
		//read interrupt status registers to release the interrupt flags
		ItStatus1 = SI4432_ReadReg(0x03);	  	 	
		ItStatus2 = SI4432_ReadReg(0x04);	     	
	//	delay_ms(1000);					
					
				}			
					
				}
				
				
				
				
				
				
				
				
			}else
			{
				uSI4432_Data_Send_Time_1S_No=0;//


			}			
					
		
		
		
		
		
		
		
		
		
		
		
		

		
		

 		delay_ms(10);
	}














}





























