
#include "bsp.h"
#include "gpio.h"
#include "uart.h"
#include "tim.h"
#include "iic.h"

void  BSP_Init (void)
{
		RCC_Configuration();
		SysTick_Init();
		GpioCfg();
		TIM_Config();
		USART1_Configuration();
		USART3_Configuration();
		//USART4_Configuration();
		IIC_Configuration();
	
		return;
}

void RCC_Configuration(void)
{
		SystemInit();
	
		return;
}

void SysTick_Init(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    uint32_t cnts;

    RCC_GetClocksFreq(&rcc_clocks);
    cnts = (uint32_t)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC; //1000times persecond, 1ms to int
		SysTick_Config(cnts);
	
		return;
}


/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;
	
    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

/*
*********************************************************************************************************
*                                         OS_CPU_SysTickClkFreq()
*
* Description : Get system tick clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : Clock frequency (of system tick).
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;

    freq = BSP_CPU_ClkFreq();
    return (freq);
}


