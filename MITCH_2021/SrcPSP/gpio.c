/*
 * gpio.c
 *
 *  Created on: Jan 29, 2021
 *      Author: Jeff
 */

#include "gpio.h"
#include "main.h"

#ifdef LD2_Pin
void toggleLed() {
	static int j = 1;
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, j);
	j = !j;
}
#endif
