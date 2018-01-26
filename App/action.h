#ifndef __ACTION
#define __ACTION

#include <stdint.h>
#include "gpio.h"
#include "tim.h"
#include "dispose.h"
#include "iic.h"

#define DUAL_BUTTON  IN3
#define EM_BUTTON    IN1
#define RST_BUTTON   IN2
#define DOWN         IN5
#define GRID         IN9

#define BEEP_ON      OUT1=0
#define BEEP_OFF     OUT1=1
#define LED_ON       OUT2=0
#define LED_OFF      OUT2=1
#define COUNTER_ON   OUT3=0
#define COUNTER_OFF  OUT3=1
#define VALVE_ON     OUT4=0
#define VALVE_OFF    OUT4=1

extern uint32_t setTime;

void NormalWork(void);
void EmergencyDispose(void);
void ResetDispose(void);
void KeyScan(void);
void ActionInit(void);
void GridDispose(void);

#endif
