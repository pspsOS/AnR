/*
 * controlLogic.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/controlLogic.h"



void loop_C() {
	modeLogic_C();

	bool modeChange = determineMode_C();

	if (modeChange) {
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

	return 0;
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
