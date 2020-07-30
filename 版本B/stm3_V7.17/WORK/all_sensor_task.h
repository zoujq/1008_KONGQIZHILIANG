#ifndef __ALL_SENSOR_TASK_H__
#define __ALL_SENSOR_TASK_H__




#define TVOC_POWER_ON     GPIO_ResetBits(GPIOB,GPIO_Pin_15);		//开TVOC电压
#define TVOC_POWER_OFF    GPIO_SetBits(GPIOB,GPIO_Pin_15);			//关TVOC电压



void User_All_Sensor_Task(void *pdata);//所有传感器检测任务




#endif
