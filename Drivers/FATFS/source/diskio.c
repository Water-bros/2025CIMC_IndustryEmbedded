#include "malloc.h"
#include "driver_usart.h"
#include "diskio.h"
#include "driver_sd_conf.h"
#include "driver_flash.h"


#define SD_CARD     0       /* SD卡,卷标为0 */

/**
 * @brief       获得磁盘状态
 * @param       pdrv : 磁盘编号0~9
 * @retval      执行结果(参见FATFS, DSTATUS的定义)
 */
DSTATUS disk_status (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    return RES_OK;
}

/**
 * @brief       初始化磁盘
 * @param       pdrv : 磁盘编号0~9
 * @retval      执行结果(参见FATFS, DSTATUS的定义)
 */
DSTATUS disk_initialize (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    uint8_t res = 0;

    switch (pdrv)
    {
        case SD_CARD:                  /* SD卡 */
            res = sdio_sd_init();      /* SD卡初始化 */
            break;

        default:
            res = 1;
            break;
    }

    if ((res != SD_OK) && (res != 0))
    {
        return  STA_NOINIT;
    }
    else
    {
        return 0; /* 初始化成功 */
    }
}

/**
 * @brief       读扇区
 * @param       pdrv   : 磁盘编号0~9
 * @param       buff   : 数据接收缓冲首地址
 * @param       sector : 扇区地址
 * @param       count  : 需要读取的扇区数
 * @retval      执行结果(参见FATFS, DRESULT的定义)
 */
DRESULT disk_read (
    BYTE pdrv,      /* Physical drive number to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
)
{
    uint8_t res = 0;

    if (!count)
    {
        return RES_PARERR;           /* count不能等于0，否则返回参数错误 */
    }

    switch (pdrv)
    {
        case SD_CARD:                /* SD卡 */
            res = sd_read_disk(buff, sector, count);

            while (res != SD_OK)     /* 读出错 */
            {
                //printf("sd rd error:%d\r\n", res);
                sdio_sd_init();      /* 重新初始化SD卡 */
                res = sd_read_disk(buff, sector, count);
            }

            break;
        default:
            res = 1;
    }

    /* 处理返回值，将返回值转成ff.c的返回值 */
    if (res == 0x00 || res == SD_OK)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}

/**
 * @brief       写扇区
 * @param       pdrv   : 磁盘编号0~9
 * @param       buff   : 发送数据缓存区首地址
 * @param       sector : 扇区地址
 * @param       count  : 需要写入的扇区数
 * @retval      执行结果(参见FATFS, DRESULT的定义)
 */
DRESULT disk_write (
    BYTE pdrv,          /* Physical drive number to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
)
{
    uint8_t res = 0;

    if (!count)
    {
        return RES_PARERR;           /* count不能等于0，否则返回参数错误 */
    }

    switch (pdrv)
    {
        case SD_CARD:                /* SD卡 */
            res = sd_write_disk((uint8_t *)buff, sector, count);

            while (res != SD_OK)     /* 写出错 */
            {
                //printf("sd wr error:%d\r\n", res);
                sdio_sd_init();      /* 重新初始化SD卡 */
                res = sd_write_disk((uint8_t *)buff, sector, count);
            }

            break;
        default:
            res = 1;
    }

    /* 处理返回值，将返回值转成ff.c的返回值 */
    if (res == 0x00 || res == SD_OK)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}

/**
 * @brief       获取其他控制参数
 * @param       pdrv   : 磁盘编号0~9
 * @param       ctrl   : 控制代码
 * @param       buff   : 发送/接收缓冲区指针
 * @retval      执行结果(参见FATFS, DRESULT的定义)
 */
DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive number (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;

    if (pdrv == SD_CARD)   /* SD卡 */
    {
        switch (cmd)
        {
            case CTRL_SYNC:
                res = RES_OK;
                break;

            case GET_SECTOR_SIZE:
                *(DWORD *)buff = 512;
                res = RES_OK;
                break;

            case GET_BLOCK_SIZE:
                *(WORD *)buff = sd_cardinfo.card_blocksize;
                res = RES_OK;
                break;

            case GET_SECTOR_COUNT:
                *(DWORD *)buff = sd_card_capacity_get() * 2;
                res = RES_OK;
                break;

            default:
                res = RES_PARERR;
                break;
        }
    }
    else
    {
        res = RES_ERROR;     /* 其他的不支持 */
    }

    return res;
}





















