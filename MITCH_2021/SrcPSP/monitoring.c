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

/* TODO: Implement checkBatteryVoltage
 * This function checks the battery voltage and stores it as a global float
 *
 * Author: markp
 * Date: 12/23/20
 */

void checkBatteryVoltage_M() {
	g_monitoringData.batteryVoltage = voltage;
}

/* TODO: Implement checkContinuity
 * This function checks the continuity of all 4 parachute charges and stores as a global bool array
 *
 * Author: markp
 * Date: 12/23/20
 */

void checkContinuity_M() {
	for (i = 0; i < 4; i++) {
		g_monitoringData.continuity[i] = continuity[i];
	}
}

/* TODO: Implement checkButtonState
 * This function checks the button state and stores it as a global bool
 *
 * Author: markp
 * Date: 12/23/20
 */

void checkButtonState_M() {
	g_monitoringData.buttonState = button;
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
