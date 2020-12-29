/*
 * processing.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/processing.h"

/* TODO: Implement getBMPData
 *
 * Author: Vishnu Vijay
 * Date: 12/29/20
 */

bool getBMPData_P() {

}


/* TODO: Implement getIMUData
 *
 * Author: Vishnu Vijay
 * Date: 12/29/20
 */

bool getIMUData_P() {

}


/* TODO: Implement getGPSData
 *
 * Author: Vishnu Vijay
 * Date: 12/29/20
 */

bool getGPSData_P() {

}


/* TODO: Implement processData
 *
 * Author: Vishnu Vijay
 * Date: 12/29/20
 */

void processData_P() {

}


/* TODO: Implement transmitData
 *
 * Author: Vishnu Vijay
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
 * @author Jeff Kaji
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

	return ((z*z / (x * x + y * y)) > TAN_THETA_SQUARED) && (z < 0);
}


