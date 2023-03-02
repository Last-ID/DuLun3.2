#include "zf_common_typedef.h"
#include "ENCODER.h"
int16 encoder_data_dl1,encoder_data_dl2,encoder_data_l;
void ENCODER_init(void)
{
    encoder_quad_init(ENCODER_DL1, ENCODER_DL1_A, ENCODER_DL1_B);   // 初始化编码器模块与引脚 正交解码编码器模式
    encoder_quad_init(ENCODER_DL2, ENCODER_DL2_A, ENCODER_DL2_B);   // 初始化编码器模块与引脚 正交解码编码器模式

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取编码器数值
//  @param      void
//  @return     void（实际赋到了encoder_data_dl1与encoder_data_dl2中）
//  @note
//  Sample usage:       Get_Encoder()
//-------------------------------------------------------------------------------------------------------------------
vint16 L_C,R_C;
void Get_Encoder(void)
{
    encoder_data_dl1 = -encoder_get_count(ENCODER_DL1);                  // 获取编码器计数
    encoder_data_dl2 = -encoder_get_count(ENCODER_DL2);                  // 获取编码器计数
    //encoder_data_l = encoder_get_count(ENCODER_L);                  // 获取编码器计数
    encoder_clear_count(ENCODER_DL1);                                       // 清空编码器计数
    encoder_clear_count(ENCODER_DL2);                                       // 清空编码器计数
}
