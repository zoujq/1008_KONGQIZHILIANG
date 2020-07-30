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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/14
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//V1.1 20120914
//1,����EXTI8_Init������
//2,����EXTI9_5_IRQHandler����								  
//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);//����
//	if(KEY3==1)	 //WK_UP����
//	{				 
//		BEEP=!BEEP;	
//	}		 
//	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
//}
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY2==0)	 //����2
	{
		LED0=!LED0;	 
	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //���EXTI0��·����λ
}
//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY1==0)	 //����2
	{				 
		LED1=!LED1;	
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //���EXTI0��·����λ
}
//�ⲿ�ж�4�������
void EXTI4_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY0==0)	 //����2
	{
		LED0=!LED0;	 
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //���EXTI0��·����λ
}		   
//�ⲿ�жϳ�ʼ������
//��ʼ��PA0/PE2/PE3/PE4Ϊ�ж�����.
void EXTIX_Init(void)
{   	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

   // KEY_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);

  //GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

 //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

 //GPIOE.4	  �ж����Լ��жϳ�ʼ������  �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  //GPIOA.0	  �ж����Լ��жϳ�ʼ������ �����ش���
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

 	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}	  
vu8 ov_sta;
extern vu8 ov_sta;
 //�ⲿ�ж�5~9�������
void EXTI9_5_IRQHandler(void)
{		 		
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif  		
	if(EXTI_GetITStatus(EXTI_Line8)==SET)//��8�ߵ��ж�
	{     
		if(ov_sta<2)
		{
			if(ov_sta==0)
			{
				OV7670_WRST=0;	 	//��λдָ��		  		 
				OV7670_WRST=1;	
				OV7670_WREN=1;		//����д��FIFO
			}else 
			{
				OV7670_WREN=0;		//��ֹд��FIFO 
				OV7670_WRST=0;	 	//��λдָ��		  		 
				OV7670_WRST=1;	
			}
			ov_sta++;
		}
			EXTI_ClearITPendingBit(EXTI_Line8);  //���EXTI8��·����λ	
		
	}else	 if(EXTI_GetITStatus(EXTI_Line6) != RESET)//�Ʋ��ж�
  {   
    /* Toggle Led connected to PC.07 */
    //GPIO_WriteBit(GPIOC, GPIO_Pin_7, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7)));

    /* Clear EXTI line9 pending bit */
		
		
		 
		/*
		
			if(fSys_Sleep_IS==1)
	{
		SystemInit();	//ϵͳ��ֹͣģʽ���Ѻ������³�ʼ��ʱ��
			uart_init(57600); 	//����1��ʼ��
		fSys_Sleep_IS=0;//����
	}
	*/
			
	///extern u16 uStep_Count_No_Time;//�Ʋ�ֵ,��ʱ�����˲���200ms~700msÿ������Ч������10��������Ч
///extern u16 uStep_Count_No_Data;//�Ʋ�ֵ
		
		
	


	
	
	/*
	
	
	
	
	if(f_User_Sleep==0)//��˯��ģʽ
	{	
		uStep_Count_No_Time=RTC->CNTH;//�õ��������е�ֵ(������)
		uStep_Count_No_Time<<=16;
		uStep_Count_No_Time+=RTC->CNTL;		


		uStep_Count_No_Time_Cos=RTC->DIVH;//�õ��������е�ֵ(32.768������������������������ֵ��ÿ��װ��32768���ݼ����������0���ͻ�������жϡ�)
		uStep_Count_No_Time_Cos<<=16;
		uStep_Count_No_Time_Cos+=RTC->DIVL;	

		
		if(uStep_Count_No_Time>uStep_Count_No_Time_B)//
		{
			
			if((uStep_Count_No_Time-uStep_Count_No_Time_B)>1)
			{
								uStep_Count_No_Data=0;//����
						f_Step_Count_Num_10=0;//����
			}
			else	if((uStep_Count_No_Time-uStep_Count_No_Time_B)==1)
			{
							if(((32768-uStep_Count_No_Time_Cos)+uStep_Count_No_Time_Cos_B)>32768)//���ʱ�䳬��1��
							{
							
										
						uStep_Count_No_Data=0;//����
						f_Step_Count_Num_10=0;//����
							
							}
				
				
				
			}	
		}else	if(uStep_Count_No_Time==uStep_Count_No_Time_B)
		{
		
			if((uStep_Count_No_Time_Cos_B-uStep_Count_No_Time_Cos)<6552)//����֮��С��200ms
			{
			
						uStep_Count_No_Data=0;//����
						f_Step_Count_Num_10=0;//����
			
			
			}
		
			
			
		
		}
		
		
		
		uStep_Count_No_Time_Cos_B=uStep_Count_No_Time_Cos;//����ֵ
		uStep_Count_No_Time_B=uStep_Count_No_Time;//����ֵ
		if(f_Step_Count_Num_10==0)	uStep_Count_No_Data++;
		if(uStep_Count_No_Data>10) 
		{
			uStep_Count_No_Data=0;//����
			f_Step_Count_Num_10=1;//�Ʋ���ʼ
			uStep_Count_No+=10;//��10��
			uStep_Count_No_10min+=10;//��10��
			
		uSedentary_Count_No_Time=0;//����������ÿ10�����廽��һ�Σ���10��1Сʱ�������10�����ϣ������㡣��˯��״̬�£�����10�Σ�����30��

			
		}
		if(f_Step_Count_Num_10==1)
		{
			uStep_Count_No++;//�Ʋ�
			uStep_Count_No_10min++;//�Ʋ�
		}
	}else
	{
		uSleep_Count_No_10min++;//˯��ģʽ�£�������ֱ�Ӽ���
	}
	*/
	/*
	if(f_User_Sleep==0)//��˯��ģʽ
	{	
		uStep_Count_No_Time=RTC->CNTH;//�õ��������е�ֵ(������)
		uStep_Count_No_Time<<=16;
		uStep_Count_No_Time+=RTC->CNTL;			 
		if((uStep_Count_No_Time-uStep_Count_No_Time_B)>1)//2��֮�����2��
		{
			uStep_Count_No_Data=0;
			f_Step_Count_Num_10=0;
		}
		uStep_Count_No_Time_B=uStep_Count_No_Time;//����ֵ
		if(f_Step_Count_Num_10==0)	uStep_Count_No_Data++;
		if(uStep_Count_No_Data>10) 
		{
			uStep_Count_No_Data=0;//����
			f_Step_Count_Num_10=1;//�Ʋ���ʼ
			uStep_Count_No+=10;//��10��
			uStep_Count_No_10min+=10;//��10��
			
		uSedentary_Count_No_Time=0;//����������ÿ10�����廽��һ�Σ���10��1Сʱ�������10�����ϣ������㡣��˯��״̬�£�����10�Σ�����30��

			
		}
		if(f_Step_Count_Num_10==1)
		{
			uStep_Count_No++;//�Ʋ�
			uStep_Count_No_10min++;//�Ʋ�
		}
	}else
	{
		uSleep_Count_No_10min++;//˯��ģʽ�£�������ֱ�Ӽ���
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
				uStep_Count_No_Time=80;//�ٸ�ֵ
		
		if(f_Step_Count_Num_10==0)	uStep_Count_No_Data++;
			if(uStep_Count_No_Data>10) 
			{
				uStep_Count_No_Data=0;//����
				f_Step_Count_Num_10=1;//�Ʋ���ʼ
				uStep_Count_No+=10;//��10��
				uStep_Count_No_10min+=10;//��10��
			
			}
			if(f_Step_Count_Num_10==1)
			{
			
			
			uStep_Count_No++;//�Ʋ�
			uStep_Count_No_10min++;//�Ʋ�
			
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
	fSys_Sleep_IS=1;//ϵͳ����˯��
SLEEP_WKUP_Init();//����ֹͣģʽ

}//ϵͳ˯��
*/

		
		//	User_Sys_Sleep_Check();	//��⿴�ܲ��ܽ���˯��ģʽ,���û��������������͵�������
		
		
		
  }

 

#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();    
#endif    			  
} 
//�ⲿ�ж�8��ʼ��
void EXTI8_Init(void)
{		EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);//PA8���ж���8
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
   
}


















