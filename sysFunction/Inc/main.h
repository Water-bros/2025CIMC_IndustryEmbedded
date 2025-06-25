#ifndef __MAIN_H
#define __MAIN_H

enum {
    STATE_INIT,
    STATE_IDLE,
    STATE_HIDE,
    STATE_UNHIDE,
    STATE_OVER_LIMIT,
    STATE_SAMPLE,
    STATE_OK,
    STATE_ERROR,
    STATE_LENGTH
};

enum {
    CMD_TEST,
    CMD_RTC_CONFIG,
    CMD_RTC_NOW,
    CMD_CONFIG,
    CMD_RATIO,
    CMD_LIMIT,
    CMD_CONFIG_SAVE,
    CMD_CONFIG_READ,
    CMD_START,
    CMD_STOP,
    CMD_HIDE,
    CMD_UNHIDE,
    CMD_CLEAN,
    CMD_TID,
    CMD_NONE,
    CMD_SYSTEM,
    CMD_LENGTH
};

typedef struct {
    char *text;
} text;

typedef struct {
    char year;
    char month;
    char date;
    char hour;
    char min;
    char sec;
} time_tt;

typedef struct {
    text *cmd_text;
    char status_text[2][32];
    char hide_text[2][32];
    char oled_text[2][32];
    char state;
    char cmd_state;
    char cmd;
    char hide_state;
    char sample_period;
    char key;
    char setup_count;
    int timer_court;
    int sample_count;
    int recv_length;
    char rx_buffer[4096];
    char recv_finish;
    time_tt time;
    float ratio;
    float limit;
    float voltage;
} app_t;


#endif /* __MAIN_H */


