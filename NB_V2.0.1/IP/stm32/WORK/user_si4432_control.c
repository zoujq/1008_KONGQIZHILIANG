#include "includes.h"	   




// 0321 注意1：SPI 空闲时为低电平，第二跳边沿生效			 //注意2：SDN直接接DGND算了
//      注意3：SI4432和STM32要用同一个VCC3.3否则的话收到全为0
#define	SI4432_NSEL   PCout(5)  //SI4432_NSEL  使能
#define	SI4432_SDN    PBout(1)  //SI4432_SDN   使能
#define	SI4432_NIRQ   PBin(0)  //SI4432_NIRQ  中断状态



// 读取寄存器
u8  SI4432_ReadReg(u8  addr)   //读取寄存器的值
{
	uint8_t temp=0;
	
	SI4432_NSEL=0;
	SPI1_ReadWriteByte(addr);    //发送读取寄存器地址
	temp=SPI1_ReadWriteByte(0Xff);
	SI4432_NSEL=1;
	
	return temp;
}
//写寄存器
void SI4432_WriteReg(u8 addr, u8 value)  //写寄存器
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
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );//A,C时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  // PC5 = NSEL 推挽 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,GPIO_Pin_5);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  // PB1 =SI_SDN    推挽 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	//GPIO_SetBits(GPIOB,GPIO_Pin_1);	
 	GPIO_ResetBits(GPIOB,GPIO_Pin_1);//除关闭模式外 SDN=0,当 SDN=1 芯片将完全关闭，寄存器所用数据将丢失

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // PB0=SI_NIRQ 产生中断时为高   
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //上拉输入	  //浮空： GPIO_Mode_IN_FLOATING
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI1_Init();		   	//初始化SPI
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

//读取中断状态 	 清除中断  释放	 SI4432_NIRQ
	ItStatus1 = SI4432_ReadReg(0x03);  
	ItStatus2 = SI4432_ReadReg(0x04);  	 		
//软件复位
	SI4432_WriteReg(0x07, 0x80); //向0X07地址  写入0X80  软件复位
	 while ( SI4432_NIRQ == 1);       
	//读取中断状态 	 清除中断  释放	 SI4432_NIRQ
	ItStatus1 = SI4432_ReadReg(0x03);  
	ItStatus2 = SI4432_ReadReg(0x04);  
/////////////////////////开始设置
// 频率设置 434			
	//SI4432_WriteReg(0x75, 0x53);  
	//SI4432_WriteReg(0x76, 0x64);  // 
	//SI4432_WriteReg(0x77, 0x00); 
	SI4432_WriteReg(0x75, 0x57);  //470
	SI4432_WriteReg(0x76, 0x00);  
	SI4432_WriteReg(0x77, 0x00); 	
	
// 1.2K bps 发射速率	
//	SI4432_WriteReg(0x2a, 0x14);
	//SI4432_WriteReg(0x6e, 0x09);//1.2
	//SI4432_WriteReg(0x6f, 0xd5);
	SI4432_WriteReg(0x6e, 0x27);//4.8
	SI4432_WriteReg(0x6f, 0x52);	
	
	SI4432_WriteReg(0x70, 0x2d);// 

//SpiWriteRegister(0x72, 0x48);	//(9.6kbps)	
  //  SI4432_WriteReg(0x72, 0x38);	//频率偏差(1.2kbps)	
	SI4432_WriteReg(0x72, 0x30);	//频率偏差(30kbps)		
// 下面的设置根据Silabs 的Excel	(9.6 kbps, deviation: 45 kHz, channel filter BW: 102.2 kHz
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
	
	
//前导码 同步字
	/*
	SI4432_WriteReg(0x34, 0X0A);  // 发射5字节的Preamble
	SI4432_WriteReg(0x35, 0x2A);  // 需要检测 Preamble							
	SI4432_WriteReg(0x33, 0x02);// 同步字3,2 是同步字
	SI4432_WriteReg(0x36, 0x2d);  // 同步字为 0x2dd4
	SI4432_WriteReg(0x37, 0xd4);
	SI4432_WriteReg(0x30, 0x8D);   // 使能PH+ FIFO模式，高位在前面，使能CRC校验	CCITTT 	
    SI4432_WriteReg(0x32, 0x00 );//	禁止帧头
	SI4432_WriteReg(0x71, 0x63); // 发射不需要 CLK，FiFo ， FSK模式	
*/
//	SI4432_WriteReg(0x34, 0X08);  // 发射4字节的Preamble
//	SI4432_WriteReg(0x35, 0x22);  // 需要检测 Preamble							
//	SI4432_WriteReg(0x33, 0x22);// 同步字3,2 是同步字
//	SI4432_WriteReg(0x36, 0x53);  // 同步字为 0x2dd4


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
	
	
	SI4432_WriteReg(0x3A, (SYS_TYPE&0xff));		//发射帧头
	SI4432_WriteReg(0x3B, ((SYS_TYPE_NO&0xff0000)>>16));		
	SI4432_WriteReg(0x3C, ((SYS_TYPE_NO&0xff00)>>8));	
	SI4432_WriteReg(0x3D, ((SYS_TYPE_NO&0xff)>>0));		
	
	


	SI4432_WriteReg(0x30, 0x8D);   // 使能PH+ FIFO模式，高位在前面，使能CRC校验	CCITTT 	
  SI4432_WriteReg(0x32, 0x0F);//	禁止帧头
	SI4432_WriteReg(0x71, 0x23); // 发射不需要 CLK，FiFo ， FSK模式	


//GPIO
	SI4432_WriteReg(0x0C, 0x12); //  
	SI4432_WriteReg(0x0D, 0x15); // 
//其他设置
	SI4432_WriteReg(0x09, 0xD7);  //  负载电容 
	SI4432_WriteReg(0x69, 0x60);  //AGC过载	 
	
	SI4432_WriteReg(0x58, 0x80);	
	
//发射功率
	SI4432_WriteReg(0x6d, 0x07);
//手动打开接收
//	SI4432_WriteReg(0x07, 0x05);
//打开 接收中断
	SI4432_WriteReg(0x05, 0x03); 
	SI4432_WriteReg(0x06, 0x00); 
//清中断
	ItStatus1 = SI4432_ReadReg(0x03);	  //read the Interrupt Status1 register
	ItStatus2 = SI4432_ReadReg(0x04);	  //read the Interrupt Status2 register		
	
//接收设置
	SI4432_WriteReg(0x08, 0x03); //清发射、接收缓冲区
	SI4432_WriteReg(0x08, 0x00);	//清发射、接收缓冲区
	SI4432_WriteReg(0x07, 0x05);//打开接收	
//	SI4432_WriteReg(0x0d, 0xf4);//GPIO2接收数据





}
void User_SI4432_Pair_ID(void)//改变为对码头码
{
	SI4432_WriteReg(0x3F, 0x21);	
	SI4432_WriteReg(0x40, 0x21);	
	SI4432_WriteReg(0x41, 0x21);	
	SI4432_WriteReg(0x42, 0x21);		
	
	SI4432_WriteReg(0x3A, 0x21);	//发射帧头
	SI4432_WriteReg(0x3B, 0x21);	
	SI4432_WriteReg(0x3C, 0x21);	
	SI4432_WriteReg(0x3D, 0x21);			
	
}


void User_SI4432_Data_ID(void)//改变为数据发送头码
{
	SI4432_WriteReg(0x3F, (SYS_TYPE&0xff));		//发射帧头
	SI4432_WriteReg(0x40, ((SYS_TYPE_NO&0xff0000)>>16));		
	SI4432_WriteReg(0x41, ((SYS_TYPE_NO&0xff00)>>8));	
	SI4432_WriteReg(0x42, ((SYS_TYPE_NO&0xff)>>0));		
	
	SI4432_WriteReg(0x3A, (SYS_TYPE&0xff));		//发射帧头
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

unsigned short uSI4432_Work_Delay=0;//RF工作延时计数，因为供电和传感器一起。要等其他 地方 开
unsigned short uSI4432_Data_Send_Time_1S_No=0;//无线数据发送时间间隔计数



unsigned short uSI4432_Pair_Data_Send_Time_1S_No=0;//发送配对时间定时，1分钟


u8 uMaster_Send_Data[25]={0};//发送的数据


void User_SI4432_Task(void *pdata)
{







    GPIO_InitTypeDef  GPIO_InitStructure;  		  

	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTC时钟

	
	
	
	
				
		//电源使能脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化
		//GPIO_ResetBits(GPIOA,GPIO_Pin_12);    		
		GPIO_SetBits(GPIOA,GPIO_Pin_12);  //关				
				
				
				

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化
		GPIO_ResetBits(GPIOC,GPIO_Pin_4);		//MCU-SPI_SCLK    				
		GPIO_ResetBits(GPIOC,GPIO_Pin_5);		//MCU-SPI_NSEL-CS1


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);		//MCU-SPI_NIRQ_INT-OUT
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);		//MCU-SPI_SDN					
				
				
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);		//MCU-SPI_SDO-MISO
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);		//MCU-SPI_SDI-MOSI
								
	


	
	
	while(1)
	{			  
		
		
					 		delay_ms(10);
					continue;//退出
		
		
		////////////////////////////////////////    屏蔽RF    ////////////////////////////////////////
		
		
		
		if(CMachine.UI_Show_No<=1)
		{
			
			if(UI_Show_No_B_SI4432_Task!=0)
			{

				

				
		//电源使能脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化
		//GPIO_ResetBits(GPIOA,GPIO_Pin_12);    		
		GPIO_SetBits(GPIOA,GPIO_Pin_12);  //关				
				
				
				

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化
		GPIO_ResetBits(GPIOC,GPIO_Pin_4);		//MCU-SPI_SCLK    				
		GPIO_ResetBits(GPIOC,GPIO_Pin_5);		//MCU-SPI_NSEL-CS1


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);		//MCU-SPI_NIRQ_INT-OUT
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);		//MCU-SPI_SDN					
				
				
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);		//MCU-SPI_SDO-MISO
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);		//MCU-SPI_SDI-MOSI
								
				
				

					
				
			uSI4432_Work_Delay=200;//2秒	
				
				
			UI_Show_No_B_SI4432_Task=0;
			}
			 		delay_ms(10);
					continue;//退出
			
			
		}else
		{
		
			if(uSI4432_Work_Delay>0)
			{
					uSI4432_Work_Delay--;
					 		delay_ms(10);
					continue;//退出			
			
			}
			
			if(UI_Show_No_B_SI4432_Task==0&&uSI4432_Work_Delay==0)
			{
			
		
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);  //电源开		
				
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
				
					User_SI4432_Pair_ID();//改变为对码头码
					uSI4432_Pair_Data_Send_Time_1S_No=0;					
				}
				
				
								
				uSI4432_Pair_Data_Send_Time_1S_No++;
				if(uSI4432_Pair_Data_Send_Time_1S_No>3)	//3秒自动退出
				{
					uSI4432_Pair_Data_Send_Time_1S_No=0;
					CMachine.fRF_Mode=0;//退出配对模式
				}
				
				
				
				
	uMaster_Send_Data[0]=0x11;
	uMaster_Send_Data[1]=0x08;
	uMaster_Send_Data[2]=0x50;
	uMaster_Send_Data[3]=(SYS_TYPE&0xff);		//发射帧头
	uMaster_Send_Data[4]=((SYS_TYPE_NO&0xff0000)>>16);		
	uMaster_Send_Data[5]=((SYS_TYPE_NO&0xff00)>>8);	
	uMaster_Send_Data[6]=((SYS_TYPE_NO&0xff)>>0);		
	uMaster_Send_Data[7]=User_CheckSum_Make(&uMaster_Send_Data[0],7);
	
		//	res = SI4432_ReadReg(0x02);
	//delay_ms(20);
		SI4432_WriteReg(0x07, 0x01);	// rf 模块进入Ready 模式	//模式转换前必需挂起超过15ms				
		delay_ms(20);		// 延时 5ms, 让系统稳定	
		
		SI4432_WriteReg(0x3e, 0x08);  // 总共发射8个字节的数据
		SI4432_WriteReg(0x7F, uMaster_Send_Data[0]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[1]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[2]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[3]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[4]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[5]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[6]);
		SI4432_WriteReg(0x7F, uMaster_Send_Data[7]);

		SI4432_WriteReg(0x05, 0x04);	// 整包数据发射完后，产生中断	
		SI4432_WriteReg(0x06, 0x00);
		ItStatus1 = SI4432_ReadReg(0x03);//清除中断	  
		ItStatus2 = SI4432_ReadReg(0x04);
		SI4432_WriteReg(0x07, 0x09);//打开发射	   
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
					User_SI4432_Data_ID();//改变为数据发送头码
				}		


				
				
	res = SI4432_ReadReg(0x02);
	delay_ms(20);
		
		SI4432_WriteReg(0x07, 0x01);	// rf 模块进入Ready 模式					
		delay_ms(20);		// 延时 5ms, 让系统稳定	
	
				
				
				
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






	
		SI4432_WriteReg(0x3e, 0x17);  // 总共发射23个字节的数据
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





		SI4432_WriteReg(0x05, 0x04);	// 整包数据发射完后，产生中断	
		SI4432_WriteReg(0x06, 0x80);
		ItStatus1 = SI4432_ReadReg(0x03);//清除中断	  
		ItStatus2 = SI4432_ReadReg(0x04);
		SI4432_WriteReg(0x07, 0x09);//打开发射	   
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





























