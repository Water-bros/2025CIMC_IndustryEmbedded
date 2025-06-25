#include "log.h"
#include "rtc.h"
#include "usart.h"

time_tt t;
FRESULT _res;
FIL _file;
FILINFO *_fno;
UINT _num_of_read;

int s_count = 0;
char buffer[128], fn[64], sample_filename[64], over_limit_filename[64], hide_data_filename[64], log_filename[16];
BYTE read_buffer[1024];

static void Get_File_Name(char *filename) {
    RTC_Get_Time(&t);
    sprintf(filename, "%s20%02d%02d%02d%02d%02d%02d.txt", filename, t.year, t.month, t.date, t.hour, t.min, t.sec);
}

void Log_Init(void) {
    strcpy(sample_filename, "sample/sampleData");
    strcpy(over_limit_filename, "overLimit/overLimit");
    strcpy(hide_data_filename, "hideData/hideData");

    Get_File_Name(sample_filename);
    Get_File_Name(over_limit_filename);
    Get_File_Name(hide_data_filename);
}


void Save_Sample_Data(app_t *app) {
    switch (app->state) {
        case STATE_SAMPLE:
            sprintf(buffer, "%s %.2fV\n", app->status_text[0], app->voltage);

            if (app->sample_count - s_count < 11) {
                TF_Write_File(sample_filename, buffer, FA_OPEN_APPEND | FA_WRITE);
            } else {
                s_count = app->sample_count;
                strcpy(fn, "sample/sampleData");
                Get_File_Name(fn);
                strcpy(sample_filename, fn);
                TF_Write_File(sample_filename, buffer, FA_OPEN_ALWAYS | FA_WRITE);
            }
            break;
        case STATE_OVER_LIMIT:
            Save_OverLimit_Data(app);
            break;
    }
}

void Save_OverLimit_Data(app_t *app) {
    sprintf(buffer, "%s %.2fV limit %.2fV\n", app->status_text[0], app->voltage, app->limit);

    if (app->sample_count - s_count < 11) {
        TF_Write_File(over_limit_filename, buffer, FA_OPEN_APPEND | FA_WRITE);
    } else {
        s_count = app->sample_count;
        strcpy(fn, "overLimit/overLimit");
        Get_File_Name(fn);
        strcpy(over_limit_filename, fn);
        TF_Write_File(over_limit_filename, buffer, FA_OPEN_ALWAYS | FA_WRITE);
    }
}

void Save_Hide_Data(app_t *app) {
    switch (app->state) {
        case STATE_OVER_LIMIT:
            Save_OverLimit_Data(app);
        case STATE_SAMPLE:
            sprintf(buffer, "%s %.2fV\nhide: %s%s\n", app->status_text[0], app->voltage, app->hide_text[0],
                    app->hide_text[1]);
            if (app->sample_count - s_count < 11) {
                TF_Write_File(hide_data_filename, buffer, FA_OPEN_APPEND | FA_WRITE);
            } else {
                s_count = app->sample_count;
                strcpy(fn, "hideData/hideData");
                Get_File_Name(fn);
                strcpy(hide_data_filename, fn);
                TF_Write_File(hide_data_filename, buffer, FA_OPEN_ALWAYS | FA_WRITE);
            }
            break;
    }
}

void Save_Log(app_t *app, char *text) {
    RTC_Get_Time(&t);
    sprintf(log_filename, "log/log%d.txt", app->setup_count - 1);
    sprintf(buffer, "20%02d-%02d-%02d %02d:%02d:%02d %s\n", t.year, t.month, t.date, t.hour, t.min, t.sec, text);

    _res = f_stat(log_filename, _fno);
    if (_res != FR_OK) {
        TF_Write_File(log_filename, buffer, FA_OPEN_ALWAYS | FA_WRITE);
    } else {
        TF_Write_File(log_filename, buffer, FA_OPEN_APPEND | FA_WRITE);
    }
}


void Read_Config_INI(app_t *app) {
    _res = f_open(&_file, "config.ini", FA_READ | FA_OPEN_EXISTING);
    if (_res == FR_OK) {
        _res = f_read(&_file, read_buffer, sizeof(read_buffer), &_num_of_read);
        if (_res == FR_OK) {
            sscanf(read_buffer, "[Ratio]\nCh0 = %f\n\n[Limit]\nCh0 = %f", &app->ratio, &app->limit);
            printf("Ratio = %.2f\n\rLimit = %.2f\n\rconfig read success\n\r", app->ratio, app->limit);
        } else {
        }
    } else {
        printf("config.ini file not found.\n\r");
    }
    f_close(&_file);
}