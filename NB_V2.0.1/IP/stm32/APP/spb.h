#include "sramlcd.h"
#include "dma.h"
#include "lcd.h"
#include "delay.h"
#include "malloc.h"
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

#define SPB_MOVE_WIN 		2 		//SPB滑动检测窗口值
#define SPB_MOVE_MIN		8		//SPB认为是滑动的最小值,超过会认为是滑动,但是不一定会翻页	   
#define SPB_MOVE_ACT 		50 		//SPB滑动的启动值,超过这个值将会进行翻页

#define SPB_ALPHA_VAL 		18 		//SPB选中透明度设置
#define SPB_ALPHA_COLOR		WHITE	//SPB透明色
#define SPB_FONT_COLOR 		BLUE 	//SPB字体颜色

////////////////////////////////////////////////////////////////////////////////////////////
//各图标/图片路径
extern const u8* spb_bkpic_path_tbl[2];		//背景图片路径表
extern const u8 * spb_icos_path_tbl[2][9];  //图标路径表
////////////////////////////////////////////////////////////////////////////////////////////
//SPB 控制器
__packed typedef struct _m_spb_icos
{										    
	u16 x;			//图标坐标及尺寸
	u16 y;
	u8 width;
	u8 height; 
	u8 * path;		//图标路径
	u8 * name;		//图标名字
}m_spb_icos;

//SPB 控制器
typedef struct _m_spb_dev
{										    
	u16 oldxpos;     			//上一次TP的x坐标位置
 	u16 curxpos;     			//当前tp按下的x坐标
	u16 curypos;     			//当前tp按下的y坐标
	u8	spbsta;					//spb状态
								//[7]:第一次按下标志
								//[6]:滑动标志;					 
								//[5~0]:保留
	u8 selico;					//当前选中的图标.
								//0~8,被选中的图标编号	 
								//其他,没有任何图标被选中	  
	m_spb_icos icos[2][9];		//2页,每页9个图标
}m_spb_dev;
extern m_spb_dev spbdev;

void spb_init(void);
void spb_load_icos(u8 frame);
void spb_set_sel(u8 sel);
u8 spb_move_chk(void);

























 




