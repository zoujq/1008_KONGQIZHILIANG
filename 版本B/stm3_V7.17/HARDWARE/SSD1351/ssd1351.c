#include "includes.h"	   
#include "oledfont.h"	   



//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//.
//.
//.
//[95]0 1 2 3 ... 127	
//u16 OLED_GRAM[128][96];	 

//u16 OLED_GRAM[6561];	 

const u16 TAN_A[7]={0.1051,0.2126,0.3249,0.4452,0.5774,0.7265,0.9004};	 
const u16 TAN_AR[7]={4,8,13,18,23,29,36};	 

const float SIN_A[61]={0,0.1045,0.2079,0.3090,0.4067,0.5000,0.5878,0.6691,0.7431,0.8090,0.8660,0.9135,0.9511,0.9781,0.9945,1.0000,
											 0.9945,0.9781,0.9511,0.9135,0.8660,0.8090,0.7431,0.6691,0.5878,0.5000,0.4067,0.3090,0.2079,0.1045,0.0000,
											 -0.1045,-0.2079,-0.3090,-0.4067,-0.5000,-0.5878,-0.6691,-0.7431,-0.8090,-0.8660,-0.9135,-0.9511,-0.9781,-0.9945,-1.0000,
											 -0.9945,-0.9781,-0.9511,-0.9135,-0.8660,-0.8090,-0.7431,-0.6691,-0.5878,-0.5000,-0.4067,-0.3090,-0.2079,-0.1045,	0
};	 

const float COS_A[61]={1,0.9945,	0.9781,	0.9511,	0.9135,	0.8660,	0.8090,	0.7431,	0.6691,	0.5878,	0.5000,	0.4067,	0.3090,	0.2079,	0.1045,	0,
												 -0.1045,	-0.2079,	-0.3090,	-0.4067,	-0.5000,	-0.5878,	-0.6691,	-0.7431,	-0.8090,	-0.8660,	-0.9135,	-0.9511,	-0.9781,	-0.9945,	-1,
												 -0.9945,	-0.9781,	-0.9511,	-0.9135,	-0.8660,	-0.8090,	-0.7431,	-0.6691,	-0.5878,	-0.5000,	-0.4067,	-0.3090,	-0.2079,	-0.1045,	0	,
												 0.1045,	0.2079,	0.3090,	0.4067,	0.5000,	0.5878,	0.6691,	0.7431,	0.8090,	0.8660,	0.9135,	0.9511,	0.9781,	0.9945,	1
};


//π值定义
#define	app_pi	3.1415926535897932384626433832795 

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色


void Write_Register(unsigned int ins)
{
	//	DC=0;
	GPIOB->BRR  = GPIO_Pin_4;
	//CS=0;
	GPIOB->BRR  = GPIO_Pin_3;
	//  WRITE=0;	
	GPIOB->BRR = GPIO_Pin_11;
	//P0= ins;
	//GPIOA->ODR  &= 0xFF00;										 
	//GPIOA->ODR  |= (u16)ins;	
		GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((ins&0x00FE)|(ins&0x0001)<<11);			
	//WRITE=1;
	GPIOB->BSRR = GPIO_Pin_11;	
	//CS=1;
	GPIOB->BSRR  = GPIO_Pin_3;
}



void Write_Parameter(unsigned int dat)
{
	//DC=1;
	GPIOB->BSRR  = GPIO_Pin_4;
	//CS=0;
	GPIOB->BRR  = GPIO_Pin_3;
	// WRITE=0;
	GPIOB->BRR = GPIO_Pin_11;
	//P0= dat;
//	GPIOA->ODR  &= 0xFF00;										 
	//GPIOA->ODR  |= (u16)dat;	
			GPIOA->ODR  &= 0xF701;	
		GPIOA->ODR  |= (u16)((dat&0x00FE)|(dat&0x0001)<<11);		
	// WRITE=1;
	GPIOB->BSRR = GPIO_Pin_11;
	// CS=1;
	GPIOB->BSRR  = GPIO_Pin_3;
}





//初始化SSD1351
void SSD1351_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	
		//注意：不要忘记在RCC_Configuration()中开启AFIO时钟，
  //AFIO时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
// 改变指定管脚的映射 GPIO_Remap_SWJ_Disable SWJ 完全禁用（JTAG+SW-DP）
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
// 改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能


	//IO有改，不用了
	BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用

	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOB, ENABLE);	 //使能相关端口时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 	//PB10,PB11 输出高
	//GPIO_ResetBits(GPIOB,GPIO_Pin_10); 	//PB10 输出低
	
	/*	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;   //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	*/
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_4 | GPIO_Pin_5  |GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	

	
	
	//GPIO_SetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出高
	//GPIO_ResetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出低
	//	GPIO_SetBits(GPIOB,GPIO_Pin_10); 	//PB5 输出高	

}


void wr_address(unsigned int xstart,unsigned int xend,unsigned int ystart,unsigned int yend) 
{ 
  

  
Write_Register(0x0015);   
Write_Parameter(xstart);   //Column start address   
Write_Parameter(xend);   //Column end address   
Write_Register(0x0075);    
Write_Parameter(ystart);   //row start address  
Write_Parameter(yend);   //rou end address  

} 


void OLCD_Clear(u16 colora)//清屏
{
	unsigned int  i;
	

	wr_address(0,127,0,9);   //刷黑屏部分
	Write_Register(0x005c);       //RAM
	
	for( i =0 ; i<1280 ; i++ ) 
	{   
		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

		GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((0x0000&0xFE00)>>8|(0x0000&0x0100)<<3);			
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
		GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((0x0000&0x00FE)|(0x0000&0x0001)<<11);		
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;
	}
	wr_address(0,127,10,95);   //
	Write_Register(0x005c);       //RAM
	
	for( i =0 ; i<11008 ; i++ ) 
	{   
		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

	//	GPIOA->ODR  &= 0xFF00;										 
		//GPIOA->ODR  |= (u16)(colora>>8);
		GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((colora&0xFE00)>>8|(colora&0x0100)<<3);		
		
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(colora&0x00FF);
				GPIOA->ODR  &= 0xF701;		
	GPIOA->ODR  |= (u16)((colora&0x00FE)|(colora&0x0001)<<11);			
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;
	}
/*		wr_address(24,103,15,94);   //
	Write_Register(0x005c);       //RAM
	for( i =0 ; i<12288 ; i++ ) 
	{   
		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

		GPIOA->ODR  &= 0xFF00;										 
		GPIOA->ODR  |= (u16)(colora>>8);			
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
		GPIOA->ODR  &= 0xFF00;										 
		GPIOA->ODR  |= (u16)(colora&0x00FF);		
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;
	}
	
	
	*/
	
}  


void Initial_ssd1351(u8 sleep)
{
	
	
	
/*

	if(sleep==0)
	{
		
	Write_Register(0xAE); 
			GPIO_ResetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出低
		
	}
		else if(sleep==2)
		{
		*/
	if(sleep==2)
	{
			 SSD1351_Init();
		
			
		GPIO_ResetBits(GPIOB,GPIO_Pin_14); 	//PB5 输出高	
			GPIO_ResetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出低
	//	delay_ms(100);
			GPIO_SetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出低
		
		 Write_Register(0xfd); /*Command Lock */
 Write_Parameter(0xb1);

 Write_Register(0xAE); /*Set Sleep mode ON */

 Write_Register(0xA0);  /* Enable External VSL */
 Write_Parameter(0x70); /*0xf0 262k  1st option ;   0x64 65k*/

 Write_Register(0xA1);  /* DISPLAY START LINE */
 Write_Parameter(0x00);

 Write_Register(0xA2);  /* DISPLAY OFFSET */
 Write_Parameter(0x00);

 Write_Register(0xA6);  /* Set Display Mode */

 Write_Register(0xAB);  /* Function Selection */
 Write_Parameter(0x01); /* 16bit */

 Write_Register(0xB1); /* Set Reset (Phase 1) /Precharge (Phase2) period */
 Write_Parameter(0x53);

 Write_Register(0xB3);  /* Set Display Clock Divider/Oscillator Frequency */
 Write_Parameter(0xf1);

 Write_Register(0xB4);  /* Set Segment Low Voltage (VSL) */
 Write_Parameter(0xA0);
 Write_Parameter(0xB5);
 Write_Parameter(0x55);

 Write_Parameter(0xb9); /* Set built-in linear LUT */
 
 Write_Register(0xBB); /* Set Pre-charge voltage */
 Write_Parameter(0x00);

 Write_Register(0xBE);  /* Set VcomH */
 Write_Parameter(0x07);

 Write_Register(0xC1);  /* Set Contrast Current for Color A,B,C */
 Write_Parameter(0x66);/* Set Contrast For Color A */
 Write_Parameter(0x6D);/* Set Contrast For ColorB */
 Write_Parameter(0x8F); /* Set Contrast For ColorC */

 Write_Register(0xc7);  /* Master Contrast Current Control */
 Write_Parameter(0x07);

 Write_Register(0xCA);  /* Set MUX Ratio */
  Write_Parameter(0x5F);//control the y(0~95) long
	
				Write_Register(0xAE); 
			GPIO_ResetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出低
				delay_ms(1);
			GPIO_SetBits(GPIOB,GPIO_Pin_14); 	//PB5 输出高	
		
				//Write_Register(0xAF); 
		//  OLCD_Clear(0x00ff);
			  //OLCD_Clear(0x0fff);	
				  //OLCD_Clear(0x000f);
	}
	else 	if(sleep==1)
	{
GPIO_ResetBits(GPIOB,GPIO_Pin_14); 	//PB5 输出高	
			GPIO_ResetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出低
	//	delay_ms(100);
			GPIO_SetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出低
		
		 Write_Register(0xfd); /*Command Lock */
 Write_Parameter(0xb1);

 Write_Register(0xAE); /*Set Sleep mode ON */

 Write_Register(0xA0);  /* Enable External VSL */
 Write_Parameter(0x70); /*0xf0 262k  1st option ;   0x64 65k*/

 Write_Register(0xA1);  /* DISPLAY START LINE */
 Write_Parameter(0x00);

 Write_Register(0xA2);  /* DISPLAY OFFSET */
 Write_Parameter(0x00);

 Write_Register(0xA6);  /* Set Display Mode */

 Write_Register(0xAB);  /* Function Selection */
 Write_Parameter(0x01); /* 16bit */

 Write_Register(0xB1); /* Set Reset (Phase 1) /Precharge (Phase2) period */
 Write_Parameter(0x53);

 Write_Register(0xB3);  /* Set Display Clock Divider/Oscillator Frequency */
 Write_Parameter(0xf1);

 Write_Register(0xB4);  /* Set Segment Low Voltage (VSL) */
 Write_Parameter(0xA0);
 Write_Parameter(0xB5);
 Write_Parameter(0x55);

 Write_Parameter(0xb9); /* Set built-in linear LUT */
 
 Write_Register(0xBB); /* Set Pre-charge voltage */
 Write_Parameter(0x00);

 Write_Register(0xBE);  /* Set VcomH */
 Write_Parameter(0x07);

 Write_Register(0xC1);  /* Set Contrast Current for Color A,B,C */
 Write_Parameter(0x66);/* Set Contrast For Color A */
 Write_Parameter(0x6D);/* Set Contrast For ColorB */
 Write_Parameter(0x8F); /* Set Contrast For ColorC */

 Write_Register(0xc7);  /* Master Contrast Current Control */
 Write_Parameter(0x07);

 Write_Register(0xCA);  /* Set MUX Ratio */
  Write_Parameter(0x5F);//control the y(0~95) long
		Write_Register(0xAF); 
		

	
	
	}	else
{

				Write_Register(0xAE); /* Set Sleep mode Off/Display on */
			GPIO_ResetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出低
				delay_ms(1);
			GPIO_SetBits(GPIOB,GPIO_Pin_14); 	//PB5 输出高	
	
					delay_ms(1);
		//输出低，降功耗
GPIO_ResetBits(GPIOA,GPIO_Pin_11); 	
GPIO_ResetBits(GPIOA,GPIO_Pin_1); 	
GPIO_ResetBits(GPIOA,GPIO_Pin_2); 	
GPIO_ResetBits(GPIOA,GPIO_Pin_3); 	
GPIO_ResetBits(GPIOA,GPIO_Pin_4); 	
GPIO_ResetBits(GPIOA,GPIO_Pin_5); 	
GPIO_ResetBits(GPIOA,GPIO_Pin_6); 	
GPIO_ResetBits(GPIOA,GPIO_Pin_7); 	
	
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_3); 	//
		GPIO_ResetBits(GPIOB,GPIO_Pin_4); 	//
	GPIO_ResetBits(GPIOB,GPIO_Pin_11); 	//
	/*
GPIO_SetBits(GPIOB,GPIO_Pin_3); 	
GPIO_SetBits(GPIOB,GPIO_Pin_4); 	
GPIO_SetBits(GPIOB,GPIO_Pin_5); 	
GPIO_SetBits(GPIOB,GPIO_Pin_11); 

GPIO_SetBits(GPIOC,GPIO_Pin_13); 
	
*/

}	
		
		/*
		GPIO_SetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出高	
		Write_Register(0xAF); 
		
		OLCD_Clear(0x00ff);
		
					Write_Register(0xAE);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出低	
			
			
			
			GPIO_SetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出高	
		Write_Register(0xAF); 
		
		OLCD_Clear(0xff00);
		
		*/
	//	}
		//	else
	//	{
			

		//////	GPIO_ResetBits(GPIOB,GPIO_Pin_10); 	//PB5 输出低
		//////			delay_ms(10);
		//////GPIO_SetBits(GPIOB,GPIO_Pin_10); 	//PB5 输出高	
				//////			delay_ms(10);
				//		GPIO_SetBits(GPIOB,GPIO_Pin_5); 	//PB5 输出高	
			// SSD1351_Init();
	//User_Other_IO_Init();//?????IO????,?





	// Write_Register(0xAF); /* Set Sleep mode Off/Display on */

//
	//	Write_Register(0xAF); /* Set Sleep mode Off/Display on */
		
	//	}

}






//更新显存到LCD
void OLED_Refresh_Gram(void)
{
		unsigned int  i;
	
		wr_address(24,104,15,95);   //
	Write_Register(0x005c);       //RAM
	
	for( i =0 ; i<6561 ; i++ ) 
	{   
		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

		GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(OLED_GRAM[i]>>8);			
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
		GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(OLED_GRAM[i]&0x00FF);		
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;
	}
}
//开启OLED显示    
void OLED_Display_On(void)
{
	//OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	///OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	//OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
//	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	//OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数
void OLED_Clear(u16 color)  
{  
	
	unsigned int  i;
//	for(i=0;i<6400;i++)	OLED_GRAM[i]=color;  
	//OLED_Refresh_Gram();//更新显示

}

//画点 
//x:0~127
//y:0~95
//color:颜色 
void OLED_DrawPoint(u8 x,u8 y,u16 color)
{
	u16 pos,bx,temp=0;
	if(x>127||y>95)return;//超出范围了.
//	pos=81*y+x;
	
			wr_address(x,x,y,y);   //
	Write_Register(0x005c);       //RAM
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(color>>8);		
		GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((color&0xFE00)>>8|(color&0x0100)<<3);			
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(color&0x00FF);		
			GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((color&0x00FE)|(color&0x0001)<<11);	
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;
	//OLED_GRAM[pos]=color;
	
	//wr_address(24,104,15,95);   //
	//Write_Register(0x005c);       //RAM
	
	
//	bx=y%8;
	//temp=1<<(7-bx);
	//if(t)OLED_GRAM[x][pos]|=temp;
	//else OLED_GRAM[x][pos]&=~temp;	    
}

//画一条粗线(方法1)
//x0,y0:起点 x1,y1:终点
//size:线粗细,仅支持:0~2.
//color:颜色
void gui_draw_bline1(u16 x0,u16 y0,u16 x1,u16 y1,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x1-x0; //计算坐标增量 
	delta_y=y1-y0; 
	uRow=x0; 
	uCol=y0; 
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
		if(size==0&&t>3)OLED_DrawPoint(uRow,uCol,color);//画点 
		if(size==1&&t>3)
		{
			OLED_DrawPoint(uRow,uCol,color);//画点 
			OLED_DrawPoint(uRow+1,uCol,color);//画点 
			OLED_DrawPoint(uRow,uCol+1,color);//画点 
			OLED_DrawPoint(uRow+1,uCol+1,color);//画点 
		}
		if(size==2&&t>3)
		{
			OLED_DrawPoint(uRow,uCol,color);//画点 
			OLED_DrawPoint(uRow+1,uCol,color);//画点 
			OLED_DrawPoint(uRow,uCol+1,color);//画点 
			OLED_DrawPoint(uRow+1,uCol+1,color);//画点 
			OLED_DrawPoint(uRow-1,uCol+1,color);//画点 
			OLED_DrawPoint(uRow+1,uCol-1,color);//画点
			OLED_DrawPoint(uRow-1,uCol-1,color);//画点  
			OLED_DrawPoint(uRow-1,uCol,color);//画点 
			OLED_DrawPoint(uRow,uCol-1,color);//画点  
		}
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


//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}

//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	//if((len==0)||(x0>gui_phy.lcdwidth)||(y0>gui_phy.lcdheight))return;
//	gui_phy.fill(x0,y0,x0+len-1,y0,color);	
	gui_draw_bline1(x0,y0,x0+len-1,y0,0,color);
}



//画三角形，开始按键
//x0,y0:坐标
//r半径
//color:颜色
void gui_fill_keyStart(u16 x0,u16 y0,u16 r,u16 color,u16 back_color)
{

	u16 x,y,i,j,k,h,r0;
	h=5;
	r0=r-6-2*h;
	x=x0+3+h;
	y=y0+4;
//	j=x0+r;
	k=r0/2;
	
		for(i=0;i<=(r0+7);i++)
	{
	gui_draw_bline1(x0,y0+i,x0+r-1,y0+i,0,back_color); 
	}
	
	for(i=0;i<=r0;i++)
	{
	gui_draw_bline1(x,y+i,x+r0,y+k,0,color); 
	}
	gui_draw_bline1(x0+4,y0+2,x0+r0+7+h,y0+2,0,color); 
	gui_draw_bline1(x0+4,y0+r0+5,x0+r0+7+h,y0+r0+5,0,color); 

gui_draw_bline1(x0+h+1,y0-1,x0+h+1,y0+r0+5,0,color); 
gui_draw_bline1(x0+r0+8+h,y0-1,x0+r0+8+h,y0+r0+5,0,color); 	

}
//画三角形，暂停按键
//x0,y0:坐标
//r半径
//color:颜色
void gui_fill_keyPause(u16 x0,u16 y0,u16 r,u16 color,u16 back_color)
{

	u16 x,y,i,j,k,h,r0;
	h=5;
	r0=r-6-2*h;
	x=x0+3+h;
	y=y0+4;
//	j=x0+r;
	k=(r0-5)/2;
	
		for(i=0;i<=(r0+7);i++)
	{
	gui_draw_bline1(x0,y0+i,x0+r-1,y0+i,0,back_color); 
	}
	
	
	

gui_draw_bline1(x0+r0+4+h-9,y0+4,x0+r0+4+h-9,y0+r0,0,color); 	
gui_draw_bline1(x0+r0+3+h-9,y0+4,x0+r0+3+h-9,y0+r0,0,color); 	

gui_draw_bline1(x0+r0+4+h-4,y0+4,x0+r0+4+h-4,y0+r0,0,color); 	
gui_draw_bline1(x0+r0+3+h-4,y0+4,x0+r0+3+h-4,y0+r0,0,color); 		
	
	
	
	gui_draw_bline1(x0+4,y0+2,x0+r0+7+h,y0+2,0,color); 
	gui_draw_bline1(x0+4,y0+r0+5,x0+r0+7+h,y0+r0+5,0,color); 

gui_draw_bline1(x0+h+1,y0-1,x0+h+1,y0+r0+5,0,color); 
gui_draw_bline1(x0+r0+8+h,y0-1,x0+r0+8+h,y0+r0+5,0,color); 	
}
//画三角形，左进按键
//x0,y0:坐标
//r半径
//color:颜色
void gui_fill_keyLeft(u16 x0,u16 y0,u16 r,u16 color,u16 back_color)
{

	u16 x,y,i,j,k,h,r0;
	h=5;
	r0=r-6-2*h;
	x=x0+3+h;
	y=y0+4;
//	j=x0+r;
	k=(r0-5)/2;
	
		for(i=0;i<=(r0+7);i++)
	{
	gui_draw_bline1(x0,y0+i,x0+r-1,y0+i,0,back_color); 
	}
	
		for(i=0;i<=(r0-5);i++)
	{
	gui_draw_bline1(x+r0-5-2+5,y+i+2,x-2+5,y+k+2,0,color); 
	}
	
		for(i=0;i<=(r0-5);i++)
	{
	gui_draw_bline1(x+r0-5-2+6+5,y+i+2,x-2+6+5,y+k+2,0,color); 
	}
	

gui_draw_bline1(x0+r0+4+h-13,y0+4,x0+r0+4+h-13,y0+r0,0,color); 	
gui_draw_bline1(x0+r0+3+h-13,y0+4,x0+r0+3+h-13,y0+r0,0,color); 		
	
	
	
	gui_draw_bline1(x0+4,y0+2,x0+r0+7+h,y0+2,0,color); 
	gui_draw_bline1(x0+4,y0+r0+5,x0+r0+7+h,y0+r0+5,0,color); 

gui_draw_bline1(x0+h+1,y0-1,x0+h+1,y0+r0+5,0,color); 
gui_draw_bline1(x0+r0+8+h,y0-1,x0+r0+8+h,y0+r0+5,0,color); 	
}
//画三角形，右进按键
//x0,y0:坐标
//r半径
//color:颜色
void gui_fill_keyRight(u16 x0,u16 y0,u16 r,u16 color,u16 back_color)
{

	u16 x,y,i,j,k,h,r0;
	h=5;
	r0=r-6-2*h;
	x=x0+3+h;
	y=y0+4;
//	j=x0+r;
	k=(r0-5)/2;
	
		for(i=0;i<=(r0+7);i++)
	{
	gui_draw_bline1(x0,y0+i,x0+r-1,y0+i,0,back_color); 
	}
	

		for(i=0;i<=(r0-5);i++)
	{
	gui_draw_bline1(x-2+6,y+i+2,x+r0-5-2+6,y+k+2,0,color); 
	}
	
		for(i=0;i<=(r0-5);i++)
	{
	gui_draw_bline1(x-2,y+i+2,x+r0-5-2,y+k+2,0,color); 
	}
gui_draw_bline1(x0+r0+4+h,y0+4,x0+r0+4+h,y0+r0,0,color); 	
gui_draw_bline1(x0+r0+3+h,y0+4,x0+r0+3+h,y0+r0,0,color); 		
	
	
	
	gui_draw_bline1(x0+4,y0+2,x0+r0+7+h,y0+2,0,color); 
	gui_draw_bline1(x0+4,y0+r0+5,x0+r0+7+h,y0+r0+5,0,color); 

gui_draw_bline1(x0+h+1,y0-1,x0+h+1,y0+r0+5,0,color); 
gui_draw_bline1(x0+r0+8+h,y0-1,x0+r0+8+h,y0+r0+5,0,color); 	

}


//画实心圆
//x0,y0:坐标
//r半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax) 
		{
			// draw lines from outside  
			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}

//画圆形指针表盘
//x,y:坐标中心点
//size:表盘大小(直径)
//d:表盘分割,秒钟的高度
void calendar_circle_clock_drawpanel(u16 x,u16 y,u16 size,u16 d)
{
	u16 r=size/2;//得到半径 
	u16 sx=x-r+1;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1; 
	u16 i; 
//	gui_fill_circle(x,y,43,0xffff);		//画外圈
//	gui_fill_circle(x,y,42,0x0000);		//画内圈

	/*for(i=0;i<60;i++)//画秒钟格
	{ 
		px0=sx+r+(r-1)*SIN_A[i];
		py0=sy+r-(r-1)*COS_A[i]; 	
		px1=sx+r+r*SIN_A[i];
		py1=sy+r-r*COS_A[i]; 
		//gui_draw_bline1(px0,py0,px1,py1,0,0xffff);		
		OLED_DrawPoint(px0,py0,0xffff);//画点 
			OLED_DrawPoint(px1,py1,0xffff);//画点 

		
		
		
	}*/
	
	for(i=0;i<12;i++)//画小时格
	{ 
		px0=sx+r+(r-4)*SIN_A[i*5];
		py0=sy+r-(r-4)*COS_A[i*5]; 	
		px1=sx+r+r*SIN_A[i*5];
		py1=sy+r-r*COS_A[i*5]; 
		gui_draw_bline1(px0,py0,px1,py1,1,0xffff);		

	}
	for(i=0;i<4;i++)//画3小时格
	{ 
		px0=sx+r+(r-8)*SIN_A[i*15];
		py0=sy+r-(r-8)*COS_A[i*15]; 	
		px1=sx+r+r*SIN_A[i*15];
		py1=sy+r-r*COS_A[i*15];  
		gui_draw_bline1(px0,py0,px1,py1,2,0xffff);		
	}
	gui_fill_circle(x,y,4,0xffff);		//画中心圈
}






//显示时间
//x,y:坐标中心点
//size:表盘大小(直径)
//d:表盘分割,秒钟的高度
//hour:时钟
//min:分钟
//sec:秒钟
void calendar_circle_clock_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec)
{
	static u8 oldhour=0;	//最近一次进入该函数的时分秒信息
	static u8 oldmin=0;
	static u8 oldsec=0;
	static	u8 oldumin_for_hour=0;//分控制小时走一点步
	u16 r=size/2;//得到半径 
	u16 sx=x-r;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1;  
	u8 r1; 
	u8 umin_for_hour=0;//分控制小时走一点步
	
	
	
	
	//if(hour>11)hour-=12;
/*	if(sec==0)	gui_draw_bline1(40,40,40,0,0,RED); 
	else	if(sec>0&&sec<=7)	gui_draw_bline1(40,40,40+TAN_AR[sec-1],0,0,RED); 
	else	if(sec>7&&sec<15)		gui_draw_bline1(40,40,80,40-TAN_AR[14-sec],0,RED); 	
	else	if(sec==15)		gui_draw_bline1(40,40,80,40,0,RED); 		
	else	if(sec>15&&sec<=22)	gui_draw_bline1(40,40,80,40+TAN_AR[sec-16],0,RED); 
	else	if(sec>22&&sec<30)	gui_draw_bline1(40,40,40+TAN_AR[29-sec],80,0,RED); 	
	else	if(sec==30)		gui_draw_bline1(40,40,40,80,0,RED); 			
	else	if(sec>30&&sec<=37)	gui_draw_bline1(40,40,40-TAN_AR[sec-31],80,0,RED); 
	else	if(sec>37&&sec<45)	gui_draw_bline1(40,40,0,40+TAN_AR[44-sec],0,RED); 	
	else	if(sec==45)		gui_draw_bline1(40,40,0,40,0,RED); 		
	else	if(sec>45&&sec<=52)	gui_draw_bline1(40,40,0,40-TAN_AR[sec-46],0,RED); 
	else	if(sec>52&&sec<60)		gui_draw_bline1(40,40,40-TAN_AR[59-sec],0,0,RED); 
*/
	
	
		//显示新的秒钟
//	px0=sx+r+(r-d-7)*sin((app_pi/30)*sec); 
	//py0=sy+r-(r-d-7)*cos((app_pi/30)*sec); 
	//px1=sx+r+r1*sin((app_pi/30)*sec); 
	//py1=sy+r-r1*cos((app_pi/30)*sec); 

		//OLED_Refresh_Gram();//更新显示
	
	
//	sx=tan(6);
	
	umin_for_hour=min/12;
	
///////////////////////////////////////////////
	//清除小时
	//r1=d/2+4;
	//清除上一次的数据
	//if(oldmin!=min)
//	{
	px0=sx+r; 
	py0=sy+r; 	
	px1=sx+r+(r-20)*SIN_A[oldhour*5+oldumin_for_hour];; 
	py1=sy+r-(r-20)*COS_A[oldhour*5+oldumin_for_hour]; 
		//px1=sx+r+(r-20)*SIN_A[oldhour];; 
	//py1=sy+r-(r-20)*COS_A[oldhour]; 
	
	gui_draw_bline1(px0,py0,px1,py1,2,0x0000); 
	
	//清除分钟
	//r1=d/2+3;
	//清除上一次的数据

	px0=sx+r; 
	py0=sy+r; 	
	px1=sx+r+(r-12)*SIN_A[oldmin];; 
	py1=sy+r-(r-12)*COS_A[oldmin]; 
	gui_draw_bline1(px0,py0,px1,py1,1,0x0000); 
//	}
	//清除秒钟 
//	r1=d/2+3;
	//清除上一次的数据

	px0=sx+r; 
	py0=sy+r; 	
	px1=sx+r+(r-6)*SIN_A[oldsec];; 
	py1=sy+r-(r-6)*COS_A[oldsec]; 
	gui_draw_bline1(px0,py0,px1,py1,0,0x0000); 
	
///////////////////////////////////////////////


	//显示小时 
	//r1=d/2+4; 
	//显示新的时钟
	//	if(oldmin!=min)
	//{
	//	oldmin=min;
	px0=sx+r; 
	py0=sy+r; 	
	px1=sx+r+(r-20)*SIN_A[hour*5+umin_for_hour];; 
	py1=sy+r-(r-20)*COS_A[hour*5+umin_for_hour]; 
	//		px1=sx+r+(r-20)*SIN_A[hour];; 
//	py1=sy+r-(r-20)*COS_A[hour]; 
	gui_draw_bline1(px0,py0,px1,py1,2,0xf800); 
	
		
	//显示分钟 
	//r1=d/2+3; 
	//显示新的分钟

	px0=sx+r; 
	py0=sy+r; 	
	px1=sx+r+(r-12)*SIN_A[min];; 
	py1=sy+r-(r-12)*COS_A[min]; 
	gui_draw_bline1(px0,py0,px1,py1,1,0x07e0); 
	//}
	

	
	
	//显示秒钟  
	//r1=d/2+3;
	//显示新的秒钟
	//px0=sx+r+(r-d-7)*sin((app_pi/30)*sec); 
	//py0=sy+r-(r-d-7)*cos((app_pi/30)*sec); 

	px0=sx+r; 
	py0=sy+r; 	
	px1=sx+r+(r-6)*SIN_A[sec];; 
	py1=sy+r-(r-6)*COS_A[sec]; 
	gui_draw_bline1(px0,py0,px1,py1,0,0x001f); 
	//	OLED_Refresh_Gram();//更新显示
	oldhour=hour;	//保存时
	oldmin=min;		//保存分
	oldsec=sec;		//保存秒
	oldumin_for_hour=umin_for_hour;
}	    




//显示时间
//x,y:坐标中心点
//size:表盘大小(直径)
//d:表盘分割,秒钟的高度
//hour:时钟
//min:分钟
//sec:秒钟
void calendar_circle_clock_showtime1(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec)
{
	static u8 oldhour=0;	//最近一次进入该函数的时分秒信息
	static u8 oldmin=0;
	static u8 oldsec=0;
	u16 r=size/2;//得到半径 
	u16 sx=x-r;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1;  
	u8 r1; 
	if(hour>11)hour-=12;
	
	
//	sx=tan(6);
	
	
	/*
///////////////////////////////////////////////
	//清除小时
	r1=d/2+4;
	//清除上一次的数据
	px0=sx+r+(r-3*d-7)*sin((app_pi/6)*oldhour); 
	py0=sy+r-(r-3*d-7)*cos((app_pi/6)*oldhour); 
	px1=sx+r+r1*sin((app_pi/6)*oldhour); 
	py1=sy+r-r1*cos((app_pi/6)*oldhour); 
	gui_draw_bline1(px0,py0,px1,py1,2,BLACK);
	//清除分钟
	r1=d/2+3;
	//清除上一次的数据
	px0=sx+r+(r-2*d-7)*sin((app_pi/30)*oldmin); 
	py0=sy+r-(r-2*d-7)*cos((app_pi/30)*oldmin); 
	px1=sx+r+r1*sin((app_pi/30)*oldmin); 
	py1=sy+r-r1*cos((app_pi/30)*oldmin); 
	gui_draw_bline1(px0,py0,px1,py1,1,BLACK); 
	//清除秒钟 
	r1=d/2+3;
	//清除上一次的数据
	px0=sx+r+(r-d-7)*sin((app_pi/30)*oldsec); 
	py0=sy+r-(r-d-7)*cos((app_pi/30)*oldsec); 
	px1=sx+r+r1*sin((app_pi/30)*oldsec); 
	py1=sy+r-r1*cos((app_pi/30)*oldsec); 
	gui_draw_bline1(px0,py0,px1,py1,0,BLACK); 
///////////////////////////////////////////////

*/
	//显示小时 
	r1=d/2+4; 
	//显示新的时钟
	px0=sx+r+(r-3*d-7)*sin((app_pi/6)*hour); 
	py0=sy+r-(r-3*d-7)*cos((app_pi/6)*hour); 
	px1=sx+r+r1*sin((app_pi/6)*hour); 
	py1=sy+r-r1*cos((app_pi/6)*hour); 
	gui_draw_bline1(px0,py0,px1,py1,2,YELLOW); 
		OLED_Refresh_Gram();//更新显示
	//显示分钟 
	r1=d/2+3; 
	//显示新的分钟
	px0=sx+r+(r-2*d-7)*sin((app_pi/30)*min); 
	py0=sy+r-(r-2*d-7)*cos((app_pi/30)*min); 
	px1=sx+r+r1*sin((app_pi/30)*min); 
	py1=sy+r-r1*cos((app_pi/30)*min); 
	gui_draw_bline1(px0,py0,px1,py1,1,GREEN); 
	OLED_Refresh_Gram();//更新显示	
	//显示秒钟  
	r1=d/2+3;
	//显示新的秒钟
	px0=sx+r+(r-d-7)*sin((app_pi/30)*sec); 
	py0=sy+r-(r-d-7)*cos((app_pi/30)*sec); 
	px1=sx+r+r1*sin((app_pi/30)*sec); 
	py1=sy+r-r1*cos((app_pi/30)*sec); 
	gui_draw_bline1(px0,py0,px1,py1,0,RED); 
		OLED_Refresh_Gram();//更新显示
	oldhour=hour;	//保存时
	oldmin=min;		//保存分
	oldsec=sec;		//保存秒
}	    







//填充图标颜色
//x:
//y:
//iconame:图标
void OLED_DrawICO_Full(u8 x,u8 y,u16 color)
{


		unsigned int  i,j=0;
	
	u16 high_b=0,low_b=0;
	
			wr_address(x,x+59,y,y+59);   //
	Write_Register(0x005c);       //RAM
	
	for( i =0 ; i<7200 ; i+=2 ) 
	{  
		
	

		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(color>>8);		
		GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((color&0xFE00)>>8|(color&0x0100)<<3);			
	
	
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(color&0x00FF);	
	
	
				GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((color&0x00FE)|(color&0x0001)<<11);			
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;	
	}
	
	
}

//画图标
//x:
//y:
//iconame:图标
void OLED_DrawICO(u8 x,u8 y,const unsigned char bmp[])
{


		unsigned int  i,j=0;
	
	u16 high_b=0,low_b=0;
	
			wr_address(x,x+59,y,y+59);   //
	Write_Register(0x005c);       //RAM
	
	for( i =0 ; i<7200 ; i+=2 ) 
	{  
		
	

		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(bmp[i]);			
				GPIOA->ODR  &= 0xF701;				
					GPIOA->ODR  |= (u16)((bmp[i]&0x00FE)|(bmp[i]&0x0001)<<11);	
	
	
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(bmp[i+1]);		
			
	
				GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((bmp[i+1]&0x00FE)|(bmp[i+1]&0x0001)<<11);	
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;	
	}
	
	
}
//画图标,缩小一半的图标
//x:
//y:
//iconame:图标
void OLED_DrawICO_Small(u8 x,u8 y,const unsigned char bmp[])
{


		unsigned int  i,j=0;
	
	u16 high_b=0,low_b=0;
	
			wr_address(x,x+29,y,y+29);   //
	Write_Register(0x005c);       //RAM
	
	for( i =0 ; i<7200 ; i+=2 ) 
	{  
		if(i%4==0&&((i/120)%2)==0)
		{
	

		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(bmp[i]);			
							GPIOA->ODR  &= 0xF701;				
			GPIOA->ODR  |= (u16)((bmp[i]&0x00FE)|(bmp[i]&0x0001)<<11);	
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(bmp[i+1]);		
							GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((bmp[i+1]&0x00FE)|(bmp[i+1]&0x0001)<<11);	
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;	
		}
	}
	
	
}
//画图标,大小30*30
//x:
//y:
//iconame:图标
void OLED_DrawICO_3030(u8 x,u8 y,const unsigned char bmp[])
{


		unsigned int  i,j=0;
	
	u16 high_b=0,low_b=0;
	
			wr_address(x,x+29,y,y+29);   //
	Write_Register(0x005c);       //RAM
	
	for( i =0 ; i<1800 ; i+=2 ) 
	{  
	//	if(i%4==0&&((i/120)%2)==0)
	//	{
	

		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(bmp[i]);			
							GPIOA->ODR  &= 0xF701;				
			GPIOA->ODR  |= (u16)((bmp[i]&0x00FE)|(bmp[i]&0x0001)<<11);	
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(bmp[i+1]);		
							GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((bmp[i+1]&0x00FE)|(bmp[i+1]&0x0001)<<11);	
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;	
	//	}
	}
	
	
}


void OLED_DrawPIC(u8 x,u8 y,const unsigned char bmp[])
{

u16 i;
		
			wr_address(x,x+79,y,y+79);   //
	Write_Register(0x005c);       //RAM
	
	for( i =0 ; i<12288 ; i+=2 ) 
	{  
		
		

		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

	//	GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(bmp[i]);			
									GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((bmp[i]&0x00FE)|(bmp[i]&0x0001)<<11);	
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
		//GPIOA->ODR  &= 0xFF00;										 
		//GPIOA->ODR  |= (u16)(bmp[i+1]);
				GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((bmp[i+1]&0x00FE)|(bmp[i+1]&0x0001)<<11);			
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;	
	}
}


//画图标,选中，换颜色
//x:
//y:
//iconame:图标
void OLED_DrawICO_Choose(u8 x,u8 y,const unsigned char bmp[])
{


		unsigned int  i,j=0;
	
	u16 high_b=0,low_b=0;
	
			wr_address(x-2,x+29+2,y-2,y+29+2);   //
	Write_Register(0x005c);       //RAM
	
	for( i =0 ; i<2312 ; i+=2 ) 
	{  
		
		if(((i%68)<4)||((i%68)>62)||i<136||(i>(2312-136)))
		{

high_b=0xff;
low_b=0xff;

}
else
{
	high_b=bmp[j];
low_b=bmp[j+1];	
	j+=2;
}

		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

		//GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(high_b);		
							GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((high_b&0xFE00)>>8|(high_b&0x0100)<<3);		
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
		//GPIOA->ODR  &= 0xFF00;										 
	//	GPIOA->ODR  |= (u16)(low_b);	
				GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((low_b&0x00FE)|(low_b&0x0001)<<11);	
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;	
	}
}

//在指定位置显示一个字符,包括部分字符
//x:0~(127-15)
//y:0~(95-30)
//ch:字符			 
//color_front:字体颜色
//color_back:背景颜色
void OLED_ShowChar(u8 x,u8 y,unsigned char ch,u16 color_front, u16 color_back)
{      			    
	
	
		//	unsigned int  i;
	
       unsigned short int i,j,k;
       unsigned char mask,buf;
	unsigned char const  *pZK;
	
	wr_address(x,x+14,y,y+29);   //
	Write_Register(0x005c);       //RAM
	
	
	for(k=0;k<63;k++){
	if(ascii_3030[k].index==ch){
       pZK = &(ascii_3030[k].msk[0]);
       //for( i = 0 ; i < 32 ; i++ )
       for( i = 0 ; i < 60 ; i+=2 )
       {
							//前半字
              mask = 0x80;
              buf = pZK[i];
              for( j = 0 ; j < 8 ; j++ )
              {
								 if( buf & mask )
								 {                           	
										//delayus(0);//好像是速度太快，必需加这个
										GPIOB->BSRR  = GPIO_Pin_4;
										GPIOB->BRR  = GPIO_Pin_3;
										GPIOB->BRR = GPIO_Pin_11;

										//GPIOA->ODR  &= 0xFF00;										 
										//GPIOA->ODR  |= (u16)(color_front>>8);			
									 			GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((color_front&0xFE00)>>8|(color_front&0x0100)<<3);	
										//delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BRR = GPIO_Pin_11;
										//delayus(10);
									//	GPIOA->ODR  &= 0xFF00;										 
									//	GPIOA->ODR  |= (u16)(color_front&0x00FF);
				GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((color_front&0x00FE)|(color_front&0x0001)<<11);										 
										//	delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BSRR  = GPIO_Pin_3;
								 }
								 else
								 {
										//delayus(0);//好像是速度太快，必需加这个
										GPIOB->BSRR  = GPIO_Pin_4;
										GPIOB->BRR  = GPIO_Pin_3;
										GPIOB->BRR = GPIO_Pin_11;

								//		GPIOA->ODR  &= 0xFF00;										 
								//		GPIOA->ODR  |= (u16)(color_back>>8);	
								 			GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((color_back&0xFE00)>>8|(color_back&0x0100)<<3);									 
										//delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BRR = GPIO_Pin_11;
										//delayus(10);
								//		GPIOA->ODR  &= 0xFF00;										 
								//		GPIOA->ODR  |= (u16)(color_back&0x00FF);		
									 			GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((color_back&0x00FE)|(color_back&0x0001)<<11);	
										//	delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BSRR  = GPIO_Pin_3;
								 }
								 mask = mask >> 1;
              }
						//后半字
              mask = 0x80;
              buf = pZK[i+1];
              for( j = 0 ; j < 7 ; j++ )
              {
								 if( buf & mask )
								 {                           	
										//delayus(0);//好像是速度太快，必需加这个
										GPIOB->BSRR  = GPIO_Pin_4;
										GPIOB->BRR  = GPIO_Pin_3;
										GPIOB->BRR = GPIO_Pin_11;

									//	GPIOA->ODR  &= 0xFF00;										 
									//	GPIOA->ODR  |= (u16)(color_front>>8);		
	 			GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((color_front&0xFE00)>>8|(color_front&0x0100)<<3);											 
										//delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BRR = GPIO_Pin_11;
										//delayus(10);
					//					GPIOA->ODR  &= 0xFF00;										 
								//		GPIOA->ODR  |= (u16)(color_front&0x00FF);		
									 									 			GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((color_front&0x00FE)|(color_front&0x0001)<<11);	
										//	delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BSRR  = GPIO_Pin_3;
								 }
								 else
								 {
										//delayus(0);//好像是速度太快，必需加这个
										GPIOB->BSRR  = GPIO_Pin_4;
										GPIOB->BRR  = GPIO_Pin_3;
										GPIOB->BRR = GPIO_Pin_11;

									//	GPIOA->ODR  &= 0xFF00;										 
									//	GPIOA->ODR  |= (u16)(color_back>>8);			
										GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((color_back&0xFE00)>>8|(color_back&0x0100)<<3);		 
										//delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BRR = GPIO_Pin_11;
										//delayus(10);
									//	GPIOA->ODR  &= 0xFF00;										 
									//	GPIOA->ODR  |= (u16)(color_back&0x00FF);
									 									 			GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((color_back&0x00FE)|(color_back&0x0001)<<11);										 
										//	delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BSRR  = GPIO_Pin_3;
								 }
								 mask = mask >> 1;
              }							
							
       }
			 break;
       }
			 
       }
	
	
	
	
	/*	
	

	
	
	
	
	
	
	
		//delayus(0);//好像是速度太快，必需加这个
		GPIOB->BSRR  = GPIO_Pin_4;
		GPIOB->BRR  = GPIO_Pin_3;
		GPIOB->BRR = GPIO_Pin_11;

		GPIOA->ODR  &= 0xFF00;										 
		GPIOA->ODR  |= (u16)(OLED_GRAM[i]>>8);			
		//delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BRR = GPIO_Pin_11;
		//delayus(10);
		GPIOA->ODR  &= 0xFF00;										 
		GPIOA->ODR  |= (u16)(OLED_GRAM[i]&0x00FF);		
		//	delayus(10);
		GPIOB->BSRR = GPIO_Pin_11;
		GPIOB->BSRR  = GPIO_Pin_3;
	
	
	*/
	
	
	
	
	
	
	
	
	
	
	/*
		   
    for(t=0;t<size;t++)
    {   
		if(size==30)temp=oled_asc2_1206[chr][t];  //调用1206字体	
		else	if(size==12)temp=oled_asc2_1206[chr][t];  //调用1206字体
		else temp=oled_asc2_1608[chr][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } */         
}
//在指定位置显示一个字符,包括部分字符
//x:0~(127-15)
//y:0~(95-30)
//ch:字符			 
//color_front:字体颜色
//color_back:背景颜色
void OLED_ShowChar2020(u8 x,u8 y,unsigned char ch,u16 color_front, u16 color_back)
{      			    
	
	
		//	unsigned int  i;
	
       unsigned short int i,j,k;
       unsigned char mask,buf;
	   unsigned char const *pZK;
	
	wr_address(x,x+9,y,y+19);   //
	Write_Register(0x005c);       //RAM
	
	
	for(k=0;k<67;k++){
	if(ascii_2020[k].index==ch){
       pZK = &(ascii_2020[k].msk[0]);
       //for( i = 0 ; i < 32 ; i++ )
       for( i = 0 ; i < 40 ; i+=2 )
       {
							//前半字
              mask = 0x80;
              buf = pZK[i];
              for( j = 0 ; j < 8 ; j++ )
              {
								 if( buf & mask )
								 {                           	
										//delayus(0);//好像是速度太快，必需加这个
										GPIOB->BSRR  = GPIO_Pin_4;
										GPIOB->BRR  = GPIO_Pin_3;
										GPIOB->BRR = GPIO_Pin_11;

									//	GPIOA->ODR  &= 0xFF00;										 
									//	GPIOA->ODR  |= (u16)(color_front>>8);		
									 GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((color_front&0xFE00)>>8|(color_front&0x0100)<<3);											 
										//delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BRR = GPIO_Pin_11;
										//delayus(10);
									//	GPIOA->ODR  &= 0xFF00;										 
									//	GPIOA->ODR  |= (u16)(color_front&0x00FF);		
									GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((color_front&0x00FE)|(color_front&0x0001)<<11);		
										//	delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BSRR  = GPIO_Pin_3;
								 }
								 else
								 {
										//delayus(0);//好像是速度太快，必需加这个
										GPIOB->BSRR  = GPIO_Pin_4;
										GPIOB->BRR  = GPIO_Pin_3;
										GPIOB->BRR = GPIO_Pin_11;

							//			GPIOA->ODR  &= 0xFF00;										 
									//	GPIOA->ODR  |= (u16)(color_back>>8);			
										 GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((color_back&0xFE00)>>8|(color_back&0x0100)<<3);									 
										//delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BRR = GPIO_Pin_11;
										//delayus(10);
							//			GPIOA->ODR  &= 0xFF00;										 
								//		GPIOA->ODR  |= (u16)(color_back&0x00FF);	
									GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((color_back&0x00FE)|(color_back&0x0001)<<11);										 
										//	delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BSRR  = GPIO_Pin_3;
								 }
								 mask = mask >> 1;
              }
						//后半字
              mask = 0x80;
              buf = pZK[i+1];
              for( j = 0 ; j < 2 ; j++ )
              {
								 if( buf & mask )
								 {                           	
										//delayus(0);//好像是速度太快，必需加这个
										GPIOB->BSRR  = GPIO_Pin_4;
										GPIOB->BRR  = GPIO_Pin_3;
										GPIOB->BRR = GPIO_Pin_11;

								//		GPIOA->ODR  &= 0xFF00;										 
								//		GPIOA->ODR  |= (u16)(color_front>>8);			
																 GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((color_front&0xFE00)>>8|(color_front&0x0100)<<3);				 
										//delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BRR = GPIO_Pin_11;
										//delayus(10);
							//			GPIOA->ODR  &= 0xFF00;										 
								//		GPIOA->ODR  |= (u16)(color_front&0x00FF);		
									 									GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((color_front&0x00FE)|(color_front&0x0001)<<11);	
										//	delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BSRR  = GPIO_Pin_3;
								 }
								 else
								 {
										//delayus(0);//好像是速度太快，必需加这个
										GPIOB->BSRR  = GPIO_Pin_4;
										GPIOB->BRR  = GPIO_Pin_3;
										GPIOB->BRR = GPIO_Pin_11;

										//GPIOA->ODR  &= 0xFF00;										 
									//	GPIOA->ODR  |= (u16)(color_back>>8);	
																			 GPIOA->ODR  &= 0xF701;				
				GPIOA->ODR  |= (u16)((color_back&0xFE00)>>8|(color_back&0x0100)<<3);			 
										//delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BRR = GPIO_Pin_11;
										//delayus(10);
									//	GPIOA->ODR  &= 0xFF00;										 
									//	GPIOA->ODR  |= (u16)(color_back&0x00FF);	
						 									GPIOA->ODR  &= 0xF701;										 
		GPIOA->ODR  |= (u16)((color_back&0x00FE)|(color_back&0x0001)<<11);										 
										//	delayus(10);
										GPIOB->BSRR = GPIO_Pin_11;
										GPIOB->BSRR  = GPIO_Pin_3;
								 }
								 mask = mask >> 1;
              }							
							
       }
			 break;
       }
			 
       }    
}
//OLED初始化
void OLED_Init(void)
{
	u16 xoff=0;
	u16 yoff=0;	//表盘y偏移量
	u16 r=0;	//表盘半径
	u8 d=0;		//指针长度
	
	float a,b;
	
	
/*	xoff=-1;
	yoff=-8;
	r=xoff*yoff;
	r=80;
	a=-0.123;
	b=r*a;
	
	
	*/
	
	
 Initial_ssd1351(2);


      //  OLCD_Clear(0xf800);
      //    OLCD_Clear(0x07e0);
         //   OLCD_Clear(0x001f);
          // OLCD_Clear(0x0000);




 //OLED_DrawPIC(10,10,gImage_2222222);

//OLED_DrawPoint(100,50,0xffff);
	//OLED_Refresh_Gram();//更新显示

	//gui_draw_bline1(0,0,80,80,0,0xff00);
	//gui_fill_circle(40,40,40,0xff00);
	
	      //OLCD_Clear(0x0000);
	//calendar_circle_clock_drawpanel(63,53,84,2);//显示指针时钟表盘 
	//	calendar_circle_clock_showtime(64,53,80,3,10,0,r);
	//OLED_Clear(0xf800);
	
	//calendar_circle_clock_showtime(40,40,80,3,10,1,30);



	
	
	/*	gui_draw_bline1(40,40,40,0,0,RED); 
				gui_draw_bline1(40,40,40+TAN_AR[0],0,0,RED); 
				gui_draw_bline1(40,40,40+TAN_AR[1],0,0,RED); 
			gui_draw_bline1(40,40,40+TAN_AR[2],0,0,RED); 
				gui_draw_bline1(40,40,40+TAN_AR[3],0,0,RED); 
					gui_draw_bline1(40,40,40+TAN_AR[4],0,0,RED); 
						gui_draw_bline1(40,40,40+TAN_AR[5],0,0,RED); 
							gui_draw_bline1(40,40,40+TAN_AR[6],0,0,RED); 
								gui_draw_bline1(40,40,80,40-TAN_AR[6],0,RED); 
								gui_draw_bline1(40,40,80,40-TAN_AR[5],0,RED); 
								gui_draw_bline1(40,40,80,40-TAN_AR[4],0,RED); 
								gui_draw_bline1(40,40,80,40-TAN_AR[3],0,RED); 
								gui_draw_bline1(40,40,80,40-TAN_AR[2],0,RED); 
								gui_draw_bline1(40,40,80,40-TAN_AR[1],0,RED); 
								gui_draw_bline1(40,40,80,40-TAN_AR[0],0,RED); 
		*/
	//OLED_Refresh_Gram();//更新显示
	
//	void OLED_ShowChar(u8 x,u8 y,unsigned char ch,u16 color_front, u16 color_back);
//	OLED_ShowChar(48,48,'1',0x00ff,0xff00);//显示ASCII字符	   

}


//OLED显示打开
void OLED_On(void)
{


Initial_ssd1351(1);//重新初始化
	// Write_Register(0xAF); /* Set Sleep mode Off/Display on */

  OLCD_Clear(0x0000);
}
	
	
//OLED显示关闭
void OLED_Off(void)
{

Initial_ssd1351(0);//重新初始化

//	 Write_Register(0xAE); /* Set Sleep mode Off/Display on */


}
		
	









