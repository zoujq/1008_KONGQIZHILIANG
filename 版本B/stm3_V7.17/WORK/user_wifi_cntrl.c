#include "includes.h"	   




void Uart5_Decode(void)//���ڴ����� 
{


		
	uUart5_Recive_Length=0;
	

				
	memset((void*)uUart5_Recive_Data,0,50);
	
}

unsigned short UI_Show_No_B_WIFI_Task=0xff;//
char User_WIFI_Thread(thread_t* pt)
{




     Thread_BEGIN
    {


    GPIO_InitTypeDef  GPIO_InitStructure;  		  

	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��

	
	while(1)
	{			  
		
		
		if(CMachine.UI_Show_No<2)
		{
			
			if(UI_Show_No_B_WIFI_Task!=0)
			{

	 //uart5_uninit();		
				
	  //USART5_RX	  PD.2	//USART5_TX   PC.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOD, &GPIO_InitStructure); //��ʼ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��
				
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);         
		GPIO_ResetBits(GPIOC,GPIO_Pin_12);      

		//״̬������nLINK		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��
		GPIO_ResetBits(GPIOE,GPIO_Pin_4);         				
				
				
		//��Դʹ�ܽ�//RF����
   /* GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��
		GPIO_SetBits(GPIOA,GPIO_Pin_12);    				
				*/
				
		//Reset
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��
		GPIO_ResetBits(GPIOE,GPIO_Pin_2);         			
		//Reload
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��
		GPIO_ResetBits(GPIOE,GPIO_Pin_3);         			

				

				
			UI_Show_No_B_WIFI_Task=0;
			}
			 		Thread_Sleep(10);
					continue;//�˳�
			
			
		}else
		{
		
			
			if(UI_Show_No_B_WIFI_Task==0)
			{
			
				



			UI_Show_No_B_WIFI_Task=0xff;
			}
			
			

					
			
			
						
		}
		
		

 		Thread_Sleep(10);
	}



    }
    Thread_END










}





























