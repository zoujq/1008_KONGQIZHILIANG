#include "includes.h"	   
#include "crc32.h"



#define CON_ASK_MAX	1000	//10��

#define ASK_RES_NEXT_DELAY 20 //��ʱ������һ��

u8  uMode_Con_No=2;//ģʽ�ţ����ֿܷ���Ϊ��������ʡ���ġ�0Ϊ����ر�GSM˯�ߣ�1Ϊ��GSM˯�ߣ�2Ϊ��������1�ζ�λ��3Ϊ��������10�ζ�λ��
u8  uCon_No=CON_NO_START0;//���ƺš�ÿִ��һ�ֶ�������1
u8  uCon_No_B=0xff;//���ƻ��ظ�ִ�ж�����־
u16  uCon_Time_Delay=0;//ÿ�ֶ���֮���ӳ�ʱ��
u16  uCon_Ask_Time=0;//Ӧ��ʱ�䶨ʱ����ʱ��û�ظ����ٴη���

u16 	uUart_Send_Length=0;//���ͳ���




unsigned char  const  st0[]	=	{"AT\r\n"};
unsigned char  const  st1[]	=	{"ATE0\r\n"};//�رջ���
unsigned char  const  st2[]	=	{"AT+CSQ\r\n"};
unsigned char  const  st3[]	=	{"AT+QIFGCNT=0\r\n"};//����PDP context
unsigned char  const  st4[]	=	{"AT+QICSGP=1,\"UNIM2M.GZM2MAPN\"\r\n"};//����APN
unsigned char  const  st5[] =	{"AT+CREG?;+CGREG?\r\n"};//��ѯ���� ״̬
unsigned char  const  st6[] =	{"AT+QISTAT\r\n"};//��ѯ��ǰ����״̬


unsigned char  const 	st8[] =	{"AT+QNTP=\"202.120.2.101\",123\r\n"};//��ѯʱ��ͬ��״̬//�Ϻ���ͨ��ѧ��������NTP��������ַ
unsigned char  const	st9[]=	{"AT+CCLK?\r\n"};//��ѯʱ��


unsigned char  const  st10[]	=	{"AT+QIREGAPP\r\n"};
unsigned char  const  st11[]	=	{"AT+QIACT\r\n"};		
unsigned char  const	 st12[]=	{"AT+QICLOSE\r\n"};
//unsigned char  const  st13[]=	{"AT+QIOPEN=\"TCP\",\"222.244.144.69\",\"51005\"\r\n"};
//unsigned char  const  st13[]=	{"AT+QIOPEN=\"TCP\",\"116.204.70.58\",\"10000\"\r\n"};
unsigned char  const  st13[]=	{"AT+QIOPEN=\"TCP\",\"47.52.24.148\",\"9000\"\r\n"};
unsigned char  const  st14[]=	{"AT+QISEND=72\r\n"};
unsigned char  const  st140[]=	{"AT+QISEND=13\r\n"};

unsigned char  const  st17 []	=	{"AT+QSCLK=2\r\n"};		//����˯��ģʽ
unsigned char  const  st18 []	=	{"AT+QSCLK=0\r\n"};		//�˳�˯��ģʽ



//u16  uPM25_Data_Send_Time=0;//���ݷ��Ͷ�ʱ
//u16 	uCon_State_Ask_Time_No=0;//����״̬��ʱ
//u8 		uTime_Heard_No=0;//��������

u32  uPM25_Data_Send_Time=27000;//���ݷ��Ͷ�ʱ//����30����ʹ�

u8 	uLine_Work=0;//����״̬
u16  uTime_Heard_No=0;//��������




u32 DATA_CRC=0;
unsigned char DATA_CRC_B[10]={0};//����


u8 	uUart_Gprs_Send[400]={0};//���ͻ���

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



u8	uSys_Time_Point_No=0;//ʱ�����
u8	uSys_Time_Recive_Buf[10];


u8 uSysTime_Get_No=0;//ϵͳʱ���ȡ������3�λ�ȡ����������

/************************************************************************************
*��������:check_sum
*��������:��������У��
*��ڲ���:@data,У�����ݵ���ʼ��ַ   @len,У�����ݵĳ���
*����ֵ:����У��ֵ
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



void Uart4_Decode(void)//���ڴ����� 
{


		
				uUart4_Recive_Length=0;//��λ
	
	
		#ifdef SEGGER
	SEGGER_RTT_printf(0,"%s",uUart4_Recive_Data);
	#endif	
										if(strstr(uUart4_Recive_Data,"PDP DEACT"))//�����쳣�ر�
								{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//����
	memset(uUart4_Recive_Data,0,50);
									return;
								}
								
								
								if(strstr(uUart4_Recive_Data,"ERROR"))//�ظ�ERROR
								{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//����
									memset(uUart4_Recive_Data,0,50);
									return;
								}										
								
						switch(uCon_No)
						{
							case	CON_NO_ST0:
								if(strstr(uUart4_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST1;		
										uCon_Ask_Time=0;//����
								}			
								break;
							case	CON_NO_ST1:
								if(strstr(uUart4_Recive_Data,"OK")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST2;		
										uCon_Ask_Time=0;//����
								}		
								break;								
							case	CON_NO_ST2:								
								if(strncmp(uUart4_Recive_Data,"\r\n+CSQ",6)==0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST3;		
										uCon_Ask_Time=0;//����
								}	
								break;
							case	CON_NO_ST3:		
								if(strncmp(uUart4_Recive_Data,"\r\nOK",4)==0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST4;		
										uCon_Ask_Time=0;//����
								}								
								break;			
							case	CON_NO_ST4:		
								if(strncmp(uUart4_Recive_Data,"\r\nOK",4)==0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST5;		
										uCon_Ask_Time=0;//����
								}								
								break;									
							case	CON_NO_ST5:
								if(uCon_Time_Delay==0)
								{
									if(strncmp(uUart4_Recive_Data,"\r\n+CREG: 0,1",12)==0)//��������
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST13;												
										uCon_Ask_Time=0;//����
									}else	if(strncmp(uUart4_Recive_Data,"\r\n+CREG: 0,5",12)==0)//�������Σ�û���붨��
									{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST13;												
										uCon_Ask_Time=0;//����
								}
								break;

							case	CON_NO_ST6:		

							
								break;										
							case	CON_NO_ST7:		
						
								break;			
							case	CON_NO_ST8:			
								if(uCon_Time_Delay==0&&(CPU_ID[1]==UID_KEY[1]))//��ֹǰ��������OK��ʾ
								{
									if(strstr(uUart4_Recive_Data,"+QNTP: 0")!=0)//�ظ�ͬ��ʱ�����
									{	

											uCon_Time_Delay=10;//��ʱ100ms
											uCon_No=CON_NO_ST9;		//
											uCon_Ask_Time=0;//����
										uSysTime_Get_No=0;//����
									}else
									if(strstr(uUart4_Recive_Data,"+QNTP:")!=0)//��������
									{	

										
										uSysTime_Get_No++;
										if(uSysTime_Get_No>=4)	uSysTime_Get_No=4;
										if(uSysTime_Get_No>=4) uCon_No=CON_NO_ST13;		//
										else	uCon_No=CON_NO_START0;												
										
										uCon_Time_Delay=100;//��ʱ1��
										uCon_Ask_Time=0;//����
									}					


									
									
								}					

											break;	

							case	CON_NO_ST9:			
												


							if(strncmp(uUart4_Recive_Data,"\r\n+CCLK:",8)==0&&uCon_Time_Delay==0)//�ظ�ʱ��
							{
							
							Sys_Time_Save.w_year=2000+(uUart4_Recive_Data[10]-0x30)*10+(uUart4_Recive_Data[11]-0x30);
								Sys_Time_Save.w_month=(uUart4_Recive_Data[13]-0x30)*10+(uUart4_Recive_Data[14]-0x30);
								Sys_Time_Save.w_date=(uUart4_Recive_Data[16]-0x30)*10+(uUart4_Recive_Data[17]-0x30);
								Sys_Time_Save.hour=(uUart4_Recive_Data[19]-0x30)*10+(uUart4_Recive_Data[20]-0x30)+8;
								if(Sys_Time_Save.hour>=24)	Sys_Time_Save.hour-=24;
								Sys_Time_Save.min=(uUart4_Recive_Data[22]-0x30)*10+(uUart4_Recive_Data[23]-0x30);
								Sys_Time_Save.sec=(uUart4_Recive_Data[25]-0x30)*10+(uUart4_Recive_Data[26]-0x30);
							
								 Sys_RTC_Init();//ʵʱʱ�ӳ�ʼ��
								
								
							
							}

							
							
											uCon_Time_Delay=10;//��ʱ100ms
											uCon_No=CON_NO_ST13;		
											uCon_Ask_Time=0;//����
							
							
							break;					
							case	CON_NO_ST10:								
								if(strncmp(uUart4_Recive_Data,"\r\nOK",4)==0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST11;		
										uCon_Ask_Time=0;//����
								}else		if(strstr(uUart4_Recive_Data,"ERROR")!=0)//û������
								{
										uCon_Time_Delay=6000;//��ʱ60��
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//����

								}								
								break;	
							case	CON_NO_ST11:								
								if(strncmp(uUart4_Recive_Data,"\r\nOK",4)==0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST13;		
										uCon_Ask_Time=0;//����
								}								
								break;	
							case	CON_NO_ST12:								
								if(strncmp(uUart4_Recive_Data,"\r\nCLOSE OK",10)==0&&uCon_Time_Delay==0)//�ظ�OK//�ر�����
								{	
										uCon_Time_Delay=200;//��ʱ2000ms
//										uCon_No=CON_NO_ST17;		//����˯��ģʽ
										uCon_Ask_Time=0;//����
									

								}								
								break;	
							case	CON_NO_ST13:								
								if(strncmp(uUart4_Recive_Data,"\r\nCONNECT OK",12)==0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST14;		
										uCon_Ask_Time=0;//����
									
											uLine_Work=1;		//���ӳɹ�			
									
								}else	if(strstr(uUart4_Recive_Data,"CONNECT FAIL")!=0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=6000;//��ʱ60s
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//����
								}					
								
								break;	
							case	CON_NO_ST14:								
								if(strncmp(uUart4_Recive_Data,"\r\n> ",4)==0&&uCon_Time_Delay==0)//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST15;		
										uCon_Ask_Time=0;//����
								}								
								break;									
							case	CON_NO_ST15:									
								if(strstr(uUart4_Recive_Data,"SEND OK"))//�ظ�OK
								{	

										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST16;		
										uCon_Ask_Time=0;//����
								}						
								break;	
							case	CON_NO_ST16:		//����״̬						
								if(uUart4_Recive_Data[0]==0xAA&&uUart4_Recive_Data[1]==0x07)
								{
										//Ӧ������
//										uCon_State_Ask_Time_No=0;
									//	uTime_Heard_No=0;//��������
								//	uAnion_Work_Set=uUart4_Recive_Data[5];				

									uTime_Heard_No=0;//��������

									
									
								}else	if(strstr(uUart4_Recive_Data,"CLOSED"))//����Ͽ�
								{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_START0;		
										uCon_Ask_Time=0;//����

								}else	if(strstr(uUart4_Recive_Data,"CONNECT OK"))//������������
								{
//										uCon_State_Ask_Time_No=0;//����	
									uTime_Heard_No=0;//��������
										uCon_Ask_Time=0;//����

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
void User_GPRS_Task(void *pdata)//GPRS����
{

      //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD  |RCC_APB2Periph_GPIOE  |  RCC_APB2Periph_AFIO  , ENABLE); 



    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PD.0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
    GPIO_Init(GPIOD, &GPIO_InitStructure); //��ʼ��PD0
	 GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
	
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PE.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
    GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��PE2
	 GPIO_ResetBits(GPIOE,GPIO_Pin_2); 						 //PE.2 	
	
		
		#ifdef SEGGER
	SEGGER_RTT_printf(0,"gprs_start\r\n");
	#endif	
	
		while(1)
	{
		
	

				if(uUart4_Recive_End>0)	
		{
		
		uUart4_Recive_End--;
		if(uUart4_Recive_End==0) 		  Uart4_Decode();//���ڽ��ս��� 

		
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

		if(CON_NO_ST0==uCon_No)
		{
uCon_No=CON_NO_START3;//���¿���

	}else
	{
			
				uCon_No_B=0xff;//���·�һ��
				uCon_Ask_Time=0;
		
	}
		
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
				
			//	if(uCon_State_Ask_Time_No>0)	uCon_State_Ask_Time_No=0;
				
				switch(uCon_No)
				{
					case	CON_NO_START0:			
						 GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
							 GPIO_SetBits(GPIOE,GPIO_Pin_2); 						 //PE.2 	
	
					
						uCon_Time_Delay=100;//��ʱ1S				
						uCon_No=CON_NO_START1;					

	uLine_Work=0;		//δ����			
					
						break;
					case	CON_NO_START1:			
					//	P1_1=1;		
								//	GPIO_SetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
								 GPIO_ResetBits(GPIOE,GPIO_Pin_2); 						 //PE.2 	
						uCon_Time_Delay=100;//��ʱ1S
						uCon_No=CON_NO_START3;						
						break;						
					case	CON_NO_START2:			
				//		P1_1=0;		
											 GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
						uCon_Time_Delay=2000;//��ʱ20S		
						uCon_No=CON_NO_START3;						
						break;	
					case	CON_NO_START3:			
									GPIO_SetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
						uCon_Time_Delay=120;//��ʱ1.2S
						uCon_No=CON_NO_START4;									
						break;						
					case	CON_NO_START4:			
											 GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 //PD.0 
						uCon_Time_Delay=500;//��ʱ5S		
						uCon_No=CON_NO_ST0;			
						break;						
					case	CON_NO_ST0:			
						uUart_Send_Length=0;						
						uart_sendstring(st0);			
						uCon_Ask_Time=100;//Ӧ����			
						break;					
					case	CON_NO_ST1:			
						uUart_Send_Length=0;				
						uart_sendstring(st1);
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����				
						break;			
					case	CON_NO_ST2:			
						uUart_Send_Length=0;				
						uart_sendstring(st2);			
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����						
						break;
					case	CON_NO_ST3:
						uUart_Send_Length=0;							
						uart_sendstring(st3);			
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;	
					case	CON_NO_ST4:
						uUart_Send_Length=0;				
						uart_sendstring(st4);
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;			
					case	CON_NO_ST5:
						uUart_Send_Length=0;				
						uart_sendstring(st5);
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;	
					case	CON_NO_ST6:
//						uart_sendstring(st6);			
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;	
					case	CON_NO_ST7:
					//uart_sendstring(st7);	
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;				
					case	CON_NO_ST8:
						uUart_Send_Length=0;	
						uart_sendstring(st8);	
						uCon_Ask_Time=3*CON_ASK_MAX;//Ӧ����	
						break;	
					case	CON_NO_ST9:
						uUart_Send_Length=0;	
						uart_sendstring(st9);
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;				
					case	CON_NO_ST10:
						uUart_Send_Length=0;				
						uart_sendstring(st10);
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;	
					case	CON_NO_ST11:
						uUart_Send_Length=0;				
						uart_sendstring(st11);
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;	
					case	CON_NO_ST12:
						uUart_Send_Length=0;								
						uart_sendstring(st12);		
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;				
					case	CON_NO_ST13:
						uUart_Send_Length=0;							
						uart_sendstring(st13);			
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
						break;	
					case	CON_NO_ST14:
						uUart_Send_Length=0;				
						uart_sendstring(st14);
						uCon_Ask_Time=CON_ASK_MAX;//Ӧ����	
					
	//	uCon_Time_Delay=100;//��ʱ1000ms
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
{//����
uUart_Gprs_Send[21]=0;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>225)&&(uSensorData.sensor_data_struct.uWind_Direction<675))
{//������
uUart_Gprs_Send[21]=1;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>=675)&&(uSensorData.sensor_data_struct.uWind_Direction<=1125))
{//����
uUart_Gprs_Send[21]=2;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>1125)&&(uSensorData.sensor_data_struct.uWind_Direction<1575))
{//���Ϸ�
uUart_Gprs_Send[21]=3;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>=1575)&&(uSensorData.sensor_data_struct.uWind_Direction<=2025))
{//�Ϸ�
uUart_Gprs_Send[21]=4;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>2025)&&(uSensorData.sensor_data_struct.uWind_Direction<2475))
{//���Ϸ�
uUart_Gprs_Send[21]=5;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>=2475)&&(uSensorData.sensor_data_struct.uWind_Direction<=2925))
{//����
uUart_Gprs_Send[21]=6;		
}else if((uSensorData.sensor_data_struct.uWind_Direction>2925)&&(uSensorData.sensor_data_struct.uWind_Direction<3375))
{//������
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
//������
uUart_Gprs_Send[40]=0x00;	
uUart_Gprs_Send[41]=0x00;				
uUart_Gprs_Send[42]=0x00;	
//����
uUart_Gprs_Send[43]=0x00;			
uUart_Gprs_Send[44]=0x00;			
//�����¶�
uUart_Gprs_Send[45]=0x00;			
uUart_Gprs_Send[46]=0x00;	
//����ʪ��
uUart_Gprs_Send[47]=0x00;			
uUart_Gprs_Send[48]=0x00;	
//ˮ������
uUart_Gprs_Send[49]=0x00;			
uUart_Gprs_Send[50]=0x00;	
//̫���ܷ���
uUart_Gprs_Send[51]=0x00;			
uUart_Gprs_Send[52]=0x00;	
//����
uUart_Gprs_Send[53]=0x00;			
uUart_Gprs_Send[54]=0x00;	
//CO2				
uUart_Gprs_Send[55]=0x00;			
uUart_Gprs_Send[56]=0x00;	
//��ȩ
uUart_Gprs_Send[57]=0x00;			
uUart_Gprs_Send[58]=0x00;	
//TVOC	
uUart_Gprs_Send[59]=0x00;			
uUart_Gprs_Send[60]=0x00;	
//O2
uUart_Gprs_Send[61]=0x00;			
uUart_Gprs_Send[62]=0x00;	
//ȼ��
uUart_Gprs_Send[63]=0x00;			
uUart_Gprs_Send[64]=0x00;	
//������־
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
//uCon_State_Ask_Time_No=1000;//10��
uTime_Heard_No++;
if(uTime_Heard_No>3)
{
uTime_Heard_No=0;//
uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
uCon_No=CON_NO_START0;		
uCon_Ask_Time=0;//����	


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
					if(uPM25_Data_Send_Time>=30000)//5����
					{
										uCon_Time_Delay=ASK_RES_NEXT_DELAY;//��ʱ100ms
										uCon_No=CON_NO_ST14;	//�ٴη���	
										uCon_Ask_Time=0;//����
						uPM25_Data_Send_Time=0;//����
						
						
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
		if(uTime_Heard_No>=90000)//ʱ���׼,������//15����������
		{
			uTime_Heard_No=0;//����
			
						uCon_No=CON_NO_START0;		
						uCon_Ask_Time=0;//����
			uLine_Work=0;//
				
			
		}	
	
	}else
	{
		uTime_Heard_No=0;
				

	}


		delay_ms(10);

	}
	





}


