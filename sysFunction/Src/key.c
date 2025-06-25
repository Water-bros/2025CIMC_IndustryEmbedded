#include "key.h"
#include "systick.h"
#include "log.h"
#include "flash.h"
#include "usart.h"

const uint32_t KEY_PINs[6] = {
        KEY1_PIN,
        KEY2_PIN,
        KEY3_PIN,
        KEY4_PIN,
        KEY5_PIN,
        KEY6_PIN
};

void Scan_key(app_t *app) {
    for (uint8_t i = 0; i < 6; i++) {
        if (gpio_input_bit_get(KEY_PORT, KEY_PINs[i]) == SET) {
            delay_1ms(100);
            if (gpio_input_bit_get(KEY_PORT, KEY_PINs[i]) == SET) {
                app->key = i + 1;
            }
        }
    }
    switch (app->state) {
        case STATE_OVER_LIMIT:
        case STATE_SAMPLE:
            switch (app->key) {
                case 1:
                    app->cmd = CMD_STOP;
                    app->cmd_state = CMD_SYSTEM;
                    break;
                case 2:
                    app->sample_period = 5;
                    Save_Config_Data(app);
                    printf("sample cycle adjust: %ds\n\r", app->sample_period);
                    Save_Log(app, "cycle switch to 5s (key press)");
                    break;
                case 3:
                    app->sample_period = 10;
                    Save_Config_Data(app);
                    printf("sample cycle adjust: %ds\n\r", app->sample_period);
                    Save_Log(app, "cycle switch to 10s (key press)");
                    break;
                case 4:
                    app->sample_period = 15;
                    Save_Config_Data(app);
                    printf("sample cycle adjust: %ds\n\r", app->sample_period);
                    Save_Log(app, "cycle switch to 15s (key press)");
                    break;
                default:
                    break;
            }
            break;
        case STATE_IDLE:
            switch (app->key) {
                case 1:
                    app->cmd = CMD_START;
                    app->cmd_state = CMD_SYSTEM;
                    break;
                default:
                    break;
            }
            break;
    }
    app->key = 0;
}