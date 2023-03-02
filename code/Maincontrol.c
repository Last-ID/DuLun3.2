#include "zf_common_headfile.h"

void init_all(void)
{
//电机
    motor_init();
//编码器
    ENCODER_init();
    pit_ms_init(CCU60_CH0 , 20);
//陀螺仪
    IMU660RA_init();
    pit_ms_init(CCU61_CH0, 20);
//PID
    PID_init(&ang);
    PID_init(&ang_v);
    PID_init(&v);
    pit_ms_init(CCU60_CH1, 2);
//UART
    wireless_uart_init();
//FLASH
    flash_go();
//IPS、KEY
    IPS_init();
    Menu_Init();
//TOF
    Tof_Init();
}
uint8 startflag=0;
void Control(void)
{
    if(startflag==1)//开始
    {
        FL1_control(-pwm);//16-1000  35-2000  54-3000 90-5000
        FL2_control(pwm);
    }
    else//刹车
    {
        gpio_set_level(STOP_FL1, 0);
        gpio_set_level(STOP_FL2,0);
    }


  //*************
    if(mt9v03x_finish_flag)
    {
        get_route();

        mt9v03x_finish_flag=0;
    }
}
void get_route(void)
{
    db_image(0);//mode：0 优化大津1 固定阈值2 原始大津
    edge_line(dbimage);//左右边线
    GetBlackEndParam();//寻黑线截止行
    get_route_endline();//截止行判断
//**
    if(LeftHDflag==0&&RightHDflag==0)
    {
        Ring_find();
    }

    cross_find();

    //元素处理
    if(RightHDflag==1)//识别到右环岛后
    {
        rightring();
    }
    else if(LeftHDflag == 1)//左
    {
        leftring();
    }
    else if(crossflag == 1)
    {
        crossroad();
    }
    GetMidline(dbimage);
//***




}

int constrain_int(int a,int b,int c)
{
    if(a<=b)
        return b;
    else if(a>=c)
        return c;
    else
        return a;
}
float constrain_float(float a,float b,float c)
{
    if(a<=b)
        return b;
    else if(a>=c)
        return c;
    else
        return a;
}
double constrain_double(double a,double b,double c)
{
    if(a<=b)
        return b;
    else if(a>=c)
        return c;
    else
        return a;


}
