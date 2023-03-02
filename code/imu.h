#ifndef __IMU_H
#define __IMU_H
#include "zf_common_headfile.h"

#define delta_T     0.001f  //1ms¼ÆËãÒ»´Î
#define M_PI        3.1415926f

extern float imu660ra_x,imu660ra_y,imu660ra_z,imu660ra_ax,imu660ra_ay,imu660ra_az;
extern float pitch_x,roll_y,yaw_z;
void IMU660RA_get(void);
void IMU660RA_init(void);

void Complementary_Filter(void);
void Kalman_Filter(float angle_m,float gyro_m);

float invSqrt(float x);
void func(float *pitch, float *roll, float *yaw, float gx, float gy, float gz, float ax, float ay, float az, float halfT);
typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float acc_x;
    float acc_y;
    float acc_z;
} icm_param_t;


typedef struct {
    float q0;
    float q1;
    float q2;
    float q3;
} quater_param_t;


typedef struct {
    float pitch;    //¸©Ñö½Ç
    float roll;     //Æ«º½½Ç
    float yaw;       //·­¹ö½Ç
} euler_param_t;


typedef struct {
    float Xdata;
    float Ydata;
    float Zdata;
} gyro_param_t;

extern euler_param_t eulerAngle;





void ICM_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az);

void ICM_getValues();

void ICM_getEulerianAngles(void);
#endif




