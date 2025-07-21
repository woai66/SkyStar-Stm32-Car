
#include "motor.h"
#include "tim.h"
#include "gpio.h"

// 定义电机控制宏
#define MOTOR_A_CW     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET)
#define MOTOR_A_CCW    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET)
#define MOTOR_B_CW     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET)
#define MOTOR_B_CCW    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET)

// 控制电机A的函数，速度范围-1000~1000，正值顺时针，负值逆时针
void Motor_A_Control(int16_t speed)
{
  if (speed >= 0) {
    MOTOR_A_CW;  // 设置顺时针方向
    if (speed > 1000) speed = 1000;  // 限制最大值
  } else {
    MOTOR_A_CCW;  // 设置逆时针方向
    speed = -speed;  // 取绝对值
    if (speed > 1000) speed = 1000;  // 限制最大值
  }
  
  // 设置PWM占空比
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, speed);
}

// 控制电机B的函数，速度范围-1000~1000，正值顺时针，负值逆时针
void Motor_B_Control(int16_t speed)
{
  if (speed >= 0) {
    MOTOR_B_CW;  // 设置顺时针方向
    if (speed > 1000) speed = 1000;  // 限制最大值
  } else {
    MOTOR_B_CCW;  // 设置逆时针方向
    speed = -speed;  // 取绝对值
    if (speed > 1000) speed = 1000;  // 限制最大值
  }
  
  // 设置PWM占空比
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, speed);
}

// 电机初始化函数
void Motor_Init(void)
{
  // 启动PWM输出
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);  // 启动电机A的PWM
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);  // 启动电机B的PWM
}

// 电机测试函数
void Motors_Test(void)
{
  // 电机A以50%速度顺时针转动1秒
  Motor_A_Control(500);
  HAL_Delay(1000);
  
  // 电机A停止0.5秒
  Motor_A_Control(0);
  HAL_Delay(500);
  
  // 电机A以50%速度逆时针转动1秒
  Motor_A_Control(-500);
  HAL_Delay(1000);
  
  // 电机A停止
  Motor_A_Control(0);
  
  // 电机B以50%速度顺时针转动1秒
  Motor_B_Control(500);
  HAL_Delay(1000);
  
  // 电机B停止0.5秒
  Motor_B_Control(0);
  HAL_Delay(500);
  
  // 电机B以50%速度逆时针转动1秒
  Motor_B_Control(-500);
  HAL_Delay(1000);
  
  // 电机B停止
  Motor_B_Control(0);
  
  // 两个电机同时转动1秒
  Motor_A_Control(300);
  Motor_B_Control(300);
  HAL_Delay(1000);
  
  // 两个电机都停止
  Motor_A_Control(0);
  Motor_B_Control(0);
}