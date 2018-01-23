
#include "pwm.h"

void PWM_Config(unsigned int DivFreq, unsigned int CouVal, unsigned int DutyCycle)  //分频、周期，占空比
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	GPIO_InitTypeDef         GPIO_InitStructure;	  
	
		/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ; //配置定时器输出引脚PB8->tim4 ch3				  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_DeInit(TIM4);	      
	TIM_TimeBaseStructure.TIM_Prescaler = DivFreq;	//分频值决定计数时钟频率	            
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_Period = CouVal;      //周期值决定输出脉冲频率         	          
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;				
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);  
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 			     
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	
	TIM_OCInitStructure.TIM_Pulse = DutyCycle;     //决定脉冲波的占空比 					           
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	    
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OC3Init(TIM4,&TIM_OCInitStructure); 
	
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);	

	TIM_Cmd(TIM4,ENABLE);
	TIM_CtrlPWMOutputs(TIM4, ENABLE); 
}
