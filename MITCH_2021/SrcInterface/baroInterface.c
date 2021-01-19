/*
 * baro.c
 *
 * Author: Dawson Moore (11/9/2019)
 */

#include <baroInterface.h>


/**
 * @brief Barometer Read
 * Gets the latest temperature and pressure values from the barometer.
 *
 * @param *temperature: Pointer to store temperature value.
 * @param *pressure: Pointer to store pressure value.
 *
 * @author Dawson Moore
 * @author Jeff Kaji
 * @date 01/11/2021
 */
void barometerRead(int32_t *temperature, int32_t *pressure) {
	// Variables
	uint8_t dataIn[2]; // Buffer to load data received
	uint8_t cmd;       // Command sent to device

	//Get values from sensor
	cmd = D1_1024; //This value will define conversion time, accuracy, and current draw
	if (sendSPI(&cmd, 1, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
	HAL_Delay(CONV_T_1024);

	cmd = 0x00;
	if (recieveSPI(&cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
	sensors.digitalPres = (dataIn[0] << 8) | dataIn[1];

	cmd = D2_1024; //This value will define conversion time, accuracy, and current draw
	if (sendSPI(&cmd, 1, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
	HAL_Delay(CONV_T_1024);

	cmd = 0x00;
	if (recieveSPI(&cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
	sensors.digitalTemp = (dataIn[0] << 8) | dataIn[1];

	//Calculate calibrated pressure
	//T = D2 - TREF = D2 - C5 * 2 ^ 8
	sensors.deltaT = sensors.digitalTemp - sensors.tref * 256;
	//TEMP = 20°C + dT* TEMPSENS = 2000 + dT * C6 / 2 ^ 23
	sensors.temp = 2000 + sensors.deltaT * sensors.tempsens / 8388608;
	//OFF = OFFT1 + TCO* dT = C2 * 2 ^ 17 + (C4 * dT) / 2 ^ 6
	sensors.off = sensors.offt1 * 131072 + (sensors.tco * sensors.deltaT) / 64;
	//SENS = SENST1 + TCS * dT = C1 * 2 ^ 16 + (C3 * dT) / 2 ^ 7
	sensors.sens = sensors.senst1 * 65536 + (sensors.tcs * sensors.deltaT) / 128;
	//P = D1 * SENS - OFF = (D1 * SENS / 2 ^ 21 - OFF) / 2 ^ 15
	sensors.pressure = (sensors.digitalPres * sensors.sens / 2097152 - sensors.off) / 32768; //This is the magic number in mbar

	if(*sensors.bmpNomPtr) {
		*temperature = sensors.temp;
		*pressure = sensors.pressure;
	}
}



/**
 * @brief barometer Initialization
 * Initialized the barometer upon power-on.
 *
 * @param *bmpNomPtr: Pointer to bmpNominal to set sensor status.
 *
 * @author Dawson Moore
 * @author Jeff Kaji
 * @date 01/11/2021
 */
void barometerInit(bool *bmpNomPtr) {
	// Variables
	uint8_t dataIn[2]; // Buffer to load data received
	uint8_t cmd;       // Command sent to device

	//Initialize bmpNominal as true
	sensors.bmpNomPtr = bmpNomPtr;
	*(sensors.bmpNomPtr) = true;

	//Reset baro after power on
	cmd = 0x1E;
	if (sendSPI(&cmd, 1, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}

	//Get value of C1
	cmd = 0xA1;
	if (recieveSPI(&cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
	sensors.senst1 = (dataIn[0] << 8) | dataIn[1];

	//Get value of C2
	cmd = 0xA2;
	if (recieveSPI(&cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
	sensors.offt1 = (dataIn[0] << 8) | dataIn[1];

	//Get value of C3
	cmd = 0xA3;
	if (recieveSPI(&cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
	sensors.tcs = (dataIn[0] << 8) | dataIn[1];

	//Get value of C4
	cmd = 0xA4;
	if (recieveSPI(&cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
	sensors.tco = (dataIn[0] << 8) | dataIn[1];

	//Get value of C5
	cmd = 0xA5;
	if (recieveSPI(&cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
	sensors.tref = (dataIn[0] << 8) | dataIn[1];

	//Get value of C6
	cmd = 0xA6;
	if (recieveSPI(&cmd, 1, dataIn, 2, BARO_CS_GPIO_Port, BARO_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
	sensors.tempsens = (dataIn[0] << 8) | dataIn[1];
}


