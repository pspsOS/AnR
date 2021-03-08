/*
 * controlLogic.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/controlLogic.h"

#define REQUIRED_TIME_FROM_STARTUP (1200000)
#define ACCEPTABLE_PERCENT_ERROR (5)
#define LAUNCH_FORCE_DETECT_FACTOR (3)
#define ROCKET_HEIGHT (5.4)
#define LAUNCH_ALTITUDE_DIFFERENCE_DETECT (ROCKET_HEIGHT * 3)



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
	if (g_staticOrientationArray[newestStaticOrientationIndex].numNotOptimal == 0) {
		orientationSatisfied = true;
	}

	bool stillnessSatisfied = false;
	insertNewIMUNode();
	stillnessSatisfied = determineStillness_C();

	if (timeSatisfied && orientationSatisfied && stillnessSatisfied) {
		//TODO: Send to Transmission & Storage

		//TODO: Store stopRewrite Address Marker

		//TODO: Set hotBoot to true

		g_chuteDeployable = true;
		g_daqStatusData.daqScaling = true;
		g_daqStatusData.hasUpdate = true;

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
	bool sustainedZForce = false;
	sustainedZForce = determineSustainedZForce_C();

	bool detectedAscent = false;
	detectedAscent = determineAscent_C();

	if (sustainedZForce && detectedAscent) {
		//TODO: Send to Transmission & Storage


		g_launchTime = getTimeStamp();

		return true;
	}
	else {
		return false;
	}
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
	//TODO: Change min and max to int16_t data types
	float minAccX = FLT_MAX;
	float maxAccX = FLT_MIN;
	float avgAccX = 0;
	float minAccY = FLT_MAX;
	float maxAccY = FLT_MIN;
	float avgAccY = 0;
	float minAccZ = FLT_MAX;
	float maxAccZ = FLT_MIN;
	float avgAccZ = 0;
	float minAlaZ = FLT_MAX;
	float maxAlaZ = FLT_MIN;
	float avgAlaZ = 0;

	float minGyrX = FLT_MAX;
	float maxGyrX = FLT_MIN;
	float avgGyrX = 0;
	float minGyrY = FLT_MAX;
	float maxGyrY = FLT_MIN;
	float avgGyrY = 0;
	float minGyrZ = FLT_MAX;
	float maxGyrZ = FLT_MIN;
	float avgGyrZ = 0;


	imuNode_t newestData = {0};
	for (int i = 0; i < IMU_ARRAY_SIZE; i++) {
		int currentIndex = (i + newestImuIndex) % IMU_ARRAY_SIZE;
		while (g_imuArray[currentIndex].lock) {
			retryTakeDelay(DEFAULT_TAKE_DELAY);
		}

		g_imuArray[currentIndex].lock = true;
		newestData.accX = g_imuArray[currentIndex].accX;
		newestData.accY = g_imuArray[currentIndex].accY;
		newestData.accZ = g_imuArray[currentIndex].accZ;
		newestData.alaZ = g_imuArray[currentIndex].alaZ;
		newestData.gyrX = g_imuArray[currentIndex].gyrX;
		newestData.gyrY = g_imuArray[currentIndex].gyrY;
		newestData.gyrZ = g_imuArray[currentIndex].gyrZ;
		g_imuArray[currentIndex].lock = false;

		avgAccX += newestData.accX / IMU_ARRAY_SIZE;
		avgAccY += newestData.accY / IMU_ARRAY_SIZE;
		avgAccZ += newestData.accZ / IMU_ARRAY_SIZE;
		avgAlaZ += newestData.alaZ / IMU_ARRAY_SIZE;
		avgGyrX += newestData.gyrX / IMU_ARRAY_SIZE;
		avgGyrY += newestData.gyrY / IMU_ARRAY_SIZE;
		avgGyrZ += newestData.gyrZ / IMU_ARRAY_SIZE;

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
		if (newestData.alaZ < minAlaZ) {
			minAlaZ = newestData.alaZ;
		}
		if (newestData.alaZ > maxAlaZ) {
			maxAlaZ = newestData.alaZ;
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
	}

	bool accSatisfied = ((100.0 * (maxAccX - minAccX) / (avgAccX)) <= ACCEPTABLE_PERCENT_ERROR) &&
						((100.0 * (maxAccY - minAccY) / (avgAccY)) <= ACCEPTABLE_PERCENT_ERROR) &&
						((100.0 * (maxAccZ - minAccZ) / (avgAccZ)) <= ACCEPTABLE_PERCENT_ERROR) &&
						((100.0 * (maxAlaZ - minAlaZ) / (avgAlaZ)) <= ACCEPTABLE_PERCENT_ERROR);
	bool gyrSatisfied = ((100.0 * (maxGyrX - minGyrX) / (avgGyrX)) <= ACCEPTABLE_PERCENT_ERROR) &&
						((100.0 * (maxGyrY - minGyrY) / (avgGyrY)) <= ACCEPTABLE_PERCENT_ERROR) &&
						((100.0 * (maxGyrZ - minGyrZ) / (avgGyrZ)) <= ACCEPTABLE_PERCENT_ERROR);

	return (accSatisfied && gyrSatisfied);
}


bool determineSustainedZForce_C() {
	if (g_alaArray[newestAlaIndex].runningForce >= abs(LAUNCH_FORCE_DETECT_FACTOR *
			g_alaArray[(newestAlaIndex + 1) % ALA_ARRAY_SIZE].runningForce)) {
		return true;
	}
	else {
		return false;
	}
}

bool determineAscent_C() {
	if (g_altitudeArray[newestAltitudeIndex].runningAltitude >= (LAUNCH_ALTITUDE_DIFFERENCE_DETECT +
			g_altitudeArray[(newestAltitudeIndex + 1) % ALTITUDE_ARRAY_SIZE].runningAltitude)) {
		return true;
	}

	return false;
}
