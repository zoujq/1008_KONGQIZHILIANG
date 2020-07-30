#ifndef __CLOCK_SHOW_H__
#define __CLOCK_SHOW_H__


extern	u16 uRang_Times_30S;//����,30�����壬ÿ3����һ�Σ�����3��

extern u8 f_Rang_Run;//����ʹ�ܱ�־λ

	extern u16	uMode_Old;//֮ǰ�Ĳ˵����жϱ���ˢ����

	extern u8 uRang_Time[2];//����ʱ��

	extern u8 f_User_Sleep;//�û�˯�߱�־��0��˯��1˯��
	
	
	
	

void User_Clock_Init(void);//RTC��ʼ��
void User_Clock_Show_Task(void *pdata);//ʱ����ʾ����



void User_Music_Set_Into(void);//�������ֲ���
void User_Music_Set_Exit(void);//�˳����ֲ���
void User_Music_Set_Bit_Next(void);//���ֲ���,��һ������
void User_Music_Set_Bit_Before(void);//���ֲ���,��һ������
void User_Music_Set_Bit_In(void);//���ֲ��ţ�����ѡ��


void User_Step_Into(void);//����Ʋ�����
void User_Step_Show(void);//��ʾ��������
void User_Step_Exit(void);//�˳��Ʋ�����
void User_Step_Clear(void);//�Ʋ�����

void User_Distance_Exit(void);//�˳��׾����
void User_Distance_Show(void);//��ʾ�׾����
void User_Distance_Into(void);//�����׾����

void User_Search_Phone_Into(void);//��������ֻ�����
void User_Search_Phone_Able(void);//ѡ������ֻ���ر��ֻ�����
void User_Search_Phone_Show(void);//��ʾ�����ֻ���̬����
void User_Calories_Exit(void);//�˳������ֻ�����



//void User_Calories_Exit(void);//�˳���·�����
//void User_Calories_Show(void);//��ʾ��·�����
//void User_Calories_Into(void);//���뿨·�����


void User_Clock_Set_Into(void);//����ʱ������
void User_Clock_Set_Exit(void);//�˳�ʱ������
void User_Clock_Set_Exit_NotSave(void);//�˳�ʱ�����ã�������
void User_Clock_Set_Bit_Next(void);//ʱ������,��һλ
void User_Clock_Set_Bit_Before(void);//ʱ������,��һλ
void User_Clock_Set_Bit_Add(void);//ʱ������,λ��

void User_Rang_Set_Into(void);//������������
void User_Rang_Set_Exit(void);//�˳���������
void User_Rang_Set_Exit_NotSave(void);//�˳��������ã�������
void User_Rang_Set_Bit_Next(void);//��������,��һλ
void User_Rang_Set_Bit_Before(void);//��������,��һλ
void User_Rang_Set_Bit_Add(void);//��������,λ��



void User_Rang_Pause(void);//������ͣ

void User_Rang_Stop(void);//����ֹͣ






void User_Sleep_Test_Into(void);//����˯�߼�����
void User_Sleep_Test_Change(void);//˯�߼������л�
void User_Sleep_Test_Exit(void);//�˳�˯�߼�����
	










#endif
