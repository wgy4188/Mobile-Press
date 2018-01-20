
#include "uart.h"

uint8_t	receCount=0;
uint8_t sendBuf[0xff]={0},receBuf[0xff]={0};

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
		USART_Cmd(USART1, ENABLE);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

		// NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
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
		
		//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void USART1_SendData(uint16_t ch)
{
		USART_SendData(USART1, ch); 
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_ClearFlag(USART1,USART_FLAG_TC); 	
}

void USART1_SendStringData(uint16_t *st)
{
		while(*st!='\0')
		{
			USART1_SendData(*st);
			st++;
		}
}

void USART4_SendData(uint8_t ch)
{
		USART_SendData(UART4, ch); 
		while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
		USART_ClearFlag(UART4,USART_FLAG_TC); 	
}


void USART4_SendStringData(uint16_t *st)
{
		while(*st!='\0')
		{
			USART1_SendData(*st);
			st++;
		}
}

static void  SerWrStr (const char * tx_str)
{
    while ((*tx_str) != 0)
		{
		if( (*tx_str) == '\n' )
		{
				USART4_SendData('\r');
				USART4_SendData('\n');
		}
		else
		{
				USART4_SendData(*tx_str);
		}
		
				tx_str++;
    }
}

void  SerPrintf(const  char *format, ...)
{
		static	char buffer[255];
		va_list	vArgs;

		va_start(vArgs, format);
		vsprintf((char *)buffer, (char const *)format, vArgs);
		va_end(vArgs);

		SerWrStr((char*) buffer);
}





