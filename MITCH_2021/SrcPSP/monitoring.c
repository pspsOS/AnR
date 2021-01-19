/*
 * monitoring.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/monitoring.h"

#ifdef NDEBUG
	#include "genericInterface.h"
#else
	#include "../IncDebug/debugSettings.h"
	#include <unistd.h>
#endif

/* Global variable declarations */


/* Local variable declarations */

float batteryVoltage; // Voltage of battery (V)
bool continuity[4]; // Continuity status of parachute deployment charges
bool buttonState; // State of button (on/off)
bool hardwareDeploymentDisable; // Terminal block disables deployment in hardware

// File pointers for Debugging
#ifndef NDEBUG
	FILE *_monitoringFile;
#endif

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
	#ifndef NDEBUG
		_monitoringFile = setupMonitoringFile_DS();
	#else
		// TODO: Implement Monitoring Setup
	#endif
	batteryVoltage = 7.4;
	for (ui8 i = 0; i < 4; i++) {
		continuity[i] = true;
	}
	buttonState = false;
	hardwareDeploymentDisable = false;
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
	#ifndef NDEBUG
		if(!simulateMonitoring) {
			if(notifyWhenReadAborted)
				print("Monitoring read aborted.\n");
			return;
		}
		fscanf(_monitoringFile, "%f", &batteryVoltage);
		fscanf(_monitoringFile, "%d", &continuity[0]);
		fscanf(_monitoringFile, "%d", &continuity[1]);
		fscanf(_monitoringFile, "%d", &continuity[2]);
		fscanf(_monitoringFile, "%d", &continuity[3]);
		fscanf(_monitoringFile, "%d", &buttonState);
		fscanf(_monitoringFile, "%d", &hardwareDeploymentDisable);
	#else
		checkBatteryVoltage_M();
		checkContinuity_M();
		checkButtonState_M();
		checkHardwareDeploymentDisable_M();
	#endif
	sendUpdate_M();
}

/**
 * @brief Check new battery voltage
 * This function checks the battery voltage and stores it as a local float
 * Never accessed for debug
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 12/29/2020
 */

void checkBatteryVoltage_M() {
	// TODO: Implement battery voltage reading from hardware
	 HAL_ADC_Start(&hadc1);
	 HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // TODO: Add appropriate time delay (not HAL_MAX_DELAY)
	 batteryVoltage = (HAL_ADC_GetValue(&hadc1)) * (ADC_VREF / 4095); // TODO: Add resistor math for total Vbat
}

/**
 * @brief Check new continuity for each parachute charge
 * This function checks the continuity of all 4 parachute charges and stores as a local bool array
 * Never accessed for debug
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 12/29/2020
 */

void checkContinuity_M() {
	#ifdef NDEBUG
		continuity[1] = HAL_GPIO_ReadPin(GPIOC, SENSE_A_Pin);
		continuity[2] = HAL_GPIO_ReadPin(GPIOC, SENSE_B_Pin);
		continuity[3] = HAL_GPIO_ReadPin(GPIOC, SENSE_C_Pin);
		continuity[4] = HAL_GPIO_ReadPin(GPIOC, SENSE_D_Pin);
	#endif
}

/**
 * @brief Check new button state
 * This function checks the button state and stores it as a local bool
 * Never accessed for debug
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 12/29/2020
 */

void checkButtonState_M() {
	#ifdef NDEBUG
		buttonState = HAL_GPIO_ReadPin(GPIOC, DISARM_INPUT_Pin);
	#endif
}

/**
 * @brief check the hardware deployment disable
 * Checks if the hardware has disabled deployment
 * Never accessed for debug
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 1/3/2021
 */

void checkHardwareDeploymentDisable_M() {
	#ifdef NDEBUG
		hardwareDeploymentDisable = HAL_GPIO_ReadPin(GPIOC, CHECK_HARDWARE_DEPLOYMENT_DISABLE_Pin);
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
	g_monitoringData.hardwareDeploymentDisable = hardwareDeploymentDisable;
	g_monitoringData.hasUpdate = true;
	g_monitoringData.lock = false;
}
