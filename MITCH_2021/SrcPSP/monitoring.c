/*
 * monitoring.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/monitoring.h"

/* Global variable declarations */


/* Local variable declarations */

float batteryVoltage;
bool continuity[4];
bool buttonState;

/**
 * @brief setup
 * Initialize the monitoring local variables
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 12/29/2020
 */

void setup_M() {
	//initialize variables
	batteryVoltage = 7.4;
	for (ui8 i = 0; i < 4; i++) {
		continuity[i] = true;
	}
	buttonState = false;
}

/**
 * @brief Flight Loop
 * This function runs the flight loops which calls all necessary monitoring flight functions
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 12/29/2020
 */

void loop_M() {
	checkBatteryVoltage_M();
	checkContinuity_M();
	checkButtonState_M();
	sendUpdate_M();
}

/**
 * @brief Check new battery voltage
 * This function checks the battery voltage and stores it as a local float
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 12/29/2020
 */

void checkBatteryVoltage_M() {
	#ifndef NDEBUG
		batteryVoltage = 7.4;
		#else
			// TODO: Implement battery voltage reading from hardware
		#endif
}

/**
 * @brief Check new continuity for each parachute charge
 * This function checks the continuity of all 4 parachute charges and stores as a local bool array
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 12/29/2020
 */

void checkContinuity_M() {
	for (ui8 i = 0; i < 4; i++) {
		#ifndef NDEBUG
			continuity[i] = true;
		#else
			// TODO: Implement continuity reading from hardware
		#endif
	}
}

/**
 * @brief Check new button state
 * This function checks the button state and stores it as a local bool
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 12/29/2020
 */

void checkButtonState_M() {
	#ifndef NDEBUG
		buttonState = false;
		#else
			// TODO: Implement button state reading from hardware
		#endif
}

/**
 * @brief send updated monitoring data
 * Takes local monitoring data and updates global monitoring variables with local data
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 12/29/2020
 */

void sendUpdate_M() {
	while(g_monitoringData.lock) {
		retryTakeDelay(0);
	}
	g_monitoringData.lock = true;
	g_monitoringData.timeStamp = getTimeStamp();
	g_monitoringData.batteryVoltage;
	for (ui8 i = 0; i < 4; i++) {
		g_monitoringData.continuity[i] = continuity[i];
	}
	g_monitoringData.buttonState = buttonState;
	g_monitoringData.hasUpdate = true;
	g_monitoringData.lock = false;
}
