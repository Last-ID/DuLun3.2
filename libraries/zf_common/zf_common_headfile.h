#ifndef _zf_common_headfile_h_
#define _zf_common_headfile_h_

//===================================================C语言 函数库===================================================
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
//===================================================C语言 函数库===================================================

//===================================================芯片 SDK 底层===================================================
#include "ifxAsclin_reg.h"
#include "SysSe/Bsp/Bsp.h"
#include "IfxCcu6_Timer.h"
#include "IfxScuEru.h"
//===================================================芯片 SDK 底层===================================================

//====================================================开源库公共层====================================================
#include "zf_common_typedef.h"
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_common_interrupt.h"
//====================================================开源库公共层====================================================

//===================================================芯片外设驱动层===================================================
#include "zf_driver_adc.h"
#include "zf_driver_delay.h"
#include "zf_driver_dma.h"
#include "zf_driver_encoder.h"
#include "zf_driver_exti.h"
#include "zf_driver_flash.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pit.h"
#include "zf_driver_pwm.h"
#include "zf_driver_soft_iic.h"
#include "zf_driver_spi.h"
#include "zf_driver_soft_spi.h"
#include "zf_driver_uart.h"
#include "zf_driver_timer.h"
//===================================================芯片外设驱动层===================================================

//===================================================外接设备驱动层===================================================
#include "zf_device_absolute_encoder.h"
#include "zf_device_bluetooth_ch9141.h"
#include "zf_device_dl1a.h"
#include "zf_device_gps_tau1201.h"
#include "zf_device_camera.h"
#include "zf_device_icm20602.h"
#include "zf_device_imu660ra.h"
#include "zf_device_imu963ra.h"
#include "zf_device_ips114.h"
#include "zf_device_ips200.h"
#include "zf_device_key.h"
#include "zf_device_mpu6050.h"
#include "zf_device_mt9v03x.h"
#include "zf_device_oled.h"
#include "zf_device_ov7725.h"
#include "zf_device_scc8660.h"
#include "zf_device_tft180.h"
#include "zf_device_tsl1401.h"
#include "zf_device_type.h"
#include "zf_device_virtual_oscilloscope.h"
#include "zf_device_wifi_uart.h"
#include "zf_device_wireless_uart.h"
//===================================================外接设备驱动层===================================================
#define LED2                    (P20_8)
#define LED3                    (P21_5)
#define LED4                    (P21_4)

#define KEY1                    (P20_6)
#define KEY2                    (P20_7)
#define KEY3                    (P20_8)
#define KEY4                    (P20_9)

#define SWITCH1                 (P33_12)
#define SWITCH2                 (P33_13)


#define DIR_FL1                     (P02_4)
#define DIR_FL2                     (P02_6)
#define STOP_FL1                     (P11_2)
#define STOP_FL2                     (P11_3)

#define PWM_FL1                 (ATOM0_CH5_P02_5)
#define PWM_FL2                 (ATOM0_CH7_P02_7)
#define PWM_L1                    (ATOM1_CH4_P02_4)
#define PWM_L2                    (ATOM1_CH5_P02_5)


#include "Camera.h"
#include "Encoder.h"
#include "Flash.h"
#include "imu.h"
#include "Ips.h"
#include "Led.h"
#include "Maincontrol.h"
#include "Motor.h"
#include "Pid.h"
#include "Uart.h"
#include "Tof.h"
#include "Ring.h"
#include "Cross.h"
#endif

