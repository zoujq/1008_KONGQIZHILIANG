#include "radio.h"
#include "settings.h"
#include "calendar.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-收音机 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

#define RADIO_TITLE_COLOR   	0XFFFF		//收音机标题颜色	
#define RADIO_TITLE_BKCOLOR     0X0000		//收音机标题背景色	
#define RADIO_INFO_COLOR   	   	0X8410	 	//信息字体的颜色	
#define RADIO_MAIN_BKCOLOR    	0X18E3		//主背景色	
#define RADIO_FREQ_COLOR    	0X07FF		//频率颜色	


#if ICOS_LOCATION==ICOS_SD_CARD//存放在SD卡  
const u8* RADIO_DEMO_PIC="0:/SYSTEM/APP/RADIO/Demo.bmp";		//demo图片路径	    
const u8* RADIO_RSSI_PIC="0:/SYSTEM/APP/RADIO/RSSI.bmp";		//RSSI图片路径	      
const u8* RADIO_STERO_PIC="0:/SYSTEM/APP/RADIO/STERO.bmp";		//STERO图片路径	      
const u8* RADIO_MONO_PIC="0:/SYSTEM/APP/RADIO/MONO.bmp";		//MONO图片路径
	      
const u8* RADIO_PLAYR_PIC="0:/SYSTEM/APP/RADIO/PlayR.bmp";		//播放 松开
const u8* RADIO_PLAYP_PIC="0:/SYSTEM/APP/RADIO/PlayP.bmp";		//播放 按下
const u8* RADIO_PAUSER_PIC="0:/SYSTEM/APP/RADIO/PauseR.bmp";	//暂停 松开
const u8* RADIO_PAUSEP_PIC="0:/SYSTEM/APP/RADIO/PauseP.bmp";	//暂停 按下
//5个图片按钮的路径
const u8* RADIO_BTN_PIC_TBL[2][5]=  						
{
{
	"0:/SYSTEM/APP/RADIO/MenuR.bmp",
	"0:/SYSTEM/APP/RADIO/PrevR.bmp", 
	"0:/SYSTEM/APP/RADIO/PauseR.bmp", 
	"0:/SYSTEM/APP/RADIO/NextR.bmp",
	"0:/SYSTEM/APP/RADIO/BackR.bmp", 
},
{
	"0:/SYSTEM/APP/RADIO/MenuP.bmp",  
	"0:/SYSTEM/APP/RADIO/PrevP.bmp",  
	"0:/SYSTEM/APP/RADIO/PlayP.bmp",  
	"0:/SYSTEM/APP/RADIO/NextP.bmp", 
	"0:/SYSTEM/APP/RADIO/BackP.bmp",  
},
};
#else//存放在 FLASH DISK
const u8* RADIO_DEMO_PIC="1:/SYSTEM/APP/RADIO/Demo.bmp";		//demo图片路径	    
const u8* RADIO_RSSI_PIC="1:/SYSTEM/APP/RADIO/RSSI.bmp";		//RSSI图片路径	      
const u8* RADIO_STERO_PIC="1:/SYSTEM/APP/RADIO/STERO.bmp";		//STERO图片路径	      
const u8* RADIO_MONO_PIC="1:/SYSTEM/APP/RADIO/MONO.bmp";		//MONO图片路径	      
	      
const u8* RADIO_PLAYR_PIC="1:/SYSTEM/APP/RADIO/PlayR.bmp";		//播放 松开
const u8* RADIO_PLAYP_PIC="1:/SYSTEM/APP/RADIO/PlayP.bmp";		//播放 按下
const u8* RADIO_PAUSER_PIC="1:/SYSTEM/APP/RADIO/PauseR.bmp";	//暂停 松开
const u8* RADIO_PAUSEP_PIC="1:/SYSTEM/APP/RADIO/PauseP.bmp";	//暂停 按下
//5个图片按钮的路径
const u8* RADIO_BTN_PIC_TBL[2][5]=  						
{
{
	"1:/SYSTEM/APP/RADIO/MenuR.bmp",
	"1:/SYSTEM/APP/RADIO/PrevR.bmp", 
	"1:/SYSTEM/APP/RADIO/PauseR.bmp", 
	"1:/SYSTEM/APP/RADIO/NextR.bmp",
	"1:/SYSTEM/APP/RADIO/BackR.bmp", 
},
{
	"1:/SYSTEM/APP/RADIO/MenuP.bmp",  
	"1:/SYSTEM/APP/RADIO/PrevP.bmp",  
	"1:/SYSTEM/APP/RADIO/PlayP.bmp",  
	"1:/SYSTEM/APP/RADIO/NextP.bmp", 
	"1:/SYSTEM/APP/RADIO/BackP.bmp",  
},
}; 
#endif

//收音机功能设置
const u8* radio_set_tbl[GUI_LANGUAGE_NUM][2]=
{
{"模式设置","频段选择",},
{"模式設置","頻段選擇",},		 
{"Mode Set","Band Set",},
};
//收音机搜台模式
const u8* radio_mode_tbl[GUI_LANGUAGE_NUM][3]=
{
{"手动搜台","半自动搜台","全自动搜台",},
{"手動搜臺","半自動搜臺","全自動搜臺",},
{"Manual","Semi-Auto","Auto",},
};		 
//收音机频段选择
const u8* radio_band_tbl[GUI_LANGUAGE_NUM][3]=
{
{"欧美频段","日本频段","扩展频段",},
{"歐美頻段","日本頻段","擴展頻段",},
{"US/Europe","Japan","Japan Wide",},
};



//加载收音机主界面UI	  
void radio_load_ui(void)
{	
 	gui_fill_rectangle(0,0,240,20,RADIO_TITLE_BKCOLOR);					//填充黑色 
	gui_show_strmid(0,0,240,20,RADIO_TITLE_COLOR,16,(u8*)APP_MFUNS_CAPTION_TBL[7][gui_phy.language]);//显示标题
 	gui_fill_rectangle(0,20,240,300,RADIO_MAIN_BKCOLOR);				//填充底色 
 	minibmp_decode((u8*)RADIO_DEMO_PIC,20,165,100,64,0,0);				//解码64*64的图片DEMO
 	minibmp_decode((u8*)APP_VOL_PIC,103,165+10,16,16,0,0);				//解码音量图标
 
 	minibmp_decode((u8*)RADIO_STERO_PIC,22,32,16,16,0,0);				//解码立体声图标
 	minibmp_decode((u8*)RADIO_RSSI_PIC,240-28-20,32,16,16,0,0);			//解码信号强度图标
     
 	gui_show_string("CH:",120,165+45,66,12,12,RADIO_INFO_COLOR);  		//显示时间
   	gui_show_string("Mhz",30+135,70+36,24,16,16,RADIO_FREQ_COLOR);   	//显示时间
}
//收音机显示音量百分比
//pctx:0~100;
void radio_show_vol(u8 pctx)
{
	gui_phy.back_color=RADIO_MAIN_BKCOLOR;//设置背景色为底色
 	gui_fill_rectangle(120,195,24,12,RADIO_MAIN_BKCOLOR);//填充背景色 
	if(pctx==100)
	{
		gui_show_num(120,195,3,RADIO_INFO_COLOR,12,pctx,0x80);//显示音量百分比   
		gui_show_ptchar(120+18,195,120+18+6,210+12,0,RADIO_INFO_COLOR,12,'%',0);	//显示百分号
	}else 
	{
		gui_show_num(120,195,2,RADIO_INFO_COLOR,12,pctx,0x80);//显示音量百分比   
 		gui_show_ptchar(120+12,195,120+12+6,210+12,0,RADIO_INFO_COLOR,12,'%',0);	//显示百分号
  	}	 
}
//显示频率
//bfbase:大字体基址
//freq:760~1080.
void radio_show_freq(u8 *bfbase,u16 freq)
{
 	app_showbignum(bfbase,30,70,freq/10,3,60,RADIO_FREQ_COLOR,RADIO_MAIN_BKCOLOR);
	app_showbigchar(bfbase,30+90,70,'.',60,RADIO_FREQ_COLOR,RADIO_MAIN_BKCOLOR);	
	app_showbignum(bfbase,30+102,70,freq%10,1,60,RADIO_FREQ_COLOR,RADIO_MAIN_BKCOLOR);	    
}  
//显示频段范围
void radio_show_freqband(u8 min,u8 max)
{
  	gui_phy.back_color=RADIO_MAIN_BKCOLOR;							//设置背景色为底色
  	gui_show_num(21,140+11,2,RADIO_INFO_COLOR,12,min,0x00);			//显示最小频率
 	gui_show_num(240-20-18,140+11,3,RADIO_INFO_COLOR,12,max,0x00);	//显示最大频率  
}
//显示频段范围
void radio_show_freqch(u8 chx)
{
  	gui_phy.back_color=RADIO_MAIN_BKCOLOR;							//设置背景色为底色
  	gui_show_num(120+18,165+45,2,RADIO_INFO_COLOR,12,chx,0x80);    	//显示最当前频率电台编号
}
 
//频段设置处理
void radio_band_process(_progressbar_obj *fmprgb,u8 band,u16 *fmin,u16 *fmax)
{
	switch(band)
	{
		case 0://设置为欧美频段	    
	 		fmprgb->totallen=210;		//总长度
			fmprgb->curpos=0;			//当前位置	  
			*fmin=870;
			*fmax=1080;		  
			break;
		case 1://设置为日本频段
	 		fmprgb->totallen=150;		//总长度
			fmprgb->curpos=0;			//当前位置	  
			*fmin=760;
			*fmax=910;		  
			break;
		case 2://设置为扩展频段
	 		fmprgb->totallen=320;		//总长度
			fmprgb->curpos=0;			//当前位置	  
			*fmin=760;
			*fmax=1080;		  
			break;	
	}							  	
	radio_show_freqband(*fmin/10,*fmax/10);
 	progressbar_draw_progressbar(fmprgb);	//画进度条	
 	RDA5820_Band_Set(band);					//设置频段	 
	RDA5820_Freq_Set(*fmin*10);				//设置当前频率
}
//从指定的一个频率开始往下调频,直到找到下一个台或者到达最大频率了.
//bfbase:大字体的字库地址
//fmprgb:频率显示进度条
//startfreq:起始频率(单位:100Khz)
//minfreq,maxfreq:终止频率(单位:100Khz)
//mode:
//[7]:0,一定要搜索一遍;1,搜索到顶部就不再搜索.
//[6:1]:保留
//[0]:搜寻的方向;0,退步搜索;1,进步搜索;
//返回值:0,没找到真正的电台;1,找到了一个电台;
u8 radio_auto_tune(u8 *bfbase,_progressbar_obj *fmprgb,u16 *startfreq,u16 minfreq,u16 maxfreq,u8 mode)
{
	u8 res=0;
	u16 temp;
	u8 rssi=0;
	u16 tempfreq=*startfreq;//保存起始频率
 	while(1)
	{
		if(mode&0X01)	//向前找台
		{	  
			if(*startfreq<maxfreq)(*startfreq)++;   	//频率增加100Khz
			else if(mode&0X80)break;
			else *startfreq=minfreq;   
		}else			//向后找台
 		{
		 	if(*startfreq>minfreq)(*startfreq)--;   	//频率增加100Khz
			else if(mode&0X80)break;
			else *startfreq=maxfreq;   	    
		}		     
		RDA5820_Freq_Set(*startfreq*10);	   	//设置频率
		delay_ms(20);							//等待调频信号稳定
 		fmprgb->curpos=*startfreq-minfreq;
 		progressbar_draw_progressbar(fmprgb);	//画进度条		 
		radio_show_freq(bfbase,*startfreq);		//显示当前频率
		temp=RDA5820_RD_Reg(0X0B);
		rssi=temp>>9;
		if(temp&(1<<8)||rssi>70)				//是一个有效电台/信号电平较大. 
		{
			res=1;
			break;		 
		}
		if(tempfreq==*startfreq)			//找了个遍也没找到合格的电台
		{		 
			res=0;								
 			break;
		}
	}
	return res;
}
//收音机数据保存在:SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj)+sizeof(_alarm_obj)
//读取收音机数据
//radio:收音机信息
void radio_read_para(_m_radiodev * radio)
{
	AT24CXX_Read(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj)+sizeof(_alarm_obj),(u8*)radio,sizeof(_m_radiodev));
}
//写入收音机数据
//radio:收音机信息 
void radio_save_para(_m_radiodev * radio)
{
   	OS_CPU_SR cpu_sr=0;
	OS_ENTER_CRITICAL();	//进入临界区(无法被中断打断) 
	AT24CXX_Write(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj)+sizeof(_alarm_obj),(u8*)radio,sizeof(_m_radiodev));
	OS_EXIT_CRITICAL();		//退出临界区(可以被中断打断)
}

//收音机
u8 radio_play(void)
{
	u8 i;
	u8 res;
	u8 tcnt=0;
	u8 rval=0;		//1,内存错误;2,返回,MP3继续播放;3,返回,停止MP3播放.
	u8 stero=0;		//立体声标志 
	u8 mode=0;		//模式,默认为手动调台模式
	u16 temp;
	u8 savemask=0;	//保存标记
	static u16 curfreq=0;//当前频率
   
	_progressbar_obj* fmprgb,*volprgb;	//频率进度条和声音进度条
	_btn_obj* tbtn[5];					//5个主按钮	
	_m_radiodev *fmdev;

	u8 pause=0;							//暂停标志.0,不暂停;1,暂停.
	u16 fmfreq=0;						//当前FM频率
	u16 fmin=0,fmax=0;					//当前FM频率的最大最小值
	u16 *fmbkctbl=0;								
					  
//////////////////////////////////////////////////////////////////////////////
//本部分加载字体
	FIL* f_radio=0;	 
	u8 *bfbase=0;		//大字库的基址		 

	fmdev=(_m_radiodev*)gui_memin_malloc(sizeof(_m_radiodev));//开辟_m_radiodev字节的内存区域 
	if(fmdev==NULL)rval=1;
	else
	{
 		radio_read_para(fmdev);			//读取收音机数据
 	}
  	f_radio=(FIL *)gui_memin_malloc(sizeof(FIL));//开辟FIL字节的内存区域 
	if(f_radio==NULL)rval=1;			//申请失败
	else
	{
		res=f_open(f_radio,(const TCHAR*)APP_ASCII_60,FA_READ);//打开文件夹
		if(res==FR_OK)
		{
			bfbase=(u8*)gui_memex_malloc(f_radio->fsize);	//为大字体开辟缓存地址
			if(bfbase==0)rval=1;
			else 
			{
				res=f_read(f_radio,bfbase,f_radio->fsize,(UINT*)&br);	//一次读取整个文件
 			}
		} 
		if(res)rval=1;
	} 	    
/////////////////////////////////////////////////////////////////////////////////
  	if(curfreq==0)//第一次开机进入收音机,需要初始化一下
	{
		if(RDA5820_Init())rval=1;						//初始化FM模块失败
	}
	fmprgb=progressbar_creat(20,140,200,10,0X40);		//FM播放进度条
	if(fmprgb==NULL)rval=1;
	volprgb=progressbar_creat(120,165+15,100,6,0X20);	//声音大小进度条				    
  	if(volprgb==NULL)rval=1;
	fmbkctbl=(u16*)gui_memex_malloc(180*192*2);			//为背景存储申请内存
  	if(fmbkctbl==NULL)rval=1;
   
 	for(i=0;i<5;i++)//循环创建5个按钮
	{
		tbtn[i]=btn_creat(0+i*48,271,48,48,0,1);			//创建图片按钮
		if(tbtn[i]==NULL){rval=1;break;}					//创建失败.
		tbtn[i]->picbtnpathu=(u8*)RADIO_BTN_PIC_TBL[0][i];
		tbtn[i]->picbtnpathd=(u8*)RADIO_BTN_PIC_TBL[1][i];
 		tbtn[i]->sta=0;	 
	}
	if(rval==0)//没有错误
	{
 	   	fmprgb->inbkcolora=0x738E;				//默认色
	 	fmprgb->inbkcolorb=RADIO_INFO_COLOR;	//默认色    
		fmprgb->prgbarlen=3;					//浮标长度2个单位.
 		
		fmprgb->totallen=320;					//总长度
		fmprgb->curpos=0;			 			//当前位置
 		fmprgb->btncolor=BLUE;					//默认色  
 
	   	volprgb->inbkcolora=RADIO_INFO_COLOR;	//默认色
	 	volprgb->inbkcolorb=RADIO_INFO_COLOR; 	//默认色 
	 	volprgb->infcolora=0X75D;				//默认色
	 	volprgb->infcolorb=0X596;				//默认色 	   						    						  		
		volprgb->curpos=fmdev->vol;				//设定最近的位置
 		volprgb->totallen=15;					//音量范围:0~15	
							 	  					   
		mode=(fmdev->sta>>2)&0X03;				//得到之前设置好的模式

		radio_load_ui();//加载主界面
		radio_show_vol((volprgb->curpos*100)/volprgb->totallen);	//显示音量百分比 
		for(i=0;i<5;i++)btn_draw(tbtn[i]);			//画按钮
		tbtn[2]->picbtnpathu=(u8*)RADIO_PLAYR_PIC;	//按下一次之后变为播放松开状态

 		RDA5820_RX_Mode();					//设置到接收模式
		RDA5820_Rssi_Set(fmdev->vol);	  	//设置音量
		RDA5820_Space_Set(0);				//设置步进频率为100Khz
 		radio_band_process(fmprgb,fmdev->sta&0X03,&fmin,&fmax);//设置频段,并显示部分信息
		if(mode!=2)//非全自动搜台模式,直接设置到最低频率
		{
			if(curfreq<fmin||curfreq>fmax)curfreq=fmin;//判断curfreq数据合法性
			fmfreq=curfreq;
		}else//全自动搜台,直接设置到上次选中的频率
		{
			fmfreq=fmdev->chtbl[fmdev->curch];
		}
		RDA5820_Freq_Set(fmfreq*10);			//设置当前频率
		radio_show_freq(bfbase,fmfreq);			//显示频率
 		fmprgb->curpos=fmfreq-fmin;				//进度条位置
  		progressbar_draw_progressbar(fmprgb);	//画进度条	 
		progressbar_draw_progressbar(volprgb);	//画进度条
		tcnt=0;									 
  		radio_show_vol((volprgb->curpos*100)/volprgb->totallen);//显示音量百分比 
  		Audiosel_Set(1);					//设置为音频输出为收音机音频
		radio_show_freqch(fmdev->curch);	//显示电台编号
  		while(rval==0)
		{
			tcnt++;//计时增加.
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(10);							//延时10ms
 			if(system_task_return)break;			//TPAD返回
	 		for(i=0;i<5;i++)
			{
				res=btn_check(tbtn[i],&in_obj);
				if(res&&((tbtn[i]->sta&(1<<7))==0)&&(tbtn[i]->sta&(1<<6)))//有按键按下且松开,并且TP松开了
				{
					switch(i)
					{
						case 0://弹出功能选项
	 						app_read_bkcolor(30,40,180,192,fmbkctbl);//读取背景色
							i=0;
							//弹出对话框	 
							res=app_items_sel(30,60,180,72+40*2,(u8**)radio_set_tbl[gui_phy.language],2,(u8*)&i,0X90,(u8*)APP_MODESEL_CAPTION_TBL[gui_phy.language]);//2个选择
	 						if(res==0)//确认键按下
							{
								if(i==0)	//设置模式
								{
									i=(fmdev->sta>>2)&0X03;
									temp=i;//记录当前模式
									res=app_items_sel(30,40,180,72+40*3,(u8**)radio_mode_tbl[gui_phy.language],3,(u8*)&i,0X90,(u8*)radio_set_tbl[gui_phy.language][0]);//3个选择
									app_recover_bkcolor(30,40,180,192,fmbkctbl);//恢复背景色  
	 								if(res==0)
									{
 										mode=i;					//得新模式
										fmdev->sta&=0XF3;		//清空之前的设置
										fmdev->sta|=0X0C&(i<<2);//更新模式
 										if(mode==2)//全自动搜台
										{
											savemask=1;			//标记数据变化了
											fmdev->curch=0;		//清空
											fmdev->totalch=0;	//清空
											fmfreq=fmin;		
 											while(1)
											{				    	   
												res=radio_auto_tune(bfbase,fmprgb,&fmfreq,fmin,fmax,0X81);//向前搜台.搜到顶点就结束
												if(res)
												{
													fmdev->chtbl[fmdev->totalch]=fmfreq;//记录这个频率
													if(fmdev->totalch<RADIO_MAX_CH_VAL-1)fmdev->totalch++;//总数增加1
												}
												if(fmfreq==fmax)
												{			 
													if(fmdev->totalch)
													{
														fmdev->curch=fmdev->totalch-1;
						 								fmfreq=fmdev->chtbl[fmdev->curch];
													}else fmfreq=fmin;
													radio_show_freq(bfbase,fmfreq);	    
													fmprgb->curpos=fmfreq-fmin;
													progressbar_draw_progressbar(fmprgb);	//画进度条		 
													RDA5820_Freq_Set(fmfreq*10);			//设置卑频率
													radio_show_freqch(fmdev->curch);						 
													break; 	
												}		   
	 										}
										}else if(temp!=mode)//模式改变了
										{
											savemask=1;				//标记数据变化了
											radio_show_freqch(0);	//CH设置为0
										} 
									}
								}else 		//设置频段
								{
									i=fmdev->sta&0X03;//取得上次的设置
									temp=i;//记录之前的频段
 									res=app_items_sel(30,40,180,72+40*3,(u8**)radio_band_tbl[gui_phy.language],3,(u8*)&i,0X90,(u8*)radio_set_tbl[gui_phy.language][1]);//3个选择
									app_recover_bkcolor(30,40,180,192,fmbkctbl);//恢复背景色  
	 								if(res==0&&temp!=i)//频段变更了
									{
										savemask=1;		 	//标记数据变化了
										fmdev->curch=0;		//当前频率号清零
										fmdev->totalch=0;	//有效频率数清零
										fmdev->sta&=0XFC;
 										fmdev->sta|=i;		//更新频段	  
										radio_band_process(fmprgb,i,&fmin,&fmax);//设置频段,并显示部分信息
										fmfreq=fmin;
										radio_show_freq(bfbase,fmfreq);	//显示频率
 									}
								}
							}else app_recover_bkcolor(30,40,180,192,fmbkctbl);//恢复背景色  
 							break;
						case 1://调节频率
						case 3:	
							switch(mode)
							{   
								case 0://手动调台模式	 
									if(i==1)
									{
										if(fmfreq>fmin)fmfreq--;
										else fmfreq=fmax;
									}else 
									{
										fmfreq++;
										if(fmfreq>fmax)fmfreq=fmin;
									}
									break;
								case 1://半自动调台模式
									if(i==1)//向后调台
									{				    	   
										res=radio_auto_tune(bfbase,fmprgb,&fmfreq,fmin,fmax,0);		   
 									}else	//向前调台
									{
										res=radio_auto_tune(bfbase,fmprgb,&fmfreq,fmin,fmax,1);		   
									}	    
									break;
								case 2://全自动调台模式
									if(fmdev->totalch>0)//至少要有一个电台.
									{
										savemask=1;//标记数据变化了
										if(i==1)//向后调台
										{		
 											if(fmdev->curch)fmdev->curch--;
											else fmdev->curch=fmdev->totalch-1; 
	 									}else	//向前调台
										{
											if(fmdev->curch<(fmdev->totalch-1))fmdev->curch++;
											else fmdev->curch=0;		    	   
										}
										fmfreq=fmdev->chtbl[fmdev->curch];	  
										radio_show_freqch(fmdev->curch);  
									}
								 	break;
							}	 
 							radio_show_freq(bfbase,fmfreq);
							fmprgb->curpos=fmfreq-fmin;
							progressbar_draw_progressbar(fmprgb);	//画进度条		 
							RDA5820_Freq_Set(fmfreq*10);			//设置当前频率
							break;
						case 2://播放/暂停
							if(pause)//切换到播放
							{
								pause=0;//取消暂停	    
								tbtn[2]->picbtnpathd=(u8*)RADIO_PLAYP_PIC; 
								tbtn[2]->picbtnpathu=(u8*)RADIO_PAUSER_PIC;
							}else	//切换到暂停状态
							{
								pause=1;//标记暂停
								tbtn[2]->picbtnpathd=(u8*)RADIO_PAUSEP_PIC; 
								tbtn[2]->picbtnpathu=(u8*)RADIO_PLAYR_PIC;
							}
							RDA5820_Mute_Set(pause);
							break;
						case 4://停止播放/返回	
							rval=1;  
							break;

					}   
				}
			}
			res=progressbar_check(volprgb,&in_obj);	//检查音量进度条
			if(res&&fmdev->vol!=volprgb->curpos)	//被按下了,且位置变化了.执行音量调整
			{	
				savemask=1;		 					//标记数据变化了	  
				fmdev->vol=volprgb->curpos&0X0F;	   								 
				RDA5820_Vol_Set(fmdev->vol);		//设置音量						   
  				radio_show_vol((volprgb->curpos*100)/volprgb->totallen);	//显示音量百分比   
 			}	  
			if((tcnt%250)==0)//更新显示信息,每2.5s执行一次
			{
				tcnt=0;
 				temp=RDA5820_RD_Reg(0X0A);		//读取0X0A的内容
 				temp=(temp>>10)&0X01;
				if(stero!=temp)
				{
					stero=temp;
			 		gui_fill_rectangle(22,32,16,16,RADIO_MAIN_BKCOLOR);//填充背景色 
					if(stero)//是立体声
					{
				 		minibmp_decode((u8*)RADIO_STERO_PIC,22,32,16,16,0,0);	//解码立体声图标
					}else
					{
				 		minibmp_decode((u8*)RADIO_MONO_PIC,22,32,16,16,0,0);	//解码单声道图标
					}
				}
				temp=RDA5820_RD_Reg(0X0B);		//读取0X0B的内容
			    temp>>=9;
			  	gui_phy.back_color=RADIO_MAIN_BKCOLOR;//设置背景色为底色
			 	gui_fill_rectangle(240-12-20,36,12,12,RADIO_MAIN_BKCOLOR);	//填充背景色 
				gui_show_num(240-12-20,36,2,RADIO_INFO_COLOR,12,temp,0x80);	//显示音量百分比
			}
 		}
	}
	if(savemask)
	{
		printf("fm save!\r\n");
		radio_save_para(fmdev);			//保存收音机数据 
	}
	if(systemset.fmrxmode==0)//不支持后台播放
	{
		if(systemset.fmtxmode==1)//需要设置发射模式
		{
	 		RDA5820_TX_Mode();							//设置为发送模式 
			RDA5820_Freq_Set(systemset.fmtxfreq*10);	//设置到systemset.fmtxfreq的频率值
			RDA5820_TxPGA_Set(3); 						//设置增益为3
 		}
		if(systemset.audiosel==1)systemset.audiosel=3;	//无声通道
		Audiosel_Set(systemset.audiosel);				//设置回原来的声道  
	}else systemset.audiosel=1;							//默认通道变为收音机
 	curfreq=fmfreq;										//保存退出时的频率
	for(i=0;i<5;i++)btn_delete(tbtn[i]);//删除按钮	
	progressbar_delete(fmprgb);
	progressbar_delete(volprgb);
	gui_memin_free(f_radio);
	gui_memin_free(fmdev);
	gui_memex_free(bfbase);
	gui_memex_free(fmbkctbl);
 	return rval;
}
































