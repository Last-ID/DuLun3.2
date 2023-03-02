#include "zf_common_headfile.h"

void init_all(void)
{
//���
    motor_init();
//������
    ENCODER_init();
    pit_ms_init(CCU60_CH0 , 20);
//������
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
//IPS��KEY
    IPS_init();
    Menu_Init();
//TOF
    Tof_Init();
}
uint8 startflag=0;
void Control(void)
{
    if(startflag==1)//��ʼ
    {
        FL1_control(-pwm);//16-1000  35-2000  54-3000 90-5000
        FL2_control(pwm);
    }
    else//ɲ��
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
    db_image(0);//mode��0 �Ż����1 �̶���ֵ2 ԭʼ���
    edge_line(dbimage);//���ұ���
    GetBlackEndParam();//Ѱ���߽�ֹ��
    get_route_endline();//��ֹ���ж�
//**
    if(LeftHDflag==0&&RightHDflag==0)
    {
        Ring_find();
    }

    cross_find();

    //Ԫ�ش���
    if(RightHDflag==1)//ʶ���һ�����
    {
        rightring();
    }
    else if(LeftHDflag == 1)//��
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
