#include "zf_common_headfile.h"
//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����ǳ�ʼ��
//  @param      void
//  @return     void
//  Sample usage:       IMU660RA_get()
//-------------------------------------------------------------------------------------------------------------------
void IMU660RA_init(void)
{
    imu660ra_init();
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����ǻ�ȡ�Ƕȣ����ٶ�
//  @param      void
//  @return     void
//  Sample usage:       IMU660RA_get()
//-------------------------------------------------------------------------------------------------------------------
float imu660ra_x,imu660ra_y,imu660ra_z,//���ٶ�
imu660ra_ax,imu660ra_ay,imu660ra_az;//�Ǽ��ٶ�
//���ٶ�x����y
//pitch����roll
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

int16 angle = 0;       //�˲���Ƕ�
int16 angle_final = 0; //�����Ƕ�

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
    @param[in] gx gy gz Ϊ������ٶȣ���λΪrad/s
    @param[in] ax ay az Ϊ������ٶȣ���λΪm/s^2
    @param[in] halfT Ϊ�������ڵ�һ�룬��λΪs
    @param[out] pitch roll yaw Ϊ��ǰŷ���ǣ���λΪ��
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
        /* ��һ�����ٶ� */
        norm = inVSqrt(ax*ax + ay*ay + az*az);
        ax = ax * norm;
        ay = ay * norm;
        az = az * norm;
        /* ���㵱ǰ������ٶ� */
        vx = 2*(q1*q3 - q0*q2);
        vy = 2*(q0*q1 + q2*q3);
        vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
        /* ������ٶ����� */
        ex = (ay*vz - az*vy) ;
        ey = (az*vx - ax*vz) ;
        ez = (ax*vy - ay*vx) ;
        /* �ں� */
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


float I_ex, I_ey, I_ez;  // ������

quater_param_t Q_info = {1, 0, 0};  // ȫ����Ԫ��
euler_param_t eulerAngle; //ŷ����

icm_param_t icm_data;
gyro_param_t GyroOffset;

bool GyroOffset_init = 0;

float param_Kp = 0.17;   // ���ٶȼƵ��������ʱ�������
float param_Ki = 0.004;   //�������������ʵĻ������� 0.004




//-------------------------------------------------------------------------------------------------------------------
//  @brief      һ�׻����˲�
//  @param      void
//  @return     void
//  @note
//  Sample usage:       Complementary_Filter();
//-------------------------------------------------------------------------------------------------------------------
void Complementary_Filter(void)
{
    static float p1 = 0.0800000, p2 = -0.01300000;
    angle = p1 * icm20602_acc_x + (1 - p1) * (angle + p2 * icm20602_gyro_y); //ֱ�� ace x gyr y
    // angle -1000��̧ͷ���� -4000����ͷ��
    angle_final = angle + 4500;
    // angle_final 3800��̧ͷ��1600��ƽ�⣩940����ͷ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���׿������˲��ں�
//  @param      angle_m   �Ƕ�
//  @param      gyro_m    ���ٶ�
//  @return     void
//  @note       �������˲����õ����Ź��ƵĽǶȡ�
//  Sample usage:       IMU_Init();
//-------------------------------------------------------------------------------------------------------------------
void Kalman_Filter(float angle_m, float gyro_m)
{
    const float Q_angle = 0.001, Q_gyro = 0.001, R_angle = 5, dt = 0.005;
    //ע�⣺dt��ȡֵΪkalman�˲�������ʱ��;Q���󣬶�̬��Ӧ����
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
    angle += K_0 * angle_err; //�����ںϽǶ�
    q_bias += K_1 * angle_err;
    //   angle_speed = gyro_m - q_bias;
}




