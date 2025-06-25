#include "driver_led.h"

const uint32_t led_PINs[7] = {0, LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN, LED5_PIN, LED6_PIN};

void LED_Init(void) {
    rcu_periph_clock_enable(RCU_GPIOE);
    gpio_mode_set(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                  LED6_PIN | LED5_PIN | LED4_PIN | LED1_PIN | LED2_PIN | LED3_PIN);
    gpio_output_options_set(LED_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                            LED6_PIN | LED5_PIN | LED4_PIN | LED1_PIN | LED2_PIN | LED3_PIN);
    gpio_bit_reset(LED_PORT, LED6_PIN | LED5_PIN | LED4_PIN | LED1_PIN | LED2_PIN | LED3_PIN);
}

void LED_On(uint8_t led) {
    gpio_bit_set(LED_PORT, led_PINs[led]);
}

void LED_Off(uint8_t led) {
    gpio_bit_reset(LED_PORT, led_PINs[led]);
}

void LED_Toggle(uint8_t led) {
    gpio_bit_toggle(LED_PORT, led_PINs[led]);
}
