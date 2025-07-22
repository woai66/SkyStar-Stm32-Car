#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx_hal.h"

// OLED参数定义
#define OLED_WIDTH   128
#define OLED_HEIGHT  64

// 声明外部引用的I2C句柄
extern I2C_HandleTypeDef hi2c2;

// 函数声明
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t size);
void OLED_ShowNum(uint8_t x, uint8_t y, int num, uint8_t size);
void OLED_Test(void);
void OLED_ClearArea(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

#endif