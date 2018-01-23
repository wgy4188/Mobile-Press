
#include "dispose.h"

ComData g_ComData;
static uint8_t cmd_flag=0;

void CheckData(uint8_t byte)
{
		static uint8_t state=0;

		switch(state)
		{
			case 0:
			{
				 if(byte==0xFB)
				 {
					state=1;
				 }
				 else
				 {
					state=0;
				 }
			}
			break;

			case 1:
			{
				 g_ComData.data=byte;
				 state=2;
			}
			break;

			case 2:
			{
				 g_ComData.cmd=byte;
				 cmd_flag=1;
				 state=0;
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
		SerPrintf("\n");
		
		if(cmd_flag == 1)
		{	
			switch(g_ComData.cmd)
			{
				case 0xE7:
				{
				}
				break;
				
				case 0xE8:
				{
				}
				break;
				
				case 0xEA:
				{
				}
				break;
				
				case 0xEC:
				{
				}
				break;						
				
				case 0xED:
				{
				}
				break;						
				
				case 0xEE:
				{
				}
				break;
				
				case 0xEF:
				{
				}
				break;	
				
				default:
					break;	
			}

			cmd_flag=0;
		}
}






