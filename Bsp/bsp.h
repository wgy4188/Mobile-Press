
#ifndef  BSP_PRESENT
#define  BSP_PRESENT

#include  <stdint.h>
#include  "stm32f10x_rcc.h"
#include  "os_cfg.h"
#include  "os_cpu.h"
#include  "cpu.h"

void RCC_Configuration(void);
void SysTick_Init(void);
void  BSP_Init (void);

#endif
