#include "motor.h"
#include "tim.h"
#include "gpio.h"
#include "oled.h"

// 定义电机控制宏
#define MOTOR_A_CW     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET)
#define MOTOR_A_CCW    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET)
#define MOTOR_B_CW     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET)
#define MOTOR_B_CCW    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET)

// 控制电机A(左)的函数，速度范围-1000~1000，正前进
void Motor_A_Control(int16_t speed)
{
  if (speed >= 0) {
    MOTOR_A_CW;  //前进
    if (speed > 1000) speed = 1000;  // 限幅
    speed = 1000 - speed;
  } else {
    MOTOR_A_CCW;  //后退
    speed = -speed;  // 取绝对值
    if (speed > 1000) speed = 1000;
    speed = 1000 - speed;
  }
  
  // 设置PWM占空比
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, speed);
}

// 控制电机B（右）的函数，速度范围-1000~1000，正前进
void Motor_B_Control(int16_t speed)
{
  if (speed >= 0) {
    MOTOR_B_CW;  // 设置顺时针方向
    if (speed > 1000) speed = 1000;  // 限制最大值
    // 反转PWM值：0表示全速，1000表示停止
    speed = 1000 - speed;
  } else {
    MOTOR_B_CCW;  // 设置逆时针方向
    speed = -speed;  // 取绝对值
    if (speed > 1000) speed = 1000;  // 限制最大值
    // 反转PWM值：0表示全速，1000表示停止
    speed = 1000 - speed;
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
  // 电机AB以50%速度顺时针转动3秒
  Motor_A_Control(500);
  Motor_B_Control(500);
  OLED_ShowString(0, 0, "Motor AB: 500", 16);
  HAL_Delay(3000);  

  
  // 电机AB停止1秒
  Motor_A_Control(0);
  Motor_B_Control(0);
  OLED_Clear();
  OLED_ShowString(0, 0, "Motor AB: 0", 16);
  HAL_Delay(3000);
  
  // 电机AB以50%速度逆时针转动1秒
  Motor_A_Control(1000);
  Motor_B_Control(1000);
  OLED_Clear();
  OLED_ShowString(0, 0, "Motor AB: 1000", 16);
  HAL_Delay(3000);
  
  // 电机AB停止
  // Motor_A_Control(0);
  // Motor_B_Control(0);
  // HAL_Delay(500);
}