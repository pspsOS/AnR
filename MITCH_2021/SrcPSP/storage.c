/*
 * storage.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/storage.h"
#ifdef NDEBUG
	#include "nandInterface.h"
#endif
// Setup

bool nandNominal = false;

/* TODO: Implement storageSetup
 * Established connection with storage device.
 *
 * Author: Jeff Kaji
 * Date: 12/24/2020
 */
bool storageSetup() {
#ifdef NDEBUG
	nandInit(&nandNominal);
#else
	nandNominal = false;
#endif
	return nandNominal;
}


// Loop

/* TODO: Implement storageWrite
 * Write data to storage device
 *
 * Author: Jeff Kaji
 * Date: 12/24/2020
 */
void storageWrite() {

}

/**
 * @brief stores Daq Status
 * Stores values from the daqStatus Struct
 *
 * @author Ryan Horvath, Vishnu Vijay
 * @date 1/30/21
 */
void storeDaqStatus() {
	if (g_daqStatusData.lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_daqStatusData.lock = true;

	uint8_t dataStream[11] = {0};
	dataStream[0] = NOMINAL_DAQ_STATUS;
	uint8_t size = 1;

	VLQ_t genericVLQ = convertToUVLQ(g_daqStatusData.timeStamp);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	dataStream[size++] = g_daqStatusData.daqScalingEnabled;
	dataStream[size++] = g_daqStatusData.gpsNominal;
	dataStream[size++] = g_daqStatusData.bmpNominal;
	dataStream[size++] = g_daqStatusData.imuNominal;
	dataStream[size++] = g_daqStatusData.alaNominal;

	g_daqStatusData.lock = false;

	uint8_t funcReturn = writeToStorage(dataStream, size);
	if (funcReturn == FAILED_FILE_WRITE) {
		printf("Failed File Write\n");
	}
	else if (funcReturn == SUCCESSFUL_FILE_WRITE) {
		printf("Successful File Write\n");
	}
}

/**
 * @brief stores GPS data
 * Stores values from the gpsData Struct
 *
 * @author Ryan Horvath
 * @date 1/30/21
 */
void storeGpsData() {
	if (g_gpsData.lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_gpsData.lock = true;

	uint8_t dataStream[6] = {0};
	dataStream[0] = NOMINAL_GPS_DATA;
	uint8_t size = 1;

	//compressing Time Stamp
	VLQ_t genericVLQ = convertToUVLQ(g_gpsData.timeStamp);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	writeToStorage((uint8_t *) g_gpsData.nmeaGGA, MAX_NMEA);
	writeToStorage((uint8_t *) g_gpsData.nmeaRMC, MAX_NMEA);

	g_gpsData.lock = false;

	uint8_t funcReturn = writeToStorage(dataStream, size);
	if (funcReturn == FAILED_FILE_WRITE) {
		printf("Failed File Write\n");
	}
	else if (funcReturn == SUCCESSFUL_FILE_WRITE) {
		printf("Successful File Write\n");
	}
}

/**
 * @brief stores BMP data
 * Stores values from the bmpData Struct
 *
 * @author Ryan Horvath, Vishnu Vijay
 * @date 1/30/21
 */
void storeBmpData() {
	if (g_bmpData.lock) {
			retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_bmpData.lock = true;

	uint8_t dataStream[16] = {0};
	dataStream[0] = NOMINAL_BMP_DATA;
	uint8_t size = 1;

	//Compressing Time Stamp
	VLQ_t genericVLQ = convertToUVLQ(g_bmpData.timeStamp);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	//Compressing Pressure
	memset(&genericVLQ, 0, sizeof(genericVLQ));
	genericVLQ = convertToSVLQ(g_bmpData.pressure);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	//Compressing Temperature
	memset(&genericVLQ, 0, sizeof(genericVLQ));
	genericVLQ = convertToSVLQ(g_bmpData.temperature);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	g_bmpData.lock = false;

	uint8_t funcReturn = writeToStorage(dataStream, size);
	if (funcReturn == FAILED_FILE_WRITE) {
		printf("Failed File Write\n");
	}
	else if (funcReturn == SUCCESSFUL_FILE_WRITE) {
		printf("Successful File Write\n");
	}
}

/**
 * @brief stores Imu Data
 * Stores values from the imuData Struct
 *
 * @author Ryan Horvath, Vishnu Vijay
 * @date 1/30/21
 */
void storeImuData() {
	if (g_imuData.lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_imuData.lock = true;

	uint8_t dataStream[1 + 5 + 10 * (3)] = {0};
	dataStream[0] = NOMINAL_IMU_DATA;
	uint8_t size = 1;

	//Compressing Time Stamp
	VLQ_t genericVLQ = convertToUVLQ(g_imuData.timeStamp);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	//Compress imu data
	int16_t tempArr[sizeof(g_imuData) / 2] = {0};
	memcpy(&tempArr, (const void *) g_imuData, sizeof(g_imuData));
	for (uint8_t i = sizeof(g_imuData.timeStamp); i < sizeof(g_imuData); i += sizeof(int16_t)) {
		int16_t tempInt = 0; //((int16_t *) g_imuData)[i];
		//memcpy(&tempInt, g_imuData[i], sizeof(int16_t));
		memset(&genericVLQ, 0, sizeof(genericVLQ));
		genericVLQ = convertToSVLQ(tempInt);
		for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
			switch (genericVLQ.quantityLength) {
				case 1:
					dataStream[i] = genericVLQ.oneByteVLQ[i - size];
					break;
				case 2:
					dataStream[i] = genericVLQ.twoByteVLQ[i - size];
					break;
				case 3:
					dataStream[i] = genericVLQ.threeByteVLQ[i - size];
					break;
				case 4:
					dataStream[i] = genericVLQ.fourByteVLQ[i - size];
					break;
			}
		}
		size += genericVLQ.quantityLength;
	}

	g_imuData.lock = false;

	uint8_t funcReturn = writeToStorage(dataStream, size);
	if (funcReturn == FAILED_FILE_WRITE) {
		printf("Failed File Write\n");
	}
	else if (funcReturn == SUCCESSFUL_FILE_WRITE) {
		printf("Successful File Write\n");
	}
}

/**
 * @brief stores Processed Data
 * Stores values from the processedData Struct
 *
 * @author Ryan Horvath
 * @date 1/30/21
 */
void storeProcessedData() {
	if (g_processedData.lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_processedData.lock = true;

	uint8_t dataStream[10] = {0};
	dataStream[0] = NOMINAL_PROCESSED_DATA;
	uint8_t size = 1;

	//compressing Time Stamp
	VLQ_t genericVLQ = convertToUVLQ(g_processedData.timeStamp);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	//compressing altitude
	memset(&genericVLQ, 0, sizeof(genericVLQ));
	genericVLQ = convertToSVLQ(g_processedData.altitude);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	//compressing velocity in x direction
	memset(&genericVLQ, 0, sizeof(genericVLQ));
	genericVLQ = convertToSVLQ(g_processedData.vel_xout);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	//compressing velocity in y direction
	memset(&genericVLQ, 0, sizeof(genericVLQ));
	genericVLQ = convertToSVLQ(g_processedData.vel_yout);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	//compressing velocity in z direction
	memset(&genericVLQ, 0, sizeof(genericVLQ));
	genericVLQ = convertToSVLQ(g_processedData.vel_zout);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	g_processedData.lock = false;

	uint8_t funcReturn = writeToStorage(dataStream, size);
	if (funcReturn == FAILED_FILE_WRITE) {
		printf("Failed File Write\n");
	}
	else if (funcReturn == SUCCESSFUL_FILE_WRITE) {
		printf("Successful File Write\n");
	}
}

/**
 * @brief stores Monitoring Data
 * Stores values from the monitoringData Struct
 *
 * @author Ryan Horvath, Vishnu Vijay
 * @date 1/30/21
 */
void storeMonitoringData() {
	if (g_monitoringData.lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_monitoringData.lock = true;

	uint8_t dataStream[sizeof(g_monitoringData) + 2] = {0};
	dataStream[0] = NOMINAL_MONITOR_DATA;
	uint8_t size = 1;

	//Compressing Time Stamp
	VLQ_t genericVLQ = convertToUVLQ(g_bmpData.timeStamp);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	//Compressing battery voltage
	memset(&genericVLQ, 0, sizeof(genericVLQ));
	genericVLQ = convertToSVLQ(g_monitoringData.batteryVoltage);
	for (uint8_t i = size; i < genericVLQ.quantityLength + size; i++) {
		switch (genericVLQ.quantityLength) {
			case 1:
				dataStream[i] = genericVLQ.oneByteVLQ[i - size];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i - size];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i - size];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i - size];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	//Assign bools to data stream
	for (int8_t i = 0; i < 4; i++) {
		dataStream[size++] = g_monitoringData.continuity[i];
	}
	dataStream[size++] = g_monitoringData.buttonState;
	dataStream[size++] = g_monitoringData.hardwareDeploymentDisable;

	g_monitoringData.lock = false;

	uint8_t funcReturn = writeToStorage(dataStream, size);
	if (funcReturn == FAILED_FILE_WRITE) {
		printf("Failed File Write\n");
	}
	else if (funcReturn == SUCCESSFUL_FILE_WRITE) {
		printf("Successful File Write\n");
	}
}

/**
 * @brief Converts unsigned int to Unsigned VLQ
 * Used for data compression
 *
 * @author Vishnu Vijay
 * @date 2/3/21
 */
VLQ_t convertToUVLQ(uint32_t originalNum) {
	uint8_t newNumArray[sizeof(originalNum) + 1] = {0};
	uint8_t buffer = 0;
	int length = 0;
	do {
		buffer = (originalNum & 0x7F);
		if (length != 0) {
			buffer |= 0x80;
		}
		newNumArray[length] = buffer;
		length++;
	} while ( originalNum >>= 7 );

	VLQ_t newNum = {0};
	newNum.quantityLength = length;
	for (int i = 0; i < length; i++) {
		switch (length) {
			case 1:
				newNum.oneByteVLQ[i] = newNumArray[length - i - 1];
				break;
			case 2:
				newNum.twoByteVLQ[i] = newNumArray[length - i - 1];
				break;
			case 3:
				newNum.threeByteVLQ[i] = newNumArray[length - i - 1];
				break;
			case 4:
				newNum.fourByteVLQ[i] = newNumArray[length - i - 1];
				break;
			case 5:
				newNum.fiveByteVLQ[i] = newNumArray[length - i - 1];
				break;
		}
	}

	return newNum;
}

/**
 * @brief Converts signed int to signed VLQ
 * Used for data compression
 *
 * @author Vishnu Vijay
 * @date 2/7/21
 */
VLQ_t convertToSVLQ(int32_t originalNum) {
	uint8_t newNumArray[sizeof(originalNum) + 1] = {0};
	uint8_t buffer = 0;
	int length = 0;
	uint8_t firstByte = 1;
	do {
		if (length == 0) {
			buffer = (originalNum & 0x3F);
			if (originalNum < 0) {
				buffer |= 0x40;
			}
		}
		else {
			buffer = (originalNum & 0x7F);
			buffer |= 0x80;
			firstByte = 0;
		}
		newNumArray[length] = buffer;
		length++;
	} while ( originalNum >>= (7 - firstByte) );

	VLQ_t newNum = {0};
	newNum.quantityLength = length;
	for (int i = 0; i < length; i++) {
		switch (length) {
			case 1:
				newNum.oneByteVLQ[i] = newNumArray[length - i - 1];
				break;
			case 2:
				newNum.twoByteVLQ[i] = newNumArray[length - i - 1];
				break;
			case 3:
				newNum.threeByteVLQ[i] = newNumArray[length - i - 1];
				break;
			case 4:
				newNum.fourByteVLQ[i] = newNumArray[length - i - 1];
				break;
			case 5:
				newNum.fiveByteVLQ[i] = newNumArray[length - i - 1];
				break;
		}
	}

	return newNum;
}
