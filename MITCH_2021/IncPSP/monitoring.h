/*
 * monitoring.h
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#ifndef INC_MONITORING_H_
#define INC_MONITORING_H_

#include "common.h"

/* function prototypes */

void setup_M();
void loop_M();
void checkBatteryVoltage();
void checkContinuity();
void checkButtonState();
void sendUpdate_M();

#endif /* INC_MONITORING_H_ */
