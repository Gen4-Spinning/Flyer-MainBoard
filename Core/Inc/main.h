/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

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
void checkACK(void);
void startLifts(void);
void startMotors(void);
void stopMotors(void);
void pauseMotors(void);
void resumeMotors(void);
void startInching(void);
void stopInching(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOC
#define GREEN_Pin GPIO_PIN_0
#define GREEN_GPIO_Port GPIOA
#define GREEN_EXTI_IRQn EXTI0_IRQn
#define YELLOW_Pin GPIO_PIN_1
#define YELLOW_GPIO_Port GPIOA
#define YELLOW_EXTI_IRQn EXTI1_IRQn
#define RED_Pin GPIO_PIN_2
#define RED_GPIO_Port GPIOA
#define RED_EXTI_IRQn EXTI2_IRQn
#define ROTARY_Pin GPIO_PIN_3
#define ROTARY_GPIO_Port GPIOA
#define ROTARY_EXTI_IRQn EXTI3_IRQn
#define DAC1_OUT_Pin GPIO_PIN_4
#define DAC1_OUT_GPIO_Port GPIOA
#define DAC2_OUT_Pin GPIO_PIN_5
#define DAC2_OUT_GPIO_Port GPIOA
#define SMPS_OK_IP_Pin GPIO_PIN_6
#define SMPS_OK_IP_GPIO_Port GPIOA
#define SMPS_OK_IP_EXTI_IRQn EXTI9_5_IRQn
#define SMPS_CNTRL_GND_Pin GPIO_PIN_7
#define SMPS_CNTRL_GND_GPIO_Port GPIOA
#define SMPS_CNTRL_OP_Pin GPIO_PIN_0
#define SMPS_CNTRL_OP_GPIO_Port GPIOB
#define EXTRA5_Pin GPIO_PIN_1
#define EXTRA5_GPIO_Port GPIOB
#define EXTRA6_Pin GPIO_PIN_2
#define EXTRA6_GPIO_Port GPIOB
#define EXTRA7_Pin GPIO_PIN_10
#define EXTRA7_GPIO_Port GPIOB
#define EXTRA4_Pin GPIO_PIN_11
#define EXTRA4_GPIO_Port GPIOB
#define EXTRA3_Pin GPIO_PIN_12
#define EXTRA3_GPIO_Port GPIOB
#define EXTRA2_Pin GPIO_PIN_13
#define EXTRA2_GPIO_Port GPIOB
#define INT_B_Pin GPIO_PIN_14
#define INT_B_GPIO_Port GPIOB
#define INT_B_EXTI_IRQn EXTI15_10_IRQn
#define INT_A_Pin GPIO_PIN_15
#define INT_A_GPIO_Port GPIOB
#define INT_A_EXTI_IRQn EXTI15_10_IRQn
#define EXTRA1_Pin GPIO_PIN_10
#define EXTRA1_GPIO_Port GPIOA
#define DBG_TP_Pin GPIO_PIN_15
#define DBG_TP_GPIO_Port GPIOA
#define DBG_USART2_TX_Pin GPIO_PIN_3
#define DBG_USART2_TX_GPIO_Port GPIOB
#define DBG_USART2_RX_Pin GPIO_PIN_4
#define DBG_USART2_RX_GPIO_Port GPIOB
#define CAN_LED_Pin GPIO_PIN_5
#define CAN_LED_GPIO_Port GPIOB
#define BT_USART1_TX_Pin GPIO_PIN_6
#define BT_USART1_TX_GPIO_Port GPIOB
#define BT_USART1_RX_Pin GPIO_PIN_7
#define BT_USART1_RX_GPIO_Port GPIOB
#define FAULT_LED_Pin GPIO_PIN_9
#define FAULT_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
