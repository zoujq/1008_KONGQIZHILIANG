#include "includes.h"	   

//以下定义UID 的开始地址,有此芯片的UID 地址不是连续的
#define UID_0 (0x1FFF00E5)
#define UID_1 (0x1FFF4253)
#define UID_2 (0x1FFF8569)
//以下定义存储加密运算结果的地址
#define KEY_1 (0x08072275+0xd00)//807f000+d00=807fd00
#define KEY_2 (0x0807385E+0xd00)//807f100+d00=807fe00
#define KEY_3 (0x08072825+0xa00)//807f500+a00=807ff00


 unsigned char	f_Sys_Check=0;//系统检测 标志，制造错误

unsigned char CPU_ID[12]; //MCU 的96 位UID
unsigned char UID_KEY[12]; //运算结果

vu32	uUID_0=0;
vu32	uUID_1=0;
vu32	uUID_2=0;


vu32	uKEY_0=0;
vu32	uKEY_1=0;
vu32	uKEY_2=0;


union u32Tou8
{
vu32 ulongx;
vu16 u16x[2];
vu8 u8x[4];
}UIDu32tou8;
/******************************************************************************
*
* 功能名称 : UID_MEG
* 描述 : UID 运算处理
* 输入 : 无
* 输出 : 无
* 返回值 : 0：两个结果不相等,验证失败; 1: 两个结果相等,验证成功
******************************************************************************/
unsigned char UID_READ(void)
{
unsigned char i;
	
	uUID_0=UID_0+0xF703;
	uUID_1=UID_1+0xB599;
	uUID_2=UID_2+0x7287;	
	
	
	
	uKEY_0=KEY_1+0xCD8B;
	uKEY_1=KEY_2+0xB8A2;
	uKEY_2=KEY_3+0xCCDB;
	
	
	
UIDu32tou8.ulongx=*(u32*) uUID_0; // 获取CPU 唯一ID
CPU_ID[0]=UIDu32tou8.u8x[0];
CPU_ID[1]=UIDu32tou8.u8x[1];
CPU_ID[2]=UIDu32tou8.u8x[2];
CPU_ID[3]=UIDu32tou8.u8x[3];
UIDu32tou8.ulongx=*(u32*) uUID_1; //获取CPU 唯一ID
CPU_ID[4]=UIDu32tou8.u8x[0];
CPU_ID[5]=UIDu32tou8.u8x[1];
CPU_ID[6]=UIDu32tou8.u8x[2];
CPU_ID[7]=UIDu32tou8.u8x[3];
UIDu32tou8.ulongx=*(u32*) uUID_2; // 获取CPU 唯一ID
CPU_ID[8]=UIDu32tou8.u8x[0];
CPU_ID[9]=UIDu32tou8.u8x[1];
CPU_ID[10]=UIDu32tou8.u8x[2];
CPU_ID[11]=UIDu32tou8.u8x[3];
/* 以上代码也可用下面指针形式
*((u32*)CPU_ID+0)=*(u32*) UID_OFFSET_1; // 获取CPU 唯一ID
*((u32*)CPU_ID+1)=*(u32*) UID_OFFSET_2; // 获取CPU 唯一ID
*((u32*)CPU_ID+2)=*(u32*) UID_OFFSET_3; // 获取CPU 唯一ID
*/
UIDu32tou8.ulongx=*(u32*) uKEY_0; //读取运算结果
UID_KEY[0]=UIDu32tou8.u8x[0];
UID_KEY[1]=UIDu32tou8.u8x[1];
UID_KEY[2]=UIDu32tou8.u8x[2];
UID_KEY[3]=UIDu32tou8.u8x[3];
UIDu32tou8.ulongx=*(u32*) uKEY_1; //读取运算结果
UID_KEY[4]=UIDu32tou8.u8x[0];
UID_KEY[5]=UIDu32tou8.u8x[1];
UID_KEY[6]=UIDu32tou8.u8x[2];
UID_KEY[7]=UIDu32tou8.u8x[3];
UIDu32tou8.ulongx=*(u32*) uKEY_2; //读取运算结果
UID_KEY[8]=UIDu32tou8.u8x[0];
UID_KEY[9]=UIDu32tou8.u8x[1];
UID_KEY[10]=UIDu32tou8.u8x[2];
UID_KEY[11]=UIDu32tou8.u8x[3];
/* 以上代码也可用下面指针形式
*((u32*)UID_KEY+0)=*(u32*) KEY_OFFSET_1; //读取运算结果
*((u32*)UID_KEY+1)=*(u32*) KEY_OFFSET_2; //读取运算结果
*((u32*)UID_KEY+2)=*(u32*) KEY_OFFSET_3; //读取运算结果
*/



	
//0
CPU_ID[0]>>=0x01; //运算法则1
CPU_ID[0]^=0x31; //运算法则2
CPU_ID[0]*=0x22; //运算法则3
CPU_ID[0]+=0x15; //运算法则4
CPU_ID[0]^=0x55; //运算法则5
//1
CPU_ID[1]+=0x56; //运算法则1
CPU_ID[1]^=0x23; //运算法则2
CPU_ID[1]-=0x33; //运算法则3
CPU_ID[1]^=0x56; //运算法则4
CPU_ID[1]*=0x44; //运算法则5
//2
CPU_ID[2]+=0x45; //运算法则1
CPU_ID[2]*=0x22; //运算法则2
CPU_ID[2]|=0x28; //运算法则3
CPU_ID[2]<<=0x01; //运算法则4
CPU_ID[2]^=0x56; //运算法则5
//3
CPU_ID[3]*=0xcd; //运算法则1
CPU_ID[3]-=0x21; //运算法则2
CPU_ID[3]|=0x19; //运算法则3
CPU_ID[3]&=0xCE; //运算法则4
CPU_ID[3]+=0x51; //运算法则5
//4
CPU_ID[4]-=0x78; //运算法则1
CPU_ID[4]^=0x35; //运算法则2
CPU_ID[4]*=0x15; //运算法则3
CPU_ID[4]|=0x23; //运算法则4
CPU_ID[4]+=0x32; //运算法则5
//5
CPU_ID[5]+=0x38; //运算法则1
CPU_ID[5]^=0x73; //运算法则2
CPU_ID[5]&=0xac; //运算法则3
CPU_ID[5]+=0x55; //运算法则4
CPU_ID[5]*=0x66; //运算法则5
//6
CPU_ID[6]^=0x58; //运算法则1
CPU_ID[6]&=0xEE; //运算法则2
CPU_ID[6]*=0x21; //运算法则3
CPU_ID[6]+=0x56; //运算法则4
CPU_ID[6]^=0x9a; //运算法则5
//7
CPU_ID[7]>>=0x01; //运算法则1
CPU_ID[7]+=0x57; //运算法则2
CPU_ID[7]|=0x39; //运算法则3
CPU_ID[7]+=0x24; //运算法则4
CPU_ID[7]^=0x23; //运算法则5
	



/*
//换算完成，开始比较
f_Sys_Check=2;//赋值
for(i=0;i<8;i++)	CPU_ID[i]=UID_KEY[i];


return 0;
*/

//换算完成，开始比较
f_Sys_Check=2;//赋值
for(i=0;i<8;i++)
{
if(CPU_ID[i]!=UID_KEY[i])//不通过，数据出错
{
	
f_Sys_Check=0;//错误
	
break;
}






}

return 0;
}













