
#include "action.h"

uint32_t setTime=0;
uint8_t ResetFlag=0;
uint8_t timeFlag=0;
uint8_t pausFlag=0;

void ActionInit()
{
		BEEP_OFF;
		VALVE_OFF;
		LED_OFF;
		COUNTER_OFF;
		OUT5=1;
		OUT6=1;
		StopTime();
		
		RecvTime=IIC_Read_4Byte(0x0001);
		USART1_Printf("page 1");
	  delay_Nms(200);
		USART1_Printf("page 0");
		ComStop();
	  delay_Nms(200);
		USART1_Printf("n_time.val=%d",RecvTime);
		ComStop();
		USART1_Printf("n_time.val=%d",RecvTime);
		ComStop();
	
		setTime = RecvTime;
}

void NormalWork()
{
		if((DUAL_BUTTON == 0)&&(GRID == 0)&&(EM_BUTTON == 0))
		{
				LED_ON;
				VALVE_ON;
		}
		
		if(setTime==0)
		{
				StopTime();
				LED_OFF;
				VALVE_OFF;
				COUNTER_ON;
				delay_Nms(10);
				COUNTER_OFF;
				timeFlag=0;
				pausFlag=0;
		}
		
		if((DOWN==0)&&(timeFlag==0))
		{
					setTime = RecvTime;
					StartTime();
					timeFlag=1;
		}
		
		if(timeFlag)
		{
				//ÔÝÍ£
				if(DOWN)
				{
						StopTime();
						pausFlag=1;
				}
				else
				{
						if(pausFlag)
						{
								StartTime();
								pausFlag=0;
						}
				}
		}
		
		return;
}

void EmergencyDispose()
{
		static uint8_t EM_Flag;
	
		if(EM_BUTTON)
		{
				StopTime();
				BEEP_ON;
				EM_Flag = 1;
		}
		if((!EM_BUTTON)&&(EM_Flag))
		{
				/*release dispose*/
				VALVE_OFF;
				BEEP_OFF;
				StopTime();
				setTime = 0;
			  ResetFlag=0;
				timeFlag=0;
				pausFlag=0;
		}
		
		return;
}

void GridDispose()
{
		if(GRID)
		{
				StopTime();
				BEEP_ON;
		}
}


void ResetDispose()
{
		if((RST_BUTTON == 0)||( ResetFlag==1 ))
		{
				VALVE_OFF;
				BEEP_OFF;
				StopTime();
				setTime = 0;
			  ResetFlag=0;
				timeFlag=0;
				pausFlag=0;
			
				USART1_Printf("n_time.val=%d",RecvTime);
				ComStop();
		}
	
		return;
}

void KeyScan()
{
		return;
}


