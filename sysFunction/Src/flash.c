#include "flash.h"

typedef union {
    float f[4];
    uint8_t b[16];
} FloatConvertor;

void Read_Team_ID(uint8_t *buffer) {
    Flash_Read(buffer, TEAM_ID_ADDR, 64);
}

void Write_Team_ID(uint8_t *buffer) {
    Flash_Write(buffer, TEAM_ID_ADDR, 64);
}

void Read_Config_Data(app_t *app) {
    FloatConvertor convertor;
    Flash_Read((uint8_t *) &convertor, CONFIG_DATA_ADDR, sizeof(convertor));
    app->ratio = convertor.f[0];
    app->limit = convertor.f[1];
    app->sample_period = convertor.f[2];
    app->setup_count = convertor.f[3];
}

void Save_Config_Data(app_t *app) {
    FloatConvertor convertor;
    convertor.f[0] = app->ratio;
    convertor.f[1] = app->limit;
    convertor.f[2] = app->sample_period;
    convertor.f[3] = app->setup_count;
    Flash_Write((uint8_t *) &convertor, CONFIG_DATA_ADDR, sizeof(convertor));
}