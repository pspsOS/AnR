/*
 * debugDataSources.c
 *
 *  Created on: Dec 31, 2020
 *      Author: Jeff
 */

#include <debugSettings.h>
#include <unistd.h>




// General Settings
bool notifyGeneralSettings = true;

bool notifyWhenDisabled = false;
bool notifyWhenReadAborted = false;
bool assertFileNames = true;

// Acquisition Settings
bool simulateGps = true;
char gpsFileName[] = "TestData/Gps_Real_RMCGGA_DATA.txt";

bool simulateBmp = true;
char bmpFileName[] = "TestData/bmpTestData.txt";

bool simulateImu = true;
char imuFileName[] = "TestData/imuTestData.txt";

bool simulateAla = false;
char alaFileName[] = "TestData/alaTestData.txt";

// Monitoring Settings
bool simulateMonitoring = true;
char monitoringFileName[] = "TestData/monitoringTestData.txt";

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
		printf("=================================\n");
		printf("DEBUG SETTINGS\n");
		//printf("=================================\n");
		printf("notifyWhenDisabled = %s\n", (notifyWhenDisabled) ? _TRUE : _FALSE);
		printf("notifyWhenReadAborted = %s\n", (notifyWhenReadAborted) ? _TRUE : _FALSE);
		printf("assertFileNames = %s\n", (assertFileNames) ? _TRUE : _FALSE);
		printf("=================================\n\n");
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
		if(notifyWhenDisabled)
			printf("%s simulation disabled\n", strName);
		*nominal = false;
		return NULL;
	}
	if( access(gpsFileName, F_OK ) == 0 ) {
		printf("%s File: \"%s\"\n", strName, fileName);
		*nominal = true;
		return fopen(fileName, "r");

	} else {
		printf("%s File: \"%s\" NOT FOUND\n", strName, fileName);
		*nominal = false;
		if(assertFileNames) assert(!assertFileNames);
		return NULL;
	}
}

/**
 * @brief setup monitoring simulation
 * check if simulating then access monitoringTestData.txt
 *
 * @param None
 * @retval None
 *
 * @author Mark Paral
 * @date 1/3/2021
 */
FILE *setupMonitoringFile_DS() {
	if (!simulateMonitoring) {
		if (notifyWhenDisabled) {
			printf("Monitoring simulation disabled\n");
		}
		return NULL;
	}
	if( access(monitoringFileName, F_OK) == 0 ) {
		printf("Monitoring File: \"%s\"\n", monitoringFileName);
		return fopen(monitoringFileName, "r");

	} else {
		printf("Monitoring File: \"%s\" NOT FOUND\n", monitoringFileName);
		if(assertFileNames) assert(!assertFileNames);
		return NULL;
	}
}
