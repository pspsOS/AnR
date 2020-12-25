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
bmpData_t getbmpData();

gpsData_t gpsRead();
bmpData_t bmpRead();
imuData_t imuRead();

void checkStatus();


// Private
void _findNmeaAddr(int addr);

#endif /* INC_ACQUISITION_H_ */
