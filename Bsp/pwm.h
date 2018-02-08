#ifndef _PWM_H_
#define _PWM_H_

#include "stm32f10x_tim.h"

void PWM_Config(unsigned int DivFreq, unsigned int CouVal, unsigned int DutyCycle);
void StartPWM(void);
void StopPWM(void);

#endif

