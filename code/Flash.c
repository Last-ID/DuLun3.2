#include "zf_common_headfile.h"

uint32 flash_buffer[DATA_SIZE] = {0};
uint32 flash_flag = 1,flag;

void Param_Cfg(void)
{
    ang.Kp = 209;
    ang.Ki = 0;
    ang.Kd = 84;

    ang_v.Kp = 209;
    ang_v.Ki = 0;
    ang_v.Kd = 84;

    v.Kp = 127;
    v.Ki = 42;
    v.Kd = 0;

    canshu = 150;

}

void flash_write(void)
{
        Param_Cfg();
        flash_buffer[0] = ang.Kp;
        flash_buffer[1] = ang.Ki;
        flash_buffer[2] = ang.Kd;
        flash_buffer[3] = ang_v.Kp;
        flash_buffer[4] = ang_v.Ki;
        flash_buffer[5] = ang_v.Kd;
        flash_buffer[6] = v.Kp;
        flash_buffer[7] = v.Ki;
        flash_buffer[8] = v.Kd;
        flash_buffer[9] = canshu;

        flash_write_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX, flash_buffer, 256);

}
void flash_write1(void)
{
        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
        flash_buffer[0] = ang.Kp;
        flash_buffer[1] = ang.Ki;
        flash_buffer[2] = ang.Kd;
        flash_buffer[3] = ang_v.Kp;
        flash_buffer[4] = ang_v.Ki;
        flash_buffer[5] = ang_v.Kd;
        flash_buffer[6] = v.Kp;
        flash_buffer[7] = v.Ki;
        flash_buffer[8] = v.Kd;
        flash_buffer[9] = canshu;

        flash_write_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX, flash_buffer, 256);
        printf("write1");

}
void flash_read(void)
{

        ang.Kp = flash_buffer[0] ;
        ang.Ki = flash_buffer[1];
        ang.Kd = flash_buffer[2];
        ang_v.Kp = flash_buffer[3];
        ang_v.Ki = flash_buffer[4];
        ang_v.Kd = flash_buffer[5];
        v.Kp = flash_buffer[6];
        v.Ki = flash_buffer[7];
        v.Kd = flash_buffer[8];
        canshu = flash_buffer[9];

}
//每次烧录都会清掉flash，烧录后flash存入数据，再复位可以通过串口看到存储的数据
void flash_go(void)
{
        if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))//flash有数据 就把数据存入缓冲区数组
        {
            flag=1;
            flash_read_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX, flash_buffer, 1024);
            flash_read();

        }
        else //flash没有数据 就把代码里的值赋给它
        {

            flash_erase_page(0, FLASH_PAGE_INDEX);
            flash_write();
            flash_write_page(0, FLASH_PAGE_INDEX, flash_buffer, 1024);
        }

//      if(flag==1)//已存入数据
//      {
//          for(i=0;i<20;i++)
//          {
//              printf("%d\n",flash_buffer[i]);
//          }
//      }
//      else printf("nothing");
}
