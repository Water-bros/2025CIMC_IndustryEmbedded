#ifndef __RTC_H
#define __RTC_H

#include "main.h"
#include "gd32f4xx.h"

void RTC_Config(void);

void RTC_Set_Time(uint8_t year, uint8_t month, uint8_t date,
                  uint8_t hour, uint8_t min, uint8_t sec);

void RTC_Set_Time_Struct(time_tt *time);

void RTC_Get_Time(time_tt *time);

#endif //__RTC_H


