#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"
#include "icon.h"


#include "includes.h"	   

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//2.4��/2.8��/3.5��/4.3�� TFTҺ������	  
//֧������IC�ͺŰ���:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/SSD1289/1505/B505/C505/NT35310/NT35510��		    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/2/11
//�汾��V2.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//V1.2�޸�˵��
//֧����SPFD5408������,�����Һ��IDֱ�Ӵ�ӡ��HEX��ʽ.����鿴LCD����IC.
//V1.3
//�����˿���IO��֧��
//�޸��˱�����Ƶļ��ԣ�������V1.8���Ժ�Ŀ�����汾��
//����1.8�汾֮ǰ(������1.8)��Һ��ģ��,���޸�LCD_Init������LCD_LED=1;ΪLCD_LED=1;
//V1.4
//�޸���LCD_ShowChar������ʹ�û��㹦�ܻ��ַ���
//�����˺�������ʾ��֧��
//V1.5 20110730
//1,�޸���B505Һ������ɫ�����bug.
//2,�޸��˿���IO�������������÷�ʽ.
//V1.6 20111116
//1,�����LGDP4535Һ��������֧��
//V1.7 20120713
//1,����LCD_RD_DATA����
//2,���Ӷ�ILI9341��֧��
//3,����ILI9325�Ķ�����������
//4,����LCD_Scan_Dir����(����ʹ��)	  
//6,�����޸��˲���ԭ���ĺ���,����Ӧ9341�Ĳ���
//V1.8 20120905
//1,����LCD��Ҫ�������ýṹ��lcddev
//2,����LCD_Display_Dir����,֧�����ߺ������л�
//V1.9 20120911
//1,����RM68042������ID:6804��������6804��֧�ֺ�����ʾ����ԭ�򣺸ı�ɨ�跽ʽ��
//����6804��������ʧЧ���Թ��ܶ෽�������У���ʱ�޽⡣
//V2.0 20120924
//�ڲ�Ӳ����λ�������,ILI9341��ID��ȡ�ᱻ�����9300,�޸�LCD_Init,���޷�ʶ��
//�����������IDΪ9300/�Ƿ�ID��,ǿ��ָ������ICΪILI9341��ִ��9341�ĳ�ʼ����
//V2.1 20120930
//����ILI9325����ɫ��bug��
//V2.2 20121007
//����LCD_Scan_Dir��bug��
//V2.3 20130120
//����6804֧�ֺ�����ʾ
//V2.4 20131120
//1,����NT35310��ID:5310����������֧��
//2,����LCD_Set_Window����,�������ô���,�Կ������,�Ƚ�����,���Ǹú����ں���ʱ,��֧��6804.
//V2.5 20140211
//1,����NT35510��ID:5510����������֧��
//////////////////////////////////////////////////////////////////////////////////	 
				 
//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 



		u32 ind0ex=0;      
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
	 
//д�Ĵ�������
//regval:�Ĵ���ֵ
void LCD_WR_REG(u16 regval)
{ 
	LCD->LCD_REG=regval;//д��Ҫд�ļĴ������	 
}
//дLCD����
//data:Ҫд���ֵ
void LCD_WR_DATA(u16 data)
{										    	   
	LCD->LCD_RAM=data;		 
}
//��LCD����
//����ֵ:������ֵ
u16 LCD_RD_DATA(void)
{										    	   
	return LCD->LCD_RAM;		 
}					   
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 
	LCD->LCD_RAM = LCD_RegValue;//д������	    		 
}	   
//���Ĵ���
//LCD_Reg:�Ĵ�����ַ
//����ֵ:����������
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
	delay_us(5);		  
	return LCD_RD_DATA();		//���ض�����ֵ
}   
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 
//LCDдGRAM
//RGB_Code:��ɫֵ
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//дʮ��λGRAM
}
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(u8 i)
{
	while(i--);
}
//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���		   
	LCD_SetCursor(x,y);	    
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)LCD_WR_REG(0X2E);//9341/6804/3510 ���Ͷ�GRAMָ��
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2E00);	//5510 ���Ͷ�GRAMָ��
	else LCD_WR_REG(R34);      		 				//����IC���Ͷ�GRAMָ��
 	if(lcddev.id==0X9320)opt_delay(2);				//FOR 9320,��ʱ2us	    
	if(LCD->LCD_RAM)r=0;							//dummy Read	   
	opt_delay(2);	  
 	r=LCD->LCD_RAM;  		  						//ʵ��������ɫ
 	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)		//9341/NT35310/NT35510Ҫ��2�ζ���
 	{
		opt_delay(2);	  
		b=LCD->LCD_RAM; 
		g=r&0XFF;		//����9341/5310/5510,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
		g<<=8;
	}else if(lcddev.id==0X6804)r=LCD->LCD_RAM;		//6804�ڶ��ζ�ȡ�Ĳ�����ʵֵ 
	if(lcddev.id==0X9325||lcddev.id==0X4535||lcddev.id==0X4531||lcddev.id==0X8989||lcddev.id==0XB505)return r;	//�⼸��ICֱ�ӷ�����ɫֵ
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510��Ҫ��ʽת��һ��
	else return LCD_BGR2RGB(r);						//����IC
}			 
//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)LCD_WR_REG(0X29);	//������ʾ
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2900);	//������ʾ
	else LCD_WriteReg(R7,0x0173); 				 	//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)LCD_WR_REG(0X28);	//�ر���ʾ
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2800);	//�ر���ʾ
	else LCD_WriteReg(R7,0x0);//�ر���ʾ 
}   
//���ù��λ��
//Xpos:������
//Ypos:������
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
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//����ʱ����
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
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//������ʵ���ǵ�תx,y����
		LCD_WriteReg(lcddev.setxcmd, Xpos);
		LCD_WriteReg(lcddev.setycmd, Ypos);
	}	 */
	
 /* LCD_WR_REG(0x002A);LCD_WR_DATA(Ypos>>8);
  LCD_WR_DATA(0x00FF&Ypos);                 //�趨X����
  LCD_WR_DATA(0x0001);LCD_WR_DATA(0x003F);

  LCD_WR_REG(0x002B);LCD_WR_DATA((480-Xpos)>>8);
  LCD_WR_DATA(0x00FF&(480-Xpos));           //�趨Y����
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

//�и���ʹ����㣬���㲻����480��320��ҪΪ479��319����Ȼ����û�ã���������һ���ַ�ˢ���ģ�ˢȫ��ֻ��ʾ����
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

//����LCD���Զ�ɨ�跽��
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341/5310/5510��IC�Ѿ�ʵ�ʲ���	   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if(lcddev.dir==1&&lcddev.id!=0X6804)//����ʱ����6804���ı�ɨ�跽��
	{			   
		switch(dir)//����ת��
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
	if(lcddev.id==0x9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510)//9341/6804/5310/5510,������
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510))regval|=0X08;//5310/5510����ҪBGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804��BIT6��9341�ķ���	   
		LCD_WriteReg(dirreg,regval);
 		if((regval&0X20)||lcddev.dir==1)
		{
			if(lcddev.width<lcddev.height)//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else  
		{
			if(lcddev.width>lcddev.height)//����X,Y
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
			case L2R_U2D://������,���ϵ���
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		}
		if(lcddev.id==0x8989)//8989 IC
		{
			dirreg=0X11;
			regval|=0X6040;	//65K   
	 	}else//��������IC		  
		{
			dirreg=0X03;
			regval|=1<<12;  
		}
		LCD_WriteReg(dirreg,regval);
	}
}   
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	//LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD->LCD_RAM=POINT_COLOR; 
}
//���ٻ���
//x,y:����
//color:��ɫ
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
		if(lcddev.dir==1)x=lcddev.width-1-x;//����ʱ����
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8); 
		LCD_WR_DATA(x&0XFF);	 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8); 
		LCD_WR_DATA(y&0XFF);
	}else
	{
 		if(lcddev.dir==1)x=lcddev.width-1-x;//������ʵ���ǵ�תx,y����
		LCD_WriteReg(lcddev.setxcmd,x);
		LCD_WriteReg(lcddev.setycmd,y);
	}			 
	LCD->LCD_REG=lcddev.wramcmd; 
	LCD->LCD_RAM=color; 
}	 


//����LCD��ʾ����
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//����
	{
		lcddev.dir=0;	//����
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
	}else 				//����
	{	  				
		lcddev.dir=1;	//����
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
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}	 
//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height.
//68042,����ʱ��֧�ִ�������!! 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{   
	u8 hsareg,heareg,vsareg,veareg;
	u16 hsaval,heaval,vsaval,veaval; 
	width=sx+width-1;
	height=sy+height-1;
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X6804)//6804������֧��
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
	}else	//��������IC
	{
		if(lcddev.dir==1)//����
		{
			//����ֵ
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
			hsareg=0X44;heareg=0X44;//ˮƽ���򴰿ڼĴ��� (1289����һ���Ĵ�������)
			hsaval|=(heaval<<8);	//�õ��Ĵ���ֵ.
			heaval=hsaval;
			vsareg=0X45;veareg=0X46;//��ֱ���򴰿ڼĴ���	  
		}else  //��������IC
		{
			hsareg=0X50;heareg=0X51;//ˮƽ���򴰿ڼĴ���
			vsareg=0X52;veareg=0X53;//��ֱ���򴰿ڼĴ���	  
		}								  
		//���üĴ���ֵ
		LCD_WriteReg(hsareg,hsaval);
		LCD_WriteReg(heareg,heaval);
		LCD_WriteReg(vsareg,vsaval);
		LCD_WriteReg(veareg,veaval);		
		LCD_SetCursor(sx,sy);	//���ù��λ��
	}
} 
//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ������ILI93XXҺ��,�������������ǻ���ILI9320��!!!
//�������ͺŵ�����оƬ��û�в���! 


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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//ʹ��PORTA,B,C,D,E,G�Լ�AFIO���ù���ʱ��

	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PA11     ��ѹ
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);		
	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD12     ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);	
	

	
	
	LCD_PWM_UnInit();
	CMachine.uLED_Back_No=0;
//	CMachine.fLED_Back=0;	
	
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PC7 ������� ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	
	
	
	
	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //PA4 ������� ��λ��
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
	
 	//PORTD�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOD, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15); 
	
	//PORTE�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOE, GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); 	  
  
	//	//PORTG12����������� A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_11;	 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOD, GPIO_Pin_7|GPIO_Pin_11); 	   




	
}

//LCD ����PWM�����ʼ��
//��ʼ���������Ϊ:PWM����Ƶ��Ϊ80Khz
//ͨ��LCD_BLPWM_VAL����LCD��������.
//0,�;250,�.
/*********************************************
    �������ƣ�TIM8_PWM_Init
    ��        �ܣ�ͨ��TIM8 CH1(PC6) CH2(PC7)PWM�����ʼ��
    ��        �Σ�arr���Զ���װֵ��
                        psc��ʱ��Ԥ��Ƶ��
    ��        �أ���
��˵����Frq = 72Mhz / ( psc + 1 ) / ( arr + 1 )
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

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//ʹ��FSMCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//ʹ��PORTA,B,C,D,E,G�Լ�AFIO���ù���ʱ��

	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PA11     ��ѹ
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);		
	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD12     ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_12);	
	
	
/* 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PC7 ������� ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
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
			
	
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //PA4 ������� ��λ��
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	
 	//PORTD�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
   	//	//PORTG12����������� A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_11;	 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
 
 
 

 
 
 
		readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // ���ݱ���ʱ��Ϊ16��HCLK,��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�죬�����1289���IC��
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    

		writeTiming.FSMC_AddressSetupTime = 0x00;	 //��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨A		
    writeTiming.FSMC_DataSetupTime = 0x03;		 ////���ݱ���ʱ��Ϊ4��HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
		FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // ʹ��BANK1 
			
	
	
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
LCD_WR_DATA(0xE8);//����ɨ�跽��

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
		
		
	//LCD_Display_Dir(1);		 	//Ĭ��Ϊ����
//	LCD_Scan_Dir(1);
	//LCD_LED=0;					//��������
*/	
	

/*	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
LCD_WR_REG(0x002C);  	
	for(ind0ex=0;ind0ex<153600;ind0ex++) 
	LCD_WR_DATA(YELLOW);   
	
	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
LCD_WR_REG(0x002C);  	
	for(ind0ex=0;ind0ex<153600;ind0ex++)
	LCD_WR_DATA(RED);   	
	
	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
LCD_WR_REG(0x002C);  	
	for(ind0ex=0;ind0ex<153600;ind0ex++)
	LCD_WR_DATA(GREEN);   		
	*/
	

	
 	 LCD_Clear(BLACK);
	
	
	
	//LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	//LCD_WR_REG(0x002C);  	
//	for(ind0ex=0;ind0ex<50000;ind0ex++) LCD_WR_DATA(RED);   
	//for(ind0ex=0;ind0ex<50000;ind0ex++) LCD_WR_DATA(GREEN);   
	//for(ind0ex=0;ind0ex<53600;ind0ex++) LCD_WR_DATA(BLUE);   
	//LCD_Clear(WHITE);
}  
//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 color)
{
	/*u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//�õ��ܵ���
	if((lcddev.id==0X6804)&&(lcddev.dir==1))//6804������ʱ�����⴦��  
	{						    
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_SetCursor(0x00,0x0000);		//���ù��λ��  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	   
	}*/
	
	
LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	for(ind0ex=0;ind0ex<153600;ind0ex++) 
	LCD_WR_DATA(color);   	
	
	
	
}  
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;


		xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      				//���ù��λ�� 
			for(j=0;j<xlen;j++)
			LCD_WR_DATA(color);	//���ù��λ�� 	    
		}
	
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*height+j];//д������ 
	}	  
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
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
//������
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawDottedLine(u16 x1, u16 y1, u16 x2, u16 y2)
{



u16 t; 
u16		iDotted=0;
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		
		
			
	if(iDotted<8)	POINT_COLOR=WHITE;	
	else	POINT_COLOR=BLACK;			
		
		iDotted++;
	if(iDotted>9)iDotted=0;
		
		LCD_DrawPoint(uRow,uCol);//���� 
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





//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR;      			     
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }    
	}else//���ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y); 
				temp<<=1;
				y++;
				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
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
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        //if(x>=width){x=x0;y+=size;}
      //  if(y>=height)break;//�˳�
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
unsigned int jj=0;			//�ֿ��ѯ��������

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
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���
	
	while(l<ulen)
	{
		c1 = dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
		c2 = dispbuff[l+1];
		for(jj=0;jj<2;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���
	
	while(l<ulen)
	{
		c1 = dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
		c2 = dispbuff[l+1];
		
		for(jj=0;jj<11;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
		c2 = 0xff&dispbuff[l+1];
		
		for(jj=0;jj<22;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���
	
	while(l<ulen)
	{
		c1 = dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
		c2 = dispbuff[l+1];
		
		for(jj=0;jj<11;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
		ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���															


		while(l<ulen)
		{
			c1 = dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
			for(jj=0;jj<11;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
		ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���															


		while(l<ulen)
		{
			c1 = dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
			for(jj=0;jj<11;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
		ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���															


		while(l<ulen)
		{
			c1 = dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
			for(jj=0;jj<12;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
��������Lcd��ѡ����
���ܣ�ѡ��Lcd��ָ���ľ�������

ע�⣺xStart��yStart��Xend��Yend������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���

��ڲ�����xStart x�������ʼ��
          ySrart y�������ʼ��
          Xend   y�������ֹ��
          Yend   y�������ֹ��
����ֵ����
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
��������Lcdͼ�����100*100
���ܣ���Lcdָ��λ�����ͼ��
��ڲ�����Index ҪѰַ�ļĴ�����ַ
          ConfigTemp д������ݻ�����ֵ
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
  unsigned long i;
	unsigned int j;

// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x0a); //�����������½ǿ�ʼ�������ң����µ���
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
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
		c2 = 0xff&dispbuff[l+1];
		
		for(jj=0;jj<22;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
		c2 = 0xff&dispbuff[l+1];
		
		for(jj=0;jj<22;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
		c2 = 0xff&dispbuff[l+1];
		
		 if( c1 > 0xA0 && c2 > 0xA0 )  //�������
		 {
			for(jj=0;jj<22;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
		 
			 
			for(jj=0;jj<22;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
		c2 = 0xff&dispbuff[l+1];
		
		for(jj=0;jj<22;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
	ulen = (unsigned int)sprintf(dispbuff,"%s",str);//�ֵĸ���
	
	while(l<ulen)
	{
		c1 = 0xff&dispbuff[l];//�Ѻ��ֲ�ֳ�������λ��
		c2 = 0xff&dispbuff[l+1];
		
		for(jj=0;jj<22;jj++)//Ѱ���ֿ⣬�����ֿⳤ�ȣ���Ѳ�ֿ�254������
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
char NUM_STR[]={"��һ�����������߰˾ź�"};


void Show_NUM100_72(unsigned int x,unsigned int y, unsigned char str,unsigned char front)
{

//NUM_STR_B[0]=NUM_STR[2*str];	
//NUM_STR_B[1]=NUM_STR[2*str+1];			
	
 //dprintf_NUM100_72(  x,  y,&NUM_STR_B[0],front,0);
	switch(str)
	{
		case 0:
			icon_show(x,  y, 65, 90, gImage_00_03,front);//��ʾ			
			break;
		case 1:
			icon_show(x,  y, 65, 90, gImage_01_03,front);//��ʾ			
			break;
		case 2:
			icon_show(x,  y, 65, 90, gImage_02_03,front);//��ʾ			
			break;
		case 3:
			icon_show(x,  y, 65, 90, gImage_03_03,front);//��ʾ			
			break;
		case 4:
			icon_show(x,  y, 65, 90, gImage_04_03,front);//��ʾ			
			break;
		case 5:
			icon_show(x,  y, 65, 90, gImage_05_03,front);//��ʾ			
			break;
		case 6:
			icon_show(x,  y, 65, 90, gImage_06_03,front);//��ʾ			
			break;
		case 7:
			icon_show(x,  y, 65, 90, gImage_07_03,front);//��ʾ			
			break;
		case 8:
			icon_show(x,  y, 65, 90, gImage_08_03,front);//��ʾ			
			break;
		case 9:
			icon_show(x,  y, 65, 90, gImage_09_03,front);//��ʾ			
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
			icon_show(x,  y, 31, 45, gImage_10_03,front);//��ʾ			
			break;
		case 1:
			icon_show(x,  y, 31, 45, gImage_11_03,front);//��ʾ			
			break;
		case 2:
			icon_show(x,  y, 31, 45, gImage_12_03,front);//��ʾ			
			break;
		case 3:
			icon_show(x,  y, 31, 45, gImage_13_03,front);//��ʾ			
			break;
		case 4:
			icon_show(x,  y, 31, 45, gImage_14_03,front);//��ʾ			
			break;
		case 5:
			icon_show(x,  y, 31, 45, gImage_15_03,front);//��ʾ			
			break;
		case 6:
			icon_show(x,  y, 31, 45, gImage_16_03,front);//��ʾ			
			break;
		case 7:
			icon_show(x,  y, 31, 45, gImage_17_03,front);//��ʾ			
			break;
		case 8:
			icon_show(x,  y, 31, 45, gImage_18_03,front);//��ʾ			
			break;
		case 9:
			icon_show(x,  y, 31, 45, gImage_19_03,front);//��ʾ			
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
			icon_show(x,  y, 15, 21, gImage_20_03,front);//��ʾ			
			break;
		case 1:
			icon_show(x,  y, 15, 21, gImage_21_03,front);//��ʾ			
			break;
		case 2:
			icon_show(x,  y, 15, 21, gImage_22_03,front);//��ʾ			
			break;
		case 3:
			icon_show(x,  y, 15, 21, gImage_23_03,front);//��ʾ			
			break;
		case 4:
			icon_show(x,  y, 15, 21, gImage_24_03,front);//��ʾ			
			break;
		case 5:
			icon_show(x,  y, 15, 21, gImage_25_03,front);//��ʾ			
			break;
		case 6:
			icon_show(x,  y, 15, 21, gImage_26_03,front);//��ʾ			
			break;
		case 7:
			icon_show(x,  y, 15, 21, gImage_27_03,front);//��ʾ			
			break;
		case 8:
			icon_show(x,  y, 15, 21, gImage_28_03,front);//��ʾ			
			break;
		case 9:
			icon_show(x,  y, 15, 21, gImage_29_03,front);//��ʾ			
			break;
		default:
			break;		
	}
	
	

}








