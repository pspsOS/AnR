/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "task.h"
/* User-defined libraries */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "retarget.h"

#include "../IncPSP/acquisition.h"
#include "../IncPSP/common.h"
#include "../IncPSP/controlLogic.h"
#include "../IncPSP/monitoring.h"
#include "../IncPSP/processing.h"
#include "../IncPSP/storage.h"
#include "../IncPSP/transmission.h"
#include "../IncPSP/gpio.h"
#include <string.h>

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
UART_HandleTypeDef huart2;

osThreadId ControlLogicHandle;
uint32_t ControlLogicBuffer[ 128 ];
osStaticThreadDef_t ControlLogicControlBlock;
osThreadId AcquisitionHandle;
uint32_t AcquisitionBuffer[ 128 ];
osStaticThreadDef_t AcquisitionControlBlock;
osThreadId ProcessingHandle;
uint32_t ProcessingBuffer[ 128 ];
osStaticThreadDef_t ProcessingControlBlock;
osThreadId MonitoringHandle;
uint32_t MonitoringBuffer[ 128 ];
osStaticThreadDef_t MonitoringControlBlock;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void startControlLogic(void const * argument);
void startAcquisition(void const * argument);
void startProcessing(void const * argument);
void startMonitoring(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  RetargetInit(&huart2);
  for(int i = 0; i < 500; i++) printf(" \r\n");
  HAL_Delay(250);
  printf("Starting:\r\n");
  HAL_Delay(250);
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of ControlLogic */
  osThreadStaticDef(ControlLogic, startControlLogic, osPriorityNormal, 0, 128, ControlLogicBuffer, &ControlLogicControlBlock);
  ControlLogicHandle = osThreadCreate(osThread(ControlLogic), NULL);

  /* definition and creation of Acquisition */
  osThreadStaticDef(Acquisition, startAcquisition, osPriorityRealtime, 0, 128, AcquisitionBuffer, &AcquisitionControlBlock);
  AcquisitionHandle = osThreadCreate(osThread(Acquisition), NULL);

  /* definition and creation of Processing */
  osThreadStaticDef(Processing, startProcessing, osPriorityNormal, 0, 128, ProcessingBuffer, &ProcessingControlBlock);
  ProcessingHandle = osThreadCreate(osThread(Processing), NULL);

  /* definition and creation of Monitoring */
  osThreadStaticDef(Monitoring, startMonitoring, osPriorityLow, 0, 128, MonitoringBuffer, &MonitoringControlBlock);
  MonitoringHandle = osThreadCreate(osThread(Monitoring), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|SENSE_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin SENSE_A_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|SENSE_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
bool printDiv = true;
/* USER CODE END 4 */

/* USER CODE BEGIN Header_startControlLogic */
/**
  * @brief  Function implementing the ControlLogic thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_startControlLogic */
void startControlLogic(void const * argument)
{
  /* USER CODE BEGIN 5 */
	static TickType_t time_init = 0;
  /* Infinite loop */
	while(ENABLE_CONTROL_LOGIC) {

		if(printDiv) printf("-------------\r\n");
		toggleLed();

		vTaskDelayUntil(&time_init, CONTROL_LOGIC_TASK_DELAY);
	}

  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_startAcquisition */
/**
* @brief Function implementing the Acquisition thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startAcquisition */
void startAcquisition(void const * argument)
{
  /* USER CODE BEGIN startAcquisition */
	static TickType_t time_init = 0;

	setup_A();
	printf("\n\n");
  /* Infinite loop */
	while(ENABLE_ACQUISITION) {
		switch(loop_A()) {
		case 1: vTaskDelayUntil(&time_init, ACQUISITION_TASK_DELAY1);
				break;

		case 2: vTaskDelayUntil(&time_init, ACQUISITION_TASK_DELAY2);
				break;

		default:vTaskDelayUntil(&time_init, ACQUISITION_TASK_DELAY0);
				break;
		}
		if(time_init > 4000) break;
	}
	//printf("%d\n\r",time_init);
	printDiv = false;
	vTaskDelete(NULL);

  /* USER CODE END startAcquisition */
}

/* USER CODE BEGIN Header_startProcessing */
/**
* @brief Function implementing the Processing thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startProcessing */
void startProcessing(void const * argument)
{
  /* USER CODE BEGIN startProcessing */
  /* Infinite loop */
	static GPIO_PinState pressed = GPIO_PIN_SET;
	static int counter = 0;

	static TickType_t time_init = 0;
  /* Infinite loop */
	while(ENABLE_PROCESSING) {
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != pressed) {
			pressed = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
			if(!pressed) printf("PRESSED  %d\r\n", ++counter);
			else printf("RELEASED %d\r\n", counter);
		}
		vTaskDelayUntil(&time_init, PROCESSING_TASK_DELAY);
	}
	vTaskDelete(NULL);
  /* USER CODE END startProcessing */
}

/* USER CODE BEGIN Header_startMonitoring */
/**
* @brief Function implementing the Monitoring thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startMonitoring */
void startMonitoring(void const * argument)
{
  /* USER CODE BEGIN startMonitoring */
	static TickType_t time_init = 0;

	extern bool continuity[4];

	printf("Monitoring Start\r\n");

	setup_M();


  /* Infinite loop */
	while(ENABLE_MONITORING) {
		//loop_M();

		checkContinuity_M();
		printf("Continuity: ");
		for(int i = 0; i < 4; i++) printf("%d ", continuity[i]);
		printf("\r\n");
		vTaskDelayUntil(&time_init, MONITORING_TASK_DELAY);
	}

	vTaskDelete(NULL);
  /* USER CODE END startMonitoring */
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
