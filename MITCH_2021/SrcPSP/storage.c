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
 * @author Ryan Horvath
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
				dataStream[i] = genericVLQ.oneByteVLQ[i];
				break;
			case 2:
				dataStream[i] = genericVLQ.twoByteVLQ[i];
				break;
			case 3:
				dataStream[i] = genericVLQ.threeByteVLQ[i];
				break;
			case 4:
				dataStream[i] = genericVLQ.fourByteVLQ[i];
				break;
		}
	}
	size += genericVLQ.quantityLength;

	for (uint8_t i = sizeof(g_daqStatusData.timeStamp); i < sizeof(g_daqStatusData); i++) {
		//dataStream[genericVLQ.quantityLength + 1 + i - sizeof(g_daqStatusData.timeStamp)] = g_daqStatusData[i];
		size++;
	}

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
 * @brief stores Daq Scaling
 * Stores values from the daqScaling Struct
 *
 * @author Ryan Horvath
 * @date 1/30/21
 */
void storeDaqScaling() {

}

/**
 * @brief stores GPS data
 * Stores values from the gpsData Struct
 *
 * @author Ryan Horvath
 * @date 1/30/21
 */
void storeGpsData() {

}

/**
 * @brief stores BMP data
 * Stores values from the bmpData Struct
 *
 * @author Ryan Horvath
 * @date 1/30/21
 */
void storeBmpData() {

}

/**
 * @brief stores Imu Data
 * Stores values from the imuData Struct
 *
 * @author Ryan Horvath
 * @date 1/30/21
 */
void storeImuData() {

}

/**
 * @brief stores Processed Data
 * Stores values from the processedData Struct
 *
 * @author Ryan Horvath
 * @date 1/30/21
 */
void storeProcessedData() {

}

/**
 * @brief stores Monitoring Data
 * Stores values from the monitoringData Struct
 *
 * @author Ryan Horvath
 * @date 1/30/21
 */
void storeMonitoringData() {

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
