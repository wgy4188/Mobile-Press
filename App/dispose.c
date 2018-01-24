
#include "dispose.h"

static ComData g_ComData;
static uint8_t cmd_flag=0;
static uint8_t state=0;

void CheckData(uint8_t byte)
{
		switch(state)
		{
			case 0:
			{
				 if(byte==0x65)
				 {
						state=1;
				 }
				 else if(byte == 0x71)
				 {
						state = 4;				  
				 }
				 else
				 {
						state = 7;
				 }
			}
			break;

			case 1:
			{
				 g_ComData.obj = ((uint16_t)byte<<8)&0xff00;//page
				 state=2;
			}
			break;

			case 2:
			{
				 g_ComData.obj |= (uint16_t)byte&0x00ff;//id
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
				byte=0xff;
				state = 5;
			}
			break;
			
			case 5:
			{
				byte=0xff;
				state = 6;
			}
			break;
			
			case 6:
			{
				byte=0xff;
				state = 0;
				
				cmd_flag = 1;
			}
			break;
			
			case 7:
			{
				g_ComData.data = (uint32_t)byte & 0x000000ff;
				state = 8;
			}
			break;
			
			case 8:
			{
				g_ComData.data |= (((uint32_t)byte)<<8) & 0x0000ff00;
				state = 9;
			}
			break;
			
			case 9:
			{
				g_ComData.data |= (((uint32_t)byte)<<16) & 0x00ff0000;
				state =10;
			}
			break;
			
			case 10:
			{
				g_ComData.data |= (((uint32_t)byte)<<24) & 0xff000000;
				g_ComData.obj = 0x0002;
				state = 4;
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
				SerPrintf("%x ",tmp[i]);
				CheckData(tmp[i]);		
		}
		state = 0;
		SerPrintf("\n");
		
		if(cmd_flag == 1)
		{	
			switch(g_ComData.obj)
			{
				case b_RESET:
				{
					if(g_ComData.event==0x00)
					{
							SerPrintf("Reset\n");
					}
				}
				break;
				
				case b_SAVE:
				{
						SerPrintf("Save data:%l\n",g_ComData.data);
				}
				break;
				
				default:
					break;	
			}

			cmd_flag=0;
		}
}






