#include "includes.h"	   

//���¶���UID �Ŀ�ʼ��ַ,�д�оƬ��UID ��ַ����������
#define UID_0 (0x1FFF00E5)
#define UID_1 (0x1FFF4253)
#define UID_2 (0x1FFF8569)
//���¶���洢�����������ĵ�ַ
#define KEY_1 (0x08072275+0xd00)//807f000+d00=807fd00
#define KEY_2 (0x0807385E+0xd00)//807f100+d00=807fe00
#define KEY_3 (0x08072825+0xa00)//807f500+a00=807ff00


 unsigned char	f_Sys_Check=0;//ϵͳ��� ��־���������

unsigned char CPU_ID[12]; //MCU ��96 λUID
unsigned char UID_KEY[12]; //������

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
* �������� : UID_MEG
* ���� : UID ���㴦��
* ���� : ��
* ��� : ��
* ����ֵ : 0��������������,��֤ʧ��; 1: ����������,��֤�ɹ�
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
	
	
	
UIDu32tou8.ulongx=*(u32*) uUID_0; // ��ȡCPU ΨһID
CPU_ID[0]=UIDu32tou8.u8x[0];
CPU_ID[1]=UIDu32tou8.u8x[1];
CPU_ID[2]=UIDu32tou8.u8x[2];
CPU_ID[3]=UIDu32tou8.u8x[3];
UIDu32tou8.ulongx=*(u32*) uUID_1; //��ȡCPU ΨһID
CPU_ID[4]=UIDu32tou8.u8x[0];
CPU_ID[5]=UIDu32tou8.u8x[1];
CPU_ID[6]=UIDu32tou8.u8x[2];
CPU_ID[7]=UIDu32tou8.u8x[3];
UIDu32tou8.ulongx=*(u32*) uUID_2; // ��ȡCPU ΨһID
CPU_ID[8]=UIDu32tou8.u8x[0];
CPU_ID[9]=UIDu32tou8.u8x[1];
CPU_ID[10]=UIDu32tou8.u8x[2];
CPU_ID[11]=UIDu32tou8.u8x[3];
/* ���ϴ���Ҳ��������ָ����ʽ
*((u32*)CPU_ID+0)=*(u32*) UID_OFFSET_1; // ��ȡCPU ΨһID
*((u32*)CPU_ID+1)=*(u32*) UID_OFFSET_2; // ��ȡCPU ΨһID
*((u32*)CPU_ID+2)=*(u32*) UID_OFFSET_3; // ��ȡCPU ΨһID
*/
UIDu32tou8.ulongx=*(u32*) uKEY_0; //��ȡ������
UID_KEY[0]=UIDu32tou8.u8x[0];
UID_KEY[1]=UIDu32tou8.u8x[1];
UID_KEY[2]=UIDu32tou8.u8x[2];
UID_KEY[3]=UIDu32tou8.u8x[3];
UIDu32tou8.ulongx=*(u32*) uKEY_1; //��ȡ������
UID_KEY[4]=UIDu32tou8.u8x[0];
UID_KEY[5]=UIDu32tou8.u8x[1];
UID_KEY[6]=UIDu32tou8.u8x[2];
UID_KEY[7]=UIDu32tou8.u8x[3];
UIDu32tou8.ulongx=*(u32*) uKEY_2; //��ȡ������
UID_KEY[8]=UIDu32tou8.u8x[0];
UID_KEY[9]=UIDu32tou8.u8x[1];
UID_KEY[10]=UIDu32tou8.u8x[2];
UID_KEY[11]=UIDu32tou8.u8x[3];
/* ���ϴ���Ҳ��������ָ����ʽ
*((u32*)UID_KEY+0)=*(u32*) KEY_OFFSET_1; //��ȡ������
*((u32*)UID_KEY+1)=*(u32*) KEY_OFFSET_2; //��ȡ������
*((u32*)UID_KEY+2)=*(u32*) KEY_OFFSET_3; //��ȡ������
*/



	
//0
CPU_ID[0]>>=0x01; //���㷨��1
CPU_ID[0]^=0x31; //���㷨��2
CPU_ID[0]*=0x22; //���㷨��3
CPU_ID[0]+=0x15; //���㷨��4
CPU_ID[0]^=0x55; //���㷨��5
//1
CPU_ID[1]+=0x56; //���㷨��1
CPU_ID[1]^=0x23; //���㷨��2
CPU_ID[1]-=0x33; //���㷨��3
CPU_ID[1]^=0x56; //���㷨��4
CPU_ID[1]*=0x44; //���㷨��5
//2
CPU_ID[2]+=0x45; //���㷨��1
CPU_ID[2]*=0x22; //���㷨��2
CPU_ID[2]|=0x28; //���㷨��3
CPU_ID[2]<<=0x01; //���㷨��4
CPU_ID[2]^=0x56; //���㷨��5
//3
CPU_ID[3]*=0xcd; //���㷨��1
CPU_ID[3]-=0x21; //���㷨��2
CPU_ID[3]|=0x19; //���㷨��3
CPU_ID[3]&=0xCE; //���㷨��4
CPU_ID[3]+=0x51; //���㷨��5
//4
CPU_ID[4]-=0x78; //���㷨��1
CPU_ID[4]^=0x35; //���㷨��2
CPU_ID[4]*=0x15; //���㷨��3
CPU_ID[4]|=0x23; //���㷨��4
CPU_ID[4]+=0x32; //���㷨��5
//5
CPU_ID[5]+=0x38; //���㷨��1
CPU_ID[5]^=0x73; //���㷨��2
CPU_ID[5]&=0xac; //���㷨��3
CPU_ID[5]+=0x55; //���㷨��4
CPU_ID[5]*=0x66; //���㷨��5
//6
CPU_ID[6]^=0x58; //���㷨��1
CPU_ID[6]&=0xEE; //���㷨��2
CPU_ID[6]*=0x21; //���㷨��3
CPU_ID[6]+=0x56; //���㷨��4
CPU_ID[6]^=0x9a; //���㷨��5
//7
CPU_ID[7]>>=0x01; //���㷨��1
CPU_ID[7]+=0x57; //���㷨��2
CPU_ID[7]|=0x39; //���㷨��3
CPU_ID[7]+=0x24; //���㷨��4
CPU_ID[7]^=0x23; //���㷨��5
	



/*
//������ɣ���ʼ�Ƚ�
f_Sys_Check=2;//��ֵ
for(i=0;i<8;i++)	CPU_ID[i]=UID_KEY[i];


return 0;
*/

//������ɣ���ʼ�Ƚ�
f_Sys_Check=2;//��ֵ
for(i=0;i<8;i++)
{
if(CPU_ID[i]!=UID_KEY[i])//��ͨ�������ݳ���
{
	
f_Sys_Check=0;//����
	
break;
}






}

return 0;
}













