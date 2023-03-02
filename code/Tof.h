#ifndef __TOF_H
#define __TOF_H
#include "zf_common_headfile.h"
// 接入DL1A激光测距模块 对应主板 TOF 接口 请注意线序
//      模块管脚            单片机管脚
//      SCL                 查看 zf_device_dl1a.h 中 DL1A_SCL_PIN  宏定义
//      SDA                 查看 zf_device_dl1a.h 中 DL1A_SDA_PIN  宏定义
//      VCC                 5V 电源
//      GND                 电源地

void Tof_Init(void);
void Tof_get(void);

#endif




