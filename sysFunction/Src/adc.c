#include "adc.h"
#include "driver_led.h"

uint16_t Get_ADC_Value(void) {
    uint16_t adc_value = 0;

    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_15);
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    while (adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET) {}
    adc_value = adc_regular_data_read(ADC0);

    return adc_value;
}


void ADC_Sample(app_t *app) {
    app->sample_count++;

    app->voltage = (double) Get_ADC_Value() * 3.3 * app->ratio / 4095.0;
    app->state = app->voltage > app->limit ? STATE_OVER_LIMIT : STATE_SAMPLE;

    switch (app->state) {
        case STATE_OVER_LIMIT:
            LED_On(2);
            break;
        case STATE_SAMPLE:
            LED_Off(2);
            break;
    }
}