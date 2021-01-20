/*
 * DebugDataSources.h
 *
 *  Created on: Dec 31, 2020
 *      Author: Jeff
 */

#ifndef DEBUGSETTINGS_H_
#define DEBUGSETTINGS_H_

#include "common.h"

// SENSOR CONSTANTS
#define GPS 0
#define BMP 1
#define IMU 2
#define ALA 3

// Sensor sensitivities 
#define ACCEL_SENSITIVITY (16) // g
#define GYRO_SENSITIVITY (2000) // dps
#define MAG_SENSITIVITY (4900) // uT

// General Settings
extern bool notifyGeneralSettings;

extern bool notifyWhenDisabled;
extern bool notifyWhenReadAborted;
extern bool assertFileNames;

// Acquisition Settings
extern bool simulateGps;
extern char gpsFileName[];

extern bool simulateBmp;
extern char bmpFileName[];

extern bool simulateImu;
extern char imuFileName[];


// Monitoring Settings
extern bool simulateMonitoring;
extern char monitoringFileName[];


// General Functions
void notifyGeneralSettings_DS();


// Acquisition Functions
FILE *setupSensorFile_DS(ui8 sensor, bool *nominal);


// Monitoring Functions
FILE *setupMonitoringFile_DS();

#endif /* DEBUGSETTINGS_H_ */
