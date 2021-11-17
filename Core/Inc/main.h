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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
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
#define ENC_A_Pin GPIO_PIN_0
#define ENC_A_GPIO_Port GPIOF
#define ENC_A_EXTI_IRQn EXTI0_1_IRQn
#define ENC_B_Pin GPIO_PIN_1
#define ENC_B_GPIO_Port GPIOF
#define SEG_LED_A_Pin GPIO_PIN_0
#define SEG_LED_A_GPIO_Port GPIOA
#define SEG_LED_B_Pin GPIO_PIN_1
#define SEG_LED_B_GPIO_Port GPIOA
#define SEG_LED_C_Pin GPIO_PIN_2
#define SEG_LED_C_GPIO_Port GPIOA
#define SEG_LED_D_Pin GPIO_PIN_3
#define SEG_LED_D_GPIO_Port GPIOA
#define SEG_LED_E_Pin GPIO_PIN_4
#define SEG_LED_E_GPIO_Port GPIOA
#define SEG_LED_F_Pin GPIO_PIN_5
#define SEG_LED_F_GPIO_Port GPIOA
#define SEG_LED_G_Pin GPIO_PIN_6
#define SEG_LED_G_GPIO_Port GPIOA
#define SEG_LED_DP_Pin GPIO_PIN_7
#define SEG_LED_DP_GPIO_Port GPIOA
#define SEG_LED_1_Pin GPIO_PIN_0
#define SEG_LED_1_GPIO_Port GPIOB
#define SEG_LED_2_Pin GPIO_PIN_1
#define SEG_LED_2_GPIO_Port GPIOB
#define SEG_LED_3_Pin GPIO_PIN_8
#define SEG_LED_3_GPIO_Port GPIOA
#define INPUT1_Pin GPIO_PIN_11
#define INPUT1_GPIO_Port GPIOA
#define MUTE_Pin GPIO_PIN_12
#define MUTE_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOA
#define VOL_DAT_Pin GPIO_PIN_3
#define VOL_DAT_GPIO_Port GPIOB
#define VOL_LOAD_Pin GPIO_PIN_4
#define VOL_LOAD_GPIO_Port GPIOB
#define VOL_CLK_Pin GPIO_PIN_5
#define VOL_CLK_GPIO_Port GPIOB
#define BTN_1_Pin GPIO_PIN_6
#define BTN_1_GPIO_Port GPIOB
#define BTN_2_Pin GPIO_PIN_7
#define BTN_2_GPIO_Port GPIOB
#define POWER_SW_Pin GPIO_PIN_8
#define POWER_SW_GPIO_Port GPIOB
#define POWER_SW_EXTI_IRQn EXTI4_15_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
