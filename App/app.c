
#include "app_cfg.h"
#include "bsp.h"
#include "uart.h"
#include "dispose.h"
#include "action.h"

__align(8) static OS_STK SignalTaskStk[SIGNAL_TASK_STACK_SIZE];
__align(8) static OS_STK ModbusTaskStk[MODBUS_TASK_STACK_SIZE];	
__align(8) static OS_STK MainTaskStk[MAIN_TASK_STACK_SIZE];

static void ModbusTask(void *p_arg);
static void SignalTask(void *p_arg);
static void MainTask(void *p_arg);

OS_EVENT *RecF;/*recv data event*/

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/
int  main (void)
{
		INT8U  err;
	
    CPU_IntDis();
		OSInit();   
		BSP_Init();	
	
#if (OS_TASK_STAT_EN > 0)
		OSStatInit();
#endif
	
		RecF = OSSemCreate(0);/*event*/
	
		/*creat signal task*/
		OSTaskCreateExt(SignalTask,	
										(void *)0,
										(OS_STK *)&SignalTaskStk[SIGNAL_TASK_STACK_SIZE - 1],
										SIGNAL_TASK_PRIO,	
										SIGNAL_TASK_PRIO,
										(OS_STK *)&SignalTaskStk[0],
										SIGNAL_TASK_STACK_SIZE, 
										(void *)0,
										OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
		 
		OSTaskNameSet(SIGNAL_TASK_PRIO, (INT8U*)"SignalTask", &err);
		/*creat modbus task*/
		OSTaskCreateExt(ModbusTask,	
										(void *)0,
										(OS_STK *)&ModbusTaskStk[MODBUS_TASK_STACK_SIZE - 1],
										MODBUS_TASK_PRIO,	
										MODBUS_TASK_PRIO,
										(OS_STK *)&ModbusTaskStk[0],
										MODBUS_TASK_STACK_SIZE, 
										(void *)0,
										OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
		 
		OSTaskNameSet(MODBUS_TASK_PRIO, (INT8U*)"ModbusTask", &err);								
		/*creat main task*/
		OSTaskCreateExt(MainTask,	
										(void *)0,
										(OS_STK *)&MainTaskStk[MAIN_TASK_STACK_SIZE - 1],
										MAIN_TASK_PRIO,	
										MAIN_TASK_PRIO,
										(OS_STK *)&MainTaskStk[0],
										MAIN_TASK_STACK_SIZE, 
										(void *)0,
										OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
										
		OSTaskNameSet(MAIN_TASK_PRIO, (INT8U*)"MainTask", &err);
										
		OSTimeSet(0);	
		OSStart();
	
    return (0);
}

static void ModbusTask(void *p_arg)
{  
		INT8U   err;
		(void)p_arg;
			
		for(;;)
		{
				OSSemPend(RecF,0, &err);
				if(err == OS_ERR_NONE)
				{
						
				}					
				OSTimeDlyHMSM(0, 0, 0, 10);		 
		}
}

static void SignalTask(void *p_arg)
{	
	 (void)p_arg;
	
	 USART1_Printf("Into signal task\n");
	 for(;;)
	 {	  	  		
				if(disposeFlag)
				{	
					Dispose();
					disposeFlag=0;
				}
				OSTimeDlyHMSM(0, 0, 0, 10); 
	 }
}

static void MainTask(void *p_arg)
{	
	 (void)p_arg;

	 for(;;)
	 {	  	  		 
				OSTimeDlyHMSM(0, 0, 0, 10); 
	 }
}

