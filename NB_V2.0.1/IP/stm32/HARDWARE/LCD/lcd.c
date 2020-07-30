#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"
#include "icon.h"


#include "includes.h"	   

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//2.4寸/2.8寸/3.5寸/4.3寸 TFT液晶驱动	  
//支持驱动IC型号包括:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/SSD1289/1505/B505/C505/NT35310/NT35510等		    
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/2/11
//版本：V2.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//V1.2修改说明
//支持了SPFD5408的驱动,另外把液晶ID直接打印成HEX格式.方便查看LCD驱动IC.
//V1.3
//加入了快速IO的支持
//修改了背光控制的极性（适用于V1.8及以后的开发板版本）
//对于1.8版本之前(不包括1.8)的液晶模块,请修改LCD_Init函数的LCD_LED=1;为LCD_LED=1;
//V1.4
//修改了LCD_ShowChar函数，使用画点功能画字符。
//加入了横竖屏显示的支持
//V1.5 20110730
//1,修改了B505液晶读颜色有误的bug.
//2,修改了快速IO及横竖屏的设置方式.
//V1.6 20111116
//1,加入对LGDP4535液晶的驱动支持
//V1.7 20120713
//1,增加LCD_RD_DATA函数
//2,增加对ILI9341的支持
//3,增加ILI9325的独立驱动代码
//4,增加LCD_Scan_Dir函数(慎重使用)	  
//6,另外修改了部分原来的函数,以适应9341的操作
//V1.8 20120905
//1,加入LCD重要参数设置结构体lcddev
//2,加入LCD_Display_Dir函数,支持在线横竖屏切换
//V1.9 20120911
//1,新增RM68042驱动（ID:6804），但是6804不支持横屏显示！！原因：改变扫描方式，
//导致6804坐标设置失效，试过很多方法都不行，暂时无解。
//V2.0 20120924
//在不硬件复位的情况下,ILI9341的ID读取会被误读成9300,修改LCD_Init,将无法识别
//的情况（读到ID为9300/非法ID）,强制指定驱动IC为ILI9341，执行9341的初始化。
//V2.1 20120930
//修正ILI9325读颜色的bug。
//V2.2 20121007
//修正LCD_Scan_Dir的bug。
//V2.3 20130120
//新增6804支持横屏显示
//V2.4 20131120
//1,新增NT35310（ID:5310）驱动器的支持
//2,新增LCD_Set_Window函数,用于设置窗口,对快速填充,比较有用,但是该函数在横屏时,不支持6804.
//V2.5 20140211
//1,新增NT35510（ID:5510）驱动器的支持
//////////////////////////////////////////////////////////////////////////////////	 
				 
//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 



		u32 ind0ex=0;      
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
	 
//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(u16 regval)
{ 
	LCD->LCD_REG=regval;//写入要写的寄存器序号	 
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(u16 data)
{										    	   
	LCD->LCD_RAM=data;		 
}
//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{										    	   
	return LCD->LCD_RAM;		 
}					   
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
}	   
//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return LCD_RD_DATA();		//返回读到的值
}   
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
	while(i--);
}
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//超过了范围,直接返回		   
	LCD_SetCursor(x,y);	    
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)LCD_WR_REG(0X2E);//9341/6804/3510 发送读GRAM指令
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2E00);	//5510 发送读GRAM指令
	else LCD_WR_REG(R34);      		 				//其他IC发送读GRAM指令
 	if(lcddev.id==0X9320)opt_delay(2);				//FOR 9320,延时2us	    
	if(LCD->LCD_RAM)r=0;							//dummy Read	   
	opt_delay(2);	  
 	r=LCD->LCD_RAM;  		  						//实际坐标颜色
 	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)		//9341/NT35310/NT35510要分2次读出
 	{
		opt_delay(2);	  
		b=LCD->LCD_RAM; 
		g=r&0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
		g<<=8;
	}else if(lcddev.id==0X6804)r=LCD->LCD_RAM;		//6804第二次读取的才是真实值 
	if(lcddev.id==0X9325||lcddev.id==0X4535||lcddev.id==0X4531||lcddev.id==0X8989||lcddev.id==0XB505)return r;	//这几种IC直接返回颜色值
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510需要公式转换一下
	else return LCD_BGR2RGB(r);						//其他IC
}			 
//LCD开启显示
void LCD_DisplayOn(void)
{					   
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)LCD_WR_REG(0X29);	//开启显示
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2900);	//开启显示
	else LCD_WriteReg(R7,0x0173); 				 	//开启显示
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)LCD_WR_REG(0X28);	//关闭显示
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2800);	//关闭显示
	else LCD_WriteReg(R7,0x0);//关闭显示 
}   
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
 /*	if(lcddev.id==0X9341||lcddev.id==0X5310)
	{		    
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8); 
		LCD_WR_DATA(Xpos&0XFF);	 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8); 
		LCD_WR_DATA(Ypos&0XFF);
	}else if(lcddev.id==0X6804)
	{
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//横屏时处理
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8); 
		LCD_WR_DATA(Xpos&0XFF);	 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8); 
		LCD_WR_DATA(Ypos&0XFF);
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8); 
		LCD_WR_REG(lcddev.setxcmd+1); 
		LCD_WR_DATA(Xpos&0XFF);	 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8); 
		LCD_WR_REG(lcddev.setycmd+1); 
		LCD_WR_DATA(Ypos&0XFF);		
	}else
	{
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//横屏其实就是调转x,y坐标
		LCD_WriteReg(lcddev.setxcmd, Xpos);
		LCD_WriteReg(lcddev.setycmd, Ypos);
	}	 */
	
 /* LCD_WR_REG(0x002A);LCD_WR_DATA(Ypos>>8);
  LCD_WR_DATA(0x00FF&Ypos);                 //设定X坐标
  LCD_WR_DATA(0x0001);LCD_WR_DATA(0x003F);

  LCD_WR_REG(0x002B);LCD_WR_DATA((480-Xpos)>>8);
  LCD_WR_DATA(0x00FF&(480-Xpos));           //设定Y坐标
  LCD_WR_DATA(0x0001);LCD_WR_DATA(0x00df);
	
	*/	
	
/*	 
LCD_WR_REG(0x2A);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x01);    
LCD_WR_DATA(0xDF);    
 
LCD_WR_REG(0x2B);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x01);    
LCD_WR_DATA(0x3F);  
*/

//有个致使问题点，最后点不能是480，320，要为479，319，不然命令没用，会出现最后一个字符刷到哪，刷全屏只显示到哪
	LCD_WR_REG(0x2A);
	LCD_WR_DATA( (Xpos) >> 8);
	LCD_WR_DATA( (Xpos) & 0x00FF);
	LCD_WR_DATA( 479 >> 8)	;
	LCD_WR_DATA( (479) & 0x00FF);

	LCD_WR_REG(0x2B);
	LCD_WR_DATA( (Ypos) >> 8);
	LCD_WR_DATA( (Ypos) & 0x00FF);
	LCD_WR_DATA( 319 >> 8);
	LCD_WR_DATA( (319) & 0x00FF);
	
	LCD_WR_REG(0x002c);
	
} 		 

//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341/5310/5510等IC已经实际测试	   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if(lcddev.dir==1&&lcddev.id!=0X6804)//横屏时，对6804不改变扫描方向！
	{			   
		switch(dir)//方向转换
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	}
	if(lcddev.id==0x9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510)//9341/6804/5310/5510,很特殊
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510))regval|=0X08;//5310/5510不需要BGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804的BIT6和9341的反了	   
		LCD_WriteReg(dirreg,regval);
 		if((regval&0X20)||lcddev.dir==1)
		{
			if(lcddev.width<lcddev.height)//交换X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else  
		{
			if(lcddev.width>lcddev.height)//交换X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}  
		if(lcddev.id==0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA((lcddev.width-1)>>8); 
			LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA((lcddev.width-1)&0XFF); 
			LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA((lcddev.height-1)>>8); 
			LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA((lcddev.height-1)&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	}else 
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		}
		if(lcddev.id==0x8989)//8989 IC
		{
			dirreg=0X11;
			regval|=0X6040;	//65K   
	 	}else//其他驱动IC		  
		{
			dirreg=0X03;
			regval|=1<<12;  
		}
		LCD_WriteReg(dirreg,regval);
	}
}   
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	//LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD->LCD_RAM=POINT_COLOR; 
}
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	if(lcddev.id==0X9341||lcddev.id==0X5310)
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8); 
		LCD_WR_DATA(x&0XFF);	 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8); 
		LCD_WR_DATA(y&0XFF);
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(x>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(x&0XFF);	  
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(y>>8);  
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(y&0XFF); 
	}else if(lcddev.id==0X6804)
	{		    
		if(lcddev.dir==1)x=lcddev.width-1-x;//横屏时处理
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8); 
		LCD_WR_DATA(x&0XFF);	 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8); 
		LCD_WR_DATA(y&0XFF);
	}else
	{
 		if(lcddev.dir==1)x=lcddev.width-1-x;//横屏其实就是调转x,y坐标
		LCD_WriteReg(lcddev.setxcmd,x);
		LCD_WriteReg(lcddev.setycmd,y);
	}			 
	LCD->LCD_REG=lcddev.wramcmd; 
	LCD->LCD_RAM=color; 
}	 


//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//竖屏
	{
		lcddev.dir=0;	//竖屏
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id==0X6804||lcddev.id==0X5310)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}else if(lcddev.id==0X8989)
		{
			lcddev.wramcmd=R34;
	 		lcddev.setxcmd=0X4E;
			lcddev.setycmd=0X4F;  
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=480;
			lcddev.height=800;
		}else
		{
			lcddev.wramcmd=R34;
	 		lcddev.setxcmd=R32;
			lcddev.setycmd=R33;  
		}
	}else 				//横屏
	{	  				
		lcddev.dir=1;	//横屏
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}else if(lcddev.id==0X6804)	 
		{
 			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2B;
			lcddev.setycmd=0X2A; 
		}else if(lcddev.id==0X8989)
		{
			lcddev.wramcmd=R34;
	 		lcddev.setxcmd=0X4F;
			lcddev.setycmd=0X4E;   
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=800;
			lcddev.height=480;
		}else
		{
			lcddev.wramcmd=R34;
	 		lcddev.setxcmd=R33;
			lcddev.setycmd=R32;  
		}
		if(lcddev.id==0X6804||lcddev.id==0X5310)
		{ 	 
			lcddev.width=480;
			lcddev.height=320; 			
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}	 
//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height.
//68042,横屏时不支持窗口设置!! 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{   
	u8 hsareg,heareg,vsareg,veareg;
	u16 hsaval,heaval,vsaval,veaval; 
	width=sx+width-1;
	height=sy+height-1;
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X6804)//6804横屏不支持
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA(width>>8); 
		LCD_WR_DATA(width&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(height>>8); 
		LCD_WR_DATA(height&0XFF); 
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(sx>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(sx&0XFF);	  
		LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA(width>>8);   
		LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA(width&0XFF);   
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(sy>>8);   
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(sy&0XFF);  
		LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA(height>>8);   
		LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA(height&0XFF);  
	}else	//其他驱动IC
	{
		if(lcddev.dir==1)//横屏
		{
			//窗口值
			hsaval=sy;				
			heaval=height;
			vsaval=lcddev.width-width-1;
			veaval=lcddev.width-sx-1;				
		}else
		{ 
			hsaval=sx;				
			heaval=width;
			vsaval=sy;
			veaval=height;
		}
	 	if(lcddev.id==0X8989)//8989 IC
		{
			hsareg=0X44;heareg=0X44;//水平方向窗口寄存器 (1289的由一个寄存器控制)
			hsaval|=(heaval<<8);	//得到寄存器值.
			heaval=hsaval;
			vsareg=0X45;veareg=0X46;//垂直方向窗口寄存器	  
		}else  //其他驱动IC
		{
			hsareg=0X50;heareg=0X51;//水平方向窗口寄存器
			vsareg=0X52;veareg=0X53;//垂直方向窗口寄存器	  
		}								  
		//设置寄存器值
		LCD_WriteReg(hsareg,hsaval);
		LCD_WriteReg(heareg,heaval);
		LCD_WriteReg(vsareg,vsaval);
		LCD_WriteReg(veareg,veaval);		
		LCD_SetCursor(sx,sy);	//设置光标位置
	}
} 
//初始化lcd
//该初始化函数可以初始化各种ILI93XX液晶,但是其他函数是基于ILI9320的!!!
//在其他型号的驱动芯片上没有测试! 


void LCD_PWM_UnInit(void)
{		 					 
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

        TIM_ARRPreloadConfig(TIM8, DISABLE);             
        TIM_CtrlPWMOutputs(TIM8, DISABLE); 
        TIM_Cmd(TIM8,DISABLE);  
} 
void LCD_UnInit(void)
{ 										  
 	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//使能PORTA,B,C,D,E,G以及AFIO复用功能时钟

	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PA11     升压
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);		
	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD12     供电
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);	
	

	
	
	LCD_PWM_UnInit();
	CMachine.uLED_Back_No=0;
//	CMachine.fLED_Back=0;	
	
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PC7 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	
	
	
	
	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //PA4 推挽输出 复位脚
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
	
 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOD, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15); 
	
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOE, GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); 	  
  
	//	//PORTG12复用推挽输出 A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_11;	 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOD, GPIO_Pin_7|GPIO_Pin_11); 	   




	
}

//LCD 背光PWM输出初始化
//初始化背光参数为:PWM控制频率为80Khz
//通过LCD_BLPWM_VAL设置LCD背光亮度.
//0,最暗;250,最暗.
/*********************************************
    函数名称：TIM8_PWM_Init
    功        能：通用TIM8 CH1(PC6) CH2(PC7)PWM输出初始化
    入        参：arr：自动重装值。
                        psc：时钟预分频数
    返        回：无
【说明】Frq = 72Mhz / ( psc + 1 ) / ( arr + 1 )
*********************************************/
void LCD_PWM_Init(void)
{		 					 
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8|RCC_APB2Periph_GPIOC, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        TIM_DeInit(TIM8);

        //2.7K
        TIM_TimeBaseStructure.TIM_Period =370;
        TIM_TimeBaseStructure.TIM_Prescaler = 71;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

        /* Output Compare Active Mode configuration: Channel4 */

        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 0;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
        TIM_OC2Init(TIM8, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);

        TIM_ARRPreloadConfig(TIM8, ENABLE);             
        TIM_CtrlPWMOutputs(TIM8, ENABLE); 
        TIM_Cmd(TIM8,ENABLE);  
} 

void LCD_PWM_Change(u16 uLoad)
{

    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8|RCC_APB2Periph_GPIOC, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        TIM_DeInit(TIM8);

        //2.7K
        TIM_TimeBaseStructure.TIM_Period =370;
        TIM_TimeBaseStructure.TIM_Prescaler = 71;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

        /* Output Compare Active Mode configuration: Channel4 */

        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = uLoad;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
        TIM_OC2Init(TIM8, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);

        TIM_ARRPreloadConfig(TIM8, ENABLE);             
        TIM_CtrlPWMOutputs(TIM8, ENABLE); 
        TIM_Cmd(TIM8,ENABLE);  

	




}



void LCD_Init(void)
{ 										  
 	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//使能PORTA,B,C,D,E,G以及AFIO复用功能时钟

	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PA11     升压
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);		
	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD12     供电
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_12);	
	
	
/* 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PC7 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOC,GPIO_Pin_7);
*/
		// LCD_PWM_Init();
		
		

		
		
		  LCD_PWM_Change(0);


		//	CMachine.uLED_Back_No=0;
			CMachine.uLED_Back_No_B=0xff;	
			CMachine.fLED_Back=0;	
			CMachine.fLED_Back_B=0xff;				
			
	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //PA4 推挽输出 复位脚
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	
 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
   	//	//PORTG12复用推挽输出 A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_11;	 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
 
 
 

 
 
 
		readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

		writeTiming.FSMC_AddressSetupTime = 0x00;	 //地址建立时间（ADDSET）为1个HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
    writeTiming.FSMC_DataSetupTime = 0x03;		 ////数据保存时间为4个HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
		FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // 使能BANK1 
			
	
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
		delay_ms(200); // delay 50 ms 
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	delay_ms(200); // delay 50 ms 
	
	
LCD_WR_REG(0xB0);
LCD_WR_DATA(0x04);
 
LCD_WR_REG(0x36);    
LCD_WR_DATA(0x21);    
 
LCD_WR_REG(0x3A);    
LCD_WR_DATA(0x55);   
 
LCD_WR_REG(0xB4);    
LCD_WR_DATA(0x00); 
 
LCD_WR_REG(0xC0);
LCD_WR_DATA(0x03);//0013
LCD_WR_DATA(0xDF);//480
LCD_WR_DATA(0x40);
LCD_WR_DATA(0x12);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x01);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x43);
 
 
LCD_WR_REG(0xC1);//frame frequency
LCD_WR_DATA(0x05);//BCn,DIVn[1:0
LCD_WR_DATA(0x2f);//RTNn[4:0] 
LCD_WR_DATA(0x08);// BPn[7:0]
LCD_WR_DATA(0x08);// FPn[7:0]
LCD_WR_DATA(0x00);
 
 
 
LCD_WR_REG(0xC4);
LCD_WR_DATA(0x63);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x08);
 
 LCD_WR_REG(0xC8);//Gamma
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x16);
LCD_WR_DATA(0x24);//26
LCD_WR_DATA(0x30);//32 
LCD_WR_DATA(0x48);
LCD_WR_DATA(0x3d);
LCD_WR_DATA(0x28);
LCD_WR_DATA(0x20);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x04);
 
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x16);
LCD_WR_DATA(0x24);
LCD_WR_DATA(0x30);
LCD_WR_DATA(0x48);
LCD_WR_DATA(0x3d);
LCD_WR_DATA(0x28);
LCD_WR_DATA(0x20);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x04);
 
 
 
LCD_WR_REG(0xC9);//Gamma
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x16);
LCD_WR_DATA(0x24);//26
LCD_WR_DATA(0x30);//32 
LCD_WR_DATA(0x48);
LCD_WR_DATA(0x3d);
LCD_WR_DATA(0x28);
LCD_WR_DATA(0x20);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x04);
 
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x16);
LCD_WR_DATA(0x24);
LCD_WR_DATA(0x30);
LCD_WR_DATA(0x48);
LCD_WR_DATA(0x3d);
LCD_WR_DATA(0x28);
LCD_WR_DATA(0x20);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x04);
 
 
 
LCD_WR_REG(0xCA);//Gamma
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x16);
LCD_WR_DATA(0x24);//26
LCD_WR_DATA(0x30);//32 
LCD_WR_DATA(0x48);
LCD_WR_DATA(0x3d);
LCD_WR_DATA(0x28);
LCD_WR_DATA(0x20);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x04);
 
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x16);
LCD_WR_DATA(0x24);
LCD_WR_DATA(0x30);
LCD_WR_DATA(0x48);
LCD_WR_DATA(0x3d);
LCD_WR_DATA(0x28);
LCD_WR_DATA(0x20);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x04);
 
 
LCD_WR_REG(0xD0);
LCD_WR_DATA(0x95);
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x10);
LCD_WR_DATA(0x3f);
 
 
LCD_WR_REG(0xD1);
LCD_WR_DATA(0x02);
LCD_WR_DATA(0x28);
LCD_WR_DATA(0x28);
LCD_WR_DATA(0x40);
 
LCD_WR_REG(0xE1);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x00);   
LCD_WR_DATA(0x00);   
 
LCD_WR_REG(0xE2);    
LCD_WR_DATA(0x80);    
 
LCD_WR_REG(0x2A);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x01);    
LCD_WR_DATA(0xDF);    
 
LCD_WR_REG(0x2B);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x00);    
LCD_WR_DATA(0x01);    
LCD_WR_DATA(0x3F);    
 
LCD_WR_REG(0x11);
 
delay_ms(120);
 
LCD_WR_REG(0x29);
 
LCD_WR_REG(0xC1);//frame frequency
LCD_WR_DATA(0x05);//BCn,DIVn[1:0
LCD_WR_DATA(0x2f);//RTNn[4:0] 
LCD_WR_DATA(0x08);// BPn[7:0]
LCD_WR_DATA(0x08);// FPn[7:0]
LCD_WR_DATA(0x00);
	
				lcddev.width=480;
		lcddev.height=320;
		
		
		
/*	
LCD_WR_REG(0xFF);   
LCD_WR_REG(0xFF);
delay_ms(5);
LCD_WR_REG(0xFF);
LCD_WR_REG(0xFF);
LCD_WR_REG(0xFF);
LCD_WR_REG(0xFF);
delay_ms(10);

LCD_WR_REG(0xB0);
LCD_WR_DATA(0x00);
LCD_WR_REG(0x11);
delay_ms(150);

LCD_WR_REG(0xB3);
LCD_WR_DATA(0x02);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x00);

LCD_WR_REG(0xC0);
LCD_WR_DATA(0x10);//13
LCD_WR_DATA(0x3B);//480
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x02);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x01);
LCD_WR_DATA(0x00);//NW
LCD_WR_DATA(0x43);

LCD_WR_REG(0xC1);
LCD_WR_DATA(0x10);//LCD_WR_DATA(0x08);
LCD_WR_DATA(0x10);//LCD_WR_DATA(0x16);//CLOCK
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x08);

LCD_WR_REG(0xC4);
LCD_WR_DATA(0x11);
LCD_WR_DATA(0x07);
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x03);

LCD_WR_REG(0xC6);
LCD_WR_DATA(0x00);

LCD_WR_REG(0xC8);//GAMMA
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x13);
LCD_WR_DATA(0x5C);
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x07);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x21);
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x07);
LCD_WR_DATA(0x53);
LCD_WR_DATA(0x0C);
LCD_WR_DATA(0x13);
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x21);
LCD_WR_DATA(0x00);

LCD_WR_REG(0x35);
LCD_WR_DATA(0x00);

LCD_WR_REG(0x36);
LCD_WR_DATA(0xE8);//设置扫描方向

LCD_WR_REG(0x3A);
LCD_WR_DATA(0x55);

LCD_WR_REG(0x44);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x01);

LCD_WR_REG(0xD0);
LCD_WR_DATA(0x07);
LCD_WR_DATA(0x07);//VCI1
LCD_WR_DATA(0x1D);//VRH

LCD_WR_REG(0xD1);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x03);//VCM
LCD_WR_DATA(0x00);//VDV

LCD_WR_REG(0xD2);
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x14);
LCD_WR_DATA(0x04);

LCD_WR_REG(0x29);
delay_ms(30);
LCD_WR_REG(0x2A);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x01);
LCD_WR_DATA(0xDF);//320

LCD_WR_REG(0x2B);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x01);
LCD_WR_DATA(0x3F);//480
LCD_WR_REG(0xB4);
LCD_WR_DATA(0x00);
delay_ms(100);
//LCD_WR_REG( 0x0021);  
LCD_WR_REG(0x2C);
	
			lcddev.width=480;
		lcddev.height=320;
		
		
	//LCD_Display_Dir(1);		 	//默认为竖屏
//	LCD_Scan_Dir(1);
	//LCD_LED=0;					//点亮背光
*/	
	

/*	LCD_SetCursor(0x00,0x0000);	//设置光标位置 
LCD_WR_REG(0x002C);  	
	for(ind0ex=0;ind0ex<153600;ind0ex++) 
	LCD_WR_DATA(YELLOW);   
	
	LCD_SetCursor(0x00,0x0000);	//设置光标位置 
LCD_WR_REG(0x002C);  	
	for(ind0ex=0;ind0ex<153600;ind0ex++)
	LCD_WR_DATA(RED);   	
	
	LCD_SetCursor(0x00,0x0000);	//设置光标位置 
LCD_WR_REG(0x002C);  	
	for(ind0ex=0;ind0ex<153600;ind0ex++)
	LCD_WR_DATA(GREEN);   		
	*/
	

	
 	 LCD_Clear(BLACK);
	
	
	
	//LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	//LCD_WR_REG(0x002C);  	
//	for(ind0ex=0;ind0ex<50000;ind0ex++) LCD_WR_DATA(RED);   
	//for(ind0ex=0;ind0ex<50000;ind0ex++) LCD_WR_DATA(GREEN);   
	//for(ind0ex=0;ind0ex<53600;ind0ex++) LCD_WR_DATA(BLUE);   
	//LCD_Clear(WHITE);
}  
//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
	/*u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//得到总点数
	if((lcddev.id==0X6804)&&(lcddev.dir==1))//6804横屏的时候特殊处理  
	{						    
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_SetCursor(0x00,0x0000);		//设置光标位置  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	LCD_WriteRAM_Prepare();     		//开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	   
	}*/
	
	
LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	for(ind0ex=0;ind0ex<153600;ind0ex++) 
	LCD_WR_DATA(color);   	
	
	
	
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;


		xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      				//设置光标位置 
			for(j=0;j<xlen;j++)
			LCD_WR_DATA(color);	//设置光标位置 	    
		}
	
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*height+j];//写入数据 
	}	  
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}  
//画虚线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawDottedLine(u16 x1, u16 y1, u16 x2, u16 y2)
{



u16 t; 
u16		iDotted=0;
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		
		
			
	if(iDotted<8)	POINT_COLOR=WHITE;	
	else	POINT_COLOR=BLACK;			
		
		iDotted++;
	if(iDotted>9)iDotted=0;
		
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  




}





//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR;      			     
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	if(!mode) //非叠加方式
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
			}  	 
	    }    
	}else//叠加方式
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y); 
				temp<<=1;
				y++;
				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        //if(x>=width){x=x0;y+=size;}
      //  if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}



void SetLcdParameter(uint16_t x,uint16_t y,uint16_t OffsetX,uint16_t OffsetY,uint16_t Height,uint16_t Width)
{

	LCD_WR_REG(0x2A);
	LCD_WR_DATA( (x+OffsetX*Width) >> 8);
	LCD_WR_DATA( (x+OffsetX*Width) & 0x00FF);
	LCD_WR_DATA( ((x+OffsetX*Width) + Width-1) >> 8)	;
	LCD_WR_DATA( ((x+OffsetX*Width) + Width-1) & 0x00FF);

	LCD_WR_REG(0x2B);
	LCD_WR_DATA( (y+OffsetY*Height) >> 8);
	LCD_WR_DATA( (y+OffsetY*Height) & 0x00FF);
	LCD_WR_DATA( ((y+OffsetY*Height) + Height-1) >> 8);
	LCD_WR_DATA( ((y+OffsetY*Height) + Height-1) & 0x00FF);
	
	LCD_WR_REG(0x002c);

	
	
  
}




char 	dispbuff[32];
unsigned int jj=0;			//字库查询辅助变量

extern const   struct typFNT_GB12 codeGB_12[];
extern  const   struct typFNT_GB16 codeGB_16[];
extern const    struct typFNT_GB24 codeGB_24[];
extern  const   struct typFNT_GB32 codeGB_32[];

extern  const   struct typFNT_GB32_16 codeGB_32_16[];
extern  const   struct typFNT_GB24_12 codeGB_24_12[];
extern  const   struct typFNT_GB48_24 codeGB_48_24[];





extern const struct  typFNT_GB100_72 codeGB100_72[];
extern const struct  typFNT_GB44_32 codeGB44_32[];
extern const struct  typFNT_GB32_32 codeGB32_32[];
extern const struct	 typENG_GB16_32 codeENG16_32[];
extern const struct  typFNT_GB21_16 codeGB21_16[];
extern const struct  typFNT_GB16_16 codeGB16_16[];


void dprintf_WORD16_16(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{
																	
  unsigned int i,j,k;	
 
  	uint16_t l=0;
	uint16_t c1;
	uint16_t c2;
	uint16_t ulen;

	k=0x80;	
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数
	
	while(l<ulen)
	{
		c1 = dispbuff[l];//把汉字拆分成两个区位码
		c2 = dispbuff[l+1];
		for(jj=0;jj<2;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
			{
				if(c1 == codeGB_16[jj].Index[0] && c2 == codeGB_16[jj].Index[1])
					break;
			}

			SetLcdParameter(x,y,l/2,0,16,16);			
		 												
		  for(i=0;i<32;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
		     	if(codeGB_16[jj].Msk[i]&k)
		     		LCD_WR_DATA(POINT_COLOR);
		     	else LCD_WR_DATA(BACK_COLOR);
			 			//LCD_WR_DATA((pstr[2*(y*480+x)]<<8)+pstr[2*(y*480+x)+1]);
			 	k=k>>1;
			
			 }
			 k=0x80;
			}
		x++;

	 	l++;
		l++;
	 }											 														  
}

void dprintf_WORD32_32(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{
																
  unsigned int i,j,k;	
 
  	uint16_t l=0;
	uint16_t c1;
	uint16_t c2;
	uint16_t ulen;

	k=0x80;	
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数
	
	while(l<ulen)
	{
		c1 = dispbuff[l];//把汉字拆分成两个区位码
		c2 = dispbuff[l+1];
		
		for(jj=0;jj<11;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
			{
				if(c1 == codeGB_32[jj].Index[0] && c2 == codeGB_32[jj].Index[1])
					break;
			}
		   			SetLcdParameter(x,y,l/2,0,32,32);													
		  for(i=0;i<128;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
		     	if(codeGB_32[jj].Msk[i]&k)
		     		LCD_WR_DATA(POINT_COLOR);
		     	else
			 	//	LCD_WR_DATA(BACK_COLOR);
					LCD_WR_DATA((pstr[2*(y*480+x)]<<8)+pstr[2*(y*480+x)+1]);
			 	k=k>>1;
			
			 }
			 k=0x80;
			}
		x++;

	 	l++;
		l++;
	 }											 														  
}

void dprintf_WORD24_24(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{
																
  unsigned int i,j,k;	
 
  	uint16_t l=0;
	uint16_t c1;
	uint16_t c2;
	uint16_t ulen;

	k=0x80;	
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//把汉字拆分成两个区位码
		c2 = 0xff&dispbuff[l+1];
		
		for(jj=0;jj<22;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
			{
				if(c1 == codeGB_24[jj].Index[0] && c2 == codeGB_24[jj].Index[1])
					break;
			}
		   			SetLcdParameter(x,y,l/2,0,24,24);													
		  for(i=0;i<72;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
		     	if(codeGB_24[jj].Msk[i]&k)
		     		LCD_WR_DATA(POINT_COLOR);
		     	else
			 		LCD_WR_DATA(BACK_COLOR);
			//		LCD_WR_DATA((pstr[2*(y*480+x)]<<8)+pstr[2*(y*480+x)+1]);
			 	k=k>>1;
			
			 }
			 k=0x80;
			}
		x++;

	 	l++;
		l++;
	 }											 														  
}


void dprintf_WORD24_24_Music(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{
																
  unsigned int i,j,k;	
 
  	uint16_t l=0;
	uint16_t c1;
	uint16_t c2;
	uint16_t ulen;

	k=0x80;	
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数
	
	while(l<ulen)
	{
		c1 = dispbuff[l];//把汉字拆分成两个区位码
		c2 = dispbuff[l+1];
		
		for(jj=0;jj<11;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
			{
				if(c1 == codeGB_24[jj].Index[0] && c2 == codeGB_24[jj].Index[1])
					break;
			}
		   			SetLcdParameter(x,y,l/2,0,24,24);													
		  for(i=0;i<72;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
		     	if(codeGB_24[jj].Msk[i]&k)
		     		LCD_WR_DATA(POINT_COLOR);
		     	else
			 	//	LCD_WR_DATA(BACK_COLOR);
					LCD_WR_DATA((pstr[2*(y*240+x)]<<8)+pstr[2*(y*240+x)+1]);
			 	k=k>>1;
			
			 }
			 k=0x80;
			}
		x++;

	 	l++;
		l++;
	 }											 														  
}

//==============================================================================================
//
//
//==============================================================================================
void dprintf_CHAR32_16(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{

	  	unsigned int i,j,k;	

	  	uint16_t l=0;
		uint16_t c1;
		uint16_t ulen;
		  							

		k=0x80;	
		ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数															


		while(l<ulen)
		{
			c1 = dispbuff[l];//把汉字拆分成两个区位码
			for(jj=0;jj<11;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
				{
					if(c1 == codeGB_32_16[jj].Index[0])
						break;
				}
		   SetLcdParameter(x,y,0,0,32,16);													
		  for(i=0;i<64;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
		     	if(codeGB_32_16[jj].Msk[i]&k)
		     		LCD_WR_DATA(POINT_COLOR);
		     	else
			 		LCD_WR_DATA((pstr[2*(y*480+x)]<<8)+pstr[2*(y*480+x)+1]);
			 	k=k>>1;
			
			 }
			 k=0x80;
			}
		x++;

	 	l++;

	 }	
}

//==============================================================================================
//
//
//==============================================================================================
void dprintf_CHAR24_12(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{

	  	unsigned int i,j,k;	

	  	uint16_t l=0;
		uint16_t c1;
		uint16_t ulen;
		  							

		k=0x80;	
		ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数															


		while(l<ulen)
		{
			c1 = dispbuff[l];//把汉字拆分成两个区位码
			for(jj=0;jj<11;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
				{
					if(c1 == codeGB_24_12[jj].Index[0])
						break;
				}
		   SetLcdParameter(x,y,0,0,32,16);													
		  for(i=0;i<48;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
					
					 
		     	if(codeGB_24_12[jj].Msk[i]&k)
					{
		      if(!(i%2==1&&j>4))		LCD_WR_DATA(POINT_COLOR);
					}
		     	else
			 		LCD_WR_DATA((pstr[2*(y*480+x)]<<8)+pstr[2*(y*480+x)+1]);
					
			 	k=k>>1;
			
			 }
			 k=0x80;
			}
		x++;

	 	l++;

	 }	
}

//==============================================================================================
//
//
//==============================================================================================
void dprintf_CHAR48_24(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{

	  	unsigned int i,j,k;	

	  	uint16_t l=0;
		uint16_t c1;
		uint16_t ulen;
		  		unsigned int m=0;							
	unsigned int n=0;				
		k=0x80;	
		ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数															


		while(l<ulen)
		{
			c1 = dispbuff[l];//把汉字拆分成两个区位码
			for(jj=0;jj<12;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
				{
					if(c1 == codeGB_48_24[jj].Index[0])
						break;
				}
		   SetLcdParameter(x,y,0,0,48,24);													
		  for(i=0;i<144;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
		     	if(codeGB_48_24[jj].Msk[i]&k)
		     		LCD_WR_DATA(POINT_COLOR);
		     	else
			 		LCD_WR_DATA((pstr[2*((y+n)*480+x+m)]<<8)+pstr[2*((y+n)*480+x+m)+1]);
			 	k=k>>1;
			
					
					m++;
					if(m>23)
					{
					m=0;
					n++;
					}
					
			 }
			 k=0x80;
			}
		x++;

	 	l++;

	 }	
}

/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart、yStart、Xend、Yend随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的起始点
          Xend   y方向的终止点
          Yend   y方向的终止点
返回值：无
***********************************************/
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	//HX8352-C

	LCD_WR_REG(0x2a);   
	LCD_WR_DATA(Xstart>>8);
	LCD_WR_DATA(Xstart&0xff);
	LCD_WR_DATA(Xend>>8);
	LCD_WR_DATA(Xend&0xff);

	LCD_WR_REG(0x2b);   
	LCD_WR_DATA(Ystart>>8);
	LCD_WR_DATA(Ystart&0xff);
	LCD_WR_DATA(Yend>>8);
	LCD_WR_DATA(Yend&0xff);


	LCD_WR_REG(0x2c);
}

#define Bank1_LCD_D    ((u32)0x60020000)    //Disp Data ADDR
#define Bank1_LCD_C    ((u32)0x60000000)	   //Disp Reg ADDR
/******************************************
函数名：Lcd图像填充100*100
功能：向Lcd指定位置填充图像
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
  unsigned long i;
	unsigned int j;

// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x0a); //横屏，从左下角开始，从左到右，从下到上
	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	for (i = 0; i < pic_H*pic_V*2; i+=2)
	{
		j=pic[i];
		j=j<<8;
		j=j+pic[i+1];
		*(__IO u16 *) (Bank1_LCD_D) = j;
	}
// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0xaa);
}



void dprintf_NUM100_72(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{

																	
  unsigned int i,j,k;	
 
  	uint16_t l=0;
	uint16_t c1;
	uint16_t c2;
	uint16_t ulen;

	k=0x80;	
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//把汉字拆分成两个区位码
		c2 = 0xff&dispbuff[l+1];
		
		for(jj=0;jj<22;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
			{
				if(c1 == codeGB100_72[jj].Index[0] && c2 == codeGB100_72[jj].Index[1])
					break;
			}
		   			SetLcdParameter(x,y,l/2,0,100,72);													
		  for(i=0;i<900;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
		     	if((codeGB100_72[jj].Msk[i]&k)&&w_b!=0)
		     		LCD_WR_DATA(POINT_COLOR);
		     	else
			 		LCD_WR_DATA(BACK_COLOR);
			 	k=k>>1;
			
			 }
			 k=0x80;
			}
		x++;

	 	l++;
		l++;
	 }			
}
void dprintf_NUM44_32(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{

																	
  unsigned int i,j,k;	
 
  	uint16_t l=0;
	uint16_t c1;
	uint16_t c2;
	uint16_t ulen;

	k=0x80;	
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//把汉字拆分成两个区位码
		c2 = 0xff&dispbuff[l+1];
		
		for(jj=0;jj<22;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
			{
				if(c1 == codeGB44_32[jj].Index[0] && c2 == codeGB44_32[jj].Index[1])
					break;
			}
		   			SetLcdParameter(x,y,l/2,0,44,32);													
		  for(i=0;i<176;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
		     	if((codeGB44_32[jj].Msk[i]&k)&&w_b!=0)
		     		LCD_WR_DATA(POINT_COLOR);
		     	else
			 		LCD_WR_DATA(BACK_COLOR);
			 	k=k>>1;
			
			 }
			 k=0x80;
			}
		x++;

	 	l++;
		l++;
	 }			
}


void dprintf_NUM32_32(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{

																	
  unsigned int i,j,k;	
 
  	uint16_t l=0;
	uint16_t c1;
	uint16_t c2;
	uint16_t ulen;

	k=0x80;	
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//把汉字拆分成两个区位码
		c2 = 0xff&dispbuff[l+1];
		
		 if( c1 > 0xA0 && c2 > 0xA0 )  //中文输出
		 {
			for(jj=0;jj<22;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
				{
					if(c1 == codeGB32_32[jj].Index[0] && c2 == codeGB32_32[jj].Index[1])
						break;
				}
							SetLcdParameter(x,y,l/2,0,32,32);													
				for(i=0;i<128;i++)													
				{																		
					for(j=0;j<8;j++)
					 {
						if((codeGB32_32[jj].Msk[i]&k)&&w_b!=0)
							LCD_WR_DATA(POINT_COLOR);
						else
						LCD_WR_DATA(BACK_COLOR);
					k=k>>1;
				
				 }
				 k=0x80;
				}
			x++;

			l++;
			l++;
		}else
		 {
		 
			 
			for(jj=0;jj<22;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
				{
					if(c1 == codeENG16_32[jj].Index[0])
						break;
				}
							SetLcdParameter(x,y,l,0,32,16);													
				for(i=0;i<64;i++)													
				{																		
					for(j=0;j<8;j++)
					 {
						if((codeENG16_32[jj].Msk[i]&k)&&w_b!=0)
							LCD_WR_DATA(POINT_COLOR);
						else
						LCD_WR_DATA(BACK_COLOR);
					k=k>>1;
				
				 }
				 k=0x80;
				}
			x++;

			l++;
	 
			 
			 
		 }
		 
		
		
	 }			
}
void dprintf_NUM21_16(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{

																	
  unsigned int i,j,k;	
 
  	uint16_t l=0;
	uint16_t c1;
	uint16_t c2;
	uint16_t ulen;

	k=0x80;	
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//把汉字拆分成两个区位码
		c2 = 0xff&dispbuff[l+1];
		
		for(jj=0;jj<22;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
			{
				if(c1 == codeGB21_16[jj].Index[0] && c2 == codeGB21_16[jj].Index[1])
					break;
			}
		   			SetLcdParameter(x,y,l/2,0,21,16);													
		  for(i=0;i<42;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
		     	if((codeGB21_16[jj].Msk[i]&k)&&w_b!=0)
		     		LCD_WR_DATA(POINT_COLOR);
		     	else
			 		LCD_WR_DATA(BACK_COLOR);
			 	k=k>>1;
			
			 }
			 k=0x80;
			}
		x++;

	 	l++;
		l++;
	 }			
}



void dprintf_NUM16_16(unsigned int x,unsigned int y,char str[20],unsigned char w_b,const unsigned char* pstr)
{

																	
  unsigned int i,j,k;	
 
  	uint16_t l=0;
	uint16_t c1;
	uint16_t c2;
	uint16_t ulen;

	k=0x80;	
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//字的个数
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//把汉字拆分成两个区位码
		c2 = 0xff&dispbuff[l+1];
		
		for(jj=0;jj<22;jj++)//寻找字库，计算字库长度，轮巡字库254个汉字
			{
				if(c1 == codeGB16_16[jj].Index[0] && c2 == codeGB16_16[jj].Index[1])
					break;
			}
		   			SetLcdParameter(x,y,l/2,0,16,16);													
		  for(i=0;i<32;i++)													
		  {																		
		    for(j=0;j<8;j++)
		     {
		     	if((codeGB16_16[jj].Msk[i]&k)&&w_b!=0)
		     		LCD_WR_DATA(POINT_COLOR);
		     	else
			 		LCD_WR_DATA(BACK_COLOR);
			 	k=k>>1;
			
			 }
			 k=0x80;
			}
		x++;

	 	l++;
		l++;
	 }			
}







char NUM_STR_B[3]={0,0,0};
char NUM_STR[]={"零一二三四五六七八九横"};


void Show_NUM100_72(unsigned int x,unsigned int y, unsigned char str,unsigned char front)
{

//NUM_STR_B[0]=NUM_STR[2*str];	
//NUM_STR_B[1]=NUM_STR[2*str+1];			
	
 //dprintf_NUM100_72(  x,  y,&NUM_STR_B[0],front,0);
	switch(str)
	{
		case 0:
			icon_show(x,  y, 65, 90, gImage_00_03,front);//显示			
			break;
		case 1:
			icon_show(x,  y, 65, 90, gImage_01_03,front);//显示			
			break;
		case 2:
			icon_show(x,  y, 65, 90, gImage_02_03,front);//显示			
			break;
		case 3:
			icon_show(x,  y, 65, 90, gImage_03_03,front);//显示			
			break;
		case 4:
			icon_show(x,  y, 65, 90, gImage_04_03,front);//显示			
			break;
		case 5:
			icon_show(x,  y, 65, 90, gImage_05_03,front);//显示			
			break;
		case 6:
			icon_show(x,  y, 65, 90, gImage_06_03,front);//显示			
			break;
		case 7:
			icon_show(x,  y, 65, 90, gImage_07_03,front);//显示			
			break;
		case 8:
			icon_show(x,  y, 65, 90, gImage_08_03,front);//显示			
			break;
		case 9:
			icon_show(x,  y, 65, 90, gImage_09_03,front);//显示			
			break;
		default:
			break;		
	}
	
}
void Show_NUM44_32(unsigned int x,unsigned int y, unsigned char str,unsigned char front)
{
//NUM_STR_B[0]=NUM_STR[2*str];	
//NUM_STR_B[1]=NUM_STR[2*str+1];		
 //dprintf_NUM44_32(  x,  y,&NUM_STR_B[0],front,0);
	
	switch(str)
	{
		case 0:
			icon_show(x,  y, 31, 45, gImage_10_03,front);//显示			
			break;
		case 1:
			icon_show(x,  y, 31, 45, gImage_11_03,front);//显示			
			break;
		case 2:
			icon_show(x,  y, 31, 45, gImage_12_03,front);//显示			
			break;
		case 3:
			icon_show(x,  y, 31, 45, gImage_13_03,front);//显示			
			break;
		case 4:
			icon_show(x,  y, 31, 45, gImage_14_03,front);//显示			
			break;
		case 5:
			icon_show(x,  y, 31, 45, gImage_15_03,front);//显示			
			break;
		case 6:
			icon_show(x,  y, 31, 45, gImage_16_03,front);//显示			
			break;
		case 7:
			icon_show(x,  y, 31, 45, gImage_17_03,front);//显示			
			break;
		case 8:
			icon_show(x,  y, 31, 45, gImage_18_03,front);//显示			
			break;
		case 9:
			icon_show(x,  y, 31, 45, gImage_19_03,front);//显示			
			break;
		default:
			break;		
	}
}

void Show_NUM21_16(unsigned int x,unsigned int y, unsigned char str,unsigned char front)
{
//NUM_STR_B[0]=NUM_STR[2*str];	
//NUM_STR_B[1]=NUM_STR[2*str+1];		
 //dprintf_NUM21_16(  x,  y,&NUM_STR_B[0],front,0);
	
	
	switch(str)
	{
		case 0:
			icon_show(x,  y, 15, 21, gImage_20_03,front);//显示			
			break;
		case 1:
			icon_show(x,  y, 15, 21, gImage_21_03,front);//显示			
			break;
		case 2:
			icon_show(x,  y, 15, 21, gImage_22_03,front);//显示			
			break;
		case 3:
			icon_show(x,  y, 15, 21, gImage_23_03,front);//显示			
			break;
		case 4:
			icon_show(x,  y, 15, 21, gImage_24_03,front);//显示			
			break;
		case 5:
			icon_show(x,  y, 15, 21, gImage_25_03,front);//显示			
			break;
		case 6:
			icon_show(x,  y, 15, 21, gImage_26_03,front);//显示			
			break;
		case 7:
			icon_show(x,  y, 15, 21, gImage_27_03,front);//显示			
			break;
		case 8:
			icon_show(x,  y, 15, 21, gImage_28_03,front);//显示			
			break;
		case 9:
			icon_show(x,  y, 15, 21, gImage_29_03,front);//显示			
			break;
		default:
			break;		
	}
	
	

}








