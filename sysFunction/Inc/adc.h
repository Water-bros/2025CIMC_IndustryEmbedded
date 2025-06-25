#ifndef __ADC_H
#define __ADC_H

#include "main.h"
#include "gd32f4xx.h"
#include "driver_adc.h"

uint16_t Get_ADC_Value(void);

void ADC_Sample(app_t *app);

#endif //__ADC_H
