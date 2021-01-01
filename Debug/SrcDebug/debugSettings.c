/*
 * debugDataSources.c
 *
 *  Created on: Dec 31, 2020
 *      Author: Jeff
 */

#include <debugSettings.h>

// General Settings
bool notifyWhenReadDisabled = true;
bool assertFileNames = true;

// Sensor Settings
bool simulateGps = true;
char gpsFileName[] = "TestData/gpsTestData.txt";

bool simulateBmp = false;
char bmpFileName[] = "TestData/bmpTestData.txt";

bool simulateImu = false;
char imuFileName[] = "TestData/imuTestData.txt";

