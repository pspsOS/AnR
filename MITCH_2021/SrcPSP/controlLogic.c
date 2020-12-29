/*
 * controlLogic.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/controlLogic.h"

/* extern definitions */

extern volatile ui8 currentNominalMode = 0;
extern volatile ui8 currentContingency = 0;


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
	printf("I'm here\n");
	return;
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


/* TODO: Implement checkPrelaunchTrans
 * Determines if conditions have been met to move from touchdown mode to program end
 * Conditions: ???
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */
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
