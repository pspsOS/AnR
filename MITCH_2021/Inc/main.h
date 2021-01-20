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
#define BUZZER_OUT_Pin GPIO_PIN_2
#define BUZZER_OUT_GPIO_Port GPIOE
#define FIRE_A_Pin GPIO_PIN_1
#define FIRE_A_GPIO_Port GPIOF
#define FIRE_B_Pin GPIO_PIN_2
#define FIRE_B_GPIO_Port GPIOF
#define FIRE_C_Pin GPIO_PIN_3
#define FIRE_C_GPIO_Port GPIOF
#define FIRE_D_Pin GPIO_PIN_4
#define FIRE_D_GPIO_Port GPIOF
#define SENSE_A_Pin GPIO_PIN_6
#define SENSE_A_GPIO_Port GPIOF
#define SENSE_B_Pin GPIO_PIN_7
#define SENSE_B_GPIO_Port GPIOF
#define SENSE_C_Pin GPIO_PIN_8
#define SENSE_C_GPIO_Port GPIOF
#define SENSE_D_Pin GPIO_PIN_9
#define SENSE_D_GPIO_Port GPIOF
#define CS1_Pin GPIO_PIN_4
#define CS1_GPIO_Port GPIOC
#define CS2_Pin GPIO_PIN_5
#define CS2_GPIO_Port GPIOC
#define ACCEL_ANALOG_Pin GPIO_PIN_0
#define ACCEL_ANALOG_GPIO_Port GPIOB
#define BATT_MEAS_Pin GPIO_PIN_1
#define BATT_MEAS_GPIO_Port GPIOB
#define ARMED_LED_Pin GPIO_PIN_13
#define ARMED_LED_GPIO_Port GPIOF
#define CENTRAL_NOMINAL_Pin GPIO_PIN_14
#define CENTRAL_NOMINAL_GPIO_Port GPIOF
#define CENTRAL_ERROR_Pin GPIO_PIN_15
#define CENTRAL_ERROR_GPIO_Port GPIOF
#define DROGUE_LED_Pin GPIO_PIN_0
#define DROGUE_LED_GPIO_Port GPIOG
#define MAIN_LED_Pin GPIO_PIN_1
#define MAIN_LED_GPIO_Port GPIOG
#define STORAGE_ERROR_Pin GPIO_PIN_7
#define STORAGE_ERROR_GPIO_Port GPIOE
#define ADC_ERROR_Pin GPIO_PIN_8
#define ADC_ERROR_GPIO_Port GPIOE
#define SENSOR_NOMINAL_Pin GPIO_PIN_10
#define SENSOR_NOMINAL_GPIO_Port GPIOE
#define SENSOR_ERROR_Pin GPIO_PIN_11
#define SENSOR_ERROR_GPIO_Port GPIOE
#define U1S_CHECK_Pin GPIO_PIN_12
#define U1S_CHECK_GPIO_Port GPIOE
#define U2S_CHECK_Pin GPIO_PIN_13
#define U2S_CHECK_GPIO_Port GPIOE
#define U3S_CHECK_Pin GPIO_PIN_14
#define U3S_CHECK_GPIO_Port GPIOE
#define U4S_CHECK_Pin GPIO_PIN_15
#define U4S_CHECK_GPIO_Port GPIOE
#define US2_ST_Pin GPIO_PIN_10
#define US2_ST_GPIO_Port GPIOB
#define US2_STATUS_Pin GPIO_PIN_11
#define US2_STATUS_GPIO_Port GPIOB
#define CHECK_HARDWARE_DEPLOYMENT_DISABLE_Pin GPIO_PIN_12
#define CHECK_HARDWARE_DEPLOYMENT_DISABLE_GPIO_Port GPIOB
#define DISARM_INPUT_Pin GPIO_PIN_4
#define DISARM_INPUT_GPIO_Port GPIOG
#define ADC_ALERT_Pin GPIO_PIN_8
#define ADC_ALERT_GPIO_Port GPIOC
#define CS3_Pin GPIO_PIN_0
#define CS3_GPIO_Port GPIOD
#define WP_Pin GPIO_PIN_1
#define WP_GPIO_Port GPIOD
#define HOLD_Pin GPIO_PIN_2
#define HOLD_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */
#define NDEBUG

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
#define CONTROL_LOGIC_TASK_DELAY (1000 / CONTROL_LOGIC_TASK_RATE / portTICK_RATE_MS)
#define MONITORING_TASK_DELAY    (1000 / MONITORING_TASK_RATE / portTICK_RATE_MS)
#define PROCESSING_TASK_DELAY    (1000 / PROCESSING_TASK_RATE / portTICK_RATE_MS)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
