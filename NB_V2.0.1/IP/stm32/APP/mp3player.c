#include "includes.h"		 
#include "mp3player.h"
#include "settings.h"
#include "ucos_ii.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-音乐播放器实现 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//mp3播放控制器
_m_mp3dev  *mp3dev=NULL;
_lyric_obj *lrcdev=NULL;	//歌词控制器

////////////////////////////////////////////////////////
#if ICOS_LOCATION==ICOS_SD_CARD//存放在SD卡  
const u8 *FLAC_PATCH_PATH="0:/SYSTEM/APP/MP3/FLAC.BIN";			//FLAC播放的PATCH的存放位置
const u8 *SPEC_PATCH_PATH="0:/SYSTEM/APP/MP3/SPCALZ.BIN";		//频谱分析的PATCH的存放位置
const u8* MP3_DEMO_PIC="0:/SYSTEM/APP/MP3/Demo.bmp";			//demo图片路径
const u8* MP3_BTN_PIC_TBL[2][5]=  								//5个图片按钮的路径
{
{
	"0:/SYSTEM/APP/MP3/ListR.bmp",
	"0:/SYSTEM/APP/MP3/PrevR.bmp", 
	"0:/SYSTEM/APP/MP3/PauseR.bmp", 
	"0:/SYSTEM/APP/MP3/NextR.bmp",
	"0:/SYSTEM/APP/MP3/BackR.bmp", 
},
{
	"0:/SYSTEM/APP/MP3/ListP.bmp",  
	"0:/SYSTEM/APP/MP3/PrevP.bmp",  
	"0:/SYSTEM/APP/MP3/PlayP.bmp",  
	"0:/SYSTEM/APP/MP3/NextP.bmp", 
	"0:/SYSTEM/APP/MP3/BackP.bmp",  
},
};
const u8* MP3_PLAYR_PIC="0:/SYSTEM/APP/MP3/PlayR.bmp";		//播放 松开
const u8* MP3_PLAYP_PIC="0:/SYSTEM/APP/MP3/PlayP.bmp";		//播放 按下
const u8* MP3_PAUSER_PIC="0:/SYSTEM/APP/MP3/PauseR.bmp";	//暂停 松开
const u8* MP3_PAUSEP_PIC="0:/SYSTEM/APP/MP3/PauseP.bmp";	//暂停 按下
#else//存放在FLASH DISK
const u8 *FLAC_PATCH_PATH="1:/SYSTEM/APP/MP3/FLAC.BIN";			//FLAC播放的PATCH的存放位置
const u8 *SPEC_PATCH_PATH="1:/SYSTEM/APP/MP3/SPCALZ.BIN";		//频谱分析的PATCH的存放位置
const u8* MP3_DEMO_PIC="1:/SYSTEM/APP/MP3/Demo.bmp";			//demo图片路径
const u8* MP3_BTN_PIC_TBL[2][5]=  								//5个图片按钮的路径
{
{
	"1:/SYSTEM/APP/MP3/ListR.bmp",
	"1:/SYSTEM/APP/MP3/PrevR.bmp", 
	"1:/SYSTEM/APP/MP3/PauseR.bmp", 
	"1:/SYSTEM/APP/MP3/NextR.bmp",
	"1:/SYSTEM/APP/MP3/BackR.bmp", 
},
{
	"1:/SYSTEM/APP/MP3/ListP.bmp",  
	"1:/SYSTEM/APP/MP3/PrevP.bmp",  
	"1:/SYSTEM/APP/MP3/PlayP.bmp",  
	"1:/SYSTEM/APP/MP3/NextP.bmp", 
	"1:/SYSTEM/APP/MP3/BackP.bmp",  
},
};
const u8* MP3_PLAYR_PIC="1:/SYSTEM/APP/MP3/PlayR.bmp";		//播放 松开
const u8* MP3_PLAYP_PIC="1:/SYSTEM/APP/MP3/PlayP.bmp";		//播放 按下
const u8* MP3_PAUSER_PIC="1:/SYSTEM/APP/MP3/PauseR.bmp";	//暂停 松开
const u8* MP3_PAUSEP_PIC="1:/SYSTEM/APP/MP3/PauseP.bmp";	//暂停 按下
#endif 

//播放音乐任务
OS_EVENT * mp3mbox;//事件控制块	   	   
void music_play_task(void *pdata)
{	 
   	DIR mp3dir;			//mp3dir专用	  
 	FILINFO mp3info;	 	   
    u16 br;
	u8 res,rval;	  
	u8 *databuf;
	u8 *patchbuf=0;	  		   
	u16 i=0;   
	u8 *pname=0;		   
 		    
 	mp3mbox=OSMboxCreate((void*) 0);//创建邮箱
//	VS_HD_Reset();
//	VS_Soft_Reset();  	//软复位VS1053
 	while(1)
	{
		mp3dev->curindex=(u32)OSMboxPend(mp3mbox,0,&rval)-1;//请求邮箱,要减去1,因为发送的时候增加了1
		rval=0;
		databuf=(u8*)mymalloc(SRAMIN,4096);		//开辟512字节的内存区域
		if(databuf==NULL)rval=1 ;				//内存申请失败.
 	
		//为长文件名申请缓存区
	 	mp3info.lfsize = _MAX_LFN * 2 + 1;
		mp3info.lfname = gui_memin_malloc(mp3info.lfsize);
		if(mp3info.lfname==NULL)rval=1;//申请内存失败 
	   	else gui_memset((u8 *)mp3info.lfname,0,mp3info.lfsize);
		if(rval==0)rval=f_opendir(&mp3dir,(const TCHAR*)mp3dev->path); //打开选中的目录
  		while(rval==0)
		{	  	 			   
			dir_sdi(&mp3dir,mp3dev->mfindextbl[mp3dev->curindex]);
	 		rval=f_readdir(&mp3dir,&mp3info);//读取文件信息
			if(rval)break;//打开成功    
 			mp3dev->name=(u8*)(*mp3info.lfname?mp3info.lfname:mp3info.fname);
			pname=gui_memin_malloc(strlen((const char*)mp3dev->name)+strlen((const char*)mp3dev->path)+2);//申请内存
			if(pname==NULL)break;//申请失败	    
			pname=gui_path_name(pname,mp3dev->path,mp3dev->name);	//文件名加入路径  			 			  	 
 			VS_Restart_Play();  	//重启播放 
			VS_Set_All();        	//设置音量等信息 			 
			VS_Reset_DecodeTime();	//复位解码时间 
			res=f_typetell(pname);
///////////////////////////////////////////////////////////////////////////////////
			if(res==0x4c)//flac
			{	
				res=f_open(mp3dev->fmp3,(const TCHAR*)FLAC_PATCH_PATH,FA_READ);//打开文件
				mp3dev->sta|=1<<4;//标记为flac文件
			}else
			{
				res=f_open(mp3dev->fmp3,(const TCHAR*)SPEC_PATCH_PATH,FA_READ);//打开文件
				mp3dev->sta&=~(1<<4);//标记为普通文件
			}	 
			if(res)break;//打开flac patch错误
			patchbuf=(u8*)mymalloc(SRAMEX,mp3dev->fmp3->fsize);	//开辟fsize字节的内存区域
			if(patchbuf==NULL)break;//申请内存失败
			res=f_read(mp3dev->fmp3,patchbuf,mp3dev->fmp3->fsize,(UINT*)&br);	//一次读取整个文件
		   	if(res==0)
			{
				VS_Load_Patch((u16*)patchbuf,mp3dev->fmp3->fsize/2);
			}
			myfree(SRAMEX,patchbuf);
			patchbuf=NULL;
			f_close(mp3dev->fmp3);	 
///////////////////////////////////////////////////////////////////////////////////
			res=f_open(mp3dev->fmp3,(const TCHAR*)pname,FA_READ);//打开文件	 
			gui_memin_free(pname);
 			pname=NULL;
			if(res==0)//打开成功.
			{ 
				VS_SPI_SpeedHigh();	//高速
				mp3dev->sta|=1<<7;	//标记开始解码MP3
 				mp3dev->sta|=1<<6;	//标记执行了一次歌曲的切换	   
				while(1)
				{
					res=f_read(mp3dev->fmp3,databuf,4096,(UINT*)&br);	//读出readlen个字节  
					i=0;
					do//主播放循环
				    {  	
						if(VS_Send_MusicData(databuf+i)==0)//给VS10XX发送音频数据
						{
							i+=32;
						}else   
						{
							delay_ms(5);//允许调度	   
							while(mp3dev->sta&(1<<5))//如果请求暂停了
							{
								delay_ms(5);//允许调度,同时等待非暂停
								if((mp3dev->sta&0x01)==0)break;//请求终止		 
							}
							if((mp3dev->sta&0x01)==0)break;//请求终止
								
						}	    	    
					}while(i<4096);//循环发送4096个字节 
					if(br!=4096||res!=0)
					{
						break;//读完了.
					}
					if((mp3dev->sta&0x01)==0)break;//请求终止
				}
				f_close(mp3dev->fmp3);
			}	  
			if(mp3dev->sta&0x01)//允许播放的条件下,循环播放
			{
				if(systemset.mp3mode==0)//顺序播放
				{
					if(mp3dev->curindex<(mp3dev->mfilenum-1))mp3dev->curindex++;
					else mp3dev->curindex=0;
				}else if(systemset.mp3mode==1)//随机播放
				{						    
					mp3dev->curindex=app_get_rand(mp3dev->mfilenum);//得到下一首歌曲的索引	  
				}else mp3dev->curindex=mp3dev->curindex;//单曲循环				
 			}else break;	  
		}
		gui_memin_free(pname);
	    gui_memin_free(mp3info.lfname);
		myfree(SRAMEX,patchbuf);	 
		myfree(SRAMIN,databuf);	  	  
 		mp3dev->sta=0;//播放停止
	}	 	    
}
///////////////////////////////////////////////////////////////////////////////////////////
//停止MP3播放
//mp3devx:MP3结构体
void mp3_stop(_m_mp3dev *mp3devx)
{
	mp3devx->sta&=~(1<<0);		//请求终止播放
	while(mp3devx->sta&0X80);	//等待播放停止
	mp3devx->sta|=1<<0;			//允许播放  
}  

//音乐列表
const u8* MUSIC_LIST[GUI_LANGUAGE_NUM]=
{
	"音乐列表","音樂列表","MUSIC LIST",
};
 
//MP3文件浏览,带文件存储功能
//mp3devx:MP3结构体
//返回值:0,正常返回/按了退出按钮.
//		 1,内存分配失败		 
u8 mp3_filelist(_m_mp3dev *mp3devx)
{
	FILINFO mp3info;	 	   
	u8 res;
	u8 rval=0;			//返回值	  
  	u16 i;	    						   
 
 	_btn_obj* rbtn;		//返回按钮控件
 	_btn_obj* qbtn;		//退出按钮控件

   	_filelistbox_obj * flistbox;
	_filelistbox_list * filelistx; 	//文件
  			    
	app_filebrower((u8*)MUSIC_LIST[gui_phy.language],0X07);	//选择目标文件,并得到目标数量
   	flistbox=filelistbox_creat(0,20,240,280,1,12);		//创建一个filelistbox
	if(flistbox==NULL)rval=1;							//申请内存失败.
	else if(mp3devx->path==NULL)  
	{
		flistbox->fliter=FLBOX_FLT_MUSIC;	//查找音乐文件
 		filelistbox_addlist(flistbox,(u8*)APP_DISK_NAME_TBL[0][gui_phy.language],0);		//磁盘0
		filelistbox_addlist(flistbox,(u8*)APP_DISK_NAME_TBL[1][gui_phy.language],0);		//磁盘1
		filelistbox_draw_listbox(flistbox);
	}else
	{
		flistbox->fliter=FLBOX_FLT_MUSIC;		//查找音乐文件	 
		flistbox->path=(u8*)gui_memin_malloc(strlen((const char*)mp3devx->path)+1);//为路径申请内存
		strcpy((char *)flistbox->path,(char *)mp3devx->path);//复制路径	    
		filelistbox_scan_filelist(flistbox);	//重新扫描列表 
		flistbox->selindex=flistbox->foldercnt+mp3devx->curindex;//选中条目为当前正在播放的条目
		if(flistbox->scbv->totalitems>flistbox->scbv->itemsperpage)flistbox->scbv->topitem=flistbox->selindex;  
		filelistbox_draw_listbox(flistbox);		//重画 		 
	} 	 		 
	//为长文件名申请缓存区
 	mp3info.lfsize = _MAX_LFN * 2 + 1;
	mp3info.lfname = gui_memin_malloc(mp3info.lfsize);
	if(mp3info.lfname==NULL)rval=1;//申请内存失败 
   	else gui_memset((u8 *)mp3info.lfname,0,mp3info.lfsize);
 
	rbtn=btn_creat(199,300,40,19,0,0x03);	//创建返回文字按钮
 	qbtn=btn_creat(0,300,40,19,0,0x03);		//创建退出文字按钮
	if(rbtn==NULL||qbtn==NULL)rval=1;	//没有足够内存够分配
	else
	{
	 	rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];	//返回
	 	rbtn->font=16;			//字体		 
		rbtn->bcfdcolor=WHITE;	//按下时的颜色
		rbtn->bcfucolor=WHITE;	//松开时的颜色
		btn_draw(rbtn);//画按钮

	 	qbtn->caption=(u8*)GUI_QUIT_CAPTION_TBL[gui_phy.language];	//名字
	 	qbtn->font=16;			//字体		 
		qbtn->bcfdcolor=WHITE;	//按下时的颜色
		qbtn->bcfucolor=WHITE;	//松开时的颜色
		btn_draw(qbtn);//画按钮
	}	   
   	while(rval==0)
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
		delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
		if(system_task_return)break;			//TPAD返回
		filelistbox_check(flistbox,&in_obj);	//扫描文件
		res=btn_check(rbtn,&in_obj);
		if(res)
		{
			if(((rbtn->sta&0X80)==0))//按钮状态改变了
			{
				if(flistbox->dbclick!=0X81)
				{
 					filelistx=filelist_search(flistbox->list,flistbox->selindex);//得到此时选中的list的信息
					if(filelistx->type==FICO_DISK)//已经不能再往上了
					{				 
						break;
					}else filelistbox_back(flistbox);//退回上一层目录	 
				} 
 			}	 
		}
		res=btn_check(qbtn,&in_obj);
		if(res)
		{
			if(((qbtn->sta&0X80)==0))//按钮状态改变了
			{ 
				break;//退出
 			}	 
		}   
		if(flistbox->dbclick==0X81)//双击文件了
		{											 
			mp3_stop(mp3devx);
			gui_memin_free(mp3devx->path);		//释放内存
			gui_memex_free(mp3devx->mfindextbl);	//释放内存
			mp3devx->path=(u8*)gui_memin_malloc(strlen((const char*)flistbox->path)+1);//为新的路径申请内存
			if(mp3devx->path==NULL){rval=1;break;}
			mp3devx->path[0]='\0';//在最开始加入结束符.
 			strcpy((char *)mp3devx->path,(char *)flistbox->path);
			mp3devx->mfindextbl=(u16*)gui_memex_malloc(flistbox->filecnt*2);//为新的tbl申请内存
			if(mp3devx->mfindextbl==NULL){rval=1;break;}
		    for(i=0;i<flistbox->filecnt;i++)mp3devx->mfindextbl[i]=flistbox->findextbl[i];//复制

			mp3devx->mfilenum=flistbox->filecnt;		//记录文件个数	  
			OSMboxPost(mp3mbox,(void*)(flistbox->selindex-flistbox->foldercnt+1));//发送邮箱,因为邮箱不能为空,所以在这必须加1
 			flistbox->dbclick=0;
			break;	 							   			   
		}
	}	
	filelistbox_delete(flistbox);	//删除filelist
	btn_delete(qbtn);				//删除按钮	  	 
	btn_delete(rbtn);				//删除按钮	   
  	gui_memin_free(mp3info.lfname);	
	if(rval)
	{
		gui_memin_free(mp3devx->path);		 //释放内存
		gui_memex_free(mp3devx->mfindextbl); //释放内存
		gui_memin_free(mp3devx);
 	}	    
	return rval; 
}

const u16 MP3_SPEC_BASE_CTBL[6]={0X6B6D,0X632C,0X5ACB,0X4A69,0X31A6,0X18E3};//频谱显示底部的颜色块
//MP3加载主界面
void mp3_load_ui(void)
{	
	u8 i;									   	  
	gui_fill_rectangle(0,0,240,20,MP3_TITLE_BKCOLOR);					//填充黑色 
	gui_show_strmid(0,0,240,20,MP3_TITLE_COLOR,16,(u8*)APP_MFUNS_CAPTION_TBL[2][gui_phy.language]);//显示标题
 	gui_fill_rectangle(0,20,240,300,MP3_MAIN_BKCOLOR);					//填充底色 
 	minibmp_decode((u8*)MP3_DEMO_PIC,20,150,100,64,0,0);				//解码64*64的图片DEMO
 	minibmp_decode((u8*)APP_VOL_PIC,103,150+10,16,16,0,0);				//解码音量图标
	gui_show_string("kbps",178+18,133,24,12,12,MP3_INFO_COLOR);			//显示kbps
 
  	gui_show_string("00%",120,178,66,12,12,MP3_INFO_COLOR);		//显示时间
	gui_show_string("00:00/00:00",21,133,66,12,12,MP3_INFO_COLOR);		//显示时间
 
 	gui_fill_colorblock(20,105,200,6,(u16*)MP3_SPEC_BASE_CTBL,0);		//纵向填充
	for(i=3;i<200;i+=4)
	{
		gui_draw_vline(20+i,105,6,MP3_MAIN_BKCOLOR);//填充间隔线
	}															  
}
//显示音量百分比 
void mp3_show_vol(u8 pctx)
{
	gui_phy.back_color=MP3_MAIN_BKCOLOR;//设置背景色为底色
 	gui_fill_rectangle(120,178,24,12,MP3_MAIN_BKCOLOR);//填充背景色 
	if(pctx==100)
	{
		gui_show_num(120,178,3,MP3_INFO_COLOR,12,pctx,0x80);//显示音量百分比   
		gui_show_ptchar(120+18,178,120+18+6,178+12,0,MP3_INFO_COLOR,12,'%',0);	//显示百分号
	}else 
	{
		gui_show_num(120,178,2,MP3_INFO_COLOR,12,pctx,0x80);//显示音量百分比   
 		gui_show_ptchar(120+12,178,120+12+6,178+12,0,MP3_INFO_COLOR,12,'%',0);	//显示百分号
  	}	 
}  
//显示MP3播放时间
//sx,sy:起始坐标
//sec:时间
void mp3_time_show(u16 sx,u16 sy,u16 sec)
{
	u16 min;
	min=sec/60;//得到分钟数
	if(min>99)min=99;
	sec=sec%60;//得到秒钟数

	gui_phy.back_color=MP3_MAIN_BKCOLOR;//设置背景色为底色
	gui_show_num(sx,sy,2,MP3_INFO_COLOR,12,min,0x80);//显示时间
	gui_show_ptchar(sx+12,sy,239,319,0,MP3_INFO_COLOR,12,':',0);//显示冒号
	gui_show_num(sx+18,sy,2,MP3_INFO_COLOR,12,sec,0x80);//显示时间	  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//显示一根柱子
//x,y,width,height:位置和尺寸
//curval:当值
//topval:最大值
void fft_show_oneband(u16 x,u16 y,u16 width,u16 height,u16 curval,u16 topval)
{  
	gui_fill_rectangle(x,y,width,height-curval,MP3_MAIN_BKCOLOR);					//填充背景色
	gui_fill_rectangle(x,y+height-curval,width,curval,FFT_BANDS_COLOR);	//填充柱状色
	gui_draw_hline(x,y+height-topval-1,width,FFT_TOP_COLOR);	
}
//显示FFT_BANDS根柱子
//mp3devx:MP3结构体
//pdt:频谱数据
void mp3_specalz_show(_m_mp3dev *mp3devx,u16 *pdt)
{
	u8 i=0;
	u8 temp;								   
	for(i=0;i<FFT_BANDS;i++)	//显示各个频谱
	{
		temp=(pdt[i]&0X3F)*2;	//得到curval值,并乘2倍
		if(mp3devx->fft_curval_tbl[i]<temp)//当前值小于temp了
		{
			mp3devx->fft_curval_tbl[i]=temp;
 		}else
		{
			if(mp3devx->fft_curval_tbl[i]>1)mp3devx->fft_curval_tbl[i]-=2;
			else mp3devx->fft_curval_tbl[i]=0;
		}
		if(mp3devx->fft_curval_tbl[i]>mp3devx->fft_topval_tbl[i])//更新峰值
		{
			mp3devx->fft_topval_tbl[i]=mp3devx->fft_curval_tbl[i];
			mp3devx->fft_time_tbl[i]=10;//重设峰值停顿时间
		}
		if(mp3devx->fft_time_tbl[i])mp3devx->fft_time_tbl[i]--;
		else 
		{
			if(mp3devx->fft_topval_tbl[i])mp3devx->fft_topval_tbl[i]--;//峰值减小1
		}
		if(mp3devx->fft_curval_tbl[i]>63)mp3devx->fft_curval_tbl[i]=63;
		if(mp3devx->fft_topval_tbl[i]>63)mp3devx->fft_topval_tbl[i]=63; 
		fft_show_oneband(20+2+i*14,105-64,12,63,mp3devx->fft_curval_tbl[i],mp3devx->fft_topval_tbl[i]);//显示柱子	   
	}   	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//新的频率值
const u16 VS_NEW_BANDS_FREQ_TBL[14]={80,300,800,1270,2016,3200,4500,6000,7500,9000,11000,13000,15000,20000};
//更新MP3播放的信息
//mp3devx:MP3控制器
//mp3prgbx:进度条
//lrcx:歌词控制器
void mp3_info_upd(_m_mp3dev *mp3devx,_progressbar_obj* mp3prgbx,_progressbar_obj* volprgbx,_lyric_obj* lrcx)
{
	u16 temp;			  
	u8 i;				 
 	if(mp3devx->sta&(1<<6))//执行了一次歌曲切换,更新歌曲名字和当前播放曲目索引,mp3prgb长度等信息
	{
		mp3devx->sta&=~(1<<6);//清空标志位
		gui_fill_rectangle(0,20+4,240,14,MP3_MAIN_BKCOLOR);//上下各多清空一点,清空之前的显示 
		gui_show_ptstrwhiterim(5,20+5,239-5,319,0,0X0000,0XFFFF,12,mp3devx->name);	//显示新的名字
		
		mp3devx->namelen=strlen((const char*)mp3devx->name);//得到所占字符的个数
		mp3devx->namelen*=6;//得到点数
		mp3devx->curnamepos=0;//得到点数

  		gui_phy.back_color=MP3_MAIN_BKCOLOR;//设置背景色为底色
  		mp3_show_vol((volprgbx->curpos*100)/volprgbx->totallen);				//显示音量百分比   
 		gui_show_num(120,193,3,MP3_INFO_COLOR,12,mp3devx->curindex,0x80);		//非叠加
		gui_show_ptchar(120+18,193,120+18+6,193+12,0,MP3_INFO_COLOR,12,'/',0);	//显示分隔符
		gui_show_num(120+24,193,3,MP3_INFO_COLOR,12,mp3devx->mfilenum,0x80);	//非叠加
		//临时数据清零
 		mp3devx->kbps=0;
		mp3devx->playtime=0;gui_fill_rectangle(20,133,66,12,MP3_MAIN_BKCOLOR);	//清空之前的显示 
		gui_show_string("00:00/00:00",21,133,66,12,12,MP3_INFO_COLOR);			//时间清零
  		gui_show_num(178,133,3,MP3_INFO_COLOR,12,mp3devx->kbps,0x80);			//非叠加显示码率
 		mp3prgbx->totallen=mp3devx->fmp3->fsize;	//更新总长度	
		mp3prgbx->curpos=0;	  	
		//清空频谱数据
		for(i=0;i<FFT_BANDS;i++)
		{
			mp3devx->fft_topval_tbl[i]=0;
 			mp3devx->fft_curval_tbl[i]=0;
  			mp3devx->fft_time_tbl[i]=0;	    
			fft_show_oneband(20+2+i*14,105-64,12,63,0,0);//每个柱子都清零	   
		}				 
		VS_Set_Bands((u16*)VS_NEW_BANDS_FREQ_TBL,14);//重设频率
		if(lrcx)
		{
			gui_fill_rectangle(20,216,220,36+10,MP3_MAIN_BKCOLOR);//清除之前的显示
			lrc_read(lrcx,mp3devx->path,mp3devx->name);
		}
	}
	if(mp3devx->namelen>240-10)//大于屏幕长度
	{
		gui_fill_rectangle(0,20+4,240,14,MP3_MAIN_BKCOLOR);//上下各多清空一点,清空之前的显示 
		gui_show_ptstrwhiterim(5,20+5,239-5,319,mp3devx->curnamepos,0X0000,0XFFFF,12,mp3devx->name);	//显示新的名字
		mp3devx->curnamepos++;
		if(mp3devx->curnamepos+240-10>mp3devx->namelen+110)mp3devx->curnamepos=0;//循环显示		
	}
	if(mp3devx->sta&(1<<7))//MP3正在播放
	{
	 	mp3prgbx->curpos=f_tell(mp3devx->fmp3);//得到当前的播放位置
		progressbar_draw_progressbar(mp3prgbx);//更新进度条位置
		temp=VS_Get_DecodeTime();
		if(temp!=mp3devx->playtime)
		{
 			mp3devx->playtime=temp;
			mp3_time_show(21,133,mp3devx->playtime);//显示播放时间
			temp=VS_Get_HeadInfo();//得到码率
			if(mp3devx->kbps!=temp)
			{
				mp3devx->kbps=temp;
				gui_phy.back_color=MP3_MAIN_BKCOLOR;//设置背景色为底色
				gui_show_num(178,133,3,MP3_INFO_COLOR,12,mp3devx->kbps,0x80);//非叠加显示码率	   
				temp=(mp3devx->fmp3->fsize/temp)/125;  	//计算总时间,单位s
				mp3_time_show(21+6*6,133,temp);			//显示总时间
			}   
		}	    
	}
}
//显示歌词
//mp3devx:MP3控制器
//lrcx:歌词控制器
void mp3_lrc_show(_m_mp3dev *mp3devx,_lyric_obj* lrcx)
{
	u8 t;
	u16 temp,temp1;	   	  
	if(lrcx->oldostime!=GUI_TIMER_10MS)//每10ms更新一下
	{
		t=gui_disabs(GUI_TIMER_10MS,lrcx->oldostime);//防止很久没有进入主程序导致的漏加
		lrcx->oldostime=GUI_TIMER_10MS;
		if(t>10)t=1;
		lrcx->curtime+=t;//增加10ms	 
		if(lrcx->indexsize)//有歌词存在 
		{
			lrcx->detatime+=t;//标志时间增加了10ms
			if(lrcx->curindex<lrcx->indexsize)//还没显示完
			{
	 			if((lrcx->curtime%100)>80)//1秒钟后过了800ms,需要查询VS10XX的解码时间寄存器以同步歌词
				{							 	 
					lrcx->curtime=(u32)VS_Get_DecodeTime()*100;//更新秒钟
				}
				if(lrcx->curtime>=lrcx->time_tbl[lrcx->curindex])//当前时间超过了,需要更新歌词
				{
					gui_fill_rectangle(20,216,200,36+10,MP3_MAIN_BKCOLOR);//清除之前的显示
	 				if(lrcx->curindex)//需要显示上一句
					{
						lrcx->curindex--;
						lrcx->color=MP3_LRC_SCOLOR;
						lrc_show_linelrc(lrcx,20,216,200,12);//显示上一句歌词
						lrcx->curindex++;//恢复.
					}
					if(lrcx->curindex<lrcx->indexsize-1)//还可以显示下一句
					{
						lrcx->curindex++;
						lrcx->color=MP3_LRC_SCOLOR;
						lrc_show_linelrc(lrcx,20,216+24+10,200,12);//显示下一句歌词
						lrcx->curindex--;//恢复.
	 				}
					lrcx->color=MP3_LRC_MCOLOR;
					lrc_show_linelrc(lrcx,20,216+12+5,200,12);		//显示本句歌词	    
					lrcx->curindex++;   
					if(lrcx->namelen>200)//需要滚动歌词
					{
						if(lrcx->curindex<lrcx->indexsize)//本句的下一句歌词还是存在的.
						{
							temp=lrcx->time_tbl[lrcx->curindex-1];//当前歌词的时间
							temp1=lrcx->time_tbl[lrcx->curindex]; //下一句歌词的时间 	 	   
							lrcx->updatetime=(temp1-temp)/(lrcx->namelen-150);//计算得到滚动间隔时间,这里多了50个单位,因为前面的程序执行时间影响.
							if(lrcx->updatetime>20)lrcx->updatetime=20;//最大不超过200ms;
 						}else lrcx->updatetime=5;//默认滚动时间.50ms	  
					}													   
				}
			}
			if(lrcx->detatime>=lrcx->updatetime)//每lrcx->updatetime*10ms滚动显示当前歌词(如果需要滚动的话)
			{			 
				if(lrcx->namelen>200)//超过了显示区域,需要滚动显示本句歌词
				{
 					gui_fill_rectangle(20,216+12+5,200,12,MP3_MAIN_BKCOLOR);//清空之前的显示
					gui_show_ptstr(20,216+12+5,239-20,319,lrcx->curnamepos,MP3_LRC_MCOLOR,lrcx->font,lrcx->buf,1);//滚动显示本句歌词
					lrcx->curnamepos++;
					if(lrcx->curnamepos+200>lrcx->namelen+50)lrcx->curnamepos=0;//循环显示		
				}
				lrcx->detatime=0;		  
			}
		}   
	}
}	  
//MP3播放
u8 mp3_play(void)
{
	u8 i;
	u8 res;
	u8 tcnt=0;
	u8 rval=0;//1,内存错误;2,返回,MP3继续播放;3,返回,停止MP3播放.
	u16 specbuf[15];
	u8 lastvolpos;
 
	_progressbar_obj* mp3prgb,*volprgb;
	_btn_obj* tbtn[5];		    
 	if(mp3dev==NULL)//第一次打开,申请必要的内存
	{
		mp3dev=(_m_mp3dev*)gui_memin_malloc(sizeof(_m_mp3dev));
		if(mp3dev==NULL)return 1;							//申请内存失败
		gui_memset((u8 *)mp3dev,0,sizeof(_m_mp3dev));			//清零	   
		mp3dev->fmp3=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//开辟FIL字节的内存区域 
		if(mp3dev->fmp3==NULL)								//内存申请失败.	
		{
			gui_memin_free(mp3dev);	 						//释放之前申请到的内存
			return 1;	
		}	 
		lrcdev=lrc_creat();									//创建歌词管理结构体
		if(lrcdev)
		{
			lrcdev->font=12;
 			lrcdev->bkcolor=MP3_MAIN_BKCOLOR;		    
		}
 		gui_memset((u8 *)mp3dev->fmp3,0,sizeof(FIL));			//清零	   
	}   
	if((mp3dev->sta&0x80)==0)	//当前没有播放MP3
	{
		VS_HD_Reset();
		VS_Soft_Reset();		//复位
		mp3_filelist(mp3dev);
		if((mp3dev->sta&0x80)==0)return 0;//还是没有MP3在播放,则退出程序
	}else mp3dev->sta|=1<<6;//模拟一次切歌,让歌曲名字等信息显示出来 
/////////////////////////////////////////////////////////////////////////////////
	mp3prgb=progressbar_creat(20,120,200,10,0X20);		//mp3播放进度条
	if(mp3prgb==NULL)rval=1;
	volprgb=progressbar_creat(120,150+15,100,6,0X20);	//声音大小进度条
  	if(volprgb==NULL)rval=1;	   
 	volprgb->totallen=150;	//更新总长度,音频从100~250.偏移为100.	
	
	if(vsset.mvol>=100&&vsset.mvol<=250)volprgb->curpos=vsset.mvol-100;
	else//错误的数据 
	{
		vsset.mvol=0;
		volprgb->curpos=0;
	}	  
	lastvolpos=volprgb->curpos;//设定最近的位置

	for(i=0;i<5;i++)//循环创建5个按钮
	{
		tbtn[i]=btn_creat(0+i*48,271,48,48,0,1);//创建图片按钮
		if(tbtn[i]==NULL){rval=1;break;}//创建失败.
		tbtn[i]->picbtnpathu=(u8*)MP3_BTN_PIC_TBL[0][i];
		tbtn[i]->picbtnpathd=(u8*)MP3_BTN_PIC_TBL[1][i];
 		tbtn[i]->sta=0;	 
	}
	if(rval==0)//没有错误
	{
	   	mp3prgb->inbkcolora=0x738E;			//默认色
	 	mp3prgb->inbkcolorb=MP3_INFO_COLOR;	//默认色 
	 	mp3prgb->infcolora=0X75D;			//默认色
	 	mp3prgb->infcolorb=0X596;			//默认色 
	
	   	volprgb->inbkcolora=MP3_INFO_COLOR;	//默认色
	 	volprgb->inbkcolorb=MP3_INFO_COLOR;	//默认色 
	 	volprgb->infcolora=0X75D;			//默认色
	 	volprgb->infcolorb=0X596;			//默认色 	   						    						  		
							 	  					   
	
		mp3_load_ui();//加载主界面
		mp3_show_vol((volprgb->curpos*100)/volprgb->totallen);	//显示音量百分比 
		for(i=0;i<5;i++)btn_draw(tbtn[i]);		//画按钮
		tbtn[2]->picbtnpathu=(u8*)MP3_PLAYR_PIC;//按下一次之后变为播放松开状态
		progressbar_draw_progressbar(mp3prgb);	//画进度条	 
		progressbar_draw_progressbar(volprgb);	//画进度条
		tcnt=0;
 		if(systemset.fmtxmode)//如果开启了发射,则打开FM发射.
		{
			RDA5820_TX_Mode();	//设置为发送模式 
			RDA5820_Freq_Set(systemset.fmtxfreq*10);	//设置到systemset.fmtxfreq的频率值
			RDA5820_TxPGA_Set(3); 						//设置增益为3
		}
		Audiosel_Set(0);		//设置为音频输出为MP3音频
		while(rval==0)
		{
			tcnt++;//计时增加.
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
	 		for(i=0;i<5;i++)
			{
				res=btn_check(tbtn[i],&in_obj);	 
				if((res&&((tbtn[i]->sta&(1<<7))==0)&&(tbtn[i]->sta&(1<<6)))||system_task_return)//有按键按下且松开,并且TP松开了或者TPAD返回
				{
					if(system_task_return)i=4;//TPAD返回
					switch(i)
					{
						case 0://file list
							mp3_filelist(mp3dev);
							mp3dev->sta|=1<<6;//模拟一次切歌,让歌曲名字等信息显示出来
							//////////////////////////////////////////////////////////////////
 							mp3_load_ui();//重新加载主界面
							if(mp3dev->sta&(1<<5))//是暂停
							{
								tbtn[2]->picbtnpathu=(u8*)MP3_PLAYR_PIC;
							}else tbtn[2]->picbtnpathu=(u8*)MP3_PAUSER_PIC;
							for(i=0;i<5;i++)btn_draw(tbtn[i]);		//画按钮
							if(mp3dev->sta&(1<<5))//是暂停
							{
								tbtn[2]->picbtnpathu=(u8*)MP3_PAUSER_PIC;
							}else tbtn[2]->picbtnpathu=(u8*)MP3_PLAYR_PIC;
							progressbar_draw_progressbar(mp3prgb);	//画进度条	 
							progressbar_draw_progressbar(volprgb);	//画进弱
							if(system_task_return)//刚刚退出文件浏览
							{
								delay_ms(100);
								system_task_return=0;
							}		 
 							break;
						case 1://上一曲或者下一曲
						case 3:
				    		mp3_stop(mp3dev);
							if(systemset.mp3mode==1)//随机播放
							{						    
								mp3dev->curindex=app_get_rand(mp3dev->mfilenum);//得到下一首歌曲的索引	  
							}else
							{
								if(i==1)//上一曲
								{
									if(mp3dev->curindex)mp3dev->curindex--;
									else mp3dev->curindex=mp3dev->mfilenum-1;
								}else
								{
									if(mp3dev->curindex<(mp3dev->mfilenum-1))mp3dev->curindex++;
									else mp3dev->curindex=0;
								}
							}
							OSMboxPost(mp3mbox,(void*)(mp3dev->curindex+1));//发送邮箱,因为邮箱不能为空,所以在这必须加1
							break;
						case 2://播放/暂停
							if(mp3dev->sta&(1<<5))//是暂停
							{
								mp3dev->sta&=~(1<<5);//取消暂停
								tbtn[2]->picbtnpathd=(u8*)MP3_PAUSEP_PIC; 
								tbtn[2]->picbtnpathu=(u8*)MP3_PLAYR_PIC;
							}else//暂停状态
							{
								mp3dev->sta|=1<<5;//标记暂停
								tbtn[2]->picbtnpathd=(u8*)MP3_PLAYP_PIC; 
								tbtn[2]->picbtnpathu=(u8*)MP3_PAUSER_PIC;
							}
							break;
						case 4://停止播放/返回
							if(mp3dev->sta&(1<<5))//暂停状态下按了返回
							{		   
								rval=3;//退出播放,MP3停止播放
								mp3_stop(mp3dev);//停止播放
							}else//暂停状态下按返回,那就关闭MP3播放功能.
							{
								rval=2;//退出播放界面,MP3继续播放
 							}	 
							break;

					}   
				}
			}
			res=progressbar_check(volprgb,&in_obj);//检查音量进度条
			if(res&&lastvolpos!=volprgb->curpos)//被按下了,且位置变化了.执行音量调整
			{		  
				lastvolpos=volprgb->curpos;
				if(volprgb->curpos)vsset.mvol=100+volprgb->curpos;//设置音量
				else vsset.mvol=0;									 
				VS_Set_Vol(vsset.mvol);//设置音量						   
  				mp3_show_vol((volprgb->curpos*100)/volprgb->totallen);	//显示音量百分比   
 			}	  
			res=progressbar_check(mp3prgb,&in_obj);
			if(res&&((mp3prgb->sta&&PRGB_BTN_DOWN)==0))//被按下了,并且松开了,执行快进快退
			{
				f_lseek(mp3dev->fmp3,mp3prgb->curpos);//快速定位
			}	  
			if((tcnt%20)==0)mp3_info_upd(mp3dev,mp3prgb,volprgb,lrcdev);//更新显示信息,每100ms执行一次
 			if(((mp3dev->sta&(1<<4))==0)&&(tcnt%5)==0)		//不是flac,执行频谱显示
			{
				res=VS_Get_Spec(specbuf);
				if(res)mp3_specalz_show(mp3dev,specbuf);	//显示频谱
			}
			if(lrcdev!=NULL&&((mp3dev->sta&(1<<5))==0)&&(mp3dev->sta&(1<<7)))//正在播放,不是暂停,且歌词结构体正常
			{
				mp3_lrc_show(mp3dev,lrcdev);	//可以显示歌词	  
			}
		}
	}
 	for(i=0;i<5;i++)btn_delete(tbtn[i]);//删除按钮	
	progressbar_delete(mp3prgb);
	progressbar_delete(volprgb);
	if(rval==3)//退出MP3播放.且不后台播放
	{
		if(systemset.audiosel==0)systemset.audiosel=3;//无声通道
		Audiosel_Set(systemset.audiosel);	//设置回原来的声道  
		gui_memin_free(mp3dev->path);		//释放内存
		gui_memex_free(mp3dev->mfindextbl);	//释放内存
		myfree(SRAMIN,mp3dev->fmp3);		//释放内存
		gui_memin_free(mp3dev);				//释放内存	 
		lrc_delete(lrcdev);					//释放歌词显示申请的内存
		lrcdev=NULL;//指向空地址
		mp3dev=NULL;//指向空地址
	}else systemset.audiosel=0;				//默认通道变为MP3通道  			  
	return rval;
}















