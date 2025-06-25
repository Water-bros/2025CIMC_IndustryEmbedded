#include "driver_key.h"

void KEY_Init(void) {
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_mode_set(KEY_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE,
                  KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN | KEY5_PIN | KEY6_PIN);
}
