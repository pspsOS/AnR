#ifndef GENERIC_INTERFACE_H
#define GENERIC_INTERFACE_H

//Includes
#include "main.h"
#include <stdbool.h>
#include <stdio.h>
//#include "common.h"
#include "gpio.h"

#ifdef STM32F4xx_HAL_SPI_H
	#define _SPI_CONFIGURED
#endif

//Variables
#ifdef _SPI_CONFIGURED
	extern SPI_HandleTypeDef  hspi1;
	extern SPI_HandleTypeDef  hspi3;
#elif !defined(STM32F4xx_HAL_SPI_H)
	typedef void SPI_HandleTypeDef;
#endif

//Generic Defines
#define	GREAT					1
#define PSP						GREAT //RRaP is better (just saying)

#define ADC_VREF				3.3

#define NUM_DEVICES (5)
typedef enum {
	GPS = 0,
	BMP = 1,
	IMU = 2,
	ALA = 3,
	NAND = 4,
} Device_ID;

bool* nomPtr[NUM_DEVICES];

//Structs
typedef struct {
	//GPS Data
	char * gpsString;       //Use this if you like

	//Linear Accel Data
	int16_t accel;

	//IMU Data

} sensors_t;

sensors_t sensors;

typedef struct srBank {
	uint16_t registered;
	uint16_t types;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	uint16_t state;
	uint16_t read;
	bool hasUpdate;
} srBank_t;


typedef enum {
	PIN_LED = 0,
	BANK_LED = 1,
	BANK_MIXED = 2
} LED_Type;

typedef struct led {
	LED_Type type;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	srBank_t* bank;
} led_t;

srBank_t newSrBank(GPIO_TypeDef* GPIOx, uint16_t);
led_t newPinLed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
led_t newBankLed(srBank_t* bank, uint16_t GPIO_Pin);
led_t newBankMixed(srBank_t* bank, uint16_t GPIO_Pin);
void setLed(led_t* led, GPIO_PinState pinState);
void setSrBank(srBank_t bank);

//Prototypes
#ifdef _SPI_CONFIGURED
#define SENSORS_SPI_BUS					&hspi1
#define STORAGE_SPI_BUS					&hspi3
#else
#define SENSORS_SPI_BUS					0
#define STORAGE_SPI_BUS					0
#endif

HAL_StatusTypeDef sendSPI(uint8_t * cmd, int len, GPIO_TypeDef * port, uint16_t pin, SPI_HandleTypeDef *bus);
HAL_StatusTypeDef recieveSPI(uint8_t * cmd, int cmdLen, uint8_t * data, int dataLen, GPIO_TypeDef * port, uint16_t pin, SPI_HandleTypeDef *bus);

void handleHalError(uint8_t SENSOR);


GPIO_PinState PSP_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void PSP_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState, char* name);

/*
 * Notes:
 *
 * As of now, the rocket struct is here simply because I don't know where you guys will want it. Typically, I just declare one rocket_t variable and extern it
 * such that the whole application can access the data. There isn't really a chance that there will be errors with using a global since there's really only
 * one overall data structure.
 *
 */
#endif
