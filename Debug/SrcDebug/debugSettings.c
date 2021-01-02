/*
 * debugDataSources.c
 *
 *  Created on: Dec 31, 2020
 *      Author: Jeff
 */

#include <debugSettings.h>
#include <unistd.h>

// Local String Definitions
#define _GPS "GPS"
#define _BMP "BMP"
#define _IMU "IMU"
#define _ALA "ALA"

#define _TRUE "TRUE"
#define _FALSE "FALSE"


// General Settings
bool notifyGeneralSettings = true;

bool notifyWhenReadDisabled = true;
bool notifyWhenReadAborted = false;
bool assertFileNames = true;

// Sensor Settings
bool simulateGps = true;
char gpsFileName[] = "TestData/gpsTestData.txt";

bool simulateBmp = false;
char bmpFileName[] = "TestData/bmpTestData.txt";

bool simulateImu = false;
char imuFileName[] = "TestData/imuTestData.txt";

bool simulateAla = false;
char alaFileName[] = "TestData/alaTestData.txt";


/** notifyGeneralSettings_DS()
 * @brief Notify General Settings
 * Prints Debug General Settings to console
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 01/02/2021
 */
void notifyGeneralSettings_DS() {
	if(notifyGeneralSettings) {
		print("=================================\n");
		print("DEBUG SETTINGS\n");
		//print("=================================\n");
		prints("notifyWhenReadDisabled = %s\n", (notifyWhenReadDisabled) ? _TRUE : _FALSE);
		prints("notifyWhenReadAborted = %s\n", (notifyWhenReadAborted) ? _TRUE : _FALSE);
		prints("assertFileNames = %s\n", (assertFileNames) ? _TRUE : _FALSE);
		print("=================================\n\n");
	}
}


/** setupSensorFile_DS
 * Loads files for sensor simulation debugging
 *
 * @param sensor: Sensor Identification constant
 * @param *nominal: Pointer to acquisition nominal bool
 * @return FILE*: File pointer for data loading
 *
 * @author Jeff Kaji
 * @date 01/02/2021
 */
FILE *setupSensorFile_DS(ui8 sensor, bool *nominal) {
	bool simulate;
	char *fileName;
	char *strName;

	switch(sensor) {
	case GPS:
		simulate = simulateGps;
		fileName = gpsFileName;
		strName = _GPS;
		break;
	case BMP:
		simulate = simulateBmp;
		fileName = bmpFileName;
		strName = _BMP;
		break;
	case IMU:
		simulate = simulateImu;
		fileName = imuFileName;
		strName = _IMU;
		break;
	case ALA:
		simulate = simulateAla;
		fileName = alaFileName;
		strName = _ALA;
	}


	if(!simulate) {
		if(notifyWhenReadDisabled)
			prints("%s simulation disabled\n", strName);
		*nominal = false;
		return NULL;
	}
	if( access(gpsFileName, F_OK ) == 0 ) {
		prints("%s File: \"%s\"\n", strName, fileName);
		*nominal = true;
		return fopen(fileName, "r");

	} else {
		printe("%s File: \"%s\" NOT FOUND\n", strName, fileName);
		*nominal = false;
		if(assertFileNames) assert(!assertFileNames);
		return NULL;
	}
}
