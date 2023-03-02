#include "zf_common_headfile.h"
//-------------------------------------------------------------------------------------------------------------------
//  @brief      陀螺仪初始化
//  @param      void
//  @return     void
//  Sample usage:       IMU660RA_get()
//-------------------------------------------------------------------------------------------------------------------
void IMU660RA_init(void)
{
    imu660ra_init();
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      陀螺仪获取角度，加速度
//  @param      void
//  @return     void
//  Sample usage:       IMU660RA_get()
//-------------------------------------------------------------------------------------------------------------------
float imu660ra_x,imu660ra_y,imu660ra_z,//角速度
imu660ra_ax,imu660ra_ay,imu660ra_az;//角加速度
//加速度x←↑y
//pitch↓→roll
void IMU660RA_get(void)
{
    imu660ra_get_acc();
    imu660ra_get_gyro();
    imu660ra_ax = imu660ra_acc_transition(imu660ra_acc_x);
    imu660ra_ay = imu660ra_acc_transition(imu660ra_acc_y);
    imu660ra_az = imu660ra_acc_transition(imu660ra_acc_z);
    imu660ra_x = imu660ra_gyro_transition(imu660ra_gyro_x);
    imu660ra_y = imu660ra_gyro_transition(imu660ra_gyro_y);
    imu660ra_z = imu660ra_gyro_transition(imu660ra_gyro_z);
}

int16 angle = 0;       //滤波后角度
int16 angle_final = 0; //换算后角度

float inVSqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int*)&x; // get bits for floating value
    i = 0x5f375a86 - (i>>1); // gives initial guess
    x = *(float*)&i; // convert bits back to float
    x = x * (1.5f - xhalf*x*x); // Newton step
    return x;
}
/*
    @brief
    @param[in] gx gy gz 为各轴角速度，单位为rad/s
    @param[in] ax ay az 为各轴加速度，单位为m/s^2
    @param[in] halfT 为更新周期的一半，单位为s
    @param[out] pitch roll yaw 为当前欧拉角，单位为度
*/
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;
float q0temp, q1temp, q2temp, q3temp;
float vx, vy, vz;
float ex, ey, ez;
float ix = 0, iy = 0, iz = 0;
float kp = 1, ki = 0;
float pitch_x = 0,roll_y = 0,yaw_z =0;
void func(float *pitch, float *roll, float *yaw, float gx, float gy, float gz, float ax, float ay, float az, float halfT)
{
    float norm;

    if(ax * ay *az != 0)
    {
        /* 归一化加速度 */
        norm = inVSqrt(ax*ax + ay*ay + az*az);
        ax = ax * norm;
        ay = ay * norm;
        az = az * norm;
        /* 计算当前各轴加速度 */
        vx = 2*(q1*q3 - q0*q2);
        vy = 2*(q0*q1 + q2*q3);
        vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
        /* 计算加速度正交 */
        ex = (ay*vz - az*vy) ;
        ey = (az*vx - ax*vz) ;
        ez = (ax*vy - ay*vx) ;
        /* 融合 */
        ix += ex;
        iy += ey;
        iz += ez;
        gx = gx + kp*ex + ki*ix;
        gy = gy + kp*ey + ki*iy;
        gz = gz + kp*ez + ki*iz;
    }
    q0temp=q0;
    q1temp=q1;
    q2temp=q2;
    q3temp=q3;

    q0 = q0temp + (-q1temp*gx - q2temp*gy - q3temp*gz)*halfT;
    q1 = q1temp + ( q0temp*gx + q2temp*gz - q3temp*gy)*halfT;
    q2 = q2temp + ( q0temp*gy - q1temp*gz + q3temp*gx)*halfT;
    q3 = q3temp + ( q0temp*gz + q1temp*gy - q2temp*gx)*halfT;

    norm = inVSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;

    *roll = atan2(2 * (q2 * q3 + q0 * q1), q0*q0 - q1*q1 - q2*q2 + q3*q3)* 57.295773f;
    *pitch = -asin(2 * (q1 * q3 - q0 * q2))*57.295773f;
    *yaw =  atan2(2 * (q1 * q2 - q0 * q3), q0*q0 + q1*q1 - q2*q2 - q3*q3)*57.295773f;
    if(*roll>0)*roll = 180 - *roll;
    else if (*roll<0)*roll = -*roll - 180;
}


float I_ex, I_ey, I_ez;  // 误差积分

quater_param_t Q_info = {1, 0, 0};  // 全局四元数
euler_param_t eulerAngle; //欧拉角

icm_param_t icm_data;
gyro_param_t GyroOffset;

bool GyroOffset_init = 0;

float param_Kp = 0.17;   // 加速度计的收敛速率比例增益
float param_Ki = 0.004;   //陀螺仪收敛速率的积分增益 0.004




//-------------------------------------------------------------------------------------------------------------------
//  @brief      一阶互补滤波
//  @param      void
//  @return     void
//  @note
//  Sample usage:       Complementary_Filter();
//-------------------------------------------------------------------------------------------------------------------
void Complementary_Filter(void)
{
    static float p1 = 0.0800000, p2 = -0.01300000;
    angle = p1 * icm20602_acc_x + (1 - p1) * (angle + p2 * icm20602_gyro_y); //直立 ace x gyr y
    // angle -1000（抬头）到 -4000（低头）
    angle_final = angle + 4500;
    // angle_final 3800（抬头）1600（平衡）940（低头）
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      二阶卡尔曼滤波融合
//  @param      angle_m   角度
//  @param      gyro_m    角速度
//  @return     void
//  @note       卡尔曼滤波，得到最优估计的角度。
//  Sample usage:       IMU_Init();
//-------------------------------------------------------------------------------------------------------------------
void Kalman_Filter(float angle_m, float gyro_m)
{
    const float Q_angle = 0.001, Q_gyro = 0.001, R_angle = 5, dt = 0.005;
    //注意：dt的取值为kalman滤波器采样时间;Q增大，动态响应增大
    static float P[2][2] = {{1, 0}, {0, 1}};
    static float Pdot[4] = {0, 0, 0, 0};
    static float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
    angle += (gyro_m - q_bias) * dt;
    Pdot[0] = Q_angle - P[0][1] - P[1][0];
    Pdot[1] = -P[1][1];
    Pdot[2] = -P[1][1];
    Pdot[3] = Q_gyro;
    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;
    angle_err = angle_m - angle;
    PCt_0 = P[0][0];
    PCt_1 = P[1][0];
    E = R_angle + PCt_0;
    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;
    t_0 = PCt_0;
    t_1 = P[0][1];
    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;
    angle += K_0 * angle_err; //最终融合角度
    q_bias += K_1 * angle_err;
    //   angle_speed = gyro_m - q_bias;
}




