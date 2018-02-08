
#include "dispose.h"
#include "pwm.h"

static ComData g_ComData;
static uint8_t cmd_flag=0;//执行通信命令标志
static uint8_t state=0;//数据校验状态
uint32_t RecvTime=0;//更新时间

void CheckData(uint8_t byte)
{
		switch(state)
		{
			case 0:
			{
				 if(byte==0xAA)
				 {
						state=1;
				 }
			}
			break;

			case 1:
			{
				 g_ComData.obj = ((uint16_t)byte<<8)&0xff00;
				 state=2;
			}
			break;

			case 2:
			{
				 g_ComData.obj |= (uint16_t)byte&0x00ff;
				 state=3;
			}
			break;
			
			case 3:
			{
				g_ComData.event=byte;
				state = 4;
			}
			break;
			
			case 4:
			{
				byte=0x55;
				cmd_flag = 1; //ready to dispose
				state = 0;
			}
			break;
			
			default:
				break;
		}

		return;
}


void Dispose(void)
{
		uint8_t tmp[10]={0};
		uint8_t count;
		uint8_t i;

		count=InquireUartRecvBuffer(tmp,10);
		if(count==0)
		{
				return;
		}

		for(i=0;i<count;i++)
		{	
				CheckData(tmp[i]);		
				USART1_Printf("%x",tmp[i]);
		}
		USART1_Printf("\n");
		
		if(cmd_flag == 1)
		{	
			switch(g_ComData.event)
			{
				case b_Freq:
				{
						StartPWM();
						USART1_Printf("Start PWM\n");
				}
				break;
				
				case b_Duty:
				{						
						StopPWM();
						USART1_Printf("Stop PWM\n");
				}
				break;
				
				default:
					break;	
			}

			cmd_flag=0;
		}
}






