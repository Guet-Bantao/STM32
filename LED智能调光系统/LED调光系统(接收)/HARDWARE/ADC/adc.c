 
#include "delay.h"
#include "sys.h"

//初始化ADC1
//这里我们仅以规则通道为例
//我们默认仅开启通道1																	   
void  Adc1_Init(void)
{      
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC3通道时钟
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC复位
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//复位结束	    
	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1的全部寄存器重设为缺省值
	
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式: 独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器  
	
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}		 
//获得ADC1某个通道的值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc1(u8 ch)   
{
  //设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC3,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC3的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC3规则组的转换结果
} 
