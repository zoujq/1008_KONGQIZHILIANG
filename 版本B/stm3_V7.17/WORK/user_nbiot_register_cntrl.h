#ifndef __USER_NBIOT_REGISTER_CNTRL_H__
#define __USER_NBIOT_REGISTER_CNTRL_H__




#define	SYS_TYPE 0x010D		//������ʽ+�ͺ�
#define	SYS_EDITION 0x01	//�汾��
//��01��Ӳ���汾1������汾1
//
//

#define	SYS_TYPE_NO1 0x000506  //3
//////////////////////////////////////////////////////////////
extern	uint64_t SYS_TYPE_NO;



#define SYS_TYPE_NO_ADDR (0x807FED0)//807ffe0

#define CON_ASK_MAX	1000	//10��

#define ASK_RES_NEXT_DELAY 20 //��ʱ������һ��




#define CON_NO_MAX	60


#define CON_NO_START0	CON_NO_MAX
#define CON_NO_START1	CON_NO_MAX-1
#define CON_NO_START2	CON_NO_MAX-2
#define CON_NO_START3	CON_NO_MAX-3
#define CON_NO_START4	CON_NO_MAX-4
#define CON_NO_START5	CON_NO_MAX-5
#define CON_NO_ST0	CON_NO_MAX-6
#define CON_NO_ST1	CON_NO_MAX-7
#define CON_NO_ST2	CON_NO_MAX-8
#define CON_NO_ST3	CON_NO_MAX-9
#define CON_NO_ST4	CON_NO_MAX-10
#define CON_NO_ST5	CON_NO_MAX-11
#define CON_NO_ST6	CON_NO_MAX-12
#define CON_NO_ST7	CON_NO_MAX-13
#define CON_NO_ST8	CON_NO_MAX-14
#define CON_NO_ST9	CON_NO_MAX-15
#define CON_NO_ST10	CON_NO_MAX-16
#define CON_NO_ST11	CON_NO_MAX-17
#define CON_NO_ST12	CON_NO_MAX-18
#define CON_NO_ST13	CON_NO_MAX-19
#define CON_NO_ST14	CON_NO_MAX-20
#define CON_NO_ST15	CON_NO_MAX-21
#define CON_NO_ST16	CON_NO_MAX-22
#define CON_NO_ST17	CON_NO_MAX-23
#define CON_NO_ST18	CON_NO_MAX-24
#define CON_NO_ST19	CON_NO_MAX-25
#define CON_NO_ST20	CON_NO_MAX-26
#define CON_NO_ST21	CON_NO_MAX-27
#define CON_NO_ST22	CON_NO_MAX-28
#define CON_NO_ST23	CON_NO_MAX-29
#define CON_NO_ST24	CON_NO_MAX-30
#define CON_NO_ST25	CON_NO_MAX-31
#define CON_NO_ST26	CON_NO_MAX-32
#define CON_NO_ST27	CON_NO_MAX-33
#define CON_NO_ST28	CON_NO_MAX-34
#define CON_NO_ST29	CON_NO_MAX-35
#define CON_NO_ST30	CON_NO_MAX-36
#define CON_NO_ST31	CON_NO_MAX-37
#define CON_NO_ST32	CON_NO_MAX-38
#define CON_NO_ST33	CON_NO_MAX-39
#define CON_NO_ST34	CON_NO_MAX-40
#define CON_NO_ST35	CON_NO_MAX-41
#define CON_NO_ST36	CON_NO_MAX-42
#define CON_NO_ST37	CON_NO_MAX-43
#define CON_NO_ST38	CON_NO_MAX-44
#define CON_NO_ST39	CON_NO_MAX-45
#define CON_NO_ST40	CON_NO_MAX-46
#define CON_NO_ST41	CON_NO_MAX-47
#define CON_NO_ST42	CON_NO_MAX-48
#define CON_NO_ST43	CON_NO_MAX-49
#define CON_NO_ST44	CON_NO_MAX-50
#define CON_NO_ST45	CON_NO_MAX-51
#define CON_NO_ST46	CON_NO_MAX-52
#define CON_NO_ST47	CON_NO_MAX-53
#define CON_NO_ST48	CON_NO_MAX-54
#define CON_NO_ST49	CON_NO_MAX-55


extern  u8  uCon_No;//���ƺš�ÿִ��һ�ֶ�������1
//extern xdata u8  uCon_No_B;//���ƻ��ظ�ִ�ж�����־
//extern xdata u16  uCon_Time_Delay;//ÿ�ֶ���֮���ӳ�ʱ��
//extern xdata u16  uCon_Ask_Time;//Ӧ��ʱ�䶨ʱ����ʱ��û�ظ����ٴη���

extern  char * str_p;//�ַ���ָ��//
extern u8 str_p_i;//���ݲ��Ҽ���
//extern  char * str_p_nest;//�ַ���ָ��//


extern  u16 crc_check;
extern  u16 crc_check_res;






extern const char	st1[];//������Ƶȫ����
extern const char	st2[];//��������֪ͨ 
extern const char	st3[];//�Զ����� 
extern const char	st4[];//�رյ͹��Ĺ���
extern const char	st5[];//��ѯ�����ip ��ַ�������Դ��ж��Ƿ����ӵ�����
extern const char	st6[];//��ѯUE ״̬������rsrp,sinr,���ǵȼ���С����Ϣ�ȣ���ѡ 
 
extern const char  st7[];//��ѯIMEI ��
extern const char  st8[];//��ѯSIM����Ϣ

extern const char	st29[];//��ѯNB�ź�ǿ�� 




extern const char    st15[];//1 Ϊsocket �ţ�����лظ�OK˵�������ɹ�,UDP


extern const char    st160[];//�����DNS
extern const char    st161[];//�ȸ��DNS
extern const char    st162[];//114��DNS����å


extern  char uDNS_No;//DNS���
extern  u16 uDNS_Res_Time;//DNS�ȴ�Ӧ��ʱ��
extern  u8 uDNS_Res_Time_No;//DNS�ȴ�Ӧ��ʱ��//����


extern  char    st17[20];	//{"AT+NSORF=1,256\r\n\0"};//




struct SYSCNTRL_typedef
{
	
	
	u8 uSys_User_Reset;//ϵͳ�û�����
	u16 uSys_User_Reset_Time;//ϵͳ�û�����,��ʱ		
	u16 uSys_User_Reset_Time_LED_Blink_No;//ϵͳ�û�����,LED��˸��ʾ
	
	
 u8 	uLine_Work;//����״̬	
 u8 	uLine_Work_No_Blink_No;//����״̬//��˸����		
	
	

//  u8  uStart_Send_Data_5;//����������δ����״̬��ÿ10�뷢��һ�����ݣ�������5��

 u8  uData_Send_No_Add;//���ݷ��ʹ����������������㣬û���س�������ģ��	
	
	u8 fSys_User_Other_Data;//������Ϣ�ϴ�			
	


 u16  uData_Send_Time;//���ݷ��Ͷ�ʱ

 //u8  fData_Send_Services_Ask;//�������ݸ���������־ 
 u8  fData_Send_Services_Ask_B;//�������ݸ���������־ 
 u8  fData_Send_Services_Ask_B1;//�������ݸ���������־ 
 
 
 u32 		uTime_Heard_No;//��������
 
 u16 		uTime_Heard_Send_No;//���������������� 
 
 
 
 u8		uSend_Data_Type_No;//�����������ͺţ�1����λ���ݣ�ֻ������ʱ�ŷ���һ�Σ�

 u8		uPlatform_No;//ƽ̨��,0:ע��ƽ̨   1���Լ�ƽ̨   2��OneNETƽ̨
 u8		uDNS_IP_Get_No;//��ȡIP��,0:ע��ƽ̨IP   1���Լ�ƽ̨IP   2��OneNETƽ̨IP
 u8 uDNS_IP_GET[4];//��ȡ�ķ�����IP
 char uSocket_No;//���ӵ�socket�� 
	

//union Var_typedef	Data_Buf;	//���������弰�ṹ�壬�����ڴ�ռ��
u8  	Data_Buf_i;//���ݻ���	
u8  	Data_Buf_strlen;//���ݻ���		

	signed long	NB_RSSI;	//NB�ź�ǿ��
u8 fNB_Open;//��ƽ̨��־	
		



	u8  	uUart_Data_De_Coding[110];//���ݱ����
//NBλ����Ϣ			
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


//NBģ����Ϣ	
	char CGSN[15];//IMEI ��	
	char CIMI[15];//SIM����Ϣ	
	
	u8 uRes_Flag;//������־λ
	
 u32 uRegister_Use_Time;//ע����ʱ


	
};
extern  struct  SYSCNTRL_typedef Sys_Cntrl;
extern  struct  SYSCNTRL_typedef Sys_Cntrl_B;




u8 CTOH(char *Pdata);
u8 HTOC(u8	Pdata);//

void uart_sendstring(char const *str);








#endif
