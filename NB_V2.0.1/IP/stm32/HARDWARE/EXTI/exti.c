#include "exti.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "ov7670.h"
#include "includes.h"
#include "wkup.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/14
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//V1.1 20120914
//1,增加EXTI8_Init函数。
//2,增加EXTI9_5_IRQHandler函数								  
//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);//消抖
//	if(KEY3==1)	 //WK_UP按键
//	{				 
//		BEEP=!BEEP;	
//	}		 
//	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
//}
//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY2==0)	 //按键2
	{
		LED0=!LED0;	 
	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除EXTI0线路挂起位
}
//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==0)	 //按键2
	{				 
		LED1=!LED1;	
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除EXTI0线路挂起位
}
//外部中断4服务程序
void EXTI4_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY0==0)	 //按键2
	{
		LED0=!LED0;	 
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除EXTI0线路挂起位
}		   
//外部中断初始化程序
//初始化PA0/PE2/PE3/PE4为中断输入.
void EXTIX_Init(void)
{   	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

   // KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);

  //GPIOE.2 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

 //GPIOE.3	  中断线以及中断初始化配置 下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

 //GPIOE.4	  中断线以及中断初始化配置  下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  //GPIOA.0	  中断线以及中断初始化配置 上升沿触发
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

 	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}	  
vu8 ov_sta;
extern vu8 ov_sta;
 //外部中断5~9服务程序
void EXTI9_5_IRQHandler(void)
{		 		
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif  		
	if(EXTI_GetITStatus(EXTI_Line8)==SET)//是8线的中断
	{     
		if(ov_sta<2)
		{
			if(ov_sta==0)
			{
				OV7670_WRST=0;	 	//复位写指针		  		 
				OV7670_WRST=1;	
				OV7670_WREN=1;		//允许写入FIFO
			}else 
			{
				OV7670_WREN=0;		//禁止写入FIFO 
				OV7670_WRST=0;	 	//复位写指针		  		 
				OV7670_WRST=1;	
			}
			ov_sta++;
		}
			EXTI_ClearITPendingBit(EXTI_Line8);  //清除EXTI8线路挂起位	
		
	}else	 if(EXTI_GetITStatus(EXTI_Line6) != RESET)//计步中断
  {   
    /* Toggle Led connected to PC.07 */
    //GPIO_WriteBit(GPIOC, GPIO_Pin_7, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7)));

    /* Clear EXTI line9 pending bit */
		
		
		 
		/*
		
			if(fSys_Sleep_IS==1)
	{
		SystemInit();	//系统从停止模式唤醒后，需重新初始化时钟
			uart_init(57600); 	//串口1初始化
		fSys_Sleep_IS=0;//唤醒
	}
	*/
			
	///extern u16 uStep_Count_No_Time;//计步值,用时间来滤波，200ms~700ms每步，有效，连续10次以上有效
///extern u16 uStep_Count_No_Data;//计步值
		
		
	


	
	
	/*
	
	
	
	
	if(f_User_Sleep==0)//非睡眠模式
	{	
		uStep_Count_No_Time=RTC->CNTH;//得到计数器中的值(秒钟数)
		uStep_Count_No_Time<<=16;
		uStep_Count_No_Time+=RTC->CNTL;		


		uStep_Count_No_Time_Cos=RTC->DIVH;//得到计数器中的值(32.768晶振产生的脉冲个数计数器的值。每次装载32768，递减，如果减到0，就会产生秒中断。)
		uStep_Count_No_Time_Cos<<=16;
		uStep_Count_No_Time_Cos+=RTC->DIVL;	

		
		if(uStep_Count_No_Time>uStep_Count_No_Time_B)//
		{
			
			if((uStep_Count_No_Time-uStep_Count_No_Time_B)>1)
			{
								uStep_Count_No_Data=0;//清零
						f_Step_Count_Num_10=0;//清零
			}
			else	if((uStep_Count_No_Time-uStep_Count_No_Time_B)==1)
			{
							if(((32768-uStep_Count_No_Time_Cos)+uStep_Count_No_Time_Cos_B)>32768)//如果时间超过1秒
							{
							
										
						uStep_Count_No_Data=0;//清零
						f_Step_Count_Num_10=0;//清零
							
							}
				
				
				
			}	
		}else	if(uStep_Count_No_Time==uStep_Count_No_Time_B)
		{
		
			if((uStep_Count_No_Time_Cos_B-uStep_Count_No_Time_Cos)<6552)//两步之间小于200ms
			{
			
						uStep_Count_No_Data=0;//清零
						f_Step_Count_Num_10=0;//清零
			
			
			}
		
			
			
		
		}
		
		
		
		uStep_Count_No_Time_Cos_B=uStep_Count_No_Time_Cos;//保存值
		uStep_Count_No_Time_B=uStep_Count_No_Time;//保存值
		if(f_Step_Count_Num_10==0)	uStep_Count_No_Data++;
		if(uStep_Count_No_Data>10) 
		{
			uStep_Count_No_Data=0;//清零
			f_Step_Count_Num_10=1;//计步开始
			uStep_Count_No+=10;//加10步
			uStep_Count_No_10min+=10;//加10步
			
		uSedentary_Count_No_Time=0;//久坐计数，每10分闹铃唤醒一次，计10次1小时。如果走10步以上，就清零。非睡眠状态下，计满10次，提醒30秒

			
		}
		if(f_Step_Count_Num_10==1)
		{
			uStep_Count_No++;//计步
			uStep_Count_No_10min++;//计步
		}
	}else
	{
		uSleep_Count_No_10min++;//睡眠模式下，动几次直接计数
	}
	*/
	/*
	if(f_User_Sleep==0)//非睡眠模式
	{	
		uStep_Count_No_Time=RTC->CNTH;//得到计数器中的值(秒钟数)
		uStep_Count_No_Time<<=16;
		uStep_Count_No_Time+=RTC->CNTL;			 
		if((uStep_Count_No_Time-uStep_Count_No_Time_B)>1)//2步之间大于2秒
		{
			uStep_Count_No_Data=0;
			f_Step_Count_Num_10=0;
		}
		uStep_Count_No_Time_B=uStep_Count_No_Time;//保存值
		if(f_Step_Count_Num_10==0)	uStep_Count_No_Data++;
		if(uStep_Count_No_Data>10) 
		{
			uStep_Count_No_Data=0;//清零
			f_Step_Count_Num_10=1;//计步开始
			uStep_Count_No+=10;//加10步
			uStep_Count_No_10min+=10;//加10步
			
		uSedentary_Count_No_Time=0;//久坐计数，每10分闹铃唤醒一次，计10次1小时。如果走10步以上，就清零。非睡眠状态下，计满10次，提醒30秒

			
		}
		if(f_Step_Count_Num_10==1)
		{
			uStep_Count_No++;//计步
			uStep_Count_No_10min++;//计步
		}
	}else
	{
		uSleep_Count_No_10min++;//睡眠模式下，动几次直接计数
	}
	*/
	
	
	//f_Step_Count_Num_10_B
	
	
	
	//uStep_Count_No_Time_10S
	
	
	
	
	
	//uStep_Count_No_Time_10S_B
	
	
	
	/*
	if((uStep_Count_No_Time>=60)||uStep_Count_No_Time==0) 
		{
			uStep_Count_No_Time=80;
			uStep_Count_No_Data=0;
			f_Step_Count_Num_10=0;
		}
		else 
		{
				uStep_Count_No_Time=80;//再给值
		
		if(f_Step_Count_Num_10==0)	uStep_Count_No_Data++;
			if(uStep_Count_No_Data>10) 
			{
				uStep_Count_No_Data=0;//清零
				f_Step_Count_Num_10=1;//计步开始
				uStep_Count_No+=10;//加10步
				uStep_Count_No_10min+=10;//加10步
			
			}
			if(f_Step_Count_Num_10==1)
			{
			
			
			uStep_Count_No++;//计步
			uStep_Count_No_10min++;//计步
			
			}
			
		
		
		}
		*/
		
		
		/*
		uStep_Count_No++;
		if(uStep_Count_No>=200) uStep_Count_No=0;//
		*/
		
		
		
		EXTI_ClearITPendingBit(EXTI_Line6);
   
		/*
if(u_Rang_Remind_No==0&&u_Sedentary_Remind_No==0&&f_Action3d_Save_NoSleep==0&&u_Remind_Time_No==0&&u_Remind_No==0&&uBuzzer_No==0&&uBuzzer_Run_No==0&&fSys_Sleep==1&&USART_RX_STA==0&&fSys_Sleep_Up_Uart==0&&((((uint16_t)GPIOA->IDR)&0x0001)==0x0000)) 
{
	fSys_Sleep_IS=1;//系统进入睡眠
SLEEP_WKUP_Init();//进入停止模式

}//系统睡眠
*/

		
		//	User_Sys_Sleep_Check();	//检测看能不能进入睡眠模式,调用或放在任务优先最低的任务里
		
		
		
  }

 

#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();    
#endif    			  
} 
//外部中断8初始化
void EXTI8_Init(void)
{		EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);//PA8对中断线8
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
   
}


















