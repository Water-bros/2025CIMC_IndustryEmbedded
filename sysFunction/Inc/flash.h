#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"
#include "gd32f4xx.h"
#include "driver_flash.h"

#define TEAM_ID_ADDR 0x7F00
#define CONFIG_DATA_ADDR 0x8000

void Read_Team_ID(uint8_t *buffer);

void Write_Team_ID(uint8_t *buffer);

void Read_Config_Data(app_t *app);

void Save_Config_Data(app_t *app);

#endif //__FLASH_H