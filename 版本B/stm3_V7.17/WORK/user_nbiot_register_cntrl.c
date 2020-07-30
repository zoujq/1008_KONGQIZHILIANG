#include "includes.h"	   


 struct  SYSCNTRL_typedef Sys_Cntrl;
 struct  SYSCNTRL_typedef Sys_Cntrl_B; 
 
 

 u8  uCon_No=0;//控制号。每执行一种动作，加1
//xdata u8  uCon_No_B=0xff;//限制或重复执行动作标志
//xdata u16  uCon_Time_Delay=0;//每种动作之间延迟时间
//xdata u16  uCon_Ask_Time=0;//应答时间定时，到时间没回复，再次发送

 char * str_p;//字符串指针//
 
 u8 str_p_i=0;//数据查找计数 
 
// char * str_p_nest;//字符串指针//


//平台链接
//u8 Iot_Website[]="https://m.poscn.com/iot/linkcode?id=1_";


const char	st1[] =	{"AT+CFUN=1\r\n"};//开启射频全功能
const char	st2[] =	{"AT+NNMI=1\r\n"};//下行数据通知 
const char	st3[] =	{"AT+CGATT=1\r\n"};//自动搜网 
const char	st4[] =	{"AT+CPSMS=0\r\n"};//关闭低功耗功能
const char	st5[] =	{"AT+CGPADDR\r\n"};//查询分配的ip 地址，可以以此判断是否连接到网络
const char	st6[] =	{"AT+NUESTATS\r\n"};//查询UE 状态，包括rsrp,sinr,覆盖等级，小区信息等，可选 
 
const char  st7[]	=	{"AT+CGSN=1\r\n"};//查询IMEI 号
const char  st8[]	=	{"AT+CIMI\r\n"};//查询SIM卡信息




const char    st15[] =	{"AT+NSOCR=DGRAM,17,9502,1\r\n"};//1 为socket 号，如果有回复OK说明创建成功,UDP
//code char    st151[] =	{"AT+NSOCL=1\r\n"};//1 为socket 号，销毁socket




//iot.poscn.com
static const char    st16[] =	{",53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//公共
const char    st160[] =	{"AT+NSOST=2,223.5.5.5"};//阿里的DNS
const char    st161[] =	{"AT+NSOST=2,8.8.8.8"};//谷歌的DNS
const char    st162[] =	{"AT+NSOST=2,114.114.114.114"};//114的DNS，流氓


//code char    st160[] =	{"AT+NSOST=2,223.5.5.5,53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//阿里的DNS
//code char    st161[] =	{"AT+NSOST=2,8.8.8.8,53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//谷歌的DNS
//code char    st162[] =	{"AT+NSOST=2,114.114.114.114,53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//114的DNS，流氓


//iot1.poscn.com 
//code char    st1601[] =	{"AT+NSOST=2,223.5.5.5,53,32,3E3A0100000100000000000004696F743105706F73636E03636F6D0000010001\r\n"};//阿里的DNS
//code char    st1611[] =	{"AT+NSOST=2,8.8.8.8,53,32,3E3A0100000100000000000004696F743105706F73636E03636F6D0000010001\r\n"};//谷歌的DNS
//code char    st1621[] =	{"AT+NSOST=2,114.114.114.114,53,32,3E3A0100000100000000000004696F743105706F73636E03636F6D0000010001\r\n"};//114的DNS，流氓



 char    st17[20] ={0};	//{"AT+NSORF=1,256\r\n\0"};//
	 

const char	st29[] =	{"AT+CSQ\r\n"};//查询NB信号强度 
 
 
	u8 fSys_Work_Res_B=0;//工作状态,缓存
	u8 fSys_Work_B=0;//工作状态
	u8 fSys_Mode_B=0;//模式	
	u8 uWind_Strength_B=0;//风速
	u16 	uSys_Filter_User_Time_B=0;//滤网使用时间
	u8 	uSys_Filter_Error_State=0;//滤网故障状态
	u8 	uSys_Filter_Error_State_B=0;//滤网故障状态


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







/////////////////////////////////////////////////////////////////////////////////////////////////  注册  ///////////////////////////////////////////////////////
 u16 uRegister_Res_Time=0;//注册等待应答时间
 u8 uRegister_Res_Time_No=0;//注册等待应答时间//次数


///////////////////////////////////////////////////////////////////////////////////////  初始化，DNS访问   //////////////////////////////////////
 char uDNS_No=0;//DNS序号
 u16 uDNS_Res_Time=0;//DNS等待应答时间
 u8 uDNS_Res_Time_No=0;//DNS等待应答时间//次数




void uart_sendstring(char const *str)
{
  u16 i = 0;
	while (str[i]!= '\0')
	{
		_net.txbuf[i] = str[i];
		i++;
	}
	_net.txlen=i;	
	User_UART_Send();			
	
	Sys_Cntrl.uTime_Heard_Send_No=0;//清零	
	
}










union u32Tou8
{
	uint64_t ulongx;
	uint8_t uMac_Base[8];
}MACu32tou8;

	uint64_t SYS_TYPE_NO=0;


unsigned short UI_Show_No_B_NBIOT_Task=0xff;//


char User_NBIOT_Register_Thread(thread_t* pt)//注册任务
{
	 Thread_BEGIN
	{	
		


    GPIO_InitTypeDef  GPIO_InitStructure;  		  

	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTC时钟

	
	

//获取MAC
MACu32tou8.ulongx=*(uint64_t*) (SYS_TYPE_NO_ADDR+0x110); 
	
		
//SYS_TYPE_NO=0;	
//SYS_TYPE_NO=(uint64_t)(MACu32tou8.uMac_Base[7]);	
//SYS_TYPE_NO|=(uint64_t)(MACu32tou8.uMac_Base[6]<<8);		
//SYS_TYPE_NO|=(uint64_t)(MACu32tou8.uMac_Base[5]<<16);				
		
	SYS_TYPE_NO=SYS_TYPE_NO1;	



		while(true)
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
				
				
				
				
				
				
				uCon_No=CON_NO_START0;//控制号。复位。防止充电时，一直按开关机，出现串口没初始化就发送数据，造成死机	
				
				
			//防止充电开关机，图标还闪
			Sys_Cntrl.uSys_User_Reset=0;//系统用户重置//清零
			Sys_Cntrl.uSys_User_Reset_Time=0;//系统用户重置,计时//清零						
				
			UI_Show_No_B_NBIOT_Task=0;
			}
			 		Thread_Sleep(10);
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


				
	/*			
			GPIO_SetBits(GPIOC,GPIO_Pin_3); //打开NB电源 	
	  //USART5_RX	  PD.2	//USART5_TX   PC.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化
		*/		
				

			UI_Show_No_B_NBIOT_Task=0xff;
			
			
		Sys_Cntrl.uPlatform_No=0;//平台号,
		Sys_Cntrl.uDNS_IP_Get_No=0;//获取IP号 ,		
	//	uCon_No=CON_NO_START0;//复位			
			uCon_No=CON_NO_START0;//控制号。复位。防止充电时，一直按开关机，出现串口没初始化就发送数据，造成死机					
			
			}
						
						
			
			
			
			if(Sys_Cntrl.uPlatform_No==0)//如果为注册
			{

				

					if(uCon_No==CON_NO_START0)
					{
						uart2_uninit();		
					GPIO_ResetBits(GPIOC,GPIO_Pin_3); //供电关		
						
						uCon_No=CON_NO_START1;		

						Sys_Cntrl.uLine_Work=0;	
						Sys_Cntrl.uData_Send_No_Add=0;//清零					
						Sys_Cntrl.fNB_Open=0;//清零	

						Sys_Cntrl.fSys_User_Other_Data=1;//重新上传信号强度及定位数据						
						Sys_Cntrl.uData_Send_Time=5500;//5秒后发数据
						
						Sys_Cntrl.uRegister_Use_Time=0;//注册超时清零
						
						Thread_Sleep(5000);	//延时5S	//电全部放完						
						continue;
					}	
					if(uCon_No==CON_NO_START1)	
					{						
						GPIO_SetBits(GPIOC,GPIO_Pin_3); //打开NB电源 
						uart2_init(9600);//	  				
						uCon_No=CON_NO_ST0;		
		
						continue;
					}
					if(uCon_No==CON_NO_ST0)	
					{
						_net.time_out=1000;//10秒								
						while(strstr(_net.rxbuf,"Lierda")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST1;//下一步
						else	//重启
						{

						}
						Thread_Sleep(1000);//延时1s						
						continue;						
					}					
					if(uCon_No==CON_NO_ST1)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存					
						uart_sendstring(st1);	

						_net.time_out=1000;//10秒													
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST2;//下一步
						else	//重启
						{

						}		
						Thread_Sleep(100);//延时100ms								
						continue;
					}		
					if(uCon_No==CON_NO_ST2)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存										
						uart_sendstring(st2);	

						_net.time_out=1000;//10秒													
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST3;//下一步
						else	//重启
						{

						}			
						Thread_Sleep(5000);//延时5s才开始搜网，防止出现ERR
						continue;
					}		
					if(uCon_No==CON_NO_ST3)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存										
						uart_sendstring(st3);	

						_net.time_out=1000;//10秒													
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST4;//下一步
						else	//重启
						{

						}	
						Thread_Sleep(100);//延时100ms								
						continue;
					}		
					if(uCon_No==CON_NO_ST4)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						uart_sendstring(st4);	

						_net.time_out=1000;//10秒													
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST5;//下一步
						else	//重启
						{

						}	
						Thread_Sleep(100);//延时100ms								
						continue;
					}	
					if(uCon_No==CON_NO_ST5)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						uart_sendstring(st5);	

						_net.time_out=300;//3秒													
						while(strstr(_net.rxbuf,"+CGPADDR:0,")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST6;//下一步
						else	//重发
						{
							
						}	
						Thread_Sleep(100);//延时100ms
						continue;
					}
					if(uCon_No==CON_NO_ST6)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						uart_sendstring(st6);	

						_net.time_out=1000;//10秒													
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}				
						if(_net.time_out)	//
						{
							if((str_p=strstr(_net.rxbuf,"Signal power:"))!=0)
							{
								Sys_Cntrl.Signal_power=atol(str_p+13);
							}
							if((str_p=strstr(_net.rxbuf,"Total power:"))!=0)
							{
								Sys_Cntrl.Total_power=atol(str_p+12);
							}									
							if((str_p=strstr(_net.rxbuf,"TX power:"))!=0)
							{
								Sys_Cntrl.TX_power=atol(str_p+9);
							}									
							if((str_p=strstr(_net.rxbuf,"TX time:"))!=0)
							{
								Sys_Cntrl.TX_time=atol(str_p+8);
							}												
							if((str_p=strstr(_net.rxbuf,"RX time:"))!=0)
							{
								Sys_Cntrl.RX_time=atol(str_p+8);
							}												
							if((str_p=strstr(_net.rxbuf,"Cell ID:"))!=0)
							{
								Sys_Cntrl.Cell_ID=atol(str_p+8);
							}													
							if((str_p=strstr(_net.rxbuf,"ECL:"))!=0)
							{
								Sys_Cntrl.ECL=atol(str_p+4);
							}												
							if((str_p=strstr(_net.rxbuf,"SNR:"))!=0)
							{
								Sys_Cntrl.SNR=atol(str_p+4);
							}											
							if((str_p=strstr(_net.rxbuf,"EARFCN:"))!=0)
							{
								Sys_Cntrl.EARFCN=atol(str_p+7);
							}											
							if((str_p=strstr(_net.rxbuf,"PCI:"))!=0)
							{
								Sys_Cntrl.PCI=atol(str_p+4);
							}														
							if((str_p=strstr(_net.rxbuf,"RSRQ:"))!=0)
							{
								Sys_Cntrl.RSRQ=atol(str_p+5);
							}													
							if((str_p=strstr(_net.rxbuf,"OPERATOR MODE:"))!=0)
							{
								Sys_Cntrl.OPERATOR_MODE=atol(str_p+14);
							}													
							if((str_p=strstr(_net.rxbuf,"CURRENT BAND:"))!=0)
							{
								Sys_Cntrl.CURRENT_BAND=atol(str_p+13);
							}				
							
							Sys_Cntrl.uSend_Data_Type_No=1;//有定位数据
							uCon_No=CON_NO_ST7;//下一步

						}
						else	//重启
						{

						}	
						Thread_Sleep(100);//延时100ms
						continue;
					}
					if(uCon_No==CON_NO_ST7)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						uart_sendstring(st7);	

						_net.time_out=1000;//10秒				
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}											
						if(_net.time_out)	
						{
							if((str_p=strstr(_net.rxbuf,"+CGSN:"))!=0)
							{
								strncpy(Sys_Cntrl.CGSN,(str_p+6),15);//								
								uCon_No=CON_NO_ST8;//下一步
							}
						}
						else	//重启
						{

						}	
						Thread_Sleep(100);//延时100ms
						continue;
					}
					if(uCon_No==CON_NO_ST8)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						uart_sendstring(st8);	

						_net.time_out=1000;//10秒		
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}										
						if(_net.time_out)	
						{
							if((str_p=strstr(_net.rxbuf,"\r\n"))!=0)
							{							
								strncpy(Sys_Cntrl.CIMI,(str_p+2),15);//						
								uCon_No=CON_NO_ST15;//下一步
							}
						}
						else	//重启
						{

						}	
						Thread_Sleep(100);//延时100ms
						continue;
					}
					if(uCon_No==CON_NO_ST15)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						uart_sendstring(st15);	

						_net.time_out=1000;//10秒													
						while((str_p=strstr(_net.rxbuf,"OK"))==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	
						{
							Sys_Cntrl.uSocket_No=_net.rxbuf[2];										
							uCon_No=CON_NO_ST16;//下一步
						}
						else	//重启
						{

						}	
						Thread_Sleep(100);//延时100ms
						continue;
					}
					if(uCon_No==CON_NO_ST16)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						memset(_net.txbuf,0,_net.txbufmax);								
						if(uDNS_No==0)
						{
							Sys_Cntrl.Data_Buf_i=strlen(st160);
							strncpy(_net.txbuf,st160,Sys_Cntrl.Data_Buf_i);			
							uDNS_No=1;
						}else
						if(uDNS_No==1)
						{
							Sys_Cntrl.Data_Buf_i=strlen(st161);
							strncpy(_net.txbuf,st161,Sys_Cntrl.Data_Buf_i);		
							uDNS_No=2;
						}else
						if(uDNS_No==2)
						{
							Sys_Cntrl.Data_Buf_i=strlen(st162);
							strncpy(_net.txbuf,st162,Sys_Cntrl.Data_Buf_i);		
							uDNS_No=0;
						}
						_net.txbuf[9]=Sys_Cntrl.uSocket_No;			
						strcat(_net.txbuf,st16);
						_net.txlen=strlen(_net.txbuf);		
						User_UART_Send();													
						
						_net.time_out=1000;//10秒													
						while((str_p=strstr(_net.rxbuf,"+NSONMI"))==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	
						{								
							uCon_No=CON_NO_ST17;//下一步
						}
						else	//重启
						{

						}	
						Thread_Sleep(100);//延时100ms
						continue;
					}
					if(uCon_No==CON_NO_ST17)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						memset(st17,0,20);
						strcpy(st17,"AT+NSORF=1,256\r\n");
						st17[9]=Sys_Cntrl.uSocket_No;
						uart_sendstring(st17);

						_net.time_out=1000;//10秒			
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}								
						if(_net.time_out)	
						{
							if((str_p=strstr(_net.rxbuf,"3E3A"))!=0)
							{
								for(str_p_i=0;str_p_i<200;str_p_i++)
								{
									if((*(str_p+str_p_i))==',')	
									{

										Sys_Cntrl.uDNS_IP_GET[0]=CTOH(str_p+str_p_i-8);
										Sys_Cntrl.uDNS_IP_GET[1]=CTOH(str_p+str_p_i-6);
										Sys_Cntrl.uDNS_IP_GET[2]=CTOH(str_p+str_p_i-4);
										Sys_Cntrl.uDNS_IP_GET[3]=CTOH(str_p+str_p_i-2);						
										uCon_No=CON_NO_ST18;//下一步
								
									
										break;
									}
								
								}
								//之前方法会导致如果网络不好，连续接收到2次数据，查找指针会出错
//								if((str_p_nest=strstr(str_p,","))!=0)
//								{
//									Sys_Cntrl.uDNS_IP_GET[0]=CTOH(str_p_nest-8);
//									Sys_Cntrl.uDNS_IP_GET[1]=CTOH(str_p_nest-6);
//									Sys_Cntrl.uDNS_IP_GET[2]=CTOH(str_p_nest-4);
//									Sys_Cntrl.uDNS_IP_GET[3]=CTOH(str_p_nest-2);						
//									uCon_No=CON_NO_ST18;//下一步
//								}
							}else	//重启
							{

							}			
						}
						else	//重启
						{

						}	
						Thread_Sleep(100);//延时100ms
						continue;
					}
					if(uCon_No==CON_NO_ST18)	
					{
						
						
						uRegister_Res_Time_No++;
						if(uRegister_Res_Time_No>6)//次数超过6次//不注册//跳回自己平台
						{
							uRegister_Res_Time_No=0;//
								uRegister_Res_Time=0;//应答检测	//10秒		
							Sys_Cntrl.uPlatform_No=1;//自己平台
						//	Sys_Cntrl.uPlatform_No=2;//OneNET
							
							uCon_No=CON_NO_START0;		
							continue;

						}								
											
						
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						memset(_net.txbuf,0,_net.txbufmax);
						strcpy(_net.txbuf,"AT+NSOST=2,");
						_net.txbuf[9]=Sys_Cntrl.uSocket_No;					
						sprintf(&_net.txbuf[11],"%d.%d.%d.%d,5683,49,",(unsigned int)Sys_Cntrl.uDNS_IP_GET[0],(unsigned int)Sys_Cntrl.uDNS_IP_GET[1],(unsigned int)Sys_Cntrl.uDNS_IP_GET[2],(unsigned int)Sys_Cntrl.uDNS_IP_GET[3]);							
					
						fUart_Send_End=1;
						_net.txlen=strlen(_net.txbuf);									
						User_UART_Send();	
						while(fUart_Send_End);

						//数据头					
						Sys_Cntrl.uUart_Data_De_Coding[0]=0x14;
						//有效数据长度
						Sys_Cntrl.uUart_Data_De_Coding[1]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[2]=0x31;		

						//校验位
						//Sys_Cntrl.uUart_Data_De_Coding[3]='0';
						//Sys_Cntrl.uUart_Data_De_Coding[4]='0';									
						//Sys_Cntrl.uUart_Data_De_Coding[5]='0';
						//Sys_Cntrl.uUart_Data_De_Coding[6]='0';

						//校验方法
						Sys_Cntrl.uUart_Data_De_Coding[7]=0x01;
						//加密方法
						Sys_Cntrl.uUart_Data_De_Coding[8]=0x00;

												
						//设备序号
						Sys_Cntrl.uUart_Data_De_Coding[9]=0x01;
						Sys_Cntrl.uUart_Data_De_Coding[10]=SYS_EDITION;
						//序号:第一个字节为版本号
						//例01：硬件版本1，软件版本1
						//
						//
						Sys_Cntrl.uUart_Data_De_Coding[11]=((SYS_TYPE_NO&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[12]=((SYS_TYPE_NO&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[13]=(SYS_TYPE_NO&0xff);
						//通讯类型
						Sys_Cntrl.uUart_Data_De_Coding[14]=0x29;
						Sys_Cntrl.uUart_Data_De_Coding[15]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[16]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[17]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[18]=((SYS_TYPE&0xff00)>>8);
						//产品类型
						Sys_Cntrl.uUart_Data_De_Coding[19]=0x2A;
						Sys_Cntrl.uUart_Data_De_Coding[20]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[21]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[22]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[23]=(SYS_TYPE&0xff);
						//数据类型
						Sys_Cntrl.uUart_Data_De_Coding[24]=0x3C;
						Sys_Cntrl.uUart_Data_De_Coding[25]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[26]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[27]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[28]=0x02;

						//IMEI号（前8位）
						Sys_Cntrl.uUart_Data_De_Coding[29]=0x50;
						Sys_Cntrl.uUart_Data_De_Coding[30]=CTOH(&Sys_Cntrl.CGSN[0]);
						Sys_Cntrl.uUart_Data_De_Coding[31]=CTOH(&Sys_Cntrl.CGSN[2]);
						Sys_Cntrl.uUart_Data_De_Coding[32]=CTOH(&Sys_Cntrl.CGSN[4]);
						Sys_Cntrl.uUart_Data_De_Coding[33]=CTOH(&Sys_Cntrl.CGSN[6]);
						//IMEI号（后7位）
						Sys_Cntrl.uUart_Data_De_Coding[34]=0x51;
						Sys_Cntrl.uUart_Data_De_Coding[35]=(CTOH(&Sys_Cntrl.CGSN[7])&0x0f);
						Sys_Cntrl.uUart_Data_De_Coding[36]=CTOH(&Sys_Cntrl.CGSN[9]);
						Sys_Cntrl.uUart_Data_De_Coding[37]=CTOH(&Sys_Cntrl.CGSN[11]);
						Sys_Cntrl.uUart_Data_De_Coding[38]=CTOH(&Sys_Cntrl.CGSN[13]);
						//CIMI号（前8位）
						Sys_Cntrl.uUart_Data_De_Coding[39]=0x52;
						Sys_Cntrl.uUart_Data_De_Coding[40]=CTOH(&Sys_Cntrl.CIMI[0]);
						Sys_Cntrl.uUart_Data_De_Coding[41]=CTOH(&Sys_Cntrl.CIMI[2]);
						Sys_Cntrl.uUart_Data_De_Coding[42]=CTOH(&Sys_Cntrl.CIMI[4]);
						Sys_Cntrl.uUart_Data_De_Coding[43]=CTOH(&Sys_Cntrl.CIMI[6]);
						//CIMI号（后7位）
						Sys_Cntrl.uUart_Data_De_Coding[44]=0x53;
						Sys_Cntrl.uUart_Data_De_Coding[45]=(CTOH(&Sys_Cntrl.CIMI[7])&0x0f);
						Sys_Cntrl.uUart_Data_De_Coding[46]=CTOH(&Sys_Cntrl.CIMI[9]);
						Sys_Cntrl.uUart_Data_De_Coding[47]=CTOH(&Sys_Cntrl.CIMI[11]);
						Sys_Cntrl.uUart_Data_De_Coding[48]=CTOH(&Sys_Cntrl.CIMI[13]);

						crc_check=GetCrC((unsigned char *)&Sys_Cntrl.uUart_Data_De_Coding[7],42);
								
						//校验位
						Sys_Cntrl.uUart_Data_De_Coding[3]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[4]=0x00;							
						Sys_Cntrl.uUart_Data_De_Coding[5]=((crc_check&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[6]=(crc_check&0xff);
														
	
						for(Sys_Cntrl.Data_Buf_i=0;Sys_Cntrl.Data_Buf_i<49;Sys_Cntrl.Data_Buf_i++)
						{
							_net.txbuf[2*Sys_Cntrl.Data_Buf_i]=HTOC(((Sys_Cntrl.uUart_Data_De_Coding[Sys_Cntrl.Data_Buf_i])&0xf0)>>4);		
							_net.txbuf[2*Sys_Cntrl.Data_Buf_i+1]=HTOC((Sys_Cntrl.uUart_Data_De_Coding[Sys_Cntrl.Data_Buf_i])&0xf);		
						}	
								
						_net.txbuf[2*Sys_Cntrl.Data_Buf_i]=0x0D;	
						_net.txbuf[2*Sys_Cntrl.Data_Buf_i+1]=0x0A;	
						_net.txbuf[2*Sys_Cntrl.Data_Buf_i+2]=0;					
						Sys_Cntrl.Data_Buf_strlen=strlen(_net.txbuf);	
							
						_net.txlen=Sys_Cntrl.Data_Buf_strlen;
						User_UART_Send();				
						
									
						_net.time_out=1000;//10秒													
						while((str_p=strstr(_net.rxbuf,"+NSONMI"))==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	
						{								
							uRegister_Res_Time=0;	//已返回注册数据，清零			
							uCon_No=CON_NO_ST19;//下一步
						}
						else	//超时，重新读取一下数据
						{
							memset(st17,0,20);
							strcpy(st17,"AT+NSORF=1,256\r\n");
							st17[9]=Sys_Cntrl.uSocket_No;
							uart_sendstring(st17);
						}	
						Thread_Sleep(100);//延时100ms
						continue;
					}					
					if(uCon_No==CON_NO_ST19)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						memset(st17,0,20);
						strcpy(st17,"AT+NSORF=1,256\r\n");
						st17[9]=Sys_Cntrl.uSocket_No;
						uart_sendstring(st17);

						_net.time_out=1000;//10秒			
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}																
						if(_net.time_out)	
						{						
							if((str_p=strstr(_net.rxbuf,"1400"))!=0)
							{								
								
								Sys_Cntrl.uData_Send_No_Add=0;//清零								
								
								//长度
								Sys_Cntrl.uUart_Data_De_Coding[0]=CTOH((str_p+4));	
								Sys_Cntrl.uUart_Data_De_Coding[0]-=7;
								//CRC
								Sys_Cntrl.uUart_Data_De_Coding[1]=CTOH((str_p+6));		
								Sys_Cntrl.uUart_Data_De_Coding[2]=CTOH((str_p+8));		
								Sys_Cntrl.uUart_Data_De_Coding[3]=CTOH((str_p+10));		
								Sys_Cntrl.uUart_Data_De_Coding[4]=CTOH((str_p+12));											

								for(Sys_Cntrl.Data_Buf_i=0;Sys_Cntrl.Data_Buf_i<Sys_Cntrl.uUart_Data_De_Coding[0];Sys_Cntrl.Data_Buf_i++)
								{
									Sys_Cntrl.uUart_Data_De_Coding[5+Sys_Cntrl.Data_Buf_i]=CTOH((str_p+14+Sys_Cntrl.Data_Buf_i*2));										
								}

								if(Sys_Cntrl.uUart_Data_De_Coding[5]==0x01)//CRC16
								{
									crc_check_res=(Sys_Cntrl.uUart_Data_De_Coding[1]<<24);
									crc_check_res|=(Sys_Cntrl.uUart_Data_De_Coding[2]<<16);	
									crc_check_res|=(Sys_Cntrl.uUart_Data_De_Coding[3]<<8);	
									crc_check_res|=(Sys_Cntrl.uUart_Data_De_Coding[4]<<0);	
									crc_check=GetCrC((unsigned char *)&Sys_Cntrl.uUart_Data_De_Coding[5],Sys_Cntrl.Data_Buf_i);
									if(crc_check_res==crc_check)
									{
										for(Sys_Cntrl.Data_Buf_i=0;Sys_Cntrl.Data_Buf_i<Sys_Cntrl.uUart_Data_De_Coding[0];Sys_Cntrl.Data_Buf_i+=5)
										{
											if(Sys_Cntrl.uUart_Data_De_Coding[7+Sys_Cntrl.Data_Buf_i]==0x54)//下发的平台
											{													
													Sys_Cntrl.uPlatform_No=Sys_Cntrl.uUart_Data_De_Coding[7+Sys_Cntrl.Data_Buf_i+4];//平台号							
											//	Sys_Cntrl.uPlatform_No=1;//自己平台
													
												
												//uCon_No=CON_NO_START0;//复位
												Sys_Cntrl.uLine_Work=0;//			
												if(Sys_Cntrl.uPlatform_No==2)
												{								
													Thread_Sleep(200);//延时200ms	//防止串口接收数据没从头开始												
													//销毁建立的连接						
													memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
													memset(st17,0,20);
													strcpy(st17,"AT+NSOCL=1\r\n");
													st17[9]=Sys_Cntrl.uSocket_No;
													uart_sendstring(st17);

													_net.time_out=1000;//10秒			
													while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//等待串口接收完毕或超时退出
													{		
														Thread_Sleep(10);
													}																
													if(_net.time_out)	
													{
															uCon_No=CON_NO_ST29;//下一步
															Sys_Cntrl.fNB_Open=0;//复位
																				
													}
													else	//重启
													{

													}	
													Thread_Sleep(200);//延时200ms
													continue;									
												}else	if(Sys_Cntrl.uPlatform_No==1)
												{
													//Thread_Sleep(200);//延时200ms	//防止串口接收数据没从头开始	
													uCon_No=CON_NO_ST16;//下一步
													continue;	
												}												
											}			
										}
									}					
								}	
							}							
						}
						else	//重启
						{

						}	
						Thread_Sleep(100);//延时100ms
						continue;
					}					
					
			
																	
				


					
				}
			
			}
				Thread_Sleep(10);
		
		}
	}
	Thread_END
}









char User_NBIOT_Register_Scan_Thread(thread_t* pt)//注册时,常监测
{

	 Thread_BEGIN
	{

			while(true)
			{
				
				if(Sys_Cntrl.uPlatform_No>2)//如果异常号
				{		
						Sys_Cntrl.uPlatform_No=0;//复位
					
						Sys_Cntrl.uRegister_Use_Time=0;//重启		
						Sys_Cntrl.uTime_Heard_No=0;//重启			
						Sys_Cntrl.uTime_Heard_Send_No=0;//重启				
						uCon_No=CON_NO_START0;		
						Sys_Cntrl.uLine_Work=0;//					

				}					
								
				
				
				if(Sys_Cntrl.uPlatform_No==0&&CMachine.UI_Show_No>1)//如果为注册
				{
					//防止异常，注册时间过长
					Sys_Cntrl.uRegister_Use_Time++;
					if(Sys_Cntrl.uRegister_Use_Time>=18000)//注册超过3分钟
					{
						Sys_Cntrl.uRegister_Use_Time=0;//重启		
						Sys_Cntrl.uTime_Heard_No=0;//重启			
						Sys_Cntrl.uTime_Heard_Send_No=0;//重启				
						uCon_No=CON_NO_START0;		
						Sys_Cntrl.uLine_Work=0;//
					}		
				}else
				{
					Sys_Cntrl.uRegister_Use_Time=0;//清零
				}				
				Thread_Sleep(10);
			}
		}
		Thread_END
}

















