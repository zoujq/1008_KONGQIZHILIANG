#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////////////////////UART 4////////////////////////////////////////////////////////////////////////////

/* .*************************************************************
***�������������ڷ����ַ�������(������)
***��ڲ�����
						 *pString��Ҫ���͵��ַ���ָ��
						 Length�������ַ���
***����ֵ��	 ��	 
***���÷�����User_UART1_SendStringx(GPIO_Text,countof(GPIO_Text));
****************************************************************/
u8 User_UART4_SendStringx(char *pString,u16 Length)
{
	u16 SendData_count = 0;
//		FlagStatus Status;	

	
OSSchedLock();//�����������
	
	
	
   //�����־λ�������1λ���ݻᶪʧ
       USART_ClearFlag(UART4,USART_FLAG_TC);

											
	while(Length-- != 0){
		//User_UART1_SendChar(*pString++);
		
	
		
		 USART_SendData(UART4,*pString++);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);
		
		
		SendData_count++; 
	}	
	
OSSchedUnlock();	//�����������		  
	
	return SendData_count;	
}



#if EN_UART4_RX   //���ʹ���˽���

u8  	uUart4_Recive_Data[50]={0};//���ջ���
u8 	uUart4_Recive_Length=0;//���ճ���
u8 	uUart4_Recive_End=0;//�������

u8  	uUart4_Send_Data[400]={0};//���ͻ���
u8 	uUart4_Send_Delay=0;//��ʱ����



void UART4_IRQHandler(void)                	//����5�жϷ������
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(UART4);//(USART1->DR);	//��ȡ���յ�������
		
		uUart4_Recive_End=3;//������ɺ�30ms��Ӧ
			
			uUart4_Recive_Data[uUart4_Recive_Length]=Res;
			
			uUart4_Recive_Length++;
			if(uUart4_Recive_Length>49)	uUart4_Recive_Length=49;

			
     } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif	
									 
//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
//CHECK OK
//091209
  
void uart4_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��GPIOC\Dʱ���Լ����ù���ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//ʹ��USART4��ʱ��	
	
     //USART4_TX   PC.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //USART4_RX	  PC.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);  

	

	
	
	
   //Usart4 NVIC ����
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure); //��ʼ������
 #if EN_UART4_RX		  //���ʹ���˽���
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�
#endif
    USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 

}
//////////////////////////////////////////////////////////////////////////////////UART 5////////////////////////////////////////////////////////////////////////////

/* .*************************************************************
***�������������ڷ����ַ�������(������)
***��ڲ�����
						 *pString��Ҫ���͵��ַ���ָ��
						 Length�������ַ���
***����ֵ��	 ��	 
***���÷�����User_UART1_SendStringx(GPIO_Text,countof(GPIO_Text));
****************************************************************/
u8 User_UART5_SendStringx(char *pString,u8 Length)
{
	u8 SendData_count = 0;
//		FlagStatus Status;	

	
OSSchedLock();//�����������
	
	
	
   //�����־λ�������1λ���ݻᶪʧ
       USART_ClearFlag(UART5,USART_FLAG_TC);

											
	while(Length-- != 0){
		//User_UART1_SendChar(*pString++);
		
	
		
		 USART_SendData(UART5,*pString++);
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC)==RESET);
		
		
		SendData_count++; 
	}	
	
OSSchedUnlock();	//�����������		  
	
	return SendData_count;	
}



#if EN_UART5_RX   //���ʹ���˽���

u8 volatile 	uUart5_Recive_Data[50]={0};//���ջ���
u8 	uUart5_Recive_Length=0;//���ճ���
u8 	uUart5_Recive_End=0;//�������

u8 volatile 	uUart5_Send_Data[250]={0};//���ͻ���
u8 	uUart5_Send_Delay=0;//��ʱ����



void UART5_IRQHandler(void)                	//����5�жϷ������
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//��ȡ���յ�������
		
		uUart5_Recive_End=3;//������ɺ�30ms��Ӧ
			
			uUart5_Recive_Data[uUart5_Recive_Length]=Res;
			
			uUart5_Recive_Length++;
			if(uUart5_Recive_Length>49)	uUart5_Recive_Length=49;

			
     } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif	
									 
//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
//CHECK OK
//091209
  
void uart5_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��GPIOC\Dʱ���Լ����ù���ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//ʹ��USART5��ʱ��	
	
     //USART5_TX   PC.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC.12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //USART5_RX	  PD.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOD, &GPIO_InitStructure);  

	
     //USART5_DE   PD.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //PD.3
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
    GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_3);
	
	
	
   //Usart5 NVIC ����
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(UART5, &USART_InitStructure); //��ʼ������
 #if EN_UART5_RX		  //���ʹ���˽���
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�
#endif
    USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ��� 

}
//////////////////////////////////////////////////////////////////////////////////UART 1////////////////////////////////////////////////////////////////////////////

/* .*************************************************************
***�������������ڷ����ַ�������(������)
***��ڲ�����
						 *pString��Ҫ���͵��ַ���ָ��
						 Length�������ַ���
***����ֵ��	 ��	 
***���÷�����User_UART1_SendStringx(GPIO_Text,countof(GPIO_Text));
****************************************************************/
u8 User_UART1_SendStringx(char *pString,u8 Length)
{
	u8 SendData_count = 0;
//		FlagStatus Status;	

	
OSSchedLock();//�����������
	
	
	
   //�����־λ�������1λ���ݻᶪʧ
       USART_ClearFlag(USART1,USART_FLAG_TC);

											
	while(Length-- != 0){
		//User_UART1_SendChar(*pString++);
		
	
		
		 USART_SendData(USART1,*pString++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
		
		
		SendData_count++; 
	}	
	
OSSchedUnlock();	//�����������		  
	
	return SendData_count;	
}



#if EN_USART1_RX   //���ʹ���˽���

u8 volatile 	uUart1_Recive_Data[50]={0};//���ջ���
u8 	uUart1_Recive_Length=0;//���ճ���
u8 	uUart1_Recive_End=0;//�������

u8 volatile 	uUart1_Send_Data[100]={0};//���ͻ���
u8 	uUart1_Send_Delay=0;//��ʱ����


void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		uUart1_Recive_End=3;//������ɺ�30ms��Ӧ
			
			uUart1_Recive_Data[uUart1_Recive_Length]=Res;
			
			uUart1_Recive_Length++;
			if(uUart1_Recive_Length>49)	uUart1_Recive_Length=49;

			
     } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif	
									 
//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
//CHECK OK
//091209
  
void uart1_init(u32 bound){
      //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
//ʹ��USART1��GPIOAʱ��
	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO  , ENABLE); 
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);  
 	USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

	






	
   //Usart2 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

//////////////////////////////////////////////////////////////////////////////////UART 2////////////////////////////////////////////////////////////////////////////

/* .*************************************************************
***�������������ڷ����ַ�������(������)
***��ڲ�����
						 *pString��Ҫ���͵��ַ���ָ��
						 Length�������ַ���
***����ֵ��	 ��	 
***���÷�����User_UART1_SendStringx(GPIO_Text,countof(GPIO_Text));
****************************************************************/
u8 User_UART2_SendStringx(char *pString,u8 Length)
{
	u8 SendData_count = 0;
//		FlagStatus Status;	

	
OSSchedLock();//�����������
	
	
	
   //�����־λ�������1λ���ݻᶪʧ
       USART_ClearFlag(USART2,USART_FLAG_TC);

											
	while(Length-- != 0){
		//User_UART1_SendChar(*pString++);
		
	
		
		 USART_SendData(USART2,*pString++);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		
		
		SendData_count++; 
	}	
	
OSSchedUnlock();	//�����������		  
	
	return SendData_count;	
}



#if EN_USART2_RX   //���ʹ���˽���

u8 volatile 	uUart2_Recive_Data[200]={0};//���ջ���
u8 	uUart2_Recive_Length=0;//���ճ���
u8 	uUart2_Recive_End=0;//�������

u8 volatile 	uUart2_Send_Data[200]={0};//���ͻ���
u8 	uUart2_Send_Delay=0;//��ʱ����


void USART2_IRQHandler(void)                	//����5�жϷ������
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
		
		uUart2_Recive_End=3;//������ɺ�30ms��Ӧ
			
			uUart2_Recive_Data[uUart2_Recive_Length]=Res;
			
			uUart2_Recive_Length++;
			if(uUart2_Recive_Length>199)	uUart2_Recive_Length=199;

			
     } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif	
									 
//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
//CHECK OK
//091209
 

void uart2_uninit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	
 	USART_DeInit(USART2);  //��λ����2	
	

   
    //USART2_RX	  PA.3
  //  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;
  //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  //  GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3

	 //USART2_TX   PA.2    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);         
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);   	

}
void uart2_init(u32 bound){
      //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
//ʹ��USART2��GPIOAʱ��
	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO  , ENABLE); 
   RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);  
 	USART_DeInit(USART2);  //��λ����2
	 //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3

	

	

	
   //Usart2 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

}

//////////////////////////////////////////////////////////////////////////////////UART 3////////////////////////////////////////////////////////////////////////////

/* .*************************************************************
***�������������ڷ����ַ�������(������)
***��ڲ�����
						 *pString��Ҫ���͵��ַ���ָ��
						 Length�������ַ���
***����ֵ��	 ��	 
***���÷�����User_UART3_SendStringx(GPIO_Text,countof(GPIO_Text));
****************************************************************/
u8 User_UART3_SendStringx(char *pString,u8 Length)
{
	u8 SendData_count = 0;
		//FlagStatus Status;	

	
OSSchedLock();//�����������
	
	
	
   //�����־λ�������1λ���ݻᶪʧ
       USART_ClearFlag(USART3,USART_FLAG_TC);

											
	while(Length-- != 0){
		//User_UART1_SendChar(*pString++);
		
	
		
		 USART_SendData(USART3,*pString++);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
		
		
		SendData_count++; 
	}	
	
OSSchedUnlock();	//�����������		  
	
	return SendData_count;	
}



#if EN_USART3_RX   //���ʹ���˽���

u8 volatile 	uUart3_Recive_Data[50]={0};//���ջ���
u8 	uUart3_Recive_Length=0;//���ճ���
u8 	uUart3_Recive_End=0;//�������

u8 volatile 	uUart3_Send_Data[20]={0};//���ͻ���
u8 	uUart3_Send_Delay=0;//��ʱ����


void USART3_IRQHandler(void)                	//����5�жϷ������
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
		
		uUart3_Recive_End=3;//������ɺ�30ms��Ӧ
			
			uUart3_Recive_Data[uUart3_Recive_Length]=Res;
			
			uUart3_Recive_Length++;
			if(uUart3_Recive_Length>49)	uUart3_Recive_Length=49;

			
     } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif	
									 
//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
//CHECK OK
//091209
  
void uart3_init(u32 bound){
      //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
//ʹ��USART3��GPIOBʱ��
	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB  |RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO  , ENABLE); 
   RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);  
 	USART_DeInit(USART3);  //��λ����3
	 //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PA2
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PA3


	

	
   //Usart3 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}

