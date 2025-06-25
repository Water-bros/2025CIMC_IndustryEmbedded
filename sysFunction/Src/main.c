#include "main.h"
#include "systick.h"
#include "driver_adc.h"
#include "driver_usart.h"
#include "driver_timer.h"
#include "driver_flash.h"
#include "driver_key.h"
#include "driver_tf.h"
#include "driver_led.h"
#include "usart.h"
#include "timer.h"
#include "flash.h"
#include "key.h"
#include "oled.h"
#include "rtc.h"
#include "log.h"

app_t app;

text cmd_text[CMD_LENGTH] = {
        {"test"},
        {"RTC Config"},
        {"RTC now"},
        {"conf"},
        {"ratio"},
        {"limit"},
        {"config save"},
        {"config read"},
        {"start"},
        {"stop"},
        {"hide"},
        {"unhide"},
        {"clean"},
        {"tid"}
};

void APP_Init() {
    app.state = STATE_INIT;
    app.hide_state = STATE_UNHIDE;
    app.cmd_text = cmd_text;
    app.cmd = CMD_NONE;
    app.cmd_state = CMD_NONE;
    app.sample_count = 0;
    app.timer_court = 0;
    app.key = 0;

    Read_Config_Data(&app);

    if (app.ratio == 0.0f || app.limit == 0.0f || app.sample_period == 0) {
        app.ratio = 1.0f;
        app.limit = 1.0f;
        app.sample_period = 5;
    }

    app.state = STATE_IDLE;
    app.setup_count++;
    strcpy(app.oled_text[0], "system idle");
    Save_Config_Data(&app);
    OLED_Show(&app);
}

int main(void) {
    systick_config();
    /* Peripheral Init */
    USART_Init(&app);
    Timer_Init();
    Flash_Init();
    TF_Init();
    KEY_Init();
    LED_Init();
    ADC_Init();
    RTC_Config();
    OLED_Init();
    Log_Init();
    APP_Init();

    delay_1ms(5000);
    char team_id[64] = {0};
    Read_Team_ID((uint8_t *) team_id);
    printf("====system init====\n\r");
    printf("Device_ID:%s\n\r", team_id);
    printf("====system ready====\n\r");
    Save_Log(&app, "system init");

    while (1) {
        Scan_CMD(&app);
        Scan_key(&app);
    }
}

void TIMER5_DAC_IRQHandler(void) {
    if (timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP) == SET) {
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
        app.timer_court++;
        LED_Timer_Handler(&app);
        Sample_Timer_Handler(&app);
    }
}

void DMA1_Channel2_IRQHandler(void) {
    if (dma_interrupt_flag_get(DMA1, DMA_CH2, DMA_INT_FLAG_FTF) == SET) {
        dma_interrupt_flag_clear(DMA1, DMA_CH2, DMA_INT_FLAG_FTF);
    }
}

void USART0_IRQHandler(void) {
    if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET) {
        usart_data_receive(USART0);
        app.recv_length = 4096 - dma_transfer_number_get(DMA1, DMA_CH2);
        dma_channel_disable(DMA1, DMA_CH2);
        USART_DMA_Config(&app);
        app.rx_buffer[app.recv_length] = '\0';
        app.recv_finish = 1;
    }
}
