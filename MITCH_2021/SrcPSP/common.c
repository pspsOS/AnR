/*
 * common.c
 *
 *  Created on: Dec 22, 2020
 *      Author: Jeff
 */

#include "../IncPSP/common.h"
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

	while (g_altitudeArray[newestAltitudeIndex]->lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_altitudeArray[newestAltitudeIndex]->lock = true;
	g_altitudeArray[newestAltitudeIndex]->currentAltitude = newAltitude;
	g_altitudeArray[newestAltitudeIndex]->runningAltitude += newAltitude * 2.0 / ((float) ALTITUDE_ARRAY_SIZE);
	g_altitudeArray[newestAltitudeIndex]->lock = false;

	return SUCCESSFUL_RETURN;
}

int insertNewALA(float newALA) {
	newestAlaIndex++;
	if (newestAlaIndex == ALA_ARRAY_SIZE) {
		newestAlaIndex = 0;
	}

	while (g_alaArray[newestAlaIndex]->lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_alaArray[newestAlaIndex]->lock = true;
	g_alaArray[newestAlaIndex]->currentForce = newALA;
	g_alaArray[newestAlaIndex]->runningForce += newALA * 2.0 / ((float) ALA_ARRAY_SIZE);
	g_alaArray[newestAlaIndex]->lock = false;

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
		while (g_staticOrientationArray[currentIndex]->lock) {
			retryTakeDelay(DEFAULT_TAKE_DELAY);
		}
		g_staticOrientationArray[currentIndex]->lock = true;

		if (!(g_staticOrientationArray[currentIndex]->staticOrientation)) {
			notPEUCounter++;
		}
		g_staticOrientationArray[currentIndex]->lock = false;
	}

	bool newStatus = pointyEndUp_P();
	if (!newStatus) {
		notPEUCounter++;
	}

	while (g_staticOrientationArray[newestStaticOrientationIndex]->lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_staticOrientationArray[newestStaticOrientationIndex]->lock = true;
	g_staticOrientationArray[newestStaticOrientationIndex]->staticOrientation = newStatus;
	g_staticOrientationArray[newestStaticOrientationIndex]->numNotOptimal = notPEUCounter;
	g_staticOrientationArray[newestStaticOrientationIndex]->lock = false;

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

	while (g_imuArray[newestImuIndex]->lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_imuArray[newestImuIndex]->lock = true;
	g_imuArray[newestImuIndex]->accX = newestData.accX;
	g_imuArray[newestImuIndex]->accY = newestData.accY;
	g_imuArray[newestImuIndex]->accZ = newestData.accZ;
	g_imuArray[newestImuIndex]->gyrX = newestData.gyrX;
	g_imuArray[newestImuIndex]->gyrY = newestData.gyrY;
	g_imuArray[newestImuIndex]->gyrZ = newestData.gyrZ;
	g_imuArray[newestImuIndex]->magX = newestData.magX;
	g_imuArray[newestImuIndex]->magY = newestData.magY;
	g_imuArray[newestImuIndex]->magZ = newestData.magZ;
	g_imuArray[newestImuIndex]->alaZ = newestData.alaZ;
	g_imuArray[newestImuIndex]->lock = false;

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
