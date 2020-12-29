/*
 * transmission.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/transmission.h"

/**
 * @brief Getting transmission data task
 *  TODO: Implement getData_T
 * This function gets the transmission data from the control logic task
 *
 * @param none
 * @retval none
 *
 * @author Ryan Horvath
 * @date 12/29/20
 */
void getData_T(){
	// Wait for unlock
	while(g_Data.lock) {
			retryTakeDelay(0);
		}

	// Lock
	g_Data.lock = true;

	if(g_Data.hasUpdate) {

		// Update local variables


		// Reset Update
		g_Data.hasUpdate = false;
	}
	// Unlock
	g_Data.lock = false;
}

/**
 * @brief Transmitting data task
 * TODO: Implement transmitData_T
 * This function sends the transmitted data to the radio
 *
 * @param none
 * @retval none
 *
 * @author Ryan Horvath
 * @date 12/29/20
 */
void transmitData_T(){

}
