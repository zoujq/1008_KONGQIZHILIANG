#ifndef __MENU_SHOW_H__
#define __MENU_SHOW_H__



//#define LCD_BACK_ON			GPIO_SetBits(GPIOC,GPIO_Pin_7);
//#define LCD_BACK_OFF		GPIO_ResetBits(GPIOC,GPIO_Pin_7);



void User_Public_Show(void);//������ʾ����
void User_Menu_Show_Task(void *pdata);//�˵�ͼ����ʾ����

void User_ID_Show(void);//ID��ʾ
void User_MAC_Show(void);//MAC��ʾ


#endif
