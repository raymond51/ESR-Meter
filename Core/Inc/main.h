/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32l0xx_hal.h"

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
#define DIFF_SIG_ADC_Pin GPIO_PIN_0
#define DIFF_SIG_ADC_GPIO_Port GPIOA
#define OUTPUT_SIG_ADC_Pin GPIO_PIN_1
#define OUTPUT_SIG_ADC_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_2
#define BUZZER_GPIO_Port GPIOA
#define BUZZER_VOL_Pin GPIO_PIN_3
#define BUZZER_VOL_GPIO_Port GPIOA
#define SW_OFF_Pin GPIO_PIN_6
#define SW_OFF_GPIO_Port GPIOA
#define ANALOG_ON_Pin GPIO_PIN_7
#define ANALOG_ON_GPIO_Port GPIOA
#define PWR_LED_Pin GPIO_PIN_0
#define PWR_LED_GPIO_Port GPIOB
#define STATUS_LED_Pin GPIO_PIN_1
#define STATUS_LED_GPIO_Port GPIOB
#define MUX_A0_Pin GPIO_PIN_8
#define MUX_A0_GPIO_Port GPIOA
#define MUX_EN_Pin GPIO_PIN_9
#define MUX_EN_GPIO_Port GPIOA
#define MUX_A1_Pin GPIO_PIN_10
#define MUX_A1_GPIO_Port GPIOA
#define EX_BTN_Pin GPIO_PIN_15
#define EX_BTN_GPIO_Port GPIOA
#define RTRY_SW_Pin GPIO_PIN_3
#define RTRY_SW_GPIO_Port GPIOB
#define RTRY_DT_Pin GPIO_PIN_4
#define RTRY_DT_GPIO_Port GPIOB
#define RTRY_CLK_Pin GPIO_PIN_5
#define RTRY_CLK_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
