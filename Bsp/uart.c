
#include "uart.h"

volatile int8_t  FifoHead = 0;
volatile int8_t  FifoTail = 0;
volatile uint8_t FifoBuff[FIFO_SIZE] = {0};
uint16_t receCount = 0;
uint8_t receBuf[0xff] = {0};
uint8_t sendBuf[0xff] = {0};

void USART1_Configuration(void)
{
		GPIO_InitTypeDef   GPIO_InitStructure;
		USART_InitTypeDef  USART_InitStructure;
		NVIC_InitTypeDef   NVIC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
				
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure);
		
		USART_ClearFlag(USART1,USART_FLAG_TC);
		USART_Cmd(USART1, ENABLE);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void USART3_Configuration(void)
{
		GPIO_InitTypeDef   GPIO_InitStructure;
		USART_InitTypeDef  USART_InitStructure;
		NVIC_InitTypeDef   NVIC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
		GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//PD8.TX3
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
			
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PD9.RX3
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		USART_Cmd(USART3, DISABLE);
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART3, &USART_InitStructure);
		
		USART_ClearFlag(USART3,USART_FLAG_TC);
		USART_Cmd(USART3, ENABLE);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
}

void USART4_Configuration(void)
{
		GPIO_InitTypeDef   GPIO_InitStructure;
		USART_InitTypeDef  USART_InitStructure;
		NVIC_InitTypeDef   NVIC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
			
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		USART_Cmd(UART4, DISABLE);
		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(UART4, &USART_InitStructure);
		USART_Cmd(UART4, ENABLE);
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
		
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*USART1 Fun*/
void USART1_SendData(uint16_t ch)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1, ch); 	
}

void  USART1_SerWrStr (const char * tx_str)
{
    while ((*tx_str) != 0)
		{
				if( (*tx_str) == '\n' )
				{
						USART1_SendData('\r');
						USART1_SendData('\n');
				}
				else
				{
						USART1_SendData(*tx_str);
				}

				tx_str++;
    }
}

void USART1_Printf(const  char *format, ...)
{
		static	char buf[255];
		va_list	vArgs;

		va_start(vArgs, format);
		vsprintf((char *)buf, (char const *)format, vArgs);
		va_end(vArgs);
	
		USART1_SerWrStr((char *)buf);

		return;
}

/*USART3 Fun*/
void USART3_SendData(uint8_t ch)
{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET){}
		USART_SendData(USART3, ch); 
	
		return;
}


void USART3_SerWrStr(const char * tx_str)
{
    while ((*tx_str) != 0)
		{
				if( (*tx_str) == '\n' )
				{
						USART3_SendData('\r');
						USART3_SendData('\n');
				}
				else
				{
						USART3_SendData(*tx_str);
				}

				tx_str++;
    }
		
		return;
}

void  USART3_Printf(const  char *format, ...)
{
		static	char buffer[255];
		va_list	vArgs;

		va_start(vArgs, format);
		vsprintf((char *)buffer, (char const *)format, vArgs);
		va_end(vArgs);

		USART3_SerWrStr((char*) buffer);
	
		return;
}


/*USART4 Fun*/
void UART4_SendData(uint8_t ch)
{
		while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET){}
	  USART_SendData(UART4, ch); 
	
		return;
}


static void  UART4_SerWrStr (const char * tx_str)
{
    while ((*tx_str) != 0)
		{
		if( (*tx_str) == '\n' )
		{
				UART4_SendData('\r');
				UART4_SendData('\n');
		}
		else
		{
				UART4_SendData(*tx_str);
		}
		
				tx_str++;
    }
		
		return;
}

void  UART4_Printf(const  char *format, ...)
{
		static	char buffer[255];
		va_list	vArgs;

		va_start(vArgs, format);
		vsprintf((char *)buffer, (char const *)format, vArgs);
		va_end(vArgs);

		UART4_SerWrStr((char*) buffer);
	
		return;
}


int32_t FifoWriteOneByte(uint8_t InputChar)
{
		if((FifoHead + 1)%FIFO_SIZE == FifoTail)
		{
				return -1;
		}
		
		FifoBuff[FifoHead] = InputChar;
		FifoHead = (FifoHead + 1)%FIFO_SIZE;
		
		return 0;
}

int32_t InquireUartRecvBuffer(uint8_t *pBuf, int8_t MaxBuffCount)
{
		int8_t i = 0;
		int8_t RcvCount = 0;

		while((FifoHead != FifoTail) && (MaxBuffCount > 0))
		{
				pBuf[i++] = FifoBuff[FifoTail];
				FifoBuff[FifoTail] = 0;
				FifoTail = (FifoTail + 1)%FIFO_SIZE;
				MaxBuffCount--;
				RcvCount++;
		}
		
		return RcvCount;
}



