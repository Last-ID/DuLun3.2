#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    cpu_wait_event_ready();         // 等待所有核心初始化完毕


    init_all();
    while (TRUE)
    {
        IMU660RA_get();
        func(&pitch_x, &roll_y, &yaw_z, imu660ra_x/57.295773, imu660ra_y/57.295773, imu660ra_z/57.295773, imu660ra_ax, imu660ra_ay, imu660ra_az, 0.01);

        Key_operation();
        page_show();
        Control();

    }
}
#pragma section all restore


