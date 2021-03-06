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
//
// monitoring
volatile monitoringData_t g_monitoringData = {0};
// transmission
volatile transmissionData_t g_transmissionData = {0};
// modes
volatile ui8 g_currentNominalMode = 0;
volatile ui8 g_currentContingency = 0;
// pointers
altitudeNode_t *g_newAltitudeNode = NULL;
alaNode_t *g_newALANode = NULL;
staticOrientationNode_t *g_newStaticOrientationNode = NULL;


altitudeNode_t *createAltitudeList(ui8 listSize) {
	altitudeNode_t *headNode = calloc(sizeof(altitudeNode_t), 1);
	altitudeNode_t *currentPointer = headNode;
	for (ui8 i = 1; i < listSize; i++) {
		currentPointer->nextNode = calloc(sizeof(altitudeNode_t), 1);
		currentPointer = currentPointer->nextNode;
	}
	currentPointer->nextNode = headNode;
	return headNode;
}

alaNode_t *createALAList(ui8 listSize) {
	alaNode_t *headNode = calloc(sizeof(alaNode_t), 1);
	alaNode_t *currentPointer = headNode;
	for (ui8 i = 1; i < listSize; i++) {
		currentPointer->nextNode = calloc(sizeof(alaNode_t), 1);
		currentPointer = currentPointer->nextNode;
	}
	currentPointer->nextNode = headNode;
	return headNode;
}

staticOrientationNode_t *createStaticOrientationList(ui8 listSize) {
	staticOrientationNode_t *headNode = calloc(sizeof(staticOrientationNode_t), 1);
	staticOrientationNode_t *currentPointer = headNode;
	for (ui8 i = 1; i < listSize; i++) {
		currentPointer->nextNode = calloc(sizeof(staticOrientationNode_t), 1);
		currentPointer = currentPointer->nextNode;
	}
	currentPointer->nextNode = headNode;
	return headNode;
}

int setupLinkedLists() {
	g_newAltitudeNode = createAltitudeList(ALTITUDE_LIST_SIZE);
	g_newALANode = createALAList(ALA_LIST_SIZE);
	g_newStaticOrientationNode = createStaticOrientationList(STATIC_ORIENTATION_LIST_SIZE);

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

int freeAllLists() {
	int counter = 0;

	counter += freeAltitudeList(g_newAltitudeNode);
	counter += freeALAList(g_newALANode);
	counter += freeStaticOrientationList(g_newStaticOrientationNode);

	return counter;
}

int insertNewAltitude(float newAltitude) {
	altitudeNode_t *lastNewNode = g_newAltitudeNode;
	g_newAltitudeNode = lastNewNode->nextNode;

	while (g_newAltitudeNode->lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
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
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_newALANode->lock = true;
	g_newALANode->gForce = newALA;
	g_newALANode->lock = false;

	return SUCCESSFUL_RETURN;
}

int insertNewStaticOrientation(float newStaticOrientation) {
	staticOrientationNode_t *lastNewNode = g_newStaticOrientationNode;
	g_newStaticOrientationNode = lastNewNode->nextNode;

	while (g_newStaticOrientationNode->lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_newStaticOrientationNode->lock = true;
	g_newStaticOrientationNode->staticOrientation = newStaticOrientation;
	g_newStaticOrientationNode->lock = false;

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
#ifdef NDEBUG
	vTaskDelay(length);
#endif
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

