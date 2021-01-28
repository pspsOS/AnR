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

#ifdef NDEBUG
GPIO_PinState PSP_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	if(GPIO_Pin == FAKE_GPIO) return 0;
	else return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}

void PSP_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
	if(GPIO_Pin != FAKE_GPIO) HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

void toggleLed() {
	static int j = 1;
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, j);
	j = !j;
}


#endif
