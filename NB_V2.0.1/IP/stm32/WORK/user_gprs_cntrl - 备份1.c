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


unsigned char  const  st10[]	=	{"AT+QIREGAPP\r\n"};
unsigned char  const  st11[]	=	{"AT+QIACT\r\n"};		
unsigned char  const	 st12[]=	{"AT+QICLOSE\r\n"};
unsigned char  const  st13[]=	{"AT+QIOPEN=\"TCP\",\"222.244.144.69\",\"51005\"\r\n"};
unsigned char  const  st14[]=	{"AT+QISEND=340\r\n"};
unsigned char  const  st140[]=	{"AT+QISEND=13\r\n"};

unsigned char  const  st17 []	=	{"AT+QSCLK=2\r\n"};		//进入睡眠模式
unsigned char  const  st18 []	=	{"AT+QSCLK=0\r\n"};		//退出睡眠模式



u16  uPM25_Data_Send_Time=0;//数据发送定时
u16 	uCon_State_Ask_Time_No=0;//连接状态计时


u32 DATA_CRC=0;
unsigned char DATA_CRC_B[10]={0};//缓存


u8 	uUart_Gprs_Send[400]={0};//发送缓存

u8 	uUart_Send_End[3]={'\r',0x1a,'\0'};




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
										uCon_No=CON_NO_ST10;												
										uCon_Ask_Time=0;//清零
									}else	if(strncmp(uUart4_Recive_Data,"\r\n+CREG: 0,5",12)==0)//网络漫游，没法秒定，
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST10;												
										uCon_Ask_Time=0;//清零
								}
								break;

							case	CON_NO_ST6:		

							
								break;										
							case	CON_NO_ST7:		
						
								break;			
							case	CON_NO_ST8:			

											break;	
							case	CON_NO_ST9:			
														
							
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
								}else	if(strncmp(uUart4_Recive_Data,"\r\nCONNECT FAIL",14)==0&&uCon_Time_Delay==0)//回复OK
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
								}else	if(strstr(uUart4_Recive_Data,"ERROR"))//回复ERROR
								{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//清零

								}								
								break;	
							case	CON_NO_ST16:		//连接状态						
								if(uUart4_Recive_Data[0]==0xAA&&uUart4_Recive_Data[2]==0x23)
								{
										//应答清零
										uCon_State_Ask_Time_No=0;
								//	uAnion_Work_Set=uUart4_Recive_Data[5];									
									
								}else	if(uUart4_Recive_Data[0]==0xAA&&uUart4_Recive_Data[2]==0xC1)
								{
									//uAnion_Work_Set=uUart4_Recive_Data[3];
										//发送应答
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST17;		
										uCon_Ask_Time=0;//清零
								}else	if(strstr(uUart4_Recive_Data,"PDP DEACT"))//场景异常关闭
								{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//清零

								}else	if(strstr(uUart4_Recive_Data,"CLOSED"))//网络断开
								{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//清零

								}else	if(strstr(uUart4_Recive_Data,"CONNECT OK"))//网络连接正常
								{
										uCon_State_Ask_Time_No=0;//清零	
										uCon_Ask_Time=0;//清零

								}
								
								
								
								
								
								
									
											
								break;										
							case	CON_NO_ST17:								
								if(strncmp(uUart4_Recive_Data,"\r\n> ",4)==0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST18;		
										uCon_Ask_Time=0;//清零
								}								
								break;	
							case	CON_NO_ST18:											
								if(strncmp(uUart4_Recive_Data,"\r\nSEND OK",4)==0&&uCon_Time_Delay==0)//回复OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST16;		
										uCon_Ask_Time=0;//清零
									//重样开始计数
									uPM25_Data_Send_Time=0;//
									uCon_State_Ask_Time_No=0;//10秒
									
								}else	if(strstr(uUart4_Recive_Data,"ERROR")&&uCon_Time_Delay==0)//回复ERROR
								{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST16;		
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
		if(uCon_Ask_Time==0||uCon_Ask_Time>CON_ASK_MAX)	
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
				
				if(uCon_State_Ask_Time_No>0)	uCon_State_Ask_Time_No=0;
				
				switch(uCon_No)
				{
					case	CON_NO_START0:			
				//		P1_1=0;		
						 GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
						
	
					
						uCon_Time_Delay=500;//延时1S				
						uCon_No=CON_NO_START1;							
						break;
					case	CON_NO_START1:			
					//	P1_1=1;		
									GPIO_SetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
						uCon_Time_Delay=80;//延时0.8S
						uCon_No=CON_NO_START2;						
						break;						
					case	CON_NO_START2:			
				//		P1_1=0;		
											 GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
						uCon_Time_Delay=2000;//延时20S		
						uCon_No=CON_NO_START3;						
						break;	
					case	CON_NO_START3:			
				//		P1_1=1;		
									GPIO_SetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
						uCon_Time_Delay=120;//延时1.2S
						uCon_No=CON_NO_START4;						
						break;						
					case	CON_NO_START4:			
					//	P1_1=0;		
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
//						uart_sendstring(st8);	
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
						break;	
					case	CON_NO_ST9:
//						uart_sendstring(st9);
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
			memset(uUart_Gprs_Send,0,400);
uUart_Gprs_Send[0]='#';
uUart_Gprs_Send[1]='#';		
					
uUart_Gprs_Send[2]='0';	
uUart_Gprs_Send[3]='3';							
uUart_Gprs_Send[4]='2';	
uUart_Gprs_Send[5]='8';			
					
uUart_Gprs_Send[6]='S';						
uUart_Gprs_Send[7]='T';						
uUart_Gprs_Send[8]='=';						
uUart_Gprs_Send[9]='2';		
uUart_Gprs_Send[10]='2';		
uUart_Gprs_Send[11]=';';		
					
uUart_Gprs_Send[12]='C';		
uUart_Gprs_Send[13]='N';						
uUart_Gprs_Send[14]='=';						
uUart_Gprs_Send[15]='2';						
uUart_Gprs_Send[16]='0';						
uUart_Gprs_Send[17]='1';						
uUart_Gprs_Send[18]='1';	
uUart_Gprs_Send[19]=';';			

uUart_Gprs_Send[20]='P';						
uUart_Gprs_Send[21]='W';						
uUart_Gprs_Send[22]='=';						
uUart_Gprs_Send[23]='1';						
uUart_Gprs_Send[24]='2';						
uUart_Gprs_Send[25]='3';							
uUart_Gprs_Send[26]='4';						
uUart_Gprs_Send[27]='5';						
uUart_Gprs_Send[28]='6';	
uUart_Gprs_Send[29]=';';	
//483237J//F0:05:00:00:01
uUart_Gprs_Send[30]='M';	
uUart_Gprs_Send[31]='N';					
uUart_Gprs_Send[32]='=';								
uUart_Gprs_Send[33]='4';								
uUart_Gprs_Send[34]='8';								
uUart_Gprs_Send[35]='3';								
uUart_Gprs_Send[36]='2';						
uUart_Gprs_Send[37]='3';						
uUart_Gprs_Send[38]='7';						
uUart_Gprs_Send[39]='J';						
uUart_Gprs_Send[40]='F';
uUart_Gprs_Send[41]='0';					
uUart_Gprs_Send[42]='0';								
uUart_Gprs_Send[43]='5';								
uUart_Gprs_Send[44]='0';								
uUart_Gprs_Send[45]='0';								
uUart_Gprs_Send[46]='1';		
uUart_Gprs_Send[47]=';';		


uUart_Gprs_Send[48]='C';	
uUart_Gprs_Send[49]='P';					
uUart_Gprs_Send[50]='=';					
uUart_Gprs_Send[51]='&';				
uUart_Gprs_Send[52]='&';				
					
uUart_Gprs_Send[53]='D';							
uUart_Gprs_Send[54]='a';		
uUart_Gprs_Send[55]='t';		
uUart_Gprs_Send[56]='a';		
uUart_Gprs_Send[57]='T';		
uUart_Gprs_Send[58]='i';		
uUart_Gprs_Send[59]='m';		
uUart_Gprs_Send[60]='e';		
uUart_Gprs_Send[61]='=';		
uUart_Gprs_Send[62]='2';							
uUart_Gprs_Send[63]='0';		
uUart_Gprs_Send[64]='1';		
uUart_Gprs_Send[65]='7';		
uUart_Gprs_Send[66]='1';		
uUart_Gprs_Send[67]='2';		
uUart_Gprs_Send[68]='0';		
uUart_Gprs_Send[69]='4';		
uUart_Gprs_Send[70]='1';		
uUart_Gprs_Send[71]='1';							
uUart_Gprs_Send[72]='1';		
uUart_Gprs_Send[73]='8';		
uUart_Gprs_Send[74]='2';		
uUart_Gprs_Send[75]='2';		
uUart_Gprs_Send[76]=';';	

uUart_Gprs_Send[77]='t';		
uUart_Gprs_Send[78]='e';		
uUart_Gprs_Send[79]='m';		
uUart_Gprs_Send[80]='p';							
uUart_Gprs_Send[81]='e';		
uUart_Gprs_Send[82]='r';		
uUart_Gprs_Send[83]='t';		
uUart_Gprs_Send[84]='u';		
uUart_Gprs_Send[85]='r';		
uUart_Gprs_Send[86]='e';		
uUart_Gprs_Send[87]='-';		
uUart_Gprs_Send[88]='R';		
uUart_Gprs_Send[89]='t';							
uUart_Gprs_Send[90]='d';		
uUart_Gprs_Send[91]='=';		
uUart_Gprs_Send[92]='+';		
uUart_Gprs_Send[93]='3';		
uUart_Gprs_Send[94]='2';		
uUart_Gprs_Send[95]='.';		
uUart_Gprs_Send[96]='1';		
uUart_Gprs_Send[97]=',';		
uUart_Gprs_Send[98]='t';		
uUart_Gprs_Send[99]='e';		
uUart_Gprs_Send[100]='m';		
uUart_Gprs_Send[101]='p';							
uUart_Gprs_Send[102]='e';		
uUart_Gprs_Send[103]='r';		
uUart_Gprs_Send[104]='t';		
uUart_Gprs_Send[105]='u';		
uUart_Gprs_Send[106]='r';		
uUart_Gprs_Send[107]='e';		
uUart_Gprs_Send[108]='-';		
uUart_Gprs_Send[109]='F';		
uUart_Gprs_Send[110]='l';							
uUart_Gprs_Send[111]='a';		
uUart_Gprs_Send[112]='g';		
uUart_Gprs_Send[113]='=';		
uUart_Gprs_Send[114]='N';		
uUart_Gprs_Send[115]=';';		

uUart_Gprs_Send[116]='h';		
uUart_Gprs_Send[117]='u';		
uUart_Gprs_Send[118]='m';		
uUart_Gprs_Send[119]='i';		
uUart_Gprs_Send[120]='d';		
uUart_Gprs_Send[121]='i';		
uUart_Gprs_Send[122]='t';		
uUart_Gprs_Send[123]='y';		
uUart_Gprs_Send[124]='-';		
uUart_Gprs_Send[125]='R';		
uUart_Gprs_Send[126]='t';		
uUart_Gprs_Send[127]='d';		
uUart_Gprs_Send[128]='=';		
uUart_Gprs_Send[129]='0';		
uUart_Gprs_Send[130]='0';		
uUart_Gprs_Send[131]='1';		
uUart_Gprs_Send[132]=',';		
uUart_Gprs_Send[133]='h';		
uUart_Gprs_Send[134]='u';		
uUart_Gprs_Send[135]='m';		
uUart_Gprs_Send[136]='i';		
uUart_Gprs_Send[137]='d';		
uUart_Gprs_Send[138]='i';		
uUart_Gprs_Send[139]='t';		
uUart_Gprs_Send[140]='y';		
uUart_Gprs_Send[141]='-';		
uUart_Gprs_Send[142]='F';		
uUart_Gprs_Send[143]='l';		
uUart_Gprs_Send[144]='a';		
uUart_Gprs_Send[145]='g';		
uUart_Gprs_Send[146]='=';		
uUart_Gprs_Send[147]='N';		
uUart_Gprs_Send[148]=';';		


uUart_Gprs_Send[149]='p';		
uUart_Gprs_Send[150]='r';	
uUart_Gprs_Send[151]='e';	
uUart_Gprs_Send[152]='s';	
uUart_Gprs_Send[153]='s';	
uUart_Gprs_Send[154]='u';	
uUart_Gprs_Send[155]='r';	
uUart_Gprs_Send[156]='e';	
uUart_Gprs_Send[157]='-';	
uUart_Gprs_Send[158]='R';	
uUart_Gprs_Send[159]='t';	
uUart_Gprs_Send[160]='d';	
uUart_Gprs_Send[161]='=';	
uUart_Gprs_Send[162]='1';	
uUart_Gprs_Send[163]='0';	
uUart_Gprs_Send[164]='2';	
uUart_Gprs_Send[165]='.';	
uUart_Gprs_Send[166]='4';	
uUart_Gprs_Send[167]=',';	
uUart_Gprs_Send[168]='p';		
uUart_Gprs_Send[169]='r';	
uUart_Gprs_Send[170]='e';	
uUart_Gprs_Send[171]='s';	
uUart_Gprs_Send[172]='s';	
uUart_Gprs_Send[173]='u';	
uUart_Gprs_Send[174]='r';	
uUart_Gprs_Send[175]='e';	
uUart_Gprs_Send[176]='-';	
uUart_Gprs_Send[177]='F';	
uUart_Gprs_Send[178]='l';	
uUart_Gprs_Send[179]='a';	
uUart_Gprs_Send[180]='g';	
uUart_Gprs_Send[181]='=';	
uUart_Gprs_Send[182]='N';	
uUart_Gprs_Send[183]=';';	

uUart_Gprs_Send[184]='g';	
uUart_Gprs_Send[185]='u';	
uUart_Gprs_Send[186]='s';	
uUart_Gprs_Send[187]='t';	
uUart_Gprs_Send[188]='S';	
uUart_Gprs_Send[189]='p';	
uUart_Gprs_Send[190]='e';	
uUart_Gprs_Send[191]='e';	
uUart_Gprs_Send[192]='d';	
uUart_Gprs_Send[193]='-';	
uUart_Gprs_Send[194]='R';	
uUart_Gprs_Send[195]='t';	
uUart_Gprs_Send[196]='d';	
uUart_Gprs_Send[197]='=';	
uUart_Gprs_Send[198]='0';	
uUart_Gprs_Send[199]=',';	
uUart_Gprs_Send[200]='g';	
uUart_Gprs_Send[201]='u';	
uUart_Gprs_Send[202]='s';	
uUart_Gprs_Send[203]='t';	
uUart_Gprs_Send[204]='S';	
uUart_Gprs_Send[205]='p';	
uUart_Gprs_Send[206]='e';	
uUart_Gprs_Send[207]='e';	
uUart_Gprs_Send[208]='d';	
uUart_Gprs_Send[209]='-';	
uUart_Gprs_Send[210]='F';	
uUart_Gprs_Send[211]='l';	
uUart_Gprs_Send[212]='a';	
uUart_Gprs_Send[213]='g';	
uUart_Gprs_Send[214]='=';	
uUart_Gprs_Send[215]='N';	
uUart_Gprs_Send[216]=';';	

uUart_Gprs_Send[217]='g';	
uUart_Gprs_Send[218]='u';	
uUart_Gprs_Send[219]='s';	
uUart_Gprs_Send[220]='t';	
uUart_Gprs_Send[221]='D';	
uUart_Gprs_Send[222]='i';	
uUart_Gprs_Send[223]='r';	
uUart_Gprs_Send[224]='-';	
uUart_Gprs_Send[225]='R';	
uUart_Gprs_Send[226]='t';	
uUart_Gprs_Send[227]='d';	
uUart_Gprs_Send[228]='=';	
uUart_Gprs_Send[229]='3';	
uUart_Gprs_Send[230]='2';	
uUart_Gprs_Send[231]='.';	
uUart_Gprs_Send[232]='4';	
uUart_Gprs_Send[233]=',';	
uUart_Gprs_Send[217]='g';	
uUart_Gprs_Send[218]='u';	
uUart_Gprs_Send[219]='s';	
uUart_Gprs_Send[220]='t';	
uUart_Gprs_Send[221]='D';	
uUart_Gprs_Send[222]='i';	
uUart_Gprs_Send[223]='r';	
uUart_Gprs_Send[224]='-';	
uUart_Gprs_Send[225]='F';	
uUart_Gprs_Send[226]='l';	
uUart_Gprs_Send[227]='a';	
uUart_Gprs_Send[228]='g';	
uUart_Gprs_Send[229]='=';	
uUart_Gprs_Send[230]='N';	
uUart_Gprs_Send[231]=';';	

uUart_Gprs_Send[232]='n';	
uUart_Gprs_Send[233]='o';	
uUart_Gprs_Send[234]='i';	
uUart_Gprs_Send[235]='s';	
uUart_Gprs_Send[236]='e';	
uUart_Gprs_Send[237]='-';	
uUart_Gprs_Send[238]='R';	
uUart_Gprs_Send[239]='t';	
uUart_Gprs_Send[240]='d';	
uUart_Gprs_Send[241]='=';	
uUart_Gprs_Send[242]='1';	
uUart_Gprs_Send[243]='3';	
uUart_Gprs_Send[244]='0';	
uUart_Gprs_Send[245]='.';	
uUart_Gprs_Send[246]='0';	
uUart_Gprs_Send[247]=',';	
uUart_Gprs_Send[248]='n';	
uUart_Gprs_Send[249]='o';	
uUart_Gprs_Send[250]='i';	
uUart_Gprs_Send[251]='s';	
uUart_Gprs_Send[252]='e';	
uUart_Gprs_Send[253]='-';	
uUart_Gprs_Send[254]='F';	
uUart_Gprs_Send[255]='l';	
uUart_Gprs_Send[256]='a';	
uUart_Gprs_Send[257]='g';	
uUart_Gprs_Send[258]='=';	
uUart_Gprs_Send[259]='N';	
uUart_Gprs_Send[260]=';';	


uUart_Gprs_Send[261]='P';	
uUart_Gprs_Send[262]='M';	
uUart_Gprs_Send[263]='2';	
uUart_Gprs_Send[264]='5';	
uUart_Gprs_Send[265]='-';	
uUart_Gprs_Send[266]='R';	
uUart_Gprs_Send[267]='t';	
uUart_Gprs_Send[268]='d';	
uUart_Gprs_Send[269]='=';	
uUart_Gprs_Send[270]='0';	
uUart_Gprs_Send[271]='1';	
uUart_Gprs_Send[272]='0';	
uUart_Gprs_Send[273]=',';	
uUart_Gprs_Send[274]='P';	
uUart_Gprs_Send[275]='M';	
uUart_Gprs_Send[276]='2';	
uUart_Gprs_Send[277]='5';	
uUart_Gprs_Send[278]='-';	
uUart_Gprs_Send[279]='F';	
uUart_Gprs_Send[280]='l';	
uUart_Gprs_Send[281]='a';	
uUart_Gprs_Send[282]='g';	
uUart_Gprs_Send[283]='=';	
uUart_Gprs_Send[284]='N';	
uUart_Gprs_Send[285]=';';	

uUart_Gprs_Send[286]='P';	
uUart_Gprs_Send[287]='M';	
uUart_Gprs_Send[288]='1';	
uUart_Gprs_Send[289]='0';	
uUart_Gprs_Send[290]='-';	
uUart_Gprs_Send[291]='R';	
uUart_Gprs_Send[292]='t';	
uUart_Gprs_Send[293]='d';	
uUart_Gprs_Send[294]='=';	
uUart_Gprs_Send[295]='0';	
uUart_Gprs_Send[296]='2';	
uUart_Gprs_Send[297]='0';	
uUart_Gprs_Send[298]=',';	
uUart_Gprs_Send[299]='P';	
uUart_Gprs_Send[300]='M';	
uUart_Gprs_Send[301]='1';	
uUart_Gprs_Send[302]='0';	
uUart_Gprs_Send[303]='-';	
uUart_Gprs_Send[304]='F';	
uUart_Gprs_Send[305]='l';	
uUart_Gprs_Send[306]='a';	
uUart_Gprs_Send[307]='g';	
uUart_Gprs_Send[308]='=';	
uUart_Gprs_Send[309]='N';	
uUart_Gprs_Send[310]=';';	

uUart_Gprs_Send[311]='T';	
uUart_Gprs_Send[312]='S';	
uUart_Gprs_Send[313]='P';	
uUart_Gprs_Send[314]='-';	
uUart_Gprs_Send[315]='R';	
uUart_Gprs_Send[316]='t';	
uUart_Gprs_Send[317]='d';	
uUart_Gprs_Send[318]='=';	
uUart_Gprs_Send[319]='0';	
uUart_Gprs_Send[320]='0';	
uUart_Gprs_Send[321]='0';	
uUart_Gprs_Send[321]=',';	
uUart_Gprs_Send[322]='T';	
uUart_Gprs_Send[323]='S';	
uUart_Gprs_Send[324]='P';	
uUart_Gprs_Send[325]='-';	
uUart_Gprs_Send[326]='F';	
uUart_Gprs_Send[327]='l';	
uUart_Gprs_Send[328]='a';	
uUart_Gprs_Send[329]='g';	
uUart_Gprs_Send[330]='=';	
uUart_Gprs_Send[331]='N';	

uUart_Gprs_Send[332]='&';	
uUart_Gprs_Send[333]='&';	


DATA_CRC=check_sum(&uUart_Gprs_Send[6],328);

sprintf(DATA_CRC_B,"%04X",DATA_CRC);

uUart_Gprs_Send[334]=DATA_CRC_B[0];	
uUart_Gprs_Send[335]=DATA_CRC_B[1];	
uUart_Gprs_Send[336]=DATA_CRC_B[2];	
uUart_Gprs_Send[337]=DATA_CRC_B[3];	

uUart_Gprs_Send[338]=0x0d;	
uUart_Gprs_Send[339]=0x0a;	



					
uUart_Send_Length=340;
uart_sendstring(&uUart_Gprs_Send[0]);

uPM25_Data_Send_Time=0;//
uCon_State_Ask_Time_No=1000;//10秒


					break;		
					case	CON_NO_ST16:


						break;					
					case	CON_NO_ST17:

						uUart_Send_Length=0;				
						uart_sendstring(st140);
						uCon_Ask_Time=CON_ASK_MAX;//应答检测	
					
						break;	
					case	CON_NO_ST18:
					uUart_Send_Length=0;								
			memset(uUart_Gprs_Send,0,400);
uUart_Gprs_Send[0]=0xBB;
uUart_Gprs_Send[1]=0x0D;					
uUart_Gprs_Send[2]=0x21;
uUart_Gprs_Send[3]=0x01;				
uUart_Gprs_Send[4]=0xF0;
uUart_Gprs_Send[5]=0x02;					
uUart_Gprs_Send[6]=0x00;			
uUart_Gprs_Send[7]=0x00;
uUart_Gprs_Send[8]=0x01;					
uUart_Gprs_Send[9]=0;					
					
					
uUart_Gprs_Send[10]==check_sum(&uUart_Gprs_Send[1],9);	
uUart_Gprs_Send[11]=0xEE;					
uUart_Gprs_Send[12]=0xFF;						
					
uUart_Send_Length=13;
uart_sendstring(&uUart_Gprs_Send[0]);


						break;						
		default:
			
			break;
		}
	}else
	{
		if(uCon_No==CON_NO_ST16)
		{
			
			if(uCon_State_Ask_Time_No>0)	
			{
				uCon_State_Ask_Time_No--;
				if(uCon_State_Ask_Time_No==0)//10S//没回复//复位
				{
						/*uCon_No=CON_NO_START0;		
						uCon_Ask_Time=0;//清零
						uPM25_Data_Send_Time=0;
					*/
				}
			}
					uPM25_Data_Send_Time++;//10ms
					if(uPM25_Data_Send_Time==1000)//1分钟
					{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//延时100ms
										uCon_No=CON_NO_ST14;	//再次发送	
										uCon_Ask_Time=0;//清零
					}
		}
	}		
	break;						
		default:
			
			break;

}
}


		delay_ms(10);

	}
	





}


