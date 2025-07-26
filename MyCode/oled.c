#include "oled.h"
#include "stdlib.h"
#include "math.h"
#include "oledfont.h"  // 如果有字体文件，需要另外创建
#include "stdio.h"
#include "string.h"
// OLED的I2C地址
#define OLED_ADDR    0x78  // OLED的I2C地址（0x78对应0x3C，左移1位）
#define OLED_CMD     0x00  // 写命令
#define OLED_DATA    0x40  // 写数据

// I2C写一个字节函数
static void OLED_Write_Byte(uint8_t dat, uint8_t cmd)
{
  uint8_t buffer[2];
  buffer[0] = cmd;         // 命令模式或数据模式
  buffer[1] = dat;         // 要写入的数据
  HAL_I2C_Master_Transmit(&hi2c2, OLED_ADDR, buffer, 2, 100);
}

// OLED初始化
void OLED_Init(void)
{
  HAL_Delay(100);  // 等待OLED上电稳定
  
  // 初始化OLED
  OLED_Write_Byte(0xAE, OLED_CMD); // 关闭显示
  
  OLED_Write_Byte(0xD5, OLED_CMD); // 设置显示时钟分频比/振荡器频率
  OLED_Write_Byte(0x80, OLED_CMD); // 设置分频比
  
  OLED_Write_Byte(0xA8, OLED_CMD); // 设置复用比
  OLED_Write_Byte(0x3F, OLED_CMD); // 复用比为1/64
  
  OLED_Write_Byte(0xD3, OLED_CMD); // 设置显示偏移
  OLED_Write_Byte(0x00, OLED_CMD); // 无偏移
  
  OLED_Write_Byte(0x40, OLED_CMD); // 设置起始行地址
  
  OLED_Write_Byte(0x8D, OLED_CMD); // 设置充电泵
  OLED_Write_Byte(0x14, OLED_CMD); // 开启充电泵
  
  OLED_Write_Byte(0x20, OLED_CMD); // 设置内存寻址模式
  OLED_Write_Byte(0x00, OLED_CMD); // 水平寻址模式
  
  OLED_Write_Byte(0xA1, OLED_CMD); // 设置段重映射（0xA0左右反置，0xA1正常）
  
  OLED_Write_Byte(0xC8, OLED_CMD); // 设置COM输出方向（反转）
  
  OLED_Write_Byte(0xDA, OLED_CMD); // 设置COM引脚硬件配置
  OLED_Write_Byte(0x12, OLED_CMD); // 
  
  OLED_Write_Byte(0x81, OLED_CMD); // 设置对比度控制寄存器
  OLED_Write_Byte(0xCF, OLED_CMD); // 设置对比度值
  
  OLED_Write_Byte(0xD9, OLED_CMD); // 设置预充电周期
  OLED_Write_Byte(0xF1, OLED_CMD); // 设置预充电周期
  
  OLED_Write_Byte(0xDB, OLED_CMD); // 设置VCOMH电压倍率
  OLED_Write_Byte(0x30, OLED_CMD); // 设置VCOMH电压倍率
  
  OLED_Write_Byte(0xA4, OLED_CMD); // 全局显示开启（0xA4为正常显示，0xA5为全亮）
  
  OLED_Write_Byte(0xA6, OLED_CMD); // 设置正常/反向显示（0xA6正常，0xA7反向）
  
  OLED_Write_Byte(0xAF, OLED_CMD); // 开启显示
  
  OLED_Clear();  // 清屏
}

// 开启OLED显示
void OLED_Display_On(void)
{
  OLED_Write_Byte(0x8D, OLED_CMD);  // 设置充电泵
  OLED_Write_Byte(0x14, OLED_CMD);  // 开启充电泵
  OLED_Write_Byte(0xAF, OLED_CMD);  // 开启显示
}

// 关闭OLED显示
void OLED_Display_Off(void)
{
  OLED_Write_Byte(0x8D, OLED_CMD);  // 设置充电泵
  OLED_Write_Byte(0x10, OLED_CMD);  // 关闭充电泵
  OLED_Write_Byte(0xAE, OLED_CMD);  // 关闭显示
}

// 清屏函数
void OLED_Clear(void)
{
  uint8_t i, n;
  for(i = 0; i < 8; i++)
  {
    OLED_Write_Byte(0xB0 + i, OLED_CMD);  // 设置页地址（0~7）
    OLED_Write_Byte(0x00, OLED_CMD);      // 设置显示位置—列低地址
    OLED_Write_Byte(0x10, OLED_CMD);      // 设置显示位置—列高地址
    
    for(n = 0; n < 128; n++)
    {
      OLED_Write_Byte(0x00, OLED_DATA);   // 写入空白数据
    }
  }
}

// 清除特定区域的显示内容
void OLED_ClearArea(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  uint8_t i, n;
  
  // 确保坐标在有效范围内
  if (x1 >= OLED_WIDTH) x1 = OLED_WIDTH-1;
  if (x2 >= OLED_WIDTH) x2 = OLED_WIDTH-1;
  if (y1 >= 8) y1 = 7;
  if (y2 >= 8) y2 = 7;
  
  // 确保x2 >= x1，y2 >= y1
  if(x1 > x2) { uint8_t temp = x1; x1 = x2; x2 = temp; }
  if(y1 > y2) { uint8_t temp = y1; y1 = y2; y2 = temp; }
  
  for(i = y1; i <= y2; i++)
  {
    OLED_SetPos(x1, i);  // 设置起始位置
    for(n = x1; n <= x2; n++)
    {
      OLED_Write_Byte(0x00, OLED_DATA);  // 写入空白数据
    }
  }
}

// 设置光标位置
void OLED_SetPos(uint8_t x, uint8_t y)
{
  OLED_Write_Byte(0xB0 + y, OLED_CMD);                // 设置页地址（0~7）
  OLED_Write_Byte(((x & 0xF0) >> 4) | 0x10, OLED_CMD);// 设置列高地址
  OLED_Write_Byte(x & 0x0F, OLED_CMD);                // 设置列低地址
}

// 在指定位置显示一个字符
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size)
{      	
  uint8_t c = 0, i = 0;	
  c = chr - ' ';  // 得到偏移后的值	
  
  if(x > OLED_WIDTH - size/2 || y > OLED_HEIGHT/8 - 1)
    return;
    
  if(size == 16)
  {
    OLED_SetPos(x, y);	
    for(i = 0; i < 8; i++)
    {
      OLED_Write_Byte(F8X16[c][i], OLED_DATA);
    }
    
    OLED_SetPos(x, y+1);
    for(i = 0; i < 8; i++)
    {
      OLED_Write_Byte(F8X16[c][i+8], OLED_DATA);
    }
  }
  else if(size == 8)
  {	
    OLED_SetPos(x, y);
    for(i = 0; i < 6; i++)
    {
      OLED_Write_Byte(F6x8[c][i], OLED_DATA);
    }
  }
}

// 显示字符串函数
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t size)
{
  uint8_t j = 0;
  uint8_t start_x = x; // 记录起始X坐标
  uint8_t char_width = (size == 16) ? 8 : 6;
  uint8_t line_height = (size == 16) ? 2 : 1;
  
  // 显示字符串
  while (str[j] != '\0')
  {    
    OLED_ShowChar(x, y, str[j], size);
    x += char_width;
    if(x > OLED_WIDTH - char_width)
    {
      x = 0;
      y += line_height;
      if(y >= OLED_HEIGHT/8) return; // 超出屏幕高度范围
    }
    j++;
  }
  
  // 在当前行的字符串后面填充空白字符，直到行末
  while(x < OLED_WIDTH)
  {
    OLED_ShowChar(x, y, ' ', size);
    x += char_width;
  }
}

// 增加显示数字函数（其实是将整数转换为字符串后调用OLED_ShowString）
void OLED_ShowNum(uint8_t x, uint8_t y, int num, uint8_t size)
{
  char buffer[16];
  sprintf(buffer, "%d", num);  // 将整数转换为字符串
  
  OLED_ShowString(x, y, buffer, size);
}

// 清除指定行
void OLED_ClearLine(uint8_t line)
{
  if(line >= 8) return; // 超出范围
  
  OLED_SetPos(0, line);
  for(uint8_t i = 0; i < OLED_WIDTH; i++)
  {
    OLED_Write_Byte(0x00, OLED_DATA);
  }
}

// 显示BNO08x陀螺仪数据


void OLED_Test(void)
{
  OLED_Clear();
  
  OLED_ShowString(0, 0, "Debug OLED Test", 16);
  OLED_ShowString(0, 2, "Hello World!", 16);
  OLED_ShowString(0, 4, "Motor Speed:", 16);
  HAL_Delay(2000);
  
  // 使用空白字符填充更新文本
  OLED_ShowString(0, 2, "Updated Text!", 16);
  HAL_Delay(1000);
  
  OLED_Clear();
    
  OLED_ShowString(0, 0, "DSCAR", 16);
  OLED_ShowString(0, 2, "Motor A: ", 16);
  OLED_ShowString(0, 4, "Motor B: ", 16);
  OLED_ShowString(0, 6, "Test Mode", 16);

}