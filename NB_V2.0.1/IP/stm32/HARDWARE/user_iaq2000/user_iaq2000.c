#include "includes.h"	   
#include "i2c0.h"



//volatile u8 pdata u_IAQppm_data=0;//空气质量，IAQ浓度

static volatile u8  u_IAQppm_buf[10]={0};

/*char aiq2000_sensor_read_bytes(u8 dev_addr, u8  reg_addr, u8* reg_data, u8  r_len)
{



}
*/




//static  u8 f_IIC_Init=0;//初始化标志

static 	volatile unsigned long	uTVOC_Buf=0;//
// 	volatile unsigned long	uTVOC=0;//

static  volatile u8 i_IAQ2000=0;
static  volatile u8 i_IAQ2000_No=0;//大于，计数



 	volatile unsigned int	uHCHO_S_Former_TVOC=0;//甲醛浓度值,//配合TVOC
 	volatile unsigned int	uHCHO_S_Former_TVOC_B=0;//甲醛浓度值,//配合TVOC//算法缓存


static  volatile u16  uHCHO_Out_Zero_Time_No=0;//甲醛为0输出时间,计数

static  volatile u16  uHCHO_TVOC_224_Time_No=0;//TVOC在0.224时，防止甲醛突变，进行5分钟倒计时


void User_IAQ2000_Read(void)//空气质量
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

	
	delayusi2c(1000);	//不知道为什么，要上电初始化IO为高，且这边加延时，读取数据才正常
	
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

	if(uTVOC_Buf>9999) //过滤
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
2、甲醛较准公式：



S=甲醛输出值


X=甲醛显示值

T=TVOC值


A=（T-0.224）÷2.44


刚开机时，由于TVOC 有3分钟预热，T=0.224,此时，X也显示为0，当Tvoc预热完，开始显示数值时，（这样可以避免开机时，甲醛数值偏高，过几分钟后又突然下降，在说明书中也要说明，甲醛也需要预热3分钟。）

X=min（A,S）；



如果S=0超过15分钟，则X=A，直到S≠0，则X=min（A,S）
*/





				uHCHO_S_Former_TVOC_B=(CMachine.tvoc-224)/2.44;		



	if(uHCHO_S_Former_TVOC==0)
	{
		uHCHO_Out_Zero_Time_No++;
		if(uHCHO_Out_Zero_Time_No>300)	//超过5分钟		
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
	







	
	//配合输出甲醛
/*
1、当甲醛输出值S=0超过5分钟时，则

甲醛显示值X=A，其中 A=（tvoc 数值 -  0.224 ）÷ 6

，当甲醛输出值S不为0时，甲醛显示值X为输出值  X=S。
说明：这设置适合于甲醛传感器失灵或中毒，采用tvoc 数值替代

2、当tvoc值 小于0.32，大于0.224时，甲醛输出数值S如果大于或者等于 A=（tvoc 值-0.224）÷ 3，则甲醛显示数值X为A，如果小于A，则显示实际数值X=S，此时如果出现1的情况，按照1的情况处理
	
3.当tvoc 大于等于0.32时小于1.5时，S如果大于或者等于B=TVOC数值÷3，则X=B,  如果S小于B,则X=S

4.当tvoc 大于等于1.5时，S如果大于或者等于  B=TVOC数值÷2.48，则X=B,  如果S小于B,则X=S	
	
	
	
	*/
	
	
	
	/*
	if(uHCHO_S_Former_TVOC==0)
	{
		uHCHO_Out_Zero_Time_No++;
		if(uHCHO_Out_Zero_Time_No>300)	//超过5分钟		
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
				if(uHCHO_TVOC_224_Time_No>0)//防止5分钟突变
				{
					uHCHO_TVOC_224_Time_No--;

					CMachine.hcho=0;		
					
				}else CMachine.hcho=uHCHO_S_Former_TVOC;				
				
			}else if(CMachine.tvoc>224&&CMachine.tvoc<320)	
			{
				uHCHO_TVOC_224_Time_No=300;//5分钟
				
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


void User_IAQ2000_Init(void)//空气质量
{


 I2C0_Init();

		
	
}
