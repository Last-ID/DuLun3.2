
#include "zf_common_headfile.h"
// ����DL1A������ģ�� ��Ӧ���� TOF �ӿ� ��ע������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      SCL                 �鿴 zf_device_dl1a.h �� DL1A_SCL_PIN  �궨��
//      SDA                 �鿴 zf_device_dl1a.h �� DL1A_SDA_PIN  �궨��
//      VCC                 5V ��Դ
//      GND                 ��Դ��
void Tof_Init(void)
{

    dl1a_init();
}
void Tof_get(void)
{
    dl1a_get_distance();
    if(dl1a_finsh_flag == 1)
    {
       dl1a_finsh_flag = 0;
//       ips114_show_uint(120, 0, dl1a_distance_mm, 4);
    }


}
