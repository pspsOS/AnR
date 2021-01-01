/*
 * DebugDataSources.h
 *
 *  Created on: Dec 31, 2020
 *      Author: Jeff
 */

#ifndef DEBUGSETTINGS_H_
#define DEBUGSETTINGS_H_

#include "common.h"

// General Settings
extern bool notifyWhenReadDisabled;
extern bool assertFileNames;

// Sensor Settings
extern bool simulateGps;
extern char gpsFileName[];

extern bool simulateBmp;
extern char bmpFileName[];

extern bool simulateImu;
extern char imuFileName[];

#endif /* DEBUGSETTINGS_H_ */
