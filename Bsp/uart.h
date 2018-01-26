
#ifndef __UART
#define __UART

#include  "stdio.h"
#include  "stdarg.h"
#include  "stm32f10x_usart.h"

#define FIFO_SIZE 256

extern volatile int8_t  FifoHead;
extern volatile int8_t  FifoTail;
extern volatile uint8_t FifoBuff[FIFO_SIZE];

extern uint16_t receCount;
extern uint8_t receBuf[0xff];
extern uint8_t sendBuf[0xff];

void USART1_Configuration(void);
void USART3_Configuration(void);
void USART4_Configuration(void);

void USART1_SendData(uint16_t ch);
void USART1_SendStringData(uint16_t *st);
void USART1_Printf(const  char *format, ...);

void USART3_SendData(uint8_t ch);
void USART3_SendStringData(uint16_t *st);

void USART4_SendData(uint8_t ch);
void USART4_SendStringData(uint16_t *st);

void SerPrintf(const  char *format, ...);

int32_t FifoWriteOneByte(uint8_t InputChar);
int32_t InquireUartRecvBuffer(uint8_t *pBuf, int8_t MaxBuffCount);

#endif

