#ifndef _DISPOSE_H_
#define _DISPOSE_H_

#include <stdint.h>
#include "uart.h"

extern uint8_t disposeFlag;

enum ControlSort
{
		b_RESET = 0x0001,
		b_SAVE = 0x0002
};

typedef struct
{
		uint16_t obj;
		uint8_t  event;
		uint32_t data;//4byte data
}ComData;



void Dispose(void);

#endif	

