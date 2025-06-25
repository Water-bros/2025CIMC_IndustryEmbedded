#ifndef __DRIVER_ADC_H
#define __DRIVER_ADC_H

#include "main.h"
#include "gd32f4xx.h"

#define ADC_PORT GPIOC
#define ADC_PIN GPIO_PIN_0

void ADC_Init(void);

#endif //__DRIVER_ADC_H
