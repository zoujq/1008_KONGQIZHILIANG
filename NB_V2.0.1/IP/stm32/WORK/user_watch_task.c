#include "includes.h"	   





 Machine CMachine;
 Machine CMachine_B;//֮ǰ��ˢ����








//PA0 ��PA11 WKUP���ѳ�ʼ��
void SLEEP_WKUP_Init(void)
{	



	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
					PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
	PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ 		 






}






void SYS_POWER_OFF(void)//ϵͳ�ر�
{	


//	IWDG_Init(4,625);    //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	   

	IWDG_Init(4,10);    //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	   


	while(1);
	
	
}






u16 uAdc_Get_Time_No=0;//ADC��������
u16	uBattery_Adc=0;
u16	uBattery_Adc_B=0;//֮ǰ

extern u8 KeyPress;

unsigned short UI_Show_No_B_Watch_Task=0xff;//

//��������



u8	uCharge_Test_Delay=2;//�������ʱ��������һ�������߼���



#define NTC_No_TIME_INTERVAL					    11 //�ɼ�����
u16 uData_Ntc_Buf[NTC_No_TIME_INTERVAL]={0};//N��
u16 uData_Ntc_No=0;//����
u8	j_Adc,i_Adc;

u16 uData_Ntc_B;




void user_watch_task(void *pdata)
{

	    GPIO_InitTypeDef  GPIO_InitStructure;  		  

	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTCʱ��

	
			//��ʼ�� ����	  ��������
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA11,GPIOA13

CMachine.fCharge=0;//���
	
	
	
	
	
	
	
	
	while(1)
	{			  
		
		if(uCharge_Test_Delay>0)	uCharge_Test_Delay--;
		if(uCharge_Test_Delay==0)
		{
		if(Bit_RESET==GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))		CMachine.fCharge=0;
		else	
		{
			CMachine.fCharge=1;//���
				if(CMachine.UI_Show_No==0)//�ػ�״̬
				{
					//		SystemInit();	//ϵͳ��ֹͣģʽ���Ѻ������³�ʼ��ʱ��//�Ӵ������ã���Ϊ�൱��λ
					CMachine.UI_Show_No=1;//	
					

				}

		}
	}
		
		if(CMachine.UI_Show_No<1&&CMachine.fCharge==0)
		{
			
			if(UI_Show_No_B_Watch_Task!=0)
			{
			

				


					//PC4 ��Ϊ��������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
				
				
					//PD6 ��Ϊ��������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);					
				
			UI_Show_No_B_Watch_Task=0;
				
				
				

				
				
			}
			
						 		OSTimeDlyHMSM(0,0,0,100);
				//	if(KeyPress==0&&uCharge_Test_Delay==0)	SLEEP_WKUP_Init();//����ֹͣģʽ

					continue;//�˳�
			
			
		}else
		{
		
			
			if(UI_Show_No_B_Watch_Task==0)
			{
			
				

					
					//PC4 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
				

				
							
			UI_Show_No_B_Watch_Task=0xff;
			}
						
		
		
		
		}
		
		
		
		
		
		if((CMachine.fLED_Back!=CMachine.fLED_Back_B)||(CMachine.uLED_Back_No!=CMachine.uLED_Back_No_B))
		{
		
		
		if(CMachine.fLED_Back==1)
		{
			if(CMachine.uLED_Back_No==0)	LCD_PWM_Change(370);
			else LCD_PWM_Change(50);
		
		}else	LCD_PWM_Change(0);
			
	

			
			CMachine.uLED_Back_No_B=CMachine.uLED_Back_No;
			CMachine.fLED_Back_B=CMachine.fLED_Back;	
		}
		


		
		

		
/*		
			if(uAdc_Get_Time_No==0)	
			{
			
				
				
			//3.3--1038	    4.0--1222  

			uBattery_Adc=Get_Adc_Average(14,5);   //��ʼ���ɣϷ�ֹ��Ϊ���ģÿ�
				
	if(uBattery_Adc>1242) CMachine.Battery=100;	
else	if(uBattery_Adc>1186) CMachine.Battery=81;
else	if(uBattery_Adc>1150) CMachine.Battery=61;
else	if(uBattery_Adc>1114) CMachine.Battery=41;				
else	if(uBattery_Adc>1100) CMachine.Battery=21;
else	if(uBattery_Adc>1075)		 CMachine.Battery=9;//С��10%����˸
else		 CMachine.Battery=0;			
				
				

				
				
			}
		*/
			
								//PD6 ��Ϊ�������                         
					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
					GPIO_Init(GPIOD, &GPIO_InitStructure);		
					GPIO_ResetBits(GPIOD,GPIO_Pin_6); //����ͣ���ʼ���ѹ
					
					 		
							OSTimeDlyHMSM(0,0,0,10);
							
		uData_Ntc_Buf[uAdc_Get_Time_No]=Get_Adc_Average(14,5);   //��ʼ���ɣϷ�ֹ��Ϊ���ģÿ�
			
			
								//PD6 ��Ϊ��������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);				
	
	
	
		uAdc_Get_Time_No++;
		if(uAdc_Get_Time_No>11)	
		{
			uAdc_Get_Time_No=0;//һ����	

			
			
				//ð�ݷ�
		for(j_Adc=0;j_Adc<(NTC_No_TIME_INTERVAL-1);j_Adc++)	
		{
			for(i_Adc=0;i_Adc<(NTC_No_TIME_INTERVAL-1)-j_Adc;i_Adc++)
			{
				if(uData_Ntc_Buf[i_Adc]>uData_Ntc_Buf[i_Adc+1])
				{
					uData_Ntc_B=uData_Ntc_Buf[i_Adc];
					uData_Ntc_Buf[i_Adc]=uData_Ntc_Buf[i_Adc+1];
					uData_Ntc_Buf[i_Adc+1]=uData_Ntc_B;
				}
			}
		}
	uBattery_Adc=uData_Ntc_Buf[NTC_No_TIME_INTERVAL/2];//�����м�ֵ
				
		
		
		if(uBattery_Adc>uBattery_Adc_B)
		{
			if((uBattery_Adc-uBattery_Adc_B)>80)	uBattery_Adc_B=uBattery_Adc;
		}else
		{
			if((uBattery_Adc_B-uBattery_Adc)>80)	uBattery_Adc_B=uBattery_Adc;
		}
		
		

					//3.0--2500	    4.0--3340  
	if(uBattery_Adc_B>3340) CMachine.Battery=100;	
else	if(uBattery_Adc_B>3172) CMachine.Battery=81;
else	if(uBattery_Adc_B>3004) CMachine.Battery=61;
else	if(uBattery_Adc_B>2836) CMachine.Battery=41;				
else	if(uBattery_Adc_B>2668) CMachine.Battery=21;
else	if(uBattery_Adc_B>2500)		 CMachine.Battery=9;//С��10%����˸
else		 CMachine.Battery=0;						
			
		

		}
		
		
		
		
		
		
		
		

 		delay_ms(10);
	}
}












































