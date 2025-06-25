#include "driver_sd_conf.h"
#include "driver_usart.h"


sd_card_info_struct sd_cardinfo;                                 /* SD卡信息 */


/**
 * @brief       初始化SD卡，获取SD卡信息，设置总线模式和传输模式
 * @param       无
 * @retval      sd_error_enum
 */
sd_error_enum sdio_sd_init(void)
{
    sd_error_enum status = SD_OK;
    uint32_t cardstate = 0;

    nvic_irq_enable(SDIO_IRQn, 2, 0);     /* SDIO中断配置，抢占优先级2，子优先级0 */
  
    status = sd_init();                   /* 初始化SD卡 */      
   
    if (SD_OK == status)
    {
        status = sd_card_information_get(&sd_cardinfo);          /* 获取卡信息 */
    }
    
    if (SD_OK == status)       
    {
        status = sd_card_select_deselect(sd_cardinfo.card_rca);  /* 选中SD卡 */
    } 
    
    status = sd_cardstatus_get(&cardstate);                       /* 获取卡状态 */
    
    if (cardstate & 0x02000000)                                  /* 卡锁住了 */                    
    {
        printf("\r\n the card is locked!");
      
        status = sd_lock_unlock(SD_UNLOCK);                      /* 解锁卡 */
        
        if (status != SD_OK) 
        {
            return SD_LOCK_UNLOCK_FAILED;                        /* 卡解锁失败 */  
        } 
        else 
        {
            printf("\r\n the card is unlocked successfully!");   /* 卡解锁成功 */  
        }
    }
    
    if ((SD_OK == status) && (!(cardstate & 0x02000000))) 
    {
        /* 设置总线模式 */
        status = sd_bus_mode_config(SDIO_BUSMODE_4BIT);          /* 4位SDIO卡总线模式 */ 
//      status = sd_bus_mode_config(SDIO_BUSMODE_1BIT);
    }
    
    if(SD_OK == status)
    {
        /* 设置传输方式 */
      status = sd_transfer_mode_config(SD_DMA_MODE);
//        status = sd_transfer_mode_config(SD_POLLING_MODE);       /* 设置为查询模式 */
    }
    
    return status;
}

/**
 * @brief       通过串口打印SD卡相关信息
 * @param       无
 * @retval      无
 */
void card_info_get(void)
{
    uint8_t sd_spec, sd_spec3, sd_spec4, sd_security;
    uint32_t block_count, block_size;
    uint16_t temp_ccc;
  
    printf("\r\n Card information:");
    sd_spec = (sd_scr[1] & 0x0F000000) >> 24;
    sd_spec3 = (sd_scr[1] & 0x00008000) >> 15;
    sd_spec4 = (sd_scr[1] & 0x00000400) >> 10; /* SD卡SCR寄存器，获取SD卡版本信息 */
  
    if (2 == sd_spec) 
    {
        if (1 == sd_spec3) 
        {
            if (1 == sd_spec4) 
            {
                printf("\r\n## Card version 4.xx ##");
            } 
            else 
            {
                printf("\r\n## Card version 3.0x ##");
            }
        } 
        else 
        {
            printf("\r\n## Card version 2.00 ##");
        }
    } 
    else if (1 == sd_spec) 
    {
        printf("\r\n## Card version 1.10 ##");
    } 
    else if (0 == sd_spec) 
    {
        printf("\r\n## Card version 1.0x ##");
    }

    sd_security = (sd_scr[1] & 0x00700000) >> 20;
    
    if (2 == sd_security) 
    {
        printf("\r\n## SDSC card ##");
    }
    else if (3 == sd_security)
    {
        printf("\r\n## SDHC card ##");
    } 
    else if (4 == sd_security) 
    {
        printf("\r\n## SDXC card ##");
    }

    block_count = (sd_cardinfo.card_csd.c_size + 1) * 1024;
    block_size = 512;
    printf("\r\n## Device size is %dKB ##", sd_card_capacity_get());  /* 显示容量 */
    printf("\r\n## Block size is %dB ##", block_size);                /* 显示块大小 */
    printf("\r\n## Block count is %d ##", block_count);               /* 显示块数量 */

    if (sd_cardinfo.card_csd.read_bl_partial) 
    {
        printf("\r\n## Partial blocks for read allowed ##");
    }
    
    if (sd_cardinfo.card_csd.write_bl_partial) 
    {
        printf("\r\n## Partial blocks for write allowed ##");
    }
    
    temp_ccc = sd_cardinfo.card_csd.ccc;                              /* SD卡命令分类 */
    printf("\r\n## CardCommandClasses is: %x ##", temp_ccc);
    
    if ((SD_CCC_BLOCK_READ & temp_ccc) && (SD_CCC_BLOCK_WRITE & temp_ccc)) 
    {
        printf("\r\n## Block operation supported ##");
    }
    
    if (SD_CCC_ERASE & temp_ccc) 
    {
        printf("\r\n## Erase supported ##");
    }
    
    if (SD_CCC_WRITE_PROTECTION & temp_ccc) 
    {
        printf("\r\n## Write protection supported ##");
    }
    
    if (SD_CCC_LOCK_CARD & temp_ccc) 
    {
        printf("\r\n## Lock unlock supported ##");
    }
    
    if (SD_CCC_APPLICATION_SPECIFIC & temp_ccc) 
    {
        printf("\r\n## Application specific supported ##");
    }
    
    if (SD_CCC_IO_MODE & temp_ccc) 
    {
        printf("\r\n## I/O mode supported ##");
    }
    
    if (SD_CCC_SWITCH & temp_ccc) 
    {
        printf("\r\n## Switch function supported ##");
    }
}

/*!
    \brief      this function handles SDIO interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SDIO_IRQHandler(void)
{
    sd_interrupts_process();
}

/**
 * @brief       读SD卡(fatfs调用)
 * @param       pbuf  : 数据缓存区
 * @param       saddr : 扇区地址
 * @param       cnt   : 扇区个数
 * @retval      SD_OK, 正常;  其他, 错误代码(详见sd_error_enum定义);
 */
sd_error_enum sd_read_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
    sd_error_enum status = SD_OK;

    if (cnt == 1)
    {
        status = sd_block_read(pbuf, saddr, 512);              /* 读取单个block */
    }
		else 
    {
        status = sd_multiblocks_read(pbuf, saddr, 512, cnt);   /* 读取多个block */  
    }

    return status;
}

/**
 * @brief       写SD卡(fatfs调用)
 * @param       pbuf  : 数据缓存区
 * @param       saddr : 扇区地址
 * @param       cnt   : 扇区个数
 * @retval      SD_OK, 正常;  其他, 错误代码(详见sd_error_enum定义);
 */
sd_error_enum sd_write_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
    sd_error_enum status = SD_OK;

    if (cnt == 1)
    {
        status = sd_block_write(pbuf, saddr, 512);    	        /* 写单个block */
    }
		else 
    {
        status = sd_multiblocks_write(pbuf, saddr, 512, cnt);   /* 写多个block */  
    }

    return status;  
}

