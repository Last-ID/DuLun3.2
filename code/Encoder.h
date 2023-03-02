#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "zf_common_headfile.h"

#define PIT0                        (CCU60_CH0 )

#define ENCODER_DL1                 (TIM2_ENCOEDER)                         // ������������Ӧʹ�õı������ӿ�
#define ENCODER_DL1_A               (TIM2_ENCOEDER_CH1_P33_7)               // A ���Ӧ������
#define ENCODER_DL1_B               (TIM2_ENCOEDER_CH2_P33_6)               // B ���Ӧ������
#define ENCODER_DL2                 (TIM5_ENCOEDER)                         // ������������Ӧʹ�õı������ӿ�
#define ENCODER_DL2_A               (TIM5_ENCOEDER_CH1_P10_3)               // A ���Ӧ������
#define ENCODER_DL2_B               (TIM5_ENCOEDER_CH2_P10_1)               // B ���Ӧ������
/*
#define ENCODER_L                   (TIM5_ENCOEDER)                         // ������������Ӧʹ�õı������ӿ�
#define ENCODER_L_A                 (TIM5_ENCOEDER_CH1_P10_3)               // A ���Ӧ������
#define ENCODER_L_B                 (TIM5_ENCOEDER_CH2_P10_1)               // B ���Ӧ������
*/
extern int16 encoder_data_dl1,encoder_data_dl2,encoder_data_l;
void ENCODER_init(void);
void Get_Encoder(void);

#endif /* CODE_ENCODER_H_ */
