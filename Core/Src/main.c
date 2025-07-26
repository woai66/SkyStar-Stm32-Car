/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "headfile.h"  // 包含自定义头文件，包含了电机、OLED、编码器等相关函数的声明

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//bno08x 传感器相关变量
float q0, q1, q2, q3;
float roll, pitch, yaw;
float accx = 0.0f, accy = 0.0f, accz = 0.0f; // 用于存储加速度数据

// 按键相关变量
uint8_t key1_state = 0; // KEY1状态
uint8_t key2_state = 0; // KEY2状态
uint8_t keyyy = 0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_I2C2_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);  // 启动定时器2，用于定时更新编码器数据
  Encoder_Init();  // 初始化编码器 (启动编码器模式的定时器)
  // 初始化电机
  Motor_Init();

  // 初始化OLED
  OLED_Init();
  OLED_Clear();
  HAL_UART_Transmit(&huart1, (uint8_t*)"oled OK\r\n", 17, HAL_MAX_DELAY); // 通过串口打印初始化信息
  OLED_ShowString(0, 0, "OLED Test", 16);
  OLED_ShowString(0, 2, "Init OK!", 16);

  // 重置传感器
  //HAL_UART_Transmit(&huart1, (uint8_t*)"BNO08x Reset\r\n", 17, HAL_MAX_DELAY); // 通过串口打印重置信息
  softReset();
  OLED_ShowString(0, 3, "BNO08x Reset OK!", 16);

  enableRotationVector(10);
  // enableGameRotationVector(100);
  // enableAccelerometer(100);
  // enableLinearAccelerometer(100);
  // enableGyro(100);
  // enableMagnetometer(100);
  // enableStepCounter(100);
  // enableStabilityClassifier(100);

  // calibrateAll();
  HAL_UART_Transmit(&huart1, (uint8_t*)"BNO08x Init OK\r\n", 17, HAL_MAX_DELAY); // 通过串口打印初始化信息
  HAL_Delay(1000);
  OLED_Clear(); // 清屏
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // 添加按键扫描处理
    uint8_t key = Key_Scan(0);
    if(key == KEY1)
    {
      // KEY1按下的处理
      OLED_ShowString(0, 6, "KEY1 Pressed", 16);
      OLED_ShowNum(0, 4, keyyy, 16); // 显示按键计数
      keyyy ++;
    }
    else if(key == KEY2)
    {
      // KEY2按下的处理
      OLED_ShowString(0, 6, "KEY2 Pressed", 16);
    }

    // char buffer[50];
    // // snprintf(buffer, sizeof(buffer), "AccX: %.2f, AccY: %.2f, AccZ: %.2f\r\n", accx, accy, accz);
    // snprintf(buffer, sizeof(buffer), "Roll: %.2f, Pitch: %.2f, Yaw: %.2f\r\n", roll, pitch, yaw);
    // HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY); // 通过串口打印加速度数据
  

    //屏幕显示，获取BNO08x传感器数据会导致卡顿，建议注释
   // OLED_ShowNum(0, 0, (int)(roll), 16); // 显示加速度X
   // OLED_ShowNum(0, 2, (int)(pitch), 16); // 显示加速度Y
   // OLED_ShowNum(0, 4, (int)(yaw), 16); // 显示加速度Z
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if (htim->Instance == TIM2)  // 检查是否是定时器2的中断
  {
    // Encoder_Update();  // 更新编码器数据
    if (dataAvailable()) // 检查是否有新数据可用
    {
      // accx = getAccelX();
      // accy = getAccelY();
      // accz = getAccelZ();
      q0 = getQuatReal();
      q1 = getQuatI();
      q2 = getQuatJ();
      q3 = getQuatK();

      roll = atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2)) * 57.3;
      pitch = asin(2 * (q0 * q2 - q3 * q1)) * 57.3;
      yaw = atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3)) * 57.3;

    }

    TIM2->CNT = 0;  // 清除定时器计数器

    // 可以在这里添加其他定时任务
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
