/*
 * controlLogic.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/controlLogic.h"
#include <stdio.h>


/* TODO: Implement modeLogic
 * Encapsulates functions that handle logic for different modes.
 * Will call other logic functions depending on current mode
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

void modeLogic_C() {
	printf("I'm here\n");
	return;
}


/* TODO: Implement prelaunchLogic
 * Called by modeLogic() function, and contains logic for prelaunch mode
 * Plays buzzer to indicate system is active
 * Checks if disarm button is pressed to change global disarm status
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

void prelaunchLogic_C() {

}


/* TODO: Implement launchLogic
 * Called by modeLogic() function, and contains logic for launch mode
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

void launchLogic_C() {

}


/* TODO: Implement ascentLogic
 * Called by modeLogic() function, and contains logic for ascent mode
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */


void ascentLogic_C() {

}


/* TODO: Implement descentDrogueLogic
 * Called by modeLogic() function, and contains logic for descent drogue mode
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */


void descentDrogueLogic_C() {

}


/* TODO: Implement descentMainLogic
 * Called by modeLogic() function, and contains logic for descent main mode
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */


void descentMainLogic_C() {

}


/* TODO: Implement touchdownLogic
 * Called by modeLogic() function, and contains logic for touchdown mode
 * Activates recovery buzzer
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */


void touchdownLogic_C() {

}


/* TODO: Implement Determine Mode
 * Uses raw and processed data to determine whether or not
 * to change modes
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool determineMode_C() {

}


/* TODO: Implement checkPrelaunchTrans
 * Determines if conditions have been met to move from prelaunch to launch mode
 * Conditions: Corrent orientation and stillness for certain period of time
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool checkPrelaunchTrans_C() {
 return 0;
}


/* TODO: Implement checkLaunchTrans
 * Determines if conditions have been met to move from launch to ascent mode
 * Conditions: Sustained Z+ Gs and BMP detecting ascent
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool checkLaunchTrans_C() {
	return 0;
}


/* TODO: Implement checkAscentTrans
 * Determines if conditions have been met to move from ascent to descent drogue mode
 * Conditions: Detects apogee (loss of alt)
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool checkAscentTrans_C() {
	return 0;
}


/* TODO: Implement checkDescentDrogueTrans
 * Determines if conditions have been met to move from descent drogue to descent main mode
 * Conditions: Main chute alt reached
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool checkDescentDrogueTrans_C() {
	return 0;
}


/* TODO: Implement checkPrelaunchTrans
 * Determines if conditions have been met to move from descent main to touchdown mode
 * Conditions: Constant altitude and BLTN is still
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
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

bool checkProgramEnd_C() {
	return 0;
}
