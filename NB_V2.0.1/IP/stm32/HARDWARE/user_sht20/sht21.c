



#include "myiic2.h"
#include "includes.h"	   


u8 uTime_No=0;//分时复用
unsigned short temp_charge_time_10min_no;//温度较正，加上充电时间	
unsigned short temp_charge_time_30min_no;//温度较正，加上充电时间	
unsigned short temp_charge_time_out_10min_no;//温度较正，充电器移除时间

unsigned short temp_buf;//温度缓存
	
	
	
//初始化IIC接口
void SHT21_Init(void)
{

	
	 uTime_No=0;//清零
	temp_charge_time_10min_no=0;//清零
	temp_charge_time_30min_no=0;//清零	
	temp_charge_time_out_10min_no=0;//清零	
	
	CMachine.temp=40;//刚开始显示0度
	
 MYIIC2_Init();                //初始化IIC的IO口			


}

//初始化IIC接口
void SHT21_UnInit(void)
{

 MYIIC2_UnInit();                //初始化IIC的IO口			


}



//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 SHT21_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    MYIIC2_Start();  
	MYIIC2_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	MYIIC2_Wait_Ack(); 
    MYIIC2_Send_Byte(ReadAddr%256);   //发送低地址
	MYIIC2_Wait_Ack();	    
	MYIIC2_Start();  	 	   
	MYIIC2_Send_Byte(0XA1);           //进入接收模式			   
	MYIIC2_Wait_Ack();	 
    temp=MYIIC2_Read_Byte(0);		   
    MYIIC2_Stop();//产生一个停止条件	    
	return temp;	
	

}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void SHT21_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
  
    MYIIC2_Start();  
	MYIIC2_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	MYIIC2_Wait_Ack();	   
    MYIIC2_Send_Byte(WriteAddr%256);   //发送低地址
	MYIIC2_Wait_Ack(); 	 										  		   
	MYIIC2_Send_Byte(DataToWrite);     //发送字节							   
	MYIIC2_Wait_Ack();  		    	   
    MYIIC2_Stop();//产生一个停止条件 
//	delay(10000);//10ms左右
	
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数

void SHT21_Read(u16 ReadAddr,u8 *pBuffer)
{


	if(uTime_No==0||uTime_No==2)  
	{		
    MYIIC2_Start();  
	MYIIC2_Send_Byte(0x80);   //
	MYIIC2_Wait_Ack(); 	
	MYIIC2_Send_Byte(ReadAddr);   //
	MYIIC2_Wait_Ack(); 
	}


	if(uTime_No==1||uTime_No==3)  
	{		
	MYIIC2_Start();  	 	   
	MYIIC2_Send_Byte(0X81);           //进入接收模式			   
	MYIIC2_Wait_Ack();	 

    (*pBuffer++)=MYIIC2_Read_Byte(1);		
	   (*pBuffer++)=MYIIC2_Read_Byte(1);	
	   (*pBuffer++)=MYIIC2_Read_Byte(0);	
    MYIIC2_Stop();//产生一个停止条件	    
	}
	

	
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void SHT21_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)//
{
	

	
	while(NumToWrite--)
	{
		SHT21_WriteOneByte(WriteAddr,*pBuffer);

		WriteAddr++;
		pBuffer++;
	}
	

}









signed int   uHumidity_Inside=0;//湿度
signed int    uTemp_Inside=0;//温度

u8  f_SHT20_Init=0;//




void User_SHT20_Task(void)
{

u8 buf[3]={0};

	

if(CMachine.uTVOC_Res_Start_Time_No!=0)	
{
	
 uTime_No=0;//清零

	
		temp_charge_time_10min_no=0;//清零
	temp_charge_time_30min_no=0;//清零
	temp_charge_time_out_10min_no=0;//清零	
	
 SHT21_UnInit();	
	
	return;
}else
{




}


/*
1、温湿度校准公式：



S=温度输出值；

X=温度显示值；


X=10/11*(S-3)


设备充电时，温度会上升，充满电后，温度会下降。


设定开始充电10分钟后，X=10/11*(S-4),  充电30分钟后，X=10/11*(S-5) ；

电量充满或者充电终止后，过10分钟，X=10/11*(S-3)；


usb 插头插在充电头上，设备可能处于充电状态，也可能是未充电状态，要根据设备充电电流来判断是否在充电。




湿度修正： 原来是+ 18，现在修改为+8 即可；



温湿度校准：
温度校准：调整到6合1显示界面，连续按5下开关键，温度闪动，按M键增加1度，按网络键减少温度1度

湿度校准：调整到6合1显示界面，连续按6下开关键，湿度闪动，按M键增加湿度1%，按网络键减少湿度1%



*/



	if(CMachine.fCharge==1&&CMachine.Battery<100)//充电状态
	{
		if(temp_charge_time_10min_no>600)	
		{
			temp_charge_time_10min_no=600;
			temp_charge_time_out_10min_no=600;//已到10分钟
		}else	temp_charge_time_10min_no++;
		

	}else
	{

		if(temp_charge_time_30min_no>0)	temp_charge_time_30min_no--;		
		if(temp_charge_time_10min_no>0)	temp_charge_time_10min_no--;
		if(temp_charge_time_out_10min_no>0)	temp_charge_time_out_10min_no--;	
	
	
	}





	if(uTime_No==0||uTime_No==1)
	{
	SHT21_Read(0xf5,buf);//湿度
if(uTime_No==1)	
{
	uHumidity_Inside=(-6+125*((buf[0]*256+buf[1])/65536.0))*10;
		CMachine.humidity=uHumidity_Inside;//湿度
	
		CMachine.humidity+=80;
		CMachine.humidity/=10;	
}
	}
		if(uTime_No==2||uTime_No==3)
	{
	SHT21_Read(0xf3,buf);//温度
if(uTime_No==3)	
{
	uTemp_Inside=(-46.85+175.72*((buf[0]*256+buf[1])/65536.0))*10;
	if(uTemp_Inside<-400)	uTemp_Inside=(-400);	
  temp_buf=(uTemp_Inside+400);//温度	

//	CMachine.pm25=temp_buf;
	
	
	
	if(temp_buf>=650)	temp_buf-=40;
	else	if(temp_buf>=600)	temp_buf-=45;
	else	if(temp_buf>=550)	temp_buf-=50;		
	else	if(temp_buf>=450)	temp_buf-=55;
	else		temp_buf-=60;
	
	


		if(temp_charge_time_out_10min_no>0)		
		{
			if(temp_buf>10) CMachine.temp=(temp_buf-10);//温度
			else	CMachine.temp=0;//温度		
		}else
		{
			
			if(temp_charge_time_10min_no>=600)
			{
				if(temp_buf>10) CMachine.temp=(temp_buf-10);//温度
				else	CMachine.temp=0;//温度						
			}else
			{
				CMachine.temp=temp_buf;//温度			
			}

			
		
		}	

		
  CMachine.temp/=10;
	
	
}
	}
	
	

/*
1、温湿度校准公式：



S=温度输出值；

X=温度显示值；


X=10/11*(S-7)


设备充电时，温度会上升，充满电后，温度会下降。


设定开始充电10分钟后，X=10/11*(S-8),  充电30分钟后，X=10/11*(S-9) ；

电量充满或者充电终止后，过10分钟，X=10/11*(S-7)；


usb 插头插在充电头上，设备可能处于充电状态，也可能是未充电状态，要根据设备充电电流来判断是否在充电。




湿度修正： 原来是+ 18，现在修改为+8 即可；





	if(CMachine.fCharge==1&&CMachine.Battery<100)//充电状态
	{
		if(temp_charge_time_10min_no>600)	
		{
			temp_charge_time_10min_no=600;
			temp_charge_time_out_10min_no=600;//已到10分钟
		}else	temp_charge_time_10min_no++;
		
		if(temp_charge_time_30min_no>1800)	temp_charge_time_30min_no=1800;		
		else	temp_charge_time_30min_no++;
		
	}else
	{

		if(temp_charge_time_30min_no>0)	temp_charge_time_30min_no--;		
		if(temp_charge_time_10min_no>0)	temp_charge_time_10min_no--;
		if(temp_charge_time_out_10min_no>0)	temp_charge_time_out_10min_no--;	
	
	
	}





	if(uTime_No==0||uTime_No==1)
	{
	SHT21_Read(0xf5,buf);//湿度
if(uTime_No==1)	
{
	uHumidity_Inside=(-6+125*((buf[0]*256+buf[1])/65536.0))*10;
		CMachine.humidity=uHumidity_Inside;//湿度
	
		CMachine.humidity+=80;
		CMachine.humidity/=10;	
}
	}
		if(uTime_No==2||uTime_No==3)
	{
	SHT21_Read(0xf3,buf);//温度
if(uTime_No==3)	
{
	uTemp_Inside=(-46.85+175.72*((buf[0]*256+buf[1])/65536.0))*10;
	if(uTemp_Inside<-400)	uTemp_Inside=(-400);	
  temp_buf=(uTemp_Inside+400);//温度	

	CMachine.pm25=temp_buf;
	
	if(temp_charge_time_10min_no<600)	
	{
		if(temp_charge_time_out_10min_no>0)		
		{
			if(temp_buf>80) CMachine.temp=10/11.0*(temp_buf-80);//温度
			else	CMachine.temp=0;//温度		
		}else
		{
			if(temp_buf>70) CMachine.temp=10/11.0*(temp_buf-70);//温度
			else	CMachine.temp=0;//温度
		
		}	
	}else
	{
		if(temp_charge_time_30min_no<1800)		
		{
			if(temp_buf>80) CMachine.temp=10/11.0*(temp_buf-80);//温度
			else	CMachine.temp=0;//温度		
		}else
		{
			if(temp_buf>90) CMachine.temp=10/11.0*(temp_buf-90);//温度
			else	CMachine.temp=0;//温度
		
		}
	}
		
  CMachine.temp/=10;
	
	
}
	}



	
*/
		


	
	


	

	


	
	
	


	
	
	/*
if(uSensorData.sensor_data_struct.uTemp>=400&&uSensorData.sensor_data_struct.uTemp<=500)//温度0-10度      温度-1  湿度+3
{
	uSensorData.sensor_data_struct.uTemp-=10;
	uSensorData.sensor_data_struct.uHumidity+=30;
}else
if(uSensorData.sensor_data_struct.uTemp>500&&uSensorData.sensor_data_struct.uTemp<=600)//温度10-20度     温度-2  湿度+4
{
	uSensorData.sensor_data_struct.uTemp-=20;
	uSensorData.sensor_data_struct.uHumidity+=40;
}else	
if(uSensorData.sensor_data_struct.uTemp>600&&uSensorData.sensor_data_struct.uTemp<=650)//温度 20-25度      温度-2.5 湿度+5
{
	uSensorData.sensor_data_struct.uTemp-=25;
	uSensorData.sensor_data_struct.uHumidity+=50;
}else	
if(uSensorData.sensor_data_struct.uTemp>650&&uSensorData.sensor_data_struct.uTemp<=700)//温度 25-30度      温度-3  湿度+6
{
	uSensorData.sensor_data_struct.uTemp-=30;
	uSensorData.sensor_data_struct.uHumidity+=60;
}else	
if(uSensorData.sensor_data_struct.uTemp>700)//温度大于30度    温度-4，湿度+7
{
	uSensorData.sensor_data_struct.uTemp-=40;
	uSensorData.sensor_data_struct.uHumidity+=70;
}	

	*/






			uTime_No++;
			if(uTime_No>3)	uTime_No=0;
			





}


















