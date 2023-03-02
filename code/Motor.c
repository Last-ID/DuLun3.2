#include "zf_common_headfile.h"

#define CW  1 //高电平顺时针
#define CCW 0 //低电平逆时针
int pwm;
void motor_init(void)
{
    gpio_init(DIR_FL1, GPO, 1, GPO_PUSH_PULL);
    gpio_init(DIR_FL2, GPO, 1, GPO_PUSH_PULL);
    gpio_init(STOP_FL1, GPO, 1, GPO_PUSH_PULL);
    gpio_init(STOP_FL2, GPO, 1, GPO_PUSH_PULL);
    pwm_init(PWM_FL1, 12500, 10000);                                                // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
    pwm_init(PWM_FL2, 12500, 10000);                                                // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
    //pwm_init(PWM_DL2_1, 17000, 0);                                                // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
    //pwm_init(PWM_FL2, 17000, 0);                                                // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
    //pwm_init(PWM_L1, 17000, 0);                                                // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
    //pwm_init(PWM_L2, 17000, 0);                                                // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     动量轮1的pwm控制
// 参数说明     pwm
// 返回参数     void

//-------------------------------------------------------------------------------------------------------------------
void FL1_control(int pwm)
{



    if(pwm == 0)
    {
        pwm_set_duty(PWM_FL1,10000);
        gpio_set_level(DIR_FL1,1);
    }
    else if(pwm>9000)//上限幅
    {
        gpio_set_level(DIR_FL1,1);
        pwm_set_duty(PWM_FL1,1000);

    }
    else if(pwm > 250)
    {
        gpio_set_level(DIR_FL1,1);
        pwm_set_duty(PWM_FL1,10000-pwm);
    }
    else if(pwm > 0)
    {
        gpio_set_level(DIR_FL1,1);
        pwm_set_duty(PWM_FL1,9750);
    }
    else if(pwm < -9000)
    {
        gpio_set_level(DIR_FL1,0);
        pwm_set_duty(PWM_FL1,1000);
    }
    else if(pwm < -250)
    {
        gpio_set_level(DIR_FL1,0);
        pwm_set_duty(PWM_FL1,10000+pwm);
    }
    else if(pwm < 0)
    {
        gpio_set_level(DIR_FL1,0);
        pwm_set_duty(PWM_FL1,9750);

    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     动量轮2的pwm控制
// 参数说明     pwm
// 返回参数     void

//-------------------------------------------------------------------------------------------------------------------
void FL2_control(int pwm)
{

    if(pwm == 0)
    {
        pwm_set_duty(PWM_FL2,10000);



    }
    else if(pwm>9000)//上限幅
    {
            pwm_set_duty(PWM_FL2,1000);
            gpio_set_level(DIR_FL2,1);
    }
    else if(pwm > 250)
    {
        gpio_set_level(DIR_FL2,1);
        pwm_set_duty(PWM_FL2,10000-pwm);
    }
    else if(pwm > 0)
    {
        gpio_set_level(DIR_FL2,1);
        pwm_set_duty(PWM_FL2,9750);
    }
    else if(pwm < -9000)
    {
        gpio_set_level(DIR_FL2,0);
        pwm_set_duty(PWM_FL2,1000);
    }
    else if(pwm < -250)
    {
        gpio_set_level(DIR_FL2,0);
        pwm_set_duty(PWM_FL2,10000+pwm);
    }
    else if(pwm < 0)
    {
        gpio_set_level(DIR_FL2,0);
        pwm_set_duty(PWM_FL2,9750);

    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     主轮动量轮1的pwm控制
// 参数说明     pwm
// 返回参数     void

//-------------------------------------------------------------------------------------------------------------------
void L_control(int pwm)
{
    if(pwm==0)
    {
        pwm_set_duty(PWM_L1,0);//初始化
        pwm_set_duty(PWM_L2,0);
    }
    else if(pwm>9000)//上限幅
    {
            pwm_set_duty(PWM_L1,9000);
            pwm_set_duty(PWM_L2,0);
    }
    else if(pwm>1400) //正转死区电压
    {
            pwm_set_duty(PWM_L1,pwm);
            pwm_set_duty(PWM_L2,0);
    }
    else if(pwm>0)
    {
            pwm_set_duty(PWM_L1,1400);
            pwm_set_duty(PWM_L2,0);
    }
    else if(pwm>-1400) //反转死区电压
    {
            pwm_set_duty(PWM_L1,0);
            pwm_set_duty(PWM_L2,1400);
    }
    else if(pwm>=-9000)
    {
            pwm_set_duty(PWM_L1,0);
            pwm_set_duty(PWM_L2,-pwm);
    }
    else//下限幅
    {
            pwm_set_duty(PWM_L1,0);
            pwm_set_duty(PWM_L2,9000);
    }
}
int16 pid1=0,pid2=0,pid3=0,pid4=0;
//void pwmset(int16 speed1, int16 speed2,int16 speed3,int16 speed4)//设置速度
//{
//
//
////            pid1=LocPid_Cal_Spe(&speedl1,actualspeedl1,speed1);
////            pid2=LocPid_Cal_Spe(&speedl2,actualspeedl2,speed2);
////            pid3=LocPid_Cal_Spe(&speedr1,actualspeedr1,speed3);
////            pid4=LocPid_Cal_Spe(&speedr2,actualspeedr2,speed4);
//            //Send1(actualspeedl1,actualspeedl2,actualspeedr1,actualspeedr2);
//            //printf("pid:%d,%d,%d,%d",pid1,pid2,pid3,pid4);
////            Motor_Set(pid1,pid2,pid3,pid4);
//
//}
