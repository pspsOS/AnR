#ifndef GENERIC_INTERFACE_H
#define GENERIC_INTERFACE_H

//Includes
#include "main.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <stdbool.h>

//Variables
extern SPI_HandleTypeDef  hspi1;

//Generic Defines
#define	GREAT					1
#define PSP						GREAT //RRaP is better (just saying)
#define SPI_BUS					&hspi1

// Sensor Defines
#define GPS 0
#define BMP 1
#define IMU 2
#define ALA 3


//Structs
typedef struct {
	//GPS Data
	char * gpsString;       //Use this if you like

	//Baro Data
	uint16_t senst1;		//C1 on datasheet
	uint16_t offt1; 		//C2 on datasheet
	uint16_t tcs;			//C3 on datasheet
	uint16_t tco;			//C4 on datasheet
	uint16_t tref;			//C5 on datasheet
	uint16_t tempsens;		//C6 on datasheet
	uint32_t digitalPres;	//D1 on datasheet (Only 24 bits will be filled)
	uint32_t digitalTemp;	//D2 on datasheet (Only 24 bits will be filled)
	int32_t	deltaT;			//dT on datasheet (This is a calculated value)
	int32_t temp;			//TEMP on datasheet
	int64_t off;			//OFF on datasheet (This is a calculated value)
	int64_t sens;			//SENS on datasheet (This is a calculated value)
	int32_t pressure;		//P on datasheet (This is a calculated value)
	bool *bmpNomPtr;

	//Linear Accel Data
	int16_t accel;

	//IMU Data

} sensors_t;

extern sensors_t sensors;

//Prototypes
HAL_StatusTypeDef sendSPI(uint8_t * cmd, int len, GPIO_TypeDef * port, uint16_t pin);
HAL_StatusTypeDef recieveSPI(uint8_t * cmd, int cmdLen, uint8_t * data, int dataLen, GPIO_TypeDef * port, uint16_t pin);
void handleHalError(uint8_t SENSOR);
#endif

/*
 * Notes:
 *
 * As of now, the rocket struct is here simply because I don't know where you guys will want it. Typically, I just declare one rocket_t variable and extern it
 * such that the whole application can access the data. There isn't really a chance that there will be errors with using a global since there's really only
 * one overall data structure.
 *
 */
