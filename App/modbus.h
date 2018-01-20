#ifndef __MODBUS
#define __MODBUS

#include <stdint.h>
#include "uart.h"

extern volatile uint8_t receTimeOut;
extern volatile uint8_t checkoutError;

typedef  struct 
{
	
		unsigned int   Add;
		unsigned int   num;

} Coil_Dis;

typedef  struct 
{
	
		unsigned int   Add;
		unsigned int   num;
	
} Regis_Dis;

void ReceiveOver(void);

#endif

