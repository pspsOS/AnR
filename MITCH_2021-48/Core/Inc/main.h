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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define SENSE_B_Pin GPIO_PIN_13
#define SENSE_B_GPIO_Port GPIOC
#define SENSE_A_Pin GPIO_PIN_14
#define SENSE_A_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOH
#define ARMING_EN_Pin GPIO_PIN_1
#define ARMING_EN_GPIO_Port GPIOH
#define ARMING_MEAS_Pin GPIO_PIN_4
#define ARMING_MEAS_GPIO_Port GPIOA
#define BAT_MEAS_Pin GPIO_PIN_0
#define BAT_MEAS_GPIO_Port GPIOB
#define ACCEL_ANALOG_Pin GPIO_PIN_1
#define ACCEL_ANALOG_GPIO_Port GPIOB
#define SPI1_LE_Pin GPIO_PIN_2
#define SPI1_LE_GPIO_Port GPIOB
#define SPI1_REG_Pin GPIO_PIN_10
#define SPI1_REG_GPIO_Port GPIOB
#define SPI2_REG_Pin GPIO_PIN_12
#define SPI2_REG_GPIO_Port GPIOB
#define SPI2_LE_Pin GPIO_PIN_8
#define SPI2_LE_GPIO_Port GPIOA
#define BUZZER_OUT_Pin GPIO_PIN_11
#define BUZZER_OUT_GPIO_Port GPIOA
#define BUTTON_IN_Pin GPIO_PIN_12
#define BUTTON_IN_GPIO_Port GPIOA
#define HDD_CHECK_Pin GPIO_PIN_15
#define HDD_CHECK_GPIO_Port GPIOA
#define FIRE_A_Pin GPIO_PIN_4
#define FIRE_A_GPIO_Port GPIOB
#define FIRE_B_Pin GPIO_PIN_5
#define FIRE_B_GPIO_Port GPIOB
#define FIRE_C_Pin GPIO_PIN_6
#define FIRE_C_GPIO_Port GPIOB
#define FIRE_D_Pin GPIO_PIN_7
#define FIRE_D_GPIO_Port GPIOB
#define SENSE_D_Pin GPIO_PIN_8
#define SENSE_D_GPIO_Port GPIOB
#define SENSE_C_Pin GPIO_PIN_9
#define SENSE_C_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
