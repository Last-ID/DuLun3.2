#include "zf_common_headfile.h"
uint16 ips_data[128];

void IPS_init(void)
{
    gpio_init(KEY1, GPI, GPIO_LOW, GPI_PULL_DOWN);
    gpio_init(KEY2, GPI, GPIO_LOW, GPI_PULL_DOWN);
    gpio_init(KEY3, GPI, GPIO_LOW, GPI_PULL_DOWN);
    gpio_init(KEY4, GPI, GPIO_LOW, GPI_PULL_DOWN);

    ips114_set_dir(IPS114_CROSSWISE_180);
    ips114_set_color(RGB565_RED, RGB565_BLACK);
    ips114_init();
}
extern int pwm;

void IPS_show(void)
{
    ips114_show_string( 0 , 16*1,   "pitch_x:");
    ips114_show_float(  8*14 , 16*1,   pitch_x,     3, 2);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
    ips114_show_string( 0 , 16*2,   "roll_y:");
    ips114_show_float(  8*14 , 16*2,   roll_y, 3, 2);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
    ips114_show_string( 0 , 16*3,   "yaw_z:");
    ips114_show_float(  8*14 , 16*3,   yaw_z,     3, 2);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
    ips114_show_string( 0 , 16*4,   "DL1:");
    ips114_show_int(  8*14 , 16*4,   encoder_data_dl1*60,     5);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
    ips114_show_string( 0 , 16*5,   "DL2:");
    ips114_show_int(  8*14 , 16*5,   encoder_data_dl2*60,     5);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位

    ips114_show_string( 0 , 16*6,   "pwm:");
    ips114_show_int(8*14 , 16*6,   pwm,     3);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位

}
uint8 page=0;
//key 按下低电平
Menu PID_menu[50];//30
PMenu menu_pointer = &PID_menu[0];
uint16 Row = 0;
uint8 showMenuFlag = 0;     // Menu mode
uint8 menu_button_flag = 0; //每次按键刷新全屏

extern uint8 startflag;
void Key_operation(void)
{
      if(!gpio_get_level(KEY1))
      {
            system_delay_ms(100);
            if(!gpio_get_level(KEY1))
            {
                startflag = 1;
                gpio_set_level(STOP_FL1,1);
                gpio_set_level(STOP_FL2,1);
                ips114_clear();
                showMenuFlag = 0;
                page++; //翻页
                if (page == 3)//超过2页回到0页
                 page = 0;
            }
      }
      if(!gpio_get_level(KEY2))
      {
          system_delay_ms(100);
          if(!gpio_get_level(KEY2))
          {

            if(showMenuFlag == 1)
            {
                  if(menu_pointer->down != NULL)
                        menu_pointer = menu_pointer->down;
                  else menu_pointer = menu_pointer-9;//地址放到头地址
            }
            else
            showMenuFlag = 1;
          }
      }
      if(!gpio_get_level(KEY4)&&!gpio_get_level(KEY3))
            {
                system_delay_ms(100);
                if(!gpio_get_level(KEY4)&&!gpio_get_level(KEY3))
                {
                    flash_write1();
                    flash_read();
                }
            }
      if(!gpio_get_level(KEY3)&&gpio_get_level(KEY4))
      {
          system_delay_ms(100);
          if(!gpio_get_level(KEY3)&&gpio_get_level(KEY4))
          {

            if (*(menu_pointer->value) > 0)
            *(menu_pointer->value) = *(menu_pointer->value) - 1;

          }
      }

      if(!gpio_get_level(KEY4)&&gpio_get_level(KEY3))
      {
          system_delay_ms(50);
          if(!gpio_get_level(KEY4)&&gpio_get_level(KEY3))
          {
            if (*(menu_pointer->value) < 65535)
            *(menu_pointer->value) = *(menu_pointer->value) + 1;

          }
      }



}
void page_show(void)
{

        switch (page)
        {
            case 0:
            {
                ips114_show_string( 0 , 16*1,   "pitch_x:");
                ips114_show_float(  8*14 , 16*1,   pitch_x,     3, 6);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
                ips114_show_string( 0 , 16*2,   "roll_y:");
                ips114_show_float(  8*14 , 16*2,   roll_y, 3, 6);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
                ips114_show_string( 0 , 16*3,   "yaw_z:");
                ips114_show_float(  8*14 , 16*3,   yaw_z,     3, 6);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
                ips114_show_string( 0 , 16*4,   "DL1:");
                ips114_show_float(  8*14 , 16*4,   encoder_data_dl1*60,     3, 6);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
                ips114_show_string( 0 , 16*5,   "DL2:");
                ips114_show_float(  8*14 , 16*5,   encoder_data_dl2*60,     3, 6);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位

                ips114_show_string( 0 , 16*6,   "pwm:");
                ips114_show_int(  8*14 , 16*6,   pwm,     5);                  // 显示 float 数据 1bit 整数 6bit 小数 应当显示 -3.141592 总共会有 9 个字符的显示占位
                break;
            }
            case 1:
            {
                ips114_show_string(8*0, 0, "ang_v.kp:");
                ips114_show_int(5*16, 0, ang_v.Kp, 3);
                ips114_show_string(0, 16*1, "ang_v.ki:");
                ips114_show_int(5*16, 16*1, ang_v.Ki, 3);
                ips114_show_string(0, 16*2, "ang_v.kd:");
                ips114_show_int(5*16, 16*2, ang_v.Kd, 3);
                ips114_show_string(0, 16*3, "ang.kp:");
                ips114_show_int(4*16, 16*3, ang.Kp, 3);
                ips114_show_string(0, 16*4, "ang.ki:");
                ips114_show_int(4*16, 16*4, ang.Ki, 3);
                ips114_show_string(0, 16*5, "ang.kd:");
                ips114_show_int(4*16, 16*5, ang.Kd, 3);
                ips114_show_string(8*16, 16*0, "v.kp:");
                ips114_show_int(11*16, 16*0, v.Kp, 3);
                ips114_show_string(8*16, 16*1, "v.ki:");
                ips114_show_int(11*16, 16*1,  v.Ki, 3);
                ips114_show_string(8*16, 16*2, "v:kd");
                ips114_show_int(11*16, 16*2, v.Kd, 3);
                ips114_show_string(6*16, 16*3, "canshu:");
                ips114_show_int(12*16, 16*3, canshu, 3);

                break;
            }
            case 2:
            {

                ips114_show_gray_image(10, 10, ipsdbimage[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
                for(int i=0;i<57;i++)
                {
                    if(midlinex[i]+10>0&&midlinex[i]+10<240)
                    ips114_draw_point(midlinex[i]+10, i+10, RGB565_PURPLE);
                    ips114_draw_point(edgeL[i]+10, i+10, RGB565_RED);
                    ips114_draw_point(edgeR[i]+10, i+10, RGB565_CYAN);
                }


            }

        }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化条目
//  @param      destination             此条目（或者说指向此条目的指针）
//  @param      Up                      上指针
//  @param      Down                    下指针
//  @param      Left                    左指针
//  @param      Right                   右指针
//  @param      Name                    条目名称
//  @param      p                       条目值（实际为指向变量的指针）
//  @return     void
//  Sample usage:       Menu_Item_Init(&PID_menu[3], &PID_menu[2], &PID_menu[4], "Pos_Kp", &Pos_Kp);
//  note:
//-------------------------------------------------------------------------------------------------------------------
void Menu_Item_Init(PMenu destination, PMenu Up, PMenu Down, char* Name,uint32 *p)
{
    destination->up = Up;
    destination->down = Down;
    destination->name = Name;
    destination->value = p;
}

void Menu_Init(void)
{
    Menu_Item_Init(&PID_menu[0], NULL, &PID_menu[1], "ang_v.Kp", &ang_v.Kp);
    Menu_Item_Init(&PID_menu[1], &PID_menu[0], &PID_menu[2], "ang_v.Ki", &ang_v.Ki);
    Menu_Item_Init(&PID_menu[2], &PID_menu[1], &PID_menu[3], "ang_v.Kd", &ang_v.Kd);
    Menu_Item_Init(&PID_menu[3], &PID_menu[2], &PID_menu[4], "ang.Kp", &ang.Kp);
    Menu_Item_Init(&PID_menu[4], &PID_menu[3], &PID_menu[5], "ang.Ki", &ang.Ki);
    Menu_Item_Init(&PID_menu[5], &PID_menu[4], &PID_menu[6], "ang.Kd", &ang.Kd);
    Menu_Item_Init(&PID_menu[6], &PID_menu[5], &PID_menu[7], "v.Kp", &v.Kp);
    Menu_Item_Init(&PID_menu[7], &PID_menu[6], &PID_menu[8], "v.Ki", &v.Ki);
    Menu_Item_Init(&PID_menu[8], &PID_menu[7], &PID_menu[9], "v.Kd", &v.Kd);
    Menu_Item_Init(&PID_menu[9], &PID_menu[8], NULL, "canshu", &canshu);

}
