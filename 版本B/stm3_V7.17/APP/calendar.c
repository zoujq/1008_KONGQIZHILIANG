#include "calendar.h" 	      						  
#include "stdio.h"
#include "settings.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-日历 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

_alarm_obj alarm;	//闹钟结构体
					 
#define TIME_TOPY	120
#define OTHER_TOPY	200 	


const u8 *calendar_week_table[GUI_LANGUAGE_NUM][7]=
{
{"星期天","星期一","星期二","星期三","星期四","星期五","星期六"},
{"星期天","星期一","星期二","星期三","星期四","星期五","星期六"},
{"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"},
};
//闹钟标题
const u8 *calendar_alarm_caption_table[GUI_LANGUAGE_NUM]=
{
"闹钟","鬧鐘","ALARM",
};
//再响按钮
const u8 *calendar_alarm_realarm_table[GUI_LANGUAGE_NUM]=
{
"再响","再響","REALARM",
};

const u8* calendar_loading_str[GUI_LANGUAGE_NUM][3]=
{
{
	"正在加载,请稍候...",
	"未检测到DS18B20!",
	"启用内部温度传感器...",
},
{
	"正在加载,请稍候...",
	"未检测到DS18B20!",
	"启用内部温度传感器...",
},
{
	"Loading...",
	"DS18B20 Check Failed!",
	"Start Inside Sensor...",
},
};	

//重新初始化闹钟		    
//alarmx:闹钟结构体
void calendar_alarm_init(_alarm_obj *alarmx) 
{	  
  	u32 curtime=0;
	u32 temptime=0;
	u32 destime=0XFFFFFFFF;//目标闹铃时间(s)设定到最大	
 	if(alarmx->weekmask)//必须要有闹钟存在,周日到周六任意一天 
	{										       
		curtime=RTC->CNTH;//得到计数器中的值(秒钟数)
		curtime<<=16;
		curtime+=RTC->CNTL;	   
		//取一个与当前时间最接近的值作为闹钟寄存器的内容
		temptime=curtime/86400;	//得到当前运行天数(此处没有用到天数,仅作说明用)
		temptime=temptime*86400;
 		temptime+=(u32)alarmx->hour*3600+(u32)alarmx->min*60;//得到秒钟数
 		if(temptime<=curtime)temptime+=86400;//执行时间已过，推迟到明天
 		destime=temptime;//更改闹钟寄存器
 	}
	RCC->APB1ENR|=1<<28;//使能电源时钟
    RCC->APB1ENR|=1<<27;//使能备份时钟
	PWR->CR|=1<<8;    //取消备份区写保护
	//上面三步是必须的!
	RTC->CRL|=1<<4;   //允许配置 
	RTC->ALRL=destime&0xffff;
	RTC->ALRH=destime>>16;
	RTC->CRL&=~(1<<4);//配置更新
	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成  
}
//闹钟响闹铃
//type:闹铃类型	   
//0,滴.
//1,滴.滴.
//2,滴.滴.滴
//4,滴.滴.滴.滴
void calendar_alarm_ring(u8 type)
{
	u8 i;	 
	for(i=0;i<(type+1);i++)
	{
		BEEP=1;
		delay_ms(50);
		BEEP=0;
		delay_ms(70);
	}	 
}

//根据当前的日期,更新日历表.
void calendar_date_refresh(void) 
{
 	u8 weekn;   //周寄存
 	//显示阳历年月日
	POINT_COLOR=BRED; 
	BACK_COLOR=BLACK; 
	LCD_ShowxNum(5,OTHER_TOPY+9,(calendar.w_year/100)%100,2,16,0);//显示年  20/19  
	LCD_ShowxNum(21,OTHER_TOPY+9,calendar.w_year%100,2,16,0);     //显示年  
	LCD_ShowString(37,OTHER_TOPY+9,240,320,16,"-"); //"-"
	LCD_ShowxNum(45,OTHER_TOPY+9,calendar.w_month,2,16,0);        //显示月
	LCD_ShowString(61,OTHER_TOPY+9,240,320,16,"-"); //"-"
	LCD_ShowxNum(69,OTHER_TOPY+9,calendar.w_date,2,16,0);         //显示日	  
	//显示周几?
	POINT_COLOR=RED;
    weekn=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//得到星期	   
	Show_Str(10,OTHER_TOPY+35,240,320,(u8 *)calendar_week_table[gui_phy.language][weekn],16,0); //显示周几?	
													 
}
//颜色列表.
const u16 t3d_colortbl[8]={0XFA8A,0XF800,0XFFFF,0X0673,0XF81F,0XFFFF,0X67E6,0XFFE0};
//更新3d时间
void calendar_3dtime_refresh(_t3d_obj* t3dx)
{
	u8 tbuf[9];
	t3d_clear_obj(5,5,230,110,t3dx,BLACK);//清除上一次的显示
	tbuf[0]=calendar.hour/10+'0';
 	tbuf[1]=calendar.hour%10+'0';
	tbuf[2]=':';
	tbuf[3]=calendar.min/10+'0';
	tbuf[4]=calendar.min%10+'0';
	tbuf[5]=':';
 	tbuf[6]=calendar.sec/10+'0';
	tbuf[7]=calendar.sec%10+'0';
	tbuf[8]=0;//结束符
	t3dx->pixsize=0;
  	t3d_show_str(t3dx,-32,-8,-10,tbuf,16,(u16 *)t3d_colortbl,1);
}

//3D显示
//t3dx:3D结构体
//dx,dy.dz:3个轴向的角度
void calendar_3dtime_show(_t3d_obj* t3dx,u8 dx,u8 dy,u8 dz)
{			   
	t3dx->ang.z+=dz;
 	t3dx->ang.y+=dy;
 	t3dx->ang.x+=dx;
	t3d_show_obj(5,5,230,110,t3dx,BLACK);
}
//3d显示的位置变更
void calendar_3dtime_pos_chg(_t3d_obj* t3dx,u8*dir)
{
	switch(*dir) 
	{
		case 0://x++
			t3dx->dis.x++;
			break;
		case 1://x--
			t3dx->dis.x--;
			break;
		case 2://y++
			t3dx->dis.y++;
			break;
		case 3://y--
			t3dx->dis.y--;
			break;
		case 4://x++,y++
			t3dx->dis.y++;
			t3dx->dis.x++;
			break;
		case 5://x--,y--
			t3dx->dis.y--;
			t3dx->dis.x--;
			break;
		case 6://y++,x--
			t3dx->dis.y++;
			t3dx->dis.x--;
			break;
		case 7://y--,x++
			t3dx->dis.x++;
			t3dx->dis.y--;
			break;
	}  
	//范围限定 
	if(t3dx->dis.x>85)
	{
		t3dx->dis.x=85;
	    *dir=1;
	}
	if(t3dx->dis.y>25)
	{
		t3dx->dis.y=25;
		*dir=3; 
	}
	if(t3dx->dis.x<-75)
	{	
		t3dx->dis.x=-75;
		*dir=0;
	}
	if(t3dx->dis.y<-15)
	{
		t3dx->dis.y=-15;
		*dir=2;
	}
}    

//闹钟数据保存在:SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj)
//读取日历闹钟信息
//alarm:闹钟信息 
void calendar_read_para(_alarm_obj * alarm)
{
	AT24CXX_Read(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj),(u8*)alarm,sizeof(_alarm_obj));
}
//写入日历闹钟信息
//alarm:闹钟信息 
void calendar_save_para(_alarm_obj * alarm)
{
  	OS_CPU_SR cpu_sr=0;
	alarm->ringsta&=0X7F;	//清空最高位
	OS_ENTER_CRITICAL();	//进入临界区(无法被中断打断) 
	AT24CXX_Write(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj),(u8*)alarm,sizeof(_alarm_obj));
	OS_EXIT_CRITICAL();		//退出临界区(可以被中断打断)
}

//闹铃处理(尺寸:200*160)
//x,y:坐标
//返回值,处理结果
u8 calendar_alarm_msg(u16 x,u16 y)
{
	u8 rval=0; 
	u8 res;
	u32 abr;						    
	FIL* falarm=0;				  	
  	u8 *bfbase=0;					//大字库的基址
	_window_obj* twin=0;			//窗体
 	_btn_obj * rbtn=0;				//取消按钮		  
 	_btn_obj * okbtn=0;				//确定按钮		  

   	twin=window_creat(x,y,200,160,0,1|1<<5|1<<6,16);//创建窗口,读取背景色
   	okbtn=btn_creat(x+20,y+120,70,30,0,0x02);		//创建按钮
 	rbtn=btn_creat(x+20+70+20,y+120,70,30,0,0x02);	//创建按钮
 	falarm=(FIL *)gui_memin_malloc(sizeof(FIL));	//开辟FIL字节的内存区域 
	if(twin==NULL||rbtn==NULL||okbtn==NULL||falarm==NULL)rval=1; 
	else
	{
		//窗口的名字和背景色
		twin->caption=(u8*)calendar_alarm_caption_table[gui_phy.language];
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

		rbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];		//取消按钮
 		okbtn->caption=(u8*)calendar_alarm_realarm_table[gui_phy.language];	//再响按钮

		res=f_open(falarm,(const TCHAR*)APP_ASCII_60,FA_READ);//打开文件 
		if(res==0)
		{
			bfbase=(u8*)gui_memex_malloc(falarm->fsize);	//为大字体开辟缓存地址
			if(bfbase==0)rval=1;
			else 
			{
				res=f_read(falarm,bfbase,falarm->fsize,(UINT*)&abr);	//一次读取整个文件
			}
			f_close(falarm);
		}

 		window_draw(twin);						//画出窗体
		btn_draw(rbtn);							//画按钮
		btn_draw(okbtn);						//画按钮			    
 		if(res)rval=res;
		else 									//显示闹铃时间
		{
			app_showbig2num(bfbase,x+15,y+32+14,alarm.hour,60,BLUE,APP_WIN_BACK_COLOR); 	//显示时
 			app_showbigchar(bfbase,x+15+60,y+32+14,':',60,BLUE,APP_WIN_BACK_COLOR); 		//":"
			app_showbig2num(bfbase,x+15+90,y+32+14,alarm.min,60,BLUE,APP_WIN_BACK_COLOR);	//显示分	 
		}
		OSTaskSuspend(6); //挂起主任务
  		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值  
			delay_ms(5); 
 	 		if(system_task_return)
			{
				rval=1;			//取消
				break;			//TPAD返回	
			}
			res=btn_check(rbtn,&in_obj);			//取消按钮检测
			if(res&&((rbtn->sta&0X80)==0))			//有有效操作
			{
				rval=1;
				break;//退出
			}
	 		res=btn_check(okbtn,&in_obj);			//再响按钮检测
	 		if(res&&((okbtn->sta&0X80)==0))			//有有效操作
			{		 
				rval=0XFF;//		   
				break;//退出
			}    
		}	 
	}
	alarm.ringsta&=~(1<<7);	//取消闹铃
	if(rval==0XFF)//稍后再响
	{
		alarm.min+=5; 			//推迟5分钟
		if(alarm.min>59)
		{
			alarm.min=alarm.min%60;
			alarm.hour++;
			if(alarm.hour>23)alarm.hour=0;
		}				  
		calendar_alarm_init((_alarm_obj*)&alarm);//重新初始化闹钟
 	}
	window_delete(twin);
	btn_delete(rbtn);
	btn_delete(okbtn); 	  
	gui_memin_free(falarm);
	gui_memex_free(bfbase);
	system_task_return=0;	//取消TPAD
	OSTaskResume(6); 		//恢复主任务
	return rval;
}
	    
//时间显示模式
//通过向左滑动退出.						 
//改用内部sram,貌似OK了.	   
u8 calendar_play(void)
{
	u8 second=0;
	short temperate=0;	//温度值		   
	u8 t=0;
	u8 tempdate=0;
	u8 rval=0;			//返回值	
	u8 res;

	u16 lastx=0;		//记录滑动最后一次检测的x值
	u8  slidercnt=0;	//滑动计数器
	  
	u8 TEMP_SEN_TYPE=0;	//默认使用DS18B20
	FIL* f_calendar=0;	 
	u8 *bfbase=0;		//大字库的基址
   	/////////////3D时间显示///////////////
	u8 dir=0;
	u8 dx=0,dy=0,dz=0;			 
	_t3d_obj* t3d_time;	//3D时间
	u16 timex=0;
	//////////////////////////////////////
	t3d_time=t3d_creat(120,60,50,500);//创建一个500个像素点的3D体
 	if(t3d_time==NULL)rval=1;		//申请失败
 	f_calendar=(FIL *)gui_memin_malloc(sizeof(FIL));//开辟FIL字节的内存区域 
	if(f_calendar==NULL)rval=1;		//申请失败
	else
	{
		res=f_open(f_calendar,(const TCHAR*)APP_ASCII_60,FA_READ);//打开文件 
		if(res==FR_OK)
		{
			bfbase=(u8*)gui_memex_malloc(f_calendar->fsize);	//为大字体开辟缓存地址
			if(bfbase==0)rval=1;
			else 
			{
				res=f_read(f_calendar,bfbase,f_calendar->fsize,(UINT*)&br);	//一次读取整个文件
 			}
			f_close(f_calendar);
		} 
		if(res)rval=res;
	} 	    
	if(rval==0)//无错误
	{	  
 		LCD_Clear(BLACK);//清黑屏    	  
		second=calendar.sec;//得到此刻的秒钟
		POINT_COLOR=GBLUE;
		Show_Str(48,60,240,320,(u8*)calendar_loading_str[gui_phy.language][0],16,0x01); //显示进入信息	    
		if(DS18B20_Init())
		{
			Show_Str(48,76,240,320,(u8*)calendar_loading_str[gui_phy.language][1],16,0x01);  
			delay_ms(500);
			Show_Str(48,92,240,320,(u8*)calendar_loading_str[gui_phy.language][2],16,0x01);  
			Adc_Init();//初始化内部温度传感器	  
			TEMP_SEN_TYPE=1; 
		}   	    
		delay_ms(1100);//等待1.1s 
		BACK_COLOR= BLACK;
		LCD_Clear(BLACK);//清黑屏 
		calendar_date_refresh();  //加载日历
		tempdate=calendar.w_date;//天数暂存器

		app_showbigchar(bfbase,70-4,TIME_TOPY,':',60,GBLUE,BLACK); 		//":"
		app_showbigchar(bfbase,150-4,TIME_TOPY,':',60,GBLUE,BLACK); 	//":"	 
		//gui_draw_rectangle(4,4,232,112,GRED);//画3D显示的边框.
	}
  	while(rval==0)
	{	
		tp_dev.scan(0);    
		if(tp_dev.sta&TP_PRES_DOWN)//触摸按下了
		{
			if(slidercnt==0)//第一次滑动按下
			{
				lastx=tp_dev.x;//记录第一次按下的坐标
				slidercnt++;				
			}else
			{
				if(tp_dev.x>(lastx+10))slidercnt=0;//往右滑动了,直接丢弃  
				if((tp_dev.x+10)<lastx)//向左滑动至少10个像素点
				{
					slidercnt++; 		//是一个有效的滑动
					lastx=tp_dev.x;
				}
				if(slidercnt>5)break;	//滑动有效,直接退出
			}
		}else slidercnt=0;				//松开了,则直接设置滑动无效
		if(system_task_return)break;	//TPAD返回	  
 		if(second!=calendar.sec) //秒钟改变了
		{ 	
  			second=calendar.sec;  
			app_showbig2num(bfbase,10,TIME_TOPY,calendar.hour,60,GBLUE,BLACK);	//显示时
 			app_showbig2num(bfbase,90,TIME_TOPY,calendar.min,60,GBLUE,BLACK);	//显示分	 
 			app_showbig2num(bfbase,170,TIME_TOPY,calendar.sec,60,GBLUE,BLACK);	//显示秒	 					   
			if(t%2==0)//等待2秒钟
			{		 
  				if(TEMP_SEN_TYPE)temperate=Get_Temp();//得到内部温度
				else temperate=DS18B20_Get_Temp();//得到18b20温度
				if(temperate<0)//温度为负数的时候，红色显示
				{
					POINT_COLOR=RED;
					temperate=-temperate;//改为正温度
				}
				else POINT_COLOR=BRRED;		   //正常为棕红色字体显示		 
				app_showbig2num(bfbase,90,OTHER_TOPY,temperate/10,60,GBLUE,BLACK);			//XX
				app_showbigchar(bfbase,150,OTHER_TOPY,'.',60,GBLUE,BLACK); 					//"."
				app_showbigchar(bfbase,180-15,OTHER_TOPY,temperate%10+'0',60,GBLUE,BLACK);	//显示小数
				app_showbigchar(bfbase,210-10,OTHER_TOPY,'C',60,GBLUE,BLACK);				//"/"
				if(t>0)t=0;			 
			}  
			if(calendar.w_date!=tempdate)
			{
				calendar_date_refresh();//天数变化了,更新日历.  
				tempdate=calendar.w_date;//修改tempdate，防止重复进入
			}
			t++;  
 			calendar_3dtime_refresh(t3d_time);
 		}
 		timex++;
   		delay_ms(10);
 		if((timex%800)==0)//每8秒钟改变一次方向
		{
 			dir=app_get_rand(8);
			timex=0;	
		}
		if((timex%400)==0)//每2秒钟改变一次角度
		{
 			dx=app_get_rand(20);
			dy=app_get_rand(20);
			dz=app_get_rand(20);	  
		}
 		if((timex%20)==0)//每200ms更新一次
		{
			calendar_3dtime_pos_chg(t3d_time,&dir);	 
 		}
  		calendar_3dtime_show(t3d_time,dx,dy,dz);
 	};
 	while(tp_dev.sta&TP_PRES_DOWN)tp_dev.scan(0);//等待TP松开.
	t3d_delete(t3d_time);		//删除申请的内存
	gui_memex_free(bfbase);		//删除申请的内存
	gui_memin_free(f_calendar);	//删除申请的内存
	POINT_COLOR=BLUE;
	BACK_COLOR=WHITE ;	
	return rval;
}




















