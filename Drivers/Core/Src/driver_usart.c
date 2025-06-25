#include "driver_usart.h"

void USART_Init(app_t *app) {
    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(RCU_GPIOA);

    gpio_af_set(USART0_TX_PORT, USART0_AF, USART0_TX_PIN);
    gpio_af_set(USART0_RX_PORT, USART0_AF, USART0_RX_PIN);

    gpio_mode_set(USART0_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART0_TX_PIN);
    gpio_mode_set(USART0_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART0_RX_PIN);

    gpio_output_options_set(USART0_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, USART0_TX_PIN);
    gpio_output_options_set(USART0_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, USART0_RX_PIN);

    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);

    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    nvic_irq_enable(USART0_IRQn, 2, 2);
    usart_interrupt_enable(USART0, USART_INT_IDLE);
    usart_enable(USART0);
}

void USART_DMA_Config(app_t *app) {
    dma_single_data_parameter_struct dma_init_struct;
    rcu_periph_clock_enable(RCU_DMA1);

    dma_deinit(DMA1, DMA_CH2);
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory0_addr = (uint32_t) app->rx_buffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(app->rx_buffer);
    dma_init_struct.periph_addr = USART0_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;

    dma_single_data_mode_init(DMA1, DMA_CH2, &dma_init_struct);
    dma_circulation_disable(DMA1, DMA_CH2);
    dma_channel_subperipheral_select(DMA1, DMA_CH2, DMA_SUBPERI4);
    dma_channel_enable(DMA1, DMA_CH2);

    dma_interrupt_enable(DMA1, DMA_CH2, DMA_CHXCTL_FTFIE);
    nvic_irq_enable(DMA1_Channel2_IRQn, 0, 1);
    usart_dma_receive_config(USART0, USART_DENR_ENABLE);
}

int __io_putchar(int ch) {
    usart_data_transmit(USART0, (uint32_t) ch);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

int _write(int fd, char *pBuffer, int size) {
    for (int i = 0; i < size; i++) {
        __io_putchar(*pBuffer++);
    }
    return size;
}

