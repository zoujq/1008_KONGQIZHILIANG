#include "includes.h"
#include "sramlcd.h"
#include "dma.h"
#include "guix.h"
#include "spb.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//SPB效果实现 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   


//SPB 控制器
m_spb_dev spbdev;
#if ICOS_LOCATION==ICOS_SD_CARD//存放在SD卡  
//背景图路径
const u8* spb_bkpic_path_tbl[2]=
{
	"0:/SYSTEM/SPB/BACKPIC/LEFT.jpg",
	"0:/SYSTEM/SPB/BACKPIC/RIGHT.jpg",	
};	  
//icos的路径表
const u8 * spb_icos_path_tbl[2][9]=
{
	{
		"0:/SYSTEM/SPB/ICOS/ebook.bmp",
	 	"0:/SYSTEM/SPB/ICOS/picture.bmp",	    
	  	"0:/SYSTEM/SPB/ICOS/music.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/appstore.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/time.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/set.bmp",	    
	  	"0:/SYSTEM/SPB/ICOS/game.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/radio.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/notebook.bmp",
	},	    
 	{
	 	"0:/SYSTEM/SPB/ICOS/setup.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/3d.bmp",	    
		"0:/SYSTEM/SPB/ICOS/paint.bmp",
	 	"0:/SYSTEM/SPB/ICOS/camera.bmp",	    
	  	"0:/SYSTEM/SPB/ICOS/recorder.bmp",	    
	  	"0:/SYSTEM/SPB/ICOS/usb.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/tomcat.bmp",
	  	"0:/SYSTEM/SPB/ICOS/wireless.bmp",	    
	 	"0:/SYSTEM/SPB/ICOS/calculator.bmp",
	},	 	    
};
#else//存放在 FLASH DISK
//背景图路径
const u8* spb_bkpic_path_tbl[2]=
{
	"1:/SYSTEM/SPB/BACKPIC/LEFT.jpg",
	"1:/SYSTEM/SPB/BACKPIC/RIGHT.jpg",	
};	  
//icos的路径表
const u8 * spb_icos_path_tbl[2][9]=
{
	{
		"1:/SYSTEM/SPB/ICOS/ebook.bmp",
	 	"1:/SYSTEM/SPB/ICOS/picture.bmp",	    
	  	"1:/SYSTEM/SPB/ICOS/music.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/appstore.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/time.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/set.bmp",	    
	  	"1:/SYSTEM/SPB/ICOS/game.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/radio.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/notebook.bmp",
	},	    
 	{
	 	"1:/SYSTEM/SPB/ICOS/setup.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/3d.bmp",	    
		"1:/SYSTEM/SPB/ICOS/paint.bmp",
	 	"1:/SYSTEM/SPB/ICOS/camera.bmp",	    
	  	"1:/SYSTEM/SPB/ICOS/recorder.bmp",	    
	  	"1:/SYSTEM/SPB/ICOS/usb.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/tomcat.bmp",
	  	"1:/SYSTEM/SPB/ICOS/wireless.bmp",	    
	 	"1:/SYSTEM/SPB/ICOS/calculator.bmp",
	},	 	    
};
#endif


//各个ICOS的对应功能名字
const u8* icos_name_tbl[GUI_LANGUAGE_NUM][2][9]=
{
	{
		{
			"电子图书","数码相框","音乐播放",
			"应用中心","时钟","系统设置",
			"FC游戏机","收音机","记事本",	   
		},
		{
			"运行器","3D","手写画笔",
			"照相机","录音机","USB 连接",
			"TOM 猫","无线传书","计算器",
		},
	},
	{
		{
			"電子圖書","數碼相框","音樂播放",
			"應用中心","時鐘","系統設置",
			"FC遊戲機","收音機","記事本",	   
		},
		{
			"運行器","3D","手寫畫筆",
			"照相机","錄音機 ","USB 連接",
			"TOM 貓","無線傳書","計算器",
		},
	},
	{
		{
			"EBOOK","PHOTOS","MUSIC",
			"APP","TIME","SETTINGS",
			"FC GAMES","RADIO","NOTEPAD",	   
		},
		{
			"EXE","3D","PAINT",
			"CAMERA","RECODER","USB",
			"TOM CAT","WIRELESS","CALC",
		},
	},
};						  

//初始化spb各个参数			  
void spb_init(void)
{
	u16 i,j;
	spbdev.oldxpos=0;
	spbdev.curxpos=0;
	spbdev.curypos=0;
	spbdev.spbsta=0;
	spbdev.selico=0xff;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			spbdev.icos[0][i*3+j].x=5+j*80;
			spbdev.icos[0][i*3+j].y=10+i*105;
			spbdev.icos[0][i*3+j].width=70;
			spbdev.icos[0][i*3+j].height=90;
			spbdev.icos[0][i*3+j].path=(u8*)spb_icos_path_tbl[0][i*3+j];
			spbdev.icos[0][i*3+j].name=(u8*)icos_name_tbl[gui_phy.language][0][i*3+j];
		
			spbdev.icos[1][i*3+j].x=5+j*80;
			spbdev.icos[1][i*3+j].y=10+i*105;
			spbdev.icos[1][i*3+j].width=70;
			spbdev.icos[1][i*3+j].height=90;
			spbdev.icos[1][i*3+j].path=(u8*)spb_icos_path_tbl[1][i*3+j]; 
			spbdev.icos[1][i*3+j].name=(u8*)icos_name_tbl[gui_phy.language][1][i*3+j];
		}
	} 
 	//配置sram->lcd的dma设置
	LCD_DMA_Coinfig();
	//指向sramlcd
	gui_phy.read_point=slcd_read_point;
	gui_phy.draw_point=slcd_draw_point;	 
	pic_phy.read_point=slcd_read_point;
	pic_phy.draw_point=slcd_draw_point;	 
	 
	SLCD.frame=0;						 
	ai_load_picfile(spb_bkpic_path_tbl[0],0,0,240,320);//画第一张图片
	spb_load_icos(SLCD.frame);
// 	SLCD.frame=1;	//帧计数器增加
//	ai_load_picfile(spb_bkpic_path_tbl[1],0,0,240,320);//画第二张图片
//	spb_load_icos(SLCD.frame);
	//指向lcd
	gui_phy.read_point=LCD_ReadPoint;
	gui_phy.draw_point=LCD_Fast_DrawPoint;	 
	pic_phy.read_point=LCD_ReadPoint;
	pic_phy.draw_point=LCD_Fast_DrawPoint;	 
	SLCD.pos=0;  		//设置当前显示的位置			 
	SLCD.frame=0;
	SLCD.show(SLCD.pos);//显示主界面
}
//装载ICOS
void spb_load_icos(u8 frame)
{
	u8 i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			minibmp_decode(spbdev.icos[frame][i*3+j].path,spbdev.icos[frame][i*3+j].x+5,spbdev.icos[frame][i*3+j].y,spbdev.icos[frame][i*3+j].width,spbdev.icos[frame][i*3+j].height,0,0);
 			gui_show_strmid(spbdev.icos[frame][i*3+j].x,spbdev.icos[frame][i*3+j].y+67,spbdev.icos[frame][i*3+j].width,16,SPB_FONT_COLOR,16,spbdev.icos[frame][i*3+j].name);//显示名字  
		}
	}
}
//设置选中哪个图标
//sel:0~8代表当前页的选中ico
void spb_set_sel(u8 sel)
{
	spbdev.selico=sel;
	SLCD.show(SLCD.pos);//刷新背景
	delay_ms(30);     
	gui_alphablend_area(spbdev.icos[SLCD.frame][sel].x,spbdev.icos[SLCD.frame][sel].y,spbdev.icos[SLCD.frame][sel].width,spbdev.icos[SLCD.frame][sel].height,SPB_ALPHA_COLOR,SPB_ALPHA_VAL);
	minibmp_decode(spbdev.icos[SLCD.frame][sel].path,spbdev.icos[SLCD.frame][sel].x+5,spbdev.icos[SLCD.frame][sel].y,spbdev.icos[SLCD.frame][sel].width,spbdev.icos[SLCD.frame][sel].height,0,0);
 	gui_show_strmid(spbdev.icos[SLCD.frame][sel].x,spbdev.icos[SLCD.frame][sel].y+67,spbdev.icos[SLCD.frame][sel].width,16,SPB_FONT_COLOR,16,spbdev.icos[SLCD.frame][sel].name);//显示名字
}
//屏幕滑动及按键检测
//返回值:0~8,被双击的图标编号.		    
//       0xff,没有任何图标被双击或者按下
u8 spb_move_chk(void)
{		 
	u8 i=0xff;
	u8 movecnt=0;	//得到滑动点数
	tp_dev.scan(0);//扫描																		 
	if(tp_dev.sta&TP_PRES_DOWN)//有按键被按下
	{
		if(spbdev.spbsta&0X80)//已经被标记了
		{
			movecnt=spbdev.spbsta&0X3F;//得到滑动点数
			if(gui_disabs(spbdev.curxpos,tp_dev.x)>=SPB_MOVE_WIN)//移动大于等于SPB_MOVE_WIN个点
			{
				if(movecnt<SPB_MOVE_MIN/SPB_MOVE_WIN)spbdev.spbsta++;//点数增加1	
			}
		}			  		    
	 	spbdev.curxpos=tp_dev.x;				//记录当前坐标
	 	spbdev.curypos=tp_dev.y;				//记录当前坐标	
 		if((spbdev.spbsta&0X80)==0)				//按键第一次被按下
		{
			spbdev.spbsta=0;				 	//状态清零
			spbdev.spbsta|=1<<7;				//标记按下	    
			spbdev.oldxpos=tp_dev.x;			//记录按下时的坐标 
		}else if(spbdev.spbsta&0X40)			//有滑动
		{
			if(spbdev.oldxpos>tp_dev.x) 		//x左移,屏幕pos右移
			{
				if(SLCD.pos<240)SLCD.pos+=spbdev.oldxpos-tp_dev.x; 
				if(SLCD.pos>240)SLCD.pos=240;	//溢出了. 
			}else 								//右移,屏幕pos左移
			{			    
				if(SLCD.pos>0)SLCD.pos-=tp_dev.x-spbdev.oldxpos;
				if(SLCD.pos>240)SLCD.pos=0;		//溢出了.	   		  
			}	    
			spbdev.oldxpos=tp_dev.x;
			SLCD.show(SLCD.pos);
 		}else if((gui_disabs(spbdev.curxpos,spbdev.oldxpos)>=SPB_MOVE_MIN)&&(movecnt>=SPB_MOVE_MIN/SPB_MOVE_WIN))//滑动距离超过SPB_MOVE_MIN,并且检测到的有效滑动数不少于SPB_MOVE_MIN/SPB_MOVE_WIN.
		{
			spbdev.spbsta|=1<<6;//标记滑动	
		}   
	}else //按键松开了
	{
		if(spbdev.spbsta&0x80)//之前有按下
		{	  
			if(spbdev.spbsta&0X40)//有滑动
			{										    
				if(SLCD.frame==1)//原来在第二帧,只能右移 
				{	
					if((240-SLCD.pos)>SPB_MOVE_ACT)
					{
						SLCD.move(1,5,SLCD.pos);
						SLCD.pos=0;
					}else 
					{
						SLCD.move(0,5,SLCD.pos);
						SLCD.pos=240;
					}			 
				}else	   //原来在第一帧,只能左移  0开始
				{
					if(SLCD.pos>SPB_MOVE_ACT)
					{
						SLCD.move(0,5,SLCD.pos);
						SLCD.pos=240;
					}else 
					{	
						SLCD.move(1,5,SLCD.pos);
						SLCD.pos=0;
					}		   

				}
 				spbdev.selico=0xff;//取消spbdev.selico原先的设置  
			}else	//属于点按.
			{
				for(i=0;i<9;i++)
				{
					if((spbdev.curxpos>spbdev.icos[SLCD.frame][i].x)&&(spbdev.curxpos<spbdev.icos[SLCD.frame][i].x+spbdev.icos[SLCD.frame][i].width)&&(spbdev.curxpos>spbdev.icos[SLCD.frame][i].x)&&
					   (spbdev.curypos<spbdev.icos[SLCD.frame][i].y+spbdev.icos[SLCD.frame][i].height))
					{
						break;//得到选中的编号	
					}
				}
				if(i<9)
				{
					if(i!=spbdev.selico)//选中了不同的图标,切换图标
					{
						spb_set_sel(i);
						i=0xff;
					}else
					{
						spbdev.selico=0XFF;//发生了双击,重新复位selico.
					}
				}else i=0xff;//无效的点按.
			}
		} 							
		spbdev.spbsta=0;//清空标志
	}
	return i;

}







