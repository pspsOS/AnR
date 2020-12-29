/*
 * common.c
 *
 *  Created on: Dec 22, 2020
 *      Author: Jeff
 */

#include "../IncPSP/common.h"

/* Global variable declarations */

// acquisition
extern volatile daqStatusData_t g_daqStatusData = {0};
extern volatile gpsData_t g_gpsData = {0};
extern volatile bmpData_t g_bmpData = {0};
extern volatile imuData_t g_imuData = {0};
// monitoring
extern volatile monitoringData_t g_monitoringData = {0};
// transmission
extern volatile transmissionData_t g_transmissionData = {0};
// modes
extern volatile ui8 g_currentNominalMode = 0;
extern volatile ui8 g_currentContingency = 0;
// pointers
extern altitudeNode_t *g_headAltitudeNode = NULL;
extern alaNode_t *g_headALANode = NULL;
extern staticOrientationNode_t *g_headStaticOrientationNode = NULL;


void *createLinkedList(void listType, ui8 listSize) {
	void *headNode = calloc(sizeof(listType), 1);
	void *currentPointer = headNode;
	for (ui8 i = 1; i < listSize; i++) {
		currentPointer->nextNode = calloc(sizeof(listType), 1);
		currentPointer = currentPointer->nextNode;
	}
	currentPointer->nextNode = headNode;
	return headNode;
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
