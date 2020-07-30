#include "includes.h"	   
#include "i2c0.h"



//volatile u8 pdata u_IAQppm_data=0;//����������IAQŨ��

static volatile u8  u_IAQppm_buf[10]={0};

/*char aiq2000_sensor_read_bytes(u8 dev_addr, u8  reg_addr, u8* reg_data, u8  r_len)
{



}
*/




//static  u8 f_IIC_Init=0;//��ʼ����־

static 	volatile unsigned long	uTVOC_Buf=0;//
// 	volatile unsigned long	uTVOC=0;//

static  volatile u8 i_IAQ2000=0;
static  volatile u8 i_IAQ2000_No=0;//���ڣ�����



 	volatile unsigned int	uHCHO_S_Former_TVOC=0;//��ȩŨ��ֵ,//���TVOC
 	volatile unsigned int	uHCHO_S_Former_TVOC_B=0;//��ȩŨ��ֵ,//���TVOC//�㷨����


static  volatile u16  uHCHO_Out_Zero_Time_No=0;//��ȩΪ0���ʱ��,����

static  volatile u16  uHCHO_TVOC_224_Time_No=0;//TVOC��0.224ʱ����ֹ��ȩͻ�䣬����5���ӵ���ʱ


void User_IAQ2000_Read(void)//��������
{


	

if(CMachine.uTVOC_Res_Start_Time_No!=0)
{
	 I2C0_UnInit();
	return;		
}
		


/*
if(f_IIC_Init==0)
{
	f_IIC_Init=1;
	//I2C0_Init();
}
*/




	I2C0_Start();
	I2C0_WriteByte(0xB5);
	I2C0_WaitACK();

	
	delayusi2c(1000);	//��֪��Ϊʲô��Ҫ�ϵ��ʼ��IOΪ�ߣ�����߼���ʱ����ȡ���ݲ�����
	
	for(i_IAQ2000=0;i_IAQ2000<8;i_IAQ2000++){
		u_IAQppm_buf[i_IAQ2000]=I2C0_ReadByte(1);
	}
	u_IAQppm_buf[8]=I2C0_ReadByte(0);
	I2C0_Stop();

	if(u_IAQppm_buf[7]==0xff)
	{
u_IAQppm_buf[7]=0;
u_IAQppm_buf[8]=0;
	}

	uTVOC_Buf=(u_IAQppm_buf[7]<<8);
	uTVOC_Buf+=u_IAQppm_buf[8];
	uTVOC_Buf*=1799;
	uTVOC_Buf/=1000;

	if(uTVOC_Buf>9999) //����
	{
		i_IAQ2000_No++;
		if(i_IAQ2000_No>3)		CMachine.tvoc=(uTVOC_Buf);
	}
	else 
	{
		i_IAQ2000_No=0;
			CMachine.tvoc=(uTVOC_Buf);
	}
	

	
	if(CMachine.tvoc>9999) CMachine.tvoc=9999;
	
	uTVOC_Buf=0;

	
/*
2����ȩ��׼��ʽ��



S=��ȩ���ֵ


X=��ȩ��ʾֵ

T=TVOCֵ


A=��T-0.224����2.44


�տ���ʱ������TVOC ��3����Ԥ�ȣ�T=0.224,��ʱ��XҲ��ʾΪ0����TvocԤ���꣬��ʼ��ʾ��ֵʱ�����������Ա��⿪��ʱ����ȩ��ֵƫ�ߣ��������Ӻ���ͻȻ�½�����˵������ҲҪ˵������ȩҲ��ҪԤ��3���ӡ���

X=min��A,S����



���S=0����15���ӣ���X=A��ֱ��S��0����X=min��A,S��
*/





				uHCHO_S_Former_TVOC_B=(CMachine.tvoc-224)/2.44;		



	if(uHCHO_S_Former_TVOC==0)
	{
		uHCHO_Out_Zero_Time_No++;
		if(uHCHO_Out_Zero_Time_No>300)	//����5����		
		{
			uHCHO_Out_Zero_Time_No=300;
			CMachine.hcho=uHCHO_S_Former_TVOC_B;	
			return;
		}
	}else
	{
		uHCHO_Out_Zero_Time_No=0;
		
		if(uHCHO_S_Former_TVOC>uHCHO_S_Former_TVOC_B) CMachine.hcho=uHCHO_S_Former_TVOC_B;		
		else CMachine.hcho=uHCHO_S_Former_TVOC;		
		
	}	
	







	
	//��������ȩ
/*
1������ȩ���ֵS=0����5����ʱ����

��ȩ��ʾֵX=A������ A=��tvoc ��ֵ -  0.224 ���� 6

������ȩ���ֵS��Ϊ0ʱ����ȩ��ʾֵXΪ���ֵ  X=S��
˵�����������ʺ��ڼ�ȩ������ʧ����ж�������tvoc ��ֵ���

2����tvocֵ С��0.32������0.224ʱ����ȩ�����ֵS������ڻ��ߵ��� A=��tvoc ֵ-0.224���� 3�����ȩ��ʾ��ֵXΪA�����С��A������ʾʵ����ֵX=S����ʱ�������1�����������1���������
	
3.��tvoc ���ڵ���0.32ʱС��1.5ʱ��S������ڻ��ߵ���B=TVOC��ֵ��3����X=B,  ���SС��B,��X=S

4.��tvoc ���ڵ���1.5ʱ��S������ڻ��ߵ���  B=TVOC��ֵ��2.48����X=B,  ���SС��B,��X=S	
	
	
	
	*/
	
	
	
	/*
	if(uHCHO_S_Former_TVOC==0)
	{
		uHCHO_Out_Zero_Time_No++;
		if(uHCHO_Out_Zero_Time_No>300)	//����5����		
		{
			uHCHO_Out_Zero_Time_No=300;
			CMachine.hcho=(CMachine.tvoc-224)/6;	
			return;
		}
	}else
	{
		uHCHO_Out_Zero_Time_No=0;
	}	
	
	
			if(CMachine.tvoc==224)		
			{
				if(uHCHO_TVOC_224_Time_No>0)//��ֹ5����ͻ��
				{
					uHCHO_TVOC_224_Time_No--;

					CMachine.hcho=0;		
					
				}else CMachine.hcho=uHCHO_S_Former_TVOC;				
				
			}else if(CMachine.tvoc>224&&CMachine.tvoc<320)	
			{
				uHCHO_TVOC_224_Time_No=300;//5����
				
				uHCHO_S_Former_TVOC_B=(CMachine.tvoc-224)/3;		
				if(uHCHO_S_Former_TVOC>=uHCHO_S_Former_TVOC_B)	CMachine.hcho=uHCHO_S_Former_TVOC_B;				
				else	CMachine.hcho=uHCHO_S_Former_TVOC;	
				
			}else if(CMachine.tvoc>=320&&CMachine.tvoc<1500)	
			{
				uHCHO_S_Former_TVOC_B=(CMachine.tvoc)/3;		
				if(uHCHO_S_Former_TVOC>=uHCHO_S_Former_TVOC_B)	CMachine.hcho=uHCHO_S_Former_TVOC_B;				
				else	CMachine.hcho=uHCHO_S_Former_TVOC;				
			}else if(CMachine.tvoc>=1500)	
			{
				uHCHO_S_Former_TVOC_B=(CMachine.tvoc)/2.48;		
				if(uHCHO_S_Former_TVOC>=uHCHO_S_Former_TVOC_B)	CMachine.hcho=uHCHO_S_Former_TVOC_B;				
				else	CMachine.hcho=uHCHO_S_Former_TVOC;				
			}
				
				
				
				*/
	
	
	
	
	
	
	
	

	





}


void User_IAQ2000_Init(void)//��������
{


 I2C0_Init();

		
	
}
