
#include "zf_common_headfile.h"
// 接入DL1A激光测距模块 对应主板 TOF 接口 请注意线序
//      模块管脚            单片机管脚
//      SCL                 查看 zf_device_dl1a.h 中 DL1A_SCL_PIN  宏定义
//      SDA                 查看 zf_device_dl1a.h 中 DL1A_SDA_PIN  宏定义
//      VCC                 5V 电源
//      GND                 电源地
void Tof_Init(void)
{

    dl1a_init();
}
void Tof_get(void)
{
    dl1a_get_distance();
    if(dl1a_finsh_flag == 1)
    {
       dl1a_finsh_flag = 0;
//       ips114_show_uint(120, 0, dl1a_distance_mm, 4);
    }


}
