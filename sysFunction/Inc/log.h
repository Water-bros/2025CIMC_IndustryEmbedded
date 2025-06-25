#ifndef __LOG_H
#define __LOG_H

#include "main.h"
#include "gd32f4xx.h"
#include "driver_tf.h"

void Log_Init(void);

void Save_Sample_Data(app_t *app);

void Save_OverLimit_Data(app_t *app);

void Save_Hide_Data(app_t *app);

void Save_Log(app_t *app, char *text);

void Read_Config_INI(app_t *app);

#endif // __LOG_H
