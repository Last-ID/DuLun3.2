#ifndef __MAINCONTROL_H
#define __MAINCONTROL_H
#include "zf_common_headfile.h"
void init_all(void);

int constrain_int(int a,int b,int c);
float constrain_float(float a,float b,float c);
double constrain_double(double a,double b,double c);
void Control(void);

void get_route(void);
#endif




