#include "driver_timer.h"

void Timer_Init(void) {
    Timer_Config();
    Timer_Off();
}

void Timer_Config(void) {
    rcu_periph_clock_enable(RCU_TIMER5);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER5);
    timer_parameter_struct timer_init_struct;
    /* 周期设置：Timer(s) = (Prescaler+1)(100khz) / Freq(Hz) * Period(us) */
    timer_init_struct.prescaler = 20000 - 1; // 默认200Mhz分频, 200-1
    timer_init_struct.alignedmode = TIMER_COUNTER_EDGE;
    timer_init_struct.counterdirection = TIMER_COUNTER_UP;
    timer_init_struct.period = 5000 - 1; // 10000us
    /* 得到定时器周期为1s */
    timer_init_struct.clockdivision = TIMER_CKDIV_DIV1;
    timer_init_struct.repetitioncounter = 0;
    timer_init(TIMER5, &timer_init_struct);

    nvic_irq_enable(TIMER5_DAC_IRQn, 3, 2);
    timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER5, TIMER_INT_UP);
}

void Timer_On(void) {
    timer_enable(TIMER5);
}

void Timer_Off(void) {
    timer_disable(TIMER5);
}