/*
 * acquisition.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/acquisition.h"

#ifndef NDEBUG
#include <unistd.h>
#include <debugSettings.h>
#endif

/* Global variable declarations */


/* Local variable declarations */

char gpsNmea[MAX_NMEA];
ui8 daqScaling;
ui8 daqScaler;
ui8 gpsNominal;
ui8 bmpNominal;
ui8 imuNominal;
ui8 alaNominal;
ui8 sendDaqStatus;
ui8 fsmCounter0;
ui8 fsmCounter1;
ui8 fsmState;
ui8 hasUpdate;

// Used for parsing NMEA data
ui8 _nmeaAddrStart;
ui8 _nmeaAddrEnd;

// File load buffer
#ifndef NDEBUG
	FILE *_gpsFile;
	FILE *_bmpFile;
	FILE *_imuFile;
	char _buff[255];
#endif


// Setup

/** setup_A()
 * @brief Setup Acquisition Task
 * Initialized local variables and runs setup functions
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/28/2020
 */
void setup_A() {

	// Initialize local variables
	daqScaling = false;
	daqScaler = 10;
	gpsNominal = false;
	bmpNominal = false;
	imuNominal = false;
	alaNominal = false;
	sendDaqStatus = false;
	fsmCounter0 = 0;
	fsmCounter1 = 0;
	fsmState = 0;
	hasUpdate = 0;


	// Setup sensors
	gpsSetup_A();
	bmpSetup_A();
	imuSetup_A();
	alaSetup_A();

	// Initialize interface structs
	g_daqStatusData.lock = false;
	sendUpdate_A();
}



/** gpsSetup_A()
 * @brief Setup GPS Sensor
 * Establishes connection with GPS
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/31/2020
 */
void gpsSetup_A() {
	#ifndef NDEBUG
		if(!simulateGps) {
			print("GPS simulation disabled\n");
			gpsNominal = false;
			return;
		}
		if( access(gpsFileName, F_OK ) == 0 ) {
			prints("GPS File: \"%s\"\n", gpsFileName);
			gpsNominal = true;
			_gpsFile = fopen(gpsFileName, "r");

		} else {
			printe("GPS File: \"%s\" NOT FOUND\n", gpsFileName);
			gpsNominal = false;
		}

	#else
		// TODO: Implement gpsSetup
	#endif
}



/** bmpSetup_A()
 * @brief Setup BMP Sensor
 * Establishes connection with BMP
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/31/2020
 */
void bmpSetup_A() {
	#ifndef NDEBUG
		if(!simulateBmp) {
			print("BMP simulation disabled\n");
			bmpNominal = false;
			return;
		}
		if( access(bmpFileName, F_OK ) == 0 ) {
			prints("BMP File: \"%s\"\n", bmpFileName);
			bmpNominal = true;
			_bmpFile = fopen(bmpFileName, "r");

		} else {
			printe("BMP File: \"%s\" NOT FOUND\n", bmpFileName);
			bmpNominal = false;
		}

	#else
		// TODO: Implement bmpSetup
	#endif
}



/** imuSetup_A()
 * @brief Setup IMU Sensor
 * Establishes connection with IMU
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/31/2020
 */
void imuSetup_A() {
	#ifndef NDEBUG
		if(!simulateImu) {
			print("IMU simulation disabled\n");
			imuNominal = false;
			return;
		}
		if( access(imuFileName, F_OK ) == 0 ) {
			prints("IMU File: \"%s\"\n", imuFileName);
			imuNominal = true;
			_imuFile = fopen(imuFileName, "r");

		} else {
			printe("IMU File: \"%s\" NOT FOUND\n", imuFileName);
			imuNominal = false;
		}

	#else
		// TODO: Implement imuSetup
	#endif
}



/** alaSetup_A()
 * @brief Setup ALA Sensor
 * Establishes connection with ALA
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/31/2020
 */
void alaSetup_A() {
	#ifndef NDEBUG
		alaNominal = true;
	#else
		// TODO: Implement alaSetup
	#endif
}

// Loop



/*
 * Read data from GPS.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
void gpsRead_A() {

	#ifndef NDEBUG
		if(!simulateGps) {
			prints("%s\n", "GPS read aborted.");
			return;
		}
		fscanf(_gpsFile, "%s", gpsNmea);
	#else
		TODO: Implement gpsRead w/ hardware
	#endif

	print("Reading:",);

// Parse each GPS packet type
	//	Type = GPGGA
	if (!(strncmp(&gpsNmea[0], "$GPGGA", 6))) {
		print("GGA\n");

	}
	//	Type = GPRMC
	else if (!(strncmp(&gpsNmea[0], "$GPRMC", 6))) {
		//puts("RMC");
		print("RMC\n");
	}

	//	Catch Bad Read
	else {
		gpsNominal = false;


	}

	g_gpsData.timeStamp = getTimeStamp();

	strncpy(g_gpsData.NMEA, gpsNmea, strlen(gpsNmea));

//	return;
}



/* TODO: Implement bmpRead
 * Read data from BMP
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
/* TODO: Implement imuRead
 * Read data from IMU and ALA
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
void bmpRead_A() {

}


void imuRead_A() {

}

/* TODO: Implement checkStatus
 * Checks status of acquisition tasks including:
 *   - Whether sensors are still nominal
 *   - Whether or not to enable/disable daqScaling
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
void checkStatus_A() {

}

/**
 * @brief Send Update
 * Updates the daqStatusData interface struct.
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/31/2020
 */
void sendUpdate_A() {
	while(g_daqStatusData.lock) {
		retryTakeDelay(0);
	}
	g_daqStatusData.lock = true;
	g_daqStatusData.timeStamp = getTimeStamp();
	g_daqStatusData.daqScaling = daqScaling;
	g_daqStatusData.gpsNominal = gpsNominal;
	g_daqStatusData.bmpNominal = bmpNominal;
	g_daqStatusData.imuNominal = imuNominal;
	g_daqStatusData.alaNominal = alaNominal;
	g_daqStatusData.hasUpdate = true;
	g_daqStatusData.lock = false;
}

/**
 * @brief Split NMEA String
 * Takes raw NMEA string and replaces ',' with 0, splitting each substring
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/26/2020
 */
void _splitNmea() {
	for(int i = 0; i < 80; i++) {
		if(gpsNmea[i] == ',')
			gpsNmea[i] = 0;
		else if(gpsNmea[i] == 0)
			return;
	}
}

/**
 * @brief NMEA Address Identification
 * Finds the start and end addresses of a given field within split string gpsNmea.
 *   Stores start address in _nmeaAddrStart
 *   Stores end address in _nmeaAddrEnd
 *
 * @param addr: Address to locate
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/25/2020
 */
void _findNmeaAddr(int addr) {
    for(int i = 0; i < 80; i++) {

    	// Find instance of ','
        if(gpsNmea[i] == 0) {
            addr -= 1;

            // Set end address
            if(addr == -1) {
                _nmeaAddrEnd = i;
                break;
            }

            // Set start address
            _nmeaAddrStart = i+1;
        }
    }
}

// Test Functions

/**
 * @brief Set Local NMEA String
 * Sets the local string gpsNmea[80] to the input string.
 * As of now, is only intended for use in the testbed.
 *
 * @param nmea: New value for local string
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/26/2020
 */
void __setNmea(char *nmea) {
	strncpy(gpsNmea, nmea, strlen(nmea));
}

/**
 * @brief Print NMEA string
 * Prints NMEA string to console. Used for debugging.
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/26/2020
 */
void __printNmea() {
	print("\n%s", gpsNmea);
}

/**
 * @brief Get float from NMEA
 * Returns a specific float from GPS string. Used for debugging.
 * @param addr: Address within the NMEA string
 * @return None
 *
 * @author Jeff Kaji
 * @date 12/27/2020
 */
float __getFloat(int addr) {
	_findNmeaAddr(addr);
	print("Start: %d\n", _nmeaAddrStart);
	print("End:   %d\n", _nmeaAddrEnd);
	print("%s\n", &gpsNmea[_nmeaAddrStart]);
	return atof(&gpsNmea[_nmeaAddrStart]);
}
void __debug() {
	print("%s",&gpsNmea[_nmeaAddrEnd + 1]);
}
