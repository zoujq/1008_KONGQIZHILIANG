#include "wirelessplay.h"
#include "paint.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-����ͨ�� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//�ػ���ť������
const u8* wireless_cbtn_caption_tbl[GUI_LANGUAGE_NUM]=
{
"�ػ�","�خ�","CLEAR",	 
}; 
//��������
const u8* wireless_remind_msg_tbl[GUI_LANGUAGE_NUM]=
{
"δ��⵽NRF24L01,����!","δ�z�y��NRF24L01,Ո�z��!","No NRF24L01,Please Check...",	 
};
//ģʽѡ��
const u8* wireless_mode_tbl[GUI_LANGUAGE_NUM][2]=
{
{"����ģʽ","����ģʽ",},
{"�l��ģʽ","����ģʽ",},		 
{"TX Mode","RX Mode",},
};	   
/////////////////////////////////////////////////////////////////////////////////

//���ߴ���
u8 wireless_play(void)
{
   	_btn_obj* cbtn=0;		//�����ť�ؼ�
  	u8 res;
	u8 *caption;			//����
	u8 mode=0;				//0,����ģʽ;1,����ģʽ
	u8 tmp_buf[6];			//buf[0~3]:����ֵ;buf[4]:0,������ͼ;1,����;2,�˳�. 
	u16 x=0,y=0;
 	u8 rval=0;

  	br=0;
	//��ѡ��ģʽ
	res=app_items_sel(30,64,180,72+40*2,(u8**)wireless_mode_tbl[gui_phy.language],2,(u8*)&mode,0X90,(u8*)APP_MODESEL_CAPTION_TBL[gui_phy.language]);//2��ѡ��
 	if(res==0)//ȷ�ϼ�������,ͬʱ������������
	{
    	caption=(u8*)APP_MFUNS_CAPTION_TBL[16][gui_phy.language];//���ߴ���	 
		gui_fill_rectangle(0,0,240,320,LGRAY);//��䱳��ɫ
 		NRF24L01_Init();    		//��ʼ��NRF24L01 
		res=0;
		while(NRF24L01_Check())		//��ⲻ��24L01,�������5��
		{
			if(res==0)window_msg_box(20,110,200,70,(u8*)wireless_remind_msg_tbl[gui_phy.language],(u8*)APP_REMIND_CAPTION_TBL[gui_phy.language],12,0,0);
			delay_ms(500);
			res++;
			if(res>5)return 0;//��ʱδ��⵽,�˳�
		}				  
   		gui_fill_rectangle(0,20,240,280,LGRAY);	//��䱳��ɫ
 		if(mode==0)NRF24L01_TX_Mode();			//����ģʽ
		else NRF24L01_RX_Mode();    
		caption=(u8*)wireless_mode_tbl[gui_phy.language][mode];	//����
 		app_filebrower(caption,0X07);	 						//��ʾ���� 
 		cbtn=btn_creat(0,300,40,19,0,0x03);//�������ְ�ť
		if(cbtn==NULL)rval=1;	//û���㹻�ڴ湻����
		else
		{																				
		 	cbtn->caption=(u8*)wireless_cbtn_caption_tbl[gui_phy.language];//�ػ�
		 	cbtn->font=16;			//����		 
			cbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
			cbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
 			btn_draw(cbtn);			//�ػ���ť
		}
		tmp_buf[4]=3;//Ĭ�ϲ����κδ���	  
		while(rval==0)
		{	 
			tp_dev.scan(0); 		 
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ 	  
 			if(system_task_return)	//TPAD����
			{
		 		tmp_buf[4]=2;		//����2,�˳�
				if(mode==0)			//����ģʽ,��Ҫ�����˳�ָ��
				{
					NRF24L01_TxPacket(tmp_buf);	//����ģʽ,�����˳�ָ��	
 				}				 
 			}	    
			if(mode==0)//����ģʽ
			{
				res=btn_check(cbtn,&in_obj);//����ػ���ť
				if(res)//�ػ���ť����Ч����
				{
					if(((cbtn->sta&0X80)==0))//��ť״̬�ı���
					{
						tmp_buf[4]=1;				//����1,����
						NRF24L01_TxPacket(tmp_buf);	//�������ָ��
		 			}	 
				}
	 			if(tp_dev.sta&TP_PRES_DOWN)			//������������
				{	
				 	if(tp_dev.y<(300-2)&&tp_dev.y>(20+1))	//�ڻ�ͼ������
					{		
						x=tp_dev.x;
						y=tp_dev.y;
						tmp_buf[0]=tp_dev.x>>8;
						tmp_buf[1]=tp_dev.x&0xFF;
						tmp_buf[2]=tp_dev.y>>8;	 
						tmp_buf[3]=tp_dev.y&0xFF; 
						tmp_buf[4]=0;				//����Ϊ0,������ͼ													        			   
						NRF24L01_TxPacket(tmp_buf);	//��������
					}
				}
			}else	//����ģʽ
			{
				if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
				{
					x=tmp_buf[0];
					x=(x<<8)+tmp_buf[1];
	 				y=tmp_buf[2];
					y=(y<<8)+tmp_buf[3];   
 				}	  
 			}
			if(tmp_buf[4]<3)//��Ҫ����
			{
				switch(tmp_buf[4])
				{
					case 0://��������
						paint_draw_point(x,y,RED,2);//��ͼ,�뾶Ϊ2				    
						break;
					case 1://���
						gui_fill_rectangle(0,20,240,280,LGRAY);//�����Ļ		
						break;
					case 2://�˳�						  
						SPI2_SetSpeed(SPI_BaudRatePrescaler_2);//���ûظ���.18Mhz 
						rval=1;//��־�˳�
						break; 
				}
				tmp_buf[4]=3;
			}else delay_ms(1000/OS_TICKS_PER_SEC);//���� ��ʱһ��ʱ�ӽ���  
		}   
	}
	SPI2_Init();    	//��ʼ��SPI	 
	SPI2_SetSpeed(SPI_BaudRatePrescaler_2);//���õ�����ģʽ	 
 	btn_delete(cbtn);	//ɾ����ť
	return 0;
}

void rx_test(void)
{
	u8 res=0;
 	u8 tmp_buf[6];			//buf[0~3]:����ֵ;buf[4]:0,������ͼ;1,����;2,�˳�. 
 	u16 t=0;
 	NRF24L01_Init();    		//��ʼ��NRF24L01 
	while(NRF24L01_Check())		//��ⲻ��24L01,�������5��
	{
 		res++;
		if(res>5)return ;//��ʱδ��⵽,�˳�
	}	
	NRF24L01_RX_Mode();  			  
	while(1)
	{
 	    		    				 
		if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
		{
			tmp_buf[5]=0;//�����ַ��������� 
			LCD_ShowString(0,190,240,320,16,tmp_buf); 
		}else delay_us(100);	   
		t++;
		if(t==10000)//��Լ1s�Ӹı�һ��״̬
		{
			t=0;
			LED0=!LED0;
		} 				    
	};	 
}


























