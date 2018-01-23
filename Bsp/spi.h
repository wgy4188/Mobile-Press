#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

void SPI_Config(void);
uint8_t SPI_ReadWrite(uint8_t writedat);	

#endif

