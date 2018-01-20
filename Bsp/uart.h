
#ifndef __UART
#define __UART

#include  "stdio.h"
#include  "stdarg.h"
#include  "stm32f10x_usart.h"

extern uint8_t	receCount;
extern uint8_t sendBuf[0xff],receBuf[0xff];

void USART1_Configuration(void);
void USART4_Configuration(void);
void USART1_SendData(uint16_t ch);
void USART1_SendStringData(uint16_t *st);
void USART4_SendData(uint8_t ch);
void USART4_SendStringData(uint16_t *st);
void SerPrintf(const  char *format, ...);

#endif

