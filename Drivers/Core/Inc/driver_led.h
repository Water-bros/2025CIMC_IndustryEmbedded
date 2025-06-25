#ifndef __DRIVER_LED_H
#define __DRIVER_LED_H

#include "main.h"
#include "gd32f4xx.h"

#define LED_PORT GPIOE
#define LED6_PIN GPIO_PIN_10
#define LED5_PIN GPIO_PIN_11
#define LED4_PIN GPIO_PIN_12
#define LED1_PIN GPIO_PIN_13
#define LED2_PIN GPIO_PIN_14
#define LED3_PIN GPIO_PIN_15

void LED_Init(void);

void LED_On(uint8_t led);

void LED_Off(uint8_t led);

void LED_Toggle(uint8_t led);

void LED_Status(void);

#endif //__DRIVER_LED_H
