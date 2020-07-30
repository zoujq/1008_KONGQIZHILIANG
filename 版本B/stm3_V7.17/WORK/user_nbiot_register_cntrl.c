#include "includes.h"	   


 struct  SYSCNTRL_typedef Sys_Cntrl;
 struct  SYSCNTRL_typedef Sys_Cntrl_B; 
 
 

 u8  uCon_No=0;//���ƺš�ÿִ��һ�ֶ�������1
//xdata u8  uCon_No_B=0xff;//���ƻ��ظ�ִ�ж�����־
//xdata u16  uCon_Time_Delay=0;//ÿ�ֶ���֮���ӳ�ʱ��
//xdata u16  uCon_Ask_Time=0;//Ӧ��ʱ�䶨ʱ����ʱ��û�ظ����ٴη���

 char * str_p;//�ַ���ָ��//
 
 u8 str_p_i=0;//���ݲ��Ҽ��� 
 
// char * str_p_nest;//�ַ���ָ��//


//ƽ̨����
//u8 Iot_Website[]="https://m.poscn.com/iot/linkcode?id=1_";


const char	st1[] =	{"AT+CFUN=1\r\n"};//������Ƶȫ����
const char	st2[] =	{"AT+NNMI=1\r\n"};//��������֪ͨ 
const char	st3[] =	{"AT+CGATT=1\r\n"};//�Զ����� 
const char	st4[] =	{"AT+CPSMS=0\r\n"};//�رյ͹��Ĺ���
const char	st5[] =	{"AT+CGPADDR\r\n"};//��ѯ�����ip ��ַ�������Դ��ж��Ƿ����ӵ�����
const char	st6[] =	{"AT+NUESTATS\r\n"};//��ѯUE ״̬������rsrp,sinr,���ǵȼ���С����Ϣ�ȣ���ѡ 
 
const char  st7[]	=	{"AT+CGSN=1\r\n"};//��ѯIMEI ��
const char  st8[]	=	{"AT+CIMI\r\n"};//��ѯSIM����Ϣ




const char    st15[] =	{"AT+NSOCR=DGRAM,17,9502,1\r\n"};//1 Ϊsocket �ţ�����лظ�OK˵�������ɹ�,UDP
//code char    st151[] =	{"AT+NSOCL=1\r\n"};//1 Ϊsocket �ţ�����socket




//iot.poscn.com
static const char    st16[] =	{",53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//����
const char    st160[] =	{"AT+NSOST=2,223.5.5.5"};//�����DNS
const char    st161[] =	{"AT+NSOST=2,8.8.8.8"};//�ȸ��DNS
const char    st162[] =	{"AT+NSOST=2,114.114.114.114"};//114��DNS����å


//code char    st160[] =	{"AT+NSOST=2,223.5.5.5,53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//�����DNS
//code char    st161[] =	{"AT+NSOST=2,8.8.8.8,53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//�ȸ��DNS
//code char    st162[] =	{"AT+NSOST=2,114.114.114.114,53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//114��DNS����å


//iot1.poscn.com 
//code char    st1601[] =	{"AT+NSOST=2,223.5.5.5,53,32,3E3A0100000100000000000004696F743105706F73636E03636F6D0000010001\r\n"};//�����DNS
//code char    st1611[] =	{"AT+NSOST=2,8.8.8.8,53,32,3E3A0100000100000000000004696F743105706F73636E03636F6D0000010001\r\n"};//�ȸ��DNS
//code char    st1621[] =	{"AT+NSOST=2,114.114.114.114,53,32,3E3A0100000100000000000004696F743105706F73636E03636F6D0000010001\r\n"};//114��DNS����å



 char    st17[20] ={0};	//{"AT+NSORF=1,256\r\n\0"};//
	 

const char	st29[] =	{"AT+CSQ\r\n"};//��ѯNB�ź�ǿ�� 
 
 
	u8 fSys_Work_Res_B=0;//����״̬,����
	u8 fSys_Work_B=0;//����״̬
	u8 fSys_Mode_B=0;//ģʽ	
	u8 uWind_Strength_B=0;//����
	u16 	uSys_Filter_User_Time_B=0;//����ʹ��ʱ��
	u8 	uSys_Filter_Error_State=0;//��������״̬
	u8 	uSys_Filter_Error_State_B=0;//��������״̬


/************************************************************************************
*��������:check_sum
*��������:��������У��
*��ڲ���:@data,У�����ݵ���ʼ��ַ   @len,У�����ݵĳ���
*����ֵ:����У��ֵ
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







/////////////////////////////////////////////////////////////////////////////////////////////////  ע��  ///////////////////////////////////////////////////////
 u16 uRegister_Res_Time=0;//ע��ȴ�Ӧ��ʱ��
 u8 uRegister_Res_Time_No=0;//ע��ȴ�Ӧ��ʱ��//����


///////////////////////////////////////////////////////////////////////////////////////  ��ʼ����DNS����   //////////////////////////////////////
 char uDNS_No=0;//DNS���
 u16 uDNS_Res_Time=0;//DNS�ȴ�Ӧ��ʱ��
 u8 uDNS_Res_Time_No=0;//DNS�ȴ�Ӧ��ʱ��//����




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
	
	Sys_Cntrl.uTime_Heard_Send_No=0;//����	
	
}










union u32Tou8
{
	uint64_t ulongx;
	uint8_t uMac_Base[8];
}MACu32tou8;

	uint64_t SYS_TYPE_NO=0;


unsigned short UI_Show_No_B_NBIOT_Task=0xff;//


char User_NBIOT_Register_Thread(thread_t* pt)//ע������
{
	 Thread_BEGIN
	{	
		


    GPIO_InitTypeDef  GPIO_InitStructure;  		  

	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTCʱ��

	
	

//��ȡMAC
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
				
   

		//״̬������		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PA.1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA1
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);         				
				
				
		//��Դʹ�ܽ�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //PC.3
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PC3
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);    				
				
				
				
				
				
				
				uCon_No=CON_NO_START0;//���ƺš���λ����ֹ���ʱ��һֱ�����ػ������ִ���û��ʼ���ͷ������ݣ��������	
				
				
			//��ֹ��翪�ػ���ͼ�껹��
			Sys_Cntrl.uSys_User_Reset=0;//ϵͳ�û�����//����
			Sys_Cntrl.uSys_User_Reset_Time=0;//ϵͳ�û�����,��ʱ//����						
				
			UI_Show_No_B_NBIOT_Task=0;
			}
			 		Thread_Sleep(10);
					continue;//�˳�
			
			
		}else
		{
		
			
			if(UI_Show_No_B_NBIOT_Task==0)
			{
			
		
		//״̬������		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PA.1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA1
 
				
		//��Դʹ�ܽ�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //PC.3
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PC3
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);    		


				
	/*			
			GPIO_SetBits(GPIOC,GPIO_Pin_3); //��NB��Դ 	
	  //USART5_RX	  PD.2	//USART5_TX   PC.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��
		*/		
				

			UI_Show_No_B_NBIOT_Task=0xff;
			
			
		Sys_Cntrl.uPlatform_No=0;//ƽ̨��,
		Sys_Cntrl.uDNS_IP_Get_No=0;//��ȡIP�� ,		
	//	uCon_No=CON_NO_START0;//��λ			
			uCon_No=CON_NO_START0;//���ƺš���λ����ֹ���ʱ��һֱ�����ػ������ִ���û��ʼ���ͷ������ݣ��������					
			
			}
						
						
			
			
			
			if(Sys_Cntrl.uPlatform_No==0)//���Ϊע��
			{

				

					if(uCon_No==CON_NO_START0)
					{
						uart2_uninit();		
					GPIO_ResetBits(GPIOC,GPIO_Pin_3); //�����		
						
						uCon_No=CON_NO_START1;		

						Sys_Cntrl.uLine_Work=0;	
						Sys_Cntrl.uData_Send_No_Add=0;//����					
						Sys_Cntrl.fNB_Open=0;//����	

						Sys_Cntrl.fSys_User_Other_Data=1;//�����ϴ��ź�ǿ�ȼ���λ����						
						Sys_Cntrl.uData_Send_Time=5500;//5�������
						
						Sys_Cntrl.uRegister_Use_Time=0;//ע�ᳬʱ����
						
						Thread_Sleep(5000);	//��ʱ5S	//��ȫ������						
						continue;
					}	
					if(uCon_No==CON_NO_START1)	
					{						
						GPIO_SetBits(GPIOC,GPIO_Pin_3); //��NB��Դ 
						uart2_init(9600);//	  				
						uCon_No=CON_NO_ST0;		
		
						continue;
					}
					if(uCon_No==CON_NO_ST0)	
					{
						_net.time_out=1000;//10��								
						while(strstr(_net.rxbuf,"Lierda")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST1;//��һ��
						else	//����
						{

						}
						Thread_Sleep(1000);//��ʱ1s						
						continue;						
					}					
					if(uCon_No==CON_NO_ST1)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�					
						uart_sendstring(st1);	

						_net.time_out=1000;//10��													
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST2;//��һ��
						else	//����
						{

						}		
						Thread_Sleep(100);//��ʱ100ms								
						continue;
					}		
					if(uCon_No==CON_NO_ST2)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�										
						uart_sendstring(st2);	

						_net.time_out=1000;//10��													
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST3;//��һ��
						else	//����
						{

						}			
						Thread_Sleep(5000);//��ʱ5s�ſ�ʼ��������ֹ����ERR
						continue;
					}		
					if(uCon_No==CON_NO_ST3)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�										
						uart_sendstring(st3);	

						_net.time_out=1000;//10��													
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST4;//��һ��
						else	//����
						{

						}	
						Thread_Sleep(100);//��ʱ100ms								
						continue;
					}		
					if(uCon_No==CON_NO_ST4)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
						uart_sendstring(st4);	

						_net.time_out=1000;//10��													
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST5;//��һ��
						else	//����
						{

						}	
						Thread_Sleep(100);//��ʱ100ms								
						continue;
					}	
					if(uCon_No==CON_NO_ST5)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
						uart_sendstring(st5);	

						_net.time_out=300;//3��													
						while(strstr(_net.rxbuf,"+CGPADDR:0,")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	uCon_No=CON_NO_ST6;//��һ��
						else	//�ط�
						{
							
						}	
						Thread_Sleep(100);//��ʱ100ms
						continue;
					}
					if(uCon_No==CON_NO_ST6)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
						uart_sendstring(st6);	

						_net.time_out=1000;//10��													
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
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
							
							Sys_Cntrl.uSend_Data_Type_No=1;//�ж�λ����
							uCon_No=CON_NO_ST7;//��һ��

						}
						else	//����
						{

						}	
						Thread_Sleep(100);//��ʱ100ms
						continue;
					}
					if(uCon_No==CON_NO_ST7)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
						uart_sendstring(st7);	

						_net.time_out=1000;//10��				
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}											
						if(_net.time_out)	
						{
							if((str_p=strstr(_net.rxbuf,"+CGSN:"))!=0)
							{
								strncpy(Sys_Cntrl.CGSN,(str_p+6),15);//								
								uCon_No=CON_NO_ST8;//��һ��
							}
						}
						else	//����
						{

						}	
						Thread_Sleep(100);//��ʱ100ms
						continue;
					}
					if(uCon_No==CON_NO_ST8)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
						uart_sendstring(st8);	

						_net.time_out=1000;//10��		
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}										
						if(_net.time_out)	
						{
							if((str_p=strstr(_net.rxbuf,"\r\n"))!=0)
							{							
								strncpy(Sys_Cntrl.CIMI,(str_p+2),15);//						
								uCon_No=CON_NO_ST15;//��һ��
							}
						}
						else	//����
						{

						}	
						Thread_Sleep(100);//��ʱ100ms
						continue;
					}
					if(uCon_No==CON_NO_ST15)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
						uart_sendstring(st15);	

						_net.time_out=1000;//10��													
						while((str_p=strstr(_net.rxbuf,"OK"))==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	
						{
							Sys_Cntrl.uSocket_No=_net.rxbuf[2];										
							uCon_No=CON_NO_ST16;//��һ��
						}
						else	//����
						{

						}	
						Thread_Sleep(100);//��ʱ100ms
						continue;
					}
					if(uCon_No==CON_NO_ST16)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
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
						
						_net.time_out=1000;//10��													
						while((str_p=strstr(_net.rxbuf,"+NSONMI"))==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	
						{								
							uCon_No=CON_NO_ST17;//��һ��
						}
						else	//����
						{

						}	
						Thread_Sleep(100);//��ʱ100ms
						continue;
					}
					if(uCon_No==CON_NO_ST17)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
						memset(st17,0,20);
						strcpy(st17,"AT+NSORF=1,256\r\n");
						st17[9]=Sys_Cntrl.uSocket_No;
						uart_sendstring(st17);

						_net.time_out=1000;//10��			
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
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
										uCon_No=CON_NO_ST18;//��һ��
								
									
										break;
									}
								
								}
								//֮ǰ�����ᵼ��������粻�ã��������յ�2�����ݣ�����ָ������
//								if((str_p_nest=strstr(str_p,","))!=0)
//								{
//									Sys_Cntrl.uDNS_IP_GET[0]=CTOH(str_p_nest-8);
//									Sys_Cntrl.uDNS_IP_GET[1]=CTOH(str_p_nest-6);
//									Sys_Cntrl.uDNS_IP_GET[2]=CTOH(str_p_nest-4);
//									Sys_Cntrl.uDNS_IP_GET[3]=CTOH(str_p_nest-2);						
//									uCon_No=CON_NO_ST18;//��һ��
//								}
							}else	//����
							{

							}			
						}
						else	//����
						{

						}	
						Thread_Sleep(100);//��ʱ100ms
						continue;
					}
					if(uCon_No==CON_NO_ST18)	
					{
						
						
						uRegister_Res_Time_No++;
						if(uRegister_Res_Time_No>6)//��������6��//��ע��//�����Լ�ƽ̨
						{
							uRegister_Res_Time_No=0;//
								uRegister_Res_Time=0;//Ӧ����	//10��		
							Sys_Cntrl.uPlatform_No=1;//�Լ�ƽ̨
						//	Sys_Cntrl.uPlatform_No=2;//OneNET
							
							uCon_No=CON_NO_START0;		
							continue;

						}								
											
						
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
						memset(_net.txbuf,0,_net.txbufmax);
						strcpy(_net.txbuf,"AT+NSOST=2,");
						_net.txbuf[9]=Sys_Cntrl.uSocket_No;					
						sprintf(&_net.txbuf[11],"%d.%d.%d.%d,5683,49,",(unsigned int)Sys_Cntrl.uDNS_IP_GET[0],(unsigned int)Sys_Cntrl.uDNS_IP_GET[1],(unsigned int)Sys_Cntrl.uDNS_IP_GET[2],(unsigned int)Sys_Cntrl.uDNS_IP_GET[3]);							
					
						fUart_Send_End=1;
						_net.txlen=strlen(_net.txbuf);									
						User_UART_Send();	
						while(fUart_Send_End);

						//����ͷ					
						Sys_Cntrl.uUart_Data_De_Coding[0]=0x14;
						//��Ч���ݳ���
						Sys_Cntrl.uUart_Data_De_Coding[1]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[2]=0x31;		

						//У��λ
						//Sys_Cntrl.uUart_Data_De_Coding[3]='0';
						//Sys_Cntrl.uUart_Data_De_Coding[4]='0';									
						//Sys_Cntrl.uUart_Data_De_Coding[5]='0';
						//Sys_Cntrl.uUart_Data_De_Coding[6]='0';

						//У�鷽��
						Sys_Cntrl.uUart_Data_De_Coding[7]=0x01;
						//���ܷ���
						Sys_Cntrl.uUart_Data_De_Coding[8]=0x00;

												
						//�豸���
						Sys_Cntrl.uUart_Data_De_Coding[9]=0x01;
						Sys_Cntrl.uUart_Data_De_Coding[10]=SYS_EDITION;
						//���:��һ���ֽ�Ϊ�汾��
						//��01��Ӳ���汾1������汾1
						//
						//
						Sys_Cntrl.uUart_Data_De_Coding[11]=((SYS_TYPE_NO&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[12]=((SYS_TYPE_NO&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[13]=(SYS_TYPE_NO&0xff);
						//ͨѶ����
						Sys_Cntrl.uUart_Data_De_Coding[14]=0x29;
						Sys_Cntrl.uUart_Data_De_Coding[15]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[16]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[17]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[18]=((SYS_TYPE&0xff00)>>8);
						//��Ʒ����
						Sys_Cntrl.uUart_Data_De_Coding[19]=0x2A;
						Sys_Cntrl.uUart_Data_De_Coding[20]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[21]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[22]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[23]=(SYS_TYPE&0xff);
						//��������
						Sys_Cntrl.uUart_Data_De_Coding[24]=0x3C;
						Sys_Cntrl.uUart_Data_De_Coding[25]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[26]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[27]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[28]=0x02;

						//IMEI�ţ�ǰ8λ��
						Sys_Cntrl.uUart_Data_De_Coding[29]=0x50;
						Sys_Cntrl.uUart_Data_De_Coding[30]=CTOH(&Sys_Cntrl.CGSN[0]);
						Sys_Cntrl.uUart_Data_De_Coding[31]=CTOH(&Sys_Cntrl.CGSN[2]);
						Sys_Cntrl.uUart_Data_De_Coding[32]=CTOH(&Sys_Cntrl.CGSN[4]);
						Sys_Cntrl.uUart_Data_De_Coding[33]=CTOH(&Sys_Cntrl.CGSN[6]);
						//IMEI�ţ���7λ��
						Sys_Cntrl.uUart_Data_De_Coding[34]=0x51;
						Sys_Cntrl.uUart_Data_De_Coding[35]=(CTOH(&Sys_Cntrl.CGSN[7])&0x0f);
						Sys_Cntrl.uUart_Data_De_Coding[36]=CTOH(&Sys_Cntrl.CGSN[9]);
						Sys_Cntrl.uUart_Data_De_Coding[37]=CTOH(&Sys_Cntrl.CGSN[11]);
						Sys_Cntrl.uUart_Data_De_Coding[38]=CTOH(&Sys_Cntrl.CGSN[13]);
						//CIMI�ţ�ǰ8λ��
						Sys_Cntrl.uUart_Data_De_Coding[39]=0x52;
						Sys_Cntrl.uUart_Data_De_Coding[40]=CTOH(&Sys_Cntrl.CIMI[0]);
						Sys_Cntrl.uUart_Data_De_Coding[41]=CTOH(&Sys_Cntrl.CIMI[2]);
						Sys_Cntrl.uUart_Data_De_Coding[42]=CTOH(&Sys_Cntrl.CIMI[4]);
						Sys_Cntrl.uUart_Data_De_Coding[43]=CTOH(&Sys_Cntrl.CIMI[6]);
						//CIMI�ţ���7λ��
						Sys_Cntrl.uUart_Data_De_Coding[44]=0x53;
						Sys_Cntrl.uUart_Data_De_Coding[45]=(CTOH(&Sys_Cntrl.CIMI[7])&0x0f);
						Sys_Cntrl.uUart_Data_De_Coding[46]=CTOH(&Sys_Cntrl.CIMI[9]);
						Sys_Cntrl.uUart_Data_De_Coding[47]=CTOH(&Sys_Cntrl.CIMI[11]);
						Sys_Cntrl.uUart_Data_De_Coding[48]=CTOH(&Sys_Cntrl.CIMI[13]);

						crc_check=GetCrC((unsigned char *)&Sys_Cntrl.uUart_Data_De_Coding[7],42);
								
						//У��λ
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
						
									
						_net.time_out=1000;//10��													
						while((str_p=strstr(_net.rxbuf,"+NSONMI"))==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	
						{								
							uRegister_Res_Time=0;	//�ѷ���ע�����ݣ�����			
							uCon_No=CON_NO_ST19;//��һ��
						}
						else	//��ʱ�����¶�ȡһ������
						{
							memset(st17,0,20);
							strcpy(st17,"AT+NSORF=1,256\r\n");
							st17[9]=Sys_Cntrl.uSocket_No;
							uart_sendstring(st17);
						}	
						Thread_Sleep(100);//��ʱ100ms
						continue;
					}					
					if(uCon_No==CON_NO_ST19)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
						memset(st17,0,20);
						strcpy(st17,"AT+NSORF=1,256\r\n");
						st17[9]=Sys_Cntrl.uSocket_No;
						uart_sendstring(st17);

						_net.time_out=1000;//10��			
						while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
						{		
							Thread_Sleep(10);
						}																
						if(_net.time_out)	
						{						
							if((str_p=strstr(_net.rxbuf,"1400"))!=0)
							{								
								
								Sys_Cntrl.uData_Send_No_Add=0;//����								
								
								//����
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
											if(Sys_Cntrl.uUart_Data_De_Coding[7+Sys_Cntrl.Data_Buf_i]==0x54)//�·���ƽ̨
											{													
													Sys_Cntrl.uPlatform_No=Sys_Cntrl.uUart_Data_De_Coding[7+Sys_Cntrl.Data_Buf_i+4];//ƽ̨��							
											//	Sys_Cntrl.uPlatform_No=1;//�Լ�ƽ̨
													
												
												//uCon_No=CON_NO_START0;//��λ
												Sys_Cntrl.uLine_Work=0;//			
												if(Sys_Cntrl.uPlatform_No==2)
												{								
													Thread_Sleep(200);//��ʱ200ms	//��ֹ���ڽ�������û��ͷ��ʼ												
													//���ٽ���������						
													memset(_net.rxbuf,0,_net.rxbufmax);//������ڴ�	
													memset(st17,0,20);
													strcpy(st17,"AT+NSOCL=1\r\n");
													st17[9]=Sys_Cntrl.uSocket_No;
													uart_sendstring(st17);

													_net.time_out=1000;//10��			
													while(strstr(_net.rxbuf,"OK")==0 && --_net.time_out)//�ȴ����ڽ�����ϻ�ʱ�˳�
													{		
														Thread_Sleep(10);
													}																
													if(_net.time_out)	
													{
															uCon_No=CON_NO_ST29;//��һ��
															Sys_Cntrl.fNB_Open=0;//��λ
																				
													}
													else	//����
													{

													}	
													Thread_Sleep(200);//��ʱ200ms
													continue;									
												}else	if(Sys_Cntrl.uPlatform_No==1)
												{
													//Thread_Sleep(200);//��ʱ200ms	//��ֹ���ڽ�������û��ͷ��ʼ	
													uCon_No=CON_NO_ST16;//��һ��
													continue;	
												}												
											}			
										}
									}					
								}	
							}							
						}
						else	//����
						{

						}	
						Thread_Sleep(100);//��ʱ100ms
						continue;
					}					
					
			
																	
				


					
				}
			
			}
				Thread_Sleep(10);
		
		}
	}
	Thread_END
}









char User_NBIOT_Register_Scan_Thread(thread_t* pt)//ע��ʱ,�����
{

	 Thread_BEGIN
	{

			while(true)
			{
				
				if(Sys_Cntrl.uPlatform_No>2)//����쳣��
				{		
						Sys_Cntrl.uPlatform_No=0;//��λ
					
						Sys_Cntrl.uRegister_Use_Time=0;//����		
						Sys_Cntrl.uTime_Heard_No=0;//����			
						Sys_Cntrl.uTime_Heard_Send_No=0;//����				
						uCon_No=CON_NO_START0;		
						Sys_Cntrl.uLine_Work=0;//					

				}					
								
				
				
				if(Sys_Cntrl.uPlatform_No==0&&CMachine.UI_Show_No>1)//���Ϊע��
				{
					//��ֹ�쳣��ע��ʱ�����
					Sys_Cntrl.uRegister_Use_Time++;
					if(Sys_Cntrl.uRegister_Use_Time>=18000)//ע�ᳬ��3����
					{
						Sys_Cntrl.uRegister_Use_Time=0;//����		
						Sys_Cntrl.uTime_Heard_No=0;//����			
						Sys_Cntrl.uTime_Heard_Send_No=0;//����				
						uCon_No=CON_NO_START0;		
						Sys_Cntrl.uLine_Work=0;//
					}		
				}else
				{
					Sys_Cntrl.uRegister_Use_Time=0;//����
				}				
				Thread_Sleep(10);
			}
		}
		Thread_END
}

















