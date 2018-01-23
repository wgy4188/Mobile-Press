#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"

void  ADC_Config(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);
	
#endif

