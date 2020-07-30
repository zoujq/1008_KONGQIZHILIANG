#include "sys.h"
#include "usmart.h"
#include "usmart_str.h"								  
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加)
#include "includes.h"
#include "t9input.h"
#include "sramlcd.h"
#include "window.h"
#include "24cxx.h"
#include "fontupd.h"


												 
extern void read_ibuton(u8 addr,u8 len);			
extern void write_ibuton(u8 addr,u8 data);
extern void gui_show_strmid(u16 x,u16 y,u16 width,u16 height,u16 color,u8 size,u8 *str);		 
extern void test_prgb(u16 x,u16 y,u16 width,u16 height,u8 type,u16 icur);
extern void progressbar_show_char(u16 x,u16 y,u8 dir,u8 num,u16 color);
extern void progressbar_show_percnt(u16 x,u16 y,u8 dir,u8 num,u16 color);
extern void gui_show_ptchar(u16 x,u16 y,u16 xend,u16 yend,u8 offset,u16 color,u16 size,u8 chr,u8 mode);
extern void gui_show_ptstr(u16 x,u16 y,u16 xend,u16 yend,u16 offset,u16 color,u8 size,u8 *str,u8 mode);

extern void tslistbox_creat(u16 x,u16 y,u16 lenth,u16 height,u8 type,u8 fsize);
extern void tslistbox_del(void);
extern u16 tslistbox_addlist(u8 nameindex);
extern void tslistbox_draw(void);
 
extern void tsflistbox_creat(u16 x,u16 y,u16 lenth,u16 height,u8 type,u8 fsize);
extern void tsflistbox_del(void);
extern u16 tsflistbox_addlist(u8 *name,u8 type);
extern void tsflistbox_draw(void);
extern void tsflistbox_setpath(u8 *path);	 
extern u32 SD_GetSectorCount(void); 
 								     						  
extern u8 NRF24L01_Read_Reg(u8 reg);			//读寄存器
extern u8 NRF24L01_Write_Reg(u8 reg, u8 value);//写寄存器

	   
#include "lyric.h"
#include "VS10XX.h"			   
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif
  	(void*)app_usmart_getsn,"void app_usmart_getsn(void)", 

 	(void*)SCCB_WR_Reg,"u8 SCCB_WR_Reg(u8 reg,u8 data)", 
	(void*)SCCB_RD_Reg,"u8 SCCB_RD_Reg(u8 reg)", 
  	(void*)OV7670_Window_Set,"void OV7670_Window_Set(u16 startx,u16 starty,u16 width, u16 height)", 
  	(void*)AT24CXX_WriteOneByte,"void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)", 
   
  	(void*)SPI_Flash_Erase_Chip,"void SPI_Flash_Erase_Chip(void)",
 				
 	(void*)delay_ms,"void delay_ms(u16 nms)",
	(void*)delay_us,"void delay_us(u32 nus)",
	(void*)LCD_Clear,"void LCD_Clear(u16 color)",
	(void*)LCD_Fill,"void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)",
 

	(void*)LCD_Color_Fill,"void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)",		    
 //	(void*)VS_Set_Effect,"void VS_Set_Effect(u8 eft)",
// 	(void*)VS_Set_Bass,"void VS_Set_Bass(u8 bfreq,u8 bass,u8 tfreq,u8 treble)",
// 	(void*)VS_Set_Vol,"void VS_Set_Vol(u8 volx)",
//	(void*)Draw_Circle,"void Draw_Circle(u16 x0,u16 y0,u8 r)",

	(void*)lrc_test,"u8 lrc_test(u8 * path,u8 *name)",
//	(void*)Draw_Circle,"void Draw_Circle(u16 x0,u16 y0,u8 r)",
//	(void*)SPI1_SetSpeed,"void SPI1_SetSpeed(u8 SpeedSet)",
	
	(void*)gui_color_chg,"u16 gui_color_chg(u32 rgb)",
//	(void*)Draw_Circle,"void Draw_Circle(u16 x0,u16 y0,u8 r)",
//	(void*)gui_fill_circle,"void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)",
 						   
	(void*)update_font,"u8 update_font(u8 src)",
	(void*)SD_GetSectorCount,"u32 SD_GetSectorCount(void)",
	(void*)mem_perused,"u8 mem_perused(u8 memx)",
 
 	(void*)btn_test,"void btn_test(u16 x,u16 y,u16 width,u16 height,u8 type,u8 sta,u8 r,u8 *caption,u8 *uppath,u8 *downpath)",
// 	(void*)gui_alpha_blend565,"u16 gui_alpha_blend565(u16 src,u16 dst,u8 alpha)",
//	(void*)gui_show_strmid,"void gui_show_strmid(u16 x,u16 y,u16 width,u16 height,u16 color,u8 size,u8 *str)",
//	(void*)scrollbar_draw_sign,"void scrollbar_draw_sign(u16 x,u16 y,u16 signcolor,u8 signx)",
//	(void*)scrollbar_draw_btn,"void scrollbar_draw_btn(u16 x,u16 y,u16 width,u16 height,u16 bkcolor,u16 outcolor,u16 signcolor,u8 signx)",
//	(void*)test_scb,"void test_scb(u16 x,u16 y,u16 width,u16 height,u8 type,u16 icur,u16 incolor,u16 btncolor,u16 outcolor)",
//	(void*)test_prgb,"void test_prgb(u16 x,u16 y,u16 width,u16 height,u8 type,u16 icur)",
	(void*)TP_Adjust,"void TP_Adjust(void)",
 				   
//	(void*)progressbar_show_char,"void progressbar_show_char(u16 x,u16 y,u8 dir,u8 num,u16 color)",
//	(void*)progressbar_show_percnt,"void progressbar_show_percnt(u16 x,u16 y,u8 dir,u8 num,u16 color)",
//	(void*)gui_show_ptchar,"void gui_show_ptchar(u16 x,u16 y,u16 xend,,u16 yend,u8 offset,u16 color,u16 size,u8 chr,u8 mode)",
//	(void*)gui_show_ptstr,"void gui_show_ptstr(u16 x,u16 y,u16 xend,u16 yend,u16 offset,u16 color,u8 size,u8 *str,u8 mode)",
//	(void*)gui_show_ptstrwhiterim,"void gui_show_ptstrwhiterim(u16 x,u16 y,u16 xend,u16 offset,u16 color,u16 rimcolor,u8 size,u8 *str)",

//	(void*)tslistbox_draw,"void tslistbox_draw(void)",
//	(void*)tslistbox_creat,"void tslistbox_creat(u16 x,u16 y,u16 lenth,u16 height,u8 type,u8 fsize)",
//	(void*)tslistbox_del,"void tslistbox_del(void)",
//	(void*)tslistbox_addlist,"u16 tslistbox_addlist(u8 index)",
//
//	(void*)tsflistbox_draw,"void tsflistbox_draw(void)",
//	(void*)tsflistbox_creat,"void tsflistbox_creat(u16 x,u16 y,u16 lenth,u16 height,u8 type,u8 fsize)",
//	(void*)tsflistbox_del,"void tsflistbox_del(void)",
//	(void*)tsflistbox_addlist,"u16 tsflistbox_addlist(u8 *name,u8 type)",
//	(void*)tsflistbox_setpath,"void tsflistbox_setpath(u8 *path)",
//
// 	(void*)t9_test,"void t9_test(u16 x,u16 y,u8 mode)",
//	(void*)t9_tsdel,"void t9_tsdel(void)",
//	(void*)t9_tsdraw,"void t9_tsdraw(void)",
  
//	(void*)gui_draw_icos,"void gui_draw_icos(u16 x,u16 y,u8 index)",
//	(void*)gui_draw_icosalpha,"void gui_draw_icosalpha(u16 x,u16 y,u8 index)",
	
	 
//	(void*)mf_mount,"u8 mf_mount(u8 drv)",
	(void*)mf_open,"u8 mf_open(u8*path,u8 mode)",
//	(void*)mf_close,"u8 mf_close(void)",
//	(void*)mf_read,"u8 mf_read(u16 len)",
//	(void*)mf_write,"u8 mf_write(u8*dat,u16 len)",
 	(void*)mf_opendir,"u8 mf_opendir(u8* path)",
	(void*)mf_readdir,"u8 mf_readdir(void)",
//
 	(void*)mf_scan_files,"u32 mf_scan_files(u8 * path)",
 	(void*)mf_showfree,"u32 mf_showfree(u8 *drv)",
	(void*)mf_lseek,"u8 mf_lseek(u32 offset)",
	(void*)mf_tell,"u8 mf_tell(void)",
 	(void*)mf_size,"u32 mf_size(void)",


	(void*)mf_fmkfs,"u8 mf_fmkfs(u8 drv,u8 mode,u16 au)",
	(void*)mf_mkdir,"u8 mf_mkdir(u8*name)",
	(void*)mf_unlink,"u8 mf_unlink(u8 *name)",
 	(void*)mf_rename,"u8 mf_rename(u8 *oldname,u8* newname)",
	(void*)mf_gets,"void mf_gets(u16 size)",
//	(void*)mf_putc,"u8 mf_putc(u8 c)",
//	(void*)mf_puts,"u8 mf_puts(u8*c)",



 	(void*)mf_fdsize,"u32 mf_fdsize(u8 *fdname)",
	(void*)mf_cpymsg,"u8 mf_cpymsg(u8*pname,u8 pct,u8 mode)",
 	(void*)mf_fcopy,"u8 mf_fcopy(u8 *psrc,u8 *pdst)",    
	(void*)mf_fdcopy,"u8 mf_fdcopy(u8 *psrc,u8 *pdst)",    
	 
	(void*)ai_load_picfile,"u8 ai_load_picfile(u8 *filename,u16 x,u16 y,u16 width,u16 height)",    
	(void*)minibmp_decode,"u8 minibmp_decode(u8 *filename,u16 x,u16 y,u16 width,u16 height,u16 acolor,u8 mode)",    
	(void*)bmp_encode,"u8 bmp_encode(u8 *filename,u16 x,u16 y,u16 width,u16 height,u8 mode)",    

	(void*)gif_decode,"u8 gif_decode(u8 *filename,u16 x,u16 y,u16 width,u16 height)",    
	(void*)gif_quit,"void gif_quit(void)",    
//	(void*)edit_test,"void edit_test(u16 x,u16 y,u16 width,u16 height,u8 type,u8 sta,u8 *text)",    
	(void*)gui_show_ptfont,"void gui_show_ptfont(u16 x,u16 y,u16 xend,u16 yend,u8 offset,u16 color,u16 size,u8* chr,u8 mode)",    
	(void*)gui_alphablend_area,"void gui_alphablend_area(u16 x,u16 y,u16 width,u16 height,u16 color,u8 aval)",
	(void*)slcd_frame_move,"void slcd_frame_move(u8 dir,u8 skips,u16 pos)",    
	(void*)slcd_frame_show,"void slcd_frame_show(u16 x)",
	
	(void*)VS_WR_Cmd,"void VS_WR_Cmd(u8 address,u16 data)",  
	(void*)gui_show_ptstr,"void gui_show_ptstr(u16 x,u16 y,u16 xend,u16 yend,u16 offset,u16 color,u8 size,u8 *str,u8 mode)",  
 	(void*)VS_WRAM_Read,"u16 VS_WRAM_Read(u16 addr)",
	
 	(void*)btn_test,"void btn_test(u16 x,u16 y,u16 width,u16 height,u8 type,u8 sta,u8 r,u8 *caption,u8 *uppath,u8 *downpath)",
	
 
 
 	(void*)win_test,"void win_test(u16 x,u16 y,u16 width,u16 height,u8 type,u16 cup,u16 cdown,u8 *caption)",
 	(void*)app_draw_smooth_line,"void app_draw_smooth_line(u16 x,u16 y,u16 width,u16 height,u32 sergb,u32 mrgb)",
  	
 
  	(void*)window_msg_box,"u8 window_msg_box(u16 x,u16 y,u16 width,u16 height,u8 *str,u8 *caption,u8 font,u16 color,u8 mode)",
  	
	 
											  
};							  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















