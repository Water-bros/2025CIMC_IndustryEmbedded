#include "adc.h"
#include "key.h"
#include "log.h"
#include "rtc.h"
#include "oled.h"
#include "time.h"
#include "timer.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "driver_led.h"

void hide_data(app_t *app) {
    struct tm time = {0};
    time.tm_year = app->time.year + 100;
    time.tm_mon = app->time.month - 1;
    time.tm_mday = app->time.date;
    time.tm_hour = app->time.hour;
    time.tm_min = app->time.min;
    time.tm_sec = app->time.sec;

    uint32_t timestamp = (uint32_t) mktime(&time) + 8 * 3600;

    uint16_t int_part = (uint16_t) app->voltage;
    uint16_t frac_part = (uint16_t) ((app->voltage - int_part) * 65536);

    sprintf(app->hide_text[0], "%08X%04X%04X", timestamp, int_part, frac_part);

    switch (app->state) {
        case STATE_SAMPLE:
            strcpy(app->hide_text[1], "");
            break;
        case STATE_OVER_LIMIT:
            strcpy(app->hide_text[1], "*");
            break;
        default:
            break;
    }
}

void unhide_data(app_t *app) {
    switch (app->state) {
        case STATE_SAMPLE:
            sprintf(app->status_text[0], "20%02d-%02d-%02d %02d:%02d:%02d", app->time.year, app->time.month,
                    app->time.date, app->time.hour, app->time.min, app->time.sec);
            sprintf(app->status_text[1], "ch0:%.2fV", app->voltage);
            break;
        case STATE_OVER_LIMIT:
            sprintf(app->status_text[0], "20%02d-%02d-%02d %02d:%02d:%02d", app->time.year, app->time.month,
                    app->time.date, app->time.hour, app->time.min, app->time.sec);
            sprintf(app->status_text[1], "ch0:%.2fV OverLimit (%.2f)!", app->voltage, app->limit);
            break;
        default:
            break;
    }

    sprintf(app->oled_text[0], "%02d:%02d:%02d", app->time.hour, app->time.min, app->time.sec);
    sprintf(app->oled_text[1], "%.2fV", app->voltage);
}

void Sample_Timer_Handler(app_t *app) {
    if (app->timer_court % (app->sample_period * 2) == 0) {
        app->timer_court = 0;

        Scan_key(app);
        Scan_CMD(app);
        ADC_Sample(app);
        RTC_Get_Time(&app->time);
        unhide_data(app);
        OLED_Show(app);
        switch (app->hide_state) {
            case STATE_UNHIDE:
                Save_Sample_Data(app);
                printf("%s %s\n\r", app->status_text[0], app->status_text[1]);
                break;
            case STATE_HIDE:
                hide_data(app);
                Save_Hide_Data(app);
                printf("%s%s\n\r", app->hide_text[0], app->hide_text[1]);
                break;
        }
    }
}

void LED_Timer_Handler(app_t *app) {
    Scan_key(app);
    Scan_CMD(app);
    LED_Toggle(1);
}