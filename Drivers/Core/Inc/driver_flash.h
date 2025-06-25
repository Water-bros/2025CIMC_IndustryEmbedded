#ifndef __DRIVER_FLASH_H
#define __DRIVER_FLASH_H

#include "main.h"
#include "gd32f4xx.h"

#define FLASH_PORT GPIOB
#define FLASH_MOSI_PIN GPIO_PIN_15
#define FLASH_MISO_PIN GPIO_PIN_14
#define FLASH_SCLK_PIN GPIO_PIN_13
#define FLASH_CS_PIN GPIO_PIN_12

#define CS_LOW() gpio_bit_reset(FLASH_PORT, FLASH_CS_PIN)
#define CS_HIGH() gpio_bit_set(FLASH_PORT, FLASH_CS_PIN)

void Flash_Init(void);

void Flash_Write_Enable(void);

void Flash_Wait_Busy(void);

void Flash_Erase_Sector(uint32_t addr);

void Flash_Read(uint8_t *buffer, uint32_t addr, uint16_t numbyte);

void Flash_Write(uint8_t *buffer, uint32_t addr, uint16_t numbyte);

uint16_t Flash_Read_ID(void);

uint8_t SPI_WR_Byte(uint8_t data);

#endif //__DRIVER_FLASH_H
