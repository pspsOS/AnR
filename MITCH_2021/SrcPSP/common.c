/*
 * common.c
 *
 *  Created on: Dec 22, 2020
 *      Author: Jeff
 */

#include "../IncPSP/common.h"
#include "../IncPSP/processing.h"
#include <unistd.h>
/* Global variable declarations */

// acquisition
volatile daqStatusData_t g_daqStatusData = {0};
volatile daqScalingData_t g_daqScalingData = {0};
volatile gpsData_t g_gpsData = {0};
volatile bmpData_t g_bmpData = {0};
volatile imuData_t g_imuData = {0};
// monitoring
volatile monitoringData_t g_monitoringData = {0};
// transmission
volatile transmissionData_t g_transmissionData = {0};
// modes
volatile uint8_t g_currentNominalMode = 0;
volatile uint8_t g_currentContingency = 0;
volatile uint32_t g_launchTime = 0;
volatile bool g_chuteDisarm = false;
volatile bool g_chuteDeployable = false;
// arrays
altitudeNode_t g_altitudeArray[ALTITUDE_ARRAY_SIZE] = {0};
alaNode_t g_alaArray[ALA_ARRAY_SIZE] = {0};
staticOrientationNode_t g_staticOrientationArray[STATIC_ORIENTATION_ARRAY_SIZE] = {0};
imuNode_t g_imuArray[IMU_ARRAY_SIZE] = {0};
// circular buffer indices
uint16_t newestAltitudeIndex = 0;
uint16_t newestAlaIndex = 0;
uint16_t newestStaticOrientationIndex = 0;
uint16_t newestImuIndex = 0;


int insertNewAltitude(float newAltitude) {
	newestAltitudeIndex++;
	if (newestAltitudeIndex == ALTITUDE_ARRAY_SIZE) {
		newestAltitudeIndex = 0;
	}

	while (g_altitudeArray[newestAltitudeIndex].lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_altitudeArray[newestAltitudeIndex].lock = true;
	g_altitudeArray[newestAltitudeIndex].currentAltitude = newAltitude;
	g_altitudeArray[newestAltitudeIndex].runningAltitude += newAltitude * 2.0 / ((float) ALTITUDE_ARRAY_SIZE);
	g_altitudeArray[newestAltitudeIndex].lock = false;

	return SUCCESSFUL_RETURN;
}

int insertNewALA(float newALA) {
	newestAlaIndex++;
	if (newestAlaIndex == ALA_ARRAY_SIZE) {
		newestAlaIndex = 0;
	}

	while (g_alaArray[newestAlaIndex].lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_alaArray[newestAlaIndex].lock = true;
	g_alaArray[newestAlaIndex].currentForce = newALA;
	g_alaArray[newestAlaIndex].runningForce += newALA * 2.0 / ((float) ALA_ARRAY_SIZE);
	g_alaArray[newestAlaIndex].lock = false;

	return SUCCESSFUL_RETURN;
}

int insertNewStaticOrientation() {
	newestStaticOrientationIndex++;
	if (newestStaticOrientationIndex == STATIC_ORIENTATION_ARRAY_SIZE) {
		newestStaticOrientationIndex = 0;
	}

	int notPEUCounter = 0;
	for (int i = 0; i < STATIC_ORIENTATION_ARRAY_SIZE; i++) {
		int currentIndex = (i + newestStaticOrientationIndex) % STATIC_ORIENTATION_ARRAY_SIZE;
		while (g_staticOrientationArray[currentIndex].lock) {
			retryTakeDelay(DEFAULT_TAKE_DELAY);
		}
		g_staticOrientationArray[currentIndex].lock = true;

		if (!(g_staticOrientationArray[currentIndex].staticOrientation)) {
			notPEUCounter++;
		}
		g_staticOrientationArray[currentIndex].lock = false;
	}

	bool newStatus = pointyEndUp_P();
	if (!newStatus) {
		notPEUCounter++;
	}

	while (g_staticOrientationArray[newestStaticOrientationIndex].lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_staticOrientationArray[newestStaticOrientationIndex].lock = true;
	g_staticOrientationArray[newestStaticOrientationIndex].staticOrientation = newStatus;
	g_staticOrientationArray[newestStaticOrientationIndex].numNotOptimal = notPEUCounter;
	g_staticOrientationArray[newestStaticOrientationIndex].lock = false;

	return SUCCESSFUL_RETURN;
}

int insertNewIMUNode() {
	newestImuIndex++;
	if (newestImuIndex == IMU_ARRAY_SIZE) {
		newestImuIndex = 0;
	}

	while (g_imuData.lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_imuData.lock = true;
	imuData_t newestData = {0};
	newestData = g_imuData;
	g_imuData.lock = false;

	while (g_imuArray[newestImuIndex].lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_imuArray[newestImuIndex].lock = true;
	g_imuArray[newestImuIndex].accX = newestData.accel_xout;
	g_imuArray[newestImuIndex].accY = newestData.accel_yout;
	g_imuArray[newestImuIndex].accZ = newestData.accel_zout;
	g_imuArray[newestImuIndex].gyrX = newestData.gyro_xout;
	g_imuArray[newestImuIndex].gyrY = newestData.gyro_yout;
	g_imuArray[newestImuIndex].gyrZ = newestData.gyro_zout;
	g_imuArray[newestImuIndex].magX = newestData.mag_xout;
	g_imuArray[newestImuIndex].magY = newestData.mag_yout;
	g_imuArray[newestImuIndex].magZ = newestData.mag_zout;
	g_imuArray[newestImuIndex].alaZ = newestData.alaZ;
	g_imuArray[newestImuIndex].lock = false;

	return SUCCESSFUL_RETURN;
}


/**
 * @brief Get system timestamp
 * Gets the system timestamp. Returns 0 when run from testbed.
 *
 * @param None
 * @return None
 *
 * @author Jeff Kaji
 * @date 12/24/2020
 */
ui32 getTimeStamp(void) {
	#ifndef NDEBUG
		return 0;
	#else
		return HAL_GetTick();
	#endif
}

/**
 * @brief Quick delay for take retry
 * Creates a short rtos delay for take retry
 *
 * @param length: TBD
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/28/2020
 */
void retryTakeDelay(int length) {
	#ifdef NDEBUG
		vTaskDelay(length);
	#endif
}


VLQ_t convertToVLQ(uint32_t originalNum) {
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


/*
void notify(Message_ID message, Device_ID device) {
#ifdef SUPRESS_ALL
	return;
#endif
#ifdef SUPRESS_TASK_UPDATES
	if(message == TASK_UPDATE) return;
#endif
#ifdef SUPRESS_SETUP_WARNING
	if(message == SETUP_WARNING) return;
#endif

	extern bool imuNominal;
	extern bool alaNominal;
	extern bool bmpNominal;
	extern bool gpsNominal;
	extern bool nandNominal;

	switch(message) {
	case TASK_UPDATE:
		printf("    Task Update: ");
		break;
	case SETUP_WARNING:
		printf("! Setup Warning: ");
		break;
	}

	switch(device) {
	case SYSTEM:
		printf("System: ");
		break;
	case GPS:
		printf("   GPS: ");
		break;
	case BMP:
		printf("   BMP: ");
		break;
	case IMU:
		printf("   IMU: ");
		break;
	case ALA:
		printf("   ALA: ");
		break;
	case NAND:
		printf("  NAND: ");
		break;
	}

	switch(message) {
	case TASK_UPDATE:
		switch(device) {
		case GPS:
			printf("Nominal Status: %s\r\n", gpsNominal ? "True" : "False");
			break;
		case BMP:
			printf("Nominal Status: %s\r\n", bmpNominal ? "True" : "False");
			break;
		case IMU:
			printf("Nominal Status: %s\r\n", imuNominal ? "True" : "False");
			break;
		case ALA:
			printf("Nominal Status: %s\r\n", alaNominal ? "True" : "False");
			break;
		case NAND:
			printf("Nominal Status: %s\r\n", nandNominal ? "True" : "False");
			break;
		default:
			break;
		}
	default:
		break;
	}


}
*/

