#ifndef __INCLUDES_H
#define __INCLUDES_H 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//通用的头文件包含 代码	   
//用于包含所有用到的头文件,使得在c文件调用的时候,只需要包含一个.h即可.
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
		 
///////ucosii自带部分////////
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ucos_ii.h"
#include "os_cpu.h"
#include "os_cfg.h"
/////////////end/////////////
#include "string.h"
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
//#include "led.h" 
#include "beep.h"
//#include "key.h"
#include "exti.h"
#include "wdg.h"
#include "spi.h"
//#include "dma.h"
#include "timer.h"
#include "lcd.h" 
//#include "touch.h"
//#include "VS10XX.h"	 
//#include "mp3player.h"	    	 
//#include "audiosel.h"
//#include "rda5820.h"
#include "rtc.h" 	 
//#include "ds18b20.h"
#include "adc.h"
//#include "adxl345.h"
//#include "ov7670.h"
//#include "24l01.h"
///////////////////////////////////////////#include "24cxx.h"
//#include "remote.h"
//#include "tpad.h"
#include "zkled.h"	



//#include "..\sram\sram.h"	  					
//#include "edit.h"
//#include "usmart.h" 

//USB部分
/*#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "usb_regs.h"

#include "gui.h"
#include "exfuns.h"
#include "fattester.h"	 
#include "fontupd.h"
#include "flash.h"   
#include "ff.h"	  
#include "text.h"	
#include "piclib.h"												   
*/
//#include "flash.h"   
//#include "ssd1351.h"
	
//显示部分
//#include "clock_show.h"	
#include "menu_show.h"	
//#include "all_sensor_task.h"	
//#include "user_gprs_cntrl.h"	
#include "clock_control.h"	
#include "flash_code.h"	

#include "calender.h"




#include "icon.h"

#include "user_key_task.h"	
#include "user_watch_task.h"	
#include "time_show.h"	
#include "all_show.h"	
#include "pm25_show.h"	
#include "hcho_show.h"	
#include "tvoc_show.h"	
#include "co2_show.h"	
#include "charge_off_show.h"	

#include "all_sensor_task.h"	
#include "user_iaq2000.h"	
#include "sht21.h"	

#include "user_nbiot_cntrl.h"	
#include "user_si4432_control.h"	

//控制部分

#include "user_at24cxx.h"
#include "user_data_save.h"	





#ifdef SEGGER
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#endif








extern volatile u8 system_task_return;		//任务强制返回标志.
#define ICOS_SD_CARD      	0				//SD卡
#define ICOS_FLASH_DISK		1				//FLASH DISK
#define ICOS_LOCATION		1				//定义图标所在位置,如果为0,则代表所有图标存放在SD卡,如果为1,则代表所有图标存放在flash disk	    
#endif
























