#include "dma.h"
#include "sramlcd.h"
																	   	  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DMA ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

DMA_InitTypeDef DMA_InitStructure;

u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 		    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ�����  
void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
    DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���	  	
} 
//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{

	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA1_Channel4,DMA1_MEM_LEN);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}	  
////////////////////////////////////////////////////////////////////////////////////
//LCD DMA��ʼ������
u16 LCD_CUR_LINE;
u32 DMA2_MEM_LEN=480*320/4;//����DMAÿ�����ݴ��͵ĳ��� ����Ļ   	  
void LCD_DMA_Coinfig(void)
{												    

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//ʹ��DMA����
	
    DMA_DeInit(DMA2_Channel5);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)LCD_GRAM_BUF;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)0X6C000800;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = DMA2_MEM_LEN;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���	  
									 
}

//����һ��LCD DMA����
//x:��ʵ�����ַ���(0~240)
void LCD_DMA_Enable(u16 x)
{	
	//��ΪDMAÿ�δ�����������,�ʷ�4�δ���.
	//��һ�δ���												    	  
 	DMA_Cmd(DMA2_Channel5,DISABLE);	//�ر�DMA����
 	DMA_ClearFlag(DMA2_FLAG_TC5);	//����ϴεĴ�����ɱ�� 
	LCD_CUR_LINE=x;
	DMA2_Channel5->CNDTR=DMA2_MEM_LEN; 
	DMA2_Channel5->CPAR=(u32)LCD_GRAM_BUF[LCD_CUR_LINE]; //DMA2,�ı�洢����ַ
	DMA_Cmd(DMA2_Channel5, ENABLE);//����DMA����
    while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);	//�ȴ�
	
	
	//�ڶ��δ���
	DMA_ClearFlag(DMA2_FLAG_TC5);	//����ϴεĴ�����ɱ��
	DMA2_Channel5->CPAR=(u32)LCD_GRAM_BUF[LCD_CUR_LINE+80]; //DMA1,�ı�洢����ַ

	DMA_Cmd(DMA2_Channel5,DISABLE);	//�ر�DMA����
	DMA2_Channel5->CNDTR=DMA2_MEM_LEN; 
	DMA_Cmd(DMA2_Channel5, ENABLE);//����DMA����
	while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);	//�ȴ�

	
	//��3�δ���
	DMA_ClearFlag(DMA2_FLAG_TC5);	//����ϴεĴ�����ɱ��
	DMA2_Channel5->CPAR=(u32)LCD_GRAM_BUF[LCD_CUR_LINE+160]; //DMA1,�ı�洢����ַ

	DMA_Cmd(DMA2_Channel5,DISABLE);	//�ر�DMA����
	DMA2_Channel5->CNDTR=DMA2_MEM_LEN; 
	DMA_Cmd(DMA2_Channel5, ENABLE);//����DMA����
	while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);	//�ȴ�
	
	
	//��4�δ���
		DMA_ClearFlag(DMA2_FLAG_TC5);	//����ϴεĴ�����ɱ��
	DMA2_Channel5->CPAR=(u32)LCD_GRAM_BUF[LCD_CUR_LINE+240]; //DMA1,�ı�洢����ַ

	DMA_Cmd(DMA2_Channel5,DISABLE);	//�ر�DMA����
	DMA2_Channel5->CNDTR=DMA2_MEM_LEN; 
	DMA_Cmd(DMA2_Channel5, ENABLE);//����DMA����
	while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);	//�ȴ�

}
 

























