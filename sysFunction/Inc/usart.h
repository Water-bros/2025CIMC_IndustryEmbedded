#ifndef __USART_H
#define __USART_H

#include "main.h"
#include "gd32f4xx.h"
#include "driver_usart.h"
#include "stdio.h"
#include "string.h"

void Scan_CMD(app_t *app);

void CMD_Handler(app_t *app);

#endif //__USART_H