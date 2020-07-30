#ifndef __SSD1351_H__
#define __SSD1351_H__

#include "sys.h"


 struct ascii{
 unsigned char index;
 unsigned char msk[64];
};
//extern struct ascii ascii_3030[];
//extern struct ascii ascii_2020[];


//��ʼ��SSD1351
//void SSD1351_Init(void);


//void Initial_ssd1351(void);

//OLED��ʼ��
void OLED_Init(void);
//OLED��ʾ��
void OLED_On(void);
	
//OLED��ʾ�ر�
void OLED_Off(void);
	
//��������
//void OLED_Clear(u16 color)    ;
void OLCD_Clear(u16 colora);//����





//��һ������(����1)
//x0,y0:��� x1,y1:�յ�
//size:�ߴ�ϸ,��֧��:0~2.
//color:��ɫ
void gui_draw_bline1(u16 x0,u16 y0,u16 x1,u16 y1,u8 size,u16 color);


//�������Σ���ʼ����
//x0,y0:����
//r�뾶
//color:��ɫ
void gui_fill_keyStart(u16 x0,u16 y0,u16 r,u16 color,u16 back_color);
//�������Σ���ͣ����
//x0,y0:����
//r�뾶
//color:��ɫ
void gui_fill_keyPause(u16 x0,u16 y0,u16 r,u16 color,u16 back_color);
	
//�������Σ��������
//x0,y0:����
//r�뾶
//color:��ɫ
void gui_fill_keyLeft(u16 x0,u16 y0,u16 r,u16 color,u16 back_color);
	//�������Σ��ҽ�����
//x0,y0:����
//r�뾶
//color:��ɫ
void gui_fill_keyRight(u16 x0,u16 y0,u16 r,u16 color,u16 back_color)	;


//���ͼ����ɫ
//x:
//y:
//iconame:ͼ��
void OLED_DrawICO_Full(u8 x,u8 y,u16 color);
	
//��ͼ��
//x:
//y:
//iconame:ͼ��
void OLED_DrawICO(u8 x,u8 y,const unsigned char bmp[]);


//��ͼ��,��Сһ���ͼ��
//x:
//y:
//iconame:ͼ��
void OLED_DrawICO_Small(u8 x,u8 y,const unsigned char bmp[]);

//��ͼ��,��С30*30
//x:
//y:
//iconame:ͼ��
void OLED_DrawICO_3030(u8 x,u8 y,const unsigned char bmp[]);
	



//��ͼ��,ѡ�У�����ɫ
//x:
//y:
//iconame:ͼ��
///void OLED_DrawICO_Choose(u8 x,u8 y,const unsigned char bmp[]);
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~(127-15)
//y:0~(95-30)
//ch:�ַ�			 
//color_front:������ɫ
//color_back:������ɫ
void OLED_ShowChar(u8 x,u8 y,unsigned char ch,u16 color_front, u16 color_back);



//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~(127-15)
//y:0~(95-30)
//ch:�ַ�			 
//color_front:������ɫ
//color_back:������ɫ
void OLED_ShowChar2020(u8 x,u8 y,unsigned char ch,u16 color_front, u16 color_back);








//��Բ��ָ�����
//x,y:�������ĵ�
//size:���̴�С(ֱ��)
//d:���̷ָ�,���ӵĸ߶�
void calendar_circle_clock_drawpanel(u16 x,u16 y,u16 size,u16 d);
//��ʾʱ��
//x,y:�������ĵ�
//size:���̴�С(ֱ��)
//d:���̷ָ�,���ӵĸ߶�
//hour:ʱ��
//min:����
//sec:����
void calendar_circle_clock_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec);	






#endif
