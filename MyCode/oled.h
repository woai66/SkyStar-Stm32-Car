#ifndef __OLED_H
#define __OLED_H

#include "main.h"
#include "i2c.h"

// OLED显示尺寸定义
#define OLED_WIDTH  128
#define OLED_HEIGHT 64

// 函数声明
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t size);
void OLED_Test(void);

#endif