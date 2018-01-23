
#include "adc.h"

void  ADC_Config(void) 
{ 	
	ADC_InitTypeDef 	ADC_InitStructure; 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* ADC clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                       //设置ADC分频因子6 72M/6=12, ADC最大时间不能超过14M	
	
	//PC0 作为模拟通道输入引脚                         	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	    //ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	          //模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	    //模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	 //转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	    //顺序进行规则转换的ADC通道的数目
	
	ADC_Init(ADC1, &ADC_InitStructure);	        //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	ADC_Cmd(ADC1, ENABLE);	                    //使能指定的ADC1
	ADC_ResetCalibration(ADC1);	                //使能复位校准  
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	ADC_StartCalibration(ADC1);	                //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	    //等待校准结束   校准ADC对于AD精度可以有部分提高
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		  //使能指定的ADC1的软件转换启动功能

}				  

u16 Get_Adc(u8 ch)   
{
		if(ch==1)
		{	
			ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );  //设置ADCx，通道，通道中第几个开始转换，采样周期
		}
		if(ch==2)
		{
			ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5 );
		}
		
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);					//使能指定的ADC1的软件转换启动功能	，并且只能转换一个通道
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//等待转换结束   转换结束后，ADC_DR里面已经存储了相对应的数据了

		return ADC_GetConversionValue(ADC1);						//返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 value =0;
	u8 t;
	
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
	}
	value =temp_val/times;
	
	return value * 2;	  
} 	 


