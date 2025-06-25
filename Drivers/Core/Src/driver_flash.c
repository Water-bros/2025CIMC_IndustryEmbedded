#include "driver_flash.h"

void Flash_Init(void) {
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_SPI1);

    gpio_af_set(FLASH_PORT, GPIO_AF_5, FLASH_SCLK_PIN);
    gpio_af_set(FLASH_PORT, GPIO_AF_5, FLASH_MISO_PIN);
    gpio_af_set(FLASH_PORT, GPIO_AF_5, FLASH_MOSI_PIN);
    gpio_af_set(FLASH_PORT, GPIO_AF_5, FLASH_CS_PIN);

    gpio_mode_set(FLASH_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_SCLK_PIN);
    gpio_mode_set(FLASH_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_MISO_PIN);
    gpio_mode_set(FLASH_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_MOSI_PIN);
    gpio_mode_set(FLASH_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, FLASH_CS_PIN);

    gpio_output_options_set(FLASH_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_SCLK_PIN);
    gpio_output_options_set(FLASH_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_MISO_PIN);
    gpio_output_options_set(FLASH_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_MOSI_PIN);
    gpio_output_options_set(FLASH_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_CS_PIN);

    gpio_bit_set(FLASH_PORT, FLASH_CS_PIN);

    spi_parameter_struct spi_init_struct;
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT;
    spi_init_struct.prescale = SPI_PSC_2;
    spi_init_struct.endian = SPI_ENDIAN_MSB;

    spi_init(SPI1, &spi_init_struct);
    spi_enable(SPI1);
}

uint8_t SPI_WR_Byte(uint8_t data) {
    while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
    spi_i2s_data_transmit(SPI1, data);
    while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE));
    return spi_i2s_data_receive(SPI1);
}

uint16_t Flash_Read_ID(void) {
    uint16_t temp = 0;
    CS_LOW();

    SPI_WR_Byte(0x90);//发送读取ID命令
    SPI_WR_Byte(0x00);
    SPI_WR_Byte(0x00);
    SPI_WR_Byte(0x00);

    temp |= SPI_WR_Byte(0xFF) << 8;
    temp |= SPI_WR_Byte(0xFF);

    CS_HIGH();
    return temp;
}

void Flash_Wait_Busy(void) {
    unsigned char byte = 0;
    do {
        CS_LOW();
        SPI_WR_Byte(0x05);
        byte = SPI_WR_Byte(0Xff);
        CS_HIGH();
    } while ((byte & 0x01) == 1);
}

void Flash_Write_Enable(void) {
    CS_LOW();
    SPI_WR_Byte(0x06);
    CS_HIGH();
}

void Flash_Erase_Sector(uint32_t addr) {
    addr *= 4096;
    Flash_Write_Enable();
    Flash_Wait_Busy();

    CS_LOW();
    SPI_WR_Byte(0x20);
    SPI_WR_Byte((uint8_t) ((addr) >> 16));
    SPI_WR_Byte((uint8_t) ((addr) >> 8));
    SPI_WR_Byte((uint8_t) addr);
    CS_HIGH();

    Flash_Wait_Busy();
}

void Flash_Write(uint8_t *buffer, uint32_t addr, uint16_t numbyte) {
    unsigned int i = 0;
    Flash_Erase_Sector(addr / 4096);
    Flash_Write_Enable();
    Flash_Wait_Busy();

    CS_LOW();
    SPI_WR_Byte(0x02);
    SPI_WR_Byte((uint8_t) ((addr) >> 16));
    SPI_WR_Byte((uint8_t) ((addr) >> 8));
    SPI_WR_Byte((uint8_t) addr);
    for (i = 0; i < numbyte; i++) {
        SPI_WR_Byte(buffer[i]);
    }
    CS_HIGH();

    Flash_Wait_Busy();
}

void Flash_Read(uint8_t *buffer, uint32_t read_addr, uint16_t read_length) {
    uint16_t i;
    CS_LOW();
    SPI_WR_Byte(0x03);
    SPI_WR_Byte((uint8_t) ((read_addr) >> 16));
    SPI_WR_Byte((uint8_t) ((read_addr) >> 8));
    SPI_WR_Byte((uint8_t) read_addr);
    for (i = 0; i < read_length; i++) {
        buffer[i] = SPI_WR_Byte(0XFF);
    }
    CS_HIGH();
}
