#include "dma.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DMA 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/8
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//u8 GBK_BUF_Flag =10;
DMA_InitTypeDef DMA_InitStructure;


u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 	    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输

	DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//注意：这里设置为数据是从外设到内存.//DMA_DIR_PeripheralDST从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//注意：这里设置为循环模式DMA_Mode_Circular，不然不能启动第二次传输//DMA_Mode_Normal工作在正常模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	// NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;//DMA1_Channel5_IRQn断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); //使能串口1的DMA接收  
	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 RX DMA1 所指示的通道 开始一次DMA传输！
	//DMA_ITConfig(DMA_CHx,DMA_IT_TC,ENABLE);	//使能DMA传输完成中断
} 
/*开启一次DMA传输
*当通道配置为非循环模式，传输结束后将不在产生DMA操作。
*要开始新的DMA传输，需要在关闭DMA通道的情况下，在DMA——CNDTRx寄存器写入传输数目*/
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
	DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1 所指示的通道      
 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}

void DMA1_Channel5_IRQHandler(void) 
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5)==SET) //判断通道4传输完成
	{
    DMA_ClearFlag(DMA1_FLAG_TC5);//清除通道传输完成标志	  
		
		//**********************数据帧处理******************//
//			GBK_BUF_Flag=GBK_BUF_Flag+1;
		//**************************************************//
//		pro=DMA_GetCurrDataCounter(DMA1_Channel4);//得到当前还剩余多少个数据
//		pro=1-pro/SEND_BUF_SIZE;//得到百分比	
//		pro*=100;      //扩大100倍 		 
  } 
}	

//void USART1_DMA_Config(void)
//{
//		DMA_InitTypeDef DMA_InitStructure;
//	
//		/*开启DMA时钟*/
//		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
//		//NVIC_Config();	   			//配置DMA中断

//		/*设置DMA源：串口数据寄存器地址*/
//		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   

//		/*内存地址(要传输的变量的指针)*/
//		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)& SendBuff;	//内存地址;

//		/*方向：从内存到外设*/		
//		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	

//		/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
//		DMA_InitStructure.DMA_BufferSize = SIZE;

//		/*外设地址不增*/	    
//		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

//		/*内存地址自增*/
//		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

//		/*外设数据单位*/	
//		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

//		/*内存数据单位 8bit*/
//		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

//		/*DMA模式：不断循环*/
//		//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
//		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 

//		/*优先级：中*/	
//		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

//		/*禁止内存到内存的传输	*/
//		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

//		/*配置DMA1的4通道*/		   
//		DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
//		
//		/*使能DMA*/
//		DMA_Cmd (DMA1_Channel4,ENABLE);					
//		//DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断
//}

