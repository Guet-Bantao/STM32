#include "dma.h"
#include "sys.h"
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
//u8 GBK_BUF_Flag =10;
DMA_InitTypeDef DMA_InitStructure;


u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 	    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����

	DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//ע�⣺��������Ϊ�����Ǵ����赽�ڴ�.//DMA_DIR_PeripheralDST���ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//ע�⣺��������Ϊѭ��ģʽDMA_Mode_Circular����Ȼ���������ڶ��δ���//DMA_Mode_Normal����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ�	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	
	// NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;//DMA1_Channel5_IRQn��ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); //ʹ�ܴ���1��DMA����  
	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 RX DMA1 ��ָʾ��ͨ�� ��ʼһ��DMA���䣡
	//DMA_ITConfig(DMA_CHx,DMA_IT_TC,ENABLE);	//ʹ��DMA��������ж�
} 
/*����һ��DMA����
*��ͨ������Ϊ��ѭ��ģʽ����������󽫲��ڲ���DMA������
*Ҫ��ʼ�µ�DMA���䣬��Ҫ�ڹر�DMAͨ��������£���DMA����CNDTRx�Ĵ���д�봫����Ŀ*/
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}

void DMA1_Channel5_IRQHandler(void) 
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5)==SET) //�ж�ͨ��4�������
	{
    DMA_ClearFlag(DMA1_FLAG_TC5);//���ͨ��������ɱ�־	  
		
		//**********************����֡����******************//
//			GBK_BUF_Flag=GBK_BUF_Flag+1;
		//**************************************************//
//		pro=DMA_GetCurrDataCounter(DMA1_Channel4);//�õ���ǰ��ʣ����ٸ�����
//		pro=1-pro/SEND_BUF_SIZE;//�õ��ٷֱ�	
//		pro*=100;      //����100�� 		 
  } 
}	

//void USART1_DMA_Config(void)
//{
//		DMA_InitTypeDef DMA_InitStructure;
//	
//		/*����DMAʱ��*/
//		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
//		//NVIC_Config();	   			//����DMA�ж�

//		/*����DMAԴ���������ݼĴ�����ַ*/
//		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   

//		/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
//		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)& SendBuff;	//�ڴ��ַ;

//		/*���򣺴��ڴ浽����*/		
//		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	

//		/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
//		DMA_InitStructure.DMA_BufferSize = SIZE;

//		/*�����ַ����*/	    
//		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

//		/*�ڴ��ַ����*/
//		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

//		/*�������ݵ�λ*/	
//		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

//		/*�ڴ����ݵ�λ 8bit*/
//		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

//		/*DMAģʽ������ѭ��*/
//		//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
//		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 

//		/*���ȼ�����*/	
//		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

//		/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
//		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

//		/*����DMA1��4ͨ��*/		   
//		DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
//		
//		/*ʹ��DMA*/
//		DMA_Cmd (DMA1_Channel4,ENABLE);					
//		//DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�
//}

