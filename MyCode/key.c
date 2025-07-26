#include "key.h"

/**
 * @brief 读取KEY1状态
 * @return 按键状态，1表示按下，0表示松开
 * @note KEY1为下拉输入，按下为高电平
 */
uint8_t KEY1_Read(void)
{
    if(HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == GPIO_PIN_SET)
    {
        return KEY_PRESS;
    }
    else
    {
        return KEY_RELEASE;
    }
}

/**
 * @brief 读取KEY2状态
 * @return 按键状态，1表示按下，0表示松开
 * @note KEY2为上拉输入，按下为低电平
 */
uint8_t KEY2_Read(void)
{
    if(HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN) == GPIO_PIN_RESET)
    {
        return KEY_PRESS;
    }
    else
    {
        return KEY_RELEASE;
    }
}

/**
 * @brief 按键扫描函数
 * @param mode: 0-不支持连续按，1-支持连续按
 * @return 按键值：1-KEY1，2-KEY2，0-无按键按下
 * @note 该函数有去抖动功能
 */
uint8_t Key_Scan(uint8_t mode)
{
    static uint8_t key_up = 1; // 按键按松开标志
    
    if(mode == 1) key_up = 1; // 支持连按
    
    if(key_up && (KEY1_Read() == KEY_PRESS || KEY2_Read() == KEY_PRESS))
    {
        HAL_Delay(10); // 去抖动
        key_up = 0;
        
        if(KEY1_Read() == KEY_PRESS)
        {
            return KEY1;
        }
        else if(KEY2_Read() == KEY_PRESS)
        {
            return KEY2;
        }
    }
    else if(KEY1_Read() == KEY_RELEASE && KEY2_Read() == KEY_RELEASE)
    {
        key_up = 1;
    }
    
    return 0; // 无按键按下
}
/**
 * @brief 按键处理函数
 */
void key_proc()
{
    
}
