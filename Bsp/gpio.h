#ifndef __GPIO
#define __GPIO

#include  "stm32f10x_gpio.h"
#include  "sys.h"

#define  IN1      	    	  PEin(6)
#define  IN2           	    PEin(5)
#define  IN3                PEin(4)	
#define  IN4                PEin(3)	             
#define  IN5      				  PEin(2)	
#define  IN6     			      PEin(1) 
#define  IN7                PEin(0) 
#define  IN8                PBin(9)
#define  IN9                PBin(5)
#define  IN10               PDin(5)
#define  IN11               PDin(4)
#define  IN12               PDin(3)
#define  IN13							  PDin(2)
#define  IN14               PDin(1)
#define  IN15               PDin(0)
#define  IN16               PCin(12)

#define  OUT1               PCout(4)
#define  OUT2               PCout(5)
#define  OUT3               PBout(0)
#define  OUT4               PBout(1)
#define  OUT5               PEout(7)
#define  OUT6               PEout(8)
#define  OUT7               PEout(9)
#define  OUT8               PEout(10)
#define  OUT9               PEout(11)
#define  OUT10              PEout(12)
#define  OUT11              PEout(13)
#define  OUT12              PEout(14)

void GpioCfg(void);
	
#endif

