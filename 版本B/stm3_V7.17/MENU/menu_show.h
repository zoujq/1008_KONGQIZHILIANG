#ifndef __MENU_SHOW_H__
#define __MENU_SHOW_H__



//#define LCD_BACK_ON			GPIO_SetBits(GPIOC,GPIO_Pin_7);
//#define LCD_BACK_OFF		GPIO_ResetBits(GPIOC,GPIO_Pin_7);



void User_Public_Show(void);//公共显示部分
void User_Menu_Show_Task(void *pdata);//菜单图标显示任务

void User_ID_Show(void);//ID显示
void User_MAC_Show(void);//MAC显示


#endif
