#include "zf_common_headfile.h"


struct Pid ang,ang_v,v;
float Move_distance;
int16 pid_v,pid_ang,pid_ang_v;

uint8 PID_init(struct Pid *pid)
{
    pid->Kp=0;
    pid->Ki=0;
    pid->Kd=0;
    pid->err=0;
    pid->err_next=0;
    pid->err_last=0;
    pid->inter=0;
    pid->value=0;
    pid->umax=0;
    pid->umin=0;
    pid->lastUd=0;

        pid->err2=0;
        pid->err_next2=0;
        pid->err_last2=0;
        pid->inter2=0;
        pid->value2=0;
        pid->umax2=0;
        pid->umin2=0;
        pid->lastUd2=0;
    return 0;
}





/***********************************************************
 * �������ƣ�X_Balance_Control
 * ����˵����X��ƽ��PID���ƣ��ǶȻ�
 * ����˵�����Ƕȡ��Ƕ���㡢���ٶ�
 * �������أ�ֱ������PMW
 ***********************************************************/
float X_Balance_Control(struct Pid *pid,float Angle,float Angle_Zero,float Gyro)
{

    float PWM,Bias;
    static float error;
    Bias=Angle-Angle_Zero;
    error+=Bias;
    if(error>120)error=120;
    else if (error<-120)error=-120;
    //error = constrain_float(error,-120,120);
    PWM=pid->Kp/100*Bias + pid->Ki/100*error + Gyro*pid->Kd/100;
    PWM = constrain_float(PWM,-9000,9000);

//    if(Start_Flag==0)PWM=0,error=0;
        return PWM;


}
/***********************************************************
 * �������ƣ�Y_Balance_Control
 * ����˵����ƽ��PID����
 * ����˵�����Ƕȡ��Ƕ���㡢���ٶ�
 * �������أ�ֱ������PWM
 ***********************************************************/
float Y_Balance_Control(struct Pid *pid,float Angle,float Angle_Zero,float Gyro)
{
    float PWM,Bias;
    static float error;
    Bias=Angle-Angle_Zero;
    error += Bias;
    error = constrain_float(error,-30,30);
    PWM=pid->Kp/100*Bias + pid->Ki/100*error + Gyro * pid->Kd/100;
//   if(Start_Flag==0)PWM=0,error=0;
    return PWM;

}
/***********************************************************
 * �������ƣ�Velocity_Control
 * ����˵�����ٶȻ�
 * ����˵����������ֵ
 * �������أ��н��ٶȿ���PWM
 ***********************************************************/
float Velocity_Control(struct Pid *pid,int encoder)
{
    static float Encoder,Encoder_Integral;
    float Velocity,Encoder_Least;

    Encoder_Least = (float)encoder-(float)Move_distance;
    Encoder *= 0.7;
    Encoder += Encoder_Least*0.3;
    Encoder_Integral += Encoder - Move_distance;
    Encoder_Integral = constrain_float(Encoder_Integral,-1300,1300);
    Velocity=Encoder * pid->Kp + Encoder_Integral * pid->Ki;
//    if(StartFlag==0)Encoder Integral=0,Encoder=0,Velocity=0;
    return Velocity;

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      λ��ʽpid����(���ٶ�)
//  @param      pid     Pid������ַ
//  @param      act     ��ǰ״ֵ̬
//  @param      set     �趨״ֵ̬
//  @return     void
//  Sample usage:       PosPid_Cal(&position,beacon_x,60);
//-------------------------------------------------------------------------------------------------------------------
int LocPid_Cal_Spe(struct Pid *pid, int16 act, int16 set)
{

    pid->err = set - act;
    pid->inter += pid->err;

    //    if((pid->err < 300) && (pid->err > -300))
    //    {
    //        pid->err=0;
    //    }

    int16 piout = pid->Ki / 1000.0 * (pid->inter);
//        if( pid->err<800||pid->err>3000)
//            if (piout > 1500 && piout < -1500)
//            {
//                    piout = 0;
//            }
//
//        if(pid->err<400&&pid->err>-400)pid->err=0;
    // piout = Int_Range_Protect(piout, -I_LIMIT, I_LIMIT);
    if(piout>1000)piout = 2000;
    else if (piout <-1000)piout = -2000;
    //    piout=0;
    pid->value = pid->Kp / 100.0 * (pid->err) + pid->Kd / 100.0 * (pid->err - pid->err_last) ;//���ٶ�piout������

    pid->err_last = pid->err;
    //pid->value = Int_Range_Protect((int32)(pid->value), -5000, 5000);

    return (int)(pid->value);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      λ��ʽpid����(�Ƕ�)
//  @param      pid     Pid������ַ
//  @param      act     ��ǰ״ֵ̬
//  @param      set     �趨״ֵ̬
//  @return     void
//  Sample usage:       PosPid_Cal(&position,beacon_x,60);
//-------------------------------------------------------------------------------------------------------------------
float LocPid_Cal_Spe2(struct Pid *pid, float act, float set)
{

    pid->err2 = set - act;
    pid->inter2 += pid->err2;


    float piout = pid->Ki / 1000.0 * (pid->inter2);



    if(piout>20||piout <-20)piout = 0;


    pid->value2 = pid->Kp / 100.0 * (pid->err2) + pid->Kd / 100.0 * (pid->err2 - pid->err_last2) + piout;

    pid->err_last2 = pid->err2;


    return (int)(pid->value2);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      λ��ʽpid����(�ٶȻ�)
//  @param      pid     Pid������ַ
//  @param      act     ��ǰ״ֵ̬
//  @param      set     �趨״ֵ̬
//  @return     void
//  Sample usage:       PosPid_Cal(&position,beacon_x,60);
//-------------------------------------------------------------------------------------------------------------------
int LocPid_Cal_Spe1(struct Pid *pid, int16 act, int16 set)
{

    pid->err = set - act;
    pid->inter += pid->err;

    if((pid->err < 200) && (pid->err > -200))
    {
        pid->err=0;
    }

    int16 piout = pid->Ki / 1000.0 * (pid->inter);
//        if( pid->err<800||pid->err>3000)
//            if (piout > 1500 && piout < -1500)
//            {
//                    piout = 0;
//            }
//
//        if(pid->err<400&&pid->err>-400)pid->err=0;
    // piout = Int_Range_Protect(piout, -I_LIMIT, I_LIMIT);
    if(piout>4000||piout <-4000)piout = 0;


    pid->value = pid->Kp / 100.0 * (pid->err) + pid->Kd / 100.0 * (pid->err - pid->err_last) + piout;

    pid->err_last = pid->err;
    pid->value = Int_Range_Protect((int32)(pid->value), -9000, 9000);

    return (int)(pid->value);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ʽpid����(�Ƕ�)
//  @param      pid     Pid������ַ
//  @param      act     ��ǰ״ֵ̬
//  @param      set     �趨ֵ
//  @return     void
//  Sample usage:           IncPid_Cal(&gyro_pid, icm_gyro_y, 0 + pidout);
//-------------------------------------------------------------------------------------------------------------------
int IncPid_Cal(struct Pid *pid, int16 act, int16 set)
{
    static double pid_temp = 0;


    pid->err_last = pid->err_next;
    pid->err_next = pid->err;
    pid->err = set - act;
    int16 piout = pid->Ki / 1000.0 * (pid->err);

    if(piout>1000)piout = 2000;
    else if (piout <-1000)piout = -2000;

    pid_temp = pid->Kp / 100.0 * (pid->err - pid->err_next) + piout + pid->Kd / 100.0 * (pid->err - 2 * pid->err_next + pid->err_last);
//    if (( pid_temp > 500) )
//            pid_temp = 500;
//    else if( pid_temp < -500)
//            pid_temp = -500;
    pid->value += pid_temp;

  //pid->value = Int_Range_Protect((int32)(pid->value), -70, 70);

    return (int)(pid->value);
}

int32 Int_Range_Protect(int32 duty, int32 min, int32 max)
{
    if (duty >= max)
    {
        return max;
    }
    if (duty < min)
    {
        return min;
    }
    else
    {
        return duty;
    }
}
