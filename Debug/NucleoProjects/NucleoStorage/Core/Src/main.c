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
#include "../IncInterface/nandInterface.h"
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
ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

UART_HandleTypeDef huart1;
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
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USB_OTG_FS_USB_Init(void);
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_USB_OTG_FS_USB_Init();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_USB_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

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
  HAL_GPIO_WritePin(WP_GPIO_Port, WP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HOLD_GPIO_Port, HOLD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CS3_Pin|LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : WP_Pin */
  GPIO_InitStruct.Pin = WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : HOLD_Pin */
  GPIO_InitStruct.Pin = HOLD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HOLD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA10 PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CS3_Pin LED_Pin */
  GPIO_InitStruct.Pin = CS3_Pin|LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//bool printDiv = true;
bool printDiv = false;
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
	uint32_t rowAddr = 0x00000000;
	uint16_t colAddr = 0x0000;
	uint8_t writeData1[16] = {0x01, 0x02, 0x03, 0x04, 0x11, 0x12, 0x13, 0x14, 0x21, 0x22, 0x23, 0x24, 0x31, 0x32, 0x33, 0x34};
	uint8_t writeData2[16] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
	uint8_t readData[16] = {0x00};
	uint8_t size = 16;
	uint8_t feature = 0x00;
	uint32_t goodRows = 0;
	uint32_t badRows = 0;
	uint32_t i = 0;
	bool badRow = false;
	bool nandNominal = false;

	if(ENABLE_CONTROL_LOGIC || true){
		HAL_GPIO_WritePin(WP_GPIO_Port, WP_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(HOLD_GPIO_Port, HOLD_Pin, GPIO_PIN_SET);

		nandInit(&nandNominal);

		setFeature(0xA0, 0x00);
		feature = getFeature(0xA0);
		printf("Feature 02X ", feature);
		feature = getFeature(0xB0);
		printf("02X \n\r", feature);

		//feature = getFeature(0xC0);
		//printf("%2X ", feature);

		//for(int i = 0; i < sizeR; i++) printf("%X ", readData[i]);
		//eraseBlock(rowAddr);
		/*writeEnable();
		nandBufferLoad(rowAddr);
		nandBufferWrite(colAddr, writeData, sizeW);
		nandBufferRead(colAddr, readData, sizeR);
		for(int i = 0; i < sizeR; i++) printf("%X ", readData[i]);
		printf("\n\r");
		writeEnable();
		//HAL_Delay(10);
		nandBufferExecute(rowAddr);*/
		//nandBufferRead(colAddr, readData, sizeR);
		eraseBlock(rowAddr);
		nandWrite(rowAddr, colAddr, writeData1, size);
		nandRead(rowAddr, colAddr, readData, size);
		for(i = 0; i < size; i++) printf("%02X ", readData[i]);
		printf("\n\r");

		nandWrite(rowAddr, colAddr, writeData2, size);
		nandRead(rowAddr, colAddr, readData, size);
		for(i = 0; i < size; i++) printf("%02X ", readData[i]);
		printf("\n\r");

		//for(int j = 0; j < 100000; j++) printf("%02X ", j);

		/*for (rowAddr = 0; rowAddr < 131072; rowAddr+=1){
			nandWrite(rowAddr, colAddr, writeData1, size);
			//nandRead(rowAddr, colAddr, readData, size);
			eraseBlock(rowAddr);
			nandWrite(rowAddr, colAddr, writeData2, size);
			nandRead(rowAddr, colAddr, readData, size);

			badRow = false;
			for (int i=0; i<size; i++){
				if(readData[i] != writeData2[i]){
					badRow = true;
				}
			}
			if(badRow){
				printf("\n\rBAD ROW!: %d", rowAddr);
				badRows++;
			}else{
				//printf("\n\rgood row: %d", rowAddr);
				goodRows++;
			}

		}*/
		printf("\n\rDone");

	}
  /* Infinite loop */
	while(ENABLE_CONTROL_LOGIC) {
		if(printDiv) printf("-------------\r\n");
		//toggleLed();
		vTaskDelayUntil(&time_init, CONTROL_LOGIC_TASK_DELAY);
	}
	vTaskDelete(NULL);
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
	if(ENABLE_ACQUISITION){
		setup_A();
		printf("\n\n");
	}
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
	if(ENABLE_ACQUISITION) printDiv = false;
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
	if(false) {
	uint8_t cmd = 0x01;

	spiLock_t* ledSpiLock = registerSpiLock();
	setSpiLock(NAND_CS_GPIO_Port, NAND_CS_Pin, ledSpiLock);
	HAL_Delay(1000);
	lockSpi(ledSpiLock);
	sendSPI(&cmd, 1, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS);
	HAL_Delay(3000);
	unlockSpi(ledSpiLock);


	static TickType_t time_init = 0;

  /* Infinite loop */
	while(ENABLE_PROCESSING) {

		HAL_GPIO_WritePin(ledSpiLock->port, ledSpiLock->pin, GPIO_PIN_RESET);
		printf("%d \n\r", ledSpiLock->pin);
		printf("%d \n\n\r", NAND_CS_Pin);
		HAL_Delay(1000);
		lockSpi(ledSpiLock);
		/*if (sendSPI(0x01, 1, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
				handleHalError(NAND);
				return;
		}
		HAL_Delay(1000);
		if (sendSPI(0x01, 1, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
			handleHalError(NAND);
			return;
		}
		HAL_Delay(4000);
		unlockSpi(ledSpiLock);
		HAL_Delay(1000);
		*/
		vTaskDelayUntil(&time_init, PROCESSING_TASK_DELAY);
	}
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
	static GPIO_PinState pressed = GPIO_PIN_SET;
	static int counter = 0;
	static TickType_t time_init = 0;
  /* Infinite loop */
	while(ENABLE_MONITORING) {
		printf("MONITORING\n\r");
		//loop_M();
		/*if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != pressed) {
			pressed = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
			if(!pressed) printf("PRESSED  %d\r\n", ++counter);
			else printf("RELEASED %d\r\n", counter);
		}*/


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
