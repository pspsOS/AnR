/*
 * baro.c
 *
 * Author: Dawson Moore (11/9/2019)
 */

#include "baro.h"

/*
 * Function: barometerTask()
 *
 * Gathers pressure using raw temp and pressure for altitude
 * calculations
 *
 */

void barometerTask()
{
	barometerInit();
	TickType_t time_init = 0;
	uint8_t dataIn[2];
	uint8_t cmd;
	while (PSP == GREAT)
	{
		//Get values from sensor
		cmd = D1_1024; //This value will define conversion time, accuracy, and current draw
		if (sendSpi(cmd, 1, BARO_CS_GPIO_Port, BARO_CS_Pin))
		{
			handleHalError();
		}
		HAL_Delay(CONV_T_1024);

		cmd = 0x00;
		if (recieveSPI(cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin))
		{
			handleHalError();
		}
		rocket.digitalPres = (dataIn[0] << 8) | dataIn[1];

		cmd = D2_1024; //This value will define conversion time, accuracy, and current draw
		if (sendSpi(cmd, 1, BARO_CS_GPIO_Port, BARO_CS_Pin))
		{
			handleHalError();
		}
		HAL_Delay(CONV_T_1024);

		cmd = 0x00;
		if (recieveSPI(cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin))
		{
			handleHalError();
		}
		rocket.digitalTemp = (dataIn[0] << 8) | dataIn[1];

		//Calculate calibrated pressure
		//T = D2 - TREF = D2 - C5 * 2 ^ 8
		rocket.deltaT = rocket.digitalTemp - rocket.tref * 256;
		//TEMP = 20°C + dT* TEMPSENS = 2000 + dT * C6 / 2 ^ 23
		rocket.temp = 2000 + rocket.deltaT * rocket.tempsens / 8388608;
		//OFF = OFFT1 + TCO* dT = C2 * 2 ^ 17 + (C4 * dT) / 2 ^ 6
		rocket.off = rocket.offt1 * 131072 + (rocket.tco * rocket.deltaT) / 64;
		//SENS = SENST1 + TCS * dT = C1 * 2 ^ 16 + (C3 * dT) / 2 ^ 7
		rocket.sens = rocket.senst1 * 65536 + (rocket.tcs * rocket.deltaT) / 128;
		//P = D1 * SENS - OFF = (D1 * SENS / 2 ^ 21 - OFF) / 2 ^ 15
		rocket.pressure = (rocket.digitalPres * rocket.sens / 2097152 - rocket.off) / 32768; //This is the magic number in mbar

		vTaskDelayUntil(&time_init, BARO_RATE);
	}
}

/*
 * Function: barometerInit()
 *
 * Gathers the factory values for C1-C6 for calculation of digital
 * pressure and temperature
 *
 */

void barometerInit()
{
	uint8_t dataIn[2];
	uint8_t cmd;

	//Reset baro after power on
	cmd = 0x1E;
	if (sendSpi(cmd, 1, BARO_CS_GPIO_Port, BARO_CS_Pin))
	{
		handleHalError();
	}

	//Get value of C1
	cmd = 0xA1;
	if (recieveSPI(cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin))
	{
		handleHalError();
	}
	rocket.senst1 = (dataIn[0] << 8) | dataIn[1];

	//Get value of C2
	cmd = 0xA2;
	if (recieveSPI(cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin))
	{
		handleHalError();
	}
	rocket.offt1 = (dataIn[0] << 8) | dataIn[1];

	//Get value of C3
	cmd = 0xA3;
	if (recieveSPI(cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin))
	{
		handleHalError();
	}
	rocket.tcs = (dataIn[0] << 8) | dataIn[1];

	//Get value of C4
	cmd = 0xA4;
	if (recieveSPI(cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin))
	{
		handleHalError();
	}
	rocket.tco = (dataIn[0] << 8) | dataIn[1];

	//Get value of C5
	cmd = 0xA5;
	if (recieveSPI(cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin))
	{
		handleHalError();
	}
	rocket.tref = (dataIn[0] << 8) | dataIn[1];

	//Get value of C6
	cmd = 0xA6;
	if (recieveSPI(cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin))
	{
		handleHalError();
	}
	rocket.tempsens = (dataIn[0] << 8) | dataIn[1];
}
