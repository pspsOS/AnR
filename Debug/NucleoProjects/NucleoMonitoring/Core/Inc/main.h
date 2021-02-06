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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../IncPSP/common.h"
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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define SENSE_A_Pin GPIO_PIN_6
#define SENSE_A_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define NDEBUG
#define BYPASS_GPS

// Acquisition Multiplier Settings
#define GPS_FREQ (1)        //   1 Hz
#define BMP_MULTIPLIER (10) //  10 Hz
#define IMU_MULTIPLIER (10) // 100 Hz

// Task Rates
#define ACQUISITION_TASK_RATE0  (GPS_FREQ)
#define ACQUISITION_TASK_RATE1  (GPS_FREQ * BMP_MULTIPLIER)
#define ACQUISITION_TASK_RATE2  (GPS_FREQ * BMP_MULTIPLIER * IMU_MULTIPLIER)
#define CONTROL_LOGIC_TASK_RATE (1)   // TODO: Determine optimal Control Logic Task Rate (currently 1 Hz)
#define MONITORING_TASK_RATE    (1)   // TODO: Determine optimal Monitoring Task Rate (currently 1 Hz)
#define PROCESSING_TASK_RATE    (125) // TODO: Determine optimal Processing Task Rate (currently 125 Hz)

// Task Delays are calculated from above rates (DO NOT EDIT)
#define ACQUISITION_TASK_DELAY0  (1000 / ACQUISITION_TASK_RATE0 / portTICK_RATE_MS)
#define ACQUISITION_TASK_DELAY1  (1000 / ACQUISITION_TASK_RATE1 / portTICK_RATE_MS)
#define ACQUISITION_TASK_DELAY2  (1000 / ACQUISITION_TASK_RATE2 / portTICK_RATE_MS)
//#define CONTROL_LOGIC_TASK_DELAY (1000 / CONTROL_LOGIC_TASK_RATE / portTICK_RATE_MS)
#define MONITORING_TASK_DELAY    (1000 / MONITORING_TASK_RATE / portTICK_RATE_MS)
#define PROCESSING_TASK_DELAY    (1000 / PROCESSING_TASK_RATE / portTICK_RATE_MS)


#define ENABLE_CONTROL_LOGIC (1)
#define ENABLE_ACQUISITION (0)
#define ENABLE_MONITORING (0)
#define ENABLE_PROCESSING (1)


// #define SUPRESS_TASK_UPDATES
// #define SUPRESS_SETUP_WARNING
 #define SUPRESS_ALL

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
