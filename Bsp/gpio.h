#ifndef __GPIO
#define __GPIO

#include  "stm32f10x_gpio.h"
#include  "sys.h"

#define  OUT1               PCout(4)	                              //OUT1
#define  OUT2               PCout(5)	                              //OUT2
#define  OUT3               PBout(0)	                              //OUT3
#define  OUT4               PBout(1)	                              //OUT4
#define  OUT5               PEout(7)	                              //OUT5
#define  OUT6               PEout(8)                	              //OUT6
#define  OUT7               PEout(9)	                              //OUT7
#define  OUT8               PEout(10)               	              //OUT8
#define  OUT9               PEout(11)	                              //OUT9
#define  OUT10              PEout(12)	                              //OUT10
#define  OUT11              PEout(13)	                              //OUT11
#define  OUT12              PEout(14)                	              //OUT12

void GpioCfg(void);
	
#endif

