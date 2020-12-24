/*
 * controlLogic.h
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#ifndef INC_CONTROLLOGIC_H_
#define INC_CONTROLLOGIC_H_

/* function prototypes */

void modeLogic();
void prelaunchLogic();
void launchLogic();
void ascentLogic();
void descentDrogueLogic();
void descentMainLogic();
void touchdownLogic();

bool determineMode();
bool checkPrelaunchTrans();
bool checkLaunchTrans();
bool checkAscentTrans();
bool checkDescentDrogueTrans();
bool checkDescentMainTrans();
bool checkProgramEnd();


#endif /* INC_CONTROLLOGIC_H_ */
