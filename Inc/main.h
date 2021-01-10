/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LF_Pin GPIO_PIN_0
#define LF_GPIO_Port GPIOA
#define RF_Pin GPIO_PIN_1
#define RF_GPIO_Port GPIOA
#define LB_Pin GPIO_PIN_2
#define LB_GPIO_Port GPIOA
#define RB_Pin GPIO_PIN_3
#define RB_GPIO_Port GPIOA
#define LB_CH1_Pin GPIO_PIN_6
#define LB_CH1_GPIO_Port GPIOA
#define LB_CH2_Pin GPIO_PIN_7
#define LB_CH2_GPIO_Port GPIOA
#define LF_IN1_Pin GPIO_PIN_0
#define LF_IN1_GPIO_Port GPIOB
#define LF_IN2_Pin GPIO_PIN_1
#define LF_IN2_GPIO_Port GPIOB
#define RF_IN1_Pin GPIO_PIN_2
#define RF_IN1_GPIO_Port GPIOB
#define RF_IN2_Pin GPIO_PIN_10
#define RF_IN2_GPIO_Port GPIOB
#define LB_IN1_Pin GPIO_PIN_11
#define LB_IN1_GPIO_Port GPIOB
#define LB_IN2_Pin GPIO_PIN_12
#define LB_IN2_GPIO_Port GPIOB
#define RB_IN1_Pin GPIO_PIN_13
#define RB_IN1_GPIO_Port GPIOB
#define RB_IN2_Pin GPIO_PIN_14
#define RB_IN2_GPIO_Port GPIOB
#define LF_CH1_Pin GPIO_PIN_8
#define LF_CH1_GPIO_Port GPIOA
#define LF_CH2_Pin GPIO_PIN_9
#define LF_CH2_GPIO_Port GPIOA
#define RF_CH1_Pin GPIO_PIN_15
#define RF_CH1_GPIO_Port GPIOA
#define RF_CH2_Pin GPIO_PIN_3
#define RF_CH2_GPIO_Port GPIOB
#define RB_CH1_Pin GPIO_PIN_6
#define RB_CH1_GPIO_Port GPIOB
#define RB_CH2_Pin GPIO_PIN_7
#define RB_CH2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
