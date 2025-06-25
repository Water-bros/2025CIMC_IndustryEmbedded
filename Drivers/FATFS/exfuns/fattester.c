#include "malloc.h"
#include "driver_usart.h"
#include "exfuns.h"
#include "fattester.h"

_m_fattester fattester;

uint8_t mf_init(void)
{
    fattester.file = (FIL *)mymalloc(SRAMIN, sizeof(FIL));
    fattester.fatbuf = (uint8_t *)mymalloc(SRAMIN, 512);

    if (fattester.file && fattester.fatbuf)
    {
        return 0;
    }
    else
    {
        mf_free();
        return 1;
    }
}

void mf_free(void)
{
    myfree(SRAMIN, fattester.file);
    myfree(SRAMIN, fattester.fatbuf);
}