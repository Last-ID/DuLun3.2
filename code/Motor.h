#ifndef __MOTOR_H
#define __MOTOR_H
#include "zf_common_headfile.h"
extern int pwm;

void motor_init(void);
void FL1_control(int pwm);
void FL2_control(int pwm);
void L_control(int pwm);


#endif




