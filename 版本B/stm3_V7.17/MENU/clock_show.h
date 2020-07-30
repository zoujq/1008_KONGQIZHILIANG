#ifndef __CLOCK_SHOW_H__
#define __CLOCK_SHOW_H__


extern	u16 uRang_Times_30S;//闹铃,30秒闹铃，每3分钟一次，连续3次

extern u8 f_Rang_Run;//闹钟使能标志位

	extern u16	uMode_Old;//之前的菜单，判断表盘刷新用

	extern u8 uRang_Time[2];//闹铃时间

	extern u8 f_User_Sleep;//用户睡眠标志、0不睡眠1睡眠
	
	
	
	

void User_Clock_Init(void);//RTC初始化
void User_Clock_Show_Task(void *pdata);//时钟显示任务



void User_Music_Set_Into(void);//进入音乐播放
void User_Music_Set_Exit(void);//退出音乐播放
void User_Music_Set_Bit_Next(void);//音乐播放,下一个按键
void User_Music_Set_Bit_Before(void);//音乐播放,上一个按键
void User_Music_Set_Bit_In(void);//音乐播放，按键选中


void User_Step_Into(void);//进入计步界面
void User_Step_Show(void);//显示步数界面
void User_Step_Exit(void);//退出计步界面
void User_Step_Clear(void);//计步清零

void User_Distance_Exit(void);//退出米距界面
void User_Distance_Show(void);//显示米距界面
void User_Distance_Into(void);//进入米距界面

void User_Search_Phone_Into(void);//进入查找手机界面
void User_Search_Phone_Able(void);//选择查找手机或关闭手机界面
void User_Search_Phone_Show(void);//显示查找手机动态界面
void User_Calories_Exit(void);//退出查找手机界面



//void User_Calories_Exit(void);//退出卡路里界面
//void User_Calories_Show(void);//显示卡路里界面
//void User_Calories_Into(void);//进入卡路里界面


void User_Clock_Set_Into(void);//进入时间设置
void User_Clock_Set_Exit(void);//退出时间设置
void User_Clock_Set_Exit_NotSave(void);//退出时间设置，不保存
void User_Clock_Set_Bit_Next(void);//时间设置,下一位
void User_Clock_Set_Bit_Before(void);//时间设置,上一位
void User_Clock_Set_Bit_Add(void);//时间设置,位加

void User_Rang_Set_Into(void);//进入闹钟设置
void User_Rang_Set_Exit(void);//退出闹钟设置
void User_Rang_Set_Exit_NotSave(void);//退出闹钟设置，不保存
void User_Rang_Set_Bit_Next(void);//闹钟设置,下一位
void User_Rang_Set_Bit_Before(void);//闹钟设置,上一位
void User_Rang_Set_Bit_Add(void);//闹钟设置,位加



void User_Rang_Pause(void);//闹铃暂停

void User_Rang_Stop(void);//闹铃停止






void User_Sleep_Test_Into(void);//进入睡眠检测界面
void User_Sleep_Test_Change(void);//睡眠检测界面切换
void User_Sleep_Test_Exit(void);//退出睡眠检测界面
	










#endif
