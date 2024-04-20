/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <string.h>

#include "bldc.h"
#include "retarget.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void DWT_Delay_us(volatile uint32_t au32_microseconds);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define S_W_Pin GPIO_PIN_1
#define S_W_GPIO_Port GPIOC
#define S_V_Pin GPIO_PIN_2
#define S_V_GPIO_Port GPIOC
#define S_U_Pin GPIO_PIN_3
#define S_U_GPIO_Port GPIOC
#define I_SENS_Pin GPIO_PIN_0
#define I_SENS_GPIO_Port GPIOA
#define V_W_Pin GPIO_PIN_1
#define V_W_GPIO_Port GPIOA
#define V_V_Pin GPIO_PIN_4
#define V_V_GPIO_Port GPIOA
#define V_U_Pin GPIO_PIN_0
#define V_U_GPIO_Port GPIOB
#define W_IN_Pin GPIO_PIN_13
#define W_IN_GPIO_Port GPIOB
#define V_IN_Pin GPIO_PIN_14
#define V_IN_GPIO_Port GPIOB
#define U_IN_Pin GPIO_PIN_15
#define U_IN_GPIO_Port GPIOB
#define U_PWM_CH_Pin GPIO_PIN_8
#define U_PWM_CH_GPIO_Port GPIOA
#define V_PWM_CH_Pin GPIO_PIN_9
#define V_PWM_CH_GPIO_Port GPIOA
#define W_PWM_CH_Pin GPIO_PIN_10
#define W_PWM_CH_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define W_CH TIM_CHANNEL_3
#define V_CH TIM_CHANNEL_2
#define U_CH TIM_CHANNEL_1

#define TIM_PWM_COMPARE_MIN 0
#define TIM_PWM_COMPARE_MAX 99
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
