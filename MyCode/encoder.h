
#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

// 函数声明
void Encoder_Init(void);
int16_t Encoder_A_GetCount(void);
int16_t Encoder_B_GetCount(void);
void Encoder_Update(void);
int16_t Encoder_A_GetSpeed(void);
int16_t Encoder_B_GetSpeed(void);
int32_t Encoder_A_GetTotal(void);
int32_t Encoder_B_GetTotal(void);
void Encoder_Reset(void);
void Encoder_Test(void);

// 外部变量声明
extern int32_t EncoderA_Count;
extern int32_t EncoderB_Count;
extern int16_t EncoderA_Speed;
extern int16_t EncoderB_Speed;

#ifdef __cplusplus
}
#endif

#endif /* __ENCODER_H */