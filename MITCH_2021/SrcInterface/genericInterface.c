/*
 * genericSpi.c
 *
 * Author: Dawson Moore (11/9/2019)
 */

#include "genericInterface.h"

GPIO_PinState PSP_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	if(GPIO_Pin == FAKE_GPIO) return 0;
	else return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}

void PSP_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState, char* name) {
	if(GPIO_Pin != FAKE_GPIO) {
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);

	}
#if !defined(SUPRESS_SETUP_WARNING) && !defined(SUPRESS_ALL)
	else printf("! Setup Warning: %s not setup on this device.\r\n", name);
#endif
}


HAL_StatusTypeDef sendSPI(uint8_t * cmd, int len, GPIO_TypeDef * port, uint16_t pin, SPI_HandleTypeDef *bus)
{
	HAL_StatusTypeDef state = HAL_ERROR;
#ifdef _SPI_CONFIGURED
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); //CS low
	state = HAL_SPI_Transmit(bus, cmd, len, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);	//CS high
#endif
	return state;
}

HAL_StatusTypeDef recieveSPI(uint8_t * cmd, int cmdLen, uint8_t * data, int dataLen, GPIO_TypeDef * port, uint16_t pin,  SPI_HandleTypeDef *bus)
{

	//Note: dataLen should be number of bytes in the register group being read
	HAL_StatusTypeDef state = HAL_ERROR;
#ifdef _SPI_CONFIGURED
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); //CS low
	state = HAL_SPI_Transmit(bus, cmd, cmdLen, HAL_MAX_DELAY);
	HAL_SPI_Receive(bus, data, dataLen, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);	//CS high
#endif
	return state;
}

void handleHalError(Device_ID device)
{
	//Basically just to let us know that we ran into an issue during transmission
	//Toggle an LED, send error messages, or something similar
	//I don't know what you guys need, but this is for you to fill out
	// TODO: Implement handleHalError

	switch(device) {
	case BMP:
		*(sensors.bmpNomPtr) = false;
		break;
	default:
		break;
	}
}

srBank_t newSrBank(GPIO_TypeDef* GPIOx, uint16_T GPIO_Pin) {
	srBank_t bank = {0, 0, GPIOx, GPIO_Pin, 0};
	return bank;
}

led_t newPinLED(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	led_t led = {PIN_LED, GPIOx, GPIO_Pin, 0, 0};
	return led;
}
led_t newBankLED(srBank_t* bank, uint16_t GPIO_Pin) {
	led_t led = {BANK_LED, 0, GPIO_Pin, bank, 0};
	if(GPIO_Pin < 16)
		bank->registered = setBit(bank->registered, GPIO_Pin, 1);
	return led;
}
led_t newBankMixed(srBank_t* bank, uint16_t GPIO_Pin) {
	led_t led = {BANK_MIXED, 0, GPIO_Pin, bank, 0};
	if(GPIO_Pin < 16) {
		bank->registered = setBit(bank->registered, GPIO_Pin, 1);
		bank->types = setBit(bank->types, GPIO_Pin, 1);
	}
	return led;
}

