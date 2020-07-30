#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////////////////////UART 4////////////////////////////////////////////////////////////////////////////

/* .*************************************************************
***功能描述：串口发送字符串函数(带长度)
***入口参数：
						 *pString：要发送的字符串指针
						 Length：发送字符数
***返回值：	 无	 
***调用方法：User_UART1_SendStringx(GPIO_Text,countof(GPIO_Text));
****************************************************************/
u8 User_UART4_SendStringx(char *pString,u16 Length)
{
	u16 SendData_count = 0;
//		FlagStatus Status;	

	
OSSchedLock();//开任务调度锁
	
	
	
   //清除标志位，否则第1位数据会丢失
       USART_ClearFlag(UART4,USART_FLAG_TC);

											
	while(Length-- != 0){
		//User_UART1_SendChar(*pString++);
		
	
		
		 USART_SendData(UART4,*pString++);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);
		
		
		SendData_count++; 
	}	
	
OSSchedUnlock();	//关任务调度锁		  
	
	return SendData_count;	
}



#if EN_UART4_RX   //如果使能了接收

u8  	uUart4_Recive_Data[50]={0};//接收缓存
u8 	uUart4_Recive_Length=0;//接收长度
u8 	uUart4_Recive_End=0;//接收完成

u8  	uUart4_Send_Data[400]={0};//发送缓存
u8 	uUart4_Send_Delay=0;//延时发送



void UART4_IRQHandler(void)                	//串口5中断服务程序
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(UART4);//(USART1->DR);	//读取接收到的数据
		
		uUart4_Recive_End=3;//接收完成后30ms反应
			
			uUart4_Recive_Data[uUart4_Recive_Length]=Res;
			
			uUart4_Recive_Length++;
			if(uUart4_Recive_Length>49)	uUart4_Recive_Length=49;

			
     } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif	
									 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//091209
  
void uart4_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);	//使能GPIOC\D时钟以及复用功能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//使能USART4，时钟	
	
     //USART4_TX   PC.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //USART4_RX	  PC.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);  

	

	
	
	
   //Usart4 NVIC 配置
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(UART4, &USART_InitStructure); //初始化串口
 #if EN_UART4_RX		  //如果使能了接收
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(UART4, ENABLE);                    //使能串口 

}
//////////////////////////////////////////////////////////////////////////////////UART 5////////////////////////////////////////////////////////////////////////////

/* .*************************************************************
***功能描述：串口发送字符串函数(带长度)
***入口参数：
						 *pString：要发送的字符串指针
						 Length：发送字符数
***返回值：	 无	 
***调用方法：User_UART1_SendStringx(GPIO_Text,countof(GPIO_Text));
****************************************************************/
u8 User_UART5_SendStringx(char *pString,u8 Length)
{
	u8 SendData_count = 0;
//		FlagStatus Status;	

	
OSSchedLock();//开任务调度锁
	
	
	
   //清除标志位，否则第1位数据会丢失
       USART_ClearFlag(UART5,USART_FLAG_TC);

											
	while(Length-- != 0){
		//User_UART1_SendChar(*pString++);
		
	
		
		 USART_SendData(UART5,*pString++);
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC)==RESET);
		
		
		SendData_count++; 
	}	
	
OSSchedUnlock();	//关任务调度锁		  
	
	return SendData_count;	
}



#if EN_UART5_RX   //如果使能了接收

u8 volatile 	uUart5_Recive_Data[50]={0};//接收缓存
u8 	uUart5_Recive_Length=0;//接收长度
u8 	uUart5_Recive_End=0;//接收完成

u8 volatile 	uUart5_Send_Data[250]={0};//发送缓存
u8 	uUart5_Send_Delay=0;//延时发送



void UART5_IRQHandler(void)                	//串口5中断服务程序
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//读取接收到的数据
		
		uUart5_Recive_End=3;//接收完成后30ms反应
			
			uUart5_Recive_Data[uUart5_Recive_Length]=Res;
			
			uUart5_Recive_Length++;
			if(uUart5_Recive_Length>49)	uUart5_Recive_Length=49;

			
     } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif	
									 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//091209
  
void uart5_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);	//使能GPIOC\D时钟以及复用功能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//使能USART5，时钟	
	
     //USART5_TX   PC.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC.12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //USART5_RX	  PD.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);  

	
     //USART5_DE   PD.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //PD.3
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
    GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_3);
	
	
	
   //Usart5 NVIC 配置
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(UART5, &USART_InitStructure); //初始化串口
 #if EN_UART5_RX		  //如果使能了接收
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(UART5, ENABLE);                    //使能串口 

}
//////////////////////////////////////////////////////////////////////////////////UART 1////////////////////////////////////////////////////////////////////////////

/* .*************************************************************
***功能描述：串口发送字符串函数(带长度)
***入口参数：
						 *pString：要发送的字符串指针
						 Length：发送字符数
***返回值：	 无	 
***调用方法：User_UART1_SendStringx(GPIO_Text,countof(GPIO_Text));
****************************************************************/
u8 User_UART1_SendStringx(char *pString,u8 Length)
{
	u8 SendData_count = 0;
//		FlagStatus Status;	

	
OSSchedLock();//开任务调度锁
	
	
	
   //清除标志位，否则第1位数据会丢失
       USART_ClearFlag(USART1,USART_FLAG_TC);

											
	while(Length-- != 0){
		//User_UART1_SendChar(*pString++);
		
	
		
		 USART_SendData(USART1,*pString++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
		
		
		SendData_count++; 
	}	
	
OSSchedUnlock();	//关任务调度锁		  
	
	return SendData_count;	
}



#if EN_USART1_RX   //如果使能了接收

u8 volatile 	uUart1_Recive_Data[50]={0};//接收缓存
u8 	uUart1_Recive_Length=0;//接收长度
u8 	uUart1_Recive_End=0;//接收完成

u8 volatile 	uUart1_Send_Data[100]={0};//发送缓存
u8 	uUart1_Send_Delay=0;//延时发送


void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		uUart1_Recive_End=3;//接收完成后30ms反应
			
			uUart1_Recive_Data[uUart1_Recive_Length]=Res;
			
			uUart1_Recive_Length++;
			if(uUart1_Recive_Length>49)	uUart1_Recive_Length=49;

			
     } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif	
									 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//091209
  
void uart1_init(u32 bound){
      //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
//使能USART1，GPIOA时钟
	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO  , ENABLE); 
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);  
 	USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

	






	
   //Usart2 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}

//////////////////////////////////////////////////////////////////////////////////UART 2////////////////////////////////////////////////////////////////////////////

/* .*************************************************************
***功能描述：串口发送字符串函数(带长度)
***入口参数：
						 *pString：要发送的字符串指针
						 Length：发送字符数
***返回值：	 无	 
***调用方法：User_UART1_SendStringx(GPIO_Text,countof(GPIO_Text));
****************************************************************/
u8 User_UART2_SendStringx(char *pString,u8 Length)
{
	u8 SendData_count = 0;
//		FlagStatus Status;	

	
OSSchedLock();//开任务调度锁
	
	
	
   //清除标志位，否则第1位数据会丢失
       USART_ClearFlag(USART2,USART_FLAG_TC);

											
	while(Length-- != 0){
		//User_UART1_SendChar(*pString++);
		
	
		
		 USART_SendData(USART2,*pString++);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		
		
		SendData_count++; 
	}	
	
OSSchedUnlock();	//关任务调度锁		  
	
	return SendData_count;	
}



#if EN_USART2_RX   //如果使能了接收

u8 volatile 	uUart2_Recive_Data[200]={0};//接收缓存
u8 	uUart2_Recive_Length=0;//接收长度
u8 	uUart2_Recive_End=0;//接收完成

u8 volatile 	uUart2_Send_Data[200]={0};//发送缓存
u8 	uUart2_Send_Delay=0;//延时发送


void USART2_IRQHandler(void)                	//串口5中断服务程序
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		
		uUart2_Recive_End=3;//接收完成后30ms反应
			
			uUart2_Recive_Data[uUart2_Recive_Length]=Res;
			
			uUart2_Recive_Length++;
			if(uUart2_Recive_Length>199)	uUart2_Recive_Length=199;

			
     } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif	
									 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//091209
 

void uart2_uninit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	
 	USART_DeInit(USART2);  //复位串口2	
	

   
    //USART2_RX	  PA.3
  //  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;
  //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  //  GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3

	 //USART2_TX   PA.2    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);         
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);   	

}
void uart2_init(u32 bound){
      //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
//使能USART2，GPIOA时钟
	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO  , ENABLE); 
   RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);  
 	USART_DeInit(USART2);  //复位串口2
	 //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3

	

	

	
   //Usart2 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);                    //使能串口 

}

//////////////////////////////////////////////////////////////////////////////////UART 3////////////////////////////////////////////////////////////////////////////

/* .*************************************************************
***功能描述：串口发送字符串函数(带长度)
***入口参数：
						 *pString：要发送的字符串指针
						 Length：发送字符数
***返回值：	 无	 
***调用方法：User_UART3_SendStringx(GPIO_Text,countof(GPIO_Text));
****************************************************************/
u8 User_UART3_SendStringx(char *pString,u8 Length)
{
	u8 SendData_count = 0;
		//FlagStatus Status;	

	
OSSchedLock();//开任务调度锁
	
	
	
   //清除标志位，否则第1位数据会丢失
       USART_ClearFlag(USART3,USART_FLAG_TC);

											
	while(Length-- != 0){
		//User_UART1_SendChar(*pString++);
		
	
		
		 USART_SendData(USART3,*pString++);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
		
		
		SendData_count++; 
	}	
	
OSSchedUnlock();	//关任务调度锁		  
	
	return SendData_count;	
}



#if EN_USART3_RX   //如果使能了接收

u8 volatile 	uUart3_Recive_Data[50]={0};//接收缓存
u8 	uUart3_Recive_Length=0;//接收长度
u8 	uUart3_Recive_End=0;//接收完成

u8 volatile 	uUart3_Send_Data[20]={0};//发送缓存
u8 	uUart3_Send_Delay=0;//延时发送


void USART3_IRQHandler(void)                	//串口5中断服务程序
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		
		uUart3_Recive_End=3;//接收完成后30ms反应
			
			uUart3_Recive_Data[uUart3_Recive_Length]=Res;
			
			uUart3_Recive_Length++;
			if(uUart3_Recive_Length>49)	uUart3_Recive_Length=49;

			
     } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif	
									 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//091209
  
void uart3_init(u32 bound){
      //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
//使能USART3，GPIOB时钟
	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB  |RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO  , ENABLE); 
   RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);  
 	USART_DeInit(USART3);  //复位串口3
	 //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PA2
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PA3


	

	
   //Usart3 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口 

}

