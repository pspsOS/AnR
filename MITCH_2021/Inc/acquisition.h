/*
 * acquisition.h
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#ifndef INC_ACQUISITION_H_
#define INC_ACQUISITION_H_

#include "common.h"

// Setup
bool gpsSetup();
bool bmpSetup();
bool imuSetup();
bool alaSetup();

// Loop
gpsData_t getGpsData();

gpsData_t gpsRead();
bmpData_t bmpRead();
imuData_t imuRead();

void checkStatus();

#endif /* INC_ACQUISITION_H_ */
