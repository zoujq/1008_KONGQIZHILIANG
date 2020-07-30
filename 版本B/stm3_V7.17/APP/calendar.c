#include "calendar.h" 	      						  
#include "stdio.h"
#include "settings.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

_alarm_obj alarm;	//���ӽṹ��
					 
#define TIME_TOPY	120
#define OTHER_TOPY	200 	


const u8 *calendar_week_table[GUI_LANGUAGE_NUM][7]=
{
{"������","����һ","���ڶ�","������","������","������","������"},
{"������","����һ","���ڶ�","������","������","������","������"},
{"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"},
};
//���ӱ���
const u8 *calendar_alarm_caption_table[GUI_LANGUAGE_NUM]=
{
"����","�[�","ALARM",
};
//���찴ť
const u8 *calendar_alarm_realarm_table[GUI_LANGUAGE_NUM]=
{
"����","���","REALARM",
};

const u8* calendar_loading_str[GUI_LANGUAGE_NUM][3]=
{
{
	"���ڼ���,���Ժ�...",
	"δ��⵽DS18B20!",
	"�����ڲ��¶ȴ�����...",
},
{
	"���ڼ���,���Ժ�...",
	"δ��⵽DS18B20!",
	"�����ڲ��¶ȴ�����...",
},
{
	"Loading...",
	"DS18B20 Check Failed!",
	"Start Inside Sensor...",
},
};	

//���³�ʼ������		    
//alarmx:���ӽṹ��
void calendar_alarm_init(_alarm_obj *alarmx) 
{	  
  	u32 curtime=0;
	u32 temptime=0;
	u32 destime=0XFFFFFFFF;//Ŀ������ʱ��(s)�趨�����	
 	if(alarmx->weekmask)//����Ҫ�����Ӵ���,���յ���������һ�� 
	{										       
		curtime=RTC->CNTH;//�õ��������е�ֵ(������)
		curtime<<=16;
		curtime+=RTC->CNTL;	   
		//ȡһ���뵱ǰʱ����ӽ���ֵ��Ϊ���ӼĴ���������
		temptime=curtime/86400;	//�õ���ǰ��������(�˴�û���õ�����,����˵����)
		temptime=temptime*86400;
 		temptime+=(u32)alarmx->hour*3600+(u32)alarmx->min*60;//�õ�������
 		if(temptime<=curtime)temptime+=86400;//ִ��ʱ���ѹ����Ƴٵ�����
 		destime=temptime;//�������ӼĴ���
 	}
	RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ��
    RCC->APB1ENR|=1<<27;//ʹ�ܱ���ʱ��
	PWR->CR|=1<<8;    //ȡ��������д����
	//���������Ǳ����!
	RTC->CRL|=1<<4;   //�������� 
	RTC->ALRL=destime&0xffff;
	RTC->ALRH=destime>>16;
	RTC->CRL&=~(1<<4);//���ø���
	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������  
}
//����������
//type:��������	   
//0,��.
//1,��.��.
//2,��.��.��
//4,��.��.��.��
void calendar_alarm_ring(u8 type)
{
	u8 i;	 
	for(i=0;i<(type+1);i++)
	{
		BEEP=1;
		delay_ms(50);
		BEEP=0;
		delay_ms(70);
	}	 
}

//���ݵ�ǰ������,����������.
void calendar_date_refresh(void) 
{
 	u8 weekn;   //�ܼĴ�
 	//��ʾ����������
	POINT_COLOR=BRED; 
	BACK_COLOR=BLACK; 
	LCD_ShowxNum(5,OTHER_TOPY+9,(calendar.w_year/100)%100,2,16,0);//��ʾ��  20/19  
	LCD_ShowxNum(21,OTHER_TOPY+9,calendar.w_year%100,2,16,0);     //��ʾ��  
	LCD_ShowString(37,OTHER_TOPY+9,240,320,16,"-"); //"-"
	LCD_ShowxNum(45,OTHER_TOPY+9,calendar.w_month,2,16,0);        //��ʾ��
	LCD_ShowString(61,OTHER_TOPY+9,240,320,16,"-"); //"-"
	LCD_ShowxNum(69,OTHER_TOPY+9,calendar.w_date,2,16,0);         //��ʾ��	  
	//��ʾ�ܼ�?
	POINT_COLOR=RED;
    weekn=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//�õ�����	   
	Show_Str(10,OTHER_TOPY+35,240,320,(u8 *)calendar_week_table[gui_phy.language][weekn],16,0); //��ʾ�ܼ�?	
													 
}
//��ɫ�б�.
const u16 t3d_colortbl[8]={0XFA8A,0XF800,0XFFFF,0X0673,0XF81F,0XFFFF,0X67E6,0XFFE0};
//����3dʱ��
void calendar_3dtime_refresh(_t3d_obj* t3dx)
{
	u8 tbuf[9];
	t3d_clear_obj(5,5,230,110,t3dx,BLACK);//�����һ�ε���ʾ
	tbuf[0]=calendar.hour/10+'0';
 	tbuf[1]=calendar.hour%10+'0';
	tbuf[2]=':';
	tbuf[3]=calendar.min/10+'0';
	tbuf[4]=calendar.min%10+'0';
	tbuf[5]=':';
 	tbuf[6]=calendar.sec/10+'0';
	tbuf[7]=calendar.sec%10+'0';
	tbuf[8]=0;//������
	t3dx->pixsize=0;
  	t3d_show_str(t3dx,-32,-8,-10,tbuf,16,(u16 *)t3d_colortbl,1);
}

//3D��ʾ
//t3dx:3D�ṹ��
//dx,dy.dz:3������ĽǶ�
void calendar_3dtime_show(_t3d_obj* t3dx,u8 dx,u8 dy,u8 dz)
{			   
	t3dx->ang.z+=dz;
 	t3dx->ang.y+=dy;
 	t3dx->ang.x+=dx;
	t3d_show_obj(5,5,230,110,t3dx,BLACK);
}
//3d��ʾ��λ�ñ��
void calendar_3dtime_pos_chg(_t3d_obj* t3dx,u8*dir)
{
	switch(*dir) 
	{
		case 0://x++
			t3dx->dis.x++;
			break;
		case 1://x--
			t3dx->dis.x--;
			break;
		case 2://y++
			t3dx->dis.y++;
			break;
		case 3://y--
			t3dx->dis.y--;
			break;
		case 4://x++,y++
			t3dx->dis.y++;
			t3dx->dis.x++;
			break;
		case 5://x--,y--
			t3dx->dis.y--;
			t3dx->dis.x--;
			break;
		case 6://y++,x--
			t3dx->dis.y++;
			t3dx->dis.x--;
			break;
		case 7://y--,x++
			t3dx->dis.x++;
			t3dx->dis.y--;
			break;
	}  
	//��Χ�޶� 
	if(t3dx->dis.x>85)
	{
		t3dx->dis.x=85;
	    *dir=1;
	}
	if(t3dx->dis.y>25)
	{
		t3dx->dis.y=25;
		*dir=3; 
	}
	if(t3dx->dis.x<-75)
	{	
		t3dx->dis.x=-75;
		*dir=0;
	}
	if(t3dx->dis.y<-15)
	{
		t3dx->dis.y=-15;
		*dir=2;
	}
}    

//�������ݱ�����:SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj)
//��ȡ����������Ϣ
//alarm:������Ϣ 
void calendar_read_para(_alarm_obj * alarm)
{
	AT24CXX_Read(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj),(u8*)alarm,sizeof(_alarm_obj));
}
//д������������Ϣ
//alarm:������Ϣ 
void calendar_save_para(_alarm_obj * alarm)
{
  	OS_CPU_SR cpu_sr=0;
	alarm->ringsta&=0X7F;	//������λ
	OS_ENTER_CRITICAL();	//�����ٽ���(�޷����жϴ��) 
	AT24CXX_Write(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj),(u8*)alarm,sizeof(_alarm_obj));
	OS_EXIT_CRITICAL();		//�˳��ٽ���(���Ա��жϴ��)
}

//���崦��(�ߴ�:200*160)
//x,y:����
//����ֵ,������
u8 calendar_alarm_msg(u16 x,u16 y)
{
	u8 rval=0; 
	u8 res;
	u32 abr;						    
	FIL* falarm=0;				  	
  	u8 *bfbase=0;					//���ֿ�Ļ�ַ
	_window_obj* twin=0;			//����
 	_btn_obj * rbtn=0;				//ȡ����ť		  
 	_btn_obj * okbtn=0;				//ȷ����ť		  

   	twin=window_creat(x,y,200,160,0,1|1<<5|1<<6,16);//��������,��ȡ����ɫ
   	okbtn=btn_creat(x+20,y+120,70,30,0,0x02);		//������ť
 	rbtn=btn_creat(x+20+70+20,y+120,70,30,0,0x02);	//������ť
 	falarm=(FIL *)gui_memin_malloc(sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
	if(twin==NULL||rbtn==NULL||okbtn==NULL||falarm==NULL)rval=1; 
	else
	{
		//���ڵ����ֺͱ���ɫ
		twin->caption=(u8*)calendar_alarm_caption_table[gui_phy.language];
		twin->windowbkc=APP_WIN_BACK_COLOR;	
		//���ذ�ť����ɫ    
		rbtn->bkctbl[0]=0X8452;		//�߿���ɫ
		rbtn->bkctbl[1]=0XAD97;		//��һ�е���ɫ				
		rbtn->bkctbl[2]=0XAD97;		//�ϰ벿����ɫ
		rbtn->bkctbl[3]=0X8452;		//�°벿����ɫ
		okbtn->bkctbl[0]=0X8452;	//�߿���ɫ
		okbtn->bkctbl[1]=0XAD97;	//��һ�е���ɫ				
		okbtn->bkctbl[2]=0XAD97;	//�ϰ벿����ɫ
		okbtn->bkctbl[3]=0X8452;	//�°벿����ɫ

		rbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];		//ȡ����ť
 		okbtn->caption=(u8*)calendar_alarm_realarm_table[gui_phy.language];	//���찴ť

		res=f_open(falarm,(const TCHAR*)APP_ASCII_60,FA_READ);//���ļ� 
		if(res==0)
		{
			bfbase=(u8*)gui_memex_malloc(falarm->fsize);	//Ϊ�����忪�ٻ����ַ
			if(bfbase==0)rval=1;
			else 
			{
				res=f_read(falarm,bfbase,falarm->fsize,(UINT*)&abr);	//һ�ζ�ȡ�����ļ�
			}
			f_close(falarm);
		}

 		window_draw(twin);						//��������
		btn_draw(rbtn);							//����ť
		btn_draw(okbtn);						//����ť			    
 		if(res)rval=res;
		else 									//��ʾ����ʱ��
		{
			app_showbig2num(bfbase,x+15,y+32+14,alarm.hour,60,BLUE,APP_WIN_BACK_COLOR); 	//��ʾʱ
 			app_showbigchar(bfbase,x+15+60,y+32+14,':',60,BLUE,APP_WIN_BACK_COLOR); 		//":"
			app_showbig2num(bfbase,x+15+90,y+32+14,alarm.min,60,BLUE,APP_WIN_BACK_COLOR);	//��ʾ��	 
		}
		OSTaskSuspend(6); //����������
  		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ  
			delay_ms(5); 
 	 		if(system_task_return)
			{
				rval=1;			//ȡ��
				break;			//TPAD����	
			}
			res=btn_check(rbtn,&in_obj);			//ȡ����ť���
			if(res&&((rbtn->sta&0X80)==0))			//����Ч����
			{
				rval=1;
				break;//�˳�
			}
	 		res=btn_check(okbtn,&in_obj);			//���찴ť���
	 		if(res&&((okbtn->sta&0X80)==0))			//����Ч����
			{		 
				rval=0XFF;//		   
				break;//�˳�
			}    
		}	 
	}
	alarm.ringsta&=~(1<<7);	//ȡ������
	if(rval==0XFF)//�Ժ�����
	{
		alarm.min+=5; 			//�Ƴ�5����
		if(alarm.min>59)
		{
			alarm.min=alarm.min%60;
			alarm.hour++;
			if(alarm.hour>23)alarm.hour=0;
		}				  
		calendar_alarm_init((_alarm_obj*)&alarm);//���³�ʼ������
 	}
	window_delete(twin);
	btn_delete(rbtn);
	btn_delete(okbtn); 	  
	gui_memin_free(falarm);
	gui_memex_free(bfbase);
	system_task_return=0;	//ȡ��TPAD
	OSTaskResume(6); 		//�ָ�������
	return rval;
}
	    
//ʱ����ʾģʽ
//ͨ�����󻬶��˳�.						 
//�����ڲ�sram,ò��OK��.	   
u8 calendar_play(void)
{
	u8 second=0;
	short temperate=0;	//�¶�ֵ		   
	u8 t=0;
	u8 tempdate=0;
	u8 rval=0;			//����ֵ	
	u8 res;

	u16 lastx=0;		//��¼�������һ�μ���xֵ
	u8  slidercnt=0;	//����������
	  
	u8 TEMP_SEN_TYPE=0;	//Ĭ��ʹ��DS18B20
	FIL* f_calendar=0;	 
	u8 *bfbase=0;		//���ֿ�Ļ�ַ
   	/////////////3Dʱ����ʾ///////////////
	u8 dir=0;
	u8 dx=0,dy=0,dz=0;			 
	_t3d_obj* t3d_time;	//3Dʱ��
	u16 timex=0;
	//////////////////////////////////////
	t3d_time=t3d_creat(120,60,50,500);//����һ��500�����ص��3D��
 	if(t3d_time==NULL)rval=1;		//����ʧ��
 	f_calendar=(FIL *)gui_memin_malloc(sizeof(FIL));//����FIL�ֽڵ��ڴ����� 
	if(f_calendar==NULL)rval=1;		//����ʧ��
	else
	{
		res=f_open(f_calendar,(const TCHAR*)APP_ASCII_60,FA_READ);//���ļ� 
		if(res==FR_OK)
		{
			bfbase=(u8*)gui_memex_malloc(f_calendar->fsize);	//Ϊ�����忪�ٻ����ַ
			if(bfbase==0)rval=1;
			else 
			{
				res=f_read(f_calendar,bfbase,f_calendar->fsize,(UINT*)&br);	//һ�ζ�ȡ�����ļ�
 			}
			f_close(f_calendar);
		} 
		if(res)rval=res;
	} 	    
	if(rval==0)//�޴���
	{	  
 		LCD_Clear(BLACK);//�����    	  
		second=calendar.sec;//�õ��˿̵�����
		POINT_COLOR=GBLUE;
		Show_Str(48,60,240,320,(u8*)calendar_loading_str[gui_phy.language][0],16,0x01); //��ʾ������Ϣ	    
		if(DS18B20_Init())
		{
			Show_Str(48,76,240,320,(u8*)calendar_loading_str[gui_phy.language][1],16,0x01);  
			delay_ms(500);
			Show_Str(48,92,240,320,(u8*)calendar_loading_str[gui_phy.language][2],16,0x01);  
			Adc_Init();//��ʼ���ڲ��¶ȴ�����	  
			TEMP_SEN_TYPE=1; 
		}   	    
		delay_ms(1100);//�ȴ�1.1s 
		BACK_COLOR= BLACK;
		LCD_Clear(BLACK);//����� 
		calendar_date_refresh();  //��������
		tempdate=calendar.w_date;//�����ݴ���

		app_showbigchar(bfbase,70-4,TIME_TOPY,':',60,GBLUE,BLACK); 		//":"
		app_showbigchar(bfbase,150-4,TIME_TOPY,':',60,GBLUE,BLACK); 	//":"	 
		//gui_draw_rectangle(4,4,232,112,GRED);//��3D��ʾ�ı߿�.
	}
  	while(rval==0)
	{	
		tp_dev.scan(0);    
		if(tp_dev.sta&TP_PRES_DOWN)//����������
		{
			if(slidercnt==0)//��һ�λ�������
			{
				lastx=tp_dev.x;//��¼��һ�ΰ��µ�����
				slidercnt++;				
			}else
			{
				if(tp_dev.x>(lastx+10))slidercnt=0;//���һ�����,ֱ�Ӷ���  
				if((tp_dev.x+10)<lastx)//���󻬶�����10�����ص�
				{
					slidercnt++; 		//��һ����Ч�Ļ���
					lastx=tp_dev.x;
				}
				if(slidercnt>5)break;	//������Ч,ֱ���˳�
			}
		}else slidercnt=0;				//�ɿ���,��ֱ�����û�����Ч
		if(system_task_return)break;	//TPAD����	  
 		if(second!=calendar.sec) //���Ӹı���
		{ 	
  			second=calendar.sec;  
			app_showbig2num(bfbase,10,TIME_TOPY,calendar.hour,60,GBLUE,BLACK);	//��ʾʱ
 			app_showbig2num(bfbase,90,TIME_TOPY,calendar.min,60,GBLUE,BLACK);	//��ʾ��	 
 			app_showbig2num(bfbase,170,TIME_TOPY,calendar.sec,60,GBLUE,BLACK);	//��ʾ��	 					   
			if(t%2==0)//�ȴ�2����
			{		 
  				if(TEMP_SEN_TYPE)temperate=Get_Temp();//�õ��ڲ��¶�
				else temperate=DS18B20_Get_Temp();//�õ�18b20�¶�
				if(temperate<0)//�¶�Ϊ������ʱ�򣬺�ɫ��ʾ
				{
					POINT_COLOR=RED;
					temperate=-temperate;//��Ϊ���¶�
				}
				else POINT_COLOR=BRRED;		   //����Ϊ�غ�ɫ������ʾ		 
				app_showbig2num(bfbase,90,OTHER_TOPY,temperate/10,60,GBLUE,BLACK);			//XX
				app_showbigchar(bfbase,150,OTHER_TOPY,'.',60,GBLUE,BLACK); 					//"."
				app_showbigchar(bfbase,180-15,OTHER_TOPY,temperate%10+'0',60,GBLUE,BLACK);	//��ʾС��
				app_showbigchar(bfbase,210-10,OTHER_TOPY,'C',60,GBLUE,BLACK);				//"/"
				if(t>0)t=0;			 
			}  
			if(calendar.w_date!=tempdate)
			{
				calendar_date_refresh();//�����仯��,��������.  
				tempdate=calendar.w_date;//�޸�tempdate����ֹ�ظ�����
			}
			t++;  
 			calendar_3dtime_refresh(t3d_time);
 		}
 		timex++;
   		delay_ms(10);
 		if((timex%800)==0)//ÿ8���Ӹı�һ�η���
		{
 			dir=app_get_rand(8);
			timex=0;	
		}
		if((timex%400)==0)//ÿ2���Ӹı�һ�νǶ�
		{
 			dx=app_get_rand(20);
			dy=app_get_rand(20);
			dz=app_get_rand(20);	  
		}
 		if((timex%20)==0)//ÿ200ms����һ��
		{
			calendar_3dtime_pos_chg(t3d_time,&dir);	 
 		}
  		calendar_3dtime_show(t3d_time,dx,dy,dz);
 	};
 	while(tp_dev.sta&TP_PRES_DOWN)tp_dev.scan(0);//�ȴ�TP�ɿ�.
	t3d_delete(t3d_time);		//ɾ��������ڴ�
	gui_memex_free(bfbase);		//ɾ��������ڴ�
	gui_memin_free(f_calendar);	//ɾ��������ڴ�
	POINT_COLOR=BLUE;
	BACK_COLOR=WHITE ;	
	return rval;
}




















