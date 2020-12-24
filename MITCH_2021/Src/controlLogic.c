/*
 * controlLogic.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "controlLogic.h"


/* TODO: Implement modeLogic
 * Encapsulates functions that handle logic for different modes.
 * Will call other logic functions depending on current mode
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

void modeLogic() {

}


/* TODO: Implement prelaunchLogic
 * Called by modeLogic() function, and contains logic for prelaunch mode
 * Plays buzzer to indicate system is active
 * Checks if disarm button is pressed to change global disarm status
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

void prelaunchLogic() {

}


/* TODO: Implement launchLogic
 * Called by modeLogic() function, and contains logic for launch mode
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

void launchLogic() {

}


/* TODO: Implement ascentLogic
 * Called by modeLogic() function, and contains logic for ascent mode
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */


void ascentLogic() {

}


/* TODO: Implement descentDrogueLogic
 * Called by modeLogic() function, and contains logic for descent drogue mode
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */


void descentDrogueLogic() {

}


/* TODO: Implement descentMainLogic
 * Called by modeLogic() function, and contains logic for descent main mode
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */


void descentMainLogic() {

}


/* TODO: Implement touchdownLogic
 * Called by modeLogic() function, and contains logic for touchdown mode
 * Activates recovery buzzer
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */


void touchdownLogic() {

}


/* TODO: Implement Determine Mode
 * Uses raw and processed data to determine whether or not
 * to change modes
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool determineMode() {

}


/* TODO: Implement checkPrelaunchTrans
 * Determines if conditions have been met to move from prelaunch to launch mode
 * Conditions: Corrent orientation and stillness for certain period of time
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool checkPrelaunchTrans() {

}


/* TODO: Implement checkLaunchTrans
 * Determines if conditions have been met to move from launch to ascent mode
 * Conditions: Sustained Z+ Gs and BMP detecting ascent
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool checkLaunchTrans() {

}


/* TODO: Implement checkAscentTrans
 * Determines if conditions have been met to move from ascent to descent drogue mode
 * Conditions: Detects apogee (loss of alt)
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool checkAscentTrans() {

}


/* TODO: Implement checkDescentDrogueTrans
 * Determines if conditions have been met to move from descent drogue to descent main mode
 * Conditions: Main chute alt reached
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool checkDescentDrogueTrans() {

}


/* TODO: Implement checkPrelaunchTrans
 * Determines if conditions have been met to move from descent main to touchdown mode
 * Conditions: Constant altitude and BLTN is still
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool checkDescentMainTrans() {

}


/* TODO: Implement checkPrelaunchTrans
 * Determines if conditions have been met to move from touchdown mode to program end
 * Conditions: ???
 *
 * Author: Vishnu Vijay
 * Date: 12/23/20
 */

bool checkProgramEnd() {

}
