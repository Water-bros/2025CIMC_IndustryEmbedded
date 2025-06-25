/**
 ****************************************************************************************************
 * @file        malloc.h
 * @version     V1.0
 * @brief       内存管理 代码
 ****************************************************************************************************
 *
 * 实验平台:    GD32F470VET6小系统板
 *
 ****************************************************************************************************
 */

#ifndef __MALLOC_H
#define __MALLOC_H

#include "main.h"
#include "gd32f4xx.h"

/* 如果没有定义NULL, 定义NULL */
#ifndef NULL
#define NULL 0
#endif


/* 定义两个内存池 */
#define SRAMIN      0       /* 内部SRAM内存池 */
#define SRAMTCM     1       /* TCMSRAM内存池(此部分SRAM仅仅CPU可以访问!!!) */

#define SRAMBANK    2       /* 定义支持的SRAM块数 */


/* 定义内存管理表类型,当外扩SDRAM的时候，必须使用uint32_t类型，否则可以定义成uint16_t，以节省内存占用 */
#define MT_TYPE     uint16_t


/* 单块内存，内存管理所占用的全部空间大小计算公式如下：
 * size = MEMx_MAX_SIZE + (MEMx_MAX_SIZE / MEMx_BLOCK_SIZE) * sizeof(MT_TYPE)
 * 以SRAMIN为例，size = 500 * 1024 + (500 * 1024 / 64) * 2 ≈ 516KB

 * 已知总内存容量(size)，最大内存池的计算公式如下：
 * MEMx_MAX_SIZE = (MEMx_BLOCK_SIZE * size) / (MEMx_BLOCK_SIZE + sizeof(MT_TYPE))
 * 以SRAMTCM为例,MEM2_MAX_SIZE = (64 * 64) / (64 + 2) = 62.06KB ≈ 62KB
 */

/* mem1内存参数设定.mem1是内部SRAM内存. */
#define MEM1_BLOCK_SIZE         64                              /* 内存块大小为64字节 */
#define MEM1_MAX_SIZE           100 * 1024                      /* 最大管理内存100K */
#define MEM1_ALLOC_TABLE_SIZE   MEM1_MAX_SIZE/MEM1_BLOCK_SIZE   /* 内存表大小 */

/* mem2内存参数设定.mem2是内部TCMSRAM内存,此部分内存仅CPU可以访问!!!!!! */
#define MEM2_BLOCK_SIZE         64                              /* 内存块大小为64字节 */
#define MEM2_MAX_SIZE           60 * 1024                       /* 最大管理内存60K */
#define MEM2_ALLOC_TABLE_SIZE   MEM2_MAX_SIZE/MEM2_BLOCK_SIZE   /* 内存表大小 */


/* 内存管理控制器 */
struct _m_mallco_dev {
    void (*init)(uint8_t);                               /* 初始化 */
    uint16_t (*perused)(uint8_t);                        /* 内存使用率 */
    uint8_t *membase[SRAMBANK];                          /* 内存池 管理SRAMBANK个区域的内存 */
    MT_TYPE *memmap[SRAMBANK];                           /* 内存管理状态表 */
    uint8_t memrdy[SRAMBANK];                           /* 内存管理是否就绪 */
};

extern struct _m_mallco_dev mallco_dev;                  /* 在mallco.c里面定义 */

/******************************************************************************************/

/* 用户调用函数 */
void my_mem_set(void *s, uint8_t c, uint32_t count);     /* 设置内存 */
void my_mem_copy(void *des, void *src, uint32_t n);      /* 复制内存 */
void my_mem_init(uint8_t memx);                          /* 内存管理初始化函数(外/内部调用) */
uint16_t my_mem_perused(uint8_t memx);                  /* 获得内存使用率(外/内部调用) */

void myfree(uint8_t memx, void *ptr);                    /* 内存释放(外部调用) */
void *mymalloc(uint8_t memx, uint32_t size);             /* 内存分配(外部调用) */
void *myrealloc(uint8_t memx, void *ptr, uint32_t size); /* 重新分配内存(外部调用) */

#endif













