/*
 * acquisition.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../IncPSP/acquisition.h"

gpsData_t g_gpsData;

char gpsNmea[MAX_NMEA];
byte daqScaling;
byte daqScaler;
byte gpsNominal;
byte bmpNominal;
byte imuNominal;
byte alaNominal;
byte sendDaqStatus;
byte i;
byte j;
byte hasUpdate;

// Used for parsing NMEA data
byte _nmeaAddrStart;
byte _nmeaAddrEnd;

// Setup
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
bool gpsSetup() {
	return 0;
}


bool bmpSetup() {
	return 0;
}


/* TODO: Implement imuSetup
 * Established connection with IMU.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool imuSetup() {
	return 0;
}


/* TODO: Implement alaSetup
 * Established connection with Analog Linear Accelerometer.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool alaSetup() {
	return 0;
}

// Loop



/* TODO: Implement gpsRead
 * Read data from GPS.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
void gpsRead() {

	printf("Reading:");

// Parse each GPS packet type
	//	Type = GPGGA
	if (!(strncmp(&gpsNmea[0], "$GPGGA", 6))) {
		printf("GGA");

		// Get Latitude
		_findNmeaAddr(2);
		//g_gpsData.lat = (int)(10000*atof(&gpsNmea[_nmeaAddrStart]));
		g_gpsData.lat = atof(&gpsNmea[_nmeaAddrStart]);
		if (gpsNmea[_nmeaAddrEnd + 1] == 'N');
		else if (gpsNmea[_nmeaAddrEnd + 1] == 'S')
			g_gpsData.lat *= -1;
		else {
			gpsNominal = FALSE;
			return;
		}

		// Get Longitude
		_findNmeaAddr(4);
		g_gpsData.lon = atof(&gpsNmea[_nmeaAddrStart]);
		if (gpsNmea[_nmeaAddrEnd + 1] == 'E');
		else if (gpsNmea[_nmeaAddrEnd + 1] == 'W')
			g_gpsData.lon *= -1;
		else {
			gpsNominal = FALSE;
			return;
		}

		printf("\n%.4f\n",g_gpsData.lat);
	}

	//	Type = GPRMC
	else if (!(strncmp(&gpsNmea[0], "$GPRMC", 6))) {
		//puts("RMC");
		printf("RMC");
	}

	//	Catch Bad Read
	else {
		gpsNominal = FALSE;

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
void bmpRead() {

}


void imuRead() {

}

/* TODO: Implement checkStatus
 * Checks status of acquisition tasks including:
 *   - Whether sensors are still nominal
 *   - Whether or not to enable/disable daqScaling
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
void checkStatus() {

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
