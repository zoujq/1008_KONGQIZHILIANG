#ifndef __USER_WIFI_CNTRL_H__
#define __USER_WIFI_CNTRL_H__



#define POWER_WIFI_RST_LOW		{GPIO_ResetBits(GPIOE,GPIO_Pin_2);		}
#define POWER_WIFI_RST_HIGH		{	GPIO_SetBits(GPIOE,GPIO_Pin_2);		}

#define POWER_WIFI_ON		{		  	GPIO_ResetBits(GPIOA,GPIO_Pin_12);      			}
#define POWER_WIFI_OFF		{		GPIO_SetBits(GPIOA,GPIO_Pin_12);   		}

#define	WIFI_NRELOAD_LOW 	{GPIO_ResetBits(GPIOE,GPIO_Pin_3);			}
#define	WIFI_NRELOAD_HIGH {	GPIO_SetBits(GPIOE,GPIO_Pin_3);				}




void User_WIFI_Task(void *pdata);//WIFIÈÎÎñ



#endif
