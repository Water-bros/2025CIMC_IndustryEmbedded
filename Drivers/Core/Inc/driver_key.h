#ifndef __DRIVER_KEY_H
#define __DRIVER_KEY_H

#include "main.h"
#include "gd32f4xx.h"

#define KEY_PORT GPIOD
#define KEY1_PIN GPIO_PIN_13
#define KEY2_PIN GPIO_PIN_12
#define KEY3_PIN GPIO_PIN_11
#define KEY4_PIN GPIO_PIN_10
#define KEY5_PIN GPIO_PIN_9
#define KEY6_PIN GPIO_PIN_8

void KEY_Init(void);

#endif //__DRIVER_KEY_H
