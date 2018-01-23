
#include "adc.h"

void  ADC_Config(void) 
{ 	
	ADC_InitTypeDef 	ADC_InitStructure; 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* ADC clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                       //����ADC��Ƶ����6 72M/6=12, ADC���ʱ�䲻�ܳ���14M	
	
	//PC0 ��Ϊģ��ͨ����������                         	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	    //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	          //ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	    //ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	 //ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	    //˳����й���ת����ADCͨ������Ŀ
	
	ADC_Init(ADC1, &ADC_InitStructure);	        //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	ADC_Cmd(ADC1, ENABLE);	                    //ʹ��ָ����ADC1
	ADC_ResetCalibration(ADC1);	                //ʹ�ܸ�λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	                //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	    //�ȴ�У׼����   У׼ADC����AD���ȿ����в������
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		  //ʹ��ָ����ADC1�����ת����������

}				  

u16 Get_Adc(u8 ch)   
{
		if(ch==1)
		{	
			ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );  //����ADCx��ͨ����ͨ���еڼ�����ʼת������������
		}
		if(ch==2)
		{
			ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5 );
		}
		
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);					//ʹ��ָ����ADC1�����ת����������	������ֻ��ת��һ��ͨ��
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//�ȴ�ת������   ת��������ADC_DR�����Ѿ��洢�����Ӧ��������

		return ADC_GetConversionValue(ADC1);						//�������һ��ADC1�������ת�����
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


