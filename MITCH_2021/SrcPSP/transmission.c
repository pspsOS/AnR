/*
 * transmission.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/transmission.h"

/*Local Variable declaration */
char transmissionString[MAX_TRANSMISSION_SIZE] = {0};

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
	while(g_transmissionData.lock) {
			retryTakeDelay(DEFAULT_TAKE_DELAY);
		}

	// Lock
	g_transmissionData.lock = true;

	if(g_transmissionData.hasUpdate) {

		// Update local variables


		// Reset Update
		g_transmissionData.hasUpdate = false;
	}
	// Unlock
	g_transmissionData.lock = false;
}
/**
 * @brief Compressing Data Task
 *  TODO: Implement compressData_T
 * This function compresses the data that will be transmitted
 *
 * @param none
 * @retval none
 *
 * @author Ryan Horvath
 * @date 12/29/20
 */
void compressData_T(){

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
