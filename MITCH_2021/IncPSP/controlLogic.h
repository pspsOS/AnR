/*
 * controlLogic.h
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#ifndef INC_CONTROLLOGIC_H_
#define INC_CONTROLLOGIC_H_

#include "../IncPSP/common.h"

/* function prototypes */

void modeLogic_C();
void prelaunchLogic_C();
void launchLogic_C();
void ascentLogic_C();
void descentDrogueLogic_C();
void descentMainLogic_C();
void touchdownLogic_C();

bool determineMode_C();
bool checkPrelaunchTrans_C();
bool checkLaunchTrans_C();
bool checkAscentTrans_C();
bool checkDescentDrogueTrans_C();
bool checkDescentMainTrans_C();
bool checkProgramEnd_C();


#endif /* INC_CONTROLLOGIC_H_ */
