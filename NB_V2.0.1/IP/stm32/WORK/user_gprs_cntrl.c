#include "includes.h"	   
#include "crc32.h"



#define CON_ASK_MAX	1000	//10秒

#define ASK_RES_NEXT_DELAY 20 //延时发送下一个

u8  uMode_Con_No=2;//模式号，功能分开，为了清晰，省功耗。0为进入关闭GSM睡眠，1为开GSM睡眠，2为机器进行1次定位，3为机器进行10次定位。
u8  uCon_No=CON_NO_START0;//控制号。每执行一种动作，加1
u8  uCon_No_B=0xff;//限制或重复执行动作标志
u16  uCon_Time_Delay=0;//每种动作之间延迟时间
u16  uCon_Ask_Time=0;//应答时间定时，到时间没回复，再次发送

u16 	uUart_Send_Length=0;//发送长度




unsigned char  const  st0[]	=	{"AT\r\n"};
unsigned char  const  st1[]	=	{"ATE0\r\n"};//关闭回显
unsigned char  const  st2[]	=	{"AT+CSQ\r\n"};
unsigned char  const  st3[]	=	{"AT+QIFGCNT=0\r\n"};//设置PDP context
unsigned char  const  st4[]	=	{"AT+QICSGP=1,\"UNIM2M.GZM2MAPN\"\r\n"};//设置APN
unsigned char  const  st5[] =	{"AT+CREG?;+CGREG?\r\n"};//查询网络 状态
unsigned char  const  st6[] =	{"AT+QISTAT\r\n"};//查询当前连接状态


unsigned char  const 	st8[] =	{"AT+QNTP=\"202.120.2.101\",123\r\n"};//查询时间同步状态//上海交通大学网络中心NTP服务器地址
unsigned char  const	st9[]=	{"AT+CCLK?\r\n"};//查询时间


unsigned char  const  st10[]	=	{"AT+QIREGAPP\r\n"};
unsigned char  const  st11[]	=	{"AT+QIACT\r\n"};		
unsigned char  const	 st12[]=	{"AT+QICLOSE\r\n"};
//unsigned char  const  st13[]=	{"AT+QIOPEN=\"TCP\",\"222.244.144.69\",\"51005\"\r\n"};
//unsigned char  const  st13[]=	{"AT+QIOPEN=\"TCP\",\"116.204.70.58\",\"10000\"\r\n"};
unsigned char  const  st13[]=	{"AT+QIOPEN=\"TCP\",\"47.52.24.148\",\"9000\"\r\n"};
unsigned char  const  st14[]=	{"AT+QISEND=72\r\n"};
unsigned char  const  st140[]=	{"AT+QISEND=13\r\n"};

unsigned char  const  st17 []	=	{"AT+QSCLK=2\r\n"};		//进入睡眠模式
unsigned char  const  st18 []	=	{"AT+QSCLK=0\r\n"};		//退出睡眠模式



//u16  uPM25_Data_Send_Time=0;//数据发送定时
//u16 	uCon_State_Ask_Time_No=0;//连接状态计时
//u8 		uTime_Heard_No=0;//心跳计数

u32  uPM25_Data_Send_Time=27000;//数据发送定时//开机30秒后发送次

u8 	uLine_Work=0;//网络状态
u16  uTime_Heard_No=0;//心跳计数




u32 DATA_CRC=0;
unsigned char DATA_CRC_B[10]={0};//缓存


u8 	uUart_Gprs_Send[400]={0};//发送缓存

u8 	uUart_Send_End[3]={'\r',0x1a,'\0'};


#define datatime_addr 44
#define temperture_addr 68
#define humidity_addr 107
#define pressure_addr 0
#define gustSpeed_addr 142
#define gustDir_addr 178
#define noise_addr 209
#define PM25_addr 238
#define PM10_addr 263
#define TSP_addr 0



u8	uSys_Time_Point_No=0;//时间计数
u8	uSys_Time_Recive_Buf[10];


u8 uSysTime_Get_No=0;//系统时间获取次数，3次获取不到，跳过

/************************************************************************************
*函数名称:check_sum
*功能描述:计算异或和校验
*入口参数:@data,校验数据的起始地址   @len,校验数据的长度
*返回值:异或和校验值
*************************************************************************************/
u32 check_sum(u8 *Pdata,u16 lenth)
{
   int crc = 0xFFFF; 
	int i = 0;
	int j = 0; 
	    for ( i = 0; i < lenth; i++) {
	    crc = Pdata[i] ^ (crc>>8); 
	     
	    for ( j = 0; j < 8; ++j){ 
		    if ((crc & 0x0001) != 0) 
		    	crc = (crc >> 1) ^ 0xA001; 
		    else 
		    	crc  = crc >> 1; 
		    } 
	    
			} 
return crc;
}



void Uart4_Decode(void)//串口处理函数 
{


		
				uUart4_Recive_Length=0;//复位
	
	
		#ifdef SEGGER
	SEGGER_RTT_printf(0,"%s",uUart4_Recive_Data);
	#endif	
										if(strstr(uUart4_Recive_Data,"PDP DEACT"))//场景异常关闭
								{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//清零
	memset(uUart4_Recive_Data,0,50);
									return;
								}
								
								
								if(strstr(uUart4_Recive_Data,"ERROR"))//回复ERROR
								{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//清零
									memset(uUart4_Recive_Data,0,50);
									return;
								}										
								
						switch(uCon_No)
						{
							case	CON_NO_ST0:
								if(strstr(uUart4_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST1;		
										uCon_Ask_Time=0;//清零
								}			
								break;
							case	CON_NO_ST1:
								if(strstr(uUart4_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST2;		
										uCon_Ask_Time=0;//清零
								}		
								break;								
							case	CON_NO_ST2:								
								if(strncmp(uUart4_Recive_Data,"\r\n+CSQ",6)==0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST3;		
										uCon_Ask_Time=0;//清零
								}	
								break;
							case	CON_NO_ST3:		
								if(strncmp(uUart4_Recive_Data,"\r\nOK",4)==0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST4;		
										uCon_Ask_Time=0;//清零
								}								
								break;			
							case	CON_NO_ST4:		
								if(strncmp(uUart4_Recive_Data,"\r\nOK",4)==0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST5;		
										uCon_Ask_Time=0;//清零
								}								
								break;									
							case	CON_NO_ST5:
								if(uCon_Time_Delay==0)
								{
									if(strncmp(uUart4_Recive_Data,"\r\n+CREG: 0,1",12)==0)//网络正常
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST13;												
										uCon_Ask_Time=0;//清零
									}else	if(strncmp(uUart4_Recive_Data,"\r\n+CREG: 0,5",12)==0)//网络漫游，没法秒定，
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST13;												
										uCon_Ask_Time=0;//清零
								}
								break;

							case	CON_NO_ST6:		

							
								break;										
							case	CON_NO_ST7:		
						
								break;			
							case	CON_NO_ST8:			
								if(uCon_Time_Delay==0&&(CPU_ID[1]==UID_KEY[1]))//防止前面命令有OK显示
								{
									if(strstr(uUart4_Recive_Data,"+QNTP: 0")!=0)//回复同步时间完成
									{	

											uCon_Time_Delay=10;//延时100ms
											uCon_No=CON_NO_ST9;		//
											uCon_Ask_Time=0;//清零
										uSysTime_Get_No=0;//清零
									}else
									if(strstr(uUart4_Recive_Data,"+QNTP:")!=0)//错误，重启
									{	

										
										uSysTime_Get_No++;
										if(uSysTime_Get_No>=4)	uSysTime_Get_No=4;
										if(uSysTime_Get_No>=4) uCon_No=CON_NO_ST13;		//
										else	uCon_No=CON_NO_START0;												
										
										uCon_Time_Delay=100;//延时1秒
										uCon_Ask_Time=0;//清零
									}					


									
									
								}					

											break;	

							case	CON_NO_ST9:			
												


							if(strncmp(uUart4_Recive_Data,"\r\n+CCLK:",8)==0&&uCon_Time_Delay==0)//回复时间
							{
							
							Sys_Time_Save.w_year=2000+(uUart4_Recive_Data[10]-0x30)*10+(uUart4_Recive_Data[11]-0x30);
								Sys_Time_Save.w_month=(uUart4_Recive_Data[13]-0x30)*10+(uUart4_Recive_Data[14]-0x30);
								Sys_Time_Save.w_date=(uUart4_Recive_Data[16]-0x30)*10+(uUart4_Recive_Data[17]-0x30);
								Sys_Time_Save.hour=(uUart4_Recive_Data[19]-0x30)*10+(uUart4_Recive_Data[20]-0x30)+8;
								if(Sys_Time_Save.hour>=24)	Sys_Time_Save.hour-=24;
								Sys_Time_Save.min=(uUart4_Recive_Data[22]-0x30)*10+(uUart4_Recive_Data[23]-0x30);
								Sys_Time_Save.sec=(uUart4_Recive_Data[25]-0x30)*10+(uUart4_Recive_Data[26]-0x30);
							
								 Sys_RTC_Init();//实时时钟初始化
								
								
							
							}

							
							
											uCon_Time_Delay=10;//延时100ms
											uCon_No=CON_NO_ST13;		
											uCon_Ask_Time=0;//清零
							
							
							break;					
							case	CON_NO_ST10:								
								if(strncmp(uUart4_Recive_Data,"\r\nOK",4)==0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST11;		
										uCon_Ask_Time=0;//清零
								}else		if(strstr(uUart4_Recive_Data,"ERROR")!=0)//没有网络
								{
										uCon_Time_Delay=6000;//延时60秒
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//清零

								}								
								break;	
							case	CON_NO_ST11:								
								if(strncmp(uUart4_Recive_Data,"\r\nOK",4)==0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST13;		
										uCon_Ask_Time=0;//清零
								}								
								break;	
							case	CON_NO_ST12:								
								if(strncmp(uUart4_Recive_Data,"\r\nCLOSE OK",10)==0&&uCon_Time_Delay==0)//回复OK//关闭网络
								{	
										uCon_Time_Delay=200;//延时2000ms
//										uCon_No=CON_NO_ST17;		//进入睡眠模式
										uCon_Ask_Time=0;//清零
									

								}								
								break;	
							case	CON_NO_ST13:								
								if(strncmp(uUart4_Recive_Data,"\r\nCONNECT OK",12)==0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST14;		
										uCon_Ask_Time=0;//清零
									
											uLine_Work=1;		//连接成功			
									
								}else	if(strstr(uUart4_Recive_Data,"CONNECT FAIL")!=0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=6000;//延时60s
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//清零
								}					
								
								break;	
							case	CON_NO_ST14:								
								if(strncmp(uUart4_Recive_Data,"\r\n> ",4)==0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST15;		
										uCon_Ask_Time=0;//清零
								}								
								break;									
							case	CON_NO_ST15:									
								if(strstr(uUart4_Recive_Data,"SEND OK"))//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST16;		
										uCon_Ask_Time=0;//清零
								}						
								break;	
							case	CON_NO_ST16:		//连接状态						
								if(uUart4_Recive_Data[0]==0xAA&&uUart4_Recive_Data[1]==0x07)
								{
										//应答清零
//										uCon_State_Ask_Time_No=0;
									//	uTime_Heard_No=0;//心跳清零
								//	uAnion_Work_Set=uUart4_Recive_Data[5];				

									uTime_Heard_No=0;//心跳清零

									
									
								}else	if(strstr(uUart4_Recive_Data,"CLOSED"))//网络断开
								{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//清零

								}else	if(strstr(uUart4_Recive_Data,"CONNECT OK"))//网络连接正常
								{
//										uCon_State_Ask_Time_No=0;//清零	
									uTime_Heard_No=0;//心跳清零
										uCon_Ask_Time=0;//清零

								}
								
								
								
								
								
								
									
											
								break;										
				
								
						
							default:
								break;

						
						}
												
					
					
					
					

					
					
					
					
					

					}
						

					
					
	memset(uUart4_Recive_Data,0,50);
}

void uart_sendstring(unsigned char const *str)
{
  u16 i = 0;
	memset(uUart4_Send_Data,0,400);
	if(uUart_Send_Length==0)
	{
		while (str[i]!= '\0')
		{
			uUart4_Send_Data[i] = str[i];
			i++;
		}
		uUart_Send_Length=i;
	}else
	{
	for(i=0;i<uUart_Send_Length;i++)	uUart4_Send_Data[i] = str[i];

	}


			User_UART4_SendStringx(&uUart4_Send_Data[0],uUart_Send_Length);
			#ifdef SEGGER
	SEGGER_RTT_printf(0,"uart_send:%s\r\n",uUart4_Send_Data);
	#endif	
}
void User_GPRS_Task(void *pdata)//GPRS任务
{

      //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD  |RCC_APB2Periph_GPIOE  |  RCC_APB2Periph_AFIO  , ENABLE); 



    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PD.0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
    GPIO_Init(GPIOD, &GPIO_InitStructure); //初始化PD0
	 GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
	
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PE.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
    GPIO_Init(GPIOE, &GPIO_InitStructure); //初始化PE2
	 GPIO_ResetBits(GPIOE,GPIO_Pin_2); 						 //PE.2 	
	
		
		#ifdef SEGGER
	SEGGER_RTT_printf(0,"gprs_start\r\n");
	#endif	
	
		while(1)
	{
		
	

				if(uUart4_Recive_End>0)	
		{
		
		uUart4_Recive_End--;
		if(uUart4_Recive_End==0) 		  Uart4_Decode();//串口接收解码 

		
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

		if(CON_NO_ST0==uCon_No)
		{
uCon_No=CON_NO_START3;//重新开机

	}else
	{
			
				uCon_No_B=0xff;//重新发一次
				uCon_Ask_Time=0;
		
	}
		
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
				
			//	if(uCon_State_Ask_Time_No>0)	uCon_State_Ask_Time_No=0;
				
				switch(uCon_No)
				{
					case	CON_NO_START0:			
						 GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
							 GPIO_SetBits(GPIOE,GPIO_Pin_2); 						 //PE.2 	
	
					
						uCon_Time_Delay=100;//延时1S				
						uCon_No=CON_NO_START1;					

	uLine_Work=0;		//未连接			
					
						break;
					case	CON_NO_START1:			
					//	P1_1=1;		
								//	GPIO_SetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
								 GPIO_ResetBits(GPIOE,GPIO_Pin_2); 						 //PE.2 	
						uCon_Time_Delay=100;//延时1S
						uCon_No=CON_NO_START3;						
						break;						
					case	CON_NO_START2:			
				//		P1_1=0;		
											 GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
						uCon_Time_Delay=2000;//延时20S		
						uCon_No=CON_NO_START3;						
						break;	
					case	CON_NO_START3:			
									GPIO_SetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
						uCon_Time_Delay=120;//延时1.2S
						uCon_No=CON_NO_START4;									
						break;						
					case	CON_NO_START4:			
											 GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
						uCon_Time_Delay=500;//延时5S		
						uCon_No=CON_NO_ST0;			
						break;						
					case	CON_NO_ST0:			
						uUart_Send_Length=0;						
						uart_sendstring(st0);			
						uCon_Ask_Time=100;//应答检测			
						break;					
					case	CON_NO_ST1:			
						uUart_Send_Length=0;				
						uart_sendstring(st1);
						uCon_Ask_Time=CON_ASK_MAX;//应答检测				
						break;			
					case	CON_NO_ST2:			
						uUart_Send_Length=0;				
						uart_sendstring(st2);			
						uCon_Ask_Time=CON_ASK_MAX;//应答检测						
						break;
					case	CON_NO_ST3:
						uUart_Send_Length=0;							
						uart_sendstring(st3);			
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;	
					case	CON_NO_ST4:
						uUart_Send_Length=0;				
						uart_sendstring(st4);
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;			
					case	CON_NO_ST5:
						uUart_Send_Length=0;				
						uart_sendstring(st5);
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;	
					case	CON_NO_ST6:
//						uart_sendstring(st6);			
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;	
					case	CON_NO_ST7:
					//uart_sendstring(st7);	
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;				
					case	CON_NO_ST8:
						uUart_Send_Length=0;	
						uart_sendstring(st8);	
						uCon_Ask_Time=3*CON_ASK_MAX;//应答检测	
						break;	
					case	CON_NO_ST9:
						uUart_Send_Length=0;	
						uart_sendstring(st9);
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;				
					case	CON_NO_ST10:
						uUart_Send_Length=0;				
						uart_sendstring(st10);
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;	
					case	CON_NO_ST11:
						uUart_Send_Length=0;				
						uart_sendstring(st11);
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;	
					case	CON_NO_ST12:
						uUart_Send_Length=0;								
						uart_sendstring(st12);		
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;				
					case	CON_NO_ST13:
						uUart_Send_Length=0;							
						uart_sendstring(st13);			
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;	
					case	CON_NO_ST14:
						uUart_Send_Length=0;				
						uart_sendstring(st14);
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
					
	//	uCon_Time_Delay=100;//延时1000ms
		//uCon_No=CON_NO_ST15;							
					
						break;				
					case	CON_NO_ST15:
					uUart_Send_Length=0;						
if(f_Sys_Check!=2)		
{

	uCon_No=CON_NO_ST1;		
	break;


}
			memset(uUart_Gprs_Send,0,400);
			/*		
uPM100=499;
					uPM25=488;
				uNoise=633;	
					uTemp=709;
					uHumidity=325;
					uPressure=1028;
					
				uWind_Speed=123;
uWind_Direction=3000;
*/


					
uUart_Gprs_Send[0]=0xBB;
uUart_Gprs_Send[1]=0x40;		
					
uUart_Gprs_Send[2]=0xA2;	
uUart_Gprs_Send[3]=0x05;							
uUart_Gprs_Send[4]=(SYS_TYPE&0xff00)>>8;	
uUart_Gprs_Send[5]=(SYS_TYPE&0xff);				
					
uUart_Gprs_Send[6]=((SYS_TYPE_NO&0xff0000)>>16);					
uUart_Gprs_Send[7]=((SYS_TYPE_NO&0xff00)>>8);						
uUart_Gprs_Send[8]=(SYS_TYPE_NO&0xff);		
					
uUart_Gprs_Send[9]=0x00;		
uUart_Gprs_Send[10]=0x00;	
					
			
uUart_Gprs_Send[11]=((uSensorData.sensor_data_struct.uPM25&0xFF00)>>8);		
uUart_Gprs_Send[12]=(uSensorData.sensor_data_struct.uPM25&0xFF);		

uUart_Gprs_Send[13]=((uSensorData.sensor_data_struct.uPM100&0xFF00)>>8);				
uUart_Gprs_Send[14]=(uSensorData.sensor_data_struct.uPM100&0xFF);		
//TSP
uUart_Gprs_Send[15]=0x00;						
uUart_Gprs_Send[16]=0x00;			
//aqi
uUart_Gprs_Send[17]=0x00;								
uUart_Gprs_Send[18]=0x00;			
//
uUart_Gprs_Send[19]=((uSensorData.sensor_data_struct.uWind_Speed&0xFF00)>>8);					
uUart_Gprs_Send[20]=(uSensorData.sensor_data_struct.uWind_Speed&0xFF);	
//
if(((uSensorData.sensor_data_struct.uWind_Direction>=0)&&(uSensorData.sensor_data_struct.uWind_Direction<=225))||uSensorData.sensor_data_struct.uWind_Direction>=3375)
{//北风
uUart_Gprs_Send[21]=0;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>225)&&(uSensorData.sensor_data_struct.uWind_Direction<675))
{//东北风
uUart_Gprs_Send[21]=1;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>=675)&&(uSensorData.sensor_data_struct.uWind_Direction<=1125))
{//东风
uUart_Gprs_Send[21]=2;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>1125)&&(uSensorData.sensor_data_struct.uWind_Direction<1575))
{//东南风
uUart_Gprs_Send[21]=3;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>=1575)&&(uSensorData.sensor_data_struct.uWind_Direction<=2025))
{//南风
uUart_Gprs_Send[21]=4;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>2025)&&(uSensorData.sensor_data_struct.uWind_Direction<2475))
{//西南风
uUart_Gprs_Send[21]=5;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>=2475)&&(uSensorData.sensor_data_struct.uWind_Direction<=2925))
{//西风
uUart_Gprs_Send[21]=6;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>2925)&&(uSensorData.sensor_data_struct.uWind_Direction<3375))
{//西北风
uUart_Gprs_Send[21]=7;		
} 
	

//
uUart_Gprs_Send[22]=((uSensorData.sensor_data_struct.uNoise&0xFF00)>>8);						
uUart_Gprs_Send[23]=(uSensorData.sensor_data_struct.uNoise&0xFF);				

uUart_Gprs_Send[24]=((uSensorData.sensor_data_struct.uTemp&0xFF00)>>8);							
uUart_Gprs_Send[25]=(uSensorData.sensor_data_struct.uTemp&0xFF);				

uUart_Gprs_Send[26]=((uSensorData.sensor_data_struct.uHumidity&0xFF00)>>8);						
uUart_Gprs_Send[27]=(uSensorData.sensor_data_struct.uHumidity&0xFF);		

uUart_Gprs_Send[28]=((uSensorData.sensor_data_struct.uPressure&0xFF00)>>8);	
uUart_Gprs_Send[29]=(uSensorData.sensor_data_struct.uPressure&0xFF);		


uUart_Gprs_Send[30]=0x00;	
uUart_Gprs_Send[31]=0x00;				
uUart_Gprs_Send[32]=0x00;								
uUart_Gprs_Send[33]=0x00;						
uUart_Gprs_Send[34]=0x00;							
uUart_Gprs_Send[35]=0x00;							
uUart_Gprs_Send[36]=0x00;						
uUart_Gprs_Send[37]=0x00;					
uUart_Gprs_Send[38]=0x00;					
uUart_Gprs_Send[39]=0x00;
//负离子
uUart_Gprs_Send[40]=0x00;	
uUart_Gprs_Send[41]=0x00;				
uUart_Gprs_Send[42]=0x00;	
//雨量
uUart_Gprs_Send[43]=0x00;			
uUart_Gprs_Send[44]=0x00;			
//土壤温度
uUart_Gprs_Send[45]=0x00;			
uUart_Gprs_Send[46]=0x00;	
//土壤湿度
uUart_Gprs_Send[47]=0x00;			
uUart_Gprs_Send[48]=0x00;	
//水面蒸发
uUart_Gprs_Send[49]=0x00;			
uUart_Gprs_Send[50]=0x00;	
//太阳总辐射
uUart_Gprs_Send[51]=0x00;			
uUart_Gprs_Send[52]=0x00;	
//光照
uUart_Gprs_Send[53]=0x00;			
uUart_Gprs_Send[54]=0x00;	
//CO2				
uUart_Gprs_Send[55]=0x00;			
uUart_Gprs_Send[56]=0x00;	
//甲醛
uUart_Gprs_Send[57]=0x00;			
uUart_Gprs_Send[58]=0x00;	
//TVOC	
uUart_Gprs_Send[59]=0x00;			
uUart_Gprs_Send[60]=0x00;	
//O2
uUart_Gprs_Send[61]=0x00;			
uUart_Gprs_Send[62]=0x00;	
//燃气
uUart_Gprs_Send[63]=0x00;			
uUart_Gprs_Send[64]=0x00;	
//报警标志
uUart_Gprs_Send[65]=0x00;			
uUart_Gprs_Send[66]=0x00;	
uUart_Gprs_Send[67]=0x00;			
uUart_Gprs_Send[68]=0x00;	

			
uUart_Gprs_Send[69]=0x0A;	
uUart_Gprs_Send[70]=0xEE;	
uUart_Gprs_Send[71]=0xFF;	



					
uUart_Send_Length=72;
uart_sendstring(&uUart_Gprs_Send[0]);


/*
uPM25_Data_Send_Time=0;//
//uCon_State_Ask_Time_No=1000;//10秒
uTime_Heard_No++;
if(uTime_Heard_No>3)
{
uTime_Heard_No=0;//
uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
uCon_No=CON_NO_START0;		
uCon_Ask_Time=0;//清零	


}*/



					break;		
					case	CON_NO_ST16:


						break;					
	
		default:
			
			break;
		}
	}else
	{
		if(uCon_No==CON_NO_ST16)
		{
			

					uPM25_Data_Send_Time++;//10ms
					if(uPM25_Data_Send_Time>=30000)//5分钟
					{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST14;	//再次发送	
										uCon_Ask_Time=0;//清零
						uPM25_Data_Send_Time=0;//清零
						
						
					}
		}
	}		
	break;						
		default:
			
			break;

}
}







	if(uLine_Work==1)
	{

		uTime_Heard_No++;
		if(uTime_Heard_No>=90000)//时间较准,心跳包//15分钟无数据
		{
			uTime_Heard_No=0;//重启
			
						uCon_No=CON_NO_START0;		
						uCon_Ask_Time=0;//清零
			uLine_Work=0;//
				
			
		}	
	
	}else
	{
		uTime_Heard_No=0;
				

	}


		delay_ms(10);

	}
	





}


