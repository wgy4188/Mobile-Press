#include "tim.h"

void TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef         NVIC_InitStructure;
	uint16_t PrescalerValue = 0;
	
	/* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t)(SystemCoreClock / 12000) - 1;
	
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = (12000-1);
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE); 
  TIM_Cmd(TIM2, DISABLE);
	
	return;
}

void StartTime(void)
{
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 
	TIM_Cmd(TIM2 , ENABLE);
	
	return;
}

void StopTime(void)
{
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE); 
	TIM_Cmd(TIM2, DISABLE);
	
	return;	
}

