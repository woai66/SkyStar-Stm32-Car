
#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "tim.h"
#include "gpio.h"

// 控制电机A和B的函数，速度范围-1000~1000，正值顺时针，负值逆时针
void Motor_A_Control(int16_t speed);
void Motor_B_Control(int16_t speed);
void Motors_Test(void);

// 初始化电机
void Motor_Init(void);

#endif /* __MOTOR_H */