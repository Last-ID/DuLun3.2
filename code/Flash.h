#ifndef __FLASH_H
#define __FLASH_H
#include "zf_common_headfile.h"

#define FLASH_SECTION_INDEX       (0)                           // �洢�����õ����� ֻ����0
#define FLASH_PAGE_INDEX          (11)                                // �洢�����õ�ҳ�� ������һ��ҳ��
#define DATA_SIZE       1024
void Param_Cfg(void);
void flash_go(void);
void flash_write(void);
void flash_read(void);
void flash_write1(void);
#endif






