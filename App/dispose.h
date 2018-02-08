#ifndef _DISPOSE_H_
#define _DISPOSE_H_

#include <stdint.h>
#include "uart.h"
#include "iic.h"
#include "action.h"

extern uint8_t disposeFlag;

enum ControlSort
{
		b_Freq = 0x0001,
		b_Duty = 0x0002
};

typedef struct
{
		uint16_t obj;
		uint8_t  event;
		uint32_t data;
}ComData;


extern uint32_t RecvTime;
extern uint8_t ResetFlag;
extern uint8_t SaveData[4];

void Dispose(void);
void ComStop(void);

#endif	

