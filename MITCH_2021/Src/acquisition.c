/*
 * acquisition.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include <string.h>
#include "acquisition.h"

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

}


bool bmpSetup() {

}


/* TODO: Implement imuSetup
 * Established connection with IMU.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool imuSetup() {

}


/* TODO: Implement alaSetup
 * Established connection with Analog Linear Accelerometer.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool alaSetup() {

}

// Loop



/* TODO: Implement gpsRead
 * Read data from GPS.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
gpsData_t gpsRead() {

// Parse each GPS packet type
	//	Type = GPGGA
	if (!(strncmp(gpsNmea[0], "$GPGGA,", 7))) {

	}

	//	Type = GPRMC
	else if (!(strncmp(gpsNmea[0], "$GPRMC,", 7))) {

	}

	//	Catch Bad Read
	else {
		gpsNominal = FALSE;
		return NULL;
	}

	g_gpsData.timeStamp = getTimeStamp();
	strncpy(gpsData.Nmea, gpsNmea, strlen(gpsNmea));

	return;
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
bmpData_t bmpRead() {

}


imuData_t imuRead() {

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


/* TODO: Document _findNmeaAddr
 *  Finds the start and end addresses of a given field
 *  within the comma separated string gpsNmea.
 *  Stores start address in _nmeaAddrStart
 *  Stores end address in _nmeaAddrEnd
 *
 *  Author: Jeff Kaji
 *  Date: 12/25/2020
 */
void _findNmeaAddr(int addr) {
    for(int i = 0; i < 80; i++) {

    	// Find instance of ','
        if(gpsNMEA[i] == ',') {
            addr -= 1;

            // Set end address
            if(addr == -1) {
                _nmeaAddrEnd = i;
                return;
            }

            // Set start address
            _nmeaAddrStart = i+1;
        }

        // Handle end of string condition
        if(gpsNMEA[i] == 0) {
            _nmeaAddrEnd = i;
            return;
        }
    }
}

