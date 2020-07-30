#include "settings.h"
#include "rtc.h" 	   
#include "calendar.h" 	      						  
#include "sramlcd.h"
#include "spb.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-设置 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

_system_setings systemset;	  
#define SYS_MENU_SIZE 			19		//条目个数	
	 
//系统设置主目录表
const u8* sysset_mmenu_tbl[GUI_LANGUAGE_NUM][SYS_MENU_SIZE]=//系统一级目录的个数
{
	{
	"1.时间设置",
	"2.日期设置",
	"3.闹钟时间设置",
	"4.闹钟开关设置",
	"5.闹钟铃声设置",
	"6.语言设置",
	"7.数码相框设置",
	"8.MP3播放模式设置",
	"9.MP3音效设置",
	"10.FM发射开关设置",
	"11.FM发射频率设置",
	"12.FM收音设置",
	"13.背光设置",
 	"14.屏幕校准",
	"15.传感器校准",
	"16.系统文件更新",
	"17.系统信息",
	"18.系统状态", 
	"19.关于"
	},
	{			   								
	"1.時間設置",
	"2.日期設置",
	"3.鬧鐘時間設置",
	"4.鬧鐘開關設置",
	"5.鬧鐘鈴聲設置",
	"6.語言設置",
	"7.數碼相框設置",
	"8.MP3播放模式設置",
	"9.MP3音效設置",
	"10.FM發射開關設置",
	"11.FM發射頻率設置",
	"12.FM收音設置",
	"13.背光設置",
 	"14.屏幕校準",
	"15.傳感器校準",
	"16.系統文件更新",
	"17.系統信息",
	"18.系統狀態", 
	"19.關於"
	}, 	 
	{
	"1.TIME SET",
	"2.DATE SET",
	"3.ALARM TIME SET",
	"4.ALARM ON/OFF SET",
	"5.ALARM RING SET",
	"6.LANGUAGE SET",
	"7.DIGITAL PHOTO FRAME SET",
	"8.MP3 MODE SET",
	"9.MP3 EFFICT SET",
	"10.FM SEND ON/OFF SET",
	"11.FM SEND FREQ SET",
	"12.FM RADIO SET",
	"13.BACKLIGHT SET",
 	"14.TOUCH SCREEN ADJUST",
	"15.3D ADJUST",
	"16.SYSTEM FILE UPDATE",
	"17.SYSTEM INFORMATION",
	"18.SYSTEM STATUS", 
	"19.ABOUT"
	},
};	    
//语言设置列表
const u8* sysset_language_tbl[GUI_LANGUAGE_NUM]=
{
	"简体中文","繁體中文","English",
};  	    
//铃声设置列表
const u8* sysset_ring_tbl[GUI_LANGUAGE_NUM][4]=
{
{"铃声1","铃声2","铃声3","铃声4",},
{"鈴聲1","鈴聲2","鈴聲3","鈴聲4",},
{"RING 1","RING 2","RING 3","RING 4",},	 
};
//播放模式设置列表
const u8* sysset_mp3mode_tbl[GUI_LANGUAGE_NUM][3]=
{
{"全部循环","随机播放","单曲循环"},
{"全部循環","隨機播放","單曲循環"},
{"LOOP PLAY","SHUFFLE PLAY","SINGLE PLAY"},
};
//MP3音效
const u8* sysset_mp3effect_tbl[6][GUI_LANGUAGE_NUM]=
{
{"主音量:","主音量:","Volume:",},
{"低频上限:","低頻上限:","Bass Freq:",},
{"低频提升:","低頻提升:","Bass:",},
{"高频下限:","高頻下限:","Treble Freq:",},
{"高频提升:","高頻提升:","Treble:",},
{"空间效果:","空間效果:","Ear Effect:"},
};
//MP3耳机空间效果
const u8* sysset_eareffect_tbl[4][GUI_LANGUAGE_NUM]=
{
{"  关","  關"," OFF",},
{"  弱","  弱"," WEEK",},
{"  中","  中","MIDDLE",},
{"  强","  強","STRONG",},
};

//FM发射开关设置列表
const u8* sysset_fmsend_onoff_tbl[GUI_LANGUAGE_NUM][2]=
{
{"FM发射关闭","FM发射开启",},
{"FM發射關閉","FM發射開啟",},
{"FM SEND OFF","FM SEND ON",},
};  	    

//FM收音设置列表
const u8* sysset_fm_radio_tbl[GUI_LANGUAGE_NUM][2]=
{
{"FM后台收音关闭","FM后台收音开启",},
{"FM後臺收音關閉","FM後臺收音開啟",},
{"FM BACKPLAY OFF","FM BACKPLAY ON",},
};
//系统更新相关提示信息
//系统更新确认提示 
const u8 *sysset_system_update_remaindmsg_tbl[GUI_LANGUAGE_NUM]=
{"您确认更新系统文件?","您確認更新系統文件?","Are you sure to update?"};
//系统更新复制提示信息
const u8 *sysset_system_update_cpymsg_tbl[2][GUI_LANGUAGE_NUM]=
{
{"正在复制:","正在複製:"," Copying:",},
{"当前文件夹:","當前文件夾:","Cur Folder:",},
}; 
//系统更新提示
const u8 *sysset_system_update_msg_tbl[GUI_LANGUAGE_NUM]=
{"系统正在更新...","系統正在更新...","SYSTEM Updating..."};
//系统更新结果提示
const u8 *sysset_system_update_err_tbl[3][GUI_LANGUAGE_NUM]=
{
{"系统文件更新成功!","系統文件更新成功!","SYSTEM file lost!",},
{"系统文件丢失!","系統文件丟失!","SYSTEM file lost!",},
{"用户终止更新!","用戶終止更新!","User stop update!",},
};
//系统信息标注表
const u8* sysset_system_info_caption_tbl[9][GUI_LANGUAGE_NUM]=
{
{"处理器:","處理器:","CPU:"},
{"内存:","內存:","RAM:"},
{"SD卡:","SD卡:","SD Card:"},
{"FLASH盘:","FLASH盤:","FLASH Disk:"},
{"操作系统:","操作系統:","OS:"},
{"图形界面:","圖形界面:","GUI:"},
{"硬件平台:","硬件平臺:","Hardware:"},
{"版权信息:","版權信息:","Copyright:"},
{"技术支持:","技術支持:","Tech Support:"},
};
//系统提示信息表
const u8* sysset_system_info_msg_tbl[9]=
{
"STM32F103ZET6 @ 72Mhz",
"64KB+1024KB",
"MB",
"KB",
"uCOS II ",
"ALIENTEK GUI ",
"ALIENTEK WarShip ",
"广州星翼电子",
"www.openedv.com",
}; 
//系统状提示信息
const u8* sysset_sysstatus_tbl[4][GUI_LANGUAGE_NUM]=
{
{"CPU使用率:","CPU使用率:","CPU USAGE:",},
{"内部内存使用率:","内部内存使用率:","IN MEMORY USAGE:",},
{"外部内存使用率:","外部内存使用率:","EX MEMORY USAGE:"},
{"温度:","溫度:","TEMP:"},
};

//系统关于提示信息
const u8* sysset_system_about_caption_tbl[2][GUI_LANGUAGE_NUM]=
{
{"ALEINTEK 战舰","ALIENTEK 戰艦","ALENTEK WarShip",},
{"产品序列号","產品序列號","Serial Number",},
};




//语言设置后的提示
const u8* sysset_remindmsg_tbl[2][GUI_LANGUAGE_NUM]=
{
{"更新主界面","更新主界面","Updating",},
{"系统正在更新主界面,请稍候...","系統正在更新主界面,請稍候...","System updating,Please wait...",}, 
};
//传感器校准提示语
const u8* sysset_sensoradjust_tbl[GUI_LANGUAGE_NUM]=
{
"传感器校准中...,请稍候...","傳感器校準中...,請稍候...","Sensor adjusting,Please wait...",	 
};

////////////////////////////////////////////////////////////////////////////////////////////

#define SYSSET_BTN1_WIDTH			50			//一类按键宽度(加减按钮)
#define SYSSET_BTN1_HEIGHT			40			//一类按键高度(加减按钮)
#define SYSSET_BTN2_WIDTH			60			//二类按键宽度(确认/取消按钮)
#define SYSSET_BTN2_HEIGHT			30			//二类按键高度(确认/取消按钮)
#define SYSSET_BTN3_WIDTH			100			//三类按键宽度(单个按钮的宽度)
#define SYSSET_EDIT_WIDTH			50			//文字编辑处的宽度
#define SYSSET_EDIT_HEIGHT			25			//文字编辑处的高度
 
#define SYSSET_INWIN_FONT_COLOR		0X736C		//0XAD53		//窗体内嵌字颜色
#define SYSSET_EDIT_BACK_COLOR		0XE79E		//显示内容区背景色
 
/////////////////////////////////////////////////////////////////////////////////////////////

//时间/闹钟设置
//x,y:窗口坐标(窗口尺寸已经固定了的)	 
//hour,min:时分
//caption:窗口名字				  
//返回值:0,ok;其他,取消或者错误.
u8 sysset_time_set(u16 x,u16 y,u8 *hour,u8 *min,u8*caption) 
{
	u8 rval=0,res;
	u8 i;
	_window_obj* twin=0;	//窗体
 	_btn_obj * tbtn[6];		//总共六个按钮:0,时钟加按钮;1,时钟减按钮;2,分钟加按钮;3,分钟减按钮;4,确认按钮;5,取消按钮.		  
 	twin=window_creat(x,y,150,199,0,1|1<<5,16);//创建窗口
 	tbtn[0]=btn_creat(x+20,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);			//创建按钮
	tbtn[1]=btn_creat(x+20,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[2]=btn_creat(x+20+60,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[3]=btn_creat(x+20+60,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[4]=btn_creat(x+10,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);		//创建按钮
	tbtn[5]=btn_creat(x+10+70,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);	//创建按钮
	for(i=0;i<6;i++)
	{
		if(tbtn[i]==NULL)
		{
			rval=1;
			break;
		}
		if(i<4)//加减按键
		{
			tbtn[i]->bcfucolor=BLACK;//松开时为黑色
			tbtn[i]->bcfdcolor=WHITE;//按下时为白色			
			tbtn[i]->bkctbl[0]=0X453A;//边框颜色
			tbtn[i]->bkctbl[1]=0X5DDC;//第一行的颜色				
			tbtn[i]->bkctbl[2]=0X5DDC;//上半部分颜色
			tbtn[i]->bkctbl[3]=0X453A;//下半部分颜色
		}else//确认和取消按键
		{
			tbtn[i]->bkctbl[0]=0X8452;//边框颜色
			tbtn[i]->bkctbl[1]=0XAD97;//第一行的颜色				
			tbtn[i]->bkctbl[2]=0XAD97;//上半部分颜色
			tbtn[i]->bkctbl[3]=0X8452;//下半部分颜色
		}
		if(i==0||i==2)tbtn[i]->caption="＋";
		if(i==1||i==3)tbtn[i]->caption="－";
		if(i==4)tbtn[i]->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];
		if(i==5)tbtn[i]->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];				
	}
    if(twin==NULL)rval=1;
	else
	{
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	     
	}
	if(rval==0)//无错误
	{
		window_draw(twin);					//画出窗体
		for(i=0;i<6;i++)btn_draw(tbtn[i]);	//画按钮
		gui_fill_rectangle(x+20+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);  		//填充时钟背景
		gui_fill_rectangle(x+20+60+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);	//填充分钟背景
 		app_show_nummid(x+20,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*hour,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		app_show_nummid(x+20+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*min,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
 			if(system_task_return)
			{
				rval=1;			//取消设置
				break;			//TPAD返回	
			}
			for(i=0;i<6;i++)
			{
				res=btn_check(tbtn[i],&in_obj);//确认按钮检测
				if(res)
				{
					if((tbtn[i]->sta&0X80)==0)//有有效操作
					{
						switch(i)
						{
							case 0://时钟增加按钮按下了
								(*hour)++;
								if(*hour>23)*hour=0;
								break;
							case 1://时钟减少按钮按下了	  
								if(*hour)(*hour)--;
								else *hour=23;
								break;
							case 2://分钟增加按钮按下了
								(*min)++;
								if(*min>59)(*min)=0;
								break;
							case 3://分钟减少按钮按下了	  
								if(*min)(*min)--;
								else *min=59;
								break;
							case 4://确认按钮按下  
								rval=0XFF;
								break;	  
							case 5://取消按钮按下	  
								rval=1;
								break;
 						}
					}
					app_show_nummid(x+20,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*hour,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
					app_show_nummid(x+20+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*min,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  

				}
			}
			
		}
 	}
	window_delete(twin);				//删除窗口
	for(i=0;i<6;i++)btn_delete(tbtn[i]);//删除按钮					   
	system_task_return=0;				//清除退出标志
	if(rval==0XFF)return 0;
	return rval;
}
//日期设置
//x,y:窗口坐标(窗口尺寸已经固定了的)
//year,month,date:年月日
//caption:窗口名字
//返回值:0,ok;其他,取消或者错误.
u8 sysset_date_set(u16 x,u16 y,u16 *year,u8 *month,u8 *date,u8*caption) 
{
	u8 rval=0,res;
	u8 i;
	u8 maxdate=31;			//2月份最大的天数
	_window_obj* twin=0;	//窗体
 	_btn_obj * tbtn[8];		//总共八个按钮:0,年份加按钮;1,年份减按钮;2,月份加按钮;3月份减按钮;4,日期加按钮;5,日期减按钮;6,确认按钮;7,取消按钮		  
 	twin=window_creat(x,y,190,199,0,1|1<<5,16);//创建窗口
 	tbtn[0]=btn_creat(x+10,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);			//创建按钮
	tbtn[1]=btn_creat(x+10,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[2]=btn_creat(x+10+60,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[3]=btn_creat(x+10+60,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[4]=btn_creat(x+10+120,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[5]=btn_creat(x+10+120,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);	//创建按钮

	tbtn[6]=btn_creat(x+20,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);		//创建按钮
	tbtn[7]=btn_creat(x+20+90,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);	//创建按钮
 	for(i=0;i<8;i++)
	{
		if(tbtn[i]==NULL)
		{
			rval=1;
			break;
		}
		if(i<6)//加减按键
		{
			tbtn[i]->bcfucolor=BLACK;//松开时为黑色
			tbtn[i]->bcfdcolor=WHITE;//按下时为白色			
			tbtn[i]->bkctbl[0]=0X453A;//边框颜色
			tbtn[i]->bkctbl[1]=0X5DDC;//第一行的颜色				
			tbtn[i]->bkctbl[2]=0X5DDC;//上半部分颜色
			tbtn[i]->bkctbl[3]=0X453A;//下半部分颜色
		}else//确认和取消按键
		{
			tbtn[i]->bkctbl[0]=0X8452;//边框颜色
			tbtn[i]->bkctbl[1]=0XAD97;//第一行的颜色				
			tbtn[i]->bkctbl[2]=0XAD97;//上半部分颜色
			tbtn[i]->bkctbl[3]=0X8452;//下半部分颜色
		}
		if(i==0||i==2||i==4)tbtn[i]->caption="＋";
		if(i==1||i==3||i==5)tbtn[i]->caption="－";
		if(i==6)tbtn[i]->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];;
		if(i==7)tbtn[i]->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];;				
	}
    if(twin==NULL)rval=1;
	else
	{
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	     
	}
	if(rval==0)//无错误
	{
		window_draw(twin);					//画出窗体
		for(i=0;i<8;i++)btn_draw(tbtn[i]);	//画按钮
		gui_fill_rectangle(x+10+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);  		//填充年份背景
		gui_fill_rectangle(x+10+60+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);	//填充月份背景
 		gui_fill_rectangle(x+10+120+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);	//填充日期背景

 		app_show_nummid(x+10,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*year,4,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		app_show_nummid(x+10+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*month,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
		app_show_nummid(x+10+120,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*date,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
 			if(system_task_return)
			{
				rval=1;			//取消设置
				break;			//TPAD返回	
			}
			for(i=0;i<8;i++)
			{
				res=btn_check(tbtn[i],&in_obj);//确认按钮检测
				if(res)
				{
					if((tbtn[i]->sta&0X80)==0)//有有效操作
					{
						if(*month==2)
						{
							if(Is_Leap_Year(*year))maxdate=29;//是闰年的2月份
	 						else maxdate=28;	  
						}else maxdate=31;  
						switch(i)
						{
							case 0://年份增加按钮按下了
								(*year)++;
								if(*year>2100)*year=2000;
								break;
							case 1://年份减少按钮按下了	  
								if(*year>2000)(*year)--;
								else *year=2100;
								break;
							case 2://月份增加按钮按下了
								(*month)++;
								if(*month>12)(*month)=1;
								break;
							case 3://月份减少按钮按下了	  
								if(*month>1)(*month)--;
								else *month=12;
								break;
							case 4://日期增加按钮按下了
								(*date)++;
								if(*date>maxdate)(*date)=1;
								break;
							case 5://日期减少按钮按下了	  
								if(*date>1)(*date)--;
								else *date=maxdate;
								break;
							case 6://确认按钮按下  
								rval=0XFF;
								break;	  
							case 7://取消按钮按下  
								rval=1;
								break;
 						}
					}   
			 		app_show_nummid(x+10,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*year,4,16,BLACK,SYSSET_EDIT_BACK_COLOR);
					app_show_nummid(x+10+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*month,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
					app_show_nummid(x+10+120,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*date,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
				}
			}
			
		}
 	}
	window_delete(twin);				//删除窗口
	for(i=0;i<8;i++)btn_delete(tbtn[i]);//删除按钮					   
	system_task_return=0;				//清除退出标志
	if(rval==0XFF)return 0;
	return rval;
}	  
		   
//发射频率设置
//x,y:窗口坐标(窗口尺寸已经固定了的,150*199)	   
//freq:频率,单位0.1Mhz
//caption:窗口名字				  
//返回值:0,ok;其他,取消或者错误.
u8 sysset_fmtx_freq_set(u16 x,u16 y,u16 *freq,u8*caption) 
{
	u8 rval=0,res;
	u8 i;
	_window_obj* twin=0;	//窗体
 	_btn_obj * tbtn[4];		//总共4个按钮:0,频率增加按钮;1,频率减少按钮;2,确认按钮;3,取消按钮			  
 	twin=window_creat(x,y,150,199,0,1|1<<5,16);//创建窗口
 	tbtn[0]=btn_creat(x+25,y+42,SYSSET_BTN3_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);			//创建按钮
	tbtn[1]=btn_creat(x+25,y+42+67,SYSSET_BTN3_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//创建按钮
	tbtn[2]=btn_creat(x+10,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);		//创建按钮
	tbtn[3]=btn_creat(x+10+70,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);	//创建按钮
	for(i=0;i<4;i++)
	{
		if(tbtn[i]==NULL)
		{
			rval=1;
			break;
		}
		if(i<2)//加减按键
		{
			tbtn[i]->bcfucolor=BLACK;//松开时为黑色
			tbtn[i]->bcfdcolor=WHITE;//按下时为白色			
			tbtn[i]->bkctbl[0]=0X453A;//边框颜色
			tbtn[i]->bkctbl[1]=0X5DDC;//第一行的颜色				
			tbtn[i]->bkctbl[2]=0X5DDC;//上半部分颜色
			tbtn[i]->bkctbl[3]=0X453A;//下半部分颜色
		}else//确认和取消按键
		{
			tbtn[i]->bkctbl[0]=0X8452;//边框颜色
			tbtn[i]->bkctbl[1]=0XAD97;//第一行的颜色				
			tbtn[i]->bkctbl[2]=0XAD97;//上半部分颜色
			tbtn[i]->bkctbl[3]=0X8452;//下半部分颜色
		}
		if(i==0)tbtn[i]->caption="＋";
		if(i==1)tbtn[i]->caption="－";
		if(i==2)tbtn[i]->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];
		if(i==3)tbtn[i]->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];				
	}
    if(twin==NULL)rval=1;
	else
	{
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	     
	}
	if(rval==0)//无错误
	{
		window_draw(twin);					//画出窗体
		for(i=0;i<4;i++)btn_draw(tbtn[i]);	//画按钮
   		gui_fill_rectangle(x+25+1,y+42+SYSSET_BTN1_HEIGHT+1,SYSSET_BTN3_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);//填充FM频率背景
		app_show_float(x+25+56,y+42+SYSSET_BTN1_HEIGHT+4,*freq,1,5,16,BLACK,SYSSET_EDIT_BACK_COLOR);	//显示频率
		gui_show_string("Mhz",x+25+57,y+42+SYSSET_BTN1_HEIGHT+4,24,16,16,BLACK);	//显示Mhz
 		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
 			if(system_task_return)
			{
				rval=1;			//取消设置
				break;			//TPAD返回	
			}
			for(i=0;i<4;i++)
			{
				res=btn_check(tbtn[i],&in_obj);//确认按钮检测
				if(res)
				{
					if((tbtn[i]->sta&0X80)==0)//有有效操作
					{
						switch(i)
						{
							case 0://时钟增加按钮按下了
								(*freq)++;
								if((*freq)>1080)*freq=870;
								break;
							case 1://时钟减少按钮按下了	  
								if((*freq)>870)(*freq)--;
								else *freq=1080;
								break;
							case 2://确认按钮按下  
								rval=0XFF;
								break;	  
							case 3://取消按钮按下	  
								rval=1;
								break;
 						}
					}
 					app_show_float(x+25+56,y+42+SYSSET_BTN1_HEIGHT+4,*freq,1,5,16,BLACK,SYSSET_EDIT_BACK_COLOR);//显示频率
 				}
			}
			
		}
 	}
	window_delete(twin);				//删除窗口
	for(i=0;i<4;i++)btn_delete(tbtn[i]);//删除按钮					   
	system_task_return=0;				//清除退出标志
	if(rval==0XFF)return 0;
	return rval;
}	  

//MP3音效显示
//x,y:sysset_mp3_effict_set的设置坐标
//mask:
//[0]:音量;0~100%
//[1]:低频上限;20~150Hz
//[2]:低频提升;0~15dB
//[3]:高频下限;1~15Khz
//[4]:高频提升;-12.0~10.5dB
//[5]:空间效果;关/弱/中/强
//[7]:更新单位
//显示的坐标
void sysset_mp3_effict_show(u16 x,u16 y,u8 mask,u8 vsval)
{
	signed char tdbval=0;
   	if(mask&(1<<7))
	{
		gui_show_string("%",x+200-10-6,y+32+5,20,12,12,SYSSET_INWIN_FONT_COLOR);		//显示各項的单位
		gui_show_string("Hz",x+200-10-12,y+32+5+35*1,20,12,12,SYSSET_INWIN_FONT_COLOR);	//显示各項的单位
 		gui_show_string("dB",x+200-10-12,y+32+5+35*2,20,12,12,SYSSET_INWIN_FONT_COLOR);	//显示各項的单位
		gui_show_string("KHz",x+200-10-18,y+32+5+35*3,20,12,12,SYSSET_INWIN_FONT_COLOR);//显示各項的单位
		gui_show_string("dB",x+200-10-12,y+32+5+35*4,20,12,12,SYSSET_INWIN_FONT_COLOR);	//显示各項的单位
 	}
	gui_phy.back_color=APP_WIN_BACK_COLOR;//设置背景色
	if(mask&(1<<0))//显示音量
	{						 
 		gui_fill_rectangle(x+200-10-24,y+32+5,18,12,APP_WIN_BACK_COLOR);//清空之前的显示
		if(vsval>=100)
		{
			vsval=100;
			gui_show_num(x+200-10-24,y+32+5,3,SYSSET_INWIN_FONT_COLOR,12,vsval,0);//显示音量百分比 
		}else 
		{
			gui_show_num(x+200-10-18,y+32+5,2,SYSSET_INWIN_FONT_COLOR,12,vsval,0);//显示音量百分比   
 	  	}
 	}
	if(mask&(1<<1))//显示低频上限
	{
		vsval=vsval*10+20;
		gui_fill_rectangle(x+200-10-30,y+32+5+35*1,18,12,APP_WIN_BACK_COLOR);//清空之前的显示
		gui_show_num(x+200-10-30,y+32+5+35*1,3,SYSSET_INWIN_FONT_COLOR,12,vsval,0);//显示数值 
	}
	if(mask&(1<<2))//显示低频提升
	{
		vsval=vsval;
		gui_fill_rectangle(x+200-10-24,y+32+5+35*2,12,12,APP_WIN_BACK_COLOR);//清空之前的显示
		gui_show_num(x+200-10-24,y+32+5+35*2,2,SYSSET_INWIN_FONT_COLOR,12,vsval,0);//显示数值  
	}
	if(mask&(1<<3))//显示高频下限
	{
		vsval=vsval+1;
		gui_fill_rectangle(x+200-10-30,y+32+5+35*3,12,12,APP_WIN_BACK_COLOR);//清空之前的显示
		gui_show_num(x+200-10-30,y+32+5+35*3,2,SYSSET_INWIN_FONT_COLOR,12,vsval,0);//显示数值 
	}	
	if(mask&(1<<4))//显示高频提升
	{
 		tdbval=((signed char)vsval-8)*15;	   
 		app_show_float(x+200-10-12,y+32+5+35*4,tdbval,1,5,12,SYSSET_INWIN_FONT_COLOR,APP_WIN_BACK_COLOR);	//显示数值 
	}
	if(mask&(1<<5))//显示高频提升
	{
		gui_fill_rectangle(x+200-10-36,y+32+5+35*5,36,12,APP_WIN_BACK_COLOR);//清空之前的显示
		gui_show_string((u8*)sysset_eareffect_tbl[vsval][gui_phy.language],x+200-10-36,y+32+5+35*5,60,12,12,SYSSET_INWIN_FONT_COLOR);//显示
 	}
}
	 
//MP3音效设置
//x,y:显示的坐标
//宽度:200,高度:292.
//caption:名称
//vsset:vs1003的设置
//返回值:0,ok;其他,取消或者错误.
u8 sysset_mp3_effict_set(u16 x,u16 y,_vs10xx_obj *vsset,u8* caption)
{
 	u8 rval=0,res;
	u8 i=0;
 	_window_obj* twin=0;			//窗体
 	_btn_obj * rbtn=0;				//取消按钮		  
 	_btn_obj * okbtn=0;				//确定按钮		  
	_progressbar_obj * mp3prgb[6];	//主音量
	u8 voltemp[6];	 				//数据缓存		  

 	twin=window_creat(x,y,200,292,0,1|1<<5,16);					//创建窗口
 	for(i=0;i<6;i++)
	{
		mp3prgb[i]=progressbar_creat(x+10,y+32+20+35*i,180,15,0X60);		//创建进度条
		if(mp3prgb[i]==NULL)rval=1;
	}
	mp3prgb[0]->totallen=150;		//主音量长度150
 	mp3prgb[1]->totallen=13;		//低频上线(0~13)+2;
	mp3prgb[2]->totallen=15;		//低频提升:0~15
	mp3prgb[3]->totallen=14;		//高频下限:(0~14)+1
  	mp3prgb[4]->totallen=15;		//高频提升:0~15
   	mp3prgb[5]->totallen=3;			//空间效果:0~3
													 
  	okbtn=btn_creat(x+30,y+32+35*6+10,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);			//创建按钮
 	rbtn=btn_creat(x+30+60+20,y+32+35*6+10,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);	//创建按钮
	if(twin==NULL||rbtn==NULL||okbtn==NULL||rval)rval=1;
	else
	{
		//窗口的名字和背景色
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
		//返回按钮的颜色    
		rbtn->bkctbl[0]=0X8452;		//边框颜色
		rbtn->bkctbl[1]=0XAD97;		//第一行的颜色				
		rbtn->bkctbl[2]=0XAD97;		//上半部分颜色
		rbtn->bkctbl[3]=0X8452;		//下半部分颜色
		okbtn->bkctbl[0]=0X8452;	//边框颜色
		okbtn->bkctbl[1]=0XAD97;	//第一行的颜色				
		okbtn->bkctbl[2]=0XAD97;	//上半部分颜色
		okbtn->bkctbl[3]=0X8452;	//下半部分颜色

		rbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];	//标题为取消
 		okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];		//标题为返回

		if(vsset->mvol>100)mp3prgb[0]->curpos=vsset->mvol-100;//mvol:100~250
		else mp3prgb[0]->curpos=0;//小于100
		mp3prgb[1]->curpos=vsset->bflimit;
		mp3prgb[2]->curpos=vsset->bass;
		mp3prgb[3]->curpos=vsset->tflimit;
		mp3prgb[4]->curpos=vsset->treble;
		mp3prgb[5]->curpos=vsset->effect;			 
											 
 		window_draw(twin);				//画出窗体
		btn_draw(rbtn);					//画按钮
		btn_draw(okbtn);				//画按钮
		sysset_mp3_effict_show(x,y,1<<7,0);//显示单位 
		for(i=0;i<6;i++)
		{
			voltemp[i]=mp3prgb[i]->curpos;	//保存开始的状态
			mp3prgb[i]->btncolor=BLUE;		//蓝色
			mp3prgb[i]->prgbarlen=5;		//滚条长度设置为5
			progressbar_draw_progressbar(mp3prgb[i]);	//显示进度条
			gui_show_string((u8*)sysset_mp3effect_tbl[i][gui_phy.language],x+10,y+32+5+35*i,190,12,12,SYSSET_INWIN_FONT_COLOR);//显示各項的名字
  			if(i==0)sysset_mp3_effict_show(x,y,1<<0,(mp3prgb[i]->curpos*100)/mp3prgb[i]->totallen);//显示音量百分比
			else sysset_mp3_effict_show(x,y,1<<i,voltemp[i]);//显示所有数据	 
		}
	}
 	while(rval==0)
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
		delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
 		if(system_task_return)
		{
			rval=1;			//退出,不保存
			break;			//TPAD返回	
		}
		res=btn_check(rbtn,&in_obj);			//取消按钮检测
		if(res&&((rbtn->sta&0X80)==0))//有有效操作
		{
			rval=1;		  	//退出,不保存
			break;			//退出
		}
 		res=btn_check(okbtn,&in_obj);			//确定按钮检测
 		if(res&&((okbtn->sta&0X80)==0))			//有有效操作
		{
	 		vsset->mvol=voltemp[0];	   			//保存到vsset里面
	 		vsset->bflimit=voltemp[1];
	 		vsset->bass=voltemp[2];
	 		vsset->tflimit=voltemp[3];
	 		vsset->treble=voltemp[4];
	 		vsset->effect=voltemp[5];  
			rval=0XFF;		   
			break;//退出
		}   
		for(i=0;i<6;i++)
		{
			res=progressbar_check(mp3prgb[i],&in_obj);
 			if(res&&(voltemp[i]!=mp3prgb[i]->curpos))		//进度条改动了
			{
				voltemp[i]=mp3prgb[i]->curpos;				//保存最新的结果
				if(i==0)
				{
 					voltemp[i]=mp3prgb[i]->curpos+100;		//保存最新的结果
					VS_Set_Vol(voltemp[i]);					//设置主音量
 					sysset_mp3_effict_show(x,y,1<<0,(mp3prgb[i]->curpos*100)/mp3prgb[i]->totallen);//显示音量百分比
				}else
				{
					if(i==5)VS_Set_Effect(voltemp[i]);	//设置音效
					else VS_Set_Bass(voltemp[1],voltemp[2],voltemp[3],voltemp[4]);//设置音效  
	  				sysset_mp3_effict_show(x,y,1<<i,voltemp[i]);//更新数据
				}	 
			}
		}
			 
	}
	window_delete(twin);							//删除窗口
	btn_delete(rbtn);								//删除按钮	
	for(i=0;i<6;i++)progressbar_delete(mp3prgb[i]);	//删除进度条  
	system_task_return=0;							//清除退出标志
	if(rval==0XFF)return 0;
	return rval;
}
//背光亮度设置
//x,y:显示的坐标
//宽度:180,高度:180
//caption:名称
//vsset:vs1003的设置
//返回值:0,ok;其他,取消或者错误.
u8 sysset_bklight_set(u16 x,u16 y,u8* caption,u16 *bkval)
{
 	u8 rval=0,res;
  	_window_obj* twin=0;			//窗体
 	_btn_obj * rbtn=0;				//取消按钮		  
 	_btn_obj * okbtn=0;				//确定按钮		  
	_progressbar_obj *bkprgb;		//背光设置滚动条
 
 	twin=window_creat(x,y,180,150,0,1|1<<5,16);				//创建窗口
	bkprgb=progressbar_creat(x+15,y+32+35,150,20,0X20);		//创建进度条,不显示浮标
	if(bkprgb==NULL)rval=1;
 													 
  	okbtn=btn_creat(x+20,y+110,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);		//创建按钮
 	rbtn=btn_creat(x+20+60+20,y+110,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);	//创建按钮
	if(twin==NULL||rbtn==NULL||okbtn==NULL||rval)rval=1;
	else
	{
		//窗口的名字和背景色
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
		//返回按钮的颜色    
		rbtn->bkctbl[0]=0X8452;		//边框颜色
		rbtn->bkctbl[1]=0XAD97;		//第一行的颜色				
		rbtn->bkctbl[2]=0XAD97;		//上半部分颜色
		rbtn->bkctbl[3]=0X8452;		//下半部分颜色
		okbtn->bkctbl[0]=0X8452;	//边框颜色
		okbtn->bkctbl[1]=0XAD97;	//第一行的颜色				
		okbtn->bkctbl[2]=0XAD97;	//上半部分颜色
		okbtn->bkctbl[3]=0X8452;	//下半部分颜色

		rbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];	//标题为取消
 		okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];		//标题为返回
  		if(*bkval<10)*bkval=10;											//至少要有10个单位
		bkprgb->curpos=*bkval-10;										//取得当前的设置百分比
   		bkprgb->totallen=100;					//总长度为100
 											 
 		window_draw(twin);						//画出窗体
		btn_draw(rbtn);							//画按钮
		btn_draw(okbtn);						//画按钮			    
		progressbar_draw_progressbar(bkprgb);	//显示进度条
		gui_phy.back_color=APP_WIN_BACK_COLOR;	//设置背景色 
		gui_show_string("%",x+15+3*6,y+32+22,20,12,12,SYSSET_INWIN_FONT_COLOR);//显示各項的单位
 		gui_show_num(x+15,y+32+22,3,SYSSET_INWIN_FONT_COLOR,12,bkprgb->curpos,0);//显示数值 
 	}
 	while(rval==0)
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
		delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
		if(system_task_return)
		{
			rval=1;			//取消设置
			break;			//TPAD返回	
		}
		res=btn_check(rbtn,&in_obj);			//取消按钮检测
		if(res&&((rbtn->sta&0X80)==0))			//有有效操作
		{
			rval=1;
			break;//退出
		}
 		res=btn_check(okbtn,&in_obj);			//确定按钮检测
 		if(res&&((okbtn->sta&0X80)==0))			//有有效操作
		{		 
			rval=0XFF;		   
			break;//退出
		}    
		res=progressbar_check(bkprgb,&in_obj);
		if(res&&(*bkval!=bkprgb->curpos))		//进度条改动了
		{
 			gui_show_num(x+15,y+32+22,3,SYSSET_INWIN_FONT_COLOR,12,bkprgb->curpos,0);//显示数值 
			*bkval=bkprgb->curpos+10;			//保存最新的结果 
 			LCD_BLPWM_VAL=*bkval;			    //设置背光亮度 
		}   		 
	}
	window_delete(twin);						//删除窗口
	btn_delete(rbtn);							//删除按钮	
	progressbar_delete(bkprgb);					//删除进度条  
	system_task_return=0;				 		//清除退出标志
	if(rval==0XFF)return 0;
	return rval;
}

 //系统启动的时候,用于显示更新进度
//*pname:更新文件名字
//pct:百分比
//mode:模式
//[0]:更新文件名
//[1]:更新百分比pct
//[2]:更新文件夹
//[3~7]:保留
//返回值:0,正常;
//       1,结束复制			  
u8 sysset_system_update_cpymsg(u8*pname,u8 pct,u8 mode)
{		
	u16 filled;										 
	if(system_task_return)return 1;//TPAD返回	
	if(mode&0X01)//显示文件名
	{
 		gui_phy.back_color=APP_WIN_BACK_COLOR;
		gui_fill_rectangle(20+10+9*6,90+32+15,126,12,APP_WIN_BACK_COLOR);		//填充底色  0XE73C 
 		gui_show_string(pname,20+10+9*6,90+32+15,126,12,12,BLACK);				//显示新的文件名	
  
  		gui_show_string("当前文件夹:",20+10,90+32+15+21,66,12,12,BLACK);	
 	}
	if(mode&0X04)//显示文件夹名
	{
 		gui_phy.back_color=APP_WIN_BACK_COLOR;
		gui_fill_rectangle(20+10+11*6,90+32+15+21,114,12,APP_WIN_BACK_COLOR);	//填充底色  0XE73C 
 		gui_show_string(pname,20+10+11*6,90+32+15+21,114,12,12,BLACK);			//显示新的文件夹名	
	}
	if(mode&0X02)//更新百分比
	{
		if(pct>100)pct=100;
	    filled=pct;
		filled=(filled*9)/5;
		gui_fill_rectangle(20+10,90+32+15+21*2,filled,16,0X071F);				//填充占用了的百分比部分     
		gui_fill_rectangle(20+10+filled,90+32+15+21*2,180-filled,16,WHITE);		//填充未占用的百分比部分  
		gui_show_num(20+10+72,90+32+15+21*2+2,3,BLACK,12,pct,1);				//显示更新进度
		gui_show_string("%",20+10+72+18,90+32+15+21*2+2,6,12,12,BLACK);			//显示百分号
  	}
	delay_ms(1000/OS_TICKS_PER_SEC);//延时一个时钟节拍
	return 0;	
}
//更新系统文件夹
//caption:标题
void sysset_system_update(u8*caption)
{
  	_window_obj* twin=0;			//窗体
	u8 res;
	u8 *p;
  	twin=window_creat(20,90,200,120,0,1,16);//创建窗口
  	if(twin!=NULL)
	{
		//窗口的名字和背景色
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
 		window_draw(twin);						//画出窗体
		gui_phy.back_color=APP_WIN_BACK_COLOR;
  		gui_show_string((u8*)sysset_system_update_cpymsg_tbl[0][gui_phy.language],20+10,90+32+15,54,12,12,BLACK);	//正在复制:
  		gui_show_string((u8*)sysset_system_update_cpymsg_tbl[1][gui_phy.language],20+10,90+32+15+21,66,12,12,BLACK);//当前文件夹:
 		gui_draw_rectangle(20+10-1,90+32+15+21*2-1,182,18,0X4A49);		//画边框
		gui_fill_rectangle(20+10,90+32+15+21*2,180,16,WHITE);			//填充底色  0XE73C 
		gui_show_num(20+10+72,90+32+15+21*2+2,3,BLACK,12,0,1);			//显示更新进度
		gui_show_string("%",20+10+72+18,90+32+15+21*2+2,6,12,12,BLACK);	//显示百分号
		res=app_system_update(sysset_system_update_cpymsg);				//更新
	   	p=(u8*)sysset_system_update_err_tbl[0][gui_phy.language];		//更新成功提示
 		if(res==0XFF)p=(u8*)sysset_system_update_err_tbl[2][gui_phy.language];	//强制退出
		else if(res)p=(u8*)sysset_system_update_err_tbl[1][gui_phy.language];	//系统文件丢失
  		window_msg_box(20,110,200,80,p,caption,12,0,0);
		delay_ms(1500); 
	}
	window_delete(twin);
	system_task_return=0;//取消TPAD
}   	  
//显示系统信息
//caption:标题
void sysset_system_info(u16 x,u16 y,u8*caption)
{
  	_window_obj* twin=0;			//窗体
 	_btn_obj * rbtn=0;				//返回按钮		  
	u8 rval=0;
	u8 *msgbuf;
	u8 numstrbuf[6];
 	u8 i;
	u32 dtsize,dfsize; 
	msgbuf=mymalloc(SRAMIN,31);		//申请31个字节
  	twin=window_creat(x,y,200,272,0,1|1<<5,16);//创建窗口
 	rbtn=btn_creat(x+50,y+32+10+20*9+10,SYSSET_BTN3_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);//创建按钮
  	if(twin!=NULL&&rbtn!=NULL)
	{
		//窗口的名字和背景色
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
 		//返回按钮的颜色    
		rbtn->bkctbl[0]=0X8452;//边框颜色
		rbtn->bkctbl[1]=0XAD97;//第一行的颜色				
		rbtn->bkctbl[2]=0XAD97;//上半部分颜色
		rbtn->bkctbl[3]=0X8452;//下半部分颜色
 		rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//标题为返回

 		window_draw(twin);	//画出窗体	
		btn_draw(rbtn);	  	//画按键
 		gui_draw_rectangle(x+10-4,y+32+14-4,188,180,0X4A49);		//画边框
 		gui_phy.back_color=APP_WIN_BACK_COLOR;
 		rval=strlen((const char*)sysset_system_info_caption_tbl[0][gui_phy.language]);
		for(i=0;i<9;i++)
		{
	 		strcpy((char*)msgbuf,(const char*)sysset_system_info_caption_tbl[i][gui_phy.language]);
	 		if(i==2||i==3)//SD卡/FLASH盘  容量
			{
				dtsize=0;
				dfsize=0;
				if(i==2)
				{
					exf_getfree("0:",&dtsize,&dfsize);	//得到SD卡剩余容量和总容量
					dtsize>>=10;
					dfsize>>=10;
				}else exf_getfree("1:",&dtsize,&dfsize);//得到FLASH盘剩余容量和总容量  
				 
 				gui_num2str(numstrbuf,dfsize);
				strcat((char*)msgbuf,(const char*)numstrbuf);//增加空闲容量
				if(i==2)strcat((char*)msgbuf,(const char*)"MB/");
				else strcat((char*)msgbuf,(const char*)"KB/"); 
				gui_num2str(numstrbuf,dtsize);
				strcat((char*)msgbuf,(const char*)numstrbuf);//增加总容量	  
			}
			strcat((char*)msgbuf,(const char*)sysset_system_info_msg_tbl[i]);
			if(i==4||i==5||i==6)
			{									   
				if(i==4)app_get_version(numstrbuf,OS_VERSION,3);	  	//OS版本
				else if(i==5)app_get_version(numstrbuf,GUI_VERSION,3);	//GUI版本
				else app_get_version(numstrbuf,HARDWARE_VERSION,2);		//硬件版本
				strcat((char*)msgbuf,(const char*)numstrbuf);		 
 			}     
	   		gui_show_string(msgbuf,x+10,y+32+14+20*i,180,12,12,BLACK);//显示信息
		} 
 		while(1)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
	 		if(system_task_return)break;			//TPAD返回	
			rval=btn_check(rbtn,&in_obj);			//返回按钮检测
			if(rval&&(rbtn->sta&0X80)==0)break;	 	//有有效操作
	 
		}
 	}
	myfree(SRAMIN,msgbuf);
	window_delete(twin);
	btn_delete(rbtn);
	system_task_return=0;//取消TPAD
}	  

//显示系统状态
//x,y:显示的坐标
//宽度:200,高度:222.
//caption:名称
void sysset_system_status(u16 x,u16 y,u8* caption)
{
 	u8 rval=0,res;
	u8 i=0;
	u16 temperate=0;
 	_window_obj* twin=0;			//窗体
 	_btn_obj * rbtn=0;				//返回按钮		  
	_progressbar_obj * cpuprgb=0;	//CPU使用率进度条
	_progressbar_obj * meminprgb=0;	//内部内存使用进度条 
	_progressbar_obj * memexprgb=0;	//外部内存使用进度条

 	twin=window_creat(x,y,200,222,0,1|1<<5,16);					//创建窗口
	cpuprgb=progressbar_creat(x+10,y+32+22,180,18,0X61);		//创建进度条
	meminprgb=progressbar_creat(x+10,y+32+22+45,180,18,0X61);	//创建进度条
	memexprgb=progressbar_creat(x+10,y+32+22+45*2,180,18,0X61);	//创建进度条
	rbtn=btn_creat(x+50,y+32+22+45*2+38,SYSSET_BTN3_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);//创建按钮
 	if(twin==NULL||cpuprgb==NULL||meminprgb==NULL||memexprgb==NULL||rbtn==NULL)rval=1;
	else
	{
		//窗口的名字和背景色
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
		//返回按钮的颜色    
		rbtn->bkctbl[0]=0X8452;//边框颜色
		rbtn->bkctbl[1]=0XAD97;//第一行的颜色				
		rbtn->bkctbl[2]=0XAD97;//上半部分颜色
		rbtn->bkctbl[3]=0X8452;//下半部分颜色

		rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//标题为返回

		cpuprgb->totallen=100; 			//最大是100
		meminprgb->totallen=100;    	//最大是100
		memexprgb->totallen=100;		//最大是100
														 
 		window_draw(twin);						//画出窗体
		btn_draw(rbtn);							//画按钮

		gui_show_string((u8*)sysset_sysstatus_tbl[0][gui_phy.language],x+10,y+32+5,190,12,12,SYSSET_INWIN_FONT_COLOR);		//显示CPU使用率
 		gui_show_string((u8*)sysset_sysstatus_tbl[1][gui_phy.language],x+10,y+32+5+45,190,12,12,SYSSET_INWIN_FONT_COLOR);	//显示内部内存使用率
		gui_show_string((u8*)sysset_sysstatus_tbl[2][gui_phy.language],x+10,y+32+5+45*2,190,12,12,SYSSET_INWIN_FONT_COLOR);	//显示外部内存使用率
 		gui_show_string((u8*)sysset_sysstatus_tbl[3][gui_phy.language],x+200-10-36-36,y+32+5,190,12,12,SYSSET_INWIN_FONT_COLOR);//显示温度
 		gui_show_string("℃",x+200-10-12,y+32+5,190,12,12,SYSSET_INWIN_FONT_COLOR);	//显示CPU当前温度
 	}
 	while(rval==0)
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
		delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
 		if(system_task_return)break;			//TPAD返回	
		res=btn_check(rbtn,&in_obj);			//返回按钮检测
		if(res)
		{
			if((rbtn->sta&0X80)==0)//有有效操作
			{
				break;//退出
			}
		}	
		if(i==0)//时间到了,更新内存,CPU等的状态
		{
			cpuprgb->curpos=OSCPUUsage;
			meminprgb->curpos=mem_perused(0);	//内部内存使用率
			memexprgb->curpos=mem_perused(1);	//外部内存使用率

			progressbar_draw_progressbar(cpuprgb);	//显示进度条
			progressbar_draw_progressbar(meminprgb);//显示进度条
			progressbar_draw_progressbar(memexprgb);//显示进度条
			//显示温度值
			temperate=Get_Temp();//得到内部温度
			app_show_float(x+200-10-12,y+32+5,temperate,1,5,12,SYSSET_INWIN_FONT_COLOR,twin->windowbkc);//显示温度
  			i=100;
		}
		i--;
	}
	window_delete(twin);			//删除窗口
	btn_delete(rbtn);				//删除按钮	
	progressbar_delete(cpuprgb);	//删除进度条
	progressbar_delete(meminprgb);	//删除进度条
	progressbar_delete(memexprgb);	//删除进度条
	system_task_return=0;		 	//清除退出标志
}
//关于系统
//caption:标题
void sysset_system_about(u16 x,u16 y,u8*caption)
{
  	_window_obj* twin=0;			//窗体
 	_btn_obj * rbtn=0;				//返回按钮		  
	u8 rval=0;
 	u32 sn0,sn1,sn2;
	u8 *tbuf;		  
	u8 verbuf[6];

 	tbuf=mymalloc(SRAMIN,31);					//申请31个字节
  	twin=window_creat(x,y,200,262,0,1|1<<5,16);	//创建窗口
 	rbtn=btn_creat(x+50,y+32+180+10,SYSSET_BTN3_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);//创建按钮
  	if(twin!=NULL&&rbtn!=NULL)
	{
		//窗口的名字和背景色
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
 		//返回按钮的颜色    
		rbtn->bkctbl[0]=0X8452;	//边框颜色
		rbtn->bkctbl[1]=0XAD97;	//第一行的颜色				
		rbtn->bkctbl[2]=0XAD97;	//上半部分颜色
		rbtn->bkctbl[3]=0X8452;	//下半部分颜色
 		rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//标题为返回

 		window_draw(twin);		//画出窗体	
		btn_draw(rbtn);	  		//画按键
 		gui_draw_rectangle(x+10-4,y+32+10+8,188,102,0X4A49);		//画边框
 		gui_draw_rectangle(x+10-4,y+32+120+18,188,42,0X4A49);		//画边框
 
  		gui_phy.back_color=APP_WIN_BACK_COLOR;
		gui_show_ptstr(x+10,y+32+10,x+10+180,y+32+10+16,0,BLACK,16,(u8*)sysset_system_about_caption_tbl[0][gui_phy.language],0);//ALIENTEK 战舰
 	 	strcpy((char*)tbuf,"HARDWARE:");
		app_get_version(verbuf,HARDWARE_VERSION,2);
		strcat((char*)tbuf,(const char*)verbuf);
		strcat((char*)tbuf,", SOFTWARE:");						   
		app_get_version(verbuf,SOFTWARE_VERSION,3);
		strcat((char*)tbuf,(const char*)verbuf);
  		gui_show_string(tbuf,x+10,y+32+34,180,12,12,BLACK);//显示信息
 		gui_show_string("Copyright (C) 2010~2020",x+10,y+32+34+20*1,180,12,12,BLACK);//显示信息
 		gui_show_string("广州星翼电子科技有限公司",x+10,y+32+34+20*2,180,12,12,BLACK);//显示信息
 		gui_show_string("www.openedv.com",x+10,y+32+34+20*3,180,12,12,BLACK);//显示信息
 
  		gui_show_ptstr(x+10,y+32+120+10,x+10+180,y+32+120+10+16,0,BLACK,16,(u8*)sysset_system_about_caption_tbl[1][gui_phy.language],0);//产品序列号
		app_getstm32_sn(&sn0,&sn1,&sn2);//得到序列号
		sprintf((char*)tbuf,"SN:%X%X%X",sn0,sn1,sn2);
		gui_show_string(tbuf,x+10,y+32+120+10+24,180,12,12,BLACK);//显示信息
  		while(1)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
	 		if(system_task_return)break;			//TPAD返回	
			rval=btn_check(rbtn,&in_obj);			//返回按钮检测
			if(rval&&(rbtn->sta&0X80)==0)break;	 	//有有效操作
	 
		}
 	}
	myfree(SRAMIN,tbuf);
	window_delete(twin);
	btn_delete(rbtn);
	system_task_return=0;//取消TPAD
}


//查找条目名字
//mcaption:主目录下的目录名(一定要有'.'字符在里面)
u8 * set_search_caption(const u8 *mcaption)
{
	while(*mcaption!='.')mcaption++;
	return (u8*)(++mcaption);
}
//系统设置数据保存在:SYSTEM_PARA_SAVE_BASE
//读取系统设置信息
//sysset:系统信息 
void sysset_read_para(_system_setings * sysset)
{
	AT24CXX_Read(SYSTEM_PARA_SAVE_BASE,(u8*)sysset,sizeof(_system_setings));
}
//写入系统设置信息
//sysset:系统信息 
void sysset_save_para(_system_setings * sysset)
{
  	OS_CPU_SR cpu_sr=0;
	OS_ENTER_CRITICAL();//进入临界区(无法被中断打断) 
	AT24CXX_Write(SYSTEM_PARA_SAVE_BASE,(u8*)sysset,sizeof(_system_setings));
	OS_EXIT_CRITICAL();	//退出临界区(可以被中断打断)
}

//系统设置 
u8 sysset_play(void)
{	
	u8 tlanguage;			//进入时语言的类型					  	   
	u8 res;		 
	u8 selitem=0;
	u8 topitem=0;  	 
	u8 **items;	     					   	   
 	u8 *caption;			//标题列表 
	u8 *scaption;			//子标题
	u16 temp1;
	u8 temp2,temp3;			//时间/日期暂存
	u8 savemask=0;		    //0,不需要保存任何数据;
							//[0]:systemset数据改动标记
							//[1]:vsset数据改动标记
							//[2]:alarm数据改动标记	   

    caption=(u8*)APP_MFUNS_CAPTION_TBL[5][gui_phy.language];	 
	items=(u8**)sysset_mmenu_tbl[gui_phy.language];
	tlanguage=gui_phy.language;							//保存刚进入是语言的种类
	while(1)
	{   						   
   		res=app_listbox_select(&selitem,&topitem,caption,items,SYS_MENU_SIZE);
 		if(system_task_return)break;					//TPAD返回
		if(res&0X80)
		{	  
			scaption=set_search_caption(items[selitem]); 	 						   
			switch(selitem)//判断选择的条目
			{
				case 0://时间设置 			   
					temp1=calendar.hour;
					temp2=calendar.min;
					res=sysset_time_set(45,60,(u8*)&temp1,(u8*)&temp2,scaption);
					if(res==0)//成功
					{
 						RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,temp1,temp2,0);//设置时间
					} 
  					break;	  
				case 1://日期设置  	    
					temp1=calendar.w_year;
					temp2=calendar.w_month;	 
					temp3=calendar.w_date;
					res=sysset_date_set(25,60,&temp1,(u8*)&temp2,(u8*)&temp3,scaption);
					if(res==0)//成功
					{
						RTC_Set(temp1,temp2,temp3,calendar.hour,calendar.min,calendar.sec);//设置时间
					} 																	  
					break;
				case 2://闹钟时间设置
					temp1=alarm.hour;
					temp2=alarm.min;
					res=sysset_time_set(45,60,(u8*)&temp1,(u8*)&temp2,scaption);
					if(res==0)//成功,点击了确认按钮
					{
						alarm.hour=temp1;
						alarm.min=temp2;
						calendar_alarm_init(&alarm);//重新初始化闹钟
						savemask|=1<<2;//标记闹钟数据改动了
					} 
  					break;
				case 3://闹钟开关设置
					temp1=alarm.weekmask;
			 		res=app_items_sel(20,9,200,302,(u8**)calendar_week_table[gui_phy.language],7,(u8*)&temp1,0XB0,scaption);//多选
					if(res==0)//设置成功
					{
						alarm.weekmask=temp1;
						calendar_alarm_init(&alarm);//重新初始化闹钟	
						savemask|=1<<2;//标记闹钟数据改动了
					}   
					break;
				case 4://闹钟铃声类型设置
					temp1=alarm.ringsta&0X03;
			 		res=app_items_sel(30,44,180,72+40*4,(u8**)sysset_ring_tbl[gui_phy.language],4,(u8*)&temp1,0X90,scaption);//单选
					if(res==0)//设置成功
					{
						alarm.ringsta&=0XFC;//取消最后两位原来的设置
						alarm.ringsta|=temp1;//重新设置铃声						    
						savemask|=1<<2;//标记闹钟数据改动了
					}   
					break;
				case 5://语言设置
					temp1=gui_phy.language;//得到之前的设置
			 		res=app_items_sel(30,64,180,72+40*3,(u8**)sysset_language_tbl,3,(u8*)&temp1,0X90,scaption);//单选
					if(res==0)//设置成功
					{												   
						gui_phy.language=temp1;		//重新设置语言
						systemset.syslanguage=temp1;//重新设置默认语言
					    caption=(u8*)APP_MFUNS_CAPTION_TBL[5][gui_phy.language];	 
						items=(u8**)sysset_mmenu_tbl[gui_phy.language];			 
						savemask|=1<<0;//标记系统设置数据改动了
					} 
					break; 
				case 6://图片浏览模式设置				 
					temp1=systemset.picmode;//得到之前的设置
			 		res=app_items_sel(30,84,180,72+40*2,(u8**)sysset_mp3mode_tbl[gui_phy.language],2,(u8*)&temp1,0X90,scaption);//2个模式
					if(res==0)//设置成功
					{												   
						systemset.picmode=temp1;//重设图片浏览模式			   		 
						savemask|=1<<0;//标记系统设置数据改动了
					} 
					break; 				
				case 7://MP3播放模式设置				 
					temp1=systemset.mp3mode;//得到之前的设置
			 		res=app_items_sel(30,64,180,72+40*3,(u8**)sysset_mp3mode_tbl[gui_phy.language],3,(u8*)&temp1,0X90,scaption);//3个模式
					if(res==0)//设置成功
					{												   
						systemset.mp3mode=temp1;//重设MP3播放模式			   		 
						savemask|=1<<0;//标记系统设置数据改动了
					} 
					break;
				case 8://MP3音效设置
					res=sysset_mp3_effict_set(20,14,&vsset,scaption);
 					if(res==0)//设置成功
					{												   
						VS_Set_All();//恢复原来设置			   		 
						savemask|=1<<1;//标记vsset数据改动了
					}  
					break;
				case 9://FM发射开关设置
					temp1=systemset.fmtxmode;//得到之前的设置
			 		res=app_items_sel(30,84,180,72+40*2,(u8**)sysset_fmsend_onoff_tbl[gui_phy.language],2,(u8*)&temp1,0X90,scaption);//2个模式
					if(res==0)//设置成功
					{												   
						systemset.fmtxmode=temp1;//设置模式			   		 
						savemask|=1<<0;//标记系统设置数据改动了
 						if(systemset.fmtxmode)//设置为发送模式
						{
							RDA5820_TX_Mode(); 
							RDA5820_Freq_Set(systemset.fmtxfreq*10);	//设置到systemset.fmtxfreq的频率值
 							RDA5820_TxPGA_Set(3); 						//设置增益为3
						}else RDA5820_RX_Mode();//设置为接收模式 	
					} 
					break; 	 
				case 10://FM发射频率设置
 					temp1=systemset.fmtxfreq;//得到之前的频率
			 		res=sysset_fmtx_freq_set(45,60,&temp1,scaption);//2个模式
					if(res==0)//设置成功
					{												   
						systemset.fmtxfreq=temp1;//设置新频率	
						if(systemset.fmtxmode)
						{
							RDA5820_TX_Mode();//设置为发送模式 
							RDA5820_Freq_Set(systemset.fmtxfreq*10);	//设置到systemset.fmtxfreq的频率值
 							RDA5820_TxPGA_Set(3); 						//设置增益为3
						}else RDA5820_RX_Mode(); //设置为接收模式 		   		 
						savemask|=1<<0;//标记系统设置数据改动了
 					} 		  
					break;						
				case 11://FM收音设置
					temp1=systemset.fmrxmode;//得到之前的设置
			 		res=app_items_sel(30,84,180,72+40*2,(u8**)sysset_fm_radio_tbl[gui_phy.language],2,(u8*)&temp1,0X90,scaption);//2个模式
					if(res==0)//设置成功
					{												   
						systemset.fmrxmode=temp1;//设置模式			   		 
						savemask|=1<<0;//标记系统设置数据改动了
					} 
					break; 
				case 12://背光设置
 					temp1=systemset.lcdbklight;//得到之前的背光
 			 		res=sysset_bklight_set(30,90,scaption,&temp1);//背光设置
					if(res==0)//设置成功
					{												   
						systemset.lcdbklight=temp1;//设置新背光亮度值	       		   		 
						savemask|=1<<0;//标记系统设置数据改动了
					}else 
					LCD_BLPWM_VAL=systemset.lcdbklight;					
					break;
				case 13://屏幕校准
					TP_Adjust();
					system_task_return=0;	//清除退出标志
					break;
				case 14://传感器校准
 			     	window_msg_box(20,110,200,100,(u8*)sysset_sensoradjust_tbl[gui_phy.language],scaption,12,0,0);
					ADXL345_AUTO_Adjust((char *)&systemset.t3dxoff,(char *)&systemset.t3dyoff,(char *)&systemset.t3dzoff);
					savemask|=1<<0;//标记系统设置数据改动了
 					break;
				case 15://系统更新
					res=window_msg_box(20,120,200,80,"",(u8*)sysset_system_update_remaindmsg_tbl[gui_phy.language],12,0,0X03);
					if(res==1)//需要更新
					{
				    	sysset_system_update((u8*)sysset_system_update_msg_tbl[gui_phy.language]);
					}
					system_task_return=0;//取消TPAD 
					break;
				case 16://系统信息
					sysset_system_info(20,24,scaption);
					break;
				case 17://系统状态 显示CPU使用率/内存状态等
					sysset_system_status(20,54,scaption);
					break;
				case 18://关于
					sysset_system_about(20,29,scaption);
					break;	  				 				
			}
  		}else break;//有错误发生了	 			   
	}
	if(savemask&1<<0)//系统设置数据改动了
	{
		printf("sysset save!\r\n");
		sysset_save_para(&systemset);	//保存系统设置信息 
	}
	if(savemask&1<<1)//vsset数据改动了
	{
		printf("vsset save!\r\n");
		vs10xx_save_para(&vsset); 		//保存VS10XX设置信息 
	}
	if(savemask&1<<2)//闹钟数据改动了
	{
		printf("alarm save!\r\n");
		calendar_save_para(&alarm);		//保存闹钟设置信息 
	}   
	if(tlanguage!=gui_phy.language)//语言发生了改变
	{
		LCD_Clear(BLACK);
     	window_msg_box(20,110,200,100,(u8*)sysset_remindmsg_tbl[1][gui_phy.language],(u8*)sysset_remindmsg_tbl[0][gui_phy.language],12,0,0);
		spb_init();//更新主界面.
		delay_ms(100);//等待界面更新完成结束.
	}
	return res;			   							  
}






















