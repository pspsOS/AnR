/*
 * acquisition.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/acquisition.h"

/* Global variable declarations */


extern volatile daqStatusData_t g_daqStatusData = {0};
extern volatile gpsData_t g_gpsData = {0};
extern volatile bmpData_t g_bmpData = {0};
extern volatile imuData_t g_imuData = {0};
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

// Setup

/**
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

	// Initialize interface structs
	g_daqStatusData.lock = false;
	sendUpdate_A();
}


/* TODO: Implement gpsSetup
 * Established connection with GPS.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
/* TODO: Implement bmpSetup
 * Established connection with Barometric Pressure Sensor.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool gpsSetup_A() {
	return 0;
}


bool bmpSetup_A() {
	return 0;
}


/* TODO: Implement imuSetup
 * Established connection with IMU.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool imuSetup_A() {
	return 0;
}


/* TODO: Implement alaSetup
 * Established connection with Analog Linear Accelerometer.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool alaSetup_A() {
	return 0;
}

// Loop



/* TODO: Implement gpsRead
 * Read data from GPS.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
void gpsRead_A() {

	printf("Reading:");

// Parse each GPS packet type
	//	Type = GPGGA
	if (!(strncmp(&gpsNmea[0], "$GPGGA", 6))) {
		printf("GGA");

	}
	//	Type = GPRMC
	else if (!(strncmp(&gpsNmea[0], "$GPRMC", 6))) {
		//puts("RMC");
		printf("RMC");
	}

	//	Catch Bad Read
	else {
		gpsNominal = false;


	}
	assert(gpsNominal);
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
 * Takes raw NMEA string and replaces ',' with '\n', splitting each substring
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

        // Handle end of string condition
        /*if(gpsNmea[i] == 0) {
            _nmeaAddrEnd = i;
            break;
        }*/
    }
    /*
    printf("Start:%d\n", _nmeaAddrStart);
    printf("End:  %d\n", _nmeaAddrEnd);
    */
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
	printf("\n%s", gpsNmea);
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
	printf("Start: %d\n", _nmeaAddrStart);
	printf("End:   %d\n", _nmeaAddrEnd);
	printf("%s\n", &gpsNmea[_nmeaAddrStart]);
	return atof(&gpsNmea[_nmeaAddrStart]);
}
void __debug() {
	printf("%s",&gpsNmea[_nmeaAddrEnd + 1]);
}
