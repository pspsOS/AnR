/*
 * acquisition.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/acquisition.h"

#include <string.h>

#include "../IncPSP/common.h"

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

// Parse each GPS packet type
	//	Type = GPGGA
	/*
	if (!(strncmp(gpsNmea[0], "$GPGGA,", 7))) {

	}

	//	Type = GPRMC
	else if (!(strncmp(gpsNmea[0], "$GPRMC,", 7))) {

	}

	//	Catch Bad Read
	else {
		gpsNominal = FALSE;

	}
	*/
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
			gpsNmea[i] = '\n';
		if(gpsNmea[i] == 0)
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
        if(gpsNmea[i] == '\n') {
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
        if(gpsNmea[i] == 0) {
            _nmeaAddrEnd = i;
            return;
        }
    }
}

