#include "includes.h"	   





#define CON_ASK_MAX	1000	//10��

#define ASK_RES_NEXT_DELAY 20 //��ʱ������һ��

 u8  uMode_Con_No=2;//ģʽ�ţ����ֿܷ���Ϊ��������ʡ���ġ�0Ϊ����ر�GSM˯�ߣ�1Ϊ��GSM˯�ߣ�2Ϊ��������1�ζ�λ��3Ϊ��������10�ζ�λ��
 u8  uCon_No=CON_NO_START0;//���ƺš�ÿִ��һ�ֶ�������1
 u8  uCon_No_B=0xff;//���ƻ��ظ�ִ�ж�����־
 u16  uCon_Time_Delay=0;//ÿ�ֶ���֮���ӳ�ʱ��
 u16  uCon_Ask_Time=0;//Ӧ��ʱ�䶨ʱ����ʱ��û�ظ����ٴη���



const char	st1[] =	{"AT+CFUN=1\r\n"};//������Ƶȫ����
const char	st2[] =	{"AT+NNMI=1\r\n"};//��������֪ͨ 
const char	st3[] =	{"AT+CGATT=1\r\n"};//�Զ����� 
const char	st4[] =	{"AT+CPSMS=0\r\n"};//�رյ͹��Ĺ���
const char	st5[] =	{"AT+CGPADDR\r\n"};//��ѯ�����ip ��ַ�������Դ��ж��Ƿ����ӵ�����
const char	st6[] =	{"AT+NUESTATS\r\n"};//��ѯUE ״̬������rsrp,sinr,���ǵȼ���С����Ϣ�ȣ���ѡ 
 
const char  st7[]	=	{"AT+CGSN=1\r\n"};//��ѯIMEI ��
const char  st8[]	=	{"AT+CIMI\r\n"};//��ѯSIM����Ϣ



const char    st15[] =	{"AT+NSOCR=DGRAM,17,9502,1\r\n"};//1 Ϊsocket �ţ�����лظ�OK˵�������ɹ�,UDP

//iot.poscn.com 
const char    st160[] =	{"AT+NSOST=2,223.5.5.5,53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//�����DNS
const char    st161[] =	{"AT+NSOST=2,8.8.8.8,53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//�ȸ��DNS
const char    st162[] =	{"AT+NSOST=2,114.114.114.114,53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//114��DNS����å


char    st17[20] ={0};	//{"AT+NSORF=1,256\r\n\0"};//
	 
	  



 char	st29[] =	{"AT+CSQ\r\n"};//��ѯNB�ź�ǿ�� 
 
 
 //OneNETƽ̨
char	st30[] =	{"AT+MIPLCREATE\r\n"};//��������ͨѶ�׼�
char	st31[] =	{"AT+MIPLADDOBJ=0,3200,1,1,1,0\r\n"};//���OBJ=3200ʵ��
char	st32[] =	{"AT+MIPLNOTIFY=0,666,3200,0,5750,1,14,\"4E4230382D3031\",0,0,0\r\n"};//��ǰNOTIFY
char	st33[] =	{"AT+MIPLOPEN=0,86400,30\r\n"};//���л���ͨѶ�׼�
//char	st34[] =	{"AT+MIPLNOTIFY=0,666,3200,0,5750,1,258,"1400810000D8200100010100004E29000000032A0000000D0300000000080000019009000000000D000000000E000000002B00
//										0000153E000000053D0000000043FFFFFB7744FFFFFBE645000000E64600000485470000106148054123BF49000000024A000000464B00000E9A4C000001014DFFFFFF8E4E000000044F00000008",0,0,0\r\n"};//notify�ϱ���Ϣ



const char    st40[] =	{"AT+NSOCR=DGRAM,17,9502,1\r\n"};//1 Ϊsocket �ţ�����лظ�OK˵�������ɹ�,UDP

//iot1.poscn.com 
const char    st410[] =	{"AT+NSOST=2,223.5.5.5,53,32,3E3A0100000100000000000004696F743105706F73636E03636F6D0000010001\r\n"};//�����DNS
const char    st411[] =	{"AT+NSOST=2,8.8.8.8,53,32,3E3A0100000100000000000004696F743105706F73636E03636F6D0000010001\r\n"};//�ȸ��DNS
const char    st412[] =	{"AT+NSOST=2,114.114.114.114,53,32,3E3A0100000100000000000004696F743105706F73636E03636F6D0000010001\r\n"};//114��DNS����å


char    st42[20] ={0};	//{"AT+NSORF=1,256\r\n\0"};//
	 

char    st45[20] ={0};	//{"AT+NSORF=1,256\r\n\0"};//



unsigned char uDNS_IP_Final[4]={0};//���շ�����IP
unsigned char uDNS_IP_Register[4]={0};//ע�������IP
 char uDNS_No=0;//DNS���
 char uSocket_No=0;//���ӵ�socket��



 u32 uDNS_Res_Time=0;//DNS�ȴ�Ӧ��ʱ��
 u32 uRegister_Res_Time=0;//ע��ȴ�Ӧ��ʱ��


 u8 uDNS_Res_Time_No=0;//DNS�ȴ�Ӧ��ʱ��//����
 u8 uRegister_Res_Time_No=0;//ע��ȴ�Ӧ��ʱ��//����




u8 uPlatform_No=1;//Ҫ���ӵ�ƽ̨��



 


  u8  uStart_Send_Data_5=6;//����������δ����״̬��ÿ10�뷢��һ�����ݣ�������5��
 

 u16  uPM25_Data_Send_Time=5500;//���ݷ��Ͷ�ʱ
//u16 	uCon_State_Ask_Time_No=0;//����״̬��ʱ
 u32 		uTime_Heard_No=0;//��������
 
 u16 		uTime_Heard_Send_No=0;//����������������

 

 u8  uData_Send_No_Add=0;//���ݷ��ʹ����������������㣬û���س�������ģ��
 
 u8  fData_Send_Services_Ask=0;//�������ݸ���������־ 
 




 u8 uSend_Data_No=0;//�������ݳ���ͳ��
 u8 uSend_Data_i=0;//


u8 fNB_Open=0;//��ƽ̨��־





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



char * str_p;//�ַ���ָ��//
char * str_p_nest;//�ַ���ָ��//

char str_msgid[10]={0};




u8 volatile 	uUart_Recive_Data_Decode[50]={0};//�������ݽ���
u8 volatile 	uUart_Recive_Data_Decode_i=0;

 u8  fData_Send_Services_Ask_B=0;//�������ݸ���������־ 
 u8  fData_Send_Services_Ask_B1=0;//�������ݸ���������־ 


 UTCTime TimeSeconds_res = 0;
 u32 TimeSeconds_res_b = 0;
 u32 TimeSeconds_b = 0;

 UTCTimeStruct	CTime_Res;//ʱ��ṹ��


void Uart2_Decode(void)//���ڴ����� 
{


		
	uUart2_Recive_Length=0;
	
	
	if(strstr((const char*)uUart2_Recive_Data,"ERROR")!=0)
	{

			uTime_Heard_Send_No=0;//����		
					uTime_Heard_No=0;//����			
						uCon_No=CON_NO_START0;		
						uCon_No_B=0xff;
						uCon_Ask_Time=0;//����
			CMachine.uLine_Work=0;//
				
		
		
		}	
	
	
	

		switch(uCon_No)
						{
				case	CON_NO_ST0:
								if(strstr((const char*)uUart2_Recive_Data,"Lierda")!=0)//�ϵ������ɹ���᷵��
								{	

										uCon_Time_Delay=100;//��ʱ1s
										uCon_No=CON_NO_ST1;		
										uCon_Ask_Time=0;//����
								}		
							
								break;
							case	CON_NO_ST1:			
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=100;//��ʱ1s
										uCon_No=CON_NO_ST2;		
										uCon_Ask_Time=0;//����
								}	
								break;		
								
							case	CON_NO_ST2:								
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=500;//��ʱ5s�ſ�ʼ��������ֹ����ERR
										uCon_No=CON_NO_ST3;		
										uCon_Ask_Time=0;//����
								}	
								break;			
							case	CON_NO_ST3:								
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=100;//��ʱ1s
										uCon_No=CON_NO_ST4;		
										uCon_Ask_Time=0;//����
								}	
								break;								
						

							case	CON_NO_ST4:								
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=100;//��ʱ1s
										uCon_No=CON_NO_ST5;		
										uCon_Ask_Time=0;//����
								}	
								break;									
								
							case	CON_NO_ST5:								
								if(strstr((const char*)uUart2_Recive_Data,"+CGPADDR:0,")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST6;		
										uCon_Ask_Time=0;//����

									
//									uNB_msgid=(SysTick->VAL)%99999;//��ȡ�����
									
								}else
								{


								}								
								break;			
							case	CON_NO_ST6:								


									if((str_p=strstr((const char*)uUart2_Recive_Data,"Signal power:"))!=0)
									{
										CMachine.Signal_power=atol(str_p+13);
									}
									if((str_p=strstr((const char*)uUart2_Recive_Data,"Total power:"))!=0)
									{
										CMachine.Total_power=atol(str_p+12);
									}									
									if((str_p=strstr((const char*)uUart2_Recive_Data,"TX power:"))!=0)
									{
										CMachine.TX_power=atol(str_p+9);
									}									
									if((str_p=strstr((const char*)uUart2_Recive_Data,"TX time:"))!=0)
									{
										CMachine.TX_time=atol(str_p+8);
									}												
									if((str_p=strstr((const char*)uUart2_Recive_Data,"RX time:"))!=0)
									{
										CMachine.RX_time=atol(str_p+8);
									}												
									if((str_p=strstr((const char*)uUart2_Recive_Data,"Cell ID:"))!=0)
									{
										CMachine.Cell_ID=atol(str_p+8);
									}													
									if((str_p=strstr((const char*)uUart2_Recive_Data,"ECL:"))!=0)
									{
										CMachine.ECL=atol(str_p+4);
									}												
									if((str_p=strstr((const char*)uUart2_Recive_Data,"SNR:"))!=0)
									{
										CMachine.SNR=atol(str_p+4);
									}											
									if((str_p=strstr((const char*)uUart2_Recive_Data,"EARFCN:"))!=0)
									{
										CMachine.EARFCN=atol(str_p+7);
									}											
									if((str_p=strstr((const char*)uUart2_Recive_Data,"PCI:"))!=0)
									{
										CMachine.PCI=atol(str_p+4);
									}														
									if((str_p=strstr((const char*)uUart2_Recive_Data,"RSRQ:"))!=0)
									{
										CMachine.RSRQ=atol(str_p+5);
									}													
									if((str_p=strstr((const char*)uUart2_Recive_Data,"OPERATOR MODE:"))!=0)
									{
										CMachine.OPERATOR_MODE=atol(str_p+14);
									}													
									if((str_p=strstr((const char*)uUart2_Recive_Data,"CURRENT BAND:"))!=0)
									{
										CMachine.CURRENT_BAND=atol(str_p+13);
									}				
									
									
									
								/*	if(strstr((const char*)uUart2_Recive_Data,"OK")!=0)//�ظ�OK			
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST15;		
										uCon_Ask_Time=0;//����									
									
									}										
									*/
								
									if(strstr((const char*)uUart2_Recive_Data,"OK")!=0)//�ظ�OK			
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST7;		
										uCon_Ask_Time=0;//����									
									
									}	
									
									
							
									
									
		/*							
										signed long	Signal_power;
	signed long	Total_power;
	signed long	TX_power;
	signed long	TX_time;
	signed long	RX_time;
	signed long	Cell_ID;
	signed long	ECL;
	signed long	SNR;
	signed long	EARFCN;
	signed long	PCI;
	signed long	RSRQ;
	signed long	OPERATOR_MODE;
	signed long	CURRENT_BAND;
									
				*/					
									

									
									
								
								break;									
							case	CON_NO_ST7:								
									if((str_p=strstr((const char*)uUart2_Recive_Data,"+CGSN:"))!=0)										
									{
										strncpy(CMachine.CGSN,(str_p+6),15);//
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST8;		
										uCon_Ask_Time=0;//����														
									}								
								break;	
							case	CON_NO_ST8:								
									if((str_p=strstr((const char*)uUart2_Recive_Data,"\r\n"))!=0)
									{

										strncpy(CMachine.CIMI,(str_p+2),15);//
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
	
										uCon_Ask_Time=0;//����		

										if(uPlatform_No==0)	uCon_No=CON_NO_ST15;	
										else	uCon_No=CON_NO_ST29;	

										
									}								
								break;				


							case	CON_NO_ST29:	
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	
									
									if((str_p=strstr((const char*)uUart2_Recive_Data,"+CSQ:"))!=0)
									{
										CMachine.NB_RSSI=atol(str_p+5);
									}																										
									uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
									uCon_Ask_Time=0;//����		
									
									if(uPlatform_No==2)//OneNET
									{
										if(fNB_Open==0)	
										{
											uCon_No=CON_NO_ST30;
											fNB_Open=1;//��
										}
										else	uCon_No=CON_NO_ST34;												
									}else		
									if(uPlatform_No==1)//�Լ�ƽ̨
									{
										if(fNB_Open==0)	
										{
											uCon_No=CON_NO_ST40;
											fNB_Open=1;//��
										}
										else	uCon_No=CON_NO_ST44;												
									}									
								}
								break;										
									
							case	CON_NO_ST30:										
									if(strstr((const char*)uUart2_Recive_Data,"OK")!=0)//�ظ�OK			
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST31;		
										uCon_Ask_Time=0;//����									
									
									}										
								break;													
							case	CON_NO_ST31:										
									if(strstr((const char*)uUart2_Recive_Data,"OK")!=0)//�ظ�OK			
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST32;		
										uCon_Ask_Time=0;//����									
									
									}										
								break;									
							case	CON_NO_ST32:										
									if(strstr((const char*)uUart2_Recive_Data,"OK")!=0)//�ظ�OK			
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST33;		
										uCon_Ask_Time=0;//����									
									
									}										
								break;									
							case	CON_NO_ST33:										
									if((str_p=strstr((const char*)uUart2_Recive_Data,"+MIPLOBSERVE:"))!=0)//�ظ�OK			
									{

										//+MIPLOBSERVE:0,200588,1,4,0,-1
										if((str_p=strstr((const char*)uUart2_Recive_Data,"1,4,0,"))!=0)		
										{
										uCon_Time_Delay=200;//��ʱ2S
										uCon_No=CON_NO_ST34;		
										uCon_Ask_Time=0;//����																			
										
										}											
										
										
									
									}										
								break;	
									
								
							case	CON_NO_ST34:	
							case	CON_NO_ST36:										
									if((str_p=strstr((const char*)uUart2_Recive_Data,"+MIPLEXECUTE:0,"))!=0)//�ظ�OK			
									{
//+MIPLEXECUTE:0,12347,3200,0,5750,38,1400130000AC0F01002C5D37F7303C00000001
										
								
											if((str_p_nest=strstr((str_p+15),",3200"))!=0)
											{
												memset(str_msgid,0,10);				
												strncpy(str_msgid,(str_p+15),(str_p_nest-str_p-15));
											
												



								if((str_p=strstr((const char*)uUart2_Recive_Data,"1400"))!=0&&uCon_Time_Delay==0)//�ϴ����ݣ��ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
	
										uCon_Ask_Time=0;//����
									
									uData_Send_No_Add=0;//����
									
									
									
									//����
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
						
									



									
									
								//	uUart_Recive_Data_Decode[5]=CTOH((str_p+14));		//У�鷽��
								//	uUart_Recive_Data_Decode[6]=CTOH((str_p+16));		//���ܷ���
									
								//	uUart_Recive_Data_Decode[7]=CTOH((str_p+18));		//�����ֵ���
									
								//	uUart_Recive_Data_Decode[8]=CTOH((str_p+20));		//����ֵ
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
											uStart_Send_Data_5=0;//����������ʱ��
											CMachine.uLine_Work=1;

							
							
										CMachine.fSys_User_Other_Data=0;//����
							
							
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
				
				
								ConvertUTCTime(&CTime_Res,TimeSeconds_res_b);		//��ȡת��ʱ��


		Sys_Time_WIFI.sec=	CTime_Res.seconds;  
		Sys_Time_WIFI.min=CTime_Res.minutes;  
		Sys_Time_WIFI.hour=CTime_Res.hour;     
		Sys_Time_WIFI.w_date=CTime_Res.day;      
		Sys_Time_WIFI.w_month=CTime_Res.month;    
		Sys_Time_WIFI.w_year=CTime_Res.year-2000;							
							
							
					
							
							
							
							
							
							
							
							
							//AT+NSOST=1,39.96.30.87,2100,54,1400360000C8F20100010000000129000000032A0000000D0300000000080000003F090000004D0C000000000D000000E00E00000300			
							
							//Ӧ��
							//14 00 0E 00 00 8D 9C 01 00 2C 5C 51 CF 64
							//����ģʽ
							//14 00 0E 00 00 AA 05 01 00 38 00 00 00 00
							//14 00 0E 00 00 6B C5 01 00 38 00 00 00 01
							//14 00 0E 00 00 2B C4 01 00 38 00 00 00 02
							//��������
							//14 00 0E 00 00 97 C5 01 00 39 00 00 00 00							
							//14 00 0E 00 00 56 05 01 00 39 00 00 00 01										
							//��������
							//14 00 0E 00 00 22 06 01 00 2E 00 00 00 01
							//14 00 0E 00 00 23 C5 01 00 2E 00 00 00 05
							//14 00 0E 00 00 A2 01 01 00 2E 00 00 00 0B							
							
							//����+����
							//14 00 13 00 00 60 C5 01 00 39 00 00 00 01 2E 00 00 00 05							
							//14 00 13 00 00 60 D7 01 00 39 00 00 00 00 2E 00 00 00 01											
							
							//����+����+ģʽ
							//14 00 18 00 00 DC C1 01 00 39 00 00 00 01 2E 00 00 00 05 38 00 00 00 01							
							//14 00 18 00 00 9D 44 01 00 39 00 00 00 01 2E 00 00 00 01 38 00 00 00 02								
							
							

						}

						if(uUart_Recive_Data_Decode[7+uUart_Recive_Data_Decode_i]==0x3D)
						{
							
							
							CMachine.uSys_User_Reset=0;//ϵͳ�û�����//����
							CMachine.uSys_User_Reset_Time=0;//ϵͳ�û�����,��ʱ//����			

												uStart_Send_Data_5=0;//����������ʱ��
												CMachine.uLine_Work=1;
	
							
						}			
						
						
					}
				
				
			}
									
		}
									
									
									

	memset((void*)uUart2_Recive_Data,0,200);
									
									
									
								}


										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST35;		
										uCon_Ask_Time=0;//����					

												
												
											}



										
										
										
										
										
									}										
								break;										
							case	CON_NO_ST35:										
									if(strstr((const char*)uUart2_Recive_Data,"OK")!=0)//�ظ�OK			
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST36;		
										uCon_Ask_Time=0;//����									
									
									}										
								break;												
									
									
									
							case	CON_NO_ST40:								
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	
									uSocket_No=uUart2_Recive_Data[2];														
									uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
									uCon_No=CON_NO_ST41;		
									uCon_Ask_Time=0;//����
								}	
								break;								
							case	CON_NO_ST41:	
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	
									uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
									uCon_No=CON_NO_ST43;		
									uCon_Ask_Time=0;//����
								}	
								break;														
							case	CON_NO_ST42:								
								if((str_p=strstr((const char*)uUart2_Recive_Data,"3E3A"))!=0&&uCon_Time_Delay==0)//�ϴ����ݣ��ظ�OK
								{	
									if((str_p_nest=strstr(str_p,","))!=0)
									{

										uDNS_IP_Final[0]=CTOH(str_p_nest-8);
										uDNS_IP_Final[1]=CTOH(str_p_nest-6);
										uDNS_IP_Final[2]=CTOH(str_p_nest-4);
										uDNS_IP_Final[3]=CTOH(str_p_nest-2);
										
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST44;		
										uCon_Ask_Time=0;//����						
										uDNS_Res_Time=0;//												
									}																		
								}								
							case	CON_NO_ST43:									
								if((str_p=strstr((const char*)uUart2_Recive_Data,"+NSONMI"))!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	
									uCon_Time_Delay=5;//��ʱ50ms
									uCon_No=CON_NO_ST42;		
									uCon_Ask_Time=0;//����								
								}	
								break;												
									
									
								
														
							case	CON_NO_ST44:	
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST46;		
										uCon_Ask_Time=0;//����
								}	
								break;										
							
							case	CON_NO_ST46:									
								if((str_p=strstr((const char*)uUart2_Recive_Data,"+NSONMI"))!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	

									
							//������ʼ����
									uPM25_Data_Send_Time=0;//
									uTime_Heard_No=0;//��������												
									

									
											uCon_Time_Delay=5;//��ʱ50ms
										uCon_No=CON_NO_ST45;		
										uCon_Ask_Time=0;//����								
									
								}	
								break;			
										
							case	CON_NO_ST45:								
								if((str_p=strstr((const char*)uUart2_Recive_Data,"1400"))!=0&&uCon_Time_Delay==0)//�ϴ����ݣ��ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
	
										uCon_Ask_Time=0;//����
									
									uData_Send_No_Add=0;//����
									
									
									
									//����
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
						
									



									
									
								//	uUart_Recive_Data_Decode[5]=CTOH((str_p+14));		//У�鷽��
								//	uUart_Recive_Data_Decode[6]=CTOH((str_p+16));		//���ܷ���
									
								//	uUart_Recive_Data_Decode[7]=CTOH((str_p+18));		//�����ֵ���
									
								//	uUart_Recive_Data_Decode[8]=CTOH((str_p+20));		//����ֵ
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
											uStart_Send_Data_5=0;//����������ʱ��
											CMachine.uLine_Work=1;
											uCon_Time_Delay=5;//��ʱ50ms
										uCon_No=CON_NO_ST46;		
										uCon_Ask_Time=0;//����								

							
							
										CMachine.fSys_User_Other_Data=0;//����
							
							
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
				
				
								ConvertUTCTime(&CTime_Res,TimeSeconds_res_b);		//��ȡת��ʱ��


		Sys_Time_WIFI.sec=	CTime_Res.seconds;  
		Sys_Time_WIFI.min=CTime_Res.minutes;  
		Sys_Time_WIFI.hour=CTime_Res.hour;     
		Sys_Time_WIFI.w_date=CTime_Res.day;      
		Sys_Time_WIFI.w_month=CTime_Res.month;    
		Sys_Time_WIFI.w_year=CTime_Res.year-2000;							
							
							
					
							
							
							
							
							
							
							
							
							//AT+NSOST=1,39.96.30.87,2100,54,1400360000C8F20100010000000129000000032A0000000D0300000000080000003F090000004D0C000000000D000000E00E00000300			
							
							//Ӧ��
							//14 00 0E 00 00 8D 9C 01 00 2C 5C 51 CF 64
							//����ģʽ
							//14 00 0E 00 00 AA 05 01 00 38 00 00 00 00
							//14 00 0E 00 00 6B C5 01 00 38 00 00 00 01
							//14 00 0E 00 00 2B C4 01 00 38 00 00 00 02
							//��������
							//14 00 0E 00 00 97 C5 01 00 39 00 00 00 00							
							//14 00 0E 00 00 56 05 01 00 39 00 00 00 01										
							//��������
							//14 00 0E 00 00 22 06 01 00 2E 00 00 00 01
							//14 00 0E 00 00 23 C5 01 00 2E 00 00 00 05
							//14 00 0E 00 00 A2 01 01 00 2E 00 00 00 0B							
							
							//����+����
							//14 00 13 00 00 60 C5 01 00 39 00 00 00 01 2E 00 00 00 05							
							//14 00 13 00 00 60 D7 01 00 39 00 00 00 00 2E 00 00 00 01											
							
							//����+����+ģʽ
							//14 00 18 00 00 DC C1 01 00 39 00 00 00 01 2E 00 00 00 05 38 00 00 00 01							
							//14 00 18 00 00 9D 44 01 00 39 00 00 00 01 2E 00 00 00 01 38 00 00 00 02								
							
							

						}

						if(uUart_Recive_Data_Decode[7+uUart_Recive_Data_Decode_i]==0x3D)
						{
							
							
							CMachine.uSys_User_Reset=0;//ϵͳ�û�����//����
							CMachine.uSys_User_Reset_Time=0;//ϵͳ�û�����,��ʱ//����			

												uStart_Send_Data_5=0;//����������ʱ��
												CMachine.uLine_Work=1;
											uCon_Time_Delay=5;//��ʱ50ms
										uCon_No=CON_NO_ST46;		
										uCon_Ask_Time=0;//����			
							
						}			
						
						
					}
				
				
			}
									
		}
									
									
									

	memset((void*)uUart2_Recive_Data,0,200);
									
									
									
								}else//������Ϊ��
								{


											uCon_Time_Delay=5;//��ʱ50ms
										uCon_No=CON_NO_ST46;		
										uCon_Ask_Time=0;//����								
																		
									


								}								
								
								
								
								
								break;											
								
								
								
									
									
									
							case	CON_NO_ST15:								
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	
									uSocket_No=uUart2_Recive_Data[2];														
									uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
									uCon_No=CON_NO_ST16;		
									uCon_Ask_Time=0;//����
								}	
								break;								
							case	CON_NO_ST16:	
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	
									uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
									uCon_No=CON_NO_ST18;		
									uCon_Ask_Time=0;//����
								}	
								break;														
							case	CON_NO_ST17:								
								if((str_p=strstr((const char*)uUart2_Recive_Data,"3E3A"))!=0&&uCon_Time_Delay==0)//�ϴ����ݣ��ظ�OK
								{	
									if((str_p_nest=strstr(str_p,","))!=0)
									{

										uDNS_IP_Register[0]=CTOH(str_p_nest-8);
										uDNS_IP_Register[1]=CTOH(str_p_nest-6);
										uDNS_IP_Register[2]=CTOH(str_p_nest-4);
										uDNS_IP_Register[3]=CTOH(str_p_nest-2);
										
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST19;		
										uCon_Ask_Time=0;//����						
										uDNS_Res_Time=0;//												
									}																		
								}								
							case	CON_NO_ST18:									
								if((str_p=strstr((const char*)uUart2_Recive_Data,"+NSONMI"))!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	
									uCon_Time_Delay=5;//��ʱ50ms
									uCon_No=CON_NO_ST17;		
									uCon_Ask_Time=0;//����								
								}	
								break;			
							case	CON_NO_ST19:	
								if(strstr((const char*)uUart2_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	
									uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
									uCon_No=CON_NO_ST21;		
									uCon_Ask_Time=0;//����
								}	
								break;														
							case	CON_NO_ST20:								
								if((str_p=strstr((const char*)uUart2_Recive_Data,"1400"))!=0&&uCon_Time_Delay==0)//�ϴ����ݣ��ظ�OK
								{	


										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
	
										uCon_Ask_Time=0;//����
									
									uData_Send_No_Add=0;//����
									
									
									
									//����
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


						if(uUart_Recive_Data_Decode[7+uUart_Recive_Data_Decode_i]==0x54)//�·���ƽ̨
						{
							
							
									uPlatform_No=uUart_Recive_Data_Decode[7+uUart_Recive_Data_Decode_i+4];//ƽ̨��
							
						//	uPlatform_No=1;//�Լ�ƽ̨
							
									uTime_Heard_Send_No=0;//����		
									uTime_Heard_No=0;//����			
									uCon_No=CON_NO_START0;		
									uCon_No_B=0xff;
									uCon_Ask_Time=0;//����
									CMachine.uLine_Work=0;//
							
						}			
						
						
					}
				
				
			}
									
		}
									
									
									

	memset((void*)uUart2_Recive_Data,0,200);
															
									
									
									
									
									
									
									
								}								
							case	CON_NO_ST21:									
								if((str_p=strstr((const char*)uUart2_Recive_Data,"+NSONMI"))!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	
									
									

									
											uCon_Time_Delay=5;//��ʱ50ms
										uCon_No=CON_NO_ST20;		
										uCon_Ask_Time=0;//����						

										uRegister_Res_Time=0;	//�ѷ���ע�����ݣ�����								
									
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
	

			uTime_Heard_Send_No=0;//����	
	
}


u8 fNBIOT_Init=0;//��ʼ����־





u8 volatile 	uSys_Data_Send[150]={0};//���ݷ��ͼ��㻺��
u8 volatile 	uSys_Data_Send_i=0;//���ݷ��ͼ��㻺��


union u32Tou8
{
	uint64_t ulongx;
	uint8_t uMac_Base[8];
}MACu32tou8;

	uint64_t SYS_TYPE_NO=0;


unsigned short UI_Show_No_B_NBIOT_Task=0xff;//
char User_NBIOT_Thread(thread_t* pt)
{

     Thread_BEGIN
    {




    GPIO_InitTypeDef  GPIO_InitStructure;  		  

	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTCʱ��

	
	

//��ȡMAC
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
				
			uCon_No=CON_NO_START0;//���ƺš���λ����ֹ���ʱ��һֱ�����ػ������ִ���û��ʼ���ͷ������ݣ��������		
			UI_Show_No_B_NBIOT_Task=0xff;
			}
						
			
			
			//delay_ms(10);
			//continue;//�˳�
			
		if(uUart2_Recive_End>0)	
		{
		
		uUart2_Recive_End--;
		if(uUart2_Recive_End==0) 		  Uart2_Decode();//���ڽ��ս��� 

		
		}			
		


if(uCon_Time_Delay>0)	
	{
		uCon_Time_Delay--;
		
	}else
	{
	if(uCon_Ask_Time>0)//1��
	{
		uCon_Ask_Time--;
		//if(uCon_Ask_Time==0||uCon_Ask_Time>CON_ASK_MAX)	
		if(uCon_Ask_Time==0)	
		{
	
				uCon_No_B=0xff;//���·�һ��
				uCon_Ask_Time=0;
		
		}
	}	
	
	if(uDNS_Res_Time>0)//5��
	{
		uDNS_Res_Time--;
		if(uDNS_Res_Time==0)	
		{
	
				uCon_No_B=0xff;//���·�һ��
				uCon_No=CON_NO_ST16;
		
		}
	}		
	
	
	if(uRegister_Res_Time>0)//10��
	{
		uRegister_Res_Time--;
		if(uRegister_Res_Time==0)	
		{
	
				uCon_No_B=0xff;//���·�һ��
				uCon_No=CON_NO_ST19;
		
		}
	}			
	
	

	switch(uMode_Con_No)
	{
		case 0:	//0Ϊ����ر�GSM˯��.
			

		
			break;
		case 1:	//1Ϊ��GSM˯��.
		

			break;			
		case 2:	//2Ϊ��������1�ζ�λ.
			if(uCon_No_B!=uCon_No)
			{
				uCon_No_B=uCon_No;
				
				
				switch(uCon_No)
				{
					case	CON_NO_START0:			
			

						uart2_uninit();		
					GPIO_ResetBits(GPIOC,GPIO_Pin_3); //�����		
						uCon_Time_Delay=1000;//��ʱ10S//��ȫ������				
						uCon_No=CON_NO_START1;		
						CMachine.uLine_Work=0;	
						uData_Send_No_Add=0;//����					
										fNB_Open=0;//����				
					
					
	
						break;
					case	CON_NO_START1:					
						GPIO_SetBits(GPIOC,GPIO_Pin_3); //��NB��Դ 
						uart2_init(9600);//	  				
						uCon_Time_Delay=500;//��ʱ5S
						uCon_No=CON_NO_ST0;		
						CMachine.fSys_User_Other_Data=1;//�����ϴ��ź�ǿ�ȼ���λ����
						uStart_Send_Data_5=6;
						break;						
					case	CON_NO_START2:			
			
						break;	
					case	CON_NO_START3:			
					
						break;						
					case	CON_NO_START4:			
			
						break;						
					case	CON_NO_ST0:			
	
						//uart_sendstring(st0);			
						//uCon_Ask_Time=100;//Ӧ����			
						break;		
					case	CON_NO_ST1:
	
						uart_sendstring(st1);	
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;	



					
					case	CON_NO_ST2:
		
						uart_sendstring(st2);
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;	
			
					case	CON_NO_ST3:
						
						uart_sendstring(st3);		
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;	
					
					case	CON_NO_ST4:
						
						uart_sendstring(st4);		
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;		


	

					case	CON_NO_ST5:
			
						uart_sendstring(st5);
						uCon_Ask_Time=200;//Ӧ����	//2��
						break;	
					
						case	CON_NO_ST6:
			
						uart_sendstring(st6);
						uCon_Ask_Time=200;//Ӧ����	//2��
						break;	
						
						
						case	CON_NO_ST7:		
							uart_sendstring(st7);		
							uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;				

						case	CON_NO_ST8:		
							uart_sendstring(st8);		
							uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
							break;								
								
						
						case	CON_NO_ST15:			
							uart_sendstring(st15);
							uCon_Ask_Time=200;//Ӧ����	//2��				

							uDNS_No=0;//���㸴λ
						
							break;								
						
						case	CON_NO_ST16:			
							
							if(uDNS_No==0)
							{
								uSend_Data_No=strlen(st160);
								strncpy((char *)uUart2_Send_Data,st160,uSend_Data_No);			
								uDNS_No=1;
							}else
							if(uDNS_No==1)
							{
								uSend_Data_No=strlen(st161);
								strncpy((char *)uUart2_Send_Data,st161,uSend_Data_No);		
								uDNS_No=2;
							}else
							if(uDNS_No==2)
							{
								uSend_Data_No=strlen(st162);
								strncpy((char *)uUart2_Send_Data,st162,uSend_Data_No);		
								uDNS_No=0;
							}
							
							uUart2_Send_Data[9]=uSocket_No;			
							User_UART2_SendStringx((char *)&uUart2_Send_Data[0],uSend_Data_No);													
							uCon_Ask_Time=100;//Ӧ����	//1��			

							uDNS_Res_Time=500;//Ӧ����	//5��			
							
							break;										
						
					case	CON_NO_ST17:
						
					memset(st17,0,20);
					strcpy(st17,"AT+NSORF=1,256\r\n");
					
						 st17[9]=uSocket_No;
						uart_sendstring(st17);
						uCon_Ask_Time=200;//Ӧ����	//2��
					

						break;											
						
					
					case	CON_NO_ST19:

					
	uRegister_Res_Time_No++;
	if(uRegister_Res_Time_No>3)//��������3��//��ע��
	{
		uRegister_Res_Time_No=0;//

								uRegister_Res_Time=0;//Ӧ����	//10��				
		uPlatform_No=1;//�Լ�ƽ̨
		uTime_Heard_Send_No=0;//����		
		uTime_Heard_No=0;//����			
		uCon_No=CON_NO_START0;		
		uCon_No_B=0xff;
		uCon_Ask_Time=0;//����
		CMachine.uLine_Work=0;//
		break;
	}
					
					
					
//����ͷ					
uSys_Data_Send[0]=0x14;
//��Ч���ݳ���
uSys_Data_Send[1]=0x00;
uSys_Data_Send[2]=0x31;		

//У��λ
//uSys_Data_Send[3]='0';
//uSys_Data_Send[4]='0';									
//uSys_Data_Send[5]='0';
//uSys_Data_Send[6]='0';

//У�鷽��
uSys_Data_Send[7]=0x01;
//���ܷ���
uSys_Data_Send[8]=0x00;

						
//�豸���
uSys_Data_Send[9]=0x01;
uSys_Data_Send[10]=SYS_EDITION;
//���:��һ���ֽ�Ϊ�汾��
//��01��Ӳ���汾1������汾1
//
//
uSys_Data_Send[11]=((SYS_TYPE_NO&0xff0000)>>16);
uSys_Data_Send[12]=((SYS_TYPE_NO&0xff00)>>8);
uSys_Data_Send[13]=(SYS_TYPE_NO&0xff);
//ͨѶ����
uSys_Data_Send[14]=0x29;
uSys_Data_Send[15]=0x00;
uSys_Data_Send[16]=0x00;
uSys_Data_Send[17]=0x00;
uSys_Data_Send[18]=((SYS_TYPE&0xff00)>>8);
//��Ʒ����
uSys_Data_Send[19]=0x2A;
uSys_Data_Send[20]=0x00;
uSys_Data_Send[21]=0x00;
uSys_Data_Send[22]=0x00;
uSys_Data_Send[23]=(SYS_TYPE&0xff);
//��������
uSys_Data_Send[24]=0x3C;
uSys_Data_Send[25]=0x00;
uSys_Data_Send[26]=0x00;
uSys_Data_Send[27]=0x00;
uSys_Data_Send[28]=0x02;

//IMEI�ţ�ǰ8λ��
uSys_Data_Send[29]=0x50;
uSys_Data_Send[30]=CTOH(&CMachine.CGSN[0]);
uSys_Data_Send[31]=CTOH(&CMachine.CGSN[2]);
uSys_Data_Send[32]=CTOH(&CMachine.CGSN[4]);
uSys_Data_Send[33]=CTOH(&CMachine.CGSN[6]);
//IMEI�ţ���7λ��
uSys_Data_Send[34]=0x51;
uSys_Data_Send[35]=(CTOH(&CMachine.CGSN[7])&0x0f);
uSys_Data_Send[36]=CTOH(&CMachine.CGSN[9]);
uSys_Data_Send[37]=CTOH(&CMachine.CGSN[11]);
uSys_Data_Send[38]=CTOH(&CMachine.CGSN[13]);
//CIMI�ţ�ǰ8λ��
uSys_Data_Send[39]=0x52;
uSys_Data_Send[40]=CTOH(&CMachine.CIMI[0]);
uSys_Data_Send[41]=CTOH(&CMachine.CIMI[2]);
uSys_Data_Send[42]=CTOH(&CMachine.CIMI[4]);
uSys_Data_Send[43]=CTOH(&CMachine.CIMI[6]);
//CIMI�ţ���7λ��
uSys_Data_Send[44]=0x53;
uSys_Data_Send[45]=(CTOH(&CMachine.CIMI[7])&0x0f);
uSys_Data_Send[46]=CTOH(&CMachine.CIMI[9]);
uSys_Data_Send[47]=CTOH(&CMachine.CIMI[11]);
uSys_Data_Send[48]=CTOH(&CMachine.CIMI[13]);

crc_check=GetCrC((unsigned char *)&uSys_Data_Send[7],42);


		
//У��λ
uSys_Data_Send[3]=0x00;
uSys_Data_Send[4]=0x00;							
uSys_Data_Send[5]=((crc_check&0xff00)>>8);
uSys_Data_Send[6]=(crc_check&0xff);




					memset((char *)uUart2_Send_Data,0,400);
					strcpy((char *)&uUart2_Send_Data[0],"AT+NSOST=2,");
					uUart2_Send_Data[9]=uSocket_No;					
					sprintf((char *)&uUart2_Send_Data[11],"%d.%d.%d.%d,1234,49,",uDNS_IP_Register[0],uDNS_IP_Register[1],uDNS_IP_Register[2],uDNS_IP_Register[3]);
					
					
				uSend_Data_No=strlen((char *)&uUart2_Send_Data);
					
	for(uSys_Data_Send_i=0;uSys_Data_Send_i<49;uSys_Data_Send_i++)
	{

	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i]=HTOC(((uSys_Data_Send[uSys_Data_Send_i])&0xf0)>>4);		
	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=HTOC((uSys_Data_Send[uSys_Data_Send_i])&0xf);		

	}	
	
					
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i]=0x0D;	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=0x0A;	





			User_UART2_SendStringx((char *)&uUart2_Send_Data[0],(uSend_Data_No+2*(uSys_Data_Send_i+1)));	

					
					
	
							uCon_Ask_Time=100;//Ӧ����	//1��			

							uRegister_Res_Time=1000;//Ӧ����	//10��				
	

	


	
	


						break;								
					
					
	
	
	
					case	CON_NO_ST20:
						
					memset(st17,0,20);
					strcpy(st17,"AT+NSORF=1,256\r\n");
					
						 st17[9]=uSocket_No;
						uart_sendstring(st17);
						uCon_Ask_Time=200;//Ӧ����	//2��
					

						break;		
	
	
	
					
					
					case	CON_NO_ST29:
						uart_sendstring(st29);		
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;		

					
					case	CON_NO_ST30:
						uart_sendstring(st30);		
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;		
					case	CON_NO_ST31:
						uart_sendstring(st31);		
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;		
					case	CON_NO_ST32:
						uart_sendstring(st32);	

			
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;		
					case	CON_NO_ST33:
						uart_sendstring(st33);		
					//	uCon_Time_Delay=500;//��ʱ5S
				//		uCon_No=CON_NO_ST34;		
			

					//	uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
					
						break;		

					case	CON_NO_ST34:

					
//char	st34[] =	{"AT+MIPLNOTIFY=0,666,3200,0,5750,1,258,"1400810000D8200100010100004E29000000032A0000000D0300000000080000019009000000000D000000000E000000002B00
//										0000153E000000053D0000000043FFFFFB7744FFFFFBE645000000E64600000485470000106148054123BF49000000024A000000464B00000E9A4C000001014DFFFFFF8E4E000000044F00000008",0,0,0\r\n"};//notify�ϱ���Ϣ
	
						
					
				memset((char *)uUart2_Send_Data,0,400);
strcpy((char *)uUart2_Send_Data,"AT+MIPLNOTIFY=0,666,3200,0,5750,1,");
					





//����ͷ					
uSys_Data_Send[0]=0x14;
//��Ч���ݳ���
if(CMachine.fSys_User_Other_Data==0)
{
	uSys_Data_Send[1]=0x00;
	uSys_Data_Send[2]=0x3B;		
}else
{
	uSys_Data_Send[1]=0x00;
	uSys_Data_Send[2]=0x81;		
}







//У��λ
//uSys_Data_Send[3]='0';
//uSys_Data_Send[4]='0';									
//uSys_Data_Send[5]='0';
//uSys_Data_Send[6]='0';

//У�鷽��
uSys_Data_Send[7]=0x01;
//���ܷ���
uSys_Data_Send[8]=0x00;


//�豸���
uSys_Data_Send[9]=0x01;
uSys_Data_Send[10]=SYS_EDITION;
//���:��һ���ֽ�Ϊ�汾��
//��01��Ӳ���汾1������汾1
//
//
uSys_Data_Send[11]=((SYS_TYPE_NO&0xff0000)>>16);
uSys_Data_Send[12]=((SYS_TYPE_NO&0xff00)>>8);
uSys_Data_Send[13]=(SYS_TYPE_NO&0xff);
//ͨѶ����
uSys_Data_Send[14]=0x29;
uSys_Data_Send[15]=0x00;
uSys_Data_Send[16]=0x00;
uSys_Data_Send[17]=0x00;
uSys_Data_Send[18]=((SYS_TYPE&0xff00)>>8);
//��Ʒ����
uSys_Data_Send[19]=0x2A;
uSys_Data_Send[20]=0x00;
uSys_Data_Send[21]=0x00;
uSys_Data_Send[22]=0x00;
uSys_Data_Send[23]=(SYS_TYPE&0xff);


//PM2.5
uSys_Data_Send[24]=0x03;
uSys_Data_Send[25]=0x00;
uSys_Data_Send[26]=0x00;
uSys_Data_Send[27]=((CMachine.pm25&0xff00)>>8);
uSys_Data_Send[28]=(CMachine.pm25&0xff);


//�¶�
uSys_Data_Send[29]=0x08;
uSys_Data_Send[30]=0x00;
uSys_Data_Send[31]=0x00;
uSys_Data_Send[32]=(((CMachine.temp*10)&0xff00)>>8);
uSys_Data_Send[33]=((CMachine.temp*10)&0xff);

//ʪ��
uSys_Data_Send[34]=0x09;
uSys_Data_Send[35]=0x00;
uSys_Data_Send[36]=0x00;
uSys_Data_Send[37]=(((CMachine.humidity*10)&0xff00)>>8);
uSys_Data_Send[38]=((CMachine.humidity*10)&0xff);

//��ȩ

//TVOC
uSys_Data_Send[39]=0x0D;
uSys_Data_Send[40]=0x00;
uSys_Data_Send[41]=0x00;
uSys_Data_Send[42]=((CMachine.tvoc&0xff00)>>8);
uSys_Data_Send[43]=(CMachine.tvoc&0xff);

//CO2
uSys_Data_Send[44]=0x0E;
uSys_Data_Send[45]=0x00;
uSys_Data_Send[46]=0x00;
uSys_Data_Send[47]=((CMachine.co2&0xff00)>>8);
uSys_Data_Send[48]=(CMachine.co2&0xff);




//����
uSys_Data_Send[49]=0x2B;
uSys_Data_Send[50]=0x00;
uSys_Data_Send[51]=0x00;
uSys_Data_Send[52]=((CMachine.Battery&0xff00)>>8);
uSys_Data_Send[53]=(CMachine.Battery&0xff);


//�����ź�ǿ��
uSys_Data_Send[54]=0x3E;
uSys_Data_Send[55]=0x00;
uSys_Data_Send[56]=0x00;
uSys_Data_Send[57]=((CMachine.NB_RSSI&0xff00)>>8);
uSys_Data_Send[58]=(CMachine.NB_RSSI&0xff);
				





if(CMachine.fSys_User_Other_Data==1)//�ϴ�����
{
	if(CMachine.uSys_User_Reset==1)//�û�Ȩ������
	{
		uSys_Data_Send[59]=0x3D;
		uSys_Data_Send[60]=0x00;
		uSys_Data_Send[61]=0x00;
		uSys_Data_Send[62]=0x00;
		uSys_Data_Send[63]=0x01;
	}else
	{
		uSys_Data_Send[59]=0x3D;
		uSys_Data_Send[60]=0x00;
		uSys_Data_Send[61]=0x00;
		uSys_Data_Send[62]=0x00;
		uSys_Data_Send[63]=0x00;
	}	

	//NB�ƶ���λ��Ϣ
	uSys_Data_Send[64]=0x43;
	uSys_Data_Send[65]=((CMachine.Signal_power&0xff000000)>>24);
	uSys_Data_Send[66]=((CMachine.Signal_power&0xff0000)>>16);
	uSys_Data_Send[67]=((CMachine.Signal_power&0xff00)>>8);
	uSys_Data_Send[68]=(CMachine.Signal_power&0xff);											
	//
	uSys_Data_Send[69]=0x44;
	uSys_Data_Send[70]=((CMachine.Total_power&0xff000000)>>24);
	uSys_Data_Send[71]=((CMachine.Total_power&0xff0000)>>16);
	uSys_Data_Send[72]=((CMachine.Total_power&0xff00)>>8);
	uSys_Data_Send[73]=(CMachine.Total_power&0xff);			
	//
	uSys_Data_Send[74]=0x45;
	uSys_Data_Send[75]=((CMachine.TX_power&0xff000000)>>24);
	uSys_Data_Send[76]=((CMachine.TX_power&0xff0000)>>16);
	uSys_Data_Send[77]=((CMachine.TX_power&0xff00)>>8);
	uSys_Data_Send[78]=(CMachine.TX_power&0xff);				
	//
	uSys_Data_Send[79]=0x46;
	uSys_Data_Send[80]=((CMachine.TX_time&0xff000000)>>24);
	uSys_Data_Send[81]=((CMachine.TX_time&0xff0000)>>16);
	uSys_Data_Send[82]=((CMachine.TX_time&0xff00)>>8);
	uSys_Data_Send[83]=(CMachine.TX_time&0xff);			
	//
	uSys_Data_Send[84]=0x47;
	uSys_Data_Send[85]=((CMachine.RX_time&0xff000000)>>24);
	uSys_Data_Send[86]=((CMachine.RX_time&0xff0000)>>16);
	uSys_Data_Send[87]=((CMachine.RX_time&0xff00)>>8);
	uSys_Data_Send[88]=(CMachine.RX_time&0xff);			
	//
	uSys_Data_Send[89]=0x48;
	uSys_Data_Send[90]=((CMachine.Cell_ID&0xff000000)>>24);
	uSys_Data_Send[91]=((CMachine.Cell_ID&0xff0000)>>16);
	uSys_Data_Send[92]=((CMachine.Cell_ID&0xff00)>>8);
	uSys_Data_Send[93]=(CMachine.Cell_ID&0xff);			
	//
	uSys_Data_Send[94]=0x49;
	uSys_Data_Send[95]=((CMachine.ECL&0xff000000)>>24);
	uSys_Data_Send[96]=((CMachine.ECL&0xff0000)>>16);
	uSys_Data_Send[97]=((CMachine.ECL&0xff00)>>8);
	uSys_Data_Send[98]=(CMachine.ECL&0xff);			
	//
	uSys_Data_Send[99]=0x4A;
	uSys_Data_Send[100]=((CMachine.SNR&0xff000000)>>24);
	uSys_Data_Send[101]=((CMachine.SNR&0xff0000)>>16);
	uSys_Data_Send[102]=((CMachine.SNR&0xff00)>>8);
	uSys_Data_Send[103]=(CMachine.SNR&0xff);			
	//
	uSys_Data_Send[104]=0x4B;
	uSys_Data_Send[105]=((CMachine.EARFCN&0xff000000)>>24);
	uSys_Data_Send[106]=((CMachine.EARFCN&0xff0000)>>16);
	uSys_Data_Send[107]=((CMachine.EARFCN&0xff00)>>8);
	uSys_Data_Send[108]=(CMachine.EARFCN&0xff);			
	//
	uSys_Data_Send[109]=0x4C;
	uSys_Data_Send[110]=((CMachine.PCI&0xff000000)>>24);
	uSys_Data_Send[111]=((CMachine.PCI&0xff0000)>>16);
	uSys_Data_Send[112]=((CMachine.PCI&0xff00)>>8);
	uSys_Data_Send[113]=(CMachine.PCI&0xff);			
	//
	uSys_Data_Send[114]=0x4D;
	uSys_Data_Send[115]=((CMachine.RSRQ&0xff000000)>>24);
	uSys_Data_Send[116]=((CMachine.RSRQ&0xff0000)>>16);
	uSys_Data_Send[117]=((CMachine.RSRQ&0xff00)>>8);
	uSys_Data_Send[118]=(CMachine.RSRQ&0xff);			
	//
	uSys_Data_Send[119]=0x4E;
	uSys_Data_Send[120]=((CMachine.OPERATOR_MODE&0xff000000)>>24);
	uSys_Data_Send[121]=((CMachine.OPERATOR_MODE&0xff0000)>>16);
	uSys_Data_Send[122]=((CMachine.OPERATOR_MODE&0xff00)>>8);
	uSys_Data_Send[123]=(CMachine.OPERATOR_MODE&0xff);			
	//
	uSys_Data_Send[124]=0x4F;
	uSys_Data_Send[125]=((CMachine.CURRENT_BAND&0xff000000)>>24);
	uSys_Data_Send[126]=((CMachine.CURRENT_BAND&0xff0000)>>16);
	uSys_Data_Send[127]=((CMachine.CURRENT_BAND&0xff00)>>8);
	uSys_Data_Send[128]=(CMachine.CURRENT_BAND&0xff);			




}				
			


if(CMachine.fSys_User_Other_Data==0)		crc_check=GetCrC((unsigned char *)&uSys_Data_Send[7],52);
else		crc_check=GetCrC((unsigned char *)&uSys_Data_Send[7],122);


		
//У��λ
uSys_Data_Send[3]=0x00;
uSys_Data_Send[4]=0x00;							
uSys_Data_Send[5]=((crc_check&0xff00)>>8);
uSys_Data_Send[6]=(crc_check&0xff);





if(CMachine.fSys_User_Other_Data==0)	
{
							
					strcat((char *)uUart2_Send_Data,"118,\"");
	
}else
{
	
					strcat((char *)uUart2_Send_Data,"258,\"");


	
}




				uSend_Data_No=strlen((char *)uUart2_Send_Data);


if(CMachine.fSys_User_Other_Data==0)	
{
	for(uSys_Data_Send_i=0;uSys_Data_Send_i<59;uSys_Data_Send_i++)
	{

	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i]=HTOC(((uSys_Data_Send[uSys_Data_Send_i])&0xf0)>>4);		
	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=HTOC((uSys_Data_Send[uSys_Data_Send_i])&0xf);		

	}
}else
{
	for(uSys_Data_Send_i=0;uSys_Data_Send_i<129;uSys_Data_Send_i++)
	{

	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i]=HTOC(((uSys_Data_Send[uSys_Data_Send_i])&0xf0)>>4);		
	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=HTOC((uSys_Data_Send[uSys_Data_Send_i])&0xf);		

	}
}







uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+0]='"';	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=',';	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+2]='0';	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+3]=',';	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+4]='0';	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+5]=',';	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+6]='0';	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+7]=0x0D;	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+8]=0x0A;	

					
					
					
					
					User_UART2_SendStringx((char *)&uUart2_Send_Data[0],strlen((char *)uUart2_Send_Data));	

								

if(uStart_Send_Data_5>0)
{
	uStart_Send_Data_5--;
	
		uPM25_Data_Send_Time=4001;//20����ٴη����ݣ�����ʱ


}
					
					
					
						break;													
					
					
					case	CON_NO_ST35:

					
//char	st34[] =	{"AT+MIPLNOTIFY=0,666,3200,0,5750,1,258,"1400810000D8200100010100004E29000000032A0000000D0300000000080000019009000000000D000000000E000000002B00
//										0000153E000000053D0000000043FFFFFB7744FFFFFBE645000000E64600000485470000106148054123BF49000000024A000000464B00000E9A4C000001014DFFFFFF8E4E000000044F00000008",0,0,0\r\n"};//notify�ϱ���Ϣ
	
						
					
				memset((char *)uUart2_Send_Data,0,400);
					
					
strcpy((char *)uUart2_Send_Data,"AT+MIPLEXECUTERSP=0,");
strcat((char *)uUart2_Send_Data,str_msgid);					
strcat((char *)uUart2_Send_Data,",2\r\n");								
					

					User_UART2_SendStringx((char *)&uUart2_Send_Data[0],strlen((char *)uUart2_Send_Data));	

								
					
						break;			

					case	CON_NO_ST36:

					

					
						break;		

					
						case	CON_NO_ST40:			
							uart_sendstring(st40);
							uCon_Ask_Time=200;//Ӧ����	//2��				

							uDNS_No=0;//���㸴λ
						
							break;								
						
						case	CON_NO_ST41:			
							
							if(uDNS_No==0)
							{
								uSend_Data_No=strlen(st410);
								strncpy((char *)uUart2_Send_Data,st160,uSend_Data_No);			
								uDNS_No=1;
							}else
							if(uDNS_No==1)
							{
								uSend_Data_No=strlen(st411);
								strncpy((char *)uUart2_Send_Data,st161,uSend_Data_No);		
								uDNS_No=2;
							}else
							if(uDNS_No==2)
							{
								uSend_Data_No=strlen(st412);
								strncpy((char *)uUart2_Send_Data,st162,uSend_Data_No);		
								uDNS_No=0;
							}
							
							uUart2_Send_Data[9]=uSocket_No;			
							User_UART2_SendStringx((char *)&uUart2_Send_Data[0],uSend_Data_No);													
							uCon_Ask_Time=100;//Ӧ����	//1��			

							uDNS_Res_Time=500;//Ӧ����	//5��			
							
							break;										
						
					case	CON_NO_ST42:
						
					memset(st42,0,20);
					strcpy(st42,"AT+NSORF=1,256\r\n");
					
						 st42[9]=uSocket_No;
						uart_sendstring(st42);
						uCon_Ask_Time=200;//Ӧ����	//2��
					

						break;								
					
					
					
					case	CON_NO_ST44:


//����ͷ					
uSys_Data_Send[0]=0x14;
//��Ч���ݳ���
if(CMachine.fSys_User_Other_Data==0)
{
	uSys_Data_Send[1]=0x00;
	uSys_Data_Send[2]=0x3B;		
}else
{
	uSys_Data_Send[1]=0x00;
	uSys_Data_Send[2]=0x81;		
}







//У��λ
//uSys_Data_Send[3]='0';
//uSys_Data_Send[4]='0';									
//uSys_Data_Send[5]='0';
//uSys_Data_Send[6]='0';

//У�鷽��
uSys_Data_Send[7]=0x01;
//���ܷ���
uSys_Data_Send[8]=0x00;


//�豸���
uSys_Data_Send[9]=0x01;
uSys_Data_Send[10]=SYS_EDITION;
//���:��һ���ֽ�Ϊ�汾��
//��01��Ӳ���汾1������汾1
//
//
uSys_Data_Send[11]=((SYS_TYPE_NO&0xff0000)>>16);
uSys_Data_Send[12]=((SYS_TYPE_NO&0xff00)>>8);
uSys_Data_Send[13]=(SYS_TYPE_NO&0xff);
//ͨѶ����
uSys_Data_Send[14]=0x29;
uSys_Data_Send[15]=0x00;
uSys_Data_Send[16]=0x00;
uSys_Data_Send[17]=0x00;
uSys_Data_Send[18]=((SYS_TYPE&0xff00)>>8);
//��Ʒ����
uSys_Data_Send[19]=0x2A;
uSys_Data_Send[20]=0x00;
uSys_Data_Send[21]=0x00;
uSys_Data_Send[22]=0x00;
uSys_Data_Send[23]=(SYS_TYPE&0xff);


//PM2.5
uSys_Data_Send[24]=0x03;
uSys_Data_Send[25]=0x00;
uSys_Data_Send[26]=0x00;
uSys_Data_Send[27]=((CMachine.pm25&0xff00)>>8);
uSys_Data_Send[28]=(CMachine.pm25&0xff);


//�¶�
uSys_Data_Send[29]=0x08;
uSys_Data_Send[30]=0x00;
uSys_Data_Send[31]=0x00;
uSys_Data_Send[32]=(((CMachine.temp*10)&0xff00)>>8);
uSys_Data_Send[33]=((CMachine.temp*10)&0xff);

//ʪ��
uSys_Data_Send[34]=0x09;
uSys_Data_Send[35]=0x00;
uSys_Data_Send[36]=0x00;
uSys_Data_Send[37]=(((CMachine.humidity*10)&0xff00)>>8);
uSys_Data_Send[38]=((CMachine.humidity*10)&0xff);

//��ȩ

//TVOC
uSys_Data_Send[39]=0x0D;
uSys_Data_Send[40]=0x00;
uSys_Data_Send[41]=0x00;
uSys_Data_Send[42]=((CMachine.tvoc&0xff00)>>8);
uSys_Data_Send[43]=(CMachine.tvoc&0xff);

//CO2
uSys_Data_Send[44]=0x0E;
uSys_Data_Send[45]=0x00;
uSys_Data_Send[46]=0x00;
uSys_Data_Send[47]=((CMachine.co2&0xff00)>>8);
uSys_Data_Send[48]=(CMachine.co2&0xff);




//����
uSys_Data_Send[49]=0x2B;
uSys_Data_Send[50]=0x00;
uSys_Data_Send[51]=0x00;
uSys_Data_Send[52]=((CMachine.Battery&0xff00)>>8);
uSys_Data_Send[53]=(CMachine.Battery&0xff);


//�����ź�ǿ��
uSys_Data_Send[54]=0x3E;
uSys_Data_Send[55]=0x00;
uSys_Data_Send[56]=0x00;
uSys_Data_Send[57]=((CMachine.NB_RSSI&0xff00)>>8);
uSys_Data_Send[58]=(CMachine.NB_RSSI&0xff);
				





if(CMachine.fSys_User_Other_Data==1)//�ϴ�����
{
	if(CMachine.uSys_User_Reset==1)//�û�Ȩ������
	{
		uSys_Data_Send[59]=0x3D;
		uSys_Data_Send[60]=0x00;
		uSys_Data_Send[61]=0x00;
		uSys_Data_Send[62]=0x00;
		uSys_Data_Send[63]=0x01;
	}else
	{
		uSys_Data_Send[59]=0x3D;
		uSys_Data_Send[60]=0x00;
		uSys_Data_Send[61]=0x00;
		uSys_Data_Send[62]=0x00;
		uSys_Data_Send[63]=0x00;
	}	


/*
CMachine.Signal_power;
CMachine.Total_power;
CMachine.TX_power;
CMachine.TX_time;
CMachine.RX_time;
CMachine.Cell_ID;
CMachine.ECL;
CMachine.SNR;
CMachine.EARFCN;
CMachine.PCI;
CMachine.RSRQ;
CMachine.OPERATOR_MODE;
CMachine.CURRENT_BAND;				
*/
	//NB�ƶ���λ��Ϣ
	uSys_Data_Send[64]=0x43;
	uSys_Data_Send[65]=((CMachine.Signal_power&0xff000000)>>24);
	uSys_Data_Send[66]=((CMachine.Signal_power&0xff0000)>>16);
	uSys_Data_Send[67]=((CMachine.Signal_power&0xff00)>>8);
	uSys_Data_Send[68]=(CMachine.Signal_power&0xff);											
	//
	uSys_Data_Send[69]=0x44;
	uSys_Data_Send[70]=((CMachine.Total_power&0xff000000)>>24);
	uSys_Data_Send[71]=((CMachine.Total_power&0xff0000)>>16);
	uSys_Data_Send[72]=((CMachine.Total_power&0xff00)>>8);
	uSys_Data_Send[73]=(CMachine.Total_power&0xff);			
	//
	uSys_Data_Send[74]=0x45;
	uSys_Data_Send[75]=((CMachine.TX_power&0xff000000)>>24);
	uSys_Data_Send[76]=((CMachine.TX_power&0xff0000)>>16);
	uSys_Data_Send[77]=((CMachine.TX_power&0xff00)>>8);
	uSys_Data_Send[78]=(CMachine.TX_power&0xff);				
	//
	uSys_Data_Send[79]=0x46;
	uSys_Data_Send[80]=((CMachine.TX_time&0xff000000)>>24);
	uSys_Data_Send[81]=((CMachine.TX_time&0xff0000)>>16);
	uSys_Data_Send[82]=((CMachine.TX_time&0xff00)>>8);
	uSys_Data_Send[83]=(CMachine.TX_time&0xff);			
	//
	uSys_Data_Send[84]=0x47;
	uSys_Data_Send[85]=((CMachine.RX_time&0xff000000)>>24);
	uSys_Data_Send[86]=((CMachine.RX_time&0xff0000)>>16);
	uSys_Data_Send[87]=((CMachine.RX_time&0xff00)>>8);
	uSys_Data_Send[88]=(CMachine.RX_time&0xff);			
	//
	uSys_Data_Send[89]=0x48;
	uSys_Data_Send[90]=((CMachine.Cell_ID&0xff000000)>>24);
	uSys_Data_Send[91]=((CMachine.Cell_ID&0xff0000)>>16);
	uSys_Data_Send[92]=((CMachine.Cell_ID&0xff00)>>8);
	uSys_Data_Send[93]=(CMachine.Cell_ID&0xff);			
	//
	uSys_Data_Send[94]=0x49;
	uSys_Data_Send[95]=((CMachine.ECL&0xff000000)>>24);
	uSys_Data_Send[96]=((CMachine.ECL&0xff0000)>>16);
	uSys_Data_Send[97]=((CMachine.ECL&0xff00)>>8);
	uSys_Data_Send[98]=(CMachine.ECL&0xff);			
	//
	uSys_Data_Send[99]=0x4A;
	uSys_Data_Send[100]=((CMachine.SNR&0xff000000)>>24);
	uSys_Data_Send[101]=((CMachine.SNR&0xff0000)>>16);
	uSys_Data_Send[102]=((CMachine.SNR&0xff00)>>8);
	uSys_Data_Send[103]=(CMachine.SNR&0xff);			
	//
	uSys_Data_Send[104]=0x4B;
	uSys_Data_Send[105]=((CMachine.EARFCN&0xff000000)>>24);
	uSys_Data_Send[106]=((CMachine.EARFCN&0xff0000)>>16);
	uSys_Data_Send[107]=((CMachine.EARFCN&0xff00)>>8);
	uSys_Data_Send[108]=(CMachine.EARFCN&0xff);			
	//
	uSys_Data_Send[109]=0x4C;
	uSys_Data_Send[110]=((CMachine.PCI&0xff000000)>>24);
	uSys_Data_Send[111]=((CMachine.PCI&0xff0000)>>16);
	uSys_Data_Send[112]=((CMachine.PCI&0xff00)>>8);
	uSys_Data_Send[113]=(CMachine.PCI&0xff);			
	//
	uSys_Data_Send[114]=0x4D;
	uSys_Data_Send[115]=((CMachine.RSRQ&0xff000000)>>24);
	uSys_Data_Send[116]=((CMachine.RSRQ&0xff0000)>>16);
	uSys_Data_Send[117]=((CMachine.RSRQ&0xff00)>>8);
	uSys_Data_Send[118]=(CMachine.RSRQ&0xff);			
	//
	uSys_Data_Send[119]=0x4E;
	uSys_Data_Send[120]=((CMachine.OPERATOR_MODE&0xff000000)>>24);
	uSys_Data_Send[121]=((CMachine.OPERATOR_MODE&0xff0000)>>16);
	uSys_Data_Send[122]=((CMachine.OPERATOR_MODE&0xff00)>>8);
	uSys_Data_Send[123]=(CMachine.OPERATOR_MODE&0xff);			
	//
	uSys_Data_Send[124]=0x4F;
	uSys_Data_Send[125]=((CMachine.CURRENT_BAND&0xff000000)>>24);
	uSys_Data_Send[126]=((CMachine.CURRENT_BAND&0xff0000)>>16);
	uSys_Data_Send[127]=((CMachine.CURRENT_BAND&0xff00)>>8);
	uSys_Data_Send[128]=(CMachine.CURRENT_BAND&0xff);			




}				
			


if(CMachine.fSys_User_Other_Data==0)		crc_check=GetCrC((unsigned char *)&uSys_Data_Send[7],52);
else		crc_check=GetCrC((unsigned char *)&uSys_Data_Send[7],122);


		
//У��λ
uSys_Data_Send[3]=0x00;
uSys_Data_Send[4]=0x00;							
uSys_Data_Send[5]=((crc_check&0xff00)>>8);
uSys_Data_Send[6]=(crc_check&0xff);




					memset((char *)uUart2_Send_Data,0,400);
if(CMachine.fSys_User_Other_Data==0)	
{
					strcpy((char *)&uUart2_Send_Data[0],"AT+NSOST=2,");
					uUart2_Send_Data[9]=uSocket_No;					
					sprintf((char *)&uUart2_Send_Data[11],"%d.%d.%d.%d,1234,59,",uDNS_IP_Final[0],uDNS_IP_Final[1],uDNS_IP_Final[2],uDNS_IP_Final[3]);
}else
{
					strcpy((char *)&uUart2_Send_Data[0],"AT+NSOST=2,");
					uUart2_Send_Data[9]=uSocket_No;					
					sprintf((char *)&uUart2_Send_Data[11],"%d.%d.%d.%d,1234,129,",uDNS_IP_Final[0],uDNS_IP_Final[1],uDNS_IP_Final[2],uDNS_IP_Final[3]);
	
}



			uSend_Data_No=strlen((char *)&uUart2_Send_Data);


if(CMachine.fSys_User_Other_Data==0)	
{
	for(uSys_Data_Send_i=0;uSys_Data_Send_i<59;uSys_Data_Send_i++)
	{

	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i]=HTOC(((uSys_Data_Send[uSys_Data_Send_i])&0xf0)>>4);		
	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=HTOC((uSys_Data_Send[uSys_Data_Send_i])&0xf);		

	}
}else
{
	for(uSys_Data_Send_i=0;uSys_Data_Send_i<129;uSys_Data_Send_i++)
	{

	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i]=HTOC(((uSys_Data_Send[uSys_Data_Send_i])&0xf0)>>4);		
	uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=HTOC((uSys_Data_Send[uSys_Data_Send_i])&0xf);		

	}
}








uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i]=0x0D;	
uUart2_Send_Data[uSend_Data_No+2*uSys_Data_Send_i+1]=0x0A;	





			User_UART2_SendStringx((char *)&uUart2_Send_Data[0],(uSend_Data_No+2*(uSys_Data_Send_i+1)));	




if(uStart_Send_Data_5>0)
{
	uStart_Send_Data_5--;
	
		uPM25_Data_Send_Time=5001;//10����ٴη����ݣ�����ʱ


}



			
			
					break;		
					case	CON_NO_ST45:
						
					memset(st45,0,20);
					strcpy(st45,"AT+NSORF=1,256\r\n");
					
						 st45[9]=uSocket_No;
						uart_sendstring(st45);
						uCon_Ask_Time=200;//Ӧ����	//2��
					

						break;					
					case	CON_NO_ST46:
			

					
	
					
						break;		

					
					
					
					
					
					
		default:
			
			break;
		}
	}else
	{
		if(uCon_No==CON_NO_ST36||uCon_No==CON_NO_ST46)
		{					
			if(CMachine.uSys_User_Reset_Time>0)	CMachine.uSys_User_Reset_Time--;
			if(CMachine.uSys_User_Reset==1&&((CMachine.uSys_User_Reset_Time%2000)==1999))	
			{
				uPM25_Data_Send_Time=5900;//1�������
			}
			uPM25_Data_Send_Time++;//10ms
			if(uPM25_Data_Send_Time>=6000)//1����//1����һ�ΰ�����ֹ�˿��ϻ�
			{
				uCon_No_B=0xff;
				uCon_No=CON_NO_ST29;
				uPM25_Data_Send_Time=0;//����
				
				uData_Send_No_Add++;
				if(uData_Send_No_Add>10)//����10��û�з���
				{
					uData_Send_No_Add=0;//							
					uTime_Heard_Send_No=0;//����		
					uTime_Heard_No=0;//����			
					uCon_No=CON_NO_START0;		
					uCon_No_B=0xff;
					uCon_Ask_Time=0;//����
					CMachine.uLine_Work=0;//									
				}
			}	
		}
	}		
	break;						
		default:
			
			break;

}
}



		//��ֹ�쳣û�з�����
		uTime_Heard_Send_No++;
		if(uTime_Heard_Send_No>=12000)//��������,������//2���������ݷ���
		{
			uTime_Heard_Send_No=0;//����		
					uTime_Heard_No=0;//����			
						uCon_No=CON_NO_START0;		
						uCon_No_B=0xff;
						uCon_Ask_Time=0;//����
			CMachine.uLine_Work=0;//
				
				

		}		


	if(CMachine.uLine_Work==1)
	{

		uTime_Heard_No++;
		if(uTime_Heard_No>=180000)//ʱ���׼,������//30����������
		{
			uTime_Heard_Send_No=0;//����		
					uTime_Heard_No=0;//����			
						uCon_No=CON_NO_START0;		
						uCon_No_B=0xff;
						uCon_Ask_Time=0;//����
			CMachine.uLine_Work=0;//
				
				

		}	
	
	}else
	{
		uTime_Heard_No=0;
				
		
	
		
		

	}

			
			
			
			
			
			
			
			
			
			
		
		
		
		}
		
		

		
		

 		Thread_Sleep(10);
	}






    }
    Thread_END








}





























