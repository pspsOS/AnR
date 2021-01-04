/*
 * processing.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/processing.h"

//#include <stdbool.h>

#ifndef NDEBUG
#include <debugSettings.h>
#include <unistd.h>
#endif

bool bmpDStatus = false;
bool IMUDStatus = false;
bool GPSDStatus = false;
bool flpd_state = false;

/* TODO: Implement getBMPData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

bool getBMPData_P() {

	// Lock
	g_bmpData.lock = true;

	// Update local variables

	// Reset Update
	g_bmpData.hasUpdate = false;
	// Unlock
	g_bmpData.lock = false;

	g_bmpData.timeStamp = getTimeStamp();

}


/* TODO: Implement getIMUData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

bool getIMUData_P() {

	// Lock
	g_imuData.lock = true;

	// Update local variables


	// Reset Update
	g_imuData.hasUpdate = false;
	// Unlock
	g_imuData.lock = false;

	g_imuData.timeStamp = getTimeStamp();

}


/* TODO: Implement getGPSData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

bool getGPSData_P() {

	// Lock
	g_gpsData.lock = true;

	// Update local variables


	// Reset Update
	g_gpsData.hasUpdate = false;
	// Unlock
	g_gpsData.lock = false;

	g_gpsData.timeStamp = getTimeStamp();

}


/* TODO: Implement processData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

void processData_P() {
	if (g_bmpData.hasUpdate && g_bmpData.lock) {
		getBMPData_P();
	}

	if (g_imuData.hasUpdate && g_imuData.lock) {
		getIMUData_P();
	}

	if (g_gpsData.hasUpdate && g_bmpData.lock) {
		getGPSData_P();
	}

	flpd_state = pointyEndUp_P();



}


/* TODO: Implement transmitData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

void transmitData_P() {

}

/**
 * @brief Evaluate Pointy-End-Up
 * Determines whether the magnitude of the acceleration vector is pointing
 * downwards within the tolerance defined in TAN_THETA_SQUARED
 *
 * @param None
 * @return bool true or false
 *
 * @author Jeff Kaji, Bahaa Elshimy
 * @date 12/29/2020
 */
bool pointyEndUp_P() {
	while(g_imuData.lock) {
			retryTakeDelay(0);
	}
	g_imuData.lock = true;
	float x = g_imuData.accX;
	float y = g_imuData.accY;
	float z = g_imuData.accZ;
	g_imuData.lock = false;

	assert((x * x + y * y) != 0);

	return ((z*z / (x * x + y * y)) > TAN_THETA_SQUARED) && (z < 0) && ((x * x + y * y) > MIN_HORZ_ACCL_SQ);
}


