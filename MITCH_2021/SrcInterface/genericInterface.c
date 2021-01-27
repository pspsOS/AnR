/*
 * genericSpi.c
 *
 * Author: Dawson Moore (11/9/2019)
 */

#include "genericInterface.h"


#ifdef _SPI_CONFIGURED
HAL_StatusTypeDef sendSPI(uint8_t * cmd, int len, GPIO_TypeDef * port, uint16_t pin, SPI_HandleTypeDef *bus)
{
	HAL_StatusTypeDef state;
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); //CS low
	state = HAL_SPI_Transmit(bus, cmd, len, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);	//CS high

	return state;
}

HAL_StatusTypeDef recieveSPI(uint8_t * cmd, int cmdLen, uint8_t * data, int dataLen, GPIO_TypeDef * port, uint16_t pin,  SPI_HandleTypeDef *bus)
{
	//Note: dataLen should be number of bytes in the register group being read
	HAL_StatusTypeDef state;
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); //CS low
	state = HAL_SPI_Transmit(bus, cmd, cmdLen, HAL_MAX_DELAY);
	HAL_SPI_Receive(bus, data, dataLen, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);	//CS high

	return state;
}
#endif
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
