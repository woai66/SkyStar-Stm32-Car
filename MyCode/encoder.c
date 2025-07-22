
#include "encoder.h"
#include "tim.h"

// 编码器计数值，用于记录当前位置
int32_t EncoderA_Count = 0;
int32_t EncoderB_Count = 0;

// 编码器计数累计值，用于记录总行程
int32_t EncoderA_Total = 0;
int32_t EncoderB_Total = 0;

// 编码器速度值（单位时间内的计数变化）
int16_t EncoderA_Speed = 0;
int16_t EncoderB_Speed = 0;

// 上一次读取的编码器计数值，用于计算速度
int16_t EncoderA_LastCount = 0;
int16_t EncoderB_LastCount = 0;

// 编码器初始化函数
void Encoder_Init(void)
{
  // 启动编码器模式的定时器
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);  // 电机A编码器
  HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);  // 电机B编码器
  
  // 初始化计数器值
  __HAL_TIM_SET_COUNTER(&htim4, 0);
  __HAL_TIM_SET_COUNTER(&htim8, 0);
}

// 读取编码器A的当前计数值
int16_t Encoder_A_GetCount(void)
{
  int16_t count;
  
  count = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
  
  // 如果需要反转编码器方向，可以使用下面的代码
  // count = -count;
  
  return count;
}

// 读取编码器B的当前计数值
int16_t Encoder_B_GetCount(void)
{
  int16_t count;
  
  count = (int16_t)__HAL_TIM_GET_COUNTER(&htim8);
  
  // 如果需要反转编码器方向，可以使用下面的代码
  // count = -count;
  
  return count;
}

// 更新编码器数据，建议在定时器中断中调用
void Encoder_Update(void)
{
  int16_t countA, countB;
  
  // 读取当前计数器值
  countA = Encoder_A_GetCount();
  countB = Encoder_B_GetCount();
  
  // 计算速度（当前值与上一次读取值之差）
  EncoderA_Speed = countA - EncoderA_LastCount;
  EncoderB_Speed = countB - EncoderB_LastCount;
  
  // 更新总行程计数
  EncoderA_Total += EncoderA_Speed;
  EncoderB_Total += EncoderB_Speed;
  
  // 保存当前计数器值作为下一次的历史值
  EncoderA_LastCount = countA;
  EncoderB_LastCount = countB;
  
  // 保存当前计数值
  EncoderA_Count = countA;
  EncoderB_Count = countB;
}

// 获取电机A速度
int16_t Encoder_A_GetSpeed(void)
{
  return EncoderA_Speed;
}

// 获取电机B速度
int16_t Encoder_B_GetSpeed(void)
{
  return EncoderB_Speed;
}

// 获取电机A累计编码器计数
int32_t Encoder_A_GetTotal(void)
{
  return EncoderA_Total;
}

// 获取电机B累计编码器计数
int32_t Encoder_B_GetTotal(void)
{
  return EncoderB_Total;
}

// 重置编码器计数
void Encoder_Reset(void)
{
  __HAL_TIM_SET_COUNTER(&htim4, 0);
  __HAL_TIM_SET_COUNTER(&htim8, 0);
  
  EncoderA_Count = 0;
  EncoderB_Count = 0;
  EncoderA_LastCount = 0;
  EncoderB_LastCount = 0;
  EncoderA_Speed = 0;
  EncoderB_Speed = 0;
  EncoderA_Total = 0;
  EncoderB_Total = 0;
}

// 编码器测试函数
void Encoder_Test(void)
{
  // 实现编码器测试功能
  // 可以在OLED上显示计数值和速度等信息
}