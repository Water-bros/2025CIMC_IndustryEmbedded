#ifndef __DRIVER_USART_H
#define __DRIVER_USART_H

#include "main.h"
#include "gd32f4xx.h"
#include "stdio.h"
#include "string.h"

#define USART0_TX_PORT GPIOA
#define USART0_RX_PORT GPIOA
#define USART0_TX_PIN GPIO_PIN_9
#define USART0_RX_PIN GPIO_PIN_10
#define USART0_AF GPIO_AF_7
#define USART0_DATA_ADDRESS ((uint32_t)&USART_DATA(USART0))
#define ARRAYNUM(arr_name)   (uint32_t)(sizeof(arr_name) / sizeof(*(arr_name)))

void USART_Init(app_t *app);

void USART_DMA_Config(app_t *app);

#endif //__DRIVER_USART_H