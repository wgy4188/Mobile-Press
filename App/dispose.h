#ifndef _DISPOSE_H_
#define _DISPOSE_H_

#include <stdint.h>
#include "uart.h"

extern uint8_t disposeFlag;

typedef struct
{
	
		uint8_t cmd;
		uint8_t data;
	
}ComData;

void Dispose(void);

#endif	

