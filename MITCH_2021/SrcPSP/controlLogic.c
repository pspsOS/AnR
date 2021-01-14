/*
 * controlLogic.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/controlLogic.h"

#define REQUIRED_TIME_FROM_STARTUP (1200000)
#define ACCEPTABLE_PERCENT_ERROR (5)


volatile uint32_t g_launchTime = 0;



void loop_C() {
	modeLogic_C();

	bool modeChange = determineMode_C();

	if (modeChange) {
		g_currentNominalMode++;
		// TODO: Send Transition data
	}

	//TODO: Send info to storage and transition
}

/**
 * @brief Encapsulates logic functions
 * Will call other logic functions depending on current mode
 *
 * @param None
 * @retval None
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

void modeLogic_C() {
	// check contingency mode first

	switch (g_currentNominalMode) {
		case PRELAUNCH:
			prelaunchLogic_C();
			break;
		case LAUNCH:
			launchLogic_C();
			break;
		case ASCENT:
			ascentLogic_C();
			break;
		case DESCENT_DROGUE:
			descentDrogueLogic_C();
			break;
		case DESCENT_MAIN:
			descentMainLogic_C();
			break;
		case TOUCHDOWN:
			touchdownLogic_C();
			break;
		case PROGRAM_END:
			// literally nothing atm
			break;
	}
}


/**
 * @brief Contains logic for PRELAUNCH Mode
 * Called by modeLogic_C() function,
 * Plays buzzer to indicate system is active,
 * Checks if disarm button is pressed to change global disarm status
 *
 * @param None
 * @retval None
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

void prelaunchLogic_C() {
	//TODO: Play Buzzer

	//TODO: Check button and set
}


/**
 * @brief Contains logic for LAUNCH Mode
 * Called by modeLogic_C() function
 *
 * @param None
 * @retval None
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

void launchLogic_C() {

}


/**
 * @brief Contains logic for ASCENT Mode
 * Called by modeLogic_C() function
 *
 * @param None
 * @retval None
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

void ascentLogic_C() {

}


/**
 * @brief Contains logic for DESCENTDROGUE Mode
 * Called by modeLogic_C() function
 *
 * @param None
 * @retval None
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

void descentDrogueLogic_C() {

}


/**
 * @brief Contains logic for DESCENTMAIN Mode
 * Called by modeLogic_C() function
 *
 * @param None
 * @retval None
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

void descentMainLogic_C() {

}


/**
 * @brief Contains logic for TOUCHDOWN Mode
 * Called by modeLogic_C() function
 *
 * @param None
 * @retval None
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

void touchdownLogic_C() {

}


/**
 * @brief Changes mode
 * Uses raw and processed data to determine whether or not
 * to change modes
 *
 * @param None
 * @retval None
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

bool determineMode_C() {
	bool modeChange = false;
	switch (g_currentNominalMode) {
		case PRELAUNCH:
			modeChange = checkPrelaunchTrans_C();
			break;
		case LAUNCH:
			modeChange = checkLaunchTrans_C();
			break;
		case ASCENT:
			modeChange = checkAscentTrans_C();
			break;
		case DESCENT_DROGUE:
			modeChange = checkDescentDrogueTrans_C();
			break;
		case DESCENT_MAIN:
			modeChange = checkDescentMainTrans_C();
			break;
		case TOUCHDOWN:
			modeChange = checkProgramEnd_C();
			break;
	}

	return modeChange;
}


/**
 * @brief Changes mode from PRELAUNCH to LAUNCH
 * Determines if conditions have been met to move from PRELAUNCH to LAUNCH mode
 * Conditions: Correct orientation and stillness for certain period of time
 *
 * @param None
 * @retval bool
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

bool checkPrelaunchTrans_C() {
	bool timeSatisfied = false;
	if (getTimeStamp() > REQUIRED_TIME_FROM_STARTUP) {
		timeSatisfied = true;
	}

	bool orientationSatisfied = false;
	if (g_newStaticOrientationNode->numNotOptimal == 0) {
		orientationSatisfied = true;
	}

	bool stillnessSatisfied = false;
	insertNewIMUNode();
	stillnessSatisfied = determineStillness_C();

	if (timeSatisfied && orientationSatisfied && stillnessSatisfied) {
		//TODO: Store stopRewrite Address Marker

		//TODO: Set hotBoot to true

		g_chuteDeployable = true;
		g_daqStatusData.daqScaling = true;

		return true;
	}
	else {
		return false;
	}
}


/**
 * @brief Changes mode from LAUNCH to ASCENT
 * Determines if conditions have been met to move from LAUNCH to ASCENT mode
 * Conditions: Sustained Z+ G's and BMP detecting ascent
 *
 * @param None
 * @retval bool
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

bool checkLaunchTrans_C() {

	return 0;
}


/**
 * @brief Changes mode from ASCENT to DESCENTDROGUE
 * Determines if conditions have been met to move from ascent to descent drogue mode
 * Conditions: Detects apogee (loss of altitude)
 *
 * @param None
 * @retval bool
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

bool checkAscentTrans_C() {
	return 0;
}


/**
 * @brief Changes mode from DESCENTDROGUE to DESCENTMAIN
 * Determines if conditions have been met to move from DESCENTDROGUE to DESCENTMAIN mode
 * Conditions: Main chute altitude reached
 *
 * @param None
 * @retval bool
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

bool checkDescentDrogueTrans_C() {
	return 0;
}


/**
 * @brief Changes mode from DESCENTMAIN to TOUCHDOWN
 * Determines if conditions have been met to move from DESCENTMAIN to TOUCHDOWN mode
 * Conditions: Constant altitude and BLTN is still
 *
 * @param None
 * @retval bool
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

bool checkDescentMainTrans_C() {
	return 0;
}


/**
 * @brief Changes mode from TOUCHDOWN to PROGRAMEND
 * Determines if conditions have been met to move from TOUCHDOWN to PROGRAMEND mode
 * Conditions: ???
 *
 * @param None
 * @retval bool
 *
 * @author Vishnu Vijay
 * @date 12/23/20
 */

bool checkProgramEnd_C() {
	return 0;
}


bool determineStillness_C() {
	float minAccX = 0;
	float maxAccX = 0;
	float avgAccX = 0;
	float minAccY = 0;
	float maxAccY = 0;
	float avgAccY = 0;
	float minAccZ = 0;
	float maxAccZ = 0;
	float avgAccZ = 0;

	float minGyrX = 0;
	float maxGyrX = 0;
	float avgGyrX = 0;
	float minGyrY = 0;
	float maxGyrY = 0;
	float avgGyrY = 0;
	float minGyrZ = 0;
	float maxGyrZ = 0;
	float avgGyrZ = 0;

	imuNode_t *tempPtr = g_newIMUNode;
	imuNode_t newestData = {0};
	for (int i = 0; i < IMU_LIST_SIZE; i++) {
		while (tempPtr->lock) {
			retryTakeDelay(0);
		}

		tempPtr->lock = true;
		tempPtr->accX = newestData.accX;
		tempPtr->accY = newestData.accY;
		tempPtr->accZ = newestData.accZ;
		tempPtr->gyrX = newestData.gyrX;
		tempPtr->gyrY = newestData.gyrY;
		tempPtr->gyrZ = newestData.gyrZ;
		tempPtr->lock = false;

		avgAccX += newestData.accX / IMU_LIST_SIZE;
		avgAccY += newestData.accY / IMU_LIST_SIZE;
		avgAccZ += newestData.accZ / IMU_LIST_SIZE;
		avgGyrX += newestData.gyrX / IMU_LIST_SIZE;
		avgGyrY += newestData.gyrY / IMU_LIST_SIZE;
		avgGyrZ += newestData.gyrZ / IMU_LIST_SIZE;

		if (newestData.accX < minAccX) {
			minAccX = newestData.accX;
		}
		if (newestData.accX > maxAccX) {
			maxAccX = newestData.accX;
		}
		if (newestData.accY < minAccY) {
			minAccY = newestData.accY;
		}
		if (newestData.accY > maxAccY) {
			maxAccY = newestData.accY;
		}
		if (newestData.accZ < minAccZ) {
			minAccZ = newestData.accZ;
		}
		if (newestData.accZ > maxAccZ) {
			maxAccZ = newestData.accZ;
		}

		if (newestData.gyrX < minGyrX) {
			minGyrX = newestData.gyrX;
		}
		if (newestData.gyrX > maxGyrX) {
			maxGyrX = newestData.gyrX;
		}
		if (newestData.gyrY < minGyrY) {
			minGyrY = newestData.gyrY;
		}
		if (newestData.gyrY > maxGyrY) {
			maxGyrY = newestData.gyrY;
		}
		if (newestData.gyrZ < minGyrZ) {
			minGyrZ = newestData.gyrZ;
		}
		if (newestData.gyrZ > maxGyrZ) {
			maxGyrZ = newestData.gyrZ;
		}
		tempPtr = tempPtr->nextNode;
	}

	bool accSatisfied = ((100.0 * (maxAccX - minAccX) / (avgAccX)) <= ACCEPTABLE_PERCENT_ERROR) &&
						((100.0 * (maxAccY - minAccY) / (avgAccY)) <= ACCEPTABLE_PERCENT_ERROR) &&
						((100.0 * (maxAccZ - minAccZ) / (avgAccZ)) <= ACCEPTABLE_PERCENT_ERROR);
	bool gyrSatisfied = ((100.0 * (maxGyrX - minGyrX) / (avgGyrX)) <= ACCEPTABLE_PERCENT_ERROR) &&
						((100.0 * (maxGyrY - minGyrY) / (avgGyrY)) <= ACCEPTABLE_PERCENT_ERROR) &&
						((100.0 * (maxGyrZ - minGyrZ) / (avgGyrZ)) <= ACCEPTABLE_PERCENT_ERROR);

	return (accSatisfied && gyrSatisfied);
}
