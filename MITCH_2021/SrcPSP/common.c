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
volatile bool g_chuteDisarm = false;
volatile bool g_chuteDeployable = false;
// pointers
altitudeNode_t *g_newAltitudeNode = NULL;
alaNode_t *g_newALANode = NULL;
staticOrientationNode_t *g_newStaticOrientationNode = NULL;
imuNode_t *g_newIMUNode = NULL;


altitudeNode_t *createAltitudeList(uint16_t listSize) {
	altitudeNode_t *headNode = calloc(sizeof(altitudeNode_t), 1);
	altitudeNode_t *currentPointer = headNode;
	for (uint16_t i = 1; i < listSize; i++) {
		currentPointer->nextNode = calloc(sizeof(altitudeNode_t), 1);
		currentPointer = currentPointer->nextNode;
	}
	currentPointer->nextNode = headNode;
	return headNode;
}

alaNode_t *createALAList(uint16_t listSize) {
	alaNode_t *headNode = calloc(sizeof(alaNode_t), 1);
	alaNode_t *currentPointer = headNode;
	for (uint16_t i = 1; i < listSize; i++) {
		currentPointer->nextNode = calloc(sizeof(alaNode_t), 1);
		currentPointer = currentPointer->nextNode;
	}
	currentPointer->nextNode = headNode;
	return headNode;
}

staticOrientationNode_t *createStaticOrientationList(uint16_t listSize) {
	staticOrientationNode_t *headNode = calloc(sizeof(staticOrientationNode_t), 1);
	staticOrientationNode_t *currentPointer = headNode;
	for (uint16_t i = 1; i < listSize; i++) {
		currentPointer->nextNode = calloc(sizeof(staticOrientationNode_t), 1);
		currentPointer = currentPointer->nextNode;
	}
	currentPointer->nextNode = headNode;
	return headNode;
}

imuNode_t *createIMUList(uint16_t listSize) {
	imuNode_t *headNode = calloc(sizeof(imuNode_t), 1);
	imuNode_t *currentPointer = headNode;
	for (uint16_t i = 1; i < listSize; i++) {
		currentPointer->nextNode = calloc(sizeof(imuNode_t), 1);
		currentPointer = currentPointer->nextNode;
	}
	currentPointer->nextNode = headNode;
	return headNode;
}

int setupLinkedLists() {
	g_newAltitudeNode = createAltitudeList(ALTITUDE_LIST_SIZE);
	g_newALANode = createALAList(ALA_LIST_SIZE);
	g_newStaticOrientationNode = createStaticOrientationList(STATIC_ORIENTATION_LIST_SIZE);
	g_newIMUNode = createIMUList(IMU_LIST_SIZE);

	return SUCCESSFUL_RETURN;
}

int freeAltitudeList(altitudeNode_t *someAltitudePtr) {
	altitudeNode_t *nextPtr = NULL;

	while (someAltitudePtr != NULL) {
		nextPtr = someAltitudePtr->nextNode;
		free(someAltitudePtr);
		someAltitudePtr = NULL;
		someAltitudePtr = nextPtr;
	}

	return SUCCESSFUL_RETURN;
}

int freeALAList(alaNode_t *someALAPtr) {
	alaNode_t *nextPtr = NULL;

	while (someALAPtr != NULL) {
		nextPtr = someALAPtr->nextNode;
		free(someALAPtr);
		someALAPtr = NULL;
		someALAPtr = nextPtr;
	}

	return SUCCESSFUL_RETURN;
}

int freeStaticOrientationList(staticOrientationNode_t *someStaticOrientationPtr) {
	staticOrientationNode_t *nextPtr = NULL;

	while (someStaticOrientationPtr != NULL) {
		nextPtr = someStaticOrientationPtr->nextNode;
		free(someStaticOrientationPtr);
		someStaticOrientationPtr = NULL;
		someStaticOrientationPtr = nextPtr;
	}

	return SUCCESSFUL_RETURN;
}

int freeIMUList(imuNode_t *someIMUPtr) {
	imuNode_t *nextPtr = NULL;

	while (someIMUPtr != NULL) {
		nextPtr = someIMUPtr->nextNode;
		free(someIMUPtr);
		someIMUPtr = NULL;
		someIMUPtr = nextPtr;
	}

	return SUCCESSFUL_RETURN;
}

int freeAllLists() {
	int counter = 0;

	counter += freeAltitudeList(g_newAltitudeNode);
	counter += freeALAList(g_newALANode);
	counter += freeStaticOrientationList(g_newStaticOrientationNode);
	counter += freeIMUList(g_newIMUNode);

	return counter;
}

int insertNewAltitude(float newAltitude) {
	altitudeNode_t *lastNewNode = g_newAltitudeNode;
	g_newAltitudeNode = lastNewNode->nextNode;

	while (g_newAltitudeNode->lock) {
		retryTakeDelay(0);
	}
	g_newAltitudeNode->lock = true;
	g_newAltitudeNode->altitude = newAltitude;
	g_newAltitudeNode->lock = false;

	return SUCCESSFUL_RETURN;
}

int insertNewALA(float newALA) {
	alaNode_t *lastNewNode = g_newALANode;
	g_newALANode = lastNewNode->nextNode;

	while (g_newALANode->lock) {
		retryTakeDelay(0);
	}
	g_newALANode->lock = true;
	g_newALANode->gForce = newALA;
	g_newALANode->lock = false;

	return SUCCESSFUL_RETURN;
}

int insertNewStaticOrientation() {
	staticOrientationNode_t *lastNewNode = g_newStaticOrientationNode;
	g_newStaticOrientationNode = lastNewNode->nextNode;

	int notPEUCounter = 0;
	staticOrientationNode_t *tempPtr = g_newStaticOrientationNode;
	for (int i = 0; i < STATIC_ORIENTATION_LIST_SIZE; i++) {
		while (tempPtr->lock) {
			retryTakeDelay(0);
		}

		tempPtr->lock = true;
		if (!tempPtr->staticOrientation) {
			notPEUCounter++;
		}
		tempPtr->lock = false;
	}
	bool newStatus = pointyEndUp_P();
	if (!newStatus) {
		notPEUCounter++;
	}

	while (g_newStaticOrientationNode->lock) {
		retryTakeDelay(0);
	}
	g_newStaticOrientationNode->lock = true;
	g_newStaticOrientationNode->staticOrientation = newStatus;
	g_newStaticOrientationNode->numNotOptimal = notPEUCounter;
	g_newStaticOrientationNode->lock = false;

	return SUCCESSFUL_RETURN;
}

int insertNewIMUNode() {
	imuNode_t *lastNewNode = g_newIMUNode;
	g_newIMUNode = lastNewNode->nextNode;

	while (g_imuData.lock) {
		retryTakeDelay(0);
	}
	g_imuData.lock = true;
	imuData_t newestData = {0};
	newestData = g_imuData;
	g_imuData.lock = false;

	while (g_newIMUNode->lock) {
		retryTakeDelay(0);
	}
	g_newIMUNode->lock = true;
	g_newIMUNode->accX = newestData.accX;
	g_newIMUNode->accY = newestData.accY;
	g_newIMUNode->accZ = newestData.accZ;
	g_newIMUNode->gyrX = newestData.gyrX;
	g_newIMUNode->gyrY = newestData.gyrY;
	g_newIMUNode->gyrZ = newestData.gyrZ;
	g_newIMUNode->magX = newestData.magX;
	g_newIMUNode->magY = newestData.magY;
	g_newIMUNode->magZ = newestData.magZ;
	g_newIMUNode->alaZ = newestData.alaZ;
	g_newIMUNode->lock = false;

	return SUCCESSFUL_RETURN;
}

float calcAvgAlt() {
	altitudeNode_t *someAltPtr = g_newAltitudeNode;
	float avgAlt = 0;
	int counter = 0;

	for (int i = 0; i < ALTITUDE_LIST_SIZE; i++) {
		if (!someAltPtr->lock) {
			someAltPtr->lock = true;
			avgAlt += someAltPtr->altitude;
			counter++;
			someAltPtr->lock = false;
		}

		someAltPtr = someAltPtr->nextNode;
	}

	return (avgAlt / counter);
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
#ifndef NDEBUG
	// Do Nothing
#else
	// TODO: Implement retryTakeDelay
	// Using rtos
#endif
}
