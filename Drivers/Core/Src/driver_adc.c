#include "driver_adc.h"

void ADC_Init(void) {
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_ADC0);

    adc_clock_config(ADC_ADCCK_PCLK2_DIV4);

    gpio_mode_set(ADC_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_PIN);

    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
    adc_channel_length_config(ADC0, ADC_CHANNEL_10, 1);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    adc_enable(ADC0);
    adc_calibration_enable(ADC0);
}