#include "log.h"
#include "rtc.h"
#include "oled.h"
#include "flash.h"
#include "timer.h"
#include "usart.h"
#include "driver_tf.h"
#include "driver_led.h"

void Scan_CMD(app_t *app) {
    if (app->cmd_state == CMD_SYSTEM) CMD_Handler(app);
    if ((app->recv_finish && app->recv_length > 0)) {
        app->recv_finish = 0;
        if (app->cmd_state == CMD_NONE) {
            for (uint8_t i = 0; i < CMD_LENGTH; i++) {
                if (strcmp(app->rx_buffer, app->cmd_text[i].text) == 0) {
                    app->cmd = i;
                    break;
                }
            }
        }
        CMD_Handler(app);
    }
}

void CMD_Handler(app_t *app) {
    text flash_state[2], tf_state[2];
    char buffer[64];
    uint8_t result;
    uint32_t flash_id, tf_memory;
    float _ratio, _limit;

    switch (app->cmd) {
        case CMD_TEST:
            Save_Log(app, "system hardware test");
            printf("======system selftest======\n\r");
            flash_id = Flash_Read_ID();
            tf_memory = TF_Read_Memory_Size();
            flash_state[0].text = flash_id ? "ok" : "error";
            tf_state[0].text = tf_memory ? "ok" : "error";

            if (tf_memory) {
                tf_state[1].text = (char *) malloc(sizeof(char) * 32);
                sprintf(tf_state[1].text, "TF card memory: %ld KB", tf_memory);
            } else {
                tf_state[1].text = "can not find TF card";
                Save_Log(app, "test error: tf card not found");
            }

            if (flash_id) {
                flash_state[1].text = (char *) malloc(sizeof(char) * 24);
                sprintf(flash_state[1].text, "flash id: 0x%04X", flash_id);
            } else {
                flash_state[1].text = "can not find flash";
            }

            printf("flash............%s\n\rTF card............%s\n\r", flash_state[0].text, tf_state[0].text);
            printf("%s\n\r%s\n\r", flash_state[1].text, tf_state[1].text);

            RTC_Get_Time(&app->time);
            printf("RTC: 20%02d-%02d-%02d %02d:%02d:%02d\n\r", app->time.year, app->time.month, app->time.date,
                   app->time.hour, app->time.min, app->time.sec);
            printf("======system selftest======\n\r");

            if (flash_id && tf_memory) { Save_Log(app, "test ok"); }

            strcpy(app->oled_text[0], "test");
            strcpy(app->oled_text[1], "");
            OLED_Show(app);
            break;
        case CMD_RTC_CONFIG:
            Save_Log(app, "rtc config");
            printf("Input Datetime\n\r");
            app->cmd_state = CMD_RTC_CONFIG;
            break;
        case CMD_RTC_NOW:
            RTC_Get_Time(&app->time);
            printf("Current time:20%02d-%02d-%02d %02d:%02d:%02d\n\r", app->time.year, app->time.month, app->time.date,
                   app->time.hour, app->time.min, app->time.sec);
            break;
        case CMD_CONFIG:
            Read_Config_INI(app);
            break;
        case CMD_RATIO:
            Save_Log(app, "ratio config");
            printf("Ratio = %.2f\n\rInput value(0-100):\n\r", app->ratio);
            app->cmd_state = CMD_RATIO;
            break;
        case CMD_LIMIT:
            Save_Log(app, "limit config");
            printf("Limit = %.2f\n\rInput value(0-200):\n\r", app->limit);
            app->cmd_state = CMD_LIMIT;
            break;
        case CMD_CONFIG_SAVE:
            Save_Config_Data(app);
            printf("ratio: %.2f\n\rlimit: %.2f\n\rsave parameters to flash\n\r", app->ratio, app->limit);
            break;
        case CMD_CONFIG_READ:
            Read_Config_Data(app);
            printf("read parameters from flash\n\rratio: %.2f\n\rlimit: %.2f\n\r", app->ratio, app->limit);
            break;
        case CMD_START:
            switch (app->cmd_state) {
                case CMD_SYSTEM:
                    sprintf(buffer, "sample start - cycle %ds (key press)", app->sample_period);
                    break;
                default:
                    sprintf(buffer, "sample start - cycle %ds (command)", app->sample_period);
                    break;
            }

            Save_Log(app, buffer);
            printf("Periodic Sampling\n\rsample cycle: %ds\n\r", app->sample_period);
            Timer_On();
            app->state = STATE_SAMPLE;
            break;
        case CMD_STOP:
            switch (app->cmd_state) {
                case CMD_SYSTEM:
                    Save_Log(app, "sample stop (key press)");
                    break;
                default:
                    Save_Log(app, "sample stop (command)");
                    break;
            }

            printf("Periodic Sampling STOP\n\r");
            LED_Off(1);
            LED_Off(2);
            Timer_Off();
            app->state = STATE_IDLE;
            strcpy(app->oled_text[0], "system idle");
            strcpy(app->oled_text[1], "");
            OLED_Show(app);
            break;
        case CMD_HIDE:
            Save_Log(app, "hide data");
            app->hide_state = STATE_HIDE;
            break;
        case CMD_UNHIDE:
            Save_Log(app, "unhide data");
            app->hide_state = STATE_UNHIDE;
            break;
        case CMD_CLEAN:
            app->setup_count = 0;
            app->ratio = 1.0f;
            app->limit = 1.0f;
            printf("clean done\n\r");
            break;
        case CMD_TID:
            strcpy(buffer, "2025-CIMC-2025xxxxxx"); // 改为自己的队伍编号
            Write_Team_ID(buffer);
            printf("team id set\n\r");
            break;
        default:
            switch (app->cmd_state) {
                case CMD_RTC_CONFIG:
                    result = sscanf(app->rx_buffer, "20%d-%d-%d %d:%d:%d", &app->time.year, &app->time.month,
                                    &app->time.date, &app->time.hour, &app->time.min, &app->time.sec);

                    if (result == 6) {
                        RTC_Set_Time_Struct(&app->time);
                        sprintf(buffer, "20%02d-%02d-%02d %02d:%02d:%02d", app->time.year,
                                app->time.month, app->time.date, app->time.hour, app->time.min, app->time.sec);
                        printf("RTC Config success\n\rTime:%s\n\r", buffer);
                        sprintf(buffer, "rtc config success to %s", buffer);
                        Save_Log(app, buffer);
                    } else {
                        printf("RTC Config Error\n\r");
                    }

                    break;
                case CMD_RATIO:
                    result = sscanf(app->rx_buffer, "%f", &_ratio);

                    if (result == 1 && _ratio > 0 && _ratio < 100) {
                        app->ratio = _ratio;
                        printf("ratio modified success\n\rRatio = %.2f\n\r", app->ratio);
                        sprintf(buffer, "ratio config success to %.2f", app->ratio);
                        Save_Log(app, buffer);
                    } else {
                        app->ratio = 1.0f;
                        printf("ratio invalid\n\rRatio = %.2f\n\r", app->ratio);
                    }

                    break;
                case CMD_LIMIT:
                    result = sscanf(app->rx_buffer, "%f", &_limit);

                    if (result == 1 && _limit > 0 && _limit < 200) {
                        app->limit = _limit;
                        printf("limit modified success\n\rlimit = %.2f\n\r", app->limit);
                        sprintf(buffer, "limit config success to %.2f", app->limit);
                        Save_Log(app, buffer);
                    } else {
                        app->ratio = 1.0f;
                        printf("limit invalid\n\rlimit = %.2f\n\r", app->limit);
                    }

                    break;
                default:
                    break;
            }
            app->cmd_state = CMD_NONE;
            break;
    }
    app->cmd = CMD_NONE;
}