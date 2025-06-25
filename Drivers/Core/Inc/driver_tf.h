#ifndef __DRIVER_TF_H
#define __DRIVER_TF_H

#include "main.h"
#include "gd32f4xx.h"
#include "driver_sd_conf.h"
#include "exfuns.h"

void TF_Init(void);

void TF_Write_File(char *filename, char *text, BYTE mode);

void TF_Read_File(char *filename, char *text, BYTE mode);

void TF_Make_Dir(char *dirname);

void TF_Init_Dir(char *dirname);

uint32_t TF_Read_Memory_Size(void);

#endif // __DRIVER_TF_H
