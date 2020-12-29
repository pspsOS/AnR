/*
 * monitoring.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/monitoring.h"

/* Global variable declarations */

extern volatile monitoringData_t g_monitoringData = {0};

/* Local variable declarations */

ui8 i;
float voltage;
bool continuity[3];
bool button;

/* TODO: Implement checkBatteryVoltage
 * This function checks the battery voltage and stores it as a global float
 *
 * Author: markp
 * Date: 12/23/20
 */

void checkBatteryVoltage_M() {
	#ifndef NDEBUG
		g_monitoringData.batteryVoltage = 7.4;
		#else
			// TODO: Implement battery voltage reading from hardware
		#endif
}

/* TODO: Implement checkContinuity
 * This function checks the continuity of all 4 parachute charges and stores as a global bool array
 *
 * Author: markp
 * Date: 12/23/20
 */

void checkContinuity_M() {
	for (i = 0; i < 4; i++) {
		#ifndef NDEBUG
			g_monitoringData.continuity[i] = true;
		#else
			// TODO: Implement continuity reading from hardware
		#endif
	}
}

/* TODO: Implement checkButtonState
 * This function checks the button state and stores it as a global bool
 *
 * Author: markp
 * Date: 12/23/20
 */

void checkButtonState_M() {
	#ifndef NDEBUG
		g_monitoringData.buttonState = false;
		#else
			// TODO: Implement button state reading from hardware
		#endif
}

/* TODO: Implement checkButtonState
 * This function checks the button state and stores it as a global bool
 *
 * Author: markp
 * Date: 12/23/20
 */

void sendUpdate_M() {
	while(g_monitoringData.lock) {
		retryTakeDelay(0);
	}
	g_monitoringData.batteryVoltage;
	g_monitoringData.continuity[3];
	g_monitoringData.buttonState;
	g_monitoringData.hasUpdate = true;
	g_monitoringData.lock = false;
}
