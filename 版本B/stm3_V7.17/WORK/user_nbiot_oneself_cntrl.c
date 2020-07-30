#include "includes.h"	   



//iot.poscn.com
static const char    st16[] =	{",53,31,3E3A0100000100000000000003696F7405706F73636E03636F6D0000010001\r\n"};//公共


static UTCTime TimeSeconds_res = 0;
static u32 TimeSeconds_res_b = 0;
//static u32 TimeSeconds_b = 0;

static UTCTimeStruct	CTime_Res;//时间结构体
 
 


char User_NBIOT_OneSELF_Thread(thread_t* pt)//自己平台任务
{
	 Thread_BEGIN
	{	

		while(true)
		{
			if(Sys_Cntrl.uPlatform_No==1&&CMachine.UI_Show_No>1)//如果为自己平台
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
						
						Thread_Sleep(200);//延时200ms	//防止串口接收数据没从头开始			
						
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
						Thread_Sleep(200);//延时200ms	//防止串口接收数据没从头开始							
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						uart_sendstring(st29);	

						_net.time_out=1000;//10秒													
						while((str_p=strstr(_net.rxbuf,"OK"))==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	
						{
							if((str_p=strstr(_net.rxbuf,"+CSQ:"))!=0)
							{
								Sys_Cntrl.NB_RSSI=atol(str_p+5);						
								uCon_No=CON_NO_ST19;	
							}
						}
						else	//重启
						{

						}	
						Thread_Sleep(100);//延时100ms
						continue;
					}					
					if(uCon_No==CON_NO_ST19)	
					{
						
								
											
						if(Sys_Cntrl.uSend_Data_Type_No==1)//定位数据
						{
						
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						memset(_net.txbuf,0,_net.txbufmax);
						strcpy(_net.txbuf,"AT+NSOST=2,");
						_net.txbuf[9]=Sys_Cntrl.uSocket_No;					
						sprintf(&_net.txbuf[11],"%d.%d.%d.%d,5683,94,",(unsigned int)Sys_Cntrl.uDNS_IP_GET[0],(unsigned int)Sys_Cntrl.uDNS_IP_GET[1],(unsigned int)Sys_Cntrl.uDNS_IP_GET[2],(unsigned int)Sys_Cntrl.uDNS_IP_GET[3]);							
					
						fUart_Send_End=1;
						_net.txlen=strlen(_net.txbuf);									
						User_UART_Send();	
						while(fUart_Send_End);

						//数据头					
						Sys_Cntrl.uUart_Data_De_Coding[0]=0x14;
						//有效数据长度
						Sys_Cntrl.uUart_Data_De_Coding[1]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[2]=0x5E;		
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
						Sys_Cntrl.uUart_Data_De_Coding[28]=0x00;//上传数据
						//NB移动定位信息
						Sys_Cntrl.uUart_Data_De_Coding[29]=0x43;
						Sys_Cntrl.uUart_Data_De_Coding[30]=((Sys_Cntrl.Signal_power&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[31]=((Sys_Cntrl.Signal_power&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[32]=((Sys_Cntrl.Signal_power&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[33]=(Sys_Cntrl.Signal_power&0xff);											
						//
						Sys_Cntrl.uUart_Data_De_Coding[34]=0x44;
						Sys_Cntrl.uUart_Data_De_Coding[35]=((Sys_Cntrl.Total_power&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[36]=((Sys_Cntrl.Total_power&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[37]=((Sys_Cntrl.Total_power&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[38]=(Sys_Cntrl.Total_power&0xff);			
						//
						Sys_Cntrl.uUart_Data_De_Coding[39]=0x45;
						Sys_Cntrl.uUart_Data_De_Coding[40]=((Sys_Cntrl.TX_power&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[41]=((Sys_Cntrl.TX_power&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[42]=((Sys_Cntrl.TX_power&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[43]=(Sys_Cntrl.TX_power&0xff);				
						//
						Sys_Cntrl.uUart_Data_De_Coding[44]=0x46;
						Sys_Cntrl.uUart_Data_De_Coding[45]=((Sys_Cntrl.TX_time&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[46]=((Sys_Cntrl.TX_time&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[47]=((Sys_Cntrl.TX_time&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[48]=(Sys_Cntrl.TX_time&0xff);			
						//
						Sys_Cntrl.uUart_Data_De_Coding[49]=0x47;
						Sys_Cntrl.uUart_Data_De_Coding[50]=((Sys_Cntrl.RX_time&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[51]=((Sys_Cntrl.RX_time&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[52]=((Sys_Cntrl.RX_time&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[53]=(Sys_Cntrl.RX_time&0xff);			
						//
						Sys_Cntrl.uUart_Data_De_Coding[54]=0x48;
						Sys_Cntrl.uUart_Data_De_Coding[55]=((Sys_Cntrl.Cell_ID&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[56]=((Sys_Cntrl.Cell_ID&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[57]=((Sys_Cntrl.Cell_ID&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[58]=(Sys_Cntrl.Cell_ID&0xff);			
						//
						Sys_Cntrl.uUart_Data_De_Coding[59]=0x49;
						Sys_Cntrl.uUart_Data_De_Coding[60]=((Sys_Cntrl.ECL&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[61]=((Sys_Cntrl.ECL&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[62]=((Sys_Cntrl.ECL&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[63]=(Sys_Cntrl.ECL&0xff);			
						//
						Sys_Cntrl.uUart_Data_De_Coding[64]=0x4A;
						Sys_Cntrl.uUart_Data_De_Coding[65]=((Sys_Cntrl.SNR&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[66]=((Sys_Cntrl.SNR&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[67]=((Sys_Cntrl.SNR&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[68]=(Sys_Cntrl.SNR&0xff);			
						//
						Sys_Cntrl.uUart_Data_De_Coding[69]=0x4B;
						Sys_Cntrl.uUart_Data_De_Coding[70]=((Sys_Cntrl.EARFCN&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[71]=((Sys_Cntrl.EARFCN&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[72]=((Sys_Cntrl.EARFCN&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[73]=(Sys_Cntrl.EARFCN&0xff);			
						//
						Sys_Cntrl.uUart_Data_De_Coding[74]=0x4C;
						Sys_Cntrl.uUart_Data_De_Coding[75]=((Sys_Cntrl.PCI&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[76]=((Sys_Cntrl.PCI&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[77]=((Sys_Cntrl.PCI&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[78]=(Sys_Cntrl.PCI&0xff);			
						//
						Sys_Cntrl.uUart_Data_De_Coding[79]=0x4D;
						Sys_Cntrl.uUart_Data_De_Coding[80]=((Sys_Cntrl.RSRQ&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[81]=((Sys_Cntrl.RSRQ&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[82]=((Sys_Cntrl.RSRQ&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[83]=(Sys_Cntrl.RSRQ&0xff);			
						//
						Sys_Cntrl.uUart_Data_De_Coding[84]=0x4E;
						Sys_Cntrl.uUart_Data_De_Coding[85]=((Sys_Cntrl.OPERATOR_MODE&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[86]=((Sys_Cntrl.OPERATOR_MODE&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[87]=((Sys_Cntrl.OPERATOR_MODE&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[88]=(Sys_Cntrl.OPERATOR_MODE&0xff);			
						//
						Sys_Cntrl.uUart_Data_De_Coding[89]=0x4F;
						Sys_Cntrl.uUart_Data_De_Coding[90]=((Sys_Cntrl.CURRENT_BAND&0xff000000)>>24);
						Sys_Cntrl.uUart_Data_De_Coding[91]=((Sys_Cntrl.CURRENT_BAND&0xff0000)>>16);
						Sys_Cntrl.uUart_Data_De_Coding[92]=((Sys_Cntrl.CURRENT_BAND&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[93]=(Sys_Cntrl.CURRENT_BAND&0xff);									

						
						crc_check=GetCrC((unsigned char *)&Sys_Cntrl.uUart_Data_De_Coding[7],87);
						//校验位
						Sys_Cntrl.uUart_Data_De_Coding[3]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[4]=0x00;							
						Sys_Cntrl.uUart_Data_De_Coding[5]=((crc_check&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[6]=(crc_check&0xff);
						
						
						
						for(Sys_Cntrl.Data_Buf_i=0;Sys_Cntrl.Data_Buf_i<50;Sys_Cntrl.Data_Buf_i++)
						{
							_net.txbuf[2*Sys_Cntrl.Data_Buf_i]=HTOC(((Sys_Cntrl.uUart_Data_De_Coding[Sys_Cntrl.Data_Buf_i])&0xf0)>>4);		
							_net.txbuf[2*Sys_Cntrl.Data_Buf_i+1]=HTOC((Sys_Cntrl.uUart_Data_De_Coding[Sys_Cntrl.Data_Buf_i])&0xf);		
						}	
						
						fUart_Send_End=1;
						_net.txlen=strlen(_net.txbuf);									
						User_UART_Send();	
						while(fUart_Send_End);		
					
						for(Sys_Cntrl.Data_Buf_i=0;Sys_Cntrl.Data_Buf_i<44;Sys_Cntrl.Data_Buf_i++)
						{
							_net.txbuf[2*Sys_Cntrl.Data_Buf_i]=HTOC(((Sys_Cntrl.uUart_Data_De_Coding[Sys_Cntrl.Data_Buf_i+50])&0xf0)>>4);		
							_net.txbuf[2*Sys_Cntrl.Data_Buf_i+1]=HTOC((Sys_Cntrl.uUart_Data_De_Coding[Sys_Cntrl.Data_Buf_i+50])&0xf);		
						}				
			
								
						_net.txbuf[2*Sys_Cntrl.Data_Buf_i]=0x0D;	
						_net.txbuf[2*Sys_Cntrl.Data_Buf_i+1]=0x0A;	
						_net.txbuf[2*Sys_Cntrl.Data_Buf_i+2]=0;					
						Sys_Cntrl.Data_Buf_strlen=strlen(_net.txbuf);	
							
						_net.txlen=Sys_Cntrl.Data_Buf_strlen;
						User_UART_Send();				
					}else	//正常数据
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						memset(_net.txbuf,0,_net.txbufmax);
						strcpy(_net.txbuf,"AT+NSOST=2,");
						_net.txbuf[9]=Sys_Cntrl.uSocket_No;					
						sprintf(&_net.txbuf[11],"%d.%d.%d.%d,5683,69,",(unsigned int)Sys_Cntrl.uDNS_IP_GET[0],(unsigned int)Sys_Cntrl.uDNS_IP_GET[1],(unsigned int)Sys_Cntrl.uDNS_IP_GET[2],(unsigned int)Sys_Cntrl.uDNS_IP_GET[3]);							
					
						fUart_Send_End=1;
						_net.txlen=strlen(_net.txbuf);									
						User_UART_Send();	
						while(fUart_Send_End);
						
						
						//数据头					
						Sys_Cntrl.uUart_Data_De_Coding[0]=0x14;
						//有效数据长度
						Sys_Cntrl.uUart_Data_De_Coding[1]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[2]=0x45;		
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


						//PM2.5
						Sys_Cntrl.uUart_Data_De_Coding[24]=0x03;
						Sys_Cntrl.uUart_Data_De_Coding[25]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[26]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[27]=((CMachine.pm25&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[28]=(CMachine.pm25&0xff);





						//温度
						Sys_Cntrl.uUart_Data_De_Coding[29]=0x08;
						Sys_Cntrl.uUart_Data_De_Coding[30]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[31]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[32]=(((CMachine.temp*10)&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[33]=((CMachine.temp*10)&0xff);

						//湿度
						Sys_Cntrl.uUart_Data_De_Coding[34]=0x09;
						Sys_Cntrl.uUart_Data_De_Coding[35]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[36]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[37]=(((CMachine.humidity*10)&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[38]=((CMachine.humidity*10)&0xff);

						//TVOC
						Sys_Cntrl.uUart_Data_De_Coding[39]=0x0D;
						Sys_Cntrl.uUart_Data_De_Coding[40]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[41]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[42]=((CMachine.tvoc&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[43]=(CMachine.tvoc&0xff);

						//CO2
						Sys_Cntrl.uUart_Data_De_Coding[44]=0x0E;
						Sys_Cntrl.uUart_Data_De_Coding[45]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[46]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[47]=((CMachine.co2&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[48]=(CMachine.co2&0xff);

						//电量
						Sys_Cntrl.uUart_Data_De_Coding[49]=0x2B;
						Sys_Cntrl.uUart_Data_De_Coding[50]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[51]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[52]=((CMachine.Battery&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[53]=(CMachine.Battery&0xff);

						//无线信号强度
						Sys_Cntrl.uUart_Data_De_Coding[54]=0x3E;
						Sys_Cntrl.uUart_Data_De_Coding[55]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[56]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[57]=((Sys_Cntrl.NB_RSSI&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[58]=(Sys_Cntrl.NB_RSSI&0xff);
										

						if(Sys_Cntrl.fData_Send_Services_Ask_B1==1)
						{
							Sys_Cntrl.fData_Send_Services_Ask_B1=0;

							Sys_Cntrl.uUart_Data_De_Coding[59]=0x3C;
							Sys_Cntrl.uUart_Data_De_Coding[60]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[61]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[62]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[63]=0x01;

						}else
						{
							Sys_Cntrl.uUart_Data_De_Coding[59]=0x3C;
							Sys_Cntrl.uUart_Data_De_Coding[60]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[61]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[62]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[63]=0x00;
						}


						if(Sys_Cntrl.uSys_User_Reset==1)
						{
							Sys_Cntrl.uUart_Data_De_Coding[64]=0x3D;
							Sys_Cntrl.uUart_Data_De_Coding[65]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[66]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[67]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[68]=0x01;
						}else
						{
							Sys_Cntrl.uUart_Data_De_Coding[64]=0x3D;
							Sys_Cntrl.uUart_Data_De_Coding[65]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[66]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[67]=0x00;
							Sys_Cntrl.uUart_Data_De_Coding[68]=0x00;
						}



						crc_check=GetCrC((unsigned char *)&Sys_Cntrl.uUart_Data_De_Coding[7],62);
								
						//校验位
						Sys_Cntrl.uUart_Data_De_Coding[3]=0x00;
						Sys_Cntrl.uUart_Data_De_Coding[4]=0x00;							
						Sys_Cntrl.uUart_Data_De_Coding[5]=((crc_check&0xff00)>>8);
						Sys_Cntrl.uUart_Data_De_Coding[6]=(crc_check&0xff);			
						


						for(Sys_Cntrl.Data_Buf_i=0;Sys_Cntrl.Data_Buf_i<50;Sys_Cntrl.Data_Buf_i++)
						{
							_net.txbuf[2*Sys_Cntrl.Data_Buf_i]=HTOC(((Sys_Cntrl.uUart_Data_De_Coding[Sys_Cntrl.Data_Buf_i])&0xf0)>>4);		
							_net.txbuf[2*Sys_Cntrl.Data_Buf_i+1]=HTOC((Sys_Cntrl.uUart_Data_De_Coding[Sys_Cntrl.Data_Buf_i])&0xf);		
						}	
						
						fUart_Send_End=1;
						_net.txlen=strlen(_net.txbuf);									
						User_UART_Send();	
						while(fUart_Send_End);		
					
						for(Sys_Cntrl.Data_Buf_i=0;Sys_Cntrl.Data_Buf_i<19;Sys_Cntrl.Data_Buf_i++)
						{
							_net.txbuf[2*Sys_Cntrl.Data_Buf_i]=HTOC(((Sys_Cntrl.uUart_Data_De_Coding[Sys_Cntrl.Data_Buf_i+50])&0xf0)>>4);		
							_net.txbuf[2*Sys_Cntrl.Data_Buf_i+1]=HTOC((Sys_Cntrl.uUart_Data_De_Coding[Sys_Cntrl.Data_Buf_i+50])&0xf);		
						}										
						_net.txbuf[2*Sys_Cntrl.Data_Buf_i]=0x0D;	
						_net.txbuf[2*Sys_Cntrl.Data_Buf_i+1]=0x0A;	
						_net.txbuf[2*Sys_Cntrl.Data_Buf_i+2]=0;					
						Sys_Cntrl.Data_Buf_strlen=strlen(_net.txbuf);	
							
						_net.txlen=Sys_Cntrl.Data_Buf_strlen;
						User_UART_Send();				

					}
									
						_net.time_out=1000;//10秒													
						while((str_p=strstr(_net.rxbuf,"+NSONMI"))==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
						}					
						if(_net.time_out)	
						{								
							uCon_No=CON_NO_ST20;//下一步
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
					if(uCon_No==CON_NO_ST20)	
					{
						memset(_net.rxbuf,0,_net.rxbufmax);//清接收内存	
						memset(st17,0,20);
						strcpy(st17,"AT+NSORF=1,256\r\n");
						st17[9]=Sys_Cntrl.uSocket_No;
						uart_sendstring(st17);

						_net.time_out=100;//1秒			
						while(strstr(_net.rxbuf,"1400")==0 && --_net.time_out)//等待串口接收完毕或超时退出
						{		
							Thread_Sleep(10);
							
							
							if(Sys_Cntrl.uSys_User_Reset_Time>0)	Sys_Cntrl.uSys_User_Reset_Time--;			
							

		
							
							if(
			
									(Sys_Cntrl.uSys_User_Reset==1&&((Sys_Cntrl.uSys_User_Reset_Time%2000)==1999))
								)
							{

								Sys_Cntrl.uData_Send_Time=5900;//1秒后发数据
							}			

							if(	
									Sys_Cntrl.fData_Send_Services_Ask_B==1
								)
							{
								Sys_Cntrl.fData_Send_Services_Ask_B=0;
								

						
								_net.time_out=0;//下发控制，直接退出
								
								Sys_Cntrl.uData_Send_Time=5900;//1秒后发数据
								Sys_Cntrl.fData_Send_Services_Ask_B1=1;//标志
							}


							
							Sys_Cntrl.uData_Send_Time++;//10ms
							if(Sys_Cntrl.uData_Send_Time>=6000)//1分钟//1分钟一次包，防止端口老化
							{

								uCon_No=CON_NO_ST18;
								Sys_Cntrl.uData_Send_Time=0;//清零
								
								Sys_Cntrl.uData_Send_No_Add++;
								if(Sys_Cntrl.uData_Send_No_Add>5)//连续5次没有返回
								{
									Sys_Cntrl.uData_Send_No_Add=0;//
									Sys_Cntrl.uTime_Heard_No=0;//重启			
									Sys_Cntrl.uTime_Heard_Send_No=0;//重启				
									uCon_No=CON_NO_START0;	
									Sys_Cntrl.uLine_Work=0;//			
								}
								Thread_Sleep(100);//加延时防止串口冲突
								break;;//退出，重新发送
							}														
							
							
							
							
							
						}																
						if(_net.time_out)	
						{			

							Thread_Sleep(200);//延时200ms接收完数据
							
							if((str_p=strstr(_net.rxbuf,"1400"))!=0)
							{								


									Sys_Cntrl.uData_Send_No_Add=0;//清零
									
									//重样开始计数
									if(Sys_Cntrl.uData_Send_Time>5500)	Sys_Cntrl.uData_Send_Time=5500;//最少延时5秒后再次发送
									Sys_Cntrl.uTime_Heard_No=0;//心跳清零													
												

									
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

									//	uUart_Recive_Data_Decode[5]=CTOH((str_p+14));		//校验方法
									//	uUart_Recive_Data_Decode[6]=CTOH((str_p+16));		//加密方法
										
									//	uUart_Recive_Data_Decode[7]=CTOH((str_p+18));		//数据字典码
										
									//	uUart_Recive_Data_Decode[8]=CTOH((str_p+20));		//数据值
									//	uUart_Recive_Data_Decode[9]=CTOH((str_p+22));		
									//	uUart_Recive_Data_Decode[10]=CTOH((str_p+24));		
									//	uUart_Recive_Data_Decode[11]=CTOH((str_p+26));											
										
		
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
												if(Sys_Cntrl.uUart_Data_De_Coding[7+Sys_Cntrl.Data_Buf_i]==0x2C)
												{
													Sys_Cntrl.uLine_Work=1;
													Sys_Cntrl.uSend_Data_Type_No=0;//清零
									
//													CMachine.fSys_User_Other_Data=0;//清零
													
																			
														TimeSeconds_res=0;
								
														TimeSeconds_res_b=Sys_Cntrl.uUart_Data_De_Coding[7+Sys_Cntrl.Data_Buf_i+1];
														TimeSeconds_res_b=((TimeSeconds_res_b<<24)&0xff000000);				
														TimeSeconds_res=TimeSeconds_res_b;
														TimeSeconds_res_b=Sys_Cntrl.uUart_Data_De_Coding[7+Sys_Cntrl.Data_Buf_i+2];
														TimeSeconds_res_b=((TimeSeconds_res_b<<16)&0xff0000);				
														TimeSeconds_res|=TimeSeconds_res_b;				
														TimeSeconds_res_b=Sys_Cntrl.uUart_Data_De_Coding[7+Sys_Cntrl.Data_Buf_i+3];
														TimeSeconds_res_b=((TimeSeconds_res_b<<8)&0xff00);				
														TimeSeconds_res|=TimeSeconds_res_b;							
														TimeSeconds_res_b=Sys_Cntrl.uUart_Data_De_Coding[7+Sys_Cntrl.Data_Buf_i+4];
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
														
													
													
												}
												if(Sys_Cntrl.uUart_Data_De_Coding[7+Sys_Cntrl.Data_Buf_i]==0x3D)
												{
													
													Sys_Cntrl.uSys_User_Reset=0;//系统用户重置//清零
													Sys_Cntrl.uSys_User_Reset_Time=0;//系统用户重置,计时//清零			
													Sys_Cntrl.uLine_Work=1;
												}			
											}
										}						
									}								
							}							
						}
						else	//重启
						{

						}	
						//Thread_Sleep(100);//延时100ms
						continue;
					}					
					
								
			
								
					
			
			}
				Thread_Sleep(10);
		
		}
	}
	Thread_END
}



char User_NBIOT_OneSELF_Scan_Thread(thread_t* pt)//自己平台,常监测
{

	 Thread_BEGIN
	{

			while(true)
			{
				
				if(Sys_Cntrl.uPlatform_No==1&&CMachine.UI_Show_No>1)//如果为自己平台
				{
					//防止异常没有发数据
					Sys_Cntrl.uTime_Heard_Send_No++;
					if(Sys_Cntrl.uTime_Heard_Send_No>=18000)//发送数据,心跳包//3分钟无数据发送
					{
						Sys_Cntrl.uTime_Heard_No=0;//重启			
						Sys_Cntrl.uTime_Heard_Send_No=0;//重启				
						uCon_No=CON_NO_START0;		
						Sys_Cntrl.uLine_Work=0;//
					}		
					if(Sys_Cntrl.uLine_Work==1)
					{
						Sys_Cntrl.uTime_Heard_No++;
						if(Sys_Cntrl.uTime_Heard_No>=30000)//时间较准,心跳包//5分钟无数据
						{
							Sys_Cntrl.uTime_Heard_No=0;//重启			
							Sys_Cntrl.uTime_Heard_Send_No=0;//重启				
							uCon_No=CON_NO_START0;	
							Sys_Cntrl.uLine_Work=0;//				
						}	
					}else
					{
						Sys_Cntrl.uTime_Heard_No=0;
					}
				}		
				Thread_Sleep(10);
			}
		}
		Thread_END
}


