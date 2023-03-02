#ifndef __FLASH_H
#define __FLASH_H
#include "zf_common_headfile.h"

#define FLASH_SECTION_INDEX       (0)                           // 存储数据用的扇区 只能填0
#define FLASH_PAGE_INDEX          (11)                                // 存储数据用的页码 倒数第一个页码
#define DATA_SIZE       1024
void Param_Cfg(void);
void flash_go(void);
void flash_write(void);
void flash_read(void);
void flash_write1(void);
#endif






