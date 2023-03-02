#include "zf_common_typedef.h"
#include "ENCODER.h"
int16 encoder_data_dl1,encoder_data_dl2,encoder_data_l;
void ENCODER_init(void)
{
    encoder_quad_init(ENCODER_DL1, ENCODER_DL1_A, ENCODER_DL1_B);   // ��ʼ��������ģ�������� �������������ģʽ
    encoder_quad_init(ENCODER_DL2, ENCODER_DL2_A, ENCODER_DL2_B);   // ��ʼ��������ģ�������� �������������ģʽ

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ��������ֵ
//  @param      void
//  @return     void��ʵ�ʸ�����encoder_data_dl1��encoder_data_dl2�У�
//  @note
//  Sample usage:       Get_Encoder()
//-------------------------------------------------------------------------------------------------------------------
vint16 L_C,R_C;
void Get_Encoder(void)
{
    encoder_data_dl1 = -encoder_get_count(ENCODER_DL1);                  // ��ȡ����������
    encoder_data_dl2 = -encoder_get_count(ENCODER_DL2);                  // ��ȡ����������
    //encoder_data_l = encoder_get_count(ENCODER_L);                  // ��ȡ����������
    encoder_clear_count(ENCODER_DL1);                                       // ��ձ���������
    encoder_clear_count(ENCODER_DL2);                                       // ��ձ���������
}
