#include "sramlcd.h"
#include "dma.h"
#include "lcd.h"
#include "delay.h"
#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SRAM-LCD ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   


//2֡�Ļ���,������malloc�ڴ漰�����֮��	 
u16 LCD_GRAM_BUF[320][480] __attribute__ ((at(0X68000000+MEM2_MAX_SIZE+MEM2_ALLOC_TABLE_SIZE*2)));//LCD���濪��MALLOC֮��.
m_sramlcd_dev SLCD=
{
	slcd_frame_show,
	slcd_draw_point,
	slcd_frame_clear,
	slcd_frame_move,
	0,
	0,
};	
//��ָ��λ�û���.
//x,y:�ڵ�ǰ֡(SLCD.frame)������.
//color:��ɫ.
void slcd_draw_point(u16 x,u16 y,u16 color)
{							   
	LCD_GRAM_BUF[x][y]=color;
}

//��ȡָ��λ�õ����ɫֵ
//x,y:�ڵ�ǰ֡(SLCD.frame)������
//����ֵ:��ɫ
u16 slcd_read_point(u16 x,u16 y)
{
	return LCD_GRAM_BUF[x+240*SLCD.frame][y];
}	  
//��ʾ֡
//x:֡��x����.
//color:��ɫ.
void slcd_frame_show(u16 x)
{							   
	LCD_Scan_Dir(L2R_U2D);		//����ɨ�跽��
	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	LCD_WriteRAM_Prepare();     //��ʼд��GRAM	 	  
	LCD_DMA_Enable(x);			//����һ��DMA����
	LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ�Ϸ���
}
//���֡.
//��յ�ǰ֡(SLCD.frame)
//color:��ɫ.
void slcd_frame_clear(u16 color)
{		
	u16 i,j;
	for(i=0;i<240;i++)
	{
		for(j=0;j<320;j++)
		LCD_GRAM_BUF[i+240*SLCD.frame][j]=color;
	}											    
}
//�ƶ���Ļ
//dir:����,0:����;1,����
//skips:ÿ����Ծ����
//pos:��ʼλ��
void slcd_frame_move(u8 dir,u8 skips,u16 pos)
{
	u16 x ;				 
	if(dir)//��Ļ����
	{
		for(x=pos;x>0;)
		{
			if(x>skips)x-=skips;
			else x=0;
			slcd_frame_show(x);
 	 	}
		SLCD.frame=0;		
	}else //��Ļ����
	{
		for(x=pos;x<240;)
		{
			x+=skips;
			if(x>240)x=240;
			slcd_frame_show(x);
 	 	}
		SLCD.frame=1;		
	} 
}










