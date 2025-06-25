#include "driver_tf.h"
#include "driver_usart.h"

FATFS *fs;
FIL file;
DIR dir;
FILINFO *fno;
FRESULT res;
UINT num_of_written, num_of_read;
BYTE work[FF_MAX_SS];
uint32_t tf_memory = 0;

char *dirs[4] = {"sample", "overLimit", "log", "hideData"};

void TF_Init(void) {
    exfuns_init();
    res = f_mount(fs, "", 1);
    if (res == FR_OK) {
        tf_memory = sd_card_capacity_get();
        for (uint8_t i = 0; i < 4; i++) {
            TF_Init_Dir(dirs[i]);
        }
    } else {
        if (res == FR_NO_FILESYSTEM) {
            res = f_mkfs("", 0, work, sizeof work);
            if (res == FR_OK) {
//                printf("mkfs done\r\n");
            } else {
//                printf("f_mkfs error code:%d\r\n", res);
            }
        }
    }
}

uint32_t TF_Read_Memory_Size(void) {
    return tf_memory;
}

void TF_Write_File(char *filename, char *text, BYTE mode) {
    res = f_open(&file, filename, mode);
    if (res == FR_OK) {
        res = f_write(&file, text, strlen(text), &num_of_written);
        if (res == FR_OK) {
//            printf("write done\r\n");
        } else {
//            printf("write error code:%d\r\n", res);
        }
    } else {
//        printf("f_write error code:%d\r\n", res);
    }
    f_close(&file);
}

void TF_Read_File(char *filename, char *text, BYTE mode) {
    res = f_open(&file, filename, mode);
    if (res == FR_OK) {
        res = f_read(&file, text, strlen(text), &num_of_read);
        if (res == FR_OK) {
//            printf("read done\r\n");
        } else {
//            printf("f_read error code:%d\r\n", res);
        }
    } else {
//        printf("f_read error code:%d\r\n", res);
    }
    f_close(&file);
}

void TF_Make_Dir(char *dirname) {
    res = f_mkdir(dirname);
    if (res == FR_OK) {
//        printf("mkdir done\r\n");
    } else {
//        printf("f_mkdir error code:%d\r\n", res);
    }
}

void TF_Init_Dir(char *dirname) {
    res = f_stat(dirname, fno);
    if (res == FR_OK) {
    } else {
        TF_Make_Dir(dirname);
    }
}

