#include "includes.h"	   





#define CON_ASK_MAX	1000	//10秒

#define ASK_RES_NEXT_DELAY 20 //延时发送下一个

 u8  uMode_Con_No=2;//模式号，功能分开，为了清晰，省功耗。0为进入关闭GSM睡眠，1为开GSM睡眠，2为机器进行1次定位，3为机器进行10次定位。
 u8  uCon_No=CON_NO_START0;//控制号。每执行一种动作，加1
 u8  uCon_No_B=0xff;//限制或重复执行动作标志
 u16  uCon_Time_Delay=0;//每种动作之间延迟时间
 u16  uCon_Ask_Time=0;//应答时间定时，到时间没回复，再次发送





 //char  const  st0[]	=	{"AT+CFUN=0\r\n"};//AT+CFUN=0
//unsigned char  const  st1[]	=	{"AT+CGMR\r\n"};//查询固件版本，可选
//unsigned char  code  st2[]	=	{"AT+CGSN=1\r\n"};//查询IMEI 号，如果返回ERROR,说明模块没有设置IMEI 号，需要以下这步设置IMEI 号
//unsigned char  const  st3[]	=	{"AT+NTSETID=1, 201612091450303\r\n"};//设置IMEI 号，仅在无IMEI 号时才需要此步骤，只能设置一次，再次设置无效。
 //char  const  st2[]	=	{"AT+NCDP=180.101.147.115\r\n"};//
//unsigned char  const  st5[] =	{"AT+CGDCONT\r\n"};//
//unsigned char  const  st6[] =	{"\r\n"};//
// char  const  st7[] =	{"AT+NRB\r\n"};//软重启
 char    st8[] =	{"AT+CFUN=1\r\n"};//开启射频全功能
//unsigned char  code  st9[] =	{"AT+CSCON=1\r\n"};//设置基站连接通知，可选
//unsigned char  code  st10[] =	{"AT+CEREG=2\r\n"};//设置连接核心网通知，可选
 char    st11[] =	{"AT+NNMI=1\r\n"};//下行数据通知
 char    st12[] =	{"AT+CGATT=1\r\n"};//自动搜网
//unsigned char  code  st13[] =	{"AT+NUESTATS\r\n"};//查询UE 状态，包括rsrp,sinr,覆盖等级，小区信息等，可选
 char    st13[] =	{"AT+CPSMS=0\r\n"};//关闭低功耗功能
 
 char    st14[] =	{"AT+CGPADDR\r\n"};//查询分配的ip 地址，可以以此判断是否连接到网络
 char    st15[] =	{"AT+NSOCR=DGRAM,17,9502,1\r\n"};//1 为socket 号，如果有回复OK说明创建成功,UDP

//char    st16[] =	{"AT+NSOST=1,47.52.24.148,2100,43,"};//测试服务器
	//char      st16[] =	{"AT+NSOST=1,112.74.173.107,2100,43,"};//测试服务器
 //char      st16[] =	{"AT+NSOST=1,39.96.30.87,2100,43,"};//测试服务器
 
 // char    st16[] =	{"AT+NSOST=1,39.96.30.87,2100,54,"};//测试服务器 
 //  char    st16[] =	{"AT+NSOST=1,219.76.218.76,1234,54,"};//测试服务器 
 
   char    st160[] =	{"AT+NSOST=1,219.76.218.76,1234,54,"};//测试服务器 
   char    st161[] =	{"AT+NSOST=1,219.76.218.76,1234,59,"};//测试服务器 	 
	 
	  
 
	 
 // char    st16[] =	{"AT+NSOST=1,\"nxinw.com\",2100,54,"};//测试服务器 


	

 char    st17[] =	{"AT+NSORF=1,256\r\n"};//


 //char  const  st15[] =	{"AT+NMGS=77,"};//


 u16  uPM25_Data_Send_Time=5500;//数据发送定时
//u16 	uCon_State_Ask_Time_No=0;//连接状态计时
 u32 		uTime_Heard_No=0;//心跳计数
 
 u16 		uTime_Heard_Send_No=0;//发送数据心跳计数

 

 u8  uData_Send_No_Add=0;//数据发送次数计数，返回清零，没返回超过重启模块
 
 u8  fData_Send_Services_Ask=0;//返回数据给服务器标志 
 




 u8 uSend_Data_No=0;//发送数据长度统计
 u8 uSend_Data_i=0;//



/************************************************************************************
*函数名称:check_sum
*功能描述:计算异或和校验
*入口参数:@data,校验数据的起始地址   @len,校验数据的长度
*返回值:异或和校验值
*************************************************************************************/



 u16 crc_check=0;
 u16 crc_check_res=0;


u8 CTOH(char *Pdata)
{
	u8 ctoh_b = 0;
	
	switch(Pdata[0])
	{
		case	'F':
			ctoh_b=0xf0;
			break;
		case	'E':
			ctoh_b=0xe0;			
			break;
		case	'D':
			ctoh_b=0xd0;			
			break;		
		case	'C':
			ctoh_b=0xc0;			
			break;
		case	'B':
			ctoh_b=0xb0;			
			break;
		case	'A':
			ctoh_b=0xa0;			
			break;		
		case	'9':
			ctoh_b=0x90;			
			break;
		case	'8':
			ctoh_b=0x80;			
			break;
		case	'7':
			ctoh_b=0x70;			
			break;		
		case	'6':
			ctoh_b=0x60;			
			break;
		case	'5':
			ctoh_b=0x50;			
			break;
		case	'4':
			ctoh_b=0x40;			
			break;				
		case	'3':
			ctoh_b=0x30;			
			break;		
		case	'2':
			ctoh_b=0x20;			
			break;
		case	'1':
			ctoh_b=0x10;			
			break;
		case	'0':
			ctoh_b=0x00;			
			break;				
		default:
			break;
		
	}
	switch(Pdata[1])
	{
		case	'F':
			ctoh_b|=0x0f;
			break;
		case	'E':
			ctoh_b|=0x0e;		
			break;
		case	'D':
			ctoh_b|=0x0d;		
			break;		
		case	'C':
			ctoh_b|=0x0c;		
			break;
		case	'B':
			ctoh_b|=0x0b;	
			break;
		case	'A':
			ctoh_b|=0x0a;		
			break;		
		case	'9':
			ctoh_b|=0x09;		
			break;
		case	'8':
			ctoh_b|=0x08;		
			break;
		case	'7':
			ctoh_b|=0x07;	
			break;		
		case	'6':
			ctoh_b|=0x06;	
			break;
		case	'5':
			ctoh_b|=0x05;	
			break;
		case	'4':
			ctoh_b|=0x04;		
			break;				
		case	'3':
			ctoh_b|=0x03;		
			break;		
		case	'2':
			ctoh_b|=0x02;	
			break;
		case	'1':
			ctoh_b|=0x01;		
			break;
		case	'0':
			ctoh_b|=0x00;		
			break;				
		default:
			break;
		
	}	
	
	
	return ctoh_b;

}

u8 HTOC(u8	Pdata)//
{
	u8 ctoh_b = 0;
	
	switch((Pdata&0x0f))
	{
		case	0xf:
			ctoh_b='F';
			break;
		case	0xe:
			ctoh_b='E';			
			break;
		case	0xd:
			ctoh_b='D';			
			break;		
		case	0xc:
			ctoh_b='C';			
			break;
		case	0xb:
			ctoh_b='B';			
			break;
		case	0xa:
			ctoh_b='A';			
			break;		
		case	0x9:
			ctoh_b='9';			
			break;
		case	0x8:
			ctoh_b='8';			
			break;
		case	0x7:
			ctoh_b='7';			
			break;		
		case	0x6:
			ctoh_b='6';			
			break;
		case	0x5:
			ctoh_b='5';			
			break;
		case	0x4:
			ctoh_b='4';			
			break;				
		case	0x3:
			ctoh_b='3';			
			break;		
		case	0x2:
			ctoh_b='2';			
			break;
		case	0x1:
			ctoh_b='1';			
			break;
		case	0x0:
			ctoh_b='0';			
			break;				
		default:
			break;
		
	}	
	
	return ctoh_b;
}



char * str_p;//字符串指针//





u8 volatile 	uUart_Recive_Data_Decode[50]={0};//接收数据解码
u8 volatile 	uUart_Recive_Data_Decode_i=0;

 u8  fData_Send_Services_Ask_B=0;//返回数据给服务器标志 
 u8  fData_Send_Services_Ask_B1=0;//返回数据给服务器标志 


 UTCTime TimeSeconds_res = 0;
 u32 TimeSeconds_res_b = 0;
 u32 TimeSeconds_b = 0;

 UTCTimeStruct	CTime_Res;//时间结构体


void Uart2_Decode(void)//串口处理函数 
{


		
	uUart2_Recive_Length=0;
	
	
	if(strstr((const char*)uUart2_Recive_Data,"ERROR")!=0)
	{

			uTime_Heard_Send_No=0;//重启		
					uTime_Heard_No=0;//重启			
						uCon_No=CON_NO_START0;		
						uCon_No_B=0xff;
						uCon_Ask_Time=0;//清零
			CMachine.uLine_Work=0;//
				
		
		
		}	
	
	
	

		switch(uCon_No)
						{
				case	CON_NO_ST0:
								if(strstr((const char*)uUart2_Recive_Data,"Lierda")!=0)//上电重启成功后会返回
								{	

										uCon_Time_Delay=100;//延时1s
										uCon_No=CON_NO_ST8;		
										uCon_Ask_Time=0;//清零
								}		
							
								break;			
							case	CON_NO_ST2:								
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST7;		
										uCon_Ask_Time=0;//清零
								}	
								break;				
							case	CON_NO_ST7:		

								break;								
						
							case	CON_NO_ST8:			
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=100;//延时1s
										uCon_No=CON_NO_ST11;		
										uCon_Ask_Time=0;//清零
								}	
								break;		


							case	CON_NO_ST11:								
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=500;//延时5s才开始搜网，防止出现ERR
										uCon_No=CON_NO_ST12;		
										uCon_Ask_Time=0;//清零
								}	
								break;		
							case	CON_NO_ST12:								
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=100;//延时1s
										uCon_No=CON_NO_ST13;		
										uCon_Ask_Time=0;//清零
								}	
								break;	
							case	CON_NO_ST13:								
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=100;//延时1s
										uCon_No=CON_NO_ST14;		
										uCon_Ask_Time=0;//清零
								}	
								break;									
								
							case	CON_NO_ST14:								
								if(strstr((const char*)uUart2_Recive_Data,"+CGPADDR:0,")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST15;		
										uCon_Ask_Time=0;//清零

								}else
								{


								}								
								break;			
							case	CON_NO_ST15:								
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST18;		
										uCon_Ask_Time=0;//清零
								}	
								break;									
							case	CON_NO_ST16:	
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST18;		
										uCon_Ask_Time=0;//清零
								}	
								break;										
							
							case	CON_NO_ST18:									
								if((str_p=strstr((const char*)uUart2_Recive_Data,"+NSONMI"))!=0&&uCon_Time_Delay==0)//回复OK
								{	

									
							//重样开始计数
									uPM25_Data_Send_Time=0;//
									uTime_Heard_No=0;//心跳清零												
									

									
											uCon_Time_Delay=5;//延时50ms
										uCon_No=CON_NO_ST17;		
										uCon_Ask_Time=0;//清零								
									
								}	
								break;			
							case	CON_NO_ST17:								
								if((str_p=strstr((const char*)uUart2_Recive_Data,"1400"))!=0&&uCon_Time_Delay==0)//上传数据，回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
	
										uCon_Ask_Time=0;//清零
									
									uData_Send_No_Add=0;//清零
									
									
									
									//长度
									uUart_Recive_Data_Decode[0]=CTOH((str_p+4));	
									uUart_Recive_Data_Decode[0]-=7;
									//CRC
									uUart_Recive_Data_Decode[1]=CTOH((str_p+6));		
									uUart_Recive_Data_Decode[2]=CTOH((str_p+8));		
									uUart_Recive_Data_Decode[3]=CTOH((str_p+10));		
									uUart_Recive_Data_Decode[4]=CTOH((str_p+12));											
									
									
									
									for(uUart_Recive_Data_Decode_i=0;uUart_Recive_Data_Decode_i<uUart_Recive_Data_Decode[0];uUart_Recive_Data_Decode_i++)
									{
										uUart_Recive_Data_Decode[5+uUart_Recive_Data_Decode_i]=CTOH((str_p+14+uUart_Recive_Data_Decode_i*2));										
									}
						
									



									
									
								//	uUart_Recive_Data_Decode[5]=CTOH((str_p+14));		//校验方法
								//	uUart_Recive_Data_Decode[6]=CTOH((str_p+16));		//加密方法
									
								//	uUart_Recive_Data_Decode[7]=CTOH((str_p+18));		//数据字典码
									
								//	uUart_Recive_Data_Decode[8]=CTOH((str_p+20));		//数据值
								//	uUart_Recive_Data_Decode[9]=CTOH((str_p+22));		
								//	uUart_Recive_Data_Decode[10]=CTOH((str_p+24));		
								//	uUart_Recive_Data_Decode[11]=CTOH((str_p+26));											
									
		
		if(uUart_Recive_Data_Decode[5]==0x01)//CRC16
		{

								crc_check_res=(uUart_Recive_Data_Decode[1]<<24);
								crc_check_res|=(uUart_Recive_Data_Decode[2]<<16);	
								crc_check_res|=(uUart_Recive_Data_Decode[3]<<8);	
								crc_check_res|=(uUart_Recive_Data_Decode[4]<<0);	

		crc_check=GetCrC((unsigned char *)&uUart_Recive_Data_Decode[5],uUart_Recive_Data_Decode_i);


			if(crc_check_res==crc_check)
			{

					for(uUart_Recive_Data_Decode_i=0;uUart_Recive_Data_Decode_i<uUart_Recive_Data_Decode[0];uUart_Recive_Data_Decode_i+=5)
					{
						
						
						if(uUart_Recive_Data_Decode[7+uUart_Recive_Data_Decode_i]==0x2C)
						{

											CMachine.uLine_Work=1;
											uCon_Time_Delay=5;//延时50ms
										uCon_No=CON_NO_ST18;		
										uCon_Ask_Time=0;//清零								

							
							
							
							
							
									TimeSeconds_res=0;
		
							
							
							


								TimeSeconds_res_b=uUart_Recive_Data_Decode[7+uUart_Recive_Data_Decode_i+1];
								TimeSeconds_res_b=((TimeSeconds_res_b<<24)&0xff000000);				
								TimeSeconds_res=TimeSeconds_res_b;
								TimeSeconds_res_b=uUart_Recive_Data_Decode[7+uUart_Recive_Data_Decode_i+2];
								TimeSeconds_res_b=((TimeSeconds_res_b<<16)&0xff0000);				
								TimeSeconds_res|=TimeSeconds_res_b;				
								TimeSeconds_res_b=uUart_Recive_Data_Decode[7+uUart_Recive_Data_Decode_i+3];
								TimeSeconds_res_b=((TimeSeconds_res_b<<8)&0xff00);				
								TimeSeconds_res|=TimeSeconds_res_b;							
								TimeSeconds_res_b=uUart_Recive_Data_Decode[7+uUart_Recive_Data_Decode_i+4];
								TimeSeconds_res_b=((TimeSeconds_res_b)&0xff);				
								TimeSeconds_res|=TimeSeconds_res_b;							
			
								TimeSeconds_res_b=(TimeSeconds_res-946569600);
				
				
								ConvertUTCTime(&CTime_Res,TimeSeconds_res_b);		//获取转换时间


		Sys_Time_WIFI.sec=	CTime_Res.seconds;  
		Sys_Time_WIFI.min=CTime_Res.minutes;  
		Sys_Time_WIFI.hour=CTime_Res.hour;     
		Sys_Time_WIFI.w_date=CTime_Res.day;      
		Sys_Time_WIFI.w_month=CTime_Res.month;    
		Sys_Time_WIFI.w_year=CTime_Res.year-2000;							
							
							
					
							
							
							
							
							
							
							
							
							//AT+NSOST=1,39.96.30.87,2100,54,1400360000C8F20100010000000129000000032A0000000D0300000000080000003F090000004D0C000000000D000000E00E00000300			
							
							//应答
							//14 00 0E 00 00 8D 9C 01 00 2C 5C 51 CF 64
							//单独模式
							//14 00 0E 00 00 AA 05 01 00 38 00 00 00 00
							//14 00 0E 00 00 6B C5 01 00 38 00 00 00 01
							//14 00 0E 00 00 2B C4 01 00 38 00 00 00 02
							//单独开关
							//14 00 0E 00 00 97 C5 01 00 39 00 00 00 00							
							//14 00 0E 00 00 56 05 01 00 39 00 00 00 01										
							//单独风速
							//14 00 0E 00 00 22 06 01 00 2E 00 00 00 01
							//14 00 0E 00 00 23 C5 01 00 2E 00 00 00 05
							//14 00 0E 00 00 A2 01 01 00 2E 00 00 00 0B							
							
							//开关+风速
							//14 00 13 00 00 60 C5 01 00 39 00 00 00 01 2E 00 00 00 05							
							//14 00 13 00 00 60 D7 01 00 39 00 00 00 00 2E 00 00 00 01											
							
							//开关+风速+模式
							//14 00 18 00 00 DC C1 01 00 39 00 00 00 01 2E 00 00 00 05 38 00 00 00 01							
							//14 00 18 00 00 9D 44 01 00 39 00 00 00 01 2E 00 00 00 01 38 00 00 00 02								
							
							

						}

						if(uUart_Recive_Data_Decode[7+uUart_Recive_Data_Decode_i]==0x3D)
						{
							
							
							CMachine.uSys_User_Reset=0;//系统用户重置//清零
							CMachine.uSys_User_Reset_Time=0;//系统用户重置,计时//清零			

												CMachine.uLine_Work=1;
											uCon_Time_Delay=5;//延时50ms
										uCon_No=CON_NO_ST18;		
										uCon_Ask_Time=0;//清零			
							
						}			
						
						
					}
				
				
			}
									
		}
									
									
									

	memset((void*)uUart2_Recive_Data,0,200);
									
									
									
								}else//读数据为空
								{


											uCon_Time_Delay=5;//延时50ms
										uCon_No=CON_NO_ST18;		
										uCon_Ask_Time=0;//清零								
																		
									


								}								
								
								
								
								
								break;																		
							
							
							

							default:
								break;

						
			
					
					
					
					

					
					
					
					
					

					}
										
	
	
	
	
	
}

void uart_sendstring(char const *str)
{
  u16 i = 0;

		while (str[i]!= '\0')
		{
			uUart2_Send_Data[i] = str[i];
			i++;
		}





			User_UART2_SendStringx((char *)&uUart2_Send_Data[0],i);	
	

			uTime_Heard_Send_No=0;//清零	
	
}


u8 fNBIOT_Init=0;//初始化标志





u8 volatile 	uSys_Data_Send[100]={0};//数据发送计算缓存
u8 volatile 	uSys_Data_Send_i=0;//数据发送计算缓存


union u32Tou8
{
	uint64_t ulongx;
	uint8_t uMac_Base[8];
}MACu32tou8;

	uint64_t SYS_TYPE_NO=0;


unsigned short UI_Show_No_B_NBIOT_Task=0xff;//
void User_NBIOT_Task(void *pdata)
{







    GPIO_InitTypeDef  GPIO_InitStructure;  		  

	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTC时钟

	
	

//获取MAC
MACu32tou8.ulongx=*(uint64_t*) (SYS_TYPE_NO_ADDR+0x110); 
	
SYS_TYPE_NO=0;	
SYS_TYPE_NO=(uint64_t)(MACu32tou8.uMac_Base[7]);	
SYS_TYPE_NO|=(uint64_t)(MACu32tou8.uMac_Base[6]<<8);		
SYS_TYPE_NO|=(uint64_t)(MACu32tou8.uMac_Base[5]<<16);		
	
	
	while(1)
	{			  
		
		
		if(CMachine.UI_Show_No<=1)
		{
			
			if(UI_Show_No_B_NBIOT_Task!=0)
			{

	 uart2_uninit();			
				
   

		//状态输入检测		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PA.1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA1
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);         				
				
				
		//电源使能脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //PC.3
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化PC3
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);    				
				
				
				
				
				
				
				
				
			UI_Show_No_B_NBIOT_Task=0;
			}
			 		delay_ms(10);
					continue;//退出
			
			
		}else
		{
		
			
			if(UI_Show_No_B_NBIOT_Task==0)
			{
			
		
		//状态输入检测		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PA.1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA1
 
				
		//电源使能脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //PC.3
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化PC3
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);    		


				
			UI_Show_No_B_NBIOT_Task=0xff;
			}
						
			
		if(uUart2_Recive_End>0)	
		{
		
		uUart2_Recive_End--;
		if(uUart2_Recive_End==0) 		  Uart2_Decode();//串口接收解码 

		
		}			
		


if(uCon_Time_Delay>0)	
	{
		uCon_Time_Delay--;
		
	}else
	{
	if(uCon_Ask_Time>0)//1秒
	{
		uCon_Ask_Time--;
		//if(uCon_Ask_Time==0||uCon_Ask_Time>CON_ASK_MAX)	
		if(uCon_Ask_Time==0)	
		{
	
				uCon_No_B=0xff;//重新发一次
				uCon_Ask_Time=0;
		
		}
	}	

	switch(uMode_Con_No)
	{
		case 0:	//0为进入关闭GSM睡眠.
			

		
			break;
		case 1:	//1为开GSM睡眠.
		

			break;			
		case 2:	//2为机器进行1次定位.
			if(uCon_No_B!=uCon_No)
			{
				uCon_No_B=uCon_No;
				
				
				switch(uCon_No)
				{
					case	CON_NO_START0:			
			

						uart2_uninit();		
					GPIO_ResetBits(GPIOC,GPIO_Pin_3); //供电关		
						uCon_Time_Delay=1000;//延时10S//电全部放完				
						uCon_No=CON_NO_START1;		
						CMachine.uLine_Work=0;	
						uData_Send_No_Add=0;//清零					

	
						break;
					case	CON_NO_START1:					
						GPIO_SetBits(GPIOC,GPIO_Pin_3); //打开NB电源 
						uart2_init(9600);//	  				
						uCon_Time_Delay=500;//延时5S
						uCon_No=CON_NO_ST0;		

						break;						
					case	CON_NO_START2:			
			
						break;	
					case	CON_NO_START3:			
					
						break;						
					case	CON_NO_START4:			
			
						break;						
					case	CON_NO_ST0:			
	
						//uart_sendstring(st0);			
						//uCon_Ask_Time=100;//应答检测			
						break;					
					case	CON_NO_ST2:			
	
					//	uart_sendstring(st2);			
						uCon_Ask_Time=CON_ASK_MAX;//应答检测						
						break;
					case	CON_NO_ST7://软重启，等待时间
		
					//	uart_sendstring(st7);	
						uCon_Time_Delay=700;//延时7S				
						uCon_No=CON_NO_ST8;		
						break;				
					case	CON_NO_ST8:
	
						uart_sendstring(st8);	
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;	
					case	CON_NO_ST11:
		
						uart_sendstring(st11);
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;	
					case	CON_NO_ST12:
						
						uart_sendstring(st12);		
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;		
					case	CON_NO_ST13:
						
						uart_sendstring(st13);		
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;
					case	CON_NO_ST14:
			
						uart_sendstring(st14);
						uCon_Ask_Time=200;//应答检测	//2秒
					
	
					
						break;	
					case	CON_NO_ST15:
			
						uart_sendstring(st15);
						uCon_Ask_Time=200;//应答检测	//2秒
					
	
					
						break;								
					case	CON_NO_ST16:

					

				
//BB4DC120F30200000105020012500020001001250000000000230011002200550030005064000000220011000501000000000000000000000000000000000000020006000033008800200AEEFF					

/*
uSensorData.sensor_data_struct.uPM25=55;//PM2.5
uSensorData.sensor_data_struct.uPM100=66;//PM10
uSensorData.sensor_data_struct.uPM1000=223;//PM100
uSensorData.sensor_data_struct.uWind_Speed=50;//风速
uSensorData.sensor_data_struct.uWind_Direction=250;//风向
uSensorData.sensor_data_struct.uTemp=28;//温度
uSensorData.sensor_data_struct.uHumidity=33;//湿度
uSensorData.sensor_data_struct.uNoise=463;//噪声
uSensorData.sensor_data_struct.uPressure=1024;//气压
uSensorData.sensor_data_struct.uHCHO=30;//甲醛
uSensorData.sensor_data_struct.uTVOC=55;//TVOC
uSensorData.sensor_data_struct.uCO2=88;//CO2
uSensorData.sensor_data_struct.uO2=102;//O2
uSensorData.sensor_data_struct.uCO=103;//CO
uSensorData.sensor_data_struct.uH2S=104;//H2S
uSensorData.sensor_data_struct.uSO2=105;//SO2
uSensorData.sensor_data_struct.uNO2=106;//NO2
uSensorData.sensor_data_struct.uO3=107;//O3
uSensorData.sensor_data_struct.uH2=108;//H2
uSensorData.sensor_data_struct.uCH4=109;//CH4
uSensorData.sensor_data_struct.uC3H8=120;//C3H8
uSensorData.sensor_data_struct.uAQI=88;//AQI
uSensorData.sensor_data_struct.uRainfall=102;//雨量
uSensorData.sensor_data_struct.uSoil_Temp=22;//土壤温度
uSensorData.sensor_data_struct.uSoil_Humidity=55;//土壤湿度
uSensorData.sensor_data_struct.uWater_Surface_Evaporation=63;//水面蒸发
uSensorData.sensor_data_struct.uSun_Radiation=556;//太阳总辐射
uSensorData.sensor_data_struct.uAnion=555;//负离子 
uSensorData.sensor_data_struct.uIllumination=456;//光照
uSensorData.sensor_data_struct.uRadiation_Dosage=553;//辐射剂量
*/



//数据头					
uSys_Data_Send[0]=0x14;
//有效数据长度
if(CMachine.uSys_User_Reset==0)
{
	uSys_Data_Send[1]=0x00;
	uSys_Data_Send[2]=0x36;		
}else
{
	uSys_Data_Send[1]=0x00;
	uSys_Data_Send[2]=0x3B;		
}







//校验位
//uSys_Data_Send[3]='0';
//uSys_Data_Send[4]='0';									
//uSys_Data_Send[5]='0';
//uSys_Data_Send[6]='0';

//校验方法
uSys_Data_Send[7]=0x01;
//加密方法
uSys_Data_Send[8]=0x00;


//设备序号
uSys_Data_Send[9]=0x01;
uSys_Data_Send[10]=SYS_EDITION;
//序号:第一个字节为版本号
//例01：硬件版本1，软件版本1
//
//
uSys_Data_Send[11]=((SYS_TYPE_NO&0xff0000)>>16);
uSys_Data_Send[12]=((SYS_TYPE_NO&0xff00)>>8);
uSys_Data_Send[13]=(SYS_TYPE_NO&0xff);
//通讯类型
uSys_Data_Send[14]=0x29;
uSys_Data_Send[15]=0x00;
uSys_Data_Send[16]=0x00;
uSys_Data_Send[17]=0x00;
uSys_Data_Send[18]=0x03;
//产品类型
uSys_Data_Send[19]=0x2A;
uSys_Data_Send[20]=0x00;
uSys_Data_Send[21]=0x00;
uSys_Data_Send[22]=0x00;
uSys_Data_Send[23]=0x0D;


//PM2.5
uSys_Data_Send[24]=0x03;
uSys_Data_Send[25]=0x00;
uSys_Data_Send[26]=0x00;
uSys_Data_Send[27]=((CMachine.pm25&0xff00)>>8);
uSys_Data_Send[28]=(CMachine.pm25&0xff);


//温度
uSys_Data_Send[29]=0x08;
uSys_Data_Send[30]=0x00;
uSys_Data_Send[31]=0x00;
uSys_Data_Send[32]=(((CMachine.temp*10)&0xff00)>>8);
uSys_Data_Send[33]=((CMachine.temp*10)&0xff);

//湿度
uSys_Data_Send[34]=0x09;
uSys_Data_Send[35]=0x00;
uSys_Data_Send[36]=0x00;
uSys_Data_Send[37]=(((CMachine.humidity*10)&0xff00)>>8);
uSys_Data_Send[38]=((CMachine.humidity*10)&0xff);

//甲醛
uSys_Data_Send[39]=0x0C;
uSys_Data_Send[40]=0x00;
uSys_Data_Send[41]=0x00;
uSys_Data_Send[42]=((CMachine.hcho&0xff00)>>8);
uSys_Data_Send[43]=(CMachine.hcho&0xff);

//TVOC
uSys_Data_Send[44]=0x0D;
uSys_Data_Send[45]=0x00;
uSys_Data_Send[46]=0x00;
uSys_Data_Send[47]=((CMachine.tvoc&0xff00)>>8);
uSys_Data_Send[48]=(CMachine.tvoc&0xff);

//CO2
uSys_Data_Send[49]=0x0E;
uSys_Data_Send[50]=0x00;
uSys_Data_Send[51]=0x00;
uSys_Data_Send[52]=((CMachine.co2&0xff00)>>8);
uSys_Data_Send[53]=(CMachine.co2&0xff);


if(CMachine.uSys_User_Reset==1)
{



uSys_Data_Send[54]=0x3D;
uSys_Data_Send[55]=0x00;
uSys_Data_Send[56]=0x00;
uSys_Data_Send[57]=0x00;
uSys_Data_Send[58]=0x01;


}



if(CMachine.uSys_User_Reset==0)		crc_check=GetCrC((unsigned char *)&uSys_Data_Send[7],47);
else		crc_check=GetCrC((unsigned char *)&uSys_Data_Send[7],52);


		
//校验位
uSys_Data_Send[3]=0x00;
uSys_Data_Send[4]=0x00;							
uSys_Data_Send[5]=((crc_check&0xff00)>>8);
uSys_Data_Send[6]=(crc_check&0xff);





if(CMachine.uSys_User_Reset==0)	
{
							uSend_Data_No=strlen(st160);
					strncpy((char *)uUart2_Send_Data,st160,uSend_Data_No);
}else
{
					uSend_Data_No=strlen(st161);
					strncpy(uUart2_Send_Data,st161,uSend_Data_No);
}







if(CMachine.uSys_User_Reset==0)	
{
	for(uSys_Data_Send_i=0;uSys_Data_Send_i<54;uSys_Data_Send_i++)
	{

	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i]=HTOC(((uSys_Data_Send[uSys_Data_Send_i])&0xf0)>>4);		
	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=HTOC((uSys_Data_Send[uSys_Data_Send_i])&0xf);		

	}
}else
{
	for(uSys_Data_Send_i=0;uSys_Data_Send_i<59;uSys_Data_Send_i++)
	{

	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i]=HTOC(((uSys_Data_Send[uSys_Data_Send_i])&0xf0)>>4);		
	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=HTOC((uSys_Data_Send[uSys_Data_Send_i])&0xf);		

	}
}








uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i]=0x0D;	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=0x0A;	





			User_UART2_SendStringx((char *)&uUart2_Send_Data[0],(uSend_Data_No+2*(uSys_Data_Send_i+1)));	








			
			
					break;		
					case	CON_NO_ST17:
						uart_sendstring(st17);
						uCon_Ask_Time=200;//应答检测	//2秒
					

						break;					
					case	CON_NO_ST18:
			

					
	
					
						break;				
		default:
			
			break;
		}
	}else
	{
		//if(uCon_No==CON_NO_ST16||uCon_No==CON_NO_ST17||uCon_No==CON_NO_ST18)
		if(uCon_No==CON_NO_ST18)
		{
			
			

if(CMachine.uSys_User_Reset_Time>0)	CMachine.uSys_User_Reset_Time--;
			

if(
		(CMachine.uSys_User_Reset==1&&((CMachine.uSys_User_Reset_Time%2000)==1999))

	)
{

	uPM25_Data_Send_Time=5900;//1秒后发数据
	
}


/*	
if(
		
			fData_Send_Services_Ask_B==1
	)
{


		fData_Send_Services_Ask_B=0;
		uPM25_Data_Send_Time=6000;//马上发数据
	fData_Send_Services_Ask_B1=1;//标志
}

*/






			
			
					uPM25_Data_Send_Time++;//10ms
					if(uPM25_Data_Send_Time>=6000)//1分钟//1分钟一次包，防止端口老化
					{
						uCon_No_B=0xff;
						uCon_No=CON_NO_ST16;
						uPM25_Data_Send_Time=0;//清零
						
						uData_Send_No_Add++;
						if(uData_Send_No_Add>10)//连续10次没有返回
						{
							uData_Send_No_Add=0;//
							
			uTime_Heard_Send_No=0;//重启		
					uTime_Heard_No=0;//重启			
						uCon_No=CON_NO_START0;		
						uCon_No_B=0xff;
						uCon_Ask_Time=0;//清零
			CMachine.uLine_Work=0;//
											
			
						}
					}else
					if(uPM25_Data_Send_Time%1000==0)//每10秒，没事就去读下有没有数据
					{
						uCon_No_B=0xff;										
						uCon_No=CON_NO_ST17;					
					}
					
	
					
			/*	if(Sys_Time_Start==1&&Sys_Time.Hours>=2&&Sys_Time.Hours<6)//2点到6点10分钟一次
				{

					uPM25_Data_Send_Time++;//10ms
					if(uPM25_Data_Send_Time>=60000)//10分钟
					{
						uCon_No_B=0xff;
						
						uPM25_Data_Send_Time=0;//清零
						
						
					}					
					
					
				}else
				{
				
					uPM25_Data_Send_Time++;//10ms
					if(uPM25_Data_Send_Time>=30000)//5分钟
					{
						uCon_No_B=0xff;
						uPM25_Data_Send_Time=0;//清零
						
						
					}				
				
				
				}*/
				
				

			
					
					
			
			
			//if(uCon_State_Ask_Time_No>0)	
		//	{
			//	uCon_State_Ask_Time_No--;
			//	if(uCon_State_Ask_Time_No==0)//10S//没回复//复位
			//	{
						/*uCon_No=CON_NO_START0;		
						uCon_Ask_Time=0;//清零
						uPM25_Data_Send_Time=0;
					*/
				//}
		//	}
			/*		uPM25_Data_Send_Time++;//10ms
					if(uPM25_Data_Send_Time==6000)//1分钟
					{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST14;	//再次发送	
										uCon_Ask_Time=0;//清零
					}*/
		}
	}		
	break;						
		default:
			
			break;

}
}



		//防止异常没有发数据
		uTime_Heard_Send_No++;
		if(uTime_Heard_Send_No>=6000)//发送数据,心跳包//1分钟无数据发送
		{
			uTime_Heard_Send_No=0;//重启		
					uTime_Heard_No=0;//重启			
						uCon_No=CON_NO_START0;		
						uCon_No_B=0xff;
						uCon_Ask_Time=0;//清零
			CMachine.uLine_Work=0;//
				
				

		}		


	if(CMachine.uLine_Work==1)
	{

		uTime_Heard_No++;
		if(uTime_Heard_No>=180000)//时间较准,心跳包//30分钟无数据
		{
			uTime_Heard_Send_No=0;//重启		
					uTime_Heard_No=0;//重启			
						uCon_No=CON_NO_START0;		
						uCon_No_B=0xff;
						uCon_Ask_Time=0;//清零
			CMachine.uLine_Work=0;//
				
				

		}	
	
	}else
	{
		uTime_Heard_No=0;
				
		
	
		
		

	}

			
			
			
			
			
			
			
			
			
			
		
		
		
		}
		
		

		
		

 		delay_ms(10);
	}














}





























