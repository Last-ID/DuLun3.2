#ifndef __TOF_H
#define __TOF_H
#include "zf_common_headfile.h"
// ����DL1A������ģ�� ��Ӧ���� TOF �ӿ� ��ע������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      SCL                 �鿴 zf_device_dl1a.h �� DL1A_SCL_PIN  �궨��
//      SDA                 �鿴 zf_device_dl1a.h �� DL1A_SDA_PIN  �궨��
//      VCC                 5V ��Դ
//      GND                 ��Դ��

void Tof_Init(void);
void Tof_get(void);

#endif




