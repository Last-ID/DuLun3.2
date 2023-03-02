#ifndef __PID_H
#define __PID_H
#include "zf_common_headfile.h"


struct Pid{

    uint32 Kp;
    uint32 Ki;
    uint32 Kd;
    int err;
    int err_next;
    int err_last;
    int inter;
    int value;
    int umax;
    int umin;
    int lastUd;
    float err2;
    float err_next2;
    float err_last2;
    float inter2;
    float value2;
    float umax2;
    float umin2;
    float lastUd2;

};
extern struct Pid ang,ang_v,v;
extern int16 pid_v,pid_ang_v,pid_ang;
float X_Balance_Control(struct Pid *pid,float Angle,float Angle_Zero,float Gyro);
float Y_Balance_Control(struct Pid *pid,float Angle,float Angle_Zero,float Gyro);
float Velocity_Control(struct Pid *pid,int encoder);
int IncPid_Cal(struct Pid *pid, int16 act, int16 set);
int LocPid_Cal_Spe(struct Pid *pid, int16 act, int16 set);
int LocPid_Cal_Spe1(struct Pid *pid, int16 act, int16 set);
float LocPid_Cal_Spe2(struct Pid *pid, float act, float set);
int32 Int_Range_Protect(int32 duty, int32 min, int32 max);



uint8 PID_init(struct Pid *pid);


#endif




