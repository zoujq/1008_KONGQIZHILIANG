#ifndef __ALL_SENSOR_TASK_H__
#define __ALL_SENSOR_TASK_H__




#define TVOC_POWER_ON     GPIO_ResetBits(GPIOB,GPIO_Pin_15);		//��TVOC��ѹ
#define TVOC_POWER_OFF    GPIO_SetBits(GPIOB,GPIO_Pin_15);			//��TVOC��ѹ



void User_All_Sensor_Task(void *pdata);//���д������������




#endif
