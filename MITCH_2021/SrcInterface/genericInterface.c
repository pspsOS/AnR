/*
 * genericSpi.c
 *
 * Author: Dawson Moore (11/9/2019)
 */

#include "genericInterface.h"

uint8_t _spiLocksRegistered = (uint8_t) 0;

GPIO_PinState PSP_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	if(GPIO_Pin == FAKE_GPIO) return 0;
	else return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}

void PSP_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState pinState, char* name) {
	if(GPIO_Pin != FAKE_GPIO) {
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, pinState);

	}
#if !defined(SUPRESS_SETUP_WARNING) && !defined(SUPRESS_ALL)
	else printf("! Setup Warning: %s not setup on this device.\r\n", name);
#endif
}

spiLock_t* registerSpiLock() {
	if(_spiLocksRegistered < NUM_SPI_LOCKS) {
		_spiLocks[_spiLocksRegistered].lock = false;
		_spiLocks[_spiLocksRegistered].pin = 0x69;
		return &_spiLocks[_spiLocksRegistered++];
	}
	else return NULL;
}

void setSpiLock(GPIO_TypeDef* port, uint16_t pin, spiLock_t* locker) {
	if(locker->lock) {
		if(pin != FAKE_GPIO) HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
		locker->lock = false;
	}
	locker->port = port;
	locker->pin = pin;
	if(pin != FAKE_GPIO) HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);

	//printf("Pin: %X\r\n", locker->pin);
}

void lockSpi(spiLock_t* locker) {
	if(locker->pin != FAKE_GPIO){
		HAL_GPIO_WritePin(locker->port, locker->pin, GPIO_PIN_RESET);
	}
	locker->lock = true;
}
void unlockSpi(spiLock_t* locker) {
	if(locker->pin != FAKE_GPIO){
		HAL_GPIO_WritePin(locker->port, locker->pin, GPIO_PIN_SET);
	}
	locker->lock = false;
}

HAL_StatusTypeDef sendSPI(uint8_t * cmd, int len, GPIO_TypeDef * port, uint16_t pin, SPI_HandleTypeDef *bus)
{
	HAL_StatusTypeDef state = HAL_ERROR;
	if (pin == FAKE_GPIO) return state;
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); //CS low
	state = HAL_SPI_Transmit(bus, cmd, len, HAL_MAX_DELAY);
	for(int i = 0; i < _spiLocksRegistered; i++)
		if((_spiLocks[i]).port == port && (_spiLocks[i]).pin == pin)
			if(_spiLocks[i].lock == true) goto bypass_unlock;
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);	//CS high
	bypass_unlock:

	return state;
}

HAL_StatusTypeDef recieveSPI(uint8_t * cmd, int cmdLen, uint8_t * data, int dataLen, GPIO_TypeDef * port, uint16_t pin,  SPI_HandleTypeDef *bus)
{

	//Note: dataLen should be number of bytes in the register group being read
	HAL_StatusTypeDef state = HAL_ERROR;
	if (pin == FAKE_GPIO) return state;
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); //CS low
	state = HAL_SPI_Transmit(bus, cmd, cmdLen, HAL_MAX_DELAY);
	HAL_SPI_Receive(bus, data, dataLen, HAL_MAX_DELAY);
	for(int i = 0; i < _spiLocksRegistered; i++)
		if((_spiLocks[i]).port == port && (_spiLocks[i]).pin == pin)
			if(_spiLocks[i].lock == true) goto bypass_unlock;
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);	//CS high
	bypass_unlock:

	return state;
}

void handleHalError(Device_ID device)
{
	//Basically just to let us know that we ran into an issue during transmission
	//Toggle an LED, send error messages, or something similar
	//I don't know what you guys need, but this is for you to fill out
	// TODO: Implement handleHalError
	extern spiLock_t* nandSpiLock;

	if(device == NAND) unlockSpi(nandSpiLock);

	nomPtr[device] = false;
}

srBank_t newSrBank(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	srBank_t bank = {0, 0, GPIOx, GPIO_Pin, 0, 0, false};
	return bank;
}

led_t newPinLed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	led_t led = {PIN_LED, GPIOx, GPIO_Pin, 0};
	return led;
}

led_t newBankLed(srBank_t* bank, uint16_t GPIO_Pin) {
	led_t led = {BANK_LED, 0, GPIO_Pin, bank};
	if(GPIO_Pin < 16)
		bank->registered = setBit(bank->registered, GPIO_Pin, 1);
	return led;
}

led_t newBankMixed(srBank_t* bank, uint16_t GPIO_Pin) {
	led_t led = {BANK_MIXED, 0, GPIO_Pin, bank};
	if(GPIO_Pin < 16) {
		bank->registered = setBit(bank->registered, GPIO_Pin, 1);
		bank->types = setBit(bank->types, GPIO_Pin, 1);
	}
	return led;
}

void setLed(led_t* led, GPIO_PinState pinState) {
	uint16_t oldState = 0;

	switch(led->type) {
	case PIN_LED:
		if(led->GPIO_Pin != FAKE_GPIO) HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, pinState);
		break;
	case BANK_LED:
	case BANK_MIXED:
		oldState = led->bank->state;
		led->bank->state = (led->bank->state | led->GPIO_Pin);
		if(pinState == GPIO_PIN_RESET) led->bank->state -= led->GPIO_Pin;
		if(oldState != led->bank->state) led->bank->hasUpdate = true;
		break;
	}
}

void setSrBank(srBank_t bank) {

}
