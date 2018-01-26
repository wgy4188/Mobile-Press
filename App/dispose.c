
#include "dispose.h"

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
				 if(byte==0x65)
				 {
						state=1;
				 }
				 else if(byte == 0x71)
				 {
						state = 7;				  
				 }
				 else
				 {
						state = 0;
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

void ComStop()
{
		USART1_SendData(0xff);
		USART1_SendData(0xff);
		USART1_SendData(0xff);
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
		}
		state = 0;
		
		if(cmd_flag == 1)
		{	
			switch(g_ComData.obj)
			{
				case b_RESET:
				{
						if(g_ComData.event==0x00)//弹起事件
						{
								SerPrintf("Reset\n");		
								ResetFlag = 1;
						}
				}
				break;
				
				case b_SAVE:
				{						
						RecvTime = g_ComData.data;
						IIC_Write_4Byte(0x0001, RecvTime);
						SerPrintf("Save data:%ld\n",RecvTime);
				}
				break;
				
				default:
					break;	
			}

			cmd_flag=0;
		}
}






