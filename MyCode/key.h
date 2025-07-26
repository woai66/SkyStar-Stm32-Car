#ifndef __KEY_H
#define __KEY_H

#include "main.h"

// 定义按键端口和引脚
#define KEY1_PORT GPIOE
#define KEY1_PIN  GPIO_PIN_7
#define KEY2_PORT GPIOE
#define KEY2_PIN  GPIO_PIN_15

// 定义按键状态
#define KEY_PRESS    1
#define KEY_RELEASE  0

// 定义按键ID
#define KEY1  1
#define KEY2  2

// 函数声明
uint8_t Key_Scan(uint8_t mode);
uint8_t KEY1_Read(void);
uint8_t KEY2_Read(void);

#endif /* __KEY_H */