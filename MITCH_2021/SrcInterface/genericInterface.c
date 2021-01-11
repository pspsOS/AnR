/*
 * genericSpi.c
 *
 * Author: Dawson Moore (11/9/2019)
 */

#include <genericInterface.h>
#include "stm32f4xx_hal_gpio.h"


HAL_StatusTypeDef sendSPI(uint8_t * cmd, int len, GPIO_TypeDef * port, uint16_t pin)
{
	HAL_StatusTypeDef state;
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); //CS low
	state = HAL_SPI_Transmit(SPI_BUS, cmd, len, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);	//CS high

	return state;
}

HAL_StatusTypeDef recieveSPI(uint8_t * cmd, int cmdLen, uint8_t * data, int dataLen, GPIO_TypeDef * port, uint16_t pin)
{
	//Note: dataLen should be number of bytes in the register group being read
	HAL_StatusTypeDef state;
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); //CS low
	state = HAL_SPI_Transmit(SPI_BUS, cmd, cmdLen, HAL_MAX_DELAY);
	HAL_SPI_Receive(SPI_BUS, data, dataLen, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);	//CS high

	return state;
}

void handleHalError(uint8_t SENSOR)
{
	//Basically just to let us know that we ran into an issue during transmission
	//Toggle an LED, send error messages, or something similar
	//I don't know what you guys need, but this is for you to fill out
	// TODO: Implement handleHalError

	switch(SENSOR) {
	case BMP:
		*(sensors.bmpNomPtr) = false;
		break;
	}
}
