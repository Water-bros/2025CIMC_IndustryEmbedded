#ifndef __TIMER_H
#define __TIMER_H

#include "main.h"
#include "gd32f4xx.h"
#include "driver_timer.h"

void Sample_Timer_Handler(app_t *app);

void LED_Timer_Handler(app_t *app);

#endif //__TIMER_H