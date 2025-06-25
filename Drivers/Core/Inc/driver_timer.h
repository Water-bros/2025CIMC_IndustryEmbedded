#ifndef __DRIVER_TIMER_H
#define __DRIVER_TIMER_H

#include "main.h"
#include "gd32f4xx.h"

void Timer_Init(void);

void Timer_Config(void);

void Timer_On(void);

void Timer_Off(void);

#endif //__DRIVER_TIMER_H