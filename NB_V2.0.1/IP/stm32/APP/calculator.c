#include "calculator.h"
#include "math.h"
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-��ѧ������ ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

#define CALC_DISP_COLOR  	0X0000	//��ʾ���ֵ���ɫ
#define CALC_DISP_BKCOLOR  	0XA5F4	//��ʾ����ı���ɫ
#define CALC_MAX_EXP		200		//������ָ����Χ,�����趨Ϊ200,����ڴ湻�Ļ�,�������õ�300����.
									//����Ҫ���Ǹ���������ֵ��Χ
//��ֵ
#define CALC_PI 			3.1415926535897932384626433832795 	  
//����														   
const u8* calc_caption_tbl[GUI_LANGUAGE_NUM]={"��ѧ������","�ƌWӋ����","Calculator",};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������Ҫ�õ���ȫ������
//��Щ���ݱ������Զ���Ϊ�ֲ�����,��������δ֪ԭ��(���ɶ�ջ����),�ڱ���������,���붨��Ϊȫ�ֱ���.
//����������!!!
static double calc_x1=0,calc_x2=0;	//2��������
static double calc_temp=0;		   	//��ʱ����
u8 *outbuf;					//���������
u8 *tempbuf;				//��ʱ���ݴ洢��

#if ICOS_LOCATION==ICOS_SD_CARD//�����SD��  
//�ɿ���ʱ�����а�ť��·��
const u8* CALC_UP_BTN_TBL[29]=
{
	"0:/SYSTEM/APP/CALC/SIN_R.bmp",
	"0:/SYSTEM/APP/CALC/COS_R.bmp",
	"0:/SYSTEM/APP/CALC/TAN_R.bmp",
	"0:/SYSTEM/APP/CALC/LOG_R.bmp",
	"0:/SYSTEM/APP/CALC/LN_R.bmp",

	"0:/SYSTEM/APP/CALC/XX_R.bmp",
	"0:/SYSTEM/APP/CALC/XY_R.bmp",
	"0:/SYSTEM/APP/CALC/SQRT_R.bmp",
	"0:/SYSTEM/APP/CALC/X-1_R.bmp",
	"0:/SYSTEM/APP/CALC/FMT_R.bmp",

	"0:/SYSTEM/APP/CALC/NUM7_R.bmp",
	"0:/SYSTEM/APP/CALC/NUM8_R.bmp",
	"0:/SYSTEM/APP/CALC/NUM9_R.bmp",
	"0:/SYSTEM/APP/CALC/DEL_R.bmp",
	"0:/SYSTEM/APP/CALC/AC_R.bmp",

	"0:/SYSTEM/APP/CALC/NUM4_R.bmp",
	"0:/SYSTEM/APP/CALC/NUM5_R.bmp",
	"0:/SYSTEM/APP/CALC/NUM6_R.bmp",
	"0:/SYSTEM/APP/CALC/PLUS_R.bmp",
	"0:/SYSTEM/APP/CALC/MINU_R.bmp",

	"0:/SYSTEM/APP/CALC/NUM1_R.bmp",
	"0:/SYSTEM/APP/CALC/NUM2_R.bmp",
	"0:/SYSTEM/APP/CALC/NUM3_R.bmp",
	"0:/SYSTEM/APP/CALC/MULT_R.bmp",
	"0:/SYSTEM/APP/CALC/DIVI_R.bmp",

	"0:/SYSTEM/APP/CALC/NUM0_R.bmp",
	"0:/SYSTEM/APP/CALC/FLAG_R.bmp",
	"0:/SYSTEM/APP/CALC/DOT_R.bmp",
	"0:/SYSTEM/APP/CALC/ANS_R.bmp",		  
};  
//���µ�ʱ�����а�ť��·��
const u8* CALC_DOWN_BTN_TBL[29]=
{
	"0:/SYSTEM/APP/CALC/SIN_P.bmp",
	"0:/SYSTEM/APP/CALC/COS_P.bmp",
	"0:/SYSTEM/APP/CALC/TAN_P.bmp",
	"0:/SYSTEM/APP/CALC/LOG_P.bmp",
	"0:/SYSTEM/APP/CALC/LN_P.bmp",

	"0:/SYSTEM/APP/CALC/XX_P.bmp",
	"0:/SYSTEM/APP/CALC/XY_P.bmp",
	"0:/SYSTEM/APP/CALC/SQRT_P.bmp",
	"0:/SYSTEM/APP/CALC/X-1_P.bmp",
	"0:/SYSTEM/APP/CALC/FMT_P.bmp",

	"0:/SYSTEM/APP/CALC/NUM7_P.bmp",
	"0:/SYSTEM/APP/CALC/NUM8_P.bmp",
	"0:/SYSTEM/APP/CALC/NUM9_P.bmp",
	"0:/SYSTEM/APP/CALC/DEL_P.bmp",
	"0:/SYSTEM/APP/CALC/AC_P.bmp",

	"0:/SYSTEM/APP/CALC/NUM4_P.bmp",
	"0:/SYSTEM/APP/CALC/NUM5_P.bmp",
	"0:/SYSTEM/APP/CALC/NUM6_P.bmp",
	"0:/SYSTEM/APP/CALC/PLUS_P.bmp",
	"0:/SYSTEM/APP/CALC/MINU_P.bmp",

	"0:/SYSTEM/APP/CALC/NUM1_P.bmp",
	"0:/SYSTEM/APP/CALC/NUM2_P.bmp",
	"0:/SYSTEM/APP/CALC/NUM3_P.bmp",
	"0:/SYSTEM/APP/CALC/MULT_P.bmp",
	"0:/SYSTEM/APP/CALC/DIVI_P.bmp",

	"0:/SYSTEM/APP/CALC/NUM0_P.bmp",
	"0:/SYSTEM/APP/CALC/FLAG_P.bmp",
	"0:/SYSTEM/APP/CALC/DOT_P.bmp",
	"0:/SYSTEM/APP/CALC/ANS_P.bmp",		  
};
#else//�����FLASH DISK
//�ɿ���ʱ�����а�ť��·��
const u8* CALC_UP_BTN_TBL[29]=
{
	"1:/SYSTEM/APP/CALC/SIN_R.bmp",
	"1:/SYSTEM/APP/CALC/COS_R.bmp",
	"1:/SYSTEM/APP/CALC/TAN_R.bmp",
	"1:/SYSTEM/APP/CALC/LOG_R.bmp",
	"1:/SYSTEM/APP/CALC/LN_R.bmp",

	"1:/SYSTEM/APP/CALC/XX_R.bmp",
	"1:/SYSTEM/APP/CALC/XY_R.bmp",
	"1:/SYSTEM/APP/CALC/SQRT_R.bmp",
	"1:/SYSTEM/APP/CALC/X-1_R.bmp",
	"1:/SYSTEM/APP/CALC/FMT_R.bmp",

	"1:/SYSTEM/APP/CALC/NUM7_R.bmp",
	"1:/SYSTEM/APP/CALC/NUM8_R.bmp",
	"1:/SYSTEM/APP/CALC/NUM9_R.bmp",
	"1:/SYSTEM/APP/CALC/DEL_R.bmp",
	"1:/SYSTEM/APP/CALC/AC_R.bmp",

	"1:/SYSTEM/APP/CALC/NUM4_R.bmp",
	"1:/SYSTEM/APP/CALC/NUM5_R.bmp",
	"1:/SYSTEM/APP/CALC/NUM6_R.bmp",
	"1:/SYSTEM/APP/CALC/PLUS_R.bmp",
	"1:/SYSTEM/APP/CALC/MINU_R.bmp",

	"1:/SYSTEM/APP/CALC/NUM1_R.bmp",
	"1:/SYSTEM/APP/CALC/NUM2_R.bmp",
	"1:/SYSTEM/APP/CALC/NUM3_R.bmp",
	"1:/SYSTEM/APP/CALC/MULT_R.bmp",
	"1:/SYSTEM/APP/CALC/DIVI_R.bmp",

	"1:/SYSTEM/APP/CALC/NUM0_R.bmp",
	"1:/SYSTEM/APP/CALC/FLAG_R.bmp",
	"1:/SYSTEM/APP/CALC/DOT_R.bmp",
	"1:/SYSTEM/APP/CALC/ANS_R.bmp",		  
};  
//���µ�ʱ�����а�ť��·��
const u8* CALC_DOWN_BTN_TBL[29]=
{
	"1:/SYSTEM/APP/CALC/SIN_P.bmp",
	"1:/SYSTEM/APP/CALC/COS_P.bmp",
	"1:/SYSTEM/APP/CALC/TAN_P.bmp",
	"1:/SYSTEM/APP/CALC/LOG_P.bmp",
	"1:/SYSTEM/APP/CALC/LN_P.bmp",

	"1:/SYSTEM/APP/CALC/XX_P.bmp",
	"1:/SYSTEM/APP/CALC/XY_P.bmp",
	"1:/SYSTEM/APP/CALC/SQRT_P.bmp",
	"1:/SYSTEM/APP/CALC/X-1_P.bmp",
	"1:/SYSTEM/APP/CALC/FMT_P.bmp",

	"1:/SYSTEM/APP/CALC/NUM7_P.bmp",
	"1:/SYSTEM/APP/CALC/NUM8_P.bmp",
	"1:/SYSTEM/APP/CALC/NUM9_P.bmp",
	"1:/SYSTEM/APP/CALC/DEL_P.bmp",
	"1:/SYSTEM/APP/CALC/AC_P.bmp",

	"1:/SYSTEM/APP/CALC/NUM4_P.bmp",
	"1:/SYSTEM/APP/CALC/NUM5_P.bmp",
	"1:/SYSTEM/APP/CALC/NUM6_P.bmp",
	"1:/SYSTEM/APP/CALC/PLUS_P.bmp",
	"1:/SYSTEM/APP/CALC/MINU_P.bmp",

	"1:/SYSTEM/APP/CALC/NUM1_P.bmp",
	"1:/SYSTEM/APP/CALC/NUM2_P.bmp",
	"1:/SYSTEM/APP/CALC/NUM3_P.bmp",
	"1:/SYSTEM/APP/CALC/MULT_P.bmp",
	"1:/SYSTEM/APP/CALC/DIVI_P.bmp",

	"1:/SYSTEM/APP/CALC/NUM0_P.bmp",
	"1:/SYSTEM/APP/CALC/FLAG_P.bmp",
	"1:/SYSTEM/APP/CALC/DOT_P.bmp",
	"1:/SYSTEM/APP/CALC/ANS_P.bmp",		  
};
#endif


//���ؼ�����������UI	  
void calc_load_ui(void)
{	
 	app_filebrower((u8*)calc_caption_tbl[gui_phy.language],0X05);//��ʾ����  
  	gui_fill_rectangle(0,20,240,300,BLACK);				//����ɫ 		  
  	gui_fill_rectangle(2,30,236,50,CALC_DISP_BKCOLOR);	//��ʾ�����ɫ���	  
} 

//��ʾ����������������
//fontbase:�ֿ��ַ		 
//buf:�ַ�����ַ
void calc_show_inbuf(u8 *fontbase,u8 *buf)
{
 	u8 len=strlen((const char*)buf);//�õ��ַ�������
   	gui_fill_rectangle(20,30+5+12,182,28,CALC_DISP_BKCOLOR);//���֮ǰ����ʾ	  
	app_showbigstring(fontbase,20+182-len*14,30+5+12,(const u8*)buf,28,BLACK,CALC_DISP_BKCOLOR);//��ʾ�ַ���
}	 				
//��������������
//fontbase:�ֿ��ַ
//x1,x2:����������
//buf:�������ݴ洢��
//ctype:���㷨��
//calc_sta:������״̬
//����ֵ:0,û�и�����ʾ;1,�Ѿ�������ʾ;
u8 calc_exe(u8 *fontbase,double *x1,double *x2,u8 *buf,u8 ctype,u8 *calc_sta)
{
	u8 inlen=0;
	u8 res=0;
	inlen=(*calc_sta)&0X0F;//�õ����볤��
	if((*calc_sta&0X80)==0)	//��û�в�����
	{	  
		*x1=atof((const char *)buf);//ת��Ϊ����
		if((*calc_sta&0X40))
		{
			*x1=-*x1;
			*calc_sta&=~(0X40);//����λ��Ϊ��   
 		}
		*calc_sta|=0X80; 		//����Ѿ���һ����������.
	}else if(inlen)				//����������
	{	 	
		*x2=atof((const char *)buf);	//ת��Ϊ����
		if((*calc_sta&0X40))*x2=-*x2; 	//������Ǹ���
 		switch(ctype)
		{
			case 1://�ӷ�
				*x1=*x1+*x2;
				break;
			case 2://����
				*x1=*x1-*x2;
				break;
			case 3://�˷�
				*x1=*x1*(*x2);
				break;
			case 4://����
				*x1=*x1/(*x2);
				break;
			case 5://x^y�η�
				*x1=pow(*x1,*x2);
				break;
			case 0://û���κ������
				*x1=*x2;
 				break;
		}	 
		res=1;//��Ҫ������ʾ
	}
	if(ctype>5)//������������
	{
		switch(ctype)
		{
			case 6://sin����
				*x1=sin((*x1*CALC_PI)/180);//ת��Ϊ�Ƕȼ�����
				break;
			case 7://cos����
				*x1=cos((*x1*CALC_PI)/180);//ת��Ϊ�Ƕȼ�����
				break;
			case 8://tan����
				*x1=tan((*x1*CALC_PI)/180);//ת��Ϊ�Ƕȼ�����
				break;
			case 9://log����
				*x1=log10(*x1);
				break;
			case 10://ln����
				*x1=log(*x1);
				break;
			case 11://x^2����
				*x1=*x1*(*x1);
				break;
			case 12://��������
				*x1=sqrt(*x1);
				break;
			case 13://��������
				*x1=1/(*x1);
				break;	   	    
		}					 
		res=1;//��Ҫ������ʾ
	}
	if(res)//������ʾ
	{
		*calc_sta&=~(0X40);				//����λ��Ϊ��
 		if(calc_show_res(fontbase,*x1))	//��ʾ�д���,�������
		{
			*calc_sta=0;
			*x1=0;
			*x2=0;
			gui_memset(buf,0,14); 	//���뻺������
 		}	    
	}
	*calc_sta&=0XF0;  		//������볤��
	gui_memset(buf,0,14); 	//���뻺������
 	return res;
}
//��ʾָ�� 
//exp:ָ��ֵ
void calc_show_exp(short exp)			   
{	   
	gui_fill_rectangle(20+182+2,30+5+12,32,28,CALC_DISP_BKCOLOR);//���֮ǰ����ʾ	  
 	if(exp!=0) 
	{ 
 		if(exp<0)
		{
			exp=-exp;
 			gui_show_ptchar(20+182+2,30+5+12,GUI_LCD_W,GUI_LCD_H,0,CALC_DISP_COLOR,16,'-',1);	//��ʾ����
		}
		gui_show_num(20+182+8+2,30+5+12,3,CALC_DISP_COLOR,16,exp,0X81);							//�����ʾָ��
		gui_show_ptchar(20+182+2,30+5+12+16,GUI_LCD_W,GUI_LCD_H,0,CALC_DISP_COLOR,12,'X',1);	//���ӷ�ʽ��ʾ�˺�
		gui_show_num(20+182+6+2,30+5+12+16,2,CALC_DISP_COLOR,12,10,0X81);						//�����ʾ����
 	}
}
//��ʾ������
//fontbase:�ֿ��ַ
//fg:0,ȥ������;1,��ʾ����.
void calc_show_flag(u8 *fontbase,u8 fg)
{
	if(fg==0)app_showbigchar(fontbase,4,30+5+12,' ',28,BLACK,CALC_DISP_BKCOLOR);	//ȥ������
	else app_showbigchar(fontbase,4,30+5+12,'-',28,BLACK,CALC_DISP_BKCOLOR);		//��ʾ����
}  
//����������ʾ
//fontbase:�ֿ��ַ
//calc_sta:������״̬
//inbuf:��������
//len:���볤��
void calc_input_fresh(u8 *fontbase,u8* calc_sta,u8 *inbuf,u8 len)
{	     
	*calc_sta&=0XF0;
	*calc_sta|=len&0X0F;						//���²���������
	calc_show_flag(fontbase,(*calc_sta)&0X40);	//���Ŵ���
	calc_show_inbuf(fontbase,inbuf);  			//��ʾ��������
 	calc_show_exp(0);							//ȥ��ָ������	 
} 
//��ʾ���㷽��
//ctype:���㷽��
void calc_ctype_show(u8 ctype)
{
	u8 *chx;
	switch(ctype)
	{			    
		case 1:	//�ӷ�
			chx="��";
			break;
		case 2:	//����
			chx="��";
			break;
		case 3:	//�˷�
			chx="��";
			break;
		case 4:	//����
			chx="��";
			break;
		case 5://x^y�η�
			chx="x^y";
			break;		  
		case 6://sin
			chx="sin";
			break;
		case 7://cos
			chx="cos";
			break;
		case 8://tan
			chx="tan";
			break;
		case 9://log
			chx="log";
			break;
		case 10://ln
			chx="ln";
			break;
		case 11://x^2
			chx="x^2";
			break;
		case 12://
			chx="�̣�";
			break;
		case 13://1/x
			chx="1/x";
			break;	    
		default:
			chx="";
			break;
	}
	gui_fill_rectangle(20,30+2,6*4,12,CALC_DISP_BKCOLOR);//���֮ǰ����ʾ	  
	gui_show_ptstr(20,30+2,GUI_LCD_W,GUI_LCD_H,0,CALC_DISP_COLOR,12,chx,1);//��ʾ���㷽��				 
}


//��ʾ���
//fontbase:�ֿ��ַ
//res:���
//����ֵ:0,�޴���;1,�д���

u8 calc_show_res(u8 *fontbase,double res)
{									  
//	u8 outbuf[20];
//	u8 tempbuf[14];//��ʱ���ݴ洢��
	signed short exp=0;
	u8 temp=13;
	u8 i;
	if(res<0)
	{
		res=-res;									    
		calc_show_flag(fontbase,1);		//��ʾ����
	}else calc_show_flag(fontbase,0);	//ȡ������		
	gui_memset(outbuf,0,20);
	gui_memset(tempbuf,0,14);
  	sprintf((char*)outbuf,"%0.11e",res);//��������outbuf����
   	outbuf[temp]=0;
  	exp=(signed short)atof((const char*)(outbuf+temp+1));		//�õ����ݵ�ָ������.
	if((outbuf[0]=='i')||(outbuf[0]=='n')||exp>CALC_MAX_EXP)	//����Ľ��
	{
		calc_show_flag(fontbase,0);	//ȡ������
 		outbuf[0]='E';
		outbuf[1]=0;
		calc_show_inbuf(fontbase,outbuf);//��������LCD
 		calc_show_exp(0);	 
		return 1;
 	}	  
 	if(exp>-5&&exp<12)
	{
		if(exp<0)
		{
			for(i=0;i<(-exp+1);i++)
			{	   
				if(i==1)tempbuf[i]='.';
				else tempbuf[i]='0';	
			}
			exp+=12;
			for(i=0;i<exp;i++)
			{	   
				if(i==0)tempbuf[12+1-exp+i]=outbuf[i];	//����	
				else tempbuf[12+1-exp+i]=outbuf[i+1];		//����	   	
			}   
			tempbuf[13]=0;//ĩβ��ӽ�����
			outbuf[0]=0;
			strcpy((char*)outbuf,(char*)tempbuf);//����tempbuf�����ݵ�outbuf.
		}else//exp>=0
		{
			for(i=0;i<exp;i++)
			{	   
				temp=outbuf[2+i];
			 	outbuf[1+i]=temp;
				outbuf[2+i]='.';	
			}   									  
		}
		exp=0;//����Ҫ��ʾexp	 					    
 	}
	for(i=12;i>0;i--)//�Ѷ����0ȥ��
	{
		if(outbuf[i]=='0')outbuf[i]=0;
		else if(outbuf[i]=='.')
		{
			outbuf[i]=0;
			break;
		}else break;
	}	
	calc_show_inbuf(fontbase,outbuf);//��������LCD
	calc_show_exp(exp);	 
 	return 0;   
}
//��ʽ����ʾ
//fontbase:�ֿ��ַ
//res:Ҫת��������,���֧��0XFFFF
//fmt:0:ʮ����
//    1:ʮ������
//    2:������
void calc_fmt_show(u8 *fontbase,u16 res,u8 fmt)
{
	u8 *fmtstr="";
	u8 outbuf[17];
	u8 i=0;
	u8 len;
	gui_memset(outbuf,0,17);
  	gui_fill_rectangle(2,30,236,50,CALC_DISP_BKCOLOR);	//������ʾ�����ɫ���	  
	switch(fmt)
	{
		case 0://ʮ����
			fmtstr="DEG"; 
			sprintf((char*)outbuf,"%d",res);	//��������outbuf����	  
			break;
		case 1://ʮ������ 
			fmtstr="HEX";
			sprintf((char*)outbuf,"%X",res);	//��������outbuf����	  
			calc_show_inbuf(fontbase,outbuf);	//��������LCD
 			break;
  		case 2://������
			fmtstr="BIN"; 
 		  	for(i=0;i<16;i++)
			{
				outbuf[15-i]='0'+(res&0x01);
				res>>=1;
			}
			for(i=0;i<16;i++)
			{
				if(outbuf[i]=='0')outbuf[i]=0;
				else break;
			}   
			len=strlen((const char*)(outbuf+i));//�õ��ַ�������
			if(i>1)app_showbigstring(fontbase,20+182-len*14,30+5+12,(const u8*)(outbuf+i),28,BLACK,CALC_DISP_BKCOLOR);//��ʾ�ַ���
			else app_showbigstring(fontbase,20+182+14*(2-i)-len*14,30+5+12,(const u8*)(outbuf+i),28,BLACK,CALC_DISP_BKCOLOR);//��ʾ�ַ��� 
  			break;	 
	}    
	if(fmt<2)calc_show_inbuf(fontbase,outbuf);	//ʮ���ƺ�16����,��������LCD
	gui_show_ptstr(20+40,30+2,GUI_LCD_W,GUI_LCD_H,0,CALC_DISP_COLOR,12,fmtstr,1);//��ʾ��ǰ��ʽ				 
}

	  
//������������
u8 calc_play(void)
{
	_btn_obj* c_btn[29];
	u8 *inbuf;		//�������12���ַ�+������,�ܹ�13��
//	double x1=0,x2=0;	//2��������
//	double temp=0;

	u8 calc_sta=0;		//������״̬
						//[7]:0,��û�в�����;1,�Ѿ���һ����������
						//[6]:����λ;0,����;1,����;
						//[5]:0,������2δ����;1,������2�ѱ���;
						//[3:0]:�������������

	u8 ctype=0;			//���㷨��
						//0,û�м��㷨��
						//1,�ӷ�.
						//2,����
						//3,�˷�
						//4,����
						//5,x^y�η�.
						//6,sin����
						//7,cos����
						//8,tan����
						//9,log����
						//10,ln����
						//11,x^2����
						//12,����
						//13,1/x����
						//14,��ʽת��
 	u8 maxlen=12;		//��������ַ���.Ĭ��12��,��������С�����Ժ�,��Ϊ13��.
	   
 	u8 i,j;
	u8 rval=0;
	u8 res;
	u8 ttype=0;			//��һ�β����������
	u8 cfmt=0;			//��ʽ,Ĭ��Ϊʮ���Ƹ�ʽ

	FIL* f_calc=0;	 
	u8 *bfbase=0;		//���ֿ�Ļ�ַ
	outbuf=(u8*)gui_memin_malloc(20);	//���������
	if(outbuf==NULL)rval=1;				//����ʧ��
	tempbuf=(u8*)gui_memin_malloc(14);	//��ʱ�洢��
 	if(tempbuf==NULL)rval=1; 			//����ʧ��
	inbuf=(u8*)gui_memin_malloc(14);	//���뻺����
  	if(inbuf==NULL)rval=1;				//����ʧ��

   	f_calc=(FIL *)gui_memin_malloc(sizeof(FIL));//����FIL�ֽڵ��ڴ����� 
	if(f_calc==NULL)rval=1;		//����ʧ��
	else
	{
		res=f_open(f_calc,(const TCHAR*)APP_ASCII_28,FA_READ);//���ļ� 
		if(res==FR_OK)
		{
			bfbase=(u8*)gui_memex_malloc(f_calc->fsize);	//Ϊ�����忪�ٻ����ַ
			if(bfbase==0)rval=1;
			else res=f_read(f_calc,bfbase,f_calc->fsize,(UINT*)&br);	//һ�ζ�ȡ�����ļ�
 		} 
		if(res)rval=res;
	} 	      
	for(i=0;i<6;i++)//����29����ť
	{
		for(j=0;j<5;j++)
		{
			res=i*5+j;
			if(i<2)c_btn[res]=btn_creat(j*48+2,i*30+100,44,18,0,1);	   	//����ͼƬ��ť
			else if(i==5&&j==3)
			{
				c_btn[res]=btn_creat(j*48+2,(i-2)*40+160,92,30,0,1); 		//����ͼƬ��ť
				j=4;
			}else c_btn[res]=btn_creat(j*48+2,(i-2)*40+160,44,30,0,1);		//����ͼƬ��ť
	 	
			if(c_btn[res]==NULL){rval=1;break;}//����ʧ��.
			c_btn[res]->picbtnpathu=(u8*)CALC_UP_BTN_TBL[res];
			c_btn[res]->picbtnpathd=(u8*)CALC_DOWN_BTN_TBL[res];
	 		c_btn[res]->sta=0;
		}	 
 	}	 
	if(rval==0)//׼�����������
	{	   
		calc_load_ui();						//����������
		calc_fmt_show(bfbase,calc_x1,cfmt);	//��ʾ��ʽ��
		for(i=0;i<29;i++)btn_draw(c_btn[i]);
		gui_memset(inbuf,0,14);				//���뻺������
		calc_show_inbuf(bfbase,"0");			//��ʾ0
		while(1)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(5);							//��ʱһ��ʱ�ӽ���
			if(system_task_return)break;	  		//TPAD����
 
	 		for(i=0;i<29;i++)
			{
				res=btn_check(c_btn[i],&in_obj);   
				if(res&&((c_btn[i]->sta&(1<<7))==0)&&(c_btn[i]->sta&(1<<6)))//�а����������ɿ�,����TP�ɿ���
				{	
					res=calc_sta&0X0F;		//�õ����볤��
					if((i!=9)&&(ctype==14))	//���Ǹ�ʽ����ť,����һ�β���ʱ��ʽת��
					{
						cfmt=0;
						calc_x1=0;
						calc_x2=0;
						calc_sta=0;
						calc_temp=0;
						ttype=1;	//ģ��һ������						  
						ctype=0;	   
						calc_fmt_show(bfbase,calc_x1,cfmt);
					}
 					switch(i)
					{
						case 0://sin����
						case 1://cos����
						case 2://tan����
						case 3://log����
						case 4://ln����
						case 5://x^2����   
						case 7://sqrt����   
						case 8://��������   
							if(i<6)ctype=6+i; 
							else ctype=5+i;   
						 	calc_exe(bfbase,&calc_x1,&calc_x2,inbuf,ctype,&calc_sta);//���㴦��  
							calc_sta&=0XF0;
							gui_memset(inbuf,0,14);	 	//���뻺������
							maxlen=12;
							calc_ctype_show(ctype);		//��ʾ��εĲ�����
							ctype=0;
							ttype=0;
							break;		 
						case 6://x^y����
						 	calc_exe(bfbase,&calc_x1,&calc_x2,inbuf,ctype,&calc_sta);//���㴦��  
							calc_sta&=0XF0;
							gui_memset(inbuf,0,14);				//���뻺������
							maxlen=12;
							ctype=5;   
							break; 	  
						case 9://��ʽת��
							cfmt++;
							if(cfmt>2)cfmt=0;
						 	if((calc_sta&0X80)==0)
							{
								calc_x1=atof((const char *)inbuf);	//ת��Ϊ����	  
								calc_sta|=0X80; 				//����Ѿ���һ��������.
							} 
							calc_sta=0XF0;
							gui_memset(inbuf,0,14);				//���뻺������
							maxlen=12;	
							ctype=14;//���Ϊ��ʽת��					   	
							calc_fmt_show(bfbase,calc_x1,cfmt);
							break; 
						case 10://����7
						case 11://����8
						case 12://����9
							if(res==1&&inbuf[0]=='0')res=0;					//��һ�����ݲ���Ϊ0 
							if(res<maxlen)inbuf[res++]='7'+i-10;			//����7/8/9
							calc_input_fresh(bfbase,&calc_sta,inbuf,res);	//������ʾ
							break;	    
						case 13://�˸�
							if(res>1)
							{	       
								res--;
								if(inbuf[res]=='.')maxlen=12;//�ָ�12�����ݳ���
								inbuf[res]=0;
  							}else if(inbuf[0]!='0')inbuf[0]='0';//���һ����������Ϊ0
 							calc_input_fresh(bfbase,&calc_sta,inbuf,res);	//������ʾ  
							break;
						case 14://����
							maxlen=12;
							gui_memset(inbuf,0,14);//���뻺������
							calc_sta=0;
							calc_show_inbuf(bfbase,"0");	//��ʾ0
							calc_show_flag(bfbase,calc_sta&0X40);	//����	  
						  	calc_show_exp(0);				//ȥ��ָ������	
							calc_x1=0;
							calc_x2=0;
							calc_temp=0;
							ttype=1;	//ģ��һ������						  
							ctype=0;
							break;
						case 15://����4
						case 16://����5
						case 17://����6
							if(res==1&&inbuf[0]=='0')res=0;					//��һ�����ݲ���Ϊ0 
							if(res<maxlen)inbuf[res++]='4'+i-15;			//����4/5/6
							calc_input_fresh(bfbase,&calc_sta,inbuf,res);	//������ʾ
							break;
						case 18://�ӷ�				 
						 	calc_exe(bfbase,&calc_x1,&calc_x2,inbuf,ctype,&calc_sta);//���㴦��  
							calc_sta&=0XF0;
							gui_memset(inbuf,0,14);				//���뻺������
							maxlen=12;
							ctype=1;   
							break;
						case 19://����
						 	calc_exe(bfbase,&calc_x1,&calc_x2,inbuf,ctype,&calc_sta);//���㴦��  
							calc_sta&=0XF0;
							gui_memset(inbuf,0,14);				//���뻺������
							maxlen=12;
							ctype=2;   
							break; 	 
						case 20://����1
						case 21://����2
						case 22://����3
							if(res==1&&inbuf[0]=='0')res=0;					//��һ�����ݲ���Ϊ0 
							if(res<maxlen)inbuf[res++]='1'+i-20;			//����1/2/3
							calc_input_fresh(bfbase,&calc_sta,inbuf,res);	//������ʾ
							break;	  
						case 23://�˷�	   
						 	calc_exe(bfbase,&calc_x1,&calc_x2,inbuf,ctype,&calc_sta);//���㴦��  
 							calc_sta&=0XF0;
							gui_memset(inbuf,0,14);				//���뻺������
							maxlen=12;
							ctype=3;   
							break;
						case 24://����	    
						 	calc_exe(bfbase,&calc_x1,&calc_x2,inbuf,ctype,&calc_sta);//���㴦��  
							calc_sta&=0XF0;
							gui_memset(inbuf,0,14);				//���뻺������
							maxlen=12;
							ctype=4;   
							break; 	 
						case 25://����0
							if(res!=1||inbuf[0]!='0')
							{
								if(res<maxlen)inbuf[res++]='0';//����0
							} 
							calc_input_fresh(bfbase,&calc_sta,inbuf,res);	//������ʾ  
							break;
 						case 26://���ŷ�ת						
							if(calc_sta&(1<<6))calc_sta&=~(1<<6);	 
							else calc_sta|=1<<6;	   
							calc_show_flag(bfbase,calc_sta&0X40);	//���Ŵ���
							break;
						case 27://����С����
							if(maxlen==12)
							{
								if(res<maxlen)
								{
									if(res==0)inbuf[res++]='0';//������.	  
									inbuf[res++]='.';//����.
									maxlen=13;
								}
							}
							calc_input_fresh(bfbase,&calc_sta,inbuf,res);	//������ʾ  
							break;
						case 28://������	   
							if(calc_sta&0X80)//�Ѿ���һ����������
							{
								if(res==0)		//û������������
								{
									if((calc_sta&0X10)==0)//������2��δ����
									{
										calc_sta|=0X10;	//����Ѿ�������
										calc_temp=calc_x1;		//������2�����ڲ�����1
									} 
								}else calc_sta&=~0X10;	//ȡ�����������2��־ 
							} 
							if(calc_exe(bfbase,&calc_x1,&calc_x2,inbuf,ctype,&calc_sta)==0)		//����ִ�м���?
							{															//��
								if(calc_sta&0X10)//�в�����2
								{
									switch(ctype)
									{
										case 1://�ӷ�
											calc_x1=calc_x1+calc_temp;
											break;
										case 2://����
											calc_x1=calc_x1-calc_temp;
											break;
										case 3://�˷�
											calc_x1=calc_x1*calc_temp;
											break;
										case 4://����
											calc_x1=calc_x1/calc_temp;
											break;
									}							
								}
						 		if(calc_show_res(bfbase,calc_x1))	//��ʾ�д���,�������
								{
									calc_sta=0;
									calc_x1=0;
									calc_x2=0;
									gui_memset(inbuf,0,14); 	//���뻺������
						 		}									   
							}
							if((calc_sta&0X10)==0)//������2��δ����
							{
								calc_sta|=0X10;	//����Ѿ�������
								calc_temp=calc_x2;		//���������2
							} 	 				   
							break;	  
					}
					//printf("key:%d pressed!\r\n",i);
				}
				if(ctype!=ttype)//���������
				{
					ttype=ctype;
					calc_ctype_show(ctype);
				}
			}
		}
	}			 
   	gui_memin_free(inbuf);	//�ͷ��ڴ�
   	gui_memin_free(outbuf);	//�ͷ��ڴ�
   	gui_memin_free(tempbuf);//�ͷ��ڴ�
 	gui_memin_free(f_calc);	//�ͷ��ڴ�
	gui_memex_free(bfbase);	//�ͷ��ڴ�
	for(i=0;i<29;i++)
	{
		btn_delete(c_btn[i]);//ɾ����Щ����
	}
	return 0;
}


























